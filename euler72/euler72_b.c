/**
 * Problem 72:
 * Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1, it is called a reduced proper fraction.
 * If we list the set of reduced proper fractions for d ≤ 8 in ascending order of size, we get:
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8
 * It can be seen that there are 21 elements in this set.
 * How many elements would be contained in the set of reduced proper fractions for d ≤ 1,000,000?
 * 
 * Solution by: Yoram Finder:
 * ==========================
 * 
 * Use both Using Euler's totient function
 * phi(n) = n*(1-1/p1)*(1-1/p2)*...(1-1/pm) where p1..pm are prime numbers that can devide n with no remainings
 * and Sieve of Eratosthenes.
 * 
 * Complexity o(n)
 */

#include <stdio.h>

long int phi[1000001L];

void main () {
	long int i, j;
	long long int c = 0L;

	// init phi array
	for (i = 0; i <= 1000000L; i++)
		phi[i] = i;

	for (i = 2; i <= 1000000L; i++) {
		// if phi[i] == i it means it was not calculated
		if (phi[i] == i) {
			for (j = i; j <= 1000000L; j += i) {
				phi[j] = phi[j] / i * (i - 1);
			}
		}
		c += phi[i];
	}

	printf("\n c=%I64d\n", c);
}