#include <stdio.h>
#include "challenge12.h"
#include "serial.h"

int main() {
    if (!serialInit()) {
        return 0;
    }

    char opt;
    while (1) {
        printf("------------------------------------------------------------------\n");
        printf("- AC5 SMART ROBOT CHALLENGE\n");
        printf("------------------------------------------------------------------\n\n");
        printf("Choose what to do:\n");
        printf("1: Challenge 1. Go to a number of checkpoints\n");
        printf("2: Challenge 2. Go to checkpoints with mines in the way\n");
        printf("other: Exit\n");
        printf("Your choice: ");
        scanf("%c", &opt);

        if (opt == '1' || opt == '2') {
            // Run assignment one
            challenge1();
        } else {
            break;
        }

    }

    closeSerial();

    return 0;
}