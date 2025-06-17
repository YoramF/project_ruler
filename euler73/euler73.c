/*
Between 1/3 to 1/2, there are 7295372 fractions

real    0m7.507s
user    0m0.031s
sys     0m0.015s

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_DENOMINATOR 12000

// find commong denominator of a, b
int gdc (int a, int b) {
    int x;

    x = a % b;
    while (x != 0) {
        a = b;
        b = x;
        x = a % b;
    }
    return b;
}

int main () {
    int n, d, f;
    long double h, l, ld;

    h = (long double)1/(long double)2;
    l = (long double)1/(long double)3;

    f = 0;
    for (d = 2; d <= MAX_DENOMINATOR; d++) {
        for (n = 1; n < d; n++) {
            if (gdc(n, d) == 1) {
                ld = (long double)n/(long double)d;
                if (ld > l && ld < h)
                    f++;
            }
        }
    }

    printf("Between 1/3 to 1/2, there are %d fractions\n", f);


    return 0;
}