/*

https://projecteuler.net/problem=112

The idea is just to implement the correct function to check if a number is a bounchy number or not.
The just iterate over all number and count how many of them are bounchy number.


total 1587000 bounchy numbers, 1571130, 99%

real	0m0.038s
user	0m0.036s
sys	    0m0.003s

*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool changed_direction(int a, int b) {
    return (a*b < 0);
} 

bool is_bouncy (unsigned int n) {
    int d1, d2;
    int direction;

    // all numbers below 100 are increasing numbers
    if (n < 100L)
        return false;

    d1 = (int)(n % 10L);
    n /= 10L;
    d2 = (int)(n % 10L);
    n /= 10L;

    direction = (d2 - d1);

    // loop over all remaining digits. if original direction remain unchanged than n is not bouchy number
    while (n) {
        d1 = d2;
        d2 = (int)(n % 10L);
        n /= 10L;
        if (direction == 0) {
            if (d1 != d2)
                direction = (d2 - d1);                            
            continue;
        }
        if (changed_direction(d2-d1, direction))
            return true;
    }

    return false;   
}

int main (int argc, char **argv) {
    unsigned int c = 0L, b = 0L;
    unsigned int d;
    char p = '%';

    while (true) {
        c++;

        if (is_bouncy(c)) {
            b++;
        }
        d = b*100/c;

        if (d >= 99 || c == 0xffffffff)
            break;
    }

    printf("\ntotal %u bounchy numbers, %u, %u%c\n", c, b, d, p);

    return 0;
}
