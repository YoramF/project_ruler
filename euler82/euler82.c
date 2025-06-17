/*
similar to euler 81 use recursive function and memoization to avoid recalculations.
here memory matrix must be of 3 dimensions: row, column and DIRECTION - that is, because
we can reach a particular location from 3 different directions. we must store also the
direction to avoid visiting same matrix cell more than once.

Minimum path = 260324

real    0m0.218s
user    0m0.045s
sys     0m0.045s
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIM 80
// #define DIM 5
#define MAX_VAL 4294967295

typedef enum {
    NONE,
    RIGHT,
    UP,
    DOWN,
    DIRECTION_OPTIONS
} DIRECTION;

static short matrix [DIM][DIM];
static unsigned sums [DIM][DIM][DIRECTION_OPTIONS];

void read_matrix () {
    FILE *f;
    char buf [DIM*5+3]; // each item is 4 chars + comma + <CR><LF> + '\0'
    char *del = ",\\r\\n";
    char* token;
    int i, j;

    if ((f = fopen("./matrix.txt", "r")) == NULL) {
        fprintf(stderr, "failed to open file matrix.txt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (!feof(f)) {
        if (fgets(buf, sizeof(buf), f) != NULL) {
            token = strtok(buf, del);
            j = 0;
            while (token != NULL) {
                matrix[i][j] = (short)atoi(token);
                j++;
                token = strtok(NULL, del);
            }
            if (j < DIM) {
                fprintf(stderr, "Failed to read line %d, read only %d numbers\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
        else {
            fprintf(stderr, "Failed to read line %d\n", i);
            exit(EXIT_FAILURE);            
        }
        i++;
    }

    if (i < DIM) {
        fprintf(stderr, "read only %d lines\n", i);
        exit(EXIT_FAILURE);
    }

    fclose(f);
}

unsigned min (unsigned a, unsigned b, unsigned c) {
    unsigned r;

    r = a;
    if (b < r) r = b;
    if (c < r) r = c;

    return r;
}

// return the sum of moved if choosing row r and column c
unsigned move (int r, int c, DIRECTION d) {
    unsigned v1, v2, v3, v4, sum;

    // make sure we stay within matrix borders
    if (r == DIM || c == DIM || r < 0)
        return MAX_VAL;

    // first check if this cell was already calculated
    if (sums[r][c][d] > 0)
        return sums[r][c][d];

    if (c == (DIM-1)) {
        sums[r][c][d] = matrix[r][c];
        return sums[r][c][d];
    }

    v3 = MAX_VAL; 
    switch (d) {
        case UP:
            v1 = move(r, c+1, RIGHT);
            v2 = r == 0? MAX_VAL: move(r-1, c, UP);
            break;
        case DOWN:
            v1 = move(r, c+1, RIGHT);
            v2 = r == (DIM-1)? MAX_VAL: move(r+1, c, DOWN);
            break;
        case RIGHT:
            v1 = c == (DIM-1)? matrix[r][c]: move(r, c+1, RIGHT);
            v2 = move(r+1, c, DOWN);
            v3 = move(r-1, c, UP);
            break;
        default:
            if (r == 0)
                v1 = move(r+1, c, DOWN);
            else if (r == (DIM-1))
                v1 = move(r-1, c, UP);
            else {
                v1 = move(r+1, c, DOWN);
                v3 = move(r-1, c, UP);
            }
            v2 = move(r, c+1, RIGHT);
    }

    v4 = min(v1, v2, v3);
    sums[r][c][d] = v4 + matrix[r][c];
    return sums[r][c][d];
}

int main () {
    unsigned sum, s;
    read_matrix();

    // reset sums matrix
    memset(sums,0,DIM*DIM*DIRECTION_OPTIONS*sizeof(unsigned));

    sum = MAX_VAL;
    for (int i = 0; i < DIM; i++) {
        s = move(i, 0, NONE);
        if (s < sum)
            sum = s;
    }

    printf("Minimum path = %u\n", sum);

    return 0;  
}