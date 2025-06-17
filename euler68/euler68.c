
/*
* Solution by: Yoram Finder
*
Problems:
==========
Consider the following "magic" 3-gon ring, filled with the numbers 1 to 6, and each line adding to nine.


Working clockwise, and starting from the group of three with the numerically lowest external node (4,3,2 in this example), each solution can be described uniquely. For example, the above solution can be described by the set: 4,3,2; 6,2,1; 5,1,3.

It is possible to complete the ring with four different totals: 9, 10, 11, and 12. There are eight solutions in total.

Total	Solution Set
9	4,2,3; 5,3,1; 6,1,2
9	4,3,2; 6,2,1; 5,1,3
10	2,3,5; 4,5,1; 6,1,3
10	2,5,3; 6,3,1; 4,1,5
11	1,4,6; 3,6,2; 5,2,4
11	1,6,4; 5,4,2; 3,2,6
12	1,5,6; 2,6,4; 3,4,5
12	1,6,5; 3,5,4; 2,4,6
By concatenating each group it is possible to form 9-digit strings; the maximum string for a 3-gon ring is 432621513.

Using the numbers 1 to 10, and depending on arrangements, it is possible to form 16- and 17-digit strings. What is the maximum 16-digit string for a "magic" 5-gon ring?

propose solution:
- the request to look only into 16 digits strings means that number 10 should be
included only once in a a 3 numbers combination, i.e. it must be on an outer node.
- the 3 digits combination with number 10 can't include 9 or 8 since no other digits
combination can equal 27.
- there might be other restrictions, but I will let the program run on all remaining
combinations anyway

*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// representing magic 5-gone ring
int digits [10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 5 threes - each number represent offset into the digits array
// 1st number of each three is external node
// since first external node should be the smaller between all external nodes
// it can't be '10'
const int threes [5][3] = {
	{0, 6, 7},
	{1, 7, 8},
	{2, 8, 9},
	{3, 9, 5},
	{4, 5, 6}
};

// initial result smallest digit string possible
char result [17] = "0000000000000000";
char sets [60];
int sumT;
unsigned iterations = 0;
unsigned combinations = 0;

/**
 * Check if current digits string is bigger than current result. If it does, update
 * result to current digits string
 */
void checkDigitString () {
	int i,j, k = 0, s = 0;
	char cds[17];

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 3; j++) {
			if (digits[threes[i][j]] == 10) {
				cds[k++] = '1';
				cds[k++] = '0';
				sets[s++] = '1';
				sets[s++] = '0';
			}
			else {
				cds[k++] = digits[threes[i][j]] + '0';
				sets[s++] = digits[threes[i][j]] + '0';
			}
		}
		sets[s++] = ',';
	}

	cds[k] = '\0';
	sets[s] = '\0';

	// compare cds with result and update result if cds is bigger
	if (strcmp(cds, result) > 0)
		strcpy(result, cds);
};

/**
 * check if digits combination is a valid magic 5-gone combination
 * returns true if it does
 */
bool isValidGone () {
	int i, n;
	int t[5];

	// first check if first external node has lowest value among other external nodes
	n = digits[0];
	for (i = 1; i < 5; i++)
		if (digits[threes[i][0]] < n)
			return false;

	// now chech that all threes totals are equal
	for (i = 0; i < 5; i++)
		t[i] = digits[threes[i][0]] + digits[threes[i][1]] + digits[threes[i][2]];

	n = t[0];
	for (i = 1; i < 5; i++)
		if (n != t[i])
			return false;

	sumT = n;
	return true;
};

/**
 * recursive function
 * sNum - array of selected numbers
 * rNum - array of remaining numbers to user
 * sn - number of selected numbers
 * rn - number of remaining numbers
 */
void combination (int sNum[], int rNum[], int sn, int rn) {
	int i = 0 , j, k;
	int rDigits [10];

	iterations++;

	if (rn > 1) {
		while (i < rn) {
			sNum[sn] = rNum[i];
			k = 0;
			for (j = 0; j < rn; j++)
				if (rNum[j] != rNum[i])
					rDigits[k++] = rNum[j];
			combination(sNum, rDigits, sn+1, rn-1);
			i++;
		}
	}
	else {
		combinations++;
		sNum[sn] = rNum[0];
		if (isValidGone()) {
			checkDigitString();
			printf("%d => %s\n", sumT, sets);
		};
	}
}


int main () {
	int rDigits [10] = {1,2,3,4,5,6,7,8,9,10};
	combination(digits, rDigits, 0, 10);
	printf("Largest 16 digits string: %s\n", result);
	printf("Overall combonations: %u, overall iterations: %u\n", combinations, iterations);
}

