/**
The primes 3, 7, 109, and 673, are quite remarkable. By taking any two primes and 
concatenating them in any order the result will always be prime. For example, taking 7 
and 109, both 7109 and 1097 are prime. The sum of these four primes, 792, represents the 
lowest sum for a set of four primes with this property.
Find the lowest sum for a set of five primes for which any two primes concatenate to 
produce another prime.

Solution:
1) generate prime numbers up to a predicted limit. increase in case needed
2) implement a concat() function. i decided not to use strings option
3) implement a check_set() function that return true if a given set of primes conform with
the problem requirements
4) main loop: brute force all combinations from smallest prime to max-prime until we find
a set of 5 members. since we go from smallest to largest prime the first set it the answer

Answer: 26033
time:
real    0m23.577s
user    0m0.030s
sys     0m0.060s

set: 
[0] 13
[1] 5197
[2] 5701
[3] 6733
[4] 8389
Sum of set: 26033

real    0m59.409s
user    0m0.030s
sys     0m0.015s    

set: 13, 5197, 5701, 6733, 8389

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#define PRIME_ALLOC 100000
#define MAX_PRIME 100000000		// max prime which is a result of concatenating two primes
#define MAX_PRIME_TO_CHECK 10000 // best guess

int binarySearch(unsigned int arr[], unsigned int low, unsigned int high, unsigned int x);

#define is_prime(num,prime_size,prime_arr) binarySearch((prime_arr),0,(prime_size),(num))

/**
 * generate all prime numbers up to prime >= max
 * return: new array size
 **/
int gen_primes (int max, unsigned int **pa) {
	int i, k, l, as, num = 0L;
	bool isPrime, foundPrime;

	// allocate new space for n new prime numers
	if (*pa == NULL) {
		// allocate new array
		*pa = malloc(PRIME_ALLOC * sizeof(int));
	}

	if (*pa == NULL) {
		fprintf(stderr, "Failed to add new prime numbers, %s\n", strerror(errno));
		return 0;
	}

	// main loop
	(*pa)[0] = 2;
	as = 0;
	i = 1;
	while (num <= max) {
		while (i < PRIME_ALLOC && num < max) {
			foundPrime = false;
			num = (*pa)[as+i-1]+1;

			// loop forward until isPrime == TRUE
			while (!foundPrime) {
				// It is enough to check up to the square root of each number.
				// Beyond this value, all factors that constract the number start to repeat
				// For instamce the number 12 is constract of 1 x 12, 2 x 6, 3 x 4, 4 x 3, 6 x 2 and 12 x 1.
				// Square root of 12 is 3.46.. -> 4. Therefor it is enought to check up to 4
				k = (int)sqrtl(num)+1;
				isPrime = true;
				for (l = 0; (*pa)[l] < k; l++)
				{
					if (num % (*pa)[l] == 0)
					{
						isPrime = false;
						break;
					}
				}
				if (isPrime)
				{
					foundPrime = true;
					(*pa)[as+i] = num;
					i++;
				}
				else {
					num++;
				}
			}
		}

		as += i;

		if (i == PRIME_ALLOC) {
			// Increase size of prime array
			*pa = realloc(*pa, (as + PRIME_ALLOC) * sizeof(int));
			if (*pa == NULL) {
				fprintf(stderr, "Failed to increase size of prime array, %s\n", strerror(errno));
				return 0;
			}
			else {
				// reset counter
				i = 0;
			}
		}
	}

	return as-1;
}

// An iterative binary search function.
int binarySearch(unsigned int arr[], unsigned int low, unsigned int high, unsigned int x) {
    while (low <= high) {
        unsigned int mid = low + (high - low) / 2;

        // Check if x is present at mid
        if (arr[mid] == x)
            return mid;

        // If x greater, ignore left half
        if (arr[mid] < x)
            low = mid + 1;

        // If x is smaller, ignore right half
        else
            high = mid - 1;
    }

    // If we reach here, then element was not present
    return -1;
}

unsigned int concat (unsigned int p1, unsigned int p2) {
	unsigned int r = p1;
	unsigned int t = p2;

	while (t) {
		t /= 10;
		r *= 10;
	}

	return r+p2;
}

bool check_set (int n, unsigned int primes[], unsigned int as, unsigned int *prime_a) {
	unsigned int i, j;
	unsigned int p;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (i == j)
				continue;
			p = concat(primes[i], primes[j]);
			if (is_prime(p, as, prime_a) == -1)
				return false;
		}
	}

	return true;
}

int main () {
	unsigned int p [5] = {0, 0, 0, 0, 0};
	unsigned int *primes = NULL;
	unsigned int as = 0;
	int i0, i1, i2, i3, i4, i, s;

	// preallocate primes
	if ((as = gen_primes(MAX_PRIME, &primes)) == 0) {
		printf("Failed to allocate more primes: %s\n", strerror(errno));
		return -1;
	}

	for (i0 = 0; primes[i0] < MAX_PRIME_TO_CHECK; i0++) {
		p[0] = primes[i0];
		for (i1 = i0+1; primes[i1] < MAX_PRIME_TO_CHECK; i1++) {
			p[1] = primes[i1];
			if (check_set(2, p, as, primes)) {
				for (i2 = i1+1; primes[i2] < MAX_PRIME_TO_CHECK; i2++) {
					p[2] = primes[i2];
					if (check_set(3, p, as, primes)) {
						for (i3 = i2+1; primes[i3] < MAX_PRIME_TO_CHECK; i3++) {
							p[3] = primes[i3];
							if (check_set(4, p, as, primes)) {
								for (i4 = i3+1; primes[i4] < MAX_PRIME_TO_CHECK; i4++) {
									p[4] = primes[i4];
									if (check_set(5, p, as, primes))							
										goto done;
								}		
							}
						}
					}
				}
			}
		}
	}

done:
	s = 0;
	for (i = 0; i < 5; i++) {
		s += p[i];
		printf("[%d] %d\n", i, p[i]);
	}

	printf("Sum of set: %d\n", s);

	return 0;

}
