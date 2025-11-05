/*

https://projecteuler.net/problem=123

 This problem is based on problem 120.
 In this problem we need to find an n such then 2*Pn*n%Pn^2 is the least value to be greater than 10^10.
 n must be odd number since for even n, the value of the formula is always 2, so we can ignore that.
 

    Max reminder: 10001595590, n=21035

    real    0m0.285s
    user    0m0.256s
    sys     0m0.028s

*/


#include <stdio.h>
#include <stdlib.h>

#include <prime.h>

#define MAX_VAL 10000000000
#define MAX_PRIME 10000000

int main () {
    long unsigned int Pn;
    int n;
    long unsigned int *primes, lp, m_r, min_m_r;

    primes = gen_prime_ll(MAX_PRIME, &lp);

    m_r = 0L;
    n = 7037;

    // start with known value for 10^9 and check only odd n's
    while (m_r < MAX_VAL) {
        n += 2;
        m_r = (2 * primes[n-1] * n) % (primes[n-1] * primes[n-1]) ;
    }

    printf("Max reminder: %lu, n=%d\n", m_r, n);
    free(primes);

    return 0;
}