//
// Created by Delano on 01/06/2017.
//

#include "matrix.h"
#include "conio.h"

#ifndef SRC_SERIAL_H
#define SRC_SERIAL_H

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>

#define COMPORT "COM4"
#define BAUDRATE CBR_9600

HANDLE hSerial;

int serialInit();
int closeSerial();
int initSio();
int writeByte(char *buffWrite);
int readByte(char *buffRead);

#endif //SRC_SERIAL_H
