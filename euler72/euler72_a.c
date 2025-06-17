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
 * Naive approach:
 * loop through d = 1 to 1M
 * for each divider d loop through n = 1 to d-1 and count number of fractions that
 * HCF(n,d) = 1. This will guaranry that we will not have any repetions.
 * No need to order anything!
 * I did 3 optimizations:
 * 1) in the HFC function it iterate through n = 2 to n/2 to look for common deliminator
 * and check for special cases where n = 1, and d is divided by n
 * 2) on the main loop, for odd divider, increase n by 2 instead of 1
 * 3) if d is a prime number then it has n-1 fractions
 * 
 * The complexity is o(n^3) -  * This approach did not solve the problem since it could take forever.
 * 
 * Using Euler's totient function
 * phi(n) = n*(1-1/p1)*(1-1/p2)*...(1-1/pm) where p1..pm are prime numbers that
 * can devide n with no remainings.
 * So basicaly we need to sum phi(n) for n = 2 to 1M.
 * Since phi(n) for n=prime = n-1 no need to call it for d = prime.
 * Complexity = o(n^2)
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

bool prInd[1000001L];
long int pr[1000001L];


/**
 * Euler's totient function for n which is not a prime number
 */ 
long int phi (long int n) {
	long int i = 1;
	long int r = n;
	long int lp;

	// largest prime factor of n can not be larger than n/2
	// if this function should have worked for prime number as well,
	// lp must be equal to n otherwise phi(n) for n = prime, will retun 0
	lp = n / 2;

	while (pr[i] <= lp) {
		if (n % pr[i] == 0) {
			r = r  / pr[i] * (pr[i] - 1);
		}
		i++;
	}

	return r;
}

/**
 * generate an array of primes where largest prime <= n
 * 
 */ 
void gen_prime (long int n) {
	long int i, j, k, ind = 1;
	int isPrime;

	prInd[0] = true;
	prInd[1] = true;
	pr[0] = 1;

	for (i = 1; i <= n; i++) {
		prInd[i] = false;
		pr[i] = 0;
	}


	for (i = 2; i <= n; i++)
	{
		isPrime = 1;

		// It is enough to check up to the square root of each number.
		// Beyond this value, all factors that constract the number start to repeat
		// For instamce the number 12 is constract of 1 x 12, 2 x 6, 3 x 4, 4 x 3, 6 x 2 and 12 x 1.
		// Square root of 12 is 3.46.. -> 4. Therefor it is enought to check up to 4
		k = (long int)sqrtl(i)+1;
		for (j = 2; j < k; j++)
		{
			if (i % j == 0)
			{
				isPrime = 0;
				break;
			}
		}
		if (isPrime)
		{
			prInd[i] = true;
			pr[ind++] = i;
		}
	}
}

void main () {
	long int n, d, a, np;
	long long int c = 0L;

	gen_prime(1000000L);

	for (d = 2; d <= 1000000L; d++) {
		if (prInd[d]) {
			c += (d - 1);
		}
		else {
			c += phi(d);	
		}
			
	}

	printf("\n c=%I64d\n", c);
}