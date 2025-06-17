/*
Solution:
    Maximum sum of the factorial for 999,999 is 2,177,280. Therefore it is safe to use unsigned
    to store sum of the factorial values
    generate an array of factorial sums each holds the count of starting numbers that generate
    this factoriral sum
    loop the factorial array and for each starting number generate its chain. once a chain
    as exactly 60 terms add to the overall count the number of starting numbers that was saved
    in the array

    solution:
Total factorial chains with 60 numbers is: 402

run time:
real    0m0.204s
user    0m0.000s
sys     0m0.045s  

*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sets.h"

#define S_NUM_ALLOC 2177280

typedef struct {
    unsigned int s_num;
    int  cnt;
} S_NUM;

static int FACTORIAL[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880}; 

// calculate sum of factorials for a given number
unsigned int s_fact (unsigned int n) {
    unsigned sum = 0;
  
    while (n) {
        sum += FACTORIAL[n % 10];
        n /= 10;
    }

    return sum;
}

int main () {
    unsigned int s_num, cnt, sum_f, c_num;
    int i;
    S_NUM *s_num_a = NULL;
    SET *set;

    if ((s_num_a = calloc(S_NUM_ALLOC, sizeof(S_NUM))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM, %s\n", strerror(errno));
        return 1;
    }

    // first generate the list of starting numbers that their factorial sum is the same
    for (s_num = 3; s_num < 1000000; s_num++) {
        sum_f = s_fact(s_num);
        if (s_num_a[sum_f].s_num == 0) {
            s_num_a[sum_f].s_num = s_num;
            s_num_a[sum_f].cnt = 1;
        }
        else
            s_num_a[sum_f].cnt++;
    }

    // now calculate chanins for each starting number in s_num_a and count how many have exactly 60
    // numbers.
    c_num = 0;

    // we know that max chain length is 60. allocate 10 more for safety
    set = set_create(70, sizeof(unsigned int));

    for (i = 0; i < S_NUM_ALLOC; i++) {
        if (s_num_a[i].s_num != 0) {
            sum_f = s_num_a[i].s_num;
            set_insert(set, &sum_f);
            cnt = 1;
            while (1) {
                sum_f = s_fact(sum_f);
                if (set_insert(set, &sum_f) == 1)
                    cnt++;
                else
                    break;
            }

            // check if we count exactly 60 numbers in chain
            if (cnt == 60) {
                c_num += s_num_a[i].cnt;
            }
         
            // clear set_s;
            set_clear(set);
        }
    }

    printf("Total factorial chains with 60 numbers is: %d\n", c_num);

    set_delete(set);
    free(s_num_a);

    return 0;
}