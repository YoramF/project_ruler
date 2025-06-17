/*
https://projecteuler.net/problem=104

Solution approach: in order to check the most significant digits we need to implement
simple BIG number representation where each block is
10^9-1. For the least significant digits it was enough to follow only the 9 last
digits without implementing BIG numbers. Since we do implement BIG numbers, Calculate the
Fibonacci sequance using BIG numbers and chek first block (least significant) if nine 
digits are 1-9 pandigital and check the last 2 blocks if 9 most significan digits are 1-9
pandigital. 

k: 329468

real    0m2.604s
user    0m2.533s
sys     0m0.067s

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define BASE 1000000000
#define INITIAL_ALLOC 100
#define INC_ALLOCC 10

typedef struct {
    int blocks;
    int c_block;
    unsigned int data [0];
} BIG_NUMBER;

// check if a given number is 1-9 pandigital
bool is_pandigital (unsigned int n) {
    char digits [10];
    int i = 9, m;

    memset(digits, 0, sizeof(digits));

    while (n) {
        m = n % 10;
        if (m > 0 && digits[m] == 0) {
            digits[m] = 1;
            i--;
            n /= 10;
        }
        else
            break;
    }

    return i == 0;
}

// init BIG_NUMBER with value
BIG_NUMBER *init_b (unsigned int n) {
    BIG_NUMBER *bn;

    if ((bn = malloc(sizeof(BIG_NUMBER)+INITIAL_ALLOC*sizeof(unsigned int))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    bn->blocks = INITIAL_ALLOC;
    memset(bn->data, 0, INITIAL_ALLOC*sizeof(unsigned int));
    bn->c_block = 0;
    bn->data[0] = n;


    return bn;
}

// increase BIG_NUMBER allocation
BIG_NUMBER *inc_b (BIG_NUMBER *bn, int blocks) {
    if (bn == NULL) {
        fprintf(stderr, "Wrong pointer to BIG_NUMBER\n");
        exit (EXIT_FAILURE);
    }

    if ((bn = realloc(bn, sizeof(BIG_NUMBER)+(bn->blocks+blocks)*sizeof(unsigned int))) == NULL) {
        fprintf(stderr, "Failed to reallocate RAM: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    memset(&bn->data[bn->blocks], 0, blocks);
    bn->blocks += blocks;

    return bn;
}

// add two big numers and write the result into result
// for simplicity, result can't point to any of the other two BIG_NUMBERS
void add_b (BIG_NUMBER **r, BIG_NUMBER *n1, BIG_NUMBER *n2) {
    int bc;
    unsigned int cr, t;
    BIG_NUMBER *n;

    // first check that r has anough space to contain the addition result
    n = n1->c_block >= n2->c_block? n1: n2;
    if (((*r)->blocks < n->blocks) || (n->c_block == n->blocks-1)) {
            // allocate additional block for result
            *r = inc_b(*r, (n->blocks-(*r)->blocks+INC_ALLOCC));
    }

    // do the arithemetic now
    cr = 0;
    bc = 0;
    while (bc <= n1->c_block && bc <= n2->c_block) {
        t = n1->data[bc] + n2->data[bc] + cr;
        (*r)->data[bc] = t % BASE;
        cr = t / BASE;
        bc++;
    }

    // continue scaning longer number
    while (bc <= n->c_block) {
        t = n->data[bc] + cr;
        (*r)->data[bc] = t % BASE;
        cr = t / BASE;
        bc++;
    }

    // finalize r data
    if (cr > 0) {
        (*r)->c_block = bc;
        (*r)->data[bc] = cr;
    }
    else
        (*r)->c_block = bc - 1;

}

// return number of digits (including trailing zeros)
int digits (unsigned int n) {
    int c = 0;

    while (n) {
        n /= 10;
        c++;
    }

    return c;
}

// get most significan 9 digits
unsigned int m_sig_d_b (BIG_NUMBER *bn) {
    unsigned int t = 0, d;

    if (bn != NULL) {
        if ((d = digits(bn->data[bn->c_block])) == 9)
            t = bn->data[bn->c_block];
        else {
            // check if current bn value is less than BASE
            if (bn->c_block == 0)
                t = bn->data[bn->c_block];
            else {
                // we need to constract the 9 digits number
                t = bn->data[bn->c_block]*(unsigned)pow((double)10,(double)(9-d));
                t += (bn->data[bn->c_block-1])/(unsigned)pow((double)10,(double)d);
            }
        }        
    }

    return t;
}

// get list significant 9 digits
unsigned int l_sig_d_b (BIG_NUMBER *bn) {
    if (bn != NULL)
        return bn->data[0];

    return 0;
}

void print_b (int k, BIG_NUMBER *bn) {
    int bc;

    if (bn != NULL) {
        bc = bn->c_block;
        printf("[%d] blocks: %d: ", k, bn->c_block+1);
        while (bc >= 0) {
            printf("%09u", bn->data[bc]);
            bc--;
        }
        printf("\n");
    }
}

 
int main () {
    BIG_NUMBER *bn1, *bn2, *bn3;
    BIG_NUMBER *f1, *f2, *f3, *f;
    int k = 2;
    unsigned int dl, dm;

    bn1 = init_b(1);
    bn2 = init_b(1);
    bn3 = init_b(2);

    f1 = bn1;
    f2 = bn2;
    f3 = bn3;

    dl = l_sig_d_b(f3);
    dm = m_sig_d_b(f3);

    while (!(is_pandigital(dl) && is_pandigital(dm))) {
        k++;
        add_b(&f3, f2, f1);
        // print_b(k, f3);
        dl = l_sig_d_b(f3);
        dm = m_sig_d_b(f3);
        f = f1;
        f1 = f2;
        f2 = f3;
        f3 = f;

    }

    free(f1);
    free(f2);
    free(f3);

    printf("k: %d\n", k);

    return 0;
} 