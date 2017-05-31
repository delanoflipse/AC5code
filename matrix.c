#define N 13

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TYPEDEFS

// base matrix
const int MATRIX_VALUES[N][N] = {
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}, //1
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}, //2
        {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1}, //3
        {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1}, //4
        { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0}, //5
        {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1}, //6
        { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0}, //7
        {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1}, //8
        { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0}, //9
        {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1}, //10
        {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1}, //11
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}, //12
        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}, //13
};

const char* MATRIX_NAMES[13][13] = {
         {""   , "", ""     , ""     , "9"    , ""     , "8"    , ""     , "7"    , ""     , ""     , "", "" }
        ,{""   , "", ""     , ""     , ""     , ""     , ""     , ""     , ""     , ""     , ""     , "", "" }
        ,{""   , "", "c00"  , "e0001", "c01"  , "e0102", "c02"  , "e0203", "c03"  , "e0304", "c04"  , "", "" }
        ,{""   , "", "e0010", ""     , "e0111", ""     , "e0212", ""     , "e0313", ""     , "e0414", "", "" }
        ,{"10" , "", "c10"  , "e1011", "c11"  , "e1112", "c12"  , "e1213", "c13"  , "e1314", "c14"  , "", "6"}
        ,{""   , "", "e1020", ""     , "e1121", ""     , "e1222", ""     , "e1323", ""     , "e1424", "", "" }
        ,{"11" , "", "c20"  , "e2021", "c21"  , "e2122", "c22"  , "e2223", "c23"  , "e2324", "c24"  , "", "5"}
        ,{""   , "", "e2030", ""     , "e2131", ""     , "e2232", ""     , "e2333", ""     , "e2434", "", "" }
        ,{"12" , "", "c30"  , "e3031", "c31"  , "e3132", "c32"  , "e3233", "c33"  , "e3334", "c34"  , "", "4"}
        ,{""   , "", "e3040", ""     , "e3141", ""     , "e3242", ""     , "e3343", ""     , "e3444", "", "" }
        ,{""   , "", "c40"  , "e4041", "c41"  , "e4142", "c42"  , "e4243", "c43"  , "e4344", "c44"  , "", "" }
        ,{""   , "", ""     , ""     , ""     , ""     , ""     , ""     , ""     , ""     , ""     , "", "" }
        ,{""   , "", ""     , ""     , "1"    , ""     , "2"    , ""     , "3"    , ""     , ""     , "", "" }
};

// Cell struct
typedef struct {
    int value;
    char name[8];
} Cell;

// Position struct
typedef struct {
    int x;
    int y;
} Position;

// Route
typedef struct RoutePoint {
    int x;
    int y;
    struct RoutePoint *left;
    struct RoutePoint *right;
    struct RoutePoint *up;
    struct RoutePoint *down;
} RoutePoint;

// GLOBAL VARS
Cell matrix[N][N];

// FUNCTIONS
void constructMatrix();
int range(int p, int min, int max);
void step(int i, int sx, int sy );
int getNewX(int x, int y, int i, int ignore);
int getNewY(int x, int y, int i, int ignore);
void backtrace(int x, int y, int px, int py);
Position findByName(char name[8]);
void reset();
void printMatrix(int opt);

void run(Position startpos, Position endpos) {
    int i = 1;

    reset();
    matrix[endpos.x][endpos.y].value = i;

    while (matrix[startpos.x][startpos.y].value == 0) {
        step(i, endpos.x, endpos.y);
        i++;
    }

    backtrace(startpos.x, startpos.y, startpos.x, startpos.y);
}

RoutePoint *createRoutePoint(int x, int y, int pos, RoutePoint *prev) {
    RoutePoint *point = malloc (sizeof (struct RoutePoint));
    point->x = x;
    point->y = y;
    if (prev != NULL) {
        if (pos == 0) prev->left = point;
        else if (pos == 1) prev->right = point;
        else if (pos == 2) prev->up = point;
        else if (pos == 3) prev->down = point;
    }

    return point;
}

// min <= p <= max
int range(int p, int min, int max) {
    if (p <= min) return min;
    if (p >= max) return max;
    return p;
}

void step(int i, int sx, int sy ) {
    int x, y, xi, yi, rx, ry, neighbour;

    // move 1 block outside
    xi = range(sx - i - 1, 0, N);
    rx = range(sx + i + 1, 0, N);

    yi = range(sy - i - 1, 0, N);
    ry = range(sy + i + 1, 0, N);

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

            if (neighbour) matrix[x][y].value = i + 1;
        }
    }
}

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

void backtrace(int x, int y, int px, int py) {
    int i = matrix[x][y].value;
    int newx = x, newy = y;

    // Prioritise going straight
    if (x - px) {
        newx = getNewX( x, y, i, 0 );
        newy = getNewY( x, y, i, newx - x ); // do not go diagonal
    } else {
        newy = getNewY( x, y, i, 0 );
        newx = getNewX( x, y, i, newy - y ); // do not go diagonal
    }

    if (matrix[x][y].name[0] != 'e') {
        printf("%s ", matrix[x][y].name);
    }

    if (matrix[x][y].value > 1) {
        backtrace( newx, newy, x, y );
    }
}

Position findByName(char name[8]) {
    int x, y;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            if (strcmp(matrix[x][y].name, name) == 0) {
                return (Position) {x, y};
            }
        }
    }

    return (Position) { -1, -1};
}

void reset() {
    int x, y;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            matrix[x][y].value = matrix[x][y].value > -1 ? 0 : -1;
        }
    }
}

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

void printMatrix(int opt) {
    Cell c;
    int x, y;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            c = matrix[x][y];

            if (opt) {
                printf("%d ",c.value);
            } else {
                printf("%s ",c.name);
            }
        }
        printf("\n");
    }
}