/*
A positive integer  is stealthy, if there exist positive integers a, b, c, d such that a*b = c*d = N and a+b = c+d+1.
For example, 36 = 4*9 = 6*6 is stealthy.

You are also given that there are 2851 stealthy numbers not exceeding 10^6.

How many stealthy numbers are there that don't exceed 10^14?

Solution:
	First attempt would be scanning all possible options for N <= 10^14
	This takes forever.
	
	Stealthy numbers are actually "bipronic number". These are numbers of the form x*(x+1) * y*(y+1), where x and y are positive integers.
	So the solution would be to generate these numbers up to 10^14 and make sure
	we are not counting same number multiple times. For that we imlement insert() function
	which returns 1 if the number was inserted or 0 if the number was already in the set

	run time:
	real    0m0.128s
	user    0m0.000s
	sys     0m0.015s
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"

static long long int x,y,v;
static long long int cnt = 0LL;

#define HASH 999983 // prime numbers usually generate a balance hash results

int main (int argc, char **argv) {
	long long int limit, N;
	table *t = NULL;

	int new_value;

	if (argc < 2) {
		printf("euler757: Usage: euler757 <N>\n");
		return -1;
	}

	N = atoll(argv[1]);

	// get max iteration limit for a & c
	limit = pow(N, 0.25);

	for (x = 1; x <= limit; x++) {

		y = x;

		while (1) {
			v = x*(x+1)*y*(y+1);
			if (v > N)
				break;

			if ((new_value = insert(&t, v, HASH)) > 0) {
				cnt++;
			}	
			else if (new_value < 0)
				return -1;

			y++;
		}
	}

	printf("There are %I64d stealthy numbers not exceeding %I64d\n", cnt, N);
}

