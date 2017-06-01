#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "challenge1.h"
#include "serial.h"

char start_str[8], mid_str[8], end_str[8];
Position *start, *mid, *end, *list, *last;

void challenge1() {
    int AT_MID = 0;

    // Build the matrix
    constructMatrix();


    // Display matrix names
    printMatrixNames();

    printf("Choose startpoint, midpoint and endpoint by typing in the names of the cells\n");

    scanf("%s%s%s", start_str, mid_str, end_str);

    start = findByName(start_str);
    mid = findByName(mid_str);
    end = findByName(end_str);

    if (
        start->x == -1 || start->y == -1 ||
        mid->x == -1 || mid->y == -1 ||
        end->x == -1 || end->y == -1
        ) {
        printf("One of the points does not exist!");
        return;
    }

    printf("start: (%d, %d)\n", start->x, start->y);
    printf("mid: (%d, %d)\n", mid->x, mid->y);
    printf("end: (%d, %d)\n", end->x, end->y);

    list = getRoute(start, mid);
    joinRoutes(list, getRoute(mid, end));

    // TODO: SEND INITIAL SIGNAL
    system("@cls||clear");
    printf("START\n");
    list = list->next;
    printf("Now at (%d, %d)\n", list->x, list->y);
    printMatrix(list, start, mid, end);

    char inp;
    while (list->next) {
        scanf("%s", &inp);
        system("@cls||clear");

        if (inp == 'q') break;
        else if(inp == 'x') {
            if (list->next && list->next->next) {
                // TODO: GET ANGLE
                // TODO: GOTO NEXT TWO

                // Check for mid pos
                if (list->next->x == mid->x && list->next->y == mid->y) {
                    AT_MID = 1;
                }

                int x1 = list->next->x - list->x;
                int y1 = list->next->y - list->y;

                int x2 = list->next->next->x - list->next->x;
                int y2 = list->next->next->y - list->next->y;

                int cross = x1 * y2 - x2 * y1;
                if (!cross && list->x == list->next->next->x && list->y == list->next->next->y) printf("ROTATE!\n");
                else if (!cross) printf("GO STRAIGHT\n");
                else if (cross < 0) printf("GO LEFT\n");
                else if (cross > 0) printf("GO RIGHT\n");

                last = list;
                list = list->next->next;
            } else {
                break;
            }
        } else if(inp == 'b'){
            matrix[list->x][list->y].value = -1;
            Position *newlist;
            if (AT_MID) {
                newlist = getRoute(last->next, end);
            } else {
                newlist = getRoute(last->next, mid);
                joinRoutes(newlist, getRoute(mid, end));
            }

            list = createPosition(list->x, list->y);
            list->next = newlist;

        }

        printf("Now at (%d, %d)\n", list->x, list->y);
        printMatrix(list, start, mid, end);
    }

    // TODO: SEND STOP SIGNAL
}