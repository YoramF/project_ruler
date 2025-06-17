/*
Each of the six faces on a cube has a different digit (0 to 9) written on it; the same is done to a second cube. By placing the two cubes side-by-side in different positions we can form a variety of 2-digit numbers.
For example, the square number 64 could be formed:

In fact, by carefully choosing the digits on both cubes it is possible to display all of the square numbers below one-hundred: 01, 04, 09, 16, 25, 36, 49, 64, and 81.
For example, one way this can be achieved is by placing {0, 5, 6, 7, 8, 9} on one cube and {1, 2, 3, 4, 8, 9} on the other cube.
However, for this problem we shall allow the 6 or 9 to be turned upside-down so that an arrangement like {0, 5, 6, 7, 8, 9} and {1, 2, 3, 4, 6, 7} allows for all nine square numbers to be displayed; otherwise it would be impossible to obtain 09.
In determining a distinct arrangement we are interested in the digits on each cube, not the order.

{1, 2, 3, 4, 5, 6} is equivalent to {3, 6, 4, 1, 2, 5}
{1, 2, 3, 4, 5, 6} is distinct from {1, 2, 3, 4, 5, 9}

But because we are allowing 6 and 9 to be reversed, the two distinct sets in the last example both represent the extended set {1, 2, 3, 4, 5, 6, 9} for the purpose of forming 2-digit numbers.

How many distinct arrangements of the two cubes allow for all of the square numbers to be displayed?

Solution: Yoram Finder Dec 2020
===============================
Use sorted sets
Sets are built with numbers from 0 - 9
There are C(10,6)=210 0-9 set options and (210^2)/2 distinct pairs
Create all set options and then generate the distinct pairs and per pair check if it has
all required digits to generate all 9 squars numbers. 
Solution: 1217
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HASH_SIZE 1471

typedef struct _node {
	long long int val;
	struct _node *next;
} NODE;

int sNum [9][2] = {{0,1}, {0,4}, {0,9}, {1,6}, {2,5}, {3,6}, {4,9}, {6,4}, {8,1}};

/**
 * return value of set
 */
long long int evalSet (int s[]) {
	int i;
	long long int v = 0;

	for (i = 0; i < 6; i++) {
		v = 10 * v + s[i];
	}

	return v;
}

/**
 * Insert new value into hash table. Return true if value was added or false if value was alredy in table
 */
bool hash (NODE *hTbl, long long int val) {
	NODE *p, *bp;
	int ind = val % HASH_SIZE;

	if (hTbl[ind].val == val) {
		return false;
	}

	if (hTbl[ind].val == 0) {
		hTbl[ind].val = val;
		return true;
	}
	else {
		bp = NULL;
		p = hTbl[ind].next;
		while (p != NULL) {
			if (p->val == val) {
				return false;
			}
			bp = p;
			p = p->next;
		}
		p = calloc(1, sizeof(NODE));
		p->val = val;
		if (bp == NULL) {
			hTbl[ind].next = p;
		}
		else {
			bp->next = p;
		}
		return true;
	}
}

int initSets (int s[210][6]) {
	int i1, i2, i3, i4, i5, i6;
	int c = 0;

	for (i1 = 0; i1 < 5; i1++) {
		for (i2 = i1+1; i2 < 6; i2++) {
			for (i3 = i2+1; i3 < 7; i3++) {
				for (i4 = i3+1; i4 < 8; i4++) {
					for (i5 = i4+1; i5 < 9; i5++) {
						for (i6 = i5+1; i6 < 10; i6++) {
							s[c][0] = i1;
							s[c][1] = i2;
							s[c][2] = i3;
							s[c][3] = i4;
							s[c][4] = i5;
							s[c][5] = i6;
							c++;
						}
					}
				}
			}
		}
	}

	return c;
}

/**
 * check if number n is in set s
 */
bool inSet (int s[], int n) {
	int i;

	for (i = 0; i < 6; i++) {
		if (s[i] == n) {
			return true;
		}
		else if ((n == 9) && (s[i] == 6)){
			return true;
		}
		else if ((n == 6) && (s[i] == 9)) {
			return true;
		}
	}

	return false;
}

/**
 * Check if s1 & s2 contains all digits needed to represent all square numbers below one-hundred
 */
bool isPair (int s1[], int s2[]) {
	int i;
	unsigned n1, n2;

	for (i = 0; i < 9; i++) {
		n1 = 0;
		n2 = 0;
		if (inSet(s1, sNum[i][0])) {
			n1++;
		}
		if (inSet(s2, sNum[i][0])) {
			n1 += 2;
		}
		if (inSet(s1, sNum[i][1])) {
			n2++;
		}
		if (inSet(s2, sNum[i][1])) {
			n2 += 2;
		}
		if ((n1 == 0) || (n2 == 0)) {
			return false;
		}
		if ((n1 == n2) && (n1 != 3)) {
			return false;
		}
	}

	return true;
}


void pSet (int s[], int n) {
	int i;

	printf("( ");
	for (i = 0; i < n; i++) {
		printf("%d ", s[i]);
	}
	printf(")");
}

void main () {
	int sets[210][6];
	int n, is1, is2;
	int c = 0;
	bool bs1, bs2;
	long long int es1, es2;
	NODE *hashTbl;

	n = initSets(sets);

	hashTbl = calloc(HASH_SIZE, sizeof(NODE));

	for (is1 = 0; is1 < n; is1++) {
		for (is2 = 0; is2 < n; is2++) {
			if (isPair(sets[is1], sets[is2])) {
				es1 = evalSet(sets[is1]);
				es2 = evalSet(sets[is2]);
				bs1 = hash(hashTbl, es1*1000000L+es2);
				bs2 = hash(hashTbl, es2*1000000L+es1);
				if (bs1 && bs2) {
					c++;
				}
			}
		}
	}

	printf("PAIRS: %d\n==============\n", c);
}