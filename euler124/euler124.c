/*
https://projecteuler.net/problem=124


Naive solution approach: 
    1) Write function rad(n) that produces product of the distinct prime factors of n
    2) Since we need to sort 100,000 values, store the values sorted with their n parameter
    3) E(k) is the value n stored in the rad() sorted array

E(10000)=21417

real    0m0.601s
user    0m0.592s
sys     0m0.006s

*/

#include <stdio.h>
#include <stdlib.h>

#include <prime.h>

#define LIMIT 100000

typedef struct {
    int n;
    unsigned rad;
} RAD;

unsigned *Primes, Lp; 

/*
    Calculate the RAD value of n
*/
unsigned rad (int n) {
    unsigned r = 1;
    unsigned p, t;
    int i = 0;

    while (n > 1) {
        p = Primes[i];
        if (n % p == 0)
            r *= p;
        while ((n % p) == 0)
            n /= p;
        i++;
    }

    return r;
}

/*
    Sort funtion to be called by qsort() when sorting RADs array
*/
int rad_sort_f (const void *a, const void *b) {
    RAD *ap, *bp;

    ap = (RAD *)a;
    bp = (RAD *)b;

    return (ap->rad - bp->rad);
}

int main () {
    RAD *rad_a;
    int n;
    unsigned r;

    // allocate primes array
    if ((Primes = gen_prime(LIMIT, &Lp)) == NULL) {
        perror("Failed to allocate prime numbers");
        exit(EXIT_FAILURE);
    }

    // allocate RAD array
    if ((rad_a = malloc(LIMIT*sizeof(RAD))) == NULL) {
        perror("Failed to allocate RAM for EAD array");
        exit(EXIT_FAILURE);
    }

    // generate RADs
    for (n = 0; n < LIMIT; n++) {
        r = rad(n+1);
        rad_a[n].n = n+1;
        rad_a[n].rad = r;
    }

    // sort rad_a
    qsort(rad_a, LIMIT, sizeof(RAD), rad_sort_f);

    printf("E(10000)=%d\n", rad_a[9999].n);

    free(Primes);
    free(rad_a);
    return 0;
}