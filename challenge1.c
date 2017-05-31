#include <stdio.h>
#define N 13

// Cell struct
typedef struct {
    int value;
    char *name;
} Cell;

extern Cell matrix[N][N];

extern void constructMatrix();
extern void printMatrix();

void challenge1() {
    constructMatrix();
    printMatrix();
}