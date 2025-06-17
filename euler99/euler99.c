/*
Problem:
Comparing two numbers written in index form like 211 and 37 is not difficult, as any calculator would confirm that 211 = 2048 < 37 = 2187.
However, confirming that 632382518061 > 519432525806 would be much more difficult, as both numbers contain over three million digits.
Using base_exp.txt (right click and 'Save Link/Target As...'), a 22K text file containing one thousand lines with a base/exponent pair on each line, determine which line number has the greatest numerical value.
NOTE: The first two lines in the file represent the numbers in the example given above.

 Solution by Yoram Finder:
 Using iteration (see bec() below)
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define INPUT "./base_exp.txt"

/**
 * compare to base_exp pairs and find which one is bigger
 * int b1, b2 - bases
 * int e1, e2 - exponents
 * 
 * since we deal with very large base and exponents we can't use regular pow() function to
 * calculate the value. we need to iterate and reduce the base and exp numbers as much as possible
 * and only then use pow() to calculate the remaining value and compare
 */ 
double bec (int b1, int e1, int b2, int e2) {
	double b[2];
	int e[2];
	int side = 0;

	b[0] = b1;
	b[1] = b2;
	e[0] = e1;
	e[1] = e2;

	// stop iterate once one side has a power of 1 or 0
	while ((e[0] > 1) && (e[1] > 1)) {
		// first check if both base and exp of one pair are greater the the others.
		// if they do, then just return. no need to contine iterate or calculate pow()
		if ((e[0] >= e[1]) && (b[0] >= b[1])) {
			return -1;
		}
		else if ((e[1] >= e[0]) && (b[1] >= b[0])) {
			return 1;
		}

		// always work on the side where exponent is bigger
		if (e[side] < e[1-side])
			side = 1-side;
		
		e[side] -= e[1-side];
		b[1-side] /= b[side];
	}

	return  (pow(b[1], (double)e[1]) - pow(b[0], (double)e[0]));
}

int main () {
	char buf [100];
	FILE *fp;
	int line = 1;
	int b1, b2, e1, e2;
	struct {
		int b;
		int e;
		int l;
	} base_exp;

	if ((fp = fopen(INPUT, "r")) == NULL) {
		printf("Failed to open %s: %s", INPUT, strerror(errno));
		return -1;
	}

	// read first base_exp and store in base_exp 
	if (fgets(buf, 99, fp) == NULL) {
		printf("Failed to read file: %s", strerror(errno));
		return -1;
	}
	sscanf(buf, "%d,%d", &base_exp.b, &base_exp.e);
	base_exp.l = line;

	// go through all base_exp pairs in file and compare
	while (fgets(buf, 99, fp) != NULL) {
		line++;
		b1 = base_exp.b;
		e1 = base_exp.e;
		sscanf(buf, "%d,%d", &b2, &e2);
		if (bec(b1, e1, b2, e2) > 0) {
			// new pair is bigger - save it
			base_exp.b = b2;
			base_exp.e = e2;
			base_exp.l = line;
		}
	}

	printf("Base_exp: %d, %d, line:%d\n", base_exp.b, base_exp.e, base_exp.l);

	fclose(fp);
}