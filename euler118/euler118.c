/*
https://projecteuler.net/problem=118

Solution approach: one option is to generate all primes up to 10^9 and work with these numbers to create the
Pandigital Prime Sets. The issue with this approach is that it takes too long to create 50,847,534 primes and then
eliminate all the numbers that include same digit more than once, and form that set of prime numbers to find
all sets that meets requirements.
Another approach is to generate all 1-9 digits numbers permutations and select the primes ones. This is much 
faster as there are only 986,409 (sum of binomial(9|r)*factorial(r) where r=1..9) options of such numbers to 
check for prime. 
The sets will be constracted only from thoese digits combinations that have at least one permutation that is
a prime number. Once we find such a set, we need to have a Cartesian product of all possible prime permutations
per digits in the set.

Solution:
Pandigital Prime Sets: 44680

real    0m0.950s
user    0m0.944s
sys     0m0.005s


*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
    int d;
    int perm;
} DIGITS;

typedef struct {
    int cnt;
    int p_cnt;
    DIGITS *digits;
} PRIME_NUM;


bool isPrime (int n) {
    int i, m;

    if (n < 2)
        return false;

    if (!(n & 1))
        return (n == 2);

    for (i = 3; i*i <= n; i++) {
        m = n % i;
        if (m == 0)
            return false;
    }

    return true;
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
                int cr = 1;

int gen_int (int ar[], int len) {
    int r = 0;

    for (int i = 0; i < len; i++)
        r = r * 10 + ar[i];
    
    return r;
}

int gen_arr (int ar[], int num) {
    int r = 0;

    while (num) {
        ar[r] = num%10;
    }
}

// return true of all integers in num[] are mutual exclusive to each other
bool is_mutual_ex (int n, int num[]) {
    int mask = 0, a;

    for (int i = 0; i < n; i++) {
        a = num[i];
        while (a) {
            int d = 1 << (a % 10);
            if (mask & d) {
                return false;
            }

            mask |= d;
            a /= 10;
        }
    }

    return true;
}


// generate combinarions
void generate_combinations(int s_i, int c_c[], int c_c_len, int k, int elements[], int n, int all_comb[], int *c_c_ind) {
    // Base Case: If the current combination has k elements, add it to the result
    if (c_c_len == k) {
        all_comb[*c_c_ind] = gen_int(c_c, c_c_len);
        (*c_c_ind)++;
        return;
    }

    // If we've gone past the last element or don't have enough elements left
    // to form a combination of size k
    if (s_i >= n)
        return;
        
    // Optimization: If there aren't enough remaining elements to form a combination of size k
    // (len(elements) - start_index) < (k - len(current_combination))
    // This optimization can be added for efficiency but is not strictly necessary for correctness.
    if ((n-s_i) < (k-c_c_len))
        return;


    // Recursive Step: Iterate through elements from start_index
    for (int i = s_i; i < n; i++) {
        // 1. Include the current element
        c_c[c_c_len++] = elements[i];

        // 2. Recurse with the next index (i + 1)
        generate_combinations(i+1, c_c, c_c_len, k, elements, n, all_comb, c_c_ind);

        // 3. Backtrack: Remove the last added element to explore other possibilities
        c_c_len--;
    }

    return;
}

// Recursive function to calculate the sum of all cartesian sums of Pandigital Prime Sets that can be form
// given an partition combination of 9 Pandigital digits
// c: number of parts in the set
// k: current index within the set
// p_n: array of indexes into PRIME_NUM
// pri: current counts per index within PRIME_NUM
// p: PRIME_NUM array
int cart (int c, int k, int p_n[], int pri[], int num[], PRIME_NUM p[], DIGITS d[]) {
    int sum = 0;
    int i, j;
    int pri_local[9];

    // stop condition - we have a set to check
    if (k >= c) {
        if (is_mutual_ex(c, num)) {
            int cr = 1;

            // Cartesian product of all prime permutations
            for (i = 0; i < c; i++) {  /******  someting with pri index is wrong  */
                cr *= d[i].perm;
            }

            sum += cr;
        }
        return sum;
    }

    // copy local indexes
    memcpy(pri_local, pri, 9*sizeof(int));

    // Main loop.
    // Per p_n[] index, go through all possible digits in p
    while (pri_local[p_n[k]] < p[p_n[k]].p_cnt) {
        d[k].d = p[p_n[k]].digits[pri_local[p_n[k]]].d;
        d[k].perm = p[p_n[k]].digits[pri_local[p_n[k]]].perm;
        num[k] = d[k].d;
        pri_local[p_n[k]]++;
        sum += cart(c, k+1, p_n, pri_local, num, p, d);
    }

    return sum;
}

// a callback function for generatePartitions.
// part: a partition combination of 9
// the return value is the total number of Pandigital Prime Sets that can be constracted 
// using the primes numbers in p[] given a partition option represented by "part"
int calc_part (int part, PRIME_NUM p[]) {
    int c = 0, p_n[9], num[9];
    int sum = 0;
    int pri[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // will be used to track combinations within prime_num array
    DIGITS d[9];

    // initialize pr & rd
    while (part) {
        p_n[c] = (part % 10)-1;     // save Prime_num index
        part /= 10;
        c++;
    }

    sum = cart(c, 0, p_n, pri, num, p, d);

    return sum;
}

// Function to generate partitions of an integer n
// k: current index in the partition array (p)
// n: the number to partition (0 <= n < 10)!!
// max_val: the maximum value allowed for the current part
// parts[]: array of partitions cases. since n<10 we can store each p[i] as a digit in up to 9 digits integer
// part_c: number of partitions generated
int generatePartitions(int n, int k, int p[], int max_val, int (*f)(int part, PRIME_NUM p[]), PRIME_NUM pr[]) {
    int sum = 0;
    // Base case: If n is 0, we found a partition
    if (n == 0) {
        int num = 0;

        // convert array of pertitions into an integer
        for (int i = 0; i < k; i++) {
            num = num*10+p[i];
        }

        // return the returned value of the callback function
        return f(num, pr);
    }

    // Recursive step: Try adding numbers from max_val down to 1
    // or from min_val up to max_val (depending on desired order of parts)
    for (int i = max_val; i >= 1; i--) {
        if (n - i >= 0) { // If the remaining number is non-negative
            p[k] = i; // Add 'i' to the current partition

            // Recurse with remaining number and new max_val 'i'
            // The 'i' as max_val ensures parts are non-increasing
            sum += generatePartitions(n - i, k + 1, p, i, f, pr); 
                                                    
        }
    }

    return sum;
}

int main () {
    int i, j, k, num;
    PRIME_NUM primes[9];
    int comb [9];
    int digits [9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    char nums [10];
    int all_comb [126];     // max combinations for 9C4 and 9C5 == 126. Other options are less
    int all_perm [126];
    int n_comb;

    int parts [9];

    // generate all possible Pandigital primes
    for (i = 0; i < 9; i++) {
        n_comb = 0;

        // generate the binomial combinations for 9Ci
        generate_combinations(0, comb, 0, i+1, digits, 9, all_comb, &n_comb);
        primes[i].p_cnt = 0;
        primes[i].cnt = n_comb;

        for (k = 0; k < n_comb; k++) {
            sprintf(nums, "%d", all_comb[k]);
            all_perm[k] = 0;

            do {
                num = atol(nums);

                if (isPrime(num))
                    all_perm[k]++;
            }
            while (next_permutation(nums, i+1));

            if (all_perm[k] > 0)              
                primes[i].p_cnt++;
        }

        // alocate perms[i].digits only for combinations that produce Pandigital Prime permutations
        if ((primes[i].digits = malloc(primes[i].p_cnt*sizeof(DIGITS))) == NULL) {
            perror("Failed to allocate RAM:");
            exit( EXIT_FAILURE);
        }

        // copy DIGITS 
        j = 0;
        for (k = 0; k < primes[i].cnt; k++) {
            if (all_perm[k] > 0) {
                primes[i].digits[j].d = all_comb[k];
                primes[i].digits[j].perm = all_perm[k];
                j++;
            }
        }
    }

    // At this point we have all n digits Pandigital numbers that have some permutations that
    // are prime. Now we need to find all possible sets that comply with the problem definition
    

    printf("Pandigital Prime Sets: %d\n", generatePartitions(9, 0, parts, 9, calc_part, primes));

    // free RAM
    for (i = 0; i < 9; i++) {
        free(primes[i].digits);
    }

    return 0;
}