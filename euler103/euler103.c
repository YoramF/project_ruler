/*
Let S(A) represent the sum of elements in set A of size n. We shall call it a special sum set if for any two non-empty disjoint subsets, B and C, the following properties are true:

S(B) ≠ S(C); that is, sums of subsets cannot be equal.
If B contains more elements than C then S(B) > S(C).
If S(A) is minimised for a given n, we shall call it an optimum special sum set. The first five optimum special sum sets are given below.

n = 1: {1}
n = 2: {1, 2}
n = 3: {2, 3, 4}
n = 4: {3, 5, 6, 7}
n = 5: {6, 9, 11, 12, 13}
It seems that for a given optimum set, A = {a1, a2, … , an}, the next optimum set is of the form B = {b, a1+b, a2+b, … ,an+b}, where b is the “middle” element on the previous row.

By applying this “rule” we would expect the optimum set for n = 6 to be A = {11, 17, 20, 22, 23, 24}, with S(A) = 117. However, this is not the optimum set, as we have merely applied an algorithm to provide a near optimum set. The optimum set for n = 6 is A = {11, 18, 19, 20, 22, 25}, with S(A) = 115 and corresponding set string: 111819202225.

Given that A is an optimum special sum set for n = 7, find its set string.

Solution by Yoram Finder Nov 2020:
given n = 6 A = {11, 17, 20, 22, 23, 24},
for n = 7 applying the rule B = {22, 33, 39, 42, 44, 45, 46} S(B) = 271
Applying an optimization algorithm on B we get S(B`) = {20, 31, 38, 39, 40, 42, 45} = 255
Solution: 20313839404245

The optimization takes the base set generated using the rule, and generates a list of permutations
where each element value is changed from -4 to +4 (Assuming the basic rule generate a close enough correct set
checking for +/-4 per element should be safe). during the process of building this super list
sets that do not comply with basic rules (like: all elements in a set must be uniqu and in ascending order)
Once we have the initial super list, we generate all possible subsets per list and then apply the
two rules on these subsets. Only sets that comply with the rules stay.
Then we search for the set with the smallest sum - this is the problem solution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

typedef struct _subSet {
	short num[7];
	short size;
	short sum;
	short mask;
	struct _subSet *next;
} SUBSET;

typedef struct _set {
	short num[7];
	short size;
	short sum;
	SUBSET *subsets[7];
	struct _set *next;
} SET;


void freeSubsets (SUBSET *s) {
	SUBSET *sp;

	while (s != NULL) {
		sp = s->next;
		free(s);
		s = sp;
	}
}

bool compSets (SET *s1, SET *s2) {
	int i;
	if (s1->size == s2->size) {
		for (i = 0; i < s1->size; i++) {
			if (s1->num[i] != s2->num[i])
				return false;
		}
		return true;
	}
	return false;
}

void freeSets (SET *s) {
	SET *sp;
	int i;

	while (s != NULL) {
		sp = s->next;
		for (i = 0; i < s->size; i++)
			freeSubsets(s->subsets[i]);

		free(s);
		s = sp;
	}
}

short setBit (short mask, short pos) {
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

				for (j = 0; j < sp->size; j++)
					nsp->num[j] = sp->num[j];
				nsp->num[j] = set->num[size-1];
				nsp->sum = sp->sum + set->num[size-1];
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
	nsp->sum = set->num[size-1];
	nsp->size = 1;
	nsp->mask = setBit(0,size-1);
	addSet(set->subsets, nsp);
}

/**
 * return true only if it has uniq members in ascending order
 */
bool checkSet (SET *s) {
	int i = 1, size;
	bool b;

	b = (s != NULL);
	if (b) {
		while (b && (i < s->size)) {
			b = (s->num[i-1] < s->num[i]);
			i++;
		}
	}

	return b;
}

/**
 * cartesian product of two lists of sets
 * the result sets must comply with the problem description:
 * and that has unique members in ascending order
 */
SET *cartesianProduct (SET *s1, SET *s2) {
	SET *rs = NULL, *cp = NULL, *p = NULL, *sp1 = s1, *sp2;
	int i, j;

	while (sp1 != NULL) {
		sp2 = s2;
		while (sp2 != NULL) {
			if (p == NULL) {
				if ((p = calloc(1, sizeof(SET))) == NULL) {
					printf("Error allocating RAM: %s\n", strerror(errno));
					freeSets(rs);
					return NULL;	
				}
			}

			p->sum = sp1->sum + sp2->sum;

			if (rs == NULL) {
				rs = p;
			}

			p->size = sp1->size + sp2->size;

			j = 0;
			for (i = 0; i < sp1->size; i++)
				p->num[j++] = sp1->num[i];

			for (i = 0; i < sp2->size; i++)
				p->num[j++] = sp2->num[i];

			// and only if its members are unique and in ascending order
			if (checkSet(p)) {
				if (cp != NULL)
					cp->next = p;
				cp = p;
				p = NULL;
			}
			sp2 = sp2->next;
		}
		sp1 = sp1->next;
	}

	return rs;
}

/**
 * generate set of sets out of set using dv as a diviator (+/-) applied to each member
 * in the set
 * 
 */ 
SET *findAllSets (int baseSet[], int size, int dv) {
	int i, j, d, n, baseSum;
	SET *nsp;
	SET **tsp, *ctsp, *ntsp;

	// first calculate sum of base set elements
	baseSum = 0;
	for (i = 0; i < size; i++)
		baseSum += baseSet[i];


	// allocate array of permutation sets from which we will generate a list of sets
	// using cartesian factors
	if ((tsp = calloc(size, sizeof(SET))) == NULL) {
		printf("Error allocating RAM: %s\n", strerror(errno));
		return NULL;
	}

	// generate the permutation sets
	// these set sets will be used to generate the final list from which
	// we sill get the solution
	for (i = 0; i < size; i++) {
		n = baseSet[i];
		ctsp = NULL;
		for (d = -dv; d <= dv; d++) {
			if ((ntsp = calloc(1, sizeof(SET))) == NULL) {
				printf("Error allocating RAM: %s\n", strerror(errno));
				for (j = 0; j < i; j++)
					freeSets(tsp[j]);
				return NULL;
			}
			if (ctsp != NULL)
				ctsp->next = ntsp;
			else
				tsp[i] = ntsp;
			
			ntsp->num[0] = n+d;
			ntsp->sum = ntsp->num[0];
			ntsp->size = 1;
			ctsp = ntsp;
		}
	}

	// apply cartesian factror on all permutation sets
	// by the end of the iteration process, nsp will point to the final list
	nsp = tsp[0];
	for (i = 1; i < size; i++) {
		nsp = cartesianProduct(nsp, tsp[i]);
		freeSets(tsp[i]);
	}
	freeSets(tsp[0]);
	free(tsp);

	return nsp;	
}

/**
 * pars all sets and allocate the set with the smallest sum
 * convers the set to sting and that is the problem solution
 */
SET *findSmallestSet (SET *s) {
	SET *cs = NULL;
	int sum = 0x7FFF; //max short;

	while (s != NULL) {
		if (s->sum < sum) {
			cs = s;
			sum = s->sum;
		}
		s = s->next;
	}
	return cs;
}

/**
 * check for
 * S(B) ≠ S(C); that is, sums of subsets cannot be equal.
 * return true if none
 */
bool rule1 (SET *s) {
	int i, j, ls;;
	SUBSET *sb, *se;

	//find largest subset we need to check
	//since we check two disjoint subsets, largest suset is 2->size / 2
	ls = s->size / 2;

	//now compare all subsetls with ls or less elements
	for (i = 0; i < ls; i++) {
		sb = s->subsets[i];
		while (sb != NULL) {
			for (j = i; j < ls; j++) {
				se = s->subsets[j];
				while (se != NULL) {
					// check that we are not comparing the same subset
					if (se != sb) {
						// then check that the two subsets do not have same elements
						if (!(sb->mask & se->mask)) {
							if (sb->sum == se->sum) {
								return false;
							}
						}
					}
					se = se->next;
				}
			}
			sb = sb->next;
		}
	}

	// we did not find any two disjoint sets with equal sum
	return true;
}

/**
 * check if for
 * If B contains more elements than C then S(B) > S(C).
 * return true or false
 */
bool rule2 (SET *s) {
	int i, j, ls;
	SUBSET *sb, *se;

	ls = s->size / 2;

	for (i = 0; i < ls; i++) {
		sb = s->subsets[i];
		while (sb != NULL) {
			for (j = i+1; j < s->size-1; j++) {
				se = s->subsets[j];
				while (se != NULL) {
					if (se->size > sb->size) {
						if (se->mask && sb->mask) {
							if (sb->sum >= se->sum) {
								return false;
							}
						}
					}
					se = se->next;
				}
			}
			sb = sb->next;
		}
	}

	return true;
}


/**
 * check if a given set is comply with both rules
 */
bool validSet (SET *s) {
	return (rule1(s) && rule2(s));
}

void main () {
	int set[7] = {22, 33, 39, 42, 44, 45, 46};
	int i, j, c = 0;
	SET *sets, *s, *bs;
 
	sets = findAllSets(set, 7, 4);

	// generate subsets
	s = sets;
	bs = NULL;
	while (s != NULL) {
		findSubsets(s, s->size);
		if(!validSet(s)) {
			if (bs == NULL) {
				sets = s->next;
				free(s);
				s = sets;
			}
			else {
				bs->next = s->next;
				free(s);
				s = bs->next;
			}
		}
		else {
			bs = s;
			s = s->next;
		}
	}

	s = findSmallestSet(sets);

	//generate the required string for solution
	printf("Solution: ");
	for (i = 0; i < s->size; i++)
		printf("%d", s->num[i]);
	printf("\n");

	freeSets(sets);
}