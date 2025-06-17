/*
By replacing the 1st digit of the 2-digit number *3, it turns out that six of the nine 
possible values: 13, 23, 43, 53, 73, and 83, are all prime.
By replacing the 3rd and 4th digits of 56**3 with the same digit, this 5-digit number is the 
first example having seven primes among the ten generated numbers, yielding the family: 
56003, 56113, 56333, 56443, 56663, 56773, and 56993. Consequently 56003, being the first 
member of this family, is the smallest prime with this property.
Find the smallest prime which, by replacing part of the number (not necessarily adjacent 
digits) with the same digit, is part of an eight prime value family.

solution:
1) for any given number we need to generate all permutations of that number with wildcards 
replacing 1 to the number of digits in that number.
2) once we generate these sets of permutations, we need to replace the wildcards with same digit
and check if the result number is prime.
3) once we find the first set with 8 smallest distinct prime numbers we got the answer
4) since prime numbers can't have 0,2,4,5,6,8 as their last digit, and we are looking for a
set of 8 distinct prime numbers, the wildcard permutation shuld be for the first n-1 digits 
of a given number.
5) write gen_prime() to generate initial list of prime numbers. the minimum number of digits
that can generate 8 or more permutations of wildcards is 6 digits numbers with 2 wildcards. therefore
we will start scaning the primes list starting with the first 6 digits prime 
6) write is_prime() function to check if a number is a prime.

answer:  121313

runtime:
real    0m2.506s
user    0m0.000s
sys     0m0.015s

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#define PRIME_ALLOC 1000000
#define MAX_DIGITS 9


/**
 * add n new prime numers to prime numbers array
 * n: number of new primes to add to array
 * as: prime array size
 * pa: address of a pointer to prime number array
 * return: new array size
 **/
int gen_prime (int n, int as, unsigned int **pa) {
	unsigned int num;
	int i, j, k, l;
	bool isPrime, foundPrime;

	// allocate new space for n new prime numers
	if (*pa == NULL) {
		// allocate new array
		*pa = malloc(n * sizeof(unsigned int));
	}
	else {
		// change exiting array
		*pa = realloc(*pa, (as + n) * sizeof(unsigned int));
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
				(*pa)[j] = num;
			}
			else
				num++;
		}
	}

	return as + n;
}

// An iterative binary search function.
int binarySearch(unsigned int arr[], int low, int high, unsigned int x)
{
    while (low <= high) {
        int mid = low + (high - low) / 2;

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

/**
 * check if a given number is a prime number using binary search.
 * this function as a side effect: it update the prime array (pa) if num is larger than
 * largest prime number in prime array.
 * return
 * prime offset in prime array
 * -1: if not found
 */
int is_prime (unsigned int num, int *as, unsigned int **pa) {
	

	while ((*pa == NULL) || (*pa)[*as-1] < num) {
		// allocate prime array
		if ((*as = gen_prime(PRIME_ALLOC, *as, pa)) == 0) {
			printf("Failed to allocate more primes: %s\n", strerror(errno));
			return -1;
		}
	}

	return (binarySearch(*pa, 0, *as-1, num));
}

/**
 * generate all permutations of 1..digits-1 '*' within a number with n digits
 * return an array of strings of the form of n digits and n-1 '*', where
 * '*' is replacing a digit.
 * This array should be freed once main program is done with the array
 * the wildcard permutation is basically the binary representation of 1 to 2^(n-1), where
 * n > 0 is number of digits. on '0' keep the original digit, on '1' replace with '*'
 * return -1 if error
 */
int gen_permu (int digits, int num, char **perms) {
	char *s_num;
	int nums, a_size;
	unsigned int i, j, k;

	// calculate required memory
	// count from 0 - 2^(n-1) since we keep last digit in input number.
	// size of each element int the array is digits+1 (for '\0')
	nums = pow(2, (digits-1));
	a_size = nums * (digits+1);

	// allocate initial string space
	if ((*perms = calloc(a_size, sizeof(char))) == NULL) {
		fprintf(stderr, "Failed to allocate strings space : %s\n", strerror(errno));
		return -1;
	}
	
	s_num = *perms;

	// first string in strings array is the imput number
	sprintf(s_num, "%.*d", digits, num);
	s_num += digits+1;	// advance number of digits + '\0' + cnt

	for (i = 1; i < nums; i++) {

		// generate permutation, so that 0 means original digit, 1 means '*'
		// but first copy initial number
		strncpy(s_num, *perms, digits+1);

		j = 0;
		k = i;
		while (k) {
			if (k & 0x1)
				s_num[j] = '*';
			j++;
			k >>= 1;
		}

		s_num += digits+1;
	}

	return nums;
}

/**
 * generate all numbers based on the wildcars string *perm
 * insert  numbers into arr[]
 * return number of elements in updated arr[]. this number can be less than 10 in case
 * sone of the result numbers will have less digits than input digits (preceding zeros)
 */
int gen_numbers(char *perm, int digits, unsigned int arr[]) {
	unsigned int num, min_num = pow(10, digits-1);
	int i, j, k = 0;

	for (i = 0; i < 10; i++) {
		num = 0;
		for (j = 0; j < digits; j++)
			num = (num*10) + (perm[j] == '*'? i: (perm[j]-'0'));

		if (num >= min_num)
			arr[k++] = num;	
	}

	return k;
}


/**
 * check if we can construct an 8 primes family according to the problem from given number
 * num: number to check
 * digits: number of digits in given number
 * family: if a family is found, this array will be filled with the 8 prime numbers
 * as: function side effect. might be updated in case more primes will be added to prime array
 * prime: function side effect. might be updated in case more primes will be added to prime array
 * return: TRUE if a family was found, FALSE otherwise
 */
bool is_family(unsigned int num, int digits, unsigned int family[], int *as, unsigned int **prime) {
	bool found = false;
	char *perm = NULL;
	char *s_num;
	int n, c, i, j, k, pc;
	char wc;
	unsigned int arr[10], t_arr[10], number;

	// first generate wildcard permutations
	if ((n = gen_permu (digits, num, &perm)) > 0) {
		c = 1;
		while (c < n) {
			s_num = perm + c*(digits+1);

			k = gen_numbers(s_num, digits, t_arr);

			// check this set only if there are more than 8 distict numbers
			if (k >= 8) {
				// count how many of these numbers are prime
				pc = 0;
				for (j = 0; j < k; j++) {
					if (is_prime(t_arr[j], as, prime) > -1) {
						arr[pc++] = t_arr[j];
					}
				
				}

				if (pc >= 8) {
					for (int l = 0; l < pc; l++)
						printf("[%d] %u\n", l, arr[l]);
					found = true;
					goto done;
				}
			}

			c++;
		}
	}

done:
	free(perm);

	return found;
}


int main () {
	unsigned int *prime = NULL;
	int digits, n, as = 0;
	unsigned int p, largest;
	bool found = false;
	unsigned int family[8];


	// start with 6 digits numbers
	digits = 6;

	while (!found && digits <= MAX_DIGITS) {

		largest = pow(10,digits)-1;
		p = pow(10,digits-1);	// lowest number 

		while (p < largest && !found) {
			// check last digits and skip if last digit is 0,2,4,5,6,8
			switch (p % 10) {
				case 0:
				case 2:
				case 4:
				case 5:
				case 6:
				case 8:
					p++;
					break;
				default:
					found = is_family(p, digits, family, &as, &prime);
					if (!found)
						p++;
					break;
			}
		}

		digits++;
	}

	free(prime);
	return 0;
}