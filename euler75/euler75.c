/**
 * Solution approach:
 * calculate Pythagorean triples (a, b, c) based on Euclid's formula:
 * a = k*(m^2-n^2), b = k*2*m*n, c = k*(m^2+n^2), where m, n, and k are positive integers
 * with m > n, and with m and n coprime and not both odd.
 * Since we are looking for L=a+b+c where L < 1,500,000, we should check m = 2..(sqrt(1,500,000))+1
 * 
 * Answer: 
 * Given that L is the length of the wire, there are exactly 161667 values of L <= 1,500,000 that exactly one integer sided right angle triangle be formed
 * 
 * real    0m0.280s
 * user    0m0.000s
 * sys     0m0.045s
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_L   1500000

typedef struct {
    int a;
    int b;
    int c;
    int L;
} TRIPLE;

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
    int m, n, L, i, sum, t;
    int a, b, c, k;
    TRIPLE *tr;

    if ((tr = calloc(MAX_L+1, sizeof(TRIPLE))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        return 1;
    }

    for (m = 2; m < sqrt(1500000); m++) {
        for (n = 1; n < m; n++) {
            if (gdc(n, m) == 1) {

                // check if both m & n are odd
                if (n % 2 && m % 2) {
                    a = m*n;
                    b = (m*m - n*n) / 2;
                    c = (m*m + n*n) / 2;
                }
                else {
                    a = m*m - n*n;
                    b = 2*m*n;
                    c = m*m + n*n;                    
                }

                if (a > b) {
                    t = a;
                    a = b;
                    b = t;
                }

                // now that we have the a primitive Pythagorean triple (a, b, c)
                // we need to generate all K*triple permutations
                // loop unil L > MAX_L
                for (k = 1; ;k++) {
                    L = k * (a + b + c);
                    if (L <= MAX_L) {
                        if (tr[L].L == 0) {
                            tr[L].a = a;
                            tr[L].b = b;
                            tr[L].c = c;
                            tr[L].L = 1;
                        }
                        // check that the generated triple is different than the one we saved
                        else if (tr[L].L == 1) {
                            if (tr[L].a != a || tr[L].b != b || tr[L].c != c)
                                // different triple with same L value.
                                // mark this L value as invalid value
                                tr[L].L = -1;
                        }
                    }
                    else
                        break;
                }
            }
        }
    }

    // count all unique L values
    sum = 0;
    for (i = 0; i <= MAX_L; i++) {
        if (tr[i].L > 0)
            sum++;
    }

    free(tr);

    printf("Given that L is the length of the wire, there are exactly %d values of L <= 1,500,000 that exactly one integer sided right angle triangle be formed\n", sum);

    return 0;
}