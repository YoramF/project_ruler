/*

https://projecteuler.net/problem=113

This problem needs different appraoch than problem 112. Iterating thrugh 10^100 numbers will
take forever. 

In general the total number of non-bouncy" numbers is: (increasing + decreasing numbers) -
9 * 100 - 1.

Use two recursive procedures one to count increasing numbers and one to count decreasing numbers and apply
the rule above to reduce duplicate numbers. Use memoize approach to reduce number of recursinve calls with
same arguments.

Toral: 51161058134250

real    0m0.016s
user    0m0.009s
sys     0m0.007s

*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM 100
static int ind;
static long unsigned int *inc_m;
static long unsigned int *dec_m;

#define inc_ind(d,p) ((p)*10+(d))
#define dec_ind(d,p,n) (((((n)*MAX_NUM)+(p))*10)+(d))

long unsigned int inc (int digit, int pos) {
    int d;
    long unsigned int c = 0L;
    int indx;

    if (pos >= MAX_NUM) {
        return 1L;
    }

    indx = inc_ind(digit, pos);
    if (inc_m[indx] > 0)
        return inc_m[indx];

    for (d = digit; d < 10; d++) {
        c += inc(d, pos+1);
    }

    inc_m[indx] = c;

    return c;
}

long unsigned int dec (int digit, int pos, int n) {
    int d;
    long unsigned int c = 0L;
    int indx;

    if (pos >= n) {
        return 1L;
    }

    indx = dec_ind(digit, pos, n-1);
    if (dec_m[indx] > 0)
        return dec_m[indx];

    for (d = digit; d >= 0; d--) {
        c += dec(d, pos+1, n);
    }

    dec_m[indx] = c;

    return c;
}

int main (int argc, char **argv) {
    long unsigned int ci = 0L, cd = 0L, tc = 0L;

    if ((inc_m = malloc(10*MAX_NUM*sizeof(long unsigned int))) == NULL) {
        perror("Failed to allocate inc_m");
        exit (EXIT_FAILURE);
    }

    if ((dec_m = malloc(10*MAX_NUM*MAX_NUM*sizeof(long unsigned int))) == NULL) {
        perror("Failed to allocate dec_m");
        exit (EXIT_FAILURE);       
    }

    memset(inc_m, 0, 10*MAX_NUM*sizeof(long unsigned int));
    memset(dec_m, 0, 10*MAX_NUM*MAX_NUM*sizeof(long unsigned int));

    ind = 0;
    ci = inc(0, 0);
    tc += ci;

    ind = 0L;
    for (int i = 1; i <= MAX_NUM; i++) {
        cd += dec(9, 0, i);
        cd--;    // don't count 0, 00, 000...
    }

    tc += cd;

    tc = tc - 9*MAX_NUM - 1;
    printf("Toral: %lu\n", tc);

    free(inc_m);
    free(dec_m);

    return 0;
}
