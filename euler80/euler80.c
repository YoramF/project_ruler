/*
use GMP library to handle large floating point numbers
for the sum calculation we can use standard long int type since the largest possible sum is
100 * 100 * 9, that is first 100 natural numbers (no all of them have irrational square root)
 * 100 digits of value 9. which is a value less than 90,000. 

 total of the digital sums: 40886

real    0m0.189s
user    0m0.045s
sys     0m0.030s
*/

#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <string.h>

#define MAX_NUMBER 100
#define MAX_DIGITS 100
#define N_DIGITS   103

int sum_m (char *f_r) {
    int i, sum = 0;

    for (i = 0; i < MAX_DIGITS; i++) {
        sum += (*f_r)-'0';
        f_r++;
    }

    return sum;
}


int main () {
    int d_sums = 0;
    int i;
    unsigned long num, sqr;
    char f_r [N_DIGITS+2];        // folowing mpf_get_str() instruction for n_digits

    // gmp variables
    mpf_t sq_r, f_num;
    mp_exp_t exp;

    // init mpf variables with enough persision
    mpf_init2(sq_r, 512);
    mpf_init2(f_num, 512);

    for (num = 1; num <= 100; num++) {
        sqr = sqrt(num);
        if (sqr*sqr == num)
            continue;

        mpf_set_ui (f_num, num);
        mpf_sqrt (sq_r, f_num);
        mpf_get_str (f_r, &exp, 10, N_DIGITS, sq_r);

        d_sums += sum_m(f_r);
    }

    printf("total of the digital sums: %d\n", d_sums);

    mpf_clear(sq_r);
    mpf_clear(f_num);

    return 0;
}
