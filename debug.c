//
// Created by Delano on 20/06/2017.
//

#include "debug.h"
#include <conio.h>
#include <stdio.h>
#include "serial.h"

void debug() {
    char c, in, string[9];

    printf("Press q to quit. Every other letter get's send over serial\n");

    for (c = getch(); c != 'q'; c = getch() ) {
        readByte(&in);
        writeByte(&c);

        itoa(in, string, 2);
        printf("Incoming byte(%c): [%s], ", in, string);

        itoa(c, string, 2);
        printf("Sending byte(%c): [%s]\n", c, string);
    }
}