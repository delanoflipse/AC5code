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

// GLOBAL VARS
extern const int MATRIX_VALUES[11][11];
extern const char *MATRIX_NAMES[11][11];
extern Cell matrix[11][11];

// FUNCTIONS
void constructMatrix();
Position *getRoute(Position *startpos, Position *endpos);
int range(int p, int min, int max);
void step(int i, int sx, int sy );
int getNewX(int x, int y, int i, int ignore);
int getNewY(int x, int y, int i, int ignore);
int backtrace(Position ptns[100]);
Position *findByName(char name[8]);
Position *getLastPosition(Position *start);
int containsPosition(Position *start, Position *point);
void appendPosition(Position *start, Position *new_pos);

void reset();
void printMatrix(Position *pos, Position *points);
void joinRoutes(Position *list, Position *list2);
Position *createPosition(int x, int y);
void printMatrixNames();
char *charToBinary(char c);




#endif //SRC_MATRIX_H
