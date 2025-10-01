/*
https://projecteuler.net/problem=120

Given A, we can prove that for some P (0 < P < A^2), ((A+1)^n + (A-1)^n)%(A^2) will never be equal to P?
Using binomial theorem, we can see that for any even n, ((A+1)^n + (A-1)^n)%(A^2) will always equal 2.
For even n, the formula can be simplified to (2*A*n)%A^2.
Or the set of possible values can be {2,2A,4A,6A,…,some multiple of A}.
So in order to find the max reminder for a given A is to simly find the max 2*n*A value that is less than A^2

Sum of Rmax: 333082500

real    0m0.013s
user    0m0.004s
sys     0m0.010s

*/

#include <stdio.h>

unsigned int max_r (unsigned int a) {
    unsigned int a2 = a * a;
    unsigned int r = 2, mr;
    int n = 2;

    while (r < a2) {
        mr = r;
        r = n * a;
        n += 2;
    }

    return mr;
}

int main () {
    int a;
    unsigned int sum = 0;

    for (a = 3; a <= 1000; a++) {
        sum += max_r(a);
    }

    printf("Sum of Rmax: %u\n", sum);

    return 0;
}