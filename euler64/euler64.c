/**
 * Solution aproach:
 *  Using Surd types to find the infinit precision of squar root.
 *  floor((b*sqrt(n) + d)/c)
 *  Loop until a, b, c repeat themselves
 * 
 * Answer:
 * Odd Periods=1322
 *
 * real    0m0.374s
 * user    0m0.045s
 * sys     0m0.061s
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// #define MAX_CF 200
#define MAX_ROOT 10000

// find commong denominator of a, b
int gdc2 (int a, int b) {
    int x;

    if (b == 0)
        return (a);
    else {
        x = a % b;
        return gdc2(b, x);  
    }   
}

// find commong denominator of b,c,d
int gdc3 (int b, int c, int d) {
    int x = gdc2(c, d);
    return gdc2(b, x);
}

int main (int argc, char *argv[]) {
    int n, i, a, x, b, c, d, bn, cn, dn, b1, c1, d1, g, f, p;
    int op;
    double rx;

    op = 0;

    for (n = 2; n < MAX_ROOT; n++) {
        rx = sqrt(n);
        x = rx;
        if (x*x == n) continue;

        b = 1;
        c = 1;
        d = 0;
        p = 0;

        while (true) {
            f = b*rx;
            a = ((f+d)/c);
            bn = b*c;
            cn = b*b*n - d*d - a*a*c*c + 2*a*c*d;
            dn = a*c*c - c*d;
            g = gdc3(bn, cn, dn);

            b = bn/g;
            c = cn/g;
            d = dn/g;

            if (p == 0) {
                b1 = b;
                c1 = c;
                d1 = d;
            }
            else if (b1 == b && c1 == c && d1 == d) {
                break;
            }
            p++;
        }

        if (p % 2) op++;

    }

    printf("Odd Periods=%d\n", op);

    return 0;
}
