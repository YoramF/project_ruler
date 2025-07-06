/*
https://projecteuler.net/problem=116

To solve this problem we need to formulate a function F(N,M,K) that solve the following problem:
Calculate how many ways can we fit M sets containing K non distinct elements each, within N empty slots.
This is a classic combinatorial problem that can be solved using the "stars and bars" method, with a 
slight modification to account for the fixed size of each set and the adjacency requirement.
Restriction: N >= M*K otherwise F(M,N,K) == 0;
F() is a classic problem of arranging indistinguishable items. i.e it is a binomial coefficient of
(M+(N-M*k))C(M).

Once we defined F() we need to iterate over K (2..4) and M (1..N/K) and sum all results.

There are 20492570929 different ways to arrange the tiles in a row of fifty units

real    0m0.005s
user    0m0.002s
sys     0m0.003s

*/

#include <stdio.h>

// Helper function for binomial coefficient (nCr)
long unsigned int nCr(long unsigned int n, long unsigned int r) {
    long unsigned int result;

    if (r < 0 || r > n)
        return 0;
    if (r == 0 || r == n)
        return 1L;
    if (r > n/2L)
        r = n - r;

    // Calculate nCr efficiently (e.g., iterative or memoized)
    result = 1;
    for (long unsigned int i = 1L; i <= r; i++)
        result = result * (n - i + 1L) / i;

    return result;
}


int main () {
    long unsigned int n, m, k;
    long unsigned int sum = 0L, limit;

    n = 50L;

    for (k = 2L; k < 5L; k++) {
        limit = n / k;
        for (m = 1L; m <= limit; m++) {
            sum += nCr((n-m*k+m), m);
        }
    }

    printf("There are %lu different ways to arrange the tiles in a row of fifty units\n", sum);


    return 0;
}