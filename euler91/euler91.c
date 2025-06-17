/*
https://projecteuler.net/problem=91


Brute force approach. Total of 51^4 coordinate combinations

sum: 14234

real    0m0.080s
user    0m0.074s
sys     0m0.007s

*/
#include <stdio.h>
#include <stdbool.h>

#define LIMIT 50

// check if tiagle between O(0, 0), P(x1, y1) and Q(x2, y2) is a right angle triangle
bool is_RT (int x1, int y1, int x2, int y2) {
    int a2, b2, c2;
    int dx, dy;

    dx = x2-x1;
    dy = y2-y1;
    a2 = x2*x2+y2*y2;
    b2 = x1*x1+y1*y1;
    c2 = dx*dx+dy*dy;

    // make sure we don't have any 0 length triangle side
    return (a2 && b2 && c2 && ((c2 == (a2+b2)) || (b2 == (a2+c2)) || (a2 == (b2+c2))));
}

void main () {
    int x1, y1, x2, y2;
    int sum = 0;

    for (x1 = 0; x1 <= LIMIT; x1++)
        for (x2 = 0; x2 <= LIMIT; x2++)
            for (y1 = 0; y1 <= LIMIT; y1++)
                for (y2 = 0; y2 <= LIMIT; y2++) {
                    if (is_RT(x1, y1, x2, y2))
                        sum++;          
                }

    sum /= 2;   // in the above we are visiting same coordinate pair twice.
    printf ("sum: %d\n", sum);
}
