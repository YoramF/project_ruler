/*
    https://projecteuler.net/p,roblem=95

    Solution:
    A number can't be included in multiple amicable chains. So once we find a chain, we
    mark all the numbers in that cahin, so that we don't need to check them again.
    Mark the longest chain and its smallest element = that is the answer.

max_min_num: 14316, max_count: 28

real    2m12.309s
user    2m12.239s
sys     0m0.007s
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <prime.h>

#define DEF_DIV_C 100
#define DIV_C_INC 50
#define MAX_LIMIT  1000000


typedef struct {
    int num;
    int d_cnt;
    int free;
    int divisors[];
} P_DIV;

// check if num is in divisors
// since divisors array is not sorted, we need to parse all array
bool in_divisors(P_DIV *p, int num) {
    bool f = false;
    int i;

    for (i = 0; i < p->d_cnt; i++)
        if (p->divisors[i] == num) {
            f = true;
            break;
        }
    return f;
}

// add new divisor to P_DIV. if not enough space increase divisor[] size
void add_divisor(P_DIV **p, unsigned num) {
    if ((*p)->free == 0) {

        // rallocate additional RAM for proper divisor list
        if ((*p = realloc(*p, sizeof(P_DIV)+((*p)->d_cnt+1)*sizeof(int)+DIV_C_INC*sizeof(int))) == NULL) {
            fprintf(stderr, "Failed to reallocate RAM for P_DIV: %s\n", strerror(errno));
            exit (EXIT_FAILURE);
        }
        (*p)->free = DIV_C_INC;    
    }

    if (!in_divisors(*p, num)) {
        (*p)->divisors[(*p)->d_cnt] = num;
        (*p)->d_cnt++;
        (*p)->free--;
    }
}


// create new divisors list for a given numeber 
P_DIV *gen_div (int num, const unsigned *primes, unsigned p_size) {
    P_DIV *p;
    int i, d, r, cnt;

    // allocate initial proper divisor list
    if ((p = malloc(sizeof(P_DIV) + DEF_DIV_C*sizeof(int))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM for P_DIV: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    p->d_cnt = 1;
    p->free = DEF_DIV_C-1;
    p->num = num;
    p->divisors[0] = 1;         // 1 is always a divisor

    // insert all prime numbers smaller than num
    i = 0;
    while (primes[i] < num) {
        if (num % primes[i] == 0)
            add_divisor(&p, primes[i]);
        i++;
    }

    // now generate all possible proper divisors based on inserted primes
    i = 1;
    cnt = p->d_cnt;
    while (i < cnt) {
        r = i;
        while (r < p->d_cnt) {
            d = p->divisors[i] * p->divisors[r];
            if (d < num) {
                if (num % d == 0)
                    add_divisor(&p, d);
                r++;
            }
            else
                break;
        }
        i++;
    }

    return p;
}

// sumerize all divisors
int sum_div (P_DIV *p) {
    int s, i;

    s = 0;
    for (i = 0; i < p->d_cnt; i++)
        s += p->divisors[i];

    return s;
}

void main () {
    int num;
    P_DIV *p_s;
    int i, sum;
    unsigned *primes, p_size;
    int count, min_num, first_num;
    char nums [MAX_LIMIT+1];
    char in_chain_nums [MAX_LIMIT+1];
    int max_count = 0, max_first = MAX_LIMIT, max_min_num;
    int num_sum [MAX_LIMIT+1];

    // reset nums 
    memset(nums, 0, sizeof(nums));

    // reset num_sum
    memset(num_sum, 0, sizeof(num_sum));

    // 1000003 is next prime after 999983. we must have prime bigger than 1M
    primes = gen_prime(1000004, &p_size);

    first_num = 4;
    while (first_num <= MAX_LIMIT) {
        min_num = MAX_LIMIT;
        count = 0;
        num = first_num;

        // reset in_chain nums
        memset(in_chain_nums, 0, sizeof(in_chain_nums));

        while (true) {
            if (num_sum[num] == 0) {
                p_s = gen_div(num, primes, p_size);
                sum = sum_div(p_s);
                num_sum[num] = sum;
                free(p_s);
            }
            else
                sum = num_sum[num];

            // validate that new sun is valid
            if (sum <= MAX_LIMIT && in_chain_nums[sum] == 0 && nums[sum] == 0) {
                num = sum;
                if (min_num > num)
                    min_num = num;

                in_chain_nums[num] = 1;
                count++;
    
                // check if we got an amicable chain
                if (first_num == num) {
                    if (max_count < count) {
                        if (max_count < count)
                            max_count = count;

                        max_min_num = min_num;                           
                        max_first = first_num;
                    }

                    // mark all numbers in this chain - no need to check them again
                    // and they can be part of another amicable chain
                    for (i = 0; i <= MAX_LIMIT; i++)
                        nums[i] |= in_chain_nums[i];

                    break;
                }
            }
            else {
                break;
            }
        }

        // get next free number
        do {
            first_num++;
        } while (first_num <= MAX_LIMIT && nums[first_num] > 0);
    }

    printf("max_min_num: %d, max_count: %d\n", max_min_num, max_count);
    free(primes);
}





