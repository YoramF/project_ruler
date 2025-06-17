/*
Consider quadratic Diophantine equations of the form:
x^2 - Dy^2 = 1
For example, when D=13, the minimal solution in x is 649^2 - 13 times 180^2 = 1.
It can be assumed that there are no solutions in positive  unsignedegers when D is square.
By finding minimal solutions in x for D = {2, 3, 5, 6, 7}, we obtain the following:

3^2 - 2 times 2^2 = 1
2^2 - 3 times 1^2 = 1
9^2 - 5 times 4^2 = 1
5^2 - 6 times 2^2 = 1
8^2 - 7 times 3^2 = 1

Hence, by considering minimal solutions in x for D <= 7, the largest x is obtained when D=5.
Find the value of D <= 1000 in minimal solutions of x for which the largest value of x is obtained.

solution:
x^2 - Dy^2 = 1 is a Pell's equation. i.e.:
using Continued Fraction Representation to find x & y.

answer:
for D=661, largest x=16421658242965910275055840472270471049

real    0m0.204s
user    0m0.091s
sys     0m0.076s

*/
#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SEQ 100     // max numbers in continued fraction sequence

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

// Pell function to check
bool Pell (mpz_t x,  mpz_t y,  int D) {
    mpz_t p1, p2, p3;
    bool b;

    mpz_init(p1);
    mpz_init(p2);
    mpz_init(p3);

    mpz_mul(p1, x, x);  // p1 = x^2
    mpz_mul(p2, y, y);  // p2 = y^2
    mpz_mul_si(p2, p2, D);  // p2 = D*p2
    mpz_sub(p3, p1, p2);    // p3 = p1-p2;
    b = mpz_cmp_si(p3, 1);  // check if p3 == 1

    mpz_clears(p1, p2, p3, NULL);

    return b == 0;
}

// generate continued fraction sequence for squar root of num
// return 1 if sequance has been created, 0 if not
int gen_seq (int max, unsigned num, unsigned seq[]) {
    int n, i, a, x, b, c, d, bn, cn, dn, b1, c1, d1, g, f, p;
    double rx;

    rx = sqrt(num);
    x = rx;
    if (x*x == num) return 0;

    b = 1;
    c = 1;
    d = 0;
    p = 0;

    while (true && (p < max)) {
        f = b*rx;
        a = ((f+d)/c);
        seq[p] = a;
        bn = b*c;
        cn = b*b*num - d*d - a*a*c*c + 2*a*c*d;
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

    // we got here because we found a repetition in the sequence or we reached max
    if (p < max) {
        i = p-1;    // number of numbers in single repetition
        n = p-i;
        while (p < max-1) {
            seq[++p] = seq[n++];
        }
    }

    return 1;
}

// generate fraction based on index into sequance numbers going backward
// assumption sequance has >= i elements
void gen_f (int i, int seq[], mpq_t *f) {
    mpq_t tf;

    mpq_init(tf);
    mpq_set_si(*f, seq[--i], 1);     // set f =  (seq[--i]/1)

    i--;
    while (i >= 0) {
        mpq_set_si(tf, seq[i], 1);     // set tf =  (seq[i]/1)
        mpq_inv(*f, *f);                  // f = 1/f
        mpq_add(*f, *f, tf);              // f = f+tf
        mpq_canonicalize(*f);            // reduce f
        i--;      
    }

    mpq_clear(tf);
}

 unsigned main () {
    unsigned seq[MAX_SEQ];
    unsigned i, D, lD;
    mpz_t y, x, X;  // big numbers
    mpq_t f;

    mpq_init(f);
    mpz_init(X);
    mpz_init(x);
    mpz_init(y);

    for (D = 2; D < 1000; D++) {
        if (gen_seq(MAX_SEQ, D, seq)) {
            i = 2;

            // assume that an integer solution for the given Pell equation
            // must be found in less than 50 iterations
            while (i < MAX_SEQ) {
                gen_f(i, seq, &f);  
                mpq_get_num(x, f);      // x = f numerator
                mpq_get_den(y, f);      // y = f denominator      
                if (Pell(x, y, D)) { 
                    break;
                }

                i++;                   
            }
            if (i < MAX_SEQ) {
                if (mpz_cmp(x, X) > 0) {
                    mpz_set(X, x);
                    lD = D;
                } 
            }
        }
    }

    printf("for D=%u, largest x=", lD);
    mpz_out_str(stdout, 10, X);
    printf("\n");

    mpq_clear(f);
    mpz_clears(X, x, y, NULL);
    return EXIT_SUCCESS;
}