#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#include "prime.h"
#include "prime_gmp.h"

long int max_r_ui = 0;

int main (int argc, char *argv[]) {
    long unsigned maxl, psizel, p;
    long unsigned *primesl;
    unsigned max, psize;
    unsigned *primes;
    time_t st, en;
    mpz_t mp;

    if (argc < 1)
        return 0;

    maxl = atoll(argv[1]);
    st = time(NULL);
    primesl = gen_prime_ll_r(50, maxl, &psizel);
    en = time(NULL);

    printf("size: %lu\n", psizel);
    if (primesl != NULL) {
        printf("time: %lu sec\n", (long unsigned)(en-st));
        printf("size: %lu, last: %lu\n", psizel, primesl[psizel-1]);
        for (int i = 0; i < psizel; i++)
            printf("%lu, ", primesl[i]);
        printf("\n");
    
    
        if (is_prime_ll_r(97, primesl, psizel))
            printf("97 is prime\n");
    
        if (is_prime_ll_r(2, primesl, psizel))
            printf("2 is prime\n");
    
    
        if (!is_prime_ll_r(1, primesl, psizel))
            printf("1 is not prime\n");
    
        free(primesl);
    }

    max = atol(argv[1]);
    st = time(NULL);
    primes = gen_prime_r(50, max, &psize);
    printf("size: %u\n", psize);
    if (primes != NULL) {
        for (int i = 0; i < psize; i++)
        printf("%u, ", primes[i]);
        en = time(NULL);

        printf("time: %lu sec\n", (long unsigned)(en-st));
        printf("size: %u, last: %u\n", psize, primes[psize-1]);

        free(primes);
    }


    p = atol(argv[2]);
    mpz_init_set_ui(mp, p);
    st = time(NULL);
    while (!is_prime(p))
        p--;
    en = time(NULL);
    printf("%lu is the closest prime to %s, time: %lu sec\n", p, argv[2], (long unsigned)(en-st));

    st = time(NULL);
    while (!is_prime_gmp(mp))
        mpz_sub_ui(mp, mp, 1);
    en = time(NULL);
    printf("%s is the closest prime to %s, time: %lu sec\n", mpz_get_str(NULL, 0, mp), argv[2], (long unsigned)(en-st));

    


    return 1;

}