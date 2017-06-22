//
// Created by Delano on 20/06/2017.
//

#include <unistd.h>
#include "challenge3.h"
#include "challenge12.h"
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include <time.h>

// make mines accessible for both calls to challenge3()
Position *mines = NULL;
void challenge3(int run2) {
    // define variables
    char pos_str[8], input, name[6];
    clock_t last_time, current_time;
    double dt;
    Position *start = NULL, *list, *last, *at;
    int n = 0, i, j, k, nMines = 0;
    Position *temp;
    last_time = clock();

    // clear
    system("@cls||clear");

    // Build the matrix
    if (!run2) constructMatrix();

    start = findByName("1");

    // Set all edges as checkpoints
    name[0] = 'e';
    name[5] = '\0';
    for (i = 0; i <= 4; i++) {
        if (i % 2) {
            for (j = 0; j <= 4; j++ ) {
                if (i < 4) {
                    name[2] = i + 48;
                    name[1] = j + 48;
                    name[4] = i + 48 + 1;
                    name[3] = j + 48;
                    temp = findByName(name);
                    if (run2 && containsPosition(mines, temp))
                        printf("Mine @(%d, %d)", temp->x, temp->y);
                    else appendPosition(start, temp);
                }
                if (j < 4)  {
                    name[2] = i + 48;
                    name[1] = j + 48;
                    name[4] = i + 48;
                    name[3] = j + 48 + 1;
                    temp = findByName(name);
                    if (run2 && containsPosition(mines, temp))
                        printf("Mine @(%d, %d)", temp->x, temp->y);
                    else appendPosition(start, temp);
                }
            }
        } else {
            for (j = 4; j >= 0; j-- ) {
                if (i < 4) {
                    name[2] = i + 48;
                    name[1] = j + 48;
                    name[4] = i + 48 + 1;
                    name[3] = j + 48;
                    temp = findByName(name);
                    if (run2 && containsPosition(mines, temp))
                        printf("Mine @(%d, %d)", temp->x, temp->y);
                    else appendPosition(start, temp);
                }
                if (j > 0)  {
                    name[4] = i + 48;
                    name[3] = j + 48;
                    name[2] = i + 48;
                    name[1] = j + 48 - 1;
                    temp = findByName(name);
                    if (run2 && containsPosition(mines, temp))
                        printf("Mine @(%d, %d)", temp->x, temp->y);
                    else appendPosition(start, temp);
                }
            }
        }
    }

    // Get route
    if (!start->next) {
        return;
    }

    list = getRoute(start, start->next);

    temp = start->next;
    while (temp->next) {
        joinRoutes(list, getRoute(temp, temp->next));

        temp = temp->next;
    }

    at = start;
    last = list;

    printf("START\n");
    // Tell the robot to start moving
    writeByte(&START_SIGNAL);
    getDirection(list);

    list = list->next; // set correct start for robot logic(as in: next crossing is the corner after the number)
    printf("Now at (%d, %d)\n", list->x, list->y);
    // current status
    printMatrix(list, start);
    i = 0;

    // while we still have points left
    while (list->next) {
        // If the input is different from last time
        char x = input;
        while (x == input) {
            readByte(&input);
        }

        system("@cls||clear");
        printf("Recieved: %c [%s]\n", input, charToBinary(input));

        // get time since last input
        current_time = clock();
        dt = (double) (current_time - last_time) / CLOCKS_PER_SEC;

        printf("dt: %f\n", dt);

        // debug quit
        if (input == 'q') break;
            // crossing
        else if(input == 0b01111000 || input == 0b01111001) {  // 'x' or 'y'
            printf("n: %d\n", ++i); // debug statement

            // If not the end of the list
            if (list->next && list->next->next) {

                // in case still on a crossing, sleep for some time
                usleep(300 * 1000); // warning: depreciated, but still works so fingers crossed
                // send new direction
                getDirection(list);

                // in case something went wrong, do not allow more then two crossings in 0.5 seconds
                if (dt >= 0.5) {
                    last = list;
                    list = list->next;
                }

                // check if we are arriving at a checkpoint
                if (at->next && list->x == at->next->x && list->y == at->next->y) {
                    at = at->next;
                    printf("Arriving at %s\n", matrix[at->x][at->y].name);
                }

                // Check for any other checkpoints we may have moved over
                temp = at;

                while(temp->next && temp->next->next) {
                    temp = temp->next;
                    if (list->x == temp->next->x &&
                        list->y == temp->next->y
                            ) {
                        temp->next = temp->next->next;
                        printf("Arriving at unexpected %s\n", matrix[at->x][at->y].name);
                    }
                }
            } else {
                break;
            }

        } else if(input == 0b01101100 || input == 0b01101101){ // 'm' or 'n'
            // set cell as mine
            matrix[list->x][list->y].value = -10;

            if (run2) {
                // check if it is the missing mine
                if (!containsPosition(mines, createPosition(list->x, list->y))) {
                    writeByte(&STOP_STATE);
                    break;
                }
            } else {
                // save mine location
                if (mines != NULL)
                    appendPosition(mines, createPosition(list->x, list->y));
                else
                    mines = createPosition(list->x, list->y);
            }

            // get new route
            nMines++;
            Position *newlist;
            printf("Mine found! @ %s\n", matrix[list->x][list->y].name);
            newlist = getRoute(last, at->next);
            temp = at->next;

            while (temp->next) {
                joinRoutes(newlist, getRoute(temp, temp->next));
                temp = temp->next;
            }

            list = createPosition(list->x, list->y);
            list->next = newlist;
            // send next two directions
            getDirection(list);
            list = list->next;
            usleep(500 * 1000);
            getDirection(list);
            list = list->next;
        }

        printf("mines found: %d\n", nMines);
        printf("Now at (%d, %d)\n", list->x, list->y);
        printMatrix(list, at->next);
    }

    // print more status
    printf("Now at (%d, %d)\n", list->x, list->y);
    writeByte(&STOP_STATE);

    if (run2) {
        // We found the mine, hooray!
        printf("Found the mine! press a key to continue\n");
        getch();
        return;
    } else {
        // Get ready for stage 2
        printMatrix(list, start);
        printf("All edges searched, press any key go to contiue to phase 2\n");
        getch();

        challenge3(1);
    }

}