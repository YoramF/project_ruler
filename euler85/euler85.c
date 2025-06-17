/*
https://projecteuler.net/problem=85

since number of rectangles in N x M and M x N is the same, we need to loop over
r = 1..M and c = r..M

total number of rectangles in a grid is the number of horizontal rectangles x vertical rectangles

closest area: 2772 = 77 x 36

real    0m0.221s
user    0m0.061s
sys     0m0.045s

*/

#include <stdio.h>

#define rec_c(n,l)     ((l)-(n)+1)

int count_rec (int raw, int col) {
    int nr, nc, c_count = 0, r_count = 0;

    for (nc = 1; nc <= col; nc++)
        c_count += rec_c(nc, col);

    for (nr = 1; nr <= raw; nr++)
        r_count += rec_c(nr, raw);

    return r_count * c_count;
}


int main () {
    int r, c, n, sr, sc;
    unsigned int recs, c_recs = 100000;

    for (r = 1; r <= 100; r++)
        for (c = r; c <= 100; c++) {
            recs = count_rec(r, c);

            if (recs >= 2000000)
                break;

            recs = 2000000-recs;
            if (recs < c_recs) {
                c_recs = recs;
                sr = r;
                sc = c;
            }
        }

    printf("closest area: %u = %d x %d\n", sr*sc, sc, sr);

    return 0;
}