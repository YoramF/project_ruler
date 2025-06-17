/**
 * Problem:
 * ========
 * n! means n × (n − 1) × ... × 3 × 2 × 1

 * For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
 * and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

 * Find the sum of the digits in the number 100!
 * 
 * Solution: Yoram Finder
 * ======================
 * 100! is a 158 digits number. As a first try I will go with naive approach of
 * representing the n! result as an array of digits and do the calculations on
 * the digits level. Once I have the result, I will sum the values of all digits
 * 
 * Result = 648
 */

#include <stdio.h>

/**
 * After nultiplying each digit with the multiplier we need to add
 * all sub results into the result
 * s_r = array of multiple sub results
 * d_a = array of digits
 * digits = number of digits in origin number
 * returns the number of digits in new result
 */
int mult_add (int s_r[], int digits, char d_a[]) {
	int i, j = 0;

	for (i = 0; i < digits-1; i++) {
		s_r[i+1] = s_r[i] + s_r[i+1]*10;
		d_a[j] = s_r[i+1] % 10;
		s_r[i+1] /= 10;
		j++;
	}

	while (s_r[i] > 0) {
		d_a[j] = s_r[i] % 10;
		s_r[i] /= 10;
		j++;
	}

	return j;
}

/**
 * Multiply number represented as array of digits by n
 * d_a = digits array
 * sigits = number of digits
 * n = multiplier
 * s_r = array of sub results
 */
void mult (char d_a[], int digits, int n, int s_r[]) {
	int i;

	for (i = 0; i < digits; i++)
		s_r[i] = d_a[i] * n;
}

void main () {
	int i, digits = 1, sum = 0;
	char d_a[200] = {1};
	int s_r[200];

	// calculate 100!
	for (i = 1; i < 100; i++) {
		mult(d_a, digits, i+1, s_r);
		digits = mult_add(s_r, digits, d_a);
	}

	i = digits - 1;
	while (i >= 0) {
		printf("%d",d_a[i]);
		sum += d_a[i];
		i--;
	}

	printf("\nResult: %d", sum);
}