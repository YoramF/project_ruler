/*
https://projecteuler.net/problem=125

Solution approach: Generate an 10^4 integers array for n^2 (n = 1..10^4).
No need to check n greater than n^4 since we are looking for numbers less than 10^8.
Once we have this array ready, just scan it for consecutive numbers and check if the sum generates a
palindromic number. If it does, add that number to the overall sum.

sum: 2906969179, c: 166

real    0m0.026s
user    0m0.025s
sys     0m0.001s

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sets.h>

#define MAX_NUMBERS 10000
#define MAX_VAL     100000000

/*
    Check if given number is a palindromic number
*/
bool is_palindromic_number (int n) {
    int m, t, c;

    if (n < 1 || n >= MAX_VAL)
        return false;

    m = 0;
    t = n;

    while (t) {
        m = (m * 10) + (t % 10);
        t /= 10;
    }

    return m == n;
}

/*
    Check if we can generate a palindromic number from squares in numbers array starting from next.
    The result must include at leasr two consecutive squares. Follow up on start and d to allow
    generating two different palindromic number with overlapping squares.
*/
unsigned int check_for_palindromic_number (int *next, unsigned int sum, int *d, int numbers[]) {
    unsigned int num = sum;
    int n = *next;

    while (n < MAX_NUMBERS && num < MAX_VAL) {
        num += (unsigned int)numbers[n];
        (*d)++;
        if (is_palindromic_number(num) && *d > 1) {
            *next = n+1;
            return num;
        }

        n++;
    }
    return 0L;
}

int main () {
    int n, start, c;
    unsigned int sum = 0L, t;
    int *numbers, d;
    SET *set;

    if ((numbers = malloc(MAX_NUMBERS*sizeof(int))) == NULL) {
        perror("Failed to allocate RAM for numbers");
        exit(EXIT_FAILURE);
    }

    if ((set = set_create(MAX_NUMBERS, sizeof(unsigned int))) == NULL) {
        fprintf(stderr, "Failed to allocate SET\n");
        exit(EXIT_FAILURE);
    }

    // init numbers
    for (n = 1; n <= MAX_NUMBERS; n++)
        numbers[n-1] = n*n;

    c = 0;
    for (n = 0; n < MAX_NUMBERS; n++) {
        start = n;
        d = 0;
        t = 0L;
        while ((t = check_for_palindromic_number(&start, t, &d, numbers)) > 0) {
            // check that we are not counting same palindromic number twice !!
            if (set_insert(set, &t) == 1) {
                c++;
                sum += t;
            }
        }
    }

    printf("sum: %u, c: %d\n", sum, c);

    set_clear(set);
    free(numbers);

    return 0;
}
