/*
https://projecteuler.net/problem=97

Solution approach: 
 do all arithmetics while keeping only the 10 last digits of eny result

last ten digits of this prime number: 8739992577

real    0m0.073s
user    0m0.069s
sys     0m0.004s

*/

#include <stdio.h>

#define TEN_DIGITS 10000000000

void main () {
    long unsigned digits = 1L, m;
    int i;

    // calculate 2^7830457
    for (i = 0; i < 7830457; i++) {
        digits *= 2;
        digits %= TEN_DIGITS;
    }

    // multiply the result by 28433
    m = digits;
    digits = 0L;
    for (i = 0; i < 28433; i++) {
        digits += m;
        digits %= TEN_DIGITS;
    }

    digits++;

    printf("last ten digits of this prime number: %lu\n", digits);
}