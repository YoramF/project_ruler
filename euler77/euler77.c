/*
Solution: similar to problem 31 with the following changes:
1) instead of using set of coins, use set of prime numbers
2) here we are looking for the first sum with 5000 Prime Summations

answer: 71

real    0m0.211s
user    0m0.046s
sys     0m0.075s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "prime.h"

#define SUM_LIMIT 5000
#define MAX_PRIME 70       // best guess 
#define MAX_VAL 72         // base on successful run without memoization

static int *mem [MAX_VAL];
static long unsigned int primes;
static long unsigned int *primes_a = NULL;

int sum_p (int val, int max) {
    int sum, c, v = val, prime;

    // if this sum_p(val, max) has been calculated, use saved result 
    if (mem[v][max] >= 0)
        return(mem[v][max]);

    prime = primes_a[max];

    // if we got to the smallest prime option, return 0 if value is not
    // equal to prime or is not a multiplication of prime
    if (max <= 0) {
        if (val % prime == 1) 
            return 0;
        else
            return 1;      
    }

    sum = 0;
    while (val >= 0) {
        sum += sum_p(val, max-1);
        val -= prime;
    }

    // save result for later use
    mem[v][max] = sum;

    return sum;
}

int main () {
    int sum, p, i, val;

    // allocate primes array
    if ((primes_a = gen_prime(MAX_PRIME, &primes)) == NULL) {
        fprintf(stderr, "failed to allocate primes array\n");
        exit (EXIT_FAILURE);
    }

    // now allocate memoization memory
    for (i = 0; i < MAX_VAL; i++) {
        if ((mem[i] = malloc((primes+1)*sizeof(int))) == NULL) {
            fprintf(stderr, "failed to allocate RAM: %s\n", strerror(errno));
            exit (EXIT_FAILURE);          
        }
        memset(mem[i], -1, (primes+1)*sizeof(int));
    }

    // val = primes_a[primes-1];
    val = 2;
    i = 0;
    sum = 0;

    while (sum < SUM_LIMIT) {
        sum = sum_p(val, primes-1);
        val++;
    }

    printf("answer: %d\n", val-1);

}