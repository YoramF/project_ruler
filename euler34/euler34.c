#include <stdio.h>

/*
145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: As 1! = 1 and 2! = 2 are not sums they are not included.

Solution:
Factorial values can be between 0!=1 end 9!=362880
Max limit to check a number that is less or equal to 9! multiply by the number's number of digits
It is easy to see that a 7 digits number will be greater than 7*9! so we will take 
this number as our limit 7*9! =  2,540,160
*/

const int factorials[] = {1,1,2,6,24,120,720,5040,40320,362880};
const int maxNum = 2540160;

int calcSumFact (int v) {
	int s = 0;
	int d1;

	while (v > 0) {
		d1 = v % 10;
		s += factorials[d1];
		v /= 10;
	}

	return s;
}

int main () {
	int c = 0, s = 0;
	int csf;

	for (int i = 3; i < maxNum; i++) {
		csf = calcSumFact(i);
		if (i == csf) {
			c++;
			s += i;
		}
	}
	printf("result: %d\n",s);
}

