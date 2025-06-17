/*
https://projecteuler.net/problem=94

stat with brute force approach;

Sine we are looking for almost equilateral triangles that their parimeter do not
exceed 1B units, we can assume that the max length of any side is less than 1B. Therefore
we can use int32_t for sides, and triangle height.
For the triangle area to be integer, height and base/2 both must be integer.

To verify that the triangle is almost equilateral triangle, we just need to verify that
height^2 + (c/2)^2 = side^2

Since we are looking only for triangles that theit parimeter is <= 1B, we can check only triangles
with sides of len (1B/3)+1 becase any other side/base combination will have a parimeter
larger than 1B

sum of the perimeters: 518408346

real    0m4.897s
user    0m4.876s
sys     0m0.008s

*/

#include <stdio.h>
#include <stdbool.h>

// alculate triangle parimeter
unsigned t_par (unsigned side, unsigned base) {
    return 2*side+base;
}

// return true if the triangle with side and base is almost_equilateral_triangle
bool is_almost_equilateral_triangle (unsigned s, unsigned b, unsigned *h) {
    long unsigned h2, s2, b2, hh2;

    if (b % 2 == 0) {
        b /= 2;

        s2 = (long unsigned)s*(long unsigned)s;
        b2 = (long unsigned)b*(long unsigned)b;
        h2 = s2 - b2;

        hh2 = (long unsigned)(*h)*(long unsigned)(*h);
        while (hh2 < h2) {
            (*h)++;
            hh2 = (long unsigned)(*h)*(long unsigned)(*h);
        }

        (*h)--;     // to make sure that in next call to is_almost_equilateral_triangle() h will not be too big

        return hh2 == h2;

    }

    return false;
}

void main () {
    unsigned side, base, limit = 1000000000/3+1;
    long unsigned sum_of_parimeters = 0L;
    unsigned height = 1;

    // we can't have side == 1
    for (side = 2; side < limit; side++) {
        if (is_almost_equilateral_triangle (side, side-1, &height)) 
            sum_of_parimeters += (unsigned long)t_par(side, side-1);

        if (is_almost_equilateral_triangle (side, side+1, &height))
           sum_of_parimeters += (unsigned long)t_par(side, side+1);                 
    }

    printf("sum of the perimeters: %lu\n", sum_of_parimeters);

}