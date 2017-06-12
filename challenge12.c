#include <stdio.h>
#include <stdlib.h>
#include "challenge12.h"
#include "serial.h"

char pos_str[8], input;
Position *start = NULL, *list, *last, *at;

char FORWARD_STATE   = 0b0000001;
char LEFT_STATE      = 0b0000010;
char RIGHT_STATE     = 0b0000100;
char ROTATE_STATE    = 0b0001000;
char STOP_STATE      = 0b0010000;

void printbinchar(char character)
{
    char output[9];
    itoa(character, output, 2);
    printf("%s\n", output);
}

void challenge1() {
    int n = 0;
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
            temp = start;
            while (temp->next) temp = temp->next;

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

    // TODO: SEND INITIAL SIGNAL
    system("@cls||clear");
    printf("START\n");
    list = list->next;
    printf("Now at (%d, %d)\n", list->x, list->y);
    printMatrix(list, start);

    writeByte(&FORWARD_STATE);

    char inp;
    while (list->next) {
        char x = input;
        while (x == input) {
            readByte(&input);
        }

        printbinchar(input);
        printf("\n%c\n", input);

        system("@cls||clear");

        if (input == 'q') break;
        else if(input == 0b00000010 || input == 0b00000011) {
//        else if(input == 'x') {
            if (list->next && list->next->next) {
                // Check for mid pos
                if (at->next && list->next->x == at->next->x && list->next->y == at->y) {
                    at = at->next;
                    printf("Arrived at %s\n", matrix[at->x][at->y].name);
                }

                getDirection(list);

                last = list;
                list = list->next;
            } else {
                break;
            }
        } else if(input == 0b00000100 || input == 0b00000101){
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
    }
}