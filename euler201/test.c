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
Calculate S(n)-S(n-50) to get the sum of last 50 elements
3) create a boolean array of 297,925 and reset all elemets to false
4) go through all possible subsets of 50 elements and sum their values and mark the corresponding
element in the boolean array to true
5) scan the boolean array and sum all true indeces 

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// #define MAXSUMS 175
// #define MAXSET	8
// #define SUBSET 4


int main (int argc, char *argv[]) {
	char *sums;
	int *set;
	int i, x, y;
	int S = 0, lc = 0;
	int c = 0;
	int *idx;
	char opt;
	int SUBSET, MAXSET, MAXSUMS;

		// get commad line input
	if (argc > 1)
	{
		while ((opt = getopt(argc, argv, "n:s:")) > 0)
			switch (opt)
			{
			case 'n':
				MAXSET = x = atoi(optarg);
				break;
			case 's':
				SUBSET = y = atoi(optarg);
				break;
			default:
				printf("wrong input\n");
				return 0;
			}
		}

	MAXSUMS = (x*(x+1)*(2*x+1) - (x-y)*((x-y)+1)*(2*(x-y)+1))/6+1;

	if ((set = malloc(MAXSET*sizeof(int))) == NULL) {
		printf("Failed to allocte memory");
		return 1;
	}

	if ((sums = malloc(MAXSUMS+1)) == NULL) {
		printf("Failed to allocte memory");
		return 1;
	}

	if ((idx = malloc(SUBSET*sizeof(int))) == NULL) {
		printf("Failed to allocte memory");
		return 1;
	}

	// inint sums
	memset(sums, -1, MAXSUMS*sizeof(char)+1);

	// init set
	for (i = 1; i <= MAXSET; i++)
		set[i-1] = i*i;

	// scan all possible 50 elements and sum their values
	for (i = 0; i < SUBSET; i++)
		idx[i] = i;


	y = SUBSET-1;
	idx[y]--;

	while (y > -1) {

		if (idx[y] < MAXSET-SUBSET+y) {
			idx[y]++;
			for (i = y+1; i < SUBSET; i++)
				idx[i] = idx[i-1]+1;
			y = SUBSET-1;

			for (x = idx[y]; x < MAXSET; x++) {
				lc++;
				printf("[%2d] { ", lc);
				c = 0;
				for (i = 0; i < SUBSET; i++) {
					printf("%3d ", set[idx[i]]);
					c += set[idx[i]];
				}
				printf("} sum = %4d\n", c);
				sums[c]++;
				idx[y]++;
			}
		}

		while (idx[y] >= MAXSET-SUBSET+y)
			y--;
	}


	// scan sums and sun all true indecs
	printf("Not Unique sums\n");
	for (i = 0, lc = 0; i < MAXSUMS; i++) {
		if (sums[i] == 0) {
			S += i;
		}
		else if (sums[i] > 0) {
			lc++;
			printf("[%d] %d\n", lc, i);
		}

	}

	printf("Sums of all integers: %d\n ", S);

	return 0;

}