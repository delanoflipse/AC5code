#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "challenge12.h"
#include "serial.h"
#include <time.h>

// Define states
char FORWARD_STATE   = 0b00000010;
char LEFT_STATE      = 0b01101100;
char RIGHT_STATE     = 0b01110010;
char ROTATE_STATE    = 0b01110011;
char STOP_STATE      = 0b00001000;
char START_SIGNAL    = 0b01111000;

void challenge1() {
    // declare variables
    char pos_str[8], input;
    clock_t last_time, current_time;
    double dt;

    Position *start = NULL, *list, *last, *at;
    // set starting time
    last_time = clock();
    int n = 0, i = 0;
    Position *temp;

    // clear
    system("@cls||clear");

    // Build the matrix
    constructMatrix();

    // Display matrix names
    printMatrixNames();

    // get number of checkpoints
    printf("Enter number of checkpoints: ");
    scanf("%d", &n);

    // Get points
    while (n-- > 0) {
        printf("Enter a name for point: ", n);
        scanf("%s", pos_str);
        if (start == NULL) {
            start = findByName(pos_str);
            if (start->x == -1 || start->y == -1) {
                printf("Point does not exists");
                return;
            }
        } else {
            temp = getLastPosition(start);
            temp->next = findByName(pos_str);

            if (temp->next->x == -1 || temp->next->y == -1) {
                printf("Point does not exists");
                return;
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

    // set starting point and last point
    at = start;
    last = list;

    system("@cls||clear");
    printf("START\n");

    // Signal start
    writeByte(&START_SIGNAL);
    getDirection(list);

    list = list->next; // set correct start for robot logic(as in: next crossing is the corner after the number)
    printf("Now at (%d, %d)\n", list->x, list->y);

    printMatrix(list, start);

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
                // check if we are arriving at a checkpoint
                if (at->next && list->next->x == at->next->x && list->next->y == at->next->y) {
                    at = at->next;
                    printf("Arriving at %s\n", matrix[at->x][at->y].name);
                }

                // in case still on a crossing, sleep for some time
                usleep(300 * 1000); // warning: depreciated, but still works so fingers crossed
                // send new direction
                getDirection(list);

                // in case something went wrong, do not allow more then two crossings in 0.5 seconds
                if (dt >= 0.5) {
                    last = list;
                    list = list->next;
                }
            } else {
                break;
            }
        } else if(input == 0b01101100 || input == 0b01101101){ // 'm' or 'n'
            // set cell as mine
            matrix[list->x][list->y].value = -10;
            Position *newlist;
            printf("Mine found! @ %s\n", matrix[list->x][list->y].name);
            // get new route
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

        // print more status
        printf("Now at (%d, %d)\n", list->x, list->y);
        printMatrix(list, start);
    }

    // End of list, next: stop
    writeByte(&STOP_STATE);
}

// get direction
void getDirection(Position *list) {
    int x1 = list->next->x - list->x;
    int y1 = list->next->y - list->y;

    int x2 = list->next->next->x - list->next->x;
    int y2 = list->next->next->y - list->next->y;

    int cross = x1 * y2 - x2 * y1; // vector math
    // if we have to go back to where we are now
    if (!cross && list->x == list->next->next->x && list->y == list->next->next->y) {
        writeByte(&ROTATE_STATE);
        printf("Rotating.\n");
    }
    else if (!cross) { // cros == 0
        writeByte(&FORWARD_STATE);
        printf("Going forward.\n");
    }
    else if (cross < 0){
        writeByte(&LEFT_STATE);
        printf("Going left.\n");
    }
    else if (cross > 0) {
        writeByte(&RIGHT_STATE);
        printf("Going right.\n");
    } else { // something is going wrong, very wrong
        printf("NIKS\n");
    }
}