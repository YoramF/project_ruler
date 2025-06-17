/*
solution:
brute fource approach.

answer: 
n: 8319823, factors:8313928, n/t_f(n):1.000709

real    0m9.602s
user    0m0.060s
sys     0m0.061s

*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sets.h"
#include "prime.h"

#define MAX_DIGITS 10
#define MAX_PRIMES 10000000

// return TRUE if n2 is a permutation of n2
bool is_perm (unsigned long n1, unsigned long n2) {
    int i;
    int digits[MAX_DIGITS];

    if (n1 == n2)
        return false;

    // reset digits
    memset(digits, 0, sizeof(digits));

    // fill digits
    i = 0;
    while (n1) {
        digits[n1 % 10]++;
        n1 /= 10;
        i++;
    }

    // now check n2
    while (n2) {
        if (digits[n2 % 10] > 0) {
            digits[n2 % 10]--;
            i--;
        }

        n2 /= 10;   
    }

    return (i == 0);
}


void gen_prime_factors (unsigned long num, SET *s, unsigned long *primes, unsigned long p_size) {
    int i, r_sum;
    set_clear(s);

    if (is_prime(num, primes, p_size)) {
        set_insert(s, &num);
        return;
    }


    i = 0;
    r_sum = num;

    while (r_sum > 1 && primes[i] <= r_sum && i < p_size) {
        if (r_sum % primes[i] == 0) {
            set_insert(s, &primes[i]);
            do {
                r_sum /= primes[i];
            } while (r_sum % primes[i] == 0);

            if ((r_sum > 1) && is_prime(r_sum, primes, p_size)) {
                set_insert(s, &r_sum);
                return;
            }
        }
        i++;
    }
}

unsigned long totient_function (unsigned long n, SET *set) {
    unsigned long  *p;
    unsigned long long d; 

    S_GET_NEXT g_n;

    g_n = set_reset_get(set);

    d = n;
    while ((p = (unsigned long *)(set_get_next_e(&g_n))) != NULL) {
        d = (d*(*p-1))/(*p);
    }

    return (unsigned long)d;
}

int main () {
    unsigned long p, Ln, Ltf, ultf, p_size;
    SET *set;
    double long d, Md;
    unsigned long *primes;

    set = set_create(20, sizeof(p));
    primes = gen_prime(MAX_PRIMES, &p_size);

    Md = 10000000.0;
    Ln = 0;

    for (p = 2; p < 10000000; p++) {
        gen_prime_factors(p, set, primes, p_size);
        ultf = totient_function(p, set);
        if (is_perm(p, ultf)) {
            d = (double long)p/(double long)ultf;
            if (d < Md) {
                Ln = p;
                Md = d;
                Ltf = ultf;
            }
        }
    }

    printf("n: %ld, factors:%ld, n/t_f(n):%Lf\n", Ln, Ltf, Md);

    set_delete(set);
    return 0;
}
