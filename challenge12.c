#include <stdio.h>
#include <stdlib.h>
#include "challenge12.h"
#include "serial.h"
#include <time.h>


char FORWARD_STATE   = 0b00000010;
char LEFT_STATE      = 0b01101100;
char RIGHT_STATE     = 0b01110010;
char ROTATE_STATE    = 0b01110011;
char STOP_STATE      = 0b00001000;
char START_SIGNAL    = 0b01111000;

void challenge1() {
    char pos_str[8], input;
    clock_t last_time, current_time;
    double dt;

    Position *start = NULL, *list, *last, *at;

    last_time = clock();
    int n = 0, i = 0;
    Position *temp;

    // clear
    system("@cls||clear");

    // Build the matrix
    constructMatrix();

    // Display matrix names
    printMatrixNames();

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

    at = start;
    last = list;

    system("@cls||clear");
    printf("START\n");

    writeByte(&START_SIGNAL);
    getDirection(list);

    list = list->next; // set correct start for robot logic(as in: next crossing is the corner after the number)
    printf("Now at (%d, %d)\n", list->x, list->y);

    printMatrix(list, start);

    char inp, inp_bin[9];
    while (list->next) {
//        char x = input;
//        while (x == input) {
            readByte(&input);
//        }

        system("@cls||clear");
        printf("Recieved: %c [%s]\n", input, charToBinary(input));

        current_time = clock();
        dt = (double) (current_time - last_time) / CLOCKS_PER_SEC;

        printf("dt: %f\n", dt);

        if (input == 'q') break;
        else if(input == 0b01111000) {
//        else if(input == 0b00011111 || input == 0b00011110) {
//        else if(input == 'x') {
            printf("n: %d\n", ++i);
            if (list->next && list->next->next && list->next->next->next) {
                // Check for checkpoints
                if (at->next && list->next->x == at->next->x && list->next->y == at->next->y) {
                    at = at->next;
                    printf("Arriving at %s\n", matrix[at->x][at->y].name);
                }

                getDirection(list);

                if (dt >= 0.5) {
                    last = list;
                    list = list->next;
                    last_time = current_time;
                }
            } else {
                break;
            }
        } else if(input == 0b01101101){
//        } else if(input == 0b11100001 || input == 0b11100000){
//        } else if(input == 'x'){
            matrix[list->x][list->y].value = -10;
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
            getDirection(list);
            list = list->next;
        }

        printf("Now at (%d, %d)\n", list->x, list->y);
        printMatrix(list, start);
    }

    writeByte(&STOP_STATE);
}

void getDirection(Position *list) {
    int x1 = list->next->x - list->x;
    int y1 = list->next->y - list->y;

    int x2 = list->next->next->x - list->next->x;
    int y2 = list->next->next->y - list->next->y;

    int cross = x1 * y2 - x2 * y1;
    if (!cross && list->x == list->next->next->x && list->y == list->next->next->y) {
        writeByte(&ROTATE_STATE);
        printf("Rotating.\n");
    }
    else if (!cross) {
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
    } else {
        printf("NIKS\n");
    }
}