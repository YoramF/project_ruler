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

real    3m45.561s
user    0m0.015s 
sys     0m0.060s 

*/
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "big_numbers.h"

#define MAX_D 60    // set number of digits in big numbers
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
bool Pell (void *x,  void *y,  int D) {
    void *p1, *p2, *p3, *d;
    bool b;

    p1 = gen_number();
    p2 = gen_number();
    p3 = gen_number();
    d = gen_number();

    make_int_u(d, D);

    multiply(x, x, p1);
    multiply(y, y, p2);
    multiply(d, p2, p3);
    sub(p1, p3, p2);
    make_int_u(p1, 1);

    b = is_equal(p1, p2);

    release(p1);
    release(p2);
    release(p3);
    release(d);

    return b;
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
        while (p < max) {
            seq[++p] = seq[n++];
        }
    }

    return 1;
}

// generate fraction based on index into sequance numbers going backward
// assumption sequance has >= i elements
void gen_f (int i, int seq[], FRACTION *f) {
    FRACTION *tf;
    void *t;
    char seqs[11];      // max int length

    t = gen_number();
    tf = gen_fraction();

    make_int_u(f->dem, 1);
    make_int_u(f->num, seq[--i]);

    i--;
    while (i >= 0) {
        make_int_u(tf->dem, 1);
        make_int_u(tf->num, seq[i]);
        assign(f->num, t);
        assign(f->dem, f->num);
        assign(t, f->dem);
        add_f(tf, f, f);
        reduce_f(f); 
        i--;      
    }

    release(t);
    release_fr(tf);
}

 unsigned main () {
    FRACTION *f;
    unsigned i, D, lD;
    void *X, *x, *y;       // big numbers
    char X_[MAX_D+1];
    unsigned seq[MAX_SEQ];

    // this must be called before any call to other big_numbers functions !!!
    if (init_big_numbers(MAX_D) != MAX_D) {
        fprintf(stderr, "Failed to init big_numbers\n");
        return EXIT_FAILURE;
    }

    f = gen_fraction();
    X = gen_number();
    x = gen_number();
    y = gen_number();

    for (D = 2; D < 1000; D++) {
        if (gen_seq(MAX_SEQ, D, seq)) {
            i = 2;

            // assume that an integer solution for the given Pell equation
            // must be found in less than 50 iterations
            while (i < MAX_SEQ) {
                gen_f(i, seq, f);
                assign(f->num, x);
                assign(f->dem, y);
                if (Pell(x, y, D)) {
                    // number2str(X, X_);
                    // number2str(x, x_);
                    // number2str(y, y_);
                    // printf("++++>>%u: %s^2 + %u*%s^2 = 1, %s, %u\n", i, x_, D, y_, X_, lD); 
                    break;
                }

                i++;                   
            }
            if (i < MAX_SEQ) {
                if (is_bigger(x, X)) {
                    assign(x, X);
                    lD = D;
                }
                // number2str(X, X_);
                // number2str(x, x_);
                // number2str(y, y_);
                // printf("%u: %s^2 + %u*%s^2 = 1, %s, %u\n", i, x_, D, y_, X_, lD);    
            }
        }
    }

    number2str(X, X_);
    printf("for D=%u, largest x=%s\n", lD, X_);

    release_fr(f);
    release(X);
    release(x);
    release(y);

    return EXIT_SUCCESS;
}