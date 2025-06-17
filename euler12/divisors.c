/*
* int divisors (unsigned int)
* return the number of divisors for a giben number
*/

#include <math.h>

int divisors (unsigned long number) {
	unsigned long iterator;
	int count = 0;
	unsigned long sq = (unsigned long)sqrt((double)number);

	// we need to check up to square root of the number and double by 2
	for (iterator = 1; iterator <= sq; iterator++) {
		if (number % iterator == 0)
			count += 2;
	}

	// check if we have a perfect root. if we do, decrement count by 1, since we
	// don't want to count the root value twice
	if (sq * sq == number)
		count--;

	return count;
}