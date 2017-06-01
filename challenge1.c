#include <stdio.h>
#include "matrix.h"
#include "challenge1.h"
#include "serial.h"

char instructions[100];

char start_str[8], mid_str[8], end_str[8];
Position *start, *mid, *end;

void challenge1() {
    Position *list;
    // Build the matrix
    constructMatrix();

    // Display matrix names
    printMatrix(0);

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

    while (list->next) {
        printf("%s : (%d, %d)\n", matrix[list->x][list->y].name, list->x, list->y);
        list = list->next;
    }
}