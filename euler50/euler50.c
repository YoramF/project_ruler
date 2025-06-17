/**
 * 
The prime 41, can be written as the sum of six consecutive primes:
41 = 2 + 3 + 5 + 7 + 11 + 13.
This is the longest sum of consecutive primes that adds to a prime below one-hundred.
The longest sum of consecutive primes below one-thousand that adds to a prime, contains 21 terms, and is equal to 953.
Which prime, below one-million, can be written as the sum of the most consecutive primes?

solution:
1) generate primes up to 1,000,000
2) loop downward over the prime numers and calculate how many consecutive primes sum to their value if any
3) save the one with the highest number of consecutive primes

answer:
997651 contains 543 terms starting from prime=7

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#define PRIME_ALLOC 10000

/**
 * generate all prime numbers up to prime >= max
 * return: new array size
 **/
int gen_primes (int max, int **pa) {
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
	(*pa)[0] = 1;
	(*pa)[1] = 2;
	as = 0;
	i = 2;
	while (num < max) {
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
				for (l = 1; (*pa)[l] < k; l++)
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

/**
 * given a prime number as an imput, return true if prime is a sum of consecutive primes.
 * assumtpion: pa array largest prime number must be bigger than input prime.
 * pa: prime array
 * prime: prime number to check
 * start: will be update with the first prime in the list if we found sum of consecutive primes
 * cnt: number of primes in the list
 */
bool is_seq (int *pa, int prime, int *start, int *cnt) {
	int i, j, s;
	bool found = false;
	int sum, c;

	s = 0;
	while (pa[s] < prime) {
		i = s;
		sum = 0;
		c = 0;

		while (sum < prime) {
			sum += pa[i];
			i++;
			c++;
		}

		if (sum == prime) {
			*start = s;
			*cnt = c;
			found = true;
			break;
		}
		else
			s++;
	}

	return found;
}

int main () {
	int *prime_a = NULL;
	int size, i, m_prime = 1, cnt = 0, start = 0, m_cnt = 0, m_start;

	size = gen_primes(1000001, &prime_a);

	for (i = 2; i < size; i++) {
		if (is_seq(prime_a, prime_a[i], &start, &cnt)) {
			if (m_cnt < cnt) {
				m_prime = prime_a[i];
				m_cnt = cnt;
				m_start = start;
			}
		}
	}

	printf("%d, %d %d\n", m_prime, m_start, m_cnt);

	free(prime_a);
	return 0;
}