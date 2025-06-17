/*
https://projecteuler.net/problem=111

    Brute force approach of generating all 10 digits primes and check each one and calculate
    M(n,d), N(n,d) and S(n,d) is not practical as it will take forever.
    A better approach would be generate combinations of numbers starting with 9 repetitions of
    same digit and if we find one which is a prime, we can stop searching other combination with
    that digit as the repetition digit. If we do't find any prime for that digit, we will seach
    combinations with less repetitions until we get to 1 repetitions of same digit.
    Do that for each digit from 0..9
    I am usgin sets() to make sure I am not counting same prime number twice. 

sum: 612407567715

real    0m0.147s
user    0m0.143s
sys     0m0.004s

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sets.h>


#define DIGITS 10
#define MIN_NUM 1000000000

typedef struct _PrimeNum {
    struct _PrimeNum *next;
    long unsigned int num;
} PrimeNum;

typedef struct {
    int M;                  // M(n,d)
    int N;                  // N(n,d)
    long unsigned int S;    // S(n,d)
    #ifdef DEBUG
    PrimeNum *p_list;       // list of primes comply with (n,d)
    #endif
} MNS;

// stracture to track sequances
typedef struct {
    char *d;        // array of digits to form new combination
    int r;          // number of digits to choose from
    int l;          // output string length
    int ind[0];     // indexes
} SEQ;

// initialize a sequance
SEQ *init_seq (char *digits, int r, int l) {
    SEQ *seq;

    if (r <= 0 || l <= 0)
        return NULL;

    if ((seq = malloc(sizeof(SEQ)+l*sizeof(int))) == NULL) {
        perror("Failed to allocate RAM for sequances stracture");
        exit (EXIT_FAILURE);
    }

    seq->d = digits;
    seq->l = l;
    seq->r = r;

    for (int i = 0; i < l; i++)
        seq->ind[i] = 0;
    seq->ind[l-1] = -1;       // to allow first call to net_num to have all indexes set to 0

    return seq;
}

// return next sequance
char *next_seq (SEQ *seq) {
    char *s;
    int i;

    if (seq == NULL)
        return NULL;

    if ((s = malloc(seq->l+1)) == NULL) {
        perror("Failed to allocate RAM for next num");
        exit (EXIT_FAILURE);
    }

    // update indexes
    bool done = false;
    i = seq->l-1;
    while (!done && i >= 0) {
        seq->ind[i]++;
        if (seq->ind[i] >= seq->r) {
            seq->ind[i] = 0;
            i--;
        }
        else
            done = true;
    }
    if (!done) {
        free(s);
        return NULL;
    }

    memset(s, 0, seq->l+1);
    for (i = 0; i < seq->l; i++) {
        s[i] = seq->d[seq->ind[i]];
    }

    return s;
}

#ifdef DEBUG
// add new prime number to list;
PrimeNum *add_prime (PrimeNum *p_ptr, long unsigned int p_n) {
    PrimeNum *np_ptr;

    if ((np_ptr = malloc(sizeof(PrimeNum))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM for new Prime: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    np_ptr->next = p_ptr;
    np_ptr->num = p_n;
    return np_ptr;
}

// delete list of prime numbers
PrimeNum *dell_primes (PrimeNum *p_ptr) {
    PrimeNum *next_p_ptr;

    while (p_ptr != NULL) {
        next_p_ptr = p_ptr->next;
        free(p_ptr);
        p_ptr = next_p_ptr;
    }

    return p_ptr;
}

// print prime numbers in list
void print_primes (PrimeNum *p_list) {
    while (p_list != NULL) {
        printf("%lu\n", p_list->num);
        p_list = p_list->next;
    }
}
#endif

int isPrime (long unsigned int n) {
    long unsigned int i, m;

    if (!(n & 1))
        return (int)(n == 2);

    for (i = 3; i*i < n; i++) {
        m = n % i;
        if (m == 0L)
            return 0;
    }

    return 1;
}

void mark_MNS (MNS *mns, int d, long unsigned int p) {
    mns[d].N++;
    mns[d].S += p;
    #ifdef DEBUG
    mns[d].p_list = add_prime(mns[d].p_list, p);
    #endif
}


// Helper function to swap two chars
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Helper function to reverse a subarray
void reverse(char *arr, int start, int end) {
    while (start < end) {
        swap(&arr[start], &arr[end]);
        start++;
        end--;
    }
}

// Comparison function for qsort
int comp_f(const void* a, const void* b) {
    return (*(char *)b - *(char *)a);
}

// Function to find the next lexicographical permutation of an array
// Returns true if a next permutation was found, false otherwise (current is the last)
bool next_permutation(char *arr, int n) {
    // 1. Find the largest index k such that a[k] > a[k+1]
    int k = -1;
    for (int i = n - 2; i >= 0; i--) {
        if (arr[i] > arr[i + 1]) {
            k = i;
            break;
        }
    }

    // If no such index exists, the permutation is the last permutation
    if (k == -1) {
        return false;
    }

    // 2. Find the largest index l > k such that a[k] > a[l]
    int l = -1;
    for (int i = n - 1; i > k; i--) {
        if (arr[k] > arr[i]) {
            l = i;
            break;
        }
    }

    // 3. Swap a[k] and a[l]
    swap(&arr[k], &arr[l]);

    // 4. Reverse the subsequence from a[k+1] up to the end
    reverse(arr, k + 1, n - 1);

    return true;
}

// set array for 9 digits from 0..9 missing one digit d
// we will use this array to create combinations of digits to add to the repeated digit d
void init_digits(int d, char *digits) {
    int i, j = 0;
    for (i = 0; i < 10; i++)
        if (i != d)
            digits[j++] = (char)i+'0';
}

int main () {
    unsigned int s1, i, d, r;
    long unsigned int ld, p;
    MNS mns [10];
    char s_num [DIGITS+1];
    char r_digits [DIGITS+1];
    char *seq;
    char digits [9];
    int b;
    SEQ *seq_s;
    SET *set;

    // init mns
    memset(&mns, 0, sizeof(mns));

    ld = 0L;
    set = set_create(100, sizeof(long int));

    // main loop over all digits
    for (d = 0; d < 10; d++) {

        // inside loop starting from DIGITS-1 repetitions for same digit down to 1 repetition
        for (r = DIGITS-1, b = 0; r > 0 && b == 0; r--) {
            // init digits and s_num to generate all combinations
            mns[d].M = r;
            mns[d].N = 0;
            mns[d].S = 0;
            #ifdef DEBUG
            if (mns[d].p_list != NULL)
                mns[d].p_list = dell_primes(mns[d].p_list);
            #endif        

            init_digits(d, digits);
            memset(r_digits, 0, sizeof(r_digits));
            for (i = 0; i < r; i++)
                r_digits[i] = d+'0';

            // init sequance stracture
            seq_s = init_seq(digits, sizeof(digits), DIGITS-r);

            // generate all combinations and per combination generate all permutations
            while ((seq = next_seq(seq_s)) != NULL) {
                strcpy(s_num, r_digits);
                strcat(s_num, seq);
                free(seq);

                // next_permutation() requires that the input arry (string) will be
                // sorted in reverse order before calling it first.
                qsort(s_num, DIGITS, sizeof(char), &comp_f);                
                do {
                    p = atoll(s_num);

                    // check only numbers with value in range of 10 digits 
                    if (p >= MIN_NUM && isPrime(p)) {

                        // make sure we don't count same prime twice
                        if (set_insert(set, &p)) {
                            b++;
                            mark_MNS(mns, d, atoll(s_num));
                        }
                    }            
                 } while (next_permutation(s_num, DIGITS));
            }
            free(seq_s);

            // clear set for next iteration
            set_clear(set);
        }
    }

    set_delete(set);

    for (i = 0; i < 10; i++) {
        ld += mns[i].S;
        #ifdef DEBUG
        printf("[%d] M:%d, N:%d, S:%ld\n", i, mns[i].M, mns[i].N, mns[i].S);
        print_primes(mns[i].p_list);
        printf("---------------\n");
        dell_primes(mns[i].p_list);
        #endif
    }

    printf("sum: %lu\n", ld);

    return 0;  
}