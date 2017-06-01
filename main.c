#include <stdio.h>
#include "challenge1.h"

int main() {
    char opt;

    printf("Choose what to do:\n1: Challenge 1. From A to B to C\nq: Exit\nYour choice: ");
    scanf("%c", &opt);

    if (opt == '1') {
        // Run assignment one
        challenge1();
    }

    return 0;
}