/*
Euler project problem #46
--------------------------
It was proposed by Christian Goldbach that every odd composite number can be written as the sum of a prime and twice a square.

9 = 7 + 2 * 1^2
15 = 7 + 2 * 2^2
21 = 3 + 2 * 3^2
25 = 7 + 2 * 3^2
27 = 19 + 2 * 2^2
33 = 31 + 2 * 1^2

It turns out that the conjecture was false.
What is the smallest odd composite that cannot be written as the sum of a prime and twice a square?

Solution approach:
use dynamic programing to save prime numbers and 2xN^2 values.
1) any prime numer conform Christian Goldbach statement: P = P + 2 * 0^2 where P is any prime number;
2) generate the first 100 prime numner and save them in allocated array. each time we reach
the last prime number in the array, allocate 100 more spaces and fill them with next 100 prime numbers
3) loop until finding the first odd number that does not conform with Christian Goldbach statement - that is the answer
4) inside the main loop, use the following algorithm:
	loop from closest prime number to 1, substruct the prime number from the odd number and check
	check that the remining can be devided by 2 and the square root of the result is a real number.
	if yes, the odd numbere conform with the rule, break, and return to main loop.
	if not, return to inner loop - move thepreceding prime number .
	if inner loop is done, the odd number is the minimum odd number the does not conform with the rule.

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

int main () {
	long int i = 0, as = 0, *pa = NULL;
	long int pi = 0, pir;
	double sq, r;
	bool found = false, match = true;

	// loop through all odd numbers which are not prime
	while (!found && i < 1000000) {
		if (i > as-1) {
			if ((as = gen_prime(1000, as, &pa)) == 0) {
				printf("Failed to allocate more primes");
				return 1;
			}
		}

		if ((i < pa[pi]) && i%2) {
			if (pi > 0) {
				match = false;
				pir = pi - 1;
				while (pir >= 0 && !match) {
					r = i - pa[pir];

					// check for odd reminider
					if (r == ((r / 2) * 2)) {
						// now check for square root
						r = r / 2;
						sq = floor(sqrt(r));
						if (r == pow(sq, 2)) {
							match = true;
						}
					}

					pir--;
				}			
			}

			found = !match;
		}
		else if (i > pa[pi]) {
			pi++;
		}

		i++;
	}

	printf("While complete: %d, %d\n", found, i-1);

	return 0;
}