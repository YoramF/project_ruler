/*
For any set A of numbers, let SUM(A) be the sum of the elements of A.
Consider the set B = {1,3,6,8,10,11}. There are 20 subsets of B containing three elements, and their sums are:

	SUM({1,3,6}) = 10,
	SUM({1,3,8}) = 12,
	SUM({1,3,10}) = 14,
	SUM({1,3,11}) = 15,
	SUM({1,6,8}) = 15,
	SUM({1,6,10}) = 17,
	SUM({1,6,11}) = 18,
	SUM({1,8,10}) = 19,
	SUM({1,8,11}) = 20,
	SUM({1,10,11}) = 22,
	SUM({3,6,8}) = 17,
	SUM({3,6,10}) = 19,
	SUM({3,6,11}) = 20,
	SUM({3,8,10}) = 21,
	SUM({3,8,11}) = 22,
	SUM({3,10,11}) = 24,
	SUM({6,8,10}) = 24,
	SUM({6,8,11}) = 25,
	SUM({6,10,11}) = 27,
	SUM({8,10,11}) = 29.

Some of these sums occur more than once, others are unique.
For a set A, let U(A,k) be the set of unique sums of k-element subsets of A, in our example we find U(B,3) = {10,12,14,18,21,25,27,29} and SUM(U(B,3)) = 156.

Now consider the 100-element set S = {1^2, 2^2, ... 100^2}.
S has 100891344545564193334812497256 50-element subsets.

Determine the sum of all integers which are the sum of exactly one of the 50-element subsets of S, i.e. find SUM(U(S,50)).

Solution:
1) largest value in this set is 10,000
2) largest possible sum of 50 elements in this set is 297,925 - Sum of Squares of n Natural Numbers.
n*(n+1)*(2n+1)/6
Calculate S(n)-S(n-50) to get the sum of last 50 elements
3) use recursion function to check if a given sum is valid - based on a modified "subset sum algorithm"
4) use recursion function to check if a given sum is a unique sum
5) sum only the unieiq sums

Solution:
Sums of all integers: 115039000

real    2m44.223s
user    0m0.000s
sys     0m0.000s

RAM: ~3GB

*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXSUMS 295426
#define MAXSET	100
#define SUBSET 50

char *prev;
char *prevU;

#define _ind(i,j,k)	(((i)*(MAXSET+1)+(j))*(SUBSET+1)+(k))

/*
	Returns  TRUE if there is a subset where its largest number to square is 'max' with 'num' elements
	in subset, where their sum is equal to 'sum'
*/
bool sumExists (int sum, int max, int num) {
	bool r;

	// check previous results
	if (prev[_ind(sum, max, num)] > -1)
		return (bool)prev[_ind(sum, max, num)];

    // Base Cases
    if ((sum == 0) && (num == 0)) {
		r = true;
		goto done;
	}

    if ((num == 0) || (max == 0)) {
		r = false;
		goto done;
	}
 
    // If last element is greater than sum,
    // then ignore it
    if (max*max > sum) {
		r = sumExists(sum, max-1, num);
		goto done;
	}
 
    // Else, check if sum can be obtained by any
    // of the following:
    // (a) excluding the last element
    // (b) substract the last element and reduce subset size by one
    r = sumExists(sum, max-1, num)
           || sumExists(sum - max*max, max-1, num-1);

	done:
		prev[_ind(sum, max, num)] = r;
		return r;
}

/*
	get largest max integer where max*max is smaller than sum and generates a true result
	when calling sumExists()
*/
int largest(int sum, int max, int num) {
	int m;

	m = sumExists(sum, max, num)? max: 0;
	while (m > 0 && !sumExists(sum-m*m, m-1, num-1)) {
		m--;
	}

	return m;
}

/*
	Return true only if 'sum' is a unique sum of subset with 'num' elements
*/
bool isUnique (int sum, int max, int num) {
	bool r;
	int c, m;
	
	// check previous results
	if (prevU[_ind(sum, max, num)] > -1)
		return (bool)prevU[_ind(sum, max, num)];

	if ((num == 1) && (sum == max*max)) {
		r = true;
		goto done;
	}

	if ((max < num) || (sum <= 0)) {
		r = false;
		goto done;
	}

	if (sum < max*max) {
		r = isUnique(sum, max-1, num);
		goto done;
	}

	c = 0;
	m = largest(sum, max, num);

	while (sumExists(sum, m, num)) {
		if (isUnique(sum-m*m, m-1, num-1))
			c++;
		else
			// this covers cases where more than one subsets with same largest integer give
			// same sum.
			c += 2;
		m = largest(sum, --m, num);
	}

	if (c == 1)
		r = true;
	else
		r = false;

	done:
		prevU[_ind(sum, max, num)] = r;
		return r;
}



int main () {
	int i;
	long long int S = 0L;


	// inint prev & prevU
	if ((prev = malloc((MAXSUMS+1)*(MAXSET+1)*(SUBSET+1)*sizeof(char))) == NULL) {
		printf("Error allocating RAM");
		return 1;
	}
	if ((prevU = malloc((MAXSUMS+1)*(MAXSET+1)*(SUBSET+1)*sizeof(char))) == NULL) {
		printf("Error allocating RAM");
		return 1;		
	}
	memset(prev, -1, (MAXSUMS+1)*(MAXSET+1)*(SUBSET+1)*sizeof(char));
	memset(prevU, -1, (MAXSUMS+1)*(MAXSET+1)*(SUBSET+1)*sizeof(char));

	// scan sums and sun all true indecs
	for (i = SUBSET*(SUBSET+1)*(2*SUBSET+1)/6; i < MAXSUMS; i++) {
		if (isUnique(i, MAXSET, SUBSET)) {
			S += i;
		}
	}	

	printf("Sums of all integers: %lld\n ", S);

	return 0;

}