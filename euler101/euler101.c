/*
https://projecteuler.net/problem=101

Solution: 
Using Cubic Sequences we need to first find the differences until they are constant.
For a power n sequence, the constant difference is found after n-1 iterations. 
Once we find it, we can generate the next term of the squander we found on the n-2 iteration. 
We go up with that until we reached the first sequence and generate the next term for that sequence.
This new term is the BOP we are looking for a given PO(k,n).

Summarize all BOPs for PO(k,n) for k = 1 .. 10 to get the answer:

sum BOPs: 37076114526

real    0m0.004s
user    0m0.002s
sys     0m0.002s

*/

#include <stdio.h>

#define MAX 11

long unsigned poly (int n) {
    long unsigned n2, n3, n4, n5, n6, n7, n8, n9, n10;
    n2 = n*n;
    n3 = n2*n;
    n4 = n3*n;
    n5 = n4*n;
    n6 = n5*n;
    n7 = n6*n;
    n8 = n7*n;
    n9 = n8*n;
    n10 = n9*n;

    return 1-n+n2-n3+n4-n5+n6-n7+n8-n9+n10;
}

// recursive function to find BOP for given k
long unsigned BOP (int k, long unsigned u[]) {
    long unsigned diff [k];
    int i;

    if (k == 1)
        return (u[1]);

    // find differences
    for (i = 1; i < k; i++)
        diff[i] = u[i+1]-u[i];

    return u[k]+BOP(k-1, diff);
}

void main () {
    int i;
    long unsigned u[MAX+1];
    long unsigned bops = 0;

    // calculate OP(11,n) as a reference for OP(k,n) where k <= 10
    for (i = 1; i <= MAX; i++)
        u[i] = poly(i);

    // sum all BOPs for 1 to k-1
    for (i = 1; i < MAX; i++)
        bops += BOP(i, u);

    printf("sum BOPs: %lu\n", bops);
}