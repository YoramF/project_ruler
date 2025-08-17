/*
https://projecteuler.net/problem=119

The approach should be looping over the sums and findigs their powers.
Keep the firsr smallest results
Stop the loop when we have 30 resuls and the new checked sum filed to be inserted into
the 30 smallest results array on its first valid sum/power combination
Main assumption is that the result is less that long unsigned (64 bit)!!

Answer:
a30 = 248155780267521

real    0m0.003s
user    0m0.000s
sys     0m0.003s

*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUM             4294934527UL     // square root of max long unsigned
#define MAX_LOMG_UNSIGNED   18446744073709551615UL

typedef struct { 
    unsigned int sum;
    int power;
    long unsigned num;
} SUM_P;

long unsigned sum_d (long unsigned n) {
    long unsigned s = 0;
    long unsigned d;

    while (n) {
        d = n % 10;
        s += d;
        n /= 10;
    }

    return s;
}

long int new_num (SUM_P *sum_p) {
    int p = sum_p->power;
    unsigned int sum = sum_p->sum;
    long unsigned n_num = sum_p->num, t_num;
    bool found_num = false;
    
    do {
        t_num = n_num;
        n_num *= sum;
        p++;

        if (sum_d(n_num) == sum) {
            found_num = true;
            break;
        }

    } while (n_num > t_num);

    if (found_num) {
        sum_p->power = p;
        sum_p->num = n_num;
    }
    else {
        sum_p->num = 0;
        n_num = 0;
    }

    return n_num;
}

void init_dps (SUM_P *dps[], int n) {
    int i;

    for (i = 0; i < n; i++) {
        if ((dps[i] = calloc(1, sizeof(SUM_P))) == NULL) {
            perror("Failed to allocate RAM:");
            exit (EXIT_FAILURE);
        }
    }
}

void free_dps (SUM_P *dps[], int n) {
    int i;
    for (i = 0; i < n; i++)
        free(dps[i]);
}

int insert_sum_p (SUM_P *dps[], unsigned int sum, int power, long unsigned num, int max, int loc) {
    SUM_P *t;
    int i;

    // find the proper location for the new record.
    // the right location is where the previous record as smaller num value and the next record has larger
    // num value, or you reached end of records array
    // start looking for loc
    while (loc < max && dps[loc]->num <= num)
        loc++;

    // insert bi
    // if loc passed end of array than there return -1
    if (loc == max)
        return -1;

    // save last record
    t = dps[max-1];

    // shift up all records after loc
    for (i = max-1; i > loc; i--)
        dps[i] = dps[i-1];

    // insert new record
    dps[loc] = t;
    dps[loc]->sum = sum;
    dps[loc]->power = power;
    dps[loc]->num = num;

    return loc;
}

int main () {
    long unsigned num, t_num;
    SUM_P *dps [30];
    int i, n, p, j, r;
    bool done = false, first_check;
    SUM_P dps_t;
    unsigned int s;

    init_dps(dps, 30);

    // insert bigest record possible
    dps[0]->sum = MAX_SUM;
    dps[0]->power = 2;
    dps[0]->num = MAX_LOMG_UNSIGNED;

    // start with smallest sum/power combination possible
    s = 2;

    while (!done) {
        num = s;
        p = 1;
        n = 0;
        first_check = true;

        // fill all possile e)ntries for a given sun = s
        do {
            dps_t.num = num;
            dps_t.power = p;
            dps_t.sum = s;
            if (new_num(&dps_t)) {
                p = dps_t.power;
                t_num = num;
                num = dps_t.num;
                r = insert_sum_p(dps, dps_t.sum, dps_t.power, dps_t.num, 30, n);

                // reached end of entry options for a given valid sum
                if (r < 0) {

                    // if we can't insert new valid record for a given sum for
                    // the first check, it means we filled the first 30 elements in the array
                    // we can escape the outer loop!
                    if (first_check)
                        done = true;

                    break;
                }

                // make sure next insert will start from current location
                n = r;
                first_check = false;
            }
        } while (num > t_num && dps_t.num > 0);

        s++;
    }

    // for (i = 0; i < 30; i++)
    //     printf("[%d] sum: %u, power: %d, num: %lu\n", i, dps[i]->sum, dps[i]->power, dps[i]->num);    
    printf("a30 = %lu\n", dps[29]->num);

    free_dps(dps, 30);

    return 0;
 }