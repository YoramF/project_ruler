/*
use recursive function and memoization to avoid recalculations

Minimum path = 427337

real    0m0.161s
user    0m0.015s
sys     0m0.031s
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIM 80
#define MAX_VAL 4294967295

static short matrix [DIM][DIM];
static unsigned sums [DIM][DIM];

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

// return the sum of moved if choosing row r and column c
unsigned move (int r, int c) {
    unsigned v1, v2, sum;

    // make sure we stay within matrix borders
    if (r == DIM || c == DIM)
        return MAX_VAL;

    // first check if this cell was already calculated
    if (sums[r][c] > 0)
        return sums[r][c];

    v1 = move(r, c+1);
    v2 = move(r+1, c);

    if (v1 == MAX_VAL && v2 == MAX_VAL) {
        sums[r][c] = matrix[r][c];
        return matrix[r][c];     
    }
    else {
        if (v1 < v2)
            sums[r][c] = v1 + matrix[r][c];
        else
            sums[r][c] = v2 + matrix[r][c];
        return sums[r][c];
    }
}

int main () {
    unsigned sum;
    read_matrix();

    // reset sums matrix
    memset(sums,0,DIM*DIM*sizeof(int));

    sum = move(0, 0);

    printf("Minimum path = %u\n", sum);

    return 0;  
}