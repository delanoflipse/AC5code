//
// Created by Delano on 20/06/2017.
//

#include "challenge3.h"
#include "challenge12.h"
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"


void challenge3() {
    char pos_str[8], input, name[4];

    Position *start = NULL, *list, *last, *at, *mines;
    int n = 0, i, j;
    Position *temp;

    // clear
    system("@cls||clear");

    // Build the matrix
    constructMatrix();

    // Display matrix names
    printMatrixNames();

    // Set all points as checkpoints
    name[0] = 'c';
    for (i = 4; i >= 0; i--) {
        for (j = 0; j <= 4; j++) {
            name[1] = i + 48;
            name[2] = j + 48;
            if (start == NULL) {
                start = findByName(pos_str);
                if (start->x == -1 || start->y == -1) {
                    printf("Point does not exists");
                    return;
                }
            } else {
                temp = start;
                while (temp->next) temp = temp->next;

                temp->next = findByName(pos_str);

                if (temp->next->x == -1 || temp->next->y == -1) {
                    printf("Point does not exists");
                    return;
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

    // TODO: SEND INITIAL SIGNAL
    system("@cls||clear");
    printf("Type any key to start phase 1:\n");
    scanf("%c", input);
    list = list->next;
    printf("Now at (%d, %d)\n", list->x, list->y);
    printMatrix(list, start);

    writeByte(&START_SIGNAL);

    char inp;
    while (list->next) {
        char x = input;
        while (x == input) {
            readByte(&input);
        }

        printf("\n%c\n", input);

        system("@cls||clear");

        if (input == 'q') break;
        else if(input == 0b11001100 || input == 0b11001101) {
//        else if(input == 'x') {
            if (list->next && list->next->next) {
                // Check for checkpoints
                temp = at;
                while (temp->next) {
                    if (temp->next && temp->next->next && list->next->x == temp->next->x && list->next->y == temp->next->y) {
                        temp->next = temp->next->next;
                        printf("Arrived at %s\n", matrix[temp->x][temp->y].name);
                    }

                    temp = temp->next;
                }

                getDirection(list);

                last = list;
                list = list->next;
            } else {
                break;
            }
        } else if(input == 0b01010100 || input == 0b01010101){
//        } else if(input == 'x'){
            matrix[list->x][list->y].value = -1;
            Position *newlist;
            printf("blocked edge! @ %s\n", matrix[list->x][list->y].name);
            newlist = getRoute(last, at->next);
            temp = at->next;
            while (temp->next) {
                joinRoutes(newlist, getRoute(temp, temp->next));

                temp = temp->next;
            }

            list = createPosition(list->x, list->y);
            list->next = newlist;
            getDirection(list);

        }

        printf("Now at (%d, %d)\n", list->x, list->y);
        printMatrix(list, start);
    }

    writeByte(&STOP_STATE);
    // TODO: SEND STOP SIGNAL
}