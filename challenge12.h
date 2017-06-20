//
// Created by Delano on 01/06/2017.
//

#include "matrix.h"

#ifndef SRC_CHALLENGE1_H
#define SRC_CHALLENGE1_H


char FORWARD_STATE;
char LEFT_STATE;
char RIGHT_STATE;
char ROTATE_STATE;
char STOP_STATE;
char START_SIGNAL;

void getDirection(Position *list);
void challenge1();

#endif //SRC_CHALLENGE1_H
