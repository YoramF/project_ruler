/*
https://projecteuler.net/problem=87

Solution approach: guess the largest prime number that can be part of the combination. It can't
be larger than sqrt(50,000,000). To make sure we are not counting same number (different combination)
I am using "sets" library

There are 1097343 numbers bellow 50000000

real    0m1.001s
user    0m0.045s
sys     0m0.045s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <prime.h>
#include <sets.h>


#define MAX_4 85        // MAX_4 ^ 4 > 50,000,000
#define MAX_3 369       // MAX_3 ^ 3 > 50,000,000
#define MAX_2 7072      // MAX_2 ^ 2 > 50,000,000
#define LIMIT 50000000

int main () {
    long unsigned *primes;
    long unsigned p_num;
    long unsigned *p2, *p3, *p4;
    long unsigned count = 0, p;
    SET *set;
    int s;

    if ((primes = gen_prime(MAX_2, &p_num)) == NULL) {
        fprintf(stderr, "Failed to allocate prime numbers\n");
        return EXIT_FAILURE;
    }

    if ((set = set_create(500000, sizeof(long))) == NULL) {
        fprintf(stderr, "Failed to create set\n");
        return EXIT_FAILURE;        
    }

    // allocations for p2, p3, p4 are much more than really needed but we don't care
    if ((p2 = malloc(MAX_2*sizeof(long))) == NULL) {
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if ((p3 = malloc(MAX_3*sizeof(long))) == NULL) {
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if ((p4 = malloc(MAX_4*sizeof(long))) == NULL) {
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // fill the 3 primes arrays
    for (int i = 0; i < p_num; i++) {
        p = primes[i]*primes[i];
        p2[i] = p;
        if (primes[i] <= MAX_3) {
            p *= primes[i];
            p3[i] = p;
            if (primes[i] <= MAX_4) {
                p *= primes[i];
                p4[i] = p;
            }
        }
    }

    // run over all combinations
    for (int i = 0; i < p_num; i++) {
        for (int j = 0; primes[j] <= MAX_3; j++) {
            for (int k = 0; primes[k] <= MAX_4; k++) {
                p = p2[i] + p3[j] + p4[k];
                if (p <= LIMIT) {
                    // insert new p into set. add to count only if p is not in set
                    if ((s = set_insert(set, &p)) == 1)
                        count++;
                    else if (s < 0) {
                        fprintf(stderr, "Failed to insert %u into set\n", p);
                    }
                }
            }
        }
    }

    printf("There are %u numbers bellow %d\n", count, LIMIT);

    free(primes);
    free(p2);
    free(p3);
    free(p4);
    set_clear(set);

    return EXIT_SUCCESS;
}