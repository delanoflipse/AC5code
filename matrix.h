#ifndef SRC_MATRIX_H
#define SRC_MATRIX_H


// Cell struct
typedef struct {
    int value;
    char name[8];
} Cell;

// Position struct
struct Position {
    int x;
    int y;
    struct Position *next;
};
typedef struct Position Position;

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
extern const int MATRIX_VALUES[13][13];
extern const char *MATRIX_NAMES[13][13];
extern Cell matrix[13][13];

// FUNCTIONS
void constructMatrix();
Position *getRoute(Position *startpos, Position *endpos);
int range(int p, int min, int max);
void step(int i, int sx, int sy );
int getNewX(int x, int y, int i, int ignore);
int getNewY(int x, int y, int i, int ignore);
int backtrace(Position ptns[100]);
Position *findByName(char name[8]);
void reset();
void printMatrix(int opt);
void joinRoutes(Position *list, Position *list2);
Position *createPosition(int x, int y);


#endif //SRC_MATRIX_H
