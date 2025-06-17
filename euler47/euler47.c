/*
The first two consecutive numbers to have two distinct prime factors are:

14 = 2 * 7
15 = 3 * 5

The first three consecutive numbers to have three distinct prime factors are:

644 = 2^2 * 7 * 23
645 = 3 * 5 * 43
646 = 2 * 17 * 19

Find the first four consecutive integers to have four distinct prime factors each. What is the first of these numbers?

Propose solution:
1) generate an array of prime numbers
2) loop over real number and for each number check if it has 4 distinct prime factors
3) go over these real numbers and once 4 consecutive numbers numers that have 4 distinct prime factors, stop.4
this is the answer.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

/**
 * add n new prime numers to prime numbers array
 * n: number of new prime numbers to add
 * as: prime array size
 * pa: address of a pointer to prime number array
 * return: new array size
 **/
long int gen_prime (long int n, long int as, long int **pa) {
	long int i, j, k, l, num;
	bool isPrime, foundPrime;

	// allocate new space for n new prime numers
	if (*pa == NULL) {
		// allocate new array
		*pa = malloc(n * sizeof(long int));
	}
	else {
		// change exiting array
		*pa = realloc(*pa, (as + n) * sizeof(long int));
	}

	if (*pa == NULL) {
		fprintf(stderr, "Failed to add new prime numbers, %s\n", strerror(errno));
		return 0;
	}

	// main loop
	for (i = 0, j = as; i < n; i++, j++) {
		foundPrime = false;

		if (j > 1)
			num = (*pa)[j-1] + 1;	// get next number and check if it is a prime number
		else {
			(*pa)[j] = j+1;		// special case for first 2 prime numbers
			continue;		
		}


		// loop forward until isPrime == TRUE
		while (!foundPrime) {
			// It is enough to check up to the square root of each number.
			// Beyond this value, all factors that constract the number start to repeat
			// For instamce the number 12 is constract of 1 x 12, 2 x 6, 3 x 4, 4 x 3, 6 x 2 and 12 x 1.
			// Square root of 12 is 3.46.. -> 4. Therefor it is enought to check up to 4
			k = (long int)sqrtl(num)+1;
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
				(*pa)[j] = num;
			}
			else
				num++;
		}
	}

	return as + n;
}

/**
 * check if given real number has 4 distinct prime factors.
 * num: given real number to check
 * primes: array of primes
 * ** major assumption: largest prime number in prime array is smaller than input num
 * ** but larger than num/2;
**/
bool is_4_dpf (long int num, long int *primes) {
	int c = 0;
	long int r, p, *pr = primes+1; // start with 2 as first prime
	bool cp = false;

	while (num > 1 && c <= 4) { 
		if (num % *pr == 0) {
			num /= *pr;
			if (!cp) {
				cp = true;
				c++;
			}
		}
		else {
			pr++;
			cp = false;
		}
	}

	return (c == 4 && num == 1);
}

int main () {
	long int num, pnum;
	long int *prime = NULL;
	long int pa = 0;
	int c = 0;

	// allocate prime array
	if ((pa = gen_prime(10000, pa, &prime)) == 0) {
		printf("Failed to allocate more primes");
		return 1;
	}

	// start the loop from 2 x 3 x 5 x 7
	for (num = 210; num < 1000000; num++) {

		// check if additional primes need to be added to prime array
		if (prime != NULL && prime[pa-1] < num) {
			// allocate prime array
			if ((pa = gen_prime(10000, pa, &prime)) == 0) {
				printf("Failed to allocate more primes");
				return 1;
			}			
		}

		if (is_4_dpf(num, prime)) {
			if (c == 0) {
				pnum = num;
				c++;
			}
			else if (c < 4) {
				if (num == pnum + 1) {
					c++;
					pnum = num;
				}
				else {
					c = 0;
				}
			}
			if (c == 4) {
				// found 4 consecutive numbers numers that have 4 distinct prime factors
				printf("%d, %d, %d, %d\n", num-3, num-2, num-1, num);
				break;
			}
		}
		else {
			c = 0;
		}
	}

	return 0;
}