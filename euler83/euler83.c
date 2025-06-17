/*
https://projecteuler.net/problem=83

Unlike euler 81 where I used recursive function and memoization here I could not find a memoization
solution to reduce the number of recursing paths.
Therefore I now using the Dijkstra's algorithm.

https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

Minimum path  = 425185

real    0m0.302s
user    0m0.000s
sys     0m0.092s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdbool.h>

#define DIM 80
#define MAX_VAL 4294967295

typedef struct _node {
    struct _node    *neighbors [4];
    struct _node    *next;
    struct _node    *origin;
    unsigned int    val;        // saved value
    bool            visited;
    char            r;
    char            c;
} NODE;

static short matrix [DIM][DIM];
static NODE nodes [DIM][DIM];

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

// update neighbor with new value
// return unvisited neighbor with minimum calculated value
NODE *check_neighbors (NODE *node) {
    NODE *nb = NULL, *rn = NULL;
    int i;
    unsigned v;

    for (i = 0; i < 4; i++) {
        nb = node->neighbors[i];
        if (nb != NULL) {
            if (nb->visited)
                continue;
           v = node->val + matrix[nb->r][nb->c];
            if (v < nb->val)
                nb->val = v;
            if (rn == NULL)
                rn = nb;
            else if (nb->val < rn->val)
                rn = nb;
        }
    }

    return rn;   
}


// scan all visited nodes, update their neighbors and select the one with the smallest val neighbor
NODE *get_next () {
    NODE *nb = &nodes[0][0], *rb = &nodes[0][0], *nnb;
    unsigned v;
   
    v = MAX_VAL;

    while (nb != NULL) {
        nnb = check_neighbors(nb);
        if (nnb != NULL) {
            if (nnb->val < v) {
                v = nnb->val;
                rb = nnb; 
                rb->origin = nb;
            }     
        }
        nb = nb->next;
    }

    return rb;
}


void print_nodes() {
    NODE *nd = &nodes[DIM-1][DIM-1];

    printf("Tracking origin:\n");
    while (nd != NULL) {
        printf("[%2d][%2d] val:%u, sum: %u\n", nd->r, nd->c, matrix[nd->r][nd->c], nd->val);
        nd = nd->origin;
    }
}

void get_neighbors(NODE *n[], char r, char c) {
    int i, j, k = 0;
    for (i = r-1; i <= r+1; i++)
        for (j = c-1; j <= c+1; j++)
            if ((i == r && j != c) || (i != r && j == c)) {
                if (i == DIM || j == DIM || i < 0 || j < 0)
                    n[k] = NULL;
                else
                    n[k] = &nodes[i][j];

                k++;
            }

}

void init_nodes () {

    for (int r = 0; r < DIM; r++)
        for (int c = 0; c < DIM; c++) {
            nodes[r][c].next = NULL;
            nodes[r][c].origin = NULL;
            nodes[r][c].val = MAX_VAL;
            nodes[r][c].c = c;
            nodes[r][c].r = r;
            nodes[r][c].visited = false;
        }

    // update neighbors
    for (int r = 0; r < DIM; r++)
        for (int c = 0; c < DIM; c++)                    
            get_neighbors(nodes[r][c].neighbors, r, c);

    nodes[0][0].val = matrix[0][0];
    nodes[0][0].visited = true;
}


int main () {
    NODE *nb, *nnb;

    read_matrix();
    init_nodes();

    nb = &nodes[0][0];

    do {
        nnb = get_next();
        nb->next = nnb;
        nb = nnb;
        if (nnb != NULL) {
            nnb->visited = true;
            if (nnb->c == DIM-1 && nnb->r == DIM-1) {
                break;
            }
        }
    } while (nb != NULL);

    printf("\n\nMinimum path  = %u\n", nb != NULL? nb->val: 0);
    // print_nodes();

    return 0;  
}