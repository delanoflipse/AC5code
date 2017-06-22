#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"
#define N 11

// base matrix
const int MATRIX_VALUES[N][N] = {
        {-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1}, //1
        {-1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1}, //2
        {-1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1}, //3
        { 0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0}, //4
        {-1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1}, //5
        { 0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0}, //6
        {-1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1}, //7
        { 0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0}, //8
        {-1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1}, //9
        {-1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1}, //10
        {-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1}, //11
};

const char* MATRIX_NAMES[N][N] = {
         { ""   , ""     , ""     , "9"    , ""     , "8"    , ""     , "7"    , ""     , ""     , "" }
        ,{ ""   , "c00"  , "e0001", "c01"  , "e0102", "c02"  , "e0203", "c03"  , "e0304", "c04"  , "" }
        ,{ ""   , "e0010", ""     , "e0111", ""     , "e0212", ""     , "e0313", ""     , "e0414", "" }
        ,{ "10" , "c10"  , "e1011", "c11"  , "e1112", "c12"  , "e1213", "c13"  , "e1314", "c14"  , "6"}
        ,{ ""   , "e1020", ""     , "e1121", ""     , "e1222", ""     , "e1323", ""     , "e1424", "" }
        ,{ "11" , "c20"  , "e2021", "c21"  , "e2122", "c22"  , "e2223", "c23"  , "e2324", "c24"  , "5"}
        ,{ ""   , "e2030", ""     , "e2131", ""     , "e2232", ""     , "e2333", ""     , "e2434", "" }
        ,{ "12" , "c30"  , "e3031", "c31"  , "e3132", "c32"  , "e3233", "c33"  , "e3334", "c34"  , "4"}
        ,{ ""   , "e3040", ""     , "e3141", ""     , "e3242", ""     , "e3343", ""     , "e3444", "" }
        ,{ ""   , "c40"  , "e4041", "c41"  , "e4142", "c42"  , "e4243", "c43"  , "e4344", "c44"  , "" }
        ,{ ""   , ""     , ""     , "1"    , ""     , "2"    , ""     , "3"    , ""     , ""     , "" }
};

Cell matrix[N][N];

// FUNCTION DEFINITIONS
Position *getRoute(Position *startpos, Position *endpos) {
    int i = 1;

    // Make a position list, starting with the start position
    Position *list = createPosition(startpos->x, startpos->y);

    // reset everything
    reset();

    // mark the end position
    matrix[endpos->x][endpos->y].value = i;

    // run step function while start position is not marked
    while (matrix[startpos->x][startpos->y].value == 0) {
        step(i, endpos->x, endpos->y);
        i++;
    }

    // run backtrace
    while(backtrace(list)) continue;

    return list;
}

// Create a new position pointer for location (x, y)
Position *createPosition(int x, int y) {
    Position *p = (Position*)malloc(sizeof(Position));
    p->x = x;
    p->y = y;
    p->next = NULL;

    return p;
}

// min <= p <= max
int range(int p, int min, int max) {
    if (p <= min) return min;
    if (p >= max) return max;
    return p;
}

// wave formation lee algorithm
void step(int i, int sx, int sy ) {
    int x, y, xi, yi, rx, ry, neighbour;

    // make wave one size bigger(contained by the matrix)
    xi = range(sx - i - 1, 0, N);
    rx = range(sx + i + 1, 0, N);

    yi = range(sy - i - 1, 0, N);
    ry = range(sy + i + 1, 0, N);

    // for each cell
    for (x = xi; x < rx; x++) {
        for (y = yi; y < ry; y++) {
            int value = matrix[x][y].value;

            if (value) continue;

            neighbour = 0;

            // left, right, up, down check neighbour
            if (x > 0 && matrix[x - 1][y].value == i)      neighbour = 1;
            else if (x < N - 1 && matrix[x + 1][y].value == i)  neighbour = 1;
            else if (y > 0 && matrix[x][y - 1].value == i)      neighbour = 1;
            else if (y < N - 1 && matrix[x][y + 1].value == i)  neighbour = 1;

            // mark self
            if (neighbour) matrix[x][y].value = i + 1;
        }
    }
}

// Get new x position when backtracing
int getNewX(int x, int y, int i, int ignore) {
    if (ignore) return x;
    int newx = x;
    if (x > 0 && matrix[x - 1][y].value == i - 1) {
        newx = x - 1;
    } else if (x < N - 1 && matrix[x + 1][y].value == i - 1) {
        newx = x + 1;
    }

    return newx;
}

// Get new y position when backtracing
int getNewY(int x, int y, int i, int ignore) {
    if (ignore) return y;
    int newy = y;

    if (y > 0 && matrix[x][y - 1].value == i - 1) {
        newy = y - 1;
    } else if (y < N - 1 && matrix[x][y + 1].value == i - 1) {
        newy = y + 1;
    }

    return newy;
}

int backtrace(Position *start) {
    if (start == NULL) {
        return 0;
    }

    struct Position *point = start;

    // go to end of list
    while (point->next != NULL && point->next->next !=NULL) point = point->next;

    int x, y, px, py;

    // get previous location, if it exists
    if (point->next != NULL) {
        px = point->x;
        py = point->y;

        point = point->next;

        x = point->x;
        y = point->y;
    } else {
        x = px = point->x;
        y = py = point->y;
    }

    int i = matrix[x][y].value;
    int newx, newy;

    // Prioritise going straight
    if (x - px) {
        newx = getNewX( x, y, i, 0 );
        newy = getNewY( x, y, i, newx - x ); // do not go diagonal
    } else {
        newy = getNewY( x, y, i, 0 );
        newx = getNewX( x, y, i, newy - y ); // do not go diagonal
    }

    // add position to list
    Position *newpos = createPosition(newx, newy);
    point->next = newpos;

    // return '1' if not done backtracing
    if (matrix[x][y].value > 1) {
        return 1;
    }

    return 0;
}

// Find position by name string
Position *findByName(char *name) {
    Position *pos = createPosition(-1, -1);
    int x, y;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            if (strcmp(matrix[x][y].name, name) == 0) {
                pos->x = x;
                pos->y = y;
                return pos;
            }
        }
    }

    return pos;
}

// set all cells that are not blocked back to 0
void reset() {
    int x, y;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            matrix[x][y].value = matrix[x][y].value > -1 ? 0 : matrix[x][y].value;
        }
    }
}

// create the matrix
void constructMatrix () {
    int x, y;

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            int v = MATRIX_VALUES[y][x];
            Cell c;
            c.value = v;
            strncpy(c.name,  MATRIX_NAMES[y][x], 8);
            matrix[x][y] = c;
//            printf("%d,%s", c.value, c.name);
        }
    }
}

// Print the names of all the matrix cells
void printMatrixNames() {
    Cell c;
    int x, y, i, end;
    char str[5];
    str[4] = '\0';
    char l;

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            c = matrix[x][y];
            end = 0;

            for (i = 0; i < 4; i++) {
                if (c.name[i] == '\0') end = 1;

                if (!end) str[i] = c.name[i];
                else str[i] = ' ';
            }

            printf("%s ", str);
        }
        printf("\n\n");
    }
}

// Status of the robot on the matrix
void printMatrix(Position *pos, Position *points) {
    Cell c;
    Position *temp;
    int x, y, i, n;
    char str[5];
    str[4] = '\0';
    char l;

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            c = matrix[x][y];
            l = c.value < 0 ? ' ' : '*';
            n = 1;
            temp = points;
            while (temp) {
                if (x == temp->x && y == temp->y) l = n + 48;
                temp = temp->next;
                n++;
            }

            if (c.value < -1 ) l = 'M'; // mines
            if (x == pos->x && y == pos->y) l = '@'; // 'at' position of robot
            // robot direction
            if (pos->next && x == pos->next->x && y == pos->next->y){
                if ( (x - pos->x) == 1 && (y - pos->y == 0) ) l = '>';
                if ( (x - pos->x) == -1 && (y - pos->y == 0) ) l = '<';
                if ( (x - pos->x) == 0 && (y - pos->y == -1) ) l = '^';
                if ( (x - pos->x) == 0 && (y - pos->y == 1) ) l = '^';
            }
            printf("%c ", l);
        }
        printf("\n");
    }
}

// join two route lists
void joinRoutes(Position *list, Position *list2) {
    while (list->next && list->next->next) list = list-> next;
    list->next = list2->next;
}

// print char as binary
char *charToBinary(char c) {
    int i = 0;
    char *output = malloc (9 * sizeof(char));
    for (i = 0; i < 8; ++i) {
        output[7-i] = (c >> i) & 1 ? '1' : '0';
    }
    output[8] = '\0';

    return output;
}

// get the one before the last position in the list
Position *getLastPosition(Position *start){
    while (start && start->next) start = start->next;
    return start;
}

// add a position to the position list
void appendPosition(Position *start, Position *new_pos) {
    start = getLastPosition(start);
    start->next = new_pos;
}

// check if the list contains a point
int containsPosition(Position *start, Position *point) {
    for (; start; start = start->next) {
        if (start->x == point->x && start->y == point->y) return 1;
    }
    return 0;
}