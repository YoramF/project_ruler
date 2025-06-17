/*
Let S(A) represent the sum of elements in set A of size n. We shall call it a special sum set if for any two non-empty disjoint subsets, B and C, the following properties are true:

S(B) ≠ S(C); that is, sums of subsets cannot be equal.
If B contains more elements than C then S(B) > S(C).
For this problem we shall assume that a given set contains n strictly increasing elements and it already satisfies the second rule.

Surprisingly, out of the 25 possible subset pairs that can be obtained from a set for which n = 4, only 1 of these pairs need to be tested for equality (first rule). Similarly, when n = 7, only 70 out of the 966 subset pairs need to be tested.

For n = 12, how many of the 261625 subset pairs that can be obtained need to be tested for equality?
*
*
Solution by Yoram Finder:
There might be a mathemtic solution for this. I will use the iteration solution.
I will use a set of 12 numbers that follow the basic requirement. I will do all checks on the number position
regardless their values.
Rule3: since all elements in each subset are ordered, and since subset pairs are constructed
by traversing all subset from smallest values to gigger, we just need parse the elements on both
subsets and see if we have any case where comparing two corespoding elements, element on the first
subset is grater than the other. If it does, this pair will need to be tested for equality.
The algorithm first generates the 261625 subsets pairs and then check only those that have 
same number of elements, and count the pairs that comply with rule3.

Total subsetPairs: 261625
Need to be tested: 21384

real    0m1.278s
user    0m0.015s
sys     0m0.015s
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _subset {
	int num[12];
	int size;
	unsigned int mask;
	bool done;
	struct _subset *next;
} SUBSET;

typedef struct _pair {
	SUBSET *s1;
	SUBSET *s2;
	struct _pair *next;
} PAIR;

typedef struct _set {
	int num[12];
	int size;
	SUBSET *subsets[12];
	PAIR *subsetPairs[6];	// largest pair of subsets qualify for test can't have more than 12/2=6 elements
} SET;

void freeSubsets (SUBSET *p[], int size) {
	SUBSET *sp, *np;
	int i;

	for (i = 0; i < size; i++) {
		sp = p[i];
		while (sp != NULL) {
			np = sp->next;
			free(sp);
			sp = np;
		}
	}
}

void freeSubsetPairs (PAIR *p[], int size) {
	PAIR *sp, *np;
	int i;

	for (i = 0; i < size; i++) {
		sp = p[i];
		while (sp != NULL) {
			np = sp->next;
			free(sp);
			sp = np;
		}
	}
}

void freeSet (SET *s) {
	freeSubsets(s->subsets, s->size);
	freeSubsetPairs(s->subsetPairs, s->size/2);
}


unsigned int setBit (unsigned int mask, int pos) {
	return ((1 << pos) | mask);
}

void addSet (SUBSET **sets, SUBSET *set) {
	SUBSET *sptr;

	sptr = sets[set->size-1];

	if (sptr != NULL) {
		while (sptr->next != NULL)
			sptr = sptr->next;
		sptr->next = set;
		}
	else {
		sptr = set;
		sets[set->size-1] = set;
	}
}

/**
 * insert new pair to list. S(sub1) must be smaller or equal to S(sub2)
 */
bool insertSubsetPairs (SET *s, SUBSET *sub1, SUBSET *sub2) {
	PAIR *p, *np;

	if ((np = calloc(1, sizeof(PAIR))) == NULL) {
		printf("Error allocating RAM: %s\n", strerror(errno));
		return false;		
	}

	np->s1 = sub1;
	np->s2 = sub2;
	p = s->subsetPairs[sub1->size-1];

	if (p != NULL) {
		while (p->next != NULL)
			p = p->next;
		p->next = np;
		}
	else {
		p = np;
		s->subsetPairs[sub1->size-1] = np;
	}

	return true;
}

/**
 * find all subsets of a given set excluding empty set but including the whole set
 * a set of integers represented as an array of integers
 * 
 */ 
SUBSET *findSubsets (SET *set, int size) {
	int i, j;
	SUBSET *sp, *nsp;

	if (size > 1) {
		// first find all subset recursively
		if (findSubsets(set, size-1) == NULL)
			return NULL;

		// parse shorter sets and insert new sets with number[n] included
		// at this point all sets have size-1 elements
		// in order to avoid duplicates we need to parse from size-1 down to 0
		for (i = size-1; i >= 0; i--) {
			sp = set->subsets[i];
			while (sp != NULL) {
				if ((nsp = calloc(1, sizeof(SUBSET))) == NULL) {
					printf("Error allocating RAM: %s\n", strerror(errno));
					return NULL;
				}

				for (j = 0; j < sp->size; j++) {
					nsp->num[j] = sp->num[j];
				}
				nsp->num[j] = set->num[size-1];
				nsp->size = sp->size + 1;
				nsp->mask = setBit(sp->mask, size-1);
				addSet(set->subsets, nsp);
				sp = sp->next;
			}
		}
	}

	// add single number set[size-1]
	if ((nsp = calloc(1, sizeof(SUBSET))) == NULL) {
		printf("Error allocating RAM: %s\n", strerror(errno));
		return NULL;
	}
	nsp->num[0] = set->num[size-1];
	nsp->size = 1;
	nsp->mask = setBit(0,size-1);
	addSet(set->subsets, nsp);
}

/**
 * Parse list of subsets and generate a new list
 * of subset pairs. Sort them according to their length
 * return number of subset pairs found or -1 if error
 * side effetc: update s->allPairs counter (just for testing)
 */
int getSubsetPairs (SET *s) {
	int i, j, ls;
	SUBSET *sb, *se;
	int pairs = 0;

	ls = s->size / 2;

	for (i = 0; i < ls; i++) {
		sb = s->subsets[i];
		while (sb != NULL) {
			for (j = i; j < s->size-1; j++) {
				se = s->subsets[j];
				while (se != NULL) {
					// check if this subset was already paired with all subsets
					if (!se->done) {
						// check for distinct subsets
						if (!(se->mask & sb->mask)) {
							pairs++;	
							// now check if they are eligible for testing
							if (se->size == sb->size) {
								if (!insertSubsetPairs(s, sb, se)) {
									return -1;
								}
							}
						}
					}
					se = se->next;
				}
			}
			// once we found all pairs with sb, it needs to be marked as used
			sb->done = true;
			sb = sb->next;
		}
	}

	return pairs;
}

/**
 * return true if rule3 as descibe in the solution above applies to these two subsets
 */
bool rule3 (PAIR *p) {
	int size = p->s1->size;
	int i, b = 0;
	SUBSET *s1 = p->s1, *s2 = p->s2;

	// if we find an element in s1 that is bigger than its corespoding element in s2
	// return true
	for (i = 0; i < size; i++) {
		if (s1->num[i] > s2->num[i]) {
			return true;
		}
	}

	return false;
}

// got through all qualified subsets and see who many need to be tested
int needToBeTest (SET *s) {
	int i, size = s->size / 2, c = 0;
	PAIR *p;

	for (i = 0; i < size; i++) {
		p = s->subsetPairs[i];
		while (p != NULL) {
			if (rule3(p)) {
				c++;
			}
			p = p->next;
		}
	}

	return c;
}

void main () {
	SET set = {
		{1,2,3,4,5,6,7,8,9,10,11,12},
		12,
		{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
		{NULL,NULL,NULL,NULL,NULL,NULL}
	};
	int subsetsPairs = 0;
	int needTest = 0;

	if (findSubsets(&set, set.size) != NULL) {
		if ((subsetsPairs = getSubsetPairs(&set))> 0) {
			needTest = needToBeTest(&set);
			printf("Total subsetPairs: %d\n",subsetsPairs);
			printf("Need to be tested: %d\n", needTest);
		}
	}

	freeSet(&set);
}