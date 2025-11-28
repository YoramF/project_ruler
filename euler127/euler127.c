/*
https://projecteuler.net/problem=127


This is the "abc Conjecture". The only way to generate the triple a, b, c, is using brute-force.
Guidelines:
1) Loop on c = 1 .. 120,000
2) based on c calculate a & b
3) rad(abc) == rad(a)*rad(b)*rad(c) - it is faster to calculate
4) check that gdc(a,b) == 1, then check that gdc(a,c) == 1, and finally check that gdc(b,c) == 1.
This will reduce the overall loops before checking the abc-hi()

c Upper Bound,Total abc-hits (rad(abc)<c)
c<100, 6
c<1000, 31
c<10000, 120
c<100000, 418
c<1000000, 1,268

So for our case the number of triples satisfies the requirements will be few hundreds and the sum of
tese c's shuld fit one integer.

Solution:

sum_c: 18407904

real    15m25.091s
user    15m23.913s
sys     0m0.078s
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <prime.h>

#define MAX_C 120000

static unsigned int *Primes, Lp;
static unsigned int *Rad;



/*
    Return true if a, b & c are an abc-hit 
*/
bool is_abc_hit (int a, int b, int c, long unsigned int v) {
    return ((a + b == c) && (a < b) && (v < (long unsigned int)c));
}

/*
    Calculate the RAD value of n, if result is bigger than max return 0
    use memoization
*/
int rad (int n) {
    int r = 1;
    int p;
    int i = 0;
    int sn = n;

    if (Rad[n] > 0)
        return Rad[n];

    while (n > 1) {
        p = Primes[i];
        if (n % p == 0)
            r *= p;
        while ((n % p) == 0)
            n /= p;
        i++;
    }

    Rad[sn] = r;
    return r;
}

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
    int r, c, a, b;
    int n;
    int sum_c;
    int rad_a, rad_b, rad_c;
    long unsigned int r_a_d;

    // for the rad() function to complete successful for any given c' value, 
    // allocate extra primes to make sure we have at least one prime bigger than largest possible c' value 
    Primes = gen_prime(MAX_C+20, &Lp);
    printf("max_prime: %u\n", Primes[Lp-1]);

    // alocate RAM
    if ((Rad = calloc(MAX_C+1, sizeof(int))) == NULL) {
        perror("Failed to allocate RAM for rad values");
        exit(EXIT_FAILURE);
    }

    sum_c = 0;
    n = 0;
    for (c = 3; c < MAX_C; c++) {
        b = c-1;
        a = 1;
        rad_c = 0;

        // the following loop makes sure we are testing all (1/4)*MAX_C^2 possibilities for 
        // (1 <= a < b), (a + b == c).
        for (; a < b; b--) {
            a = c - b;

            // optimization: advance per gdc(z,y) == 1
            if (gdc(a,b) == 1) {
                if (gdc(a,c) == 1) {
                    if (gdc(b,c) == 1) {

                        // the following lines are possible since gdc(a,b) == gdc(a,c) == gdc(b,c) == 1
                        // otherwise I can't assume that rad(a)*rad(b)*rad(c) == rad(a*b*c)
                        rad_c = rad(c);
                        rad_a = rad(a);
                        rad_b = rad(b);

                        // rad(abc) can have values bigger than 2^31
                        r_a_d = (long unsigned int)rad_a * (long unsigned int)rad_b * (long unsigned int)rad_c; // == rad(a*b*c)

                        if (is_abc_hit (a, b, c, r_a_d)) {
                            sum_c += c;                     
                        }
                    }
                }
            }
        }
    }

    printf("\nsum_c: %d\n", sum_c);

    free(Rad);
    free(Primes);
    return 0;
}



