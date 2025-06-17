/**
 * 
The arithmetic sequence, 1487, 4817, 8147, in which each of the terms increases by 3330, 
is unusual in two ways: (i) each of the three terms are prime, and, (ii) each of the 
4-digit numbers are permutations of one another.
There are no arithmetic sequences made up of three 1-, 2-, or 3-digit primes, exhibiting 
this property, but there is one other 4-digit increasing sequence.
What 12-digit number do you form by concatenating the three terms in this sequence?

Propose solution:
1) generate all 4 digits primes array
2) create a translation array for 0..9 digits when 0=0, 1=10, 2=20... 9-900,000,000.
3) using this translation table, each 4 digits number will have a unique keye regardless the 
digits permutations
4) loop through all 4 digits primes and find the 3 primes that has similar keys and conform 
with the problem reauirement.

program output:
Found arithmetic sequence: 1487, 4817, 8147
Found arithmetic sequence: 2969, 6299, 9629 ==> the required set

Answer: 2969, 6299, 9629

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#define PRIME_ALLOC 2000

typedef struct {
	int prime;
	unsigned int p_key;
} P_KEY;

const unsigned int tr [10] = {0, 1, 20, 300, 4000, 50000, 600000, 7000000, 80000000, 900000000};

/**
 * sort function for qsort()
 * compare two P_KEY structs based on p_key and then on prime value
 * return:
 * =0 if two keys are equal
 * >0 if k1->p_key > k2->p_key or p_key is equale but k1->prime > k2->prime
 * <0 if k1->p_key < k2->p_key or p_key is equale but k1->prime < k2->prime
 */
int cmp_p_key (const void *k1, const void *k2) {
	P_KEY *pk1, *pk2;

	pk1 = (P_KEY *)k1;
	pk2 = (P_KEY *)k2;

	if (pk1->p_key == pk2->p_key) {
		if (pk1->prime == pk2->prime)
			return 0;
		else
			return (pk1->prime - pk2->prime);
	}

	return (pk1->p_key - pk2->p_key);
}

/**
 * generate all 4 digits prime numbers
 * return: new array size
 **/
int gen_4_digits_primes (int **pa) {
	int i, k, l, num = 0L;
	bool isPrime, foundPrime;
	int *ipa = NULL;

	// allocate new space for n new prime numers
	if (ipa == NULL) {
		// allocate new array
		ipa = malloc(PRIME_ALLOC * sizeof(int));
	}

	if (ipa == NULL) {
		fprintf(stderr, "Failed to add new prime numbers, %s\n", strerror(errno));
		return 0;
	}

	// main loop
	ipa[0] = 2;

	for (i = 1; i < PRIME_ALLOC && num < 9999; i++) {
		foundPrime = false;
		num = ipa[i-1]+1;

		// loop forward until isPrime == TRUE
		while (!foundPrime) {
			// It is enough to check up to the square root of each number.
			// Beyond this value, all factors that constract the number start to repeat
			// For instamce the number 12 is constract of 1 x 12, 2 x 6, 3 x 4, 4 x 3, 6 x 2 and 12 x 1.
			// Square root of 12 is 3.46.. -> 4. Therefor it is enought to check up to 4
			k = (int)sqrtl(num)+1;
			isPrime = true;
			for (l = 0; ipa[l] < k; l++)
			{
				if (num % ipa[l] == 0)
				{
					isPrime = false;
					break;
				}
			}
			if (isPrime)
			{
				foundPrime = true;
				ipa[i] = num;
			}
			else {
				num++;
			}
		}
	}

	if (i == PRIME_ALLOC) {
		// PRIME_ALLOC is too small
		free(ipa);
		return 0;
	}
	else {
		for (k = 0, l = 0; k < i && ipa[k] < 1000; k++) l++;
		// allocate space for returned prime numers array
		k = i - l - 1;
		if ((*pa = malloc(k * sizeof(int))) == NULL) {
			// allocate new array
			fprintf(stderr, "Failed to add new prime numbers, %s\n", strerror(errno));
			return 0;
		}
		else {
			memcpy(*pa, ipa+l, k*sizeof(int));
			free(ipa);
			return k;
		}
	}
}

/**
 * generate a unique key for a 4 digits prime number.
 * no mater what is the digits order within the number, the key value will be the same
 */
unsigned int gen_key (int prime) {
	unsigned int r = 0L;
	int m;

	while (prime > 0) {
		m = prime % 10;
		r += tr[m];
		prime = (prime - m) / 10;
	}

	return r;
}

/**
 * check for arithmetic sequence
 * 
 * p_key_a: first P_KEY array element
 * size: number of elements to check
 * prime1, prime2 & prime3: 3 prime numbers in the arithmetic sequence that was found
 * return: TRUE if arithmetic sequence was found, FLASE if not
 */
bool a_seq(P_KEY *p_key_a, int size, unsigned int *prime1, unsigned int *prime2, unsigned int *prime3) {
	int distances [size][size];
	int dist;
	int c, r, c1, r1;
	bool found = false;

	// first fill the matrix with the distances between the prime numbers
	for (r = 0; r < size-1; r++) {
		for (c = r+1; c < size; c++)
			distances[r][c] = p_key_a[c].prime - p_key_a[r].prime;
	}

	// scan the matrix and see if there are two identical distance between 3
	// prime numbers so that prime3 - prime2 == prime2 - prime1
	for (r = 0; r < size-1 && !found; r++) {
		for (c = r+1; c < size && !found; c++) {
			dist = distances[r][c];
			r1 = c;

			// now scan rest of the matrix for identical distance but start with raw = c
			for (c1 = r1+1; c1 < size; c1++) {
				if (distances[r1][c1] == dist) {
					found = true;
					break;
				}
			}
		}
		if (found)
			break;
	}

	if (found) {
		*prime1 = p_key_a[r].prime;
		*prime2 = p_key_a[c-1].prime; // c-1 since c has been increased before breaking out of the main for loop
		*prime3 = p_key_a[c1].prime;
	}
	
	return found;
}

int main () {
	int *prime_a = NULL;
	int size;
	P_KEY *p_key_a = NULL;
	int  i, s, e;
	unsigned int p_key, prime1, prime2, prime3;

	// generate 4 digits prime array
	size = gen_4_digits_primes (&prime_a);

	if (size > 0) {

		// allocate prime keys array
		if ((p_key_a = malloc(size * sizeof(P_KEY))) == NULL) {
			fprintf(stderr, "failed to allocate prime keys array: %s", strerror(errno));
			free(prime_a);
			return 1;
		}

		// fill prime keys array
		for (i = 0; i < size; i++) {
			p_key_a[i].prime = prime_a[i];
			p_key_a[i].p_key = gen_key(prime_a[i]);
		}

		// sort prime keys array based on p_key & prime values
		qsort(p_key_a, size, sizeof(P_KEY), &cmp_p_key);

		// scan sorted p_key array and identify set of primes with 3 or more identical keys
		s = 0;
		e = 1;
		p_key = p_key_a[s].p_key;
		while (e < size) {
			while (p_key == p_key_a[e].p_key && e < size)
				e++;

			// check for arithmetic sequence only for sets of 3 or more primes with
			// identical keys
			if ((e - s) > 2) {
				if (a_seq(&p_key_a[s], e-s, &prime1, &prime2, &prime3))
					printf("Found arithmetic sequence: %u, %u, %u\n", prime1, prime2, prime3);
			}

			s = e;
			p_key = p_key_a[s].p_key;
		}

	}

	free(prime_a);
	free(p_key_a);
	return 0;
}