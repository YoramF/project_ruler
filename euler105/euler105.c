/*
Let S(A) represent the sum of elements in set A of size n. We shall call it a special sum set if for any two non-empty disjoint subsets, B and C, the following properties are true:

S(B) ≠ S(C); that is, sums of subsets cannot be equal.
If B contains more elements than C then S(B) > S(C).
For example, {81, 88, 75, 42, 87, 84, 86, 65} is not a special sum set because 65 + 87 + 88 = 75 + 81 + 84, whereas {157, 150, 164, 119, 79, 159, 161, 139, 158} satisfies both rules for all possible subset pair combinations and S(A) = 1286.

Using sets.txt (right click and "Save Link/Target As..."), a 4K text file with one-hundred sets containing seven to twelve elements (the two examples given above are the first two sets in the file), identify all the special sum sets, A1, A2, ..., Ak, and find the value of S(A1) + S(A2) + ... + S(Ak).

Solution: By Yoram Finder
-------------------------
Based on solution for problem 103, we just need to read the file.
For each new set, first check that it does not have duplicate elements.
Then generate its subsets and check rule1 & rule2. If pass, then add its sun to the total sum 
S(A1)+..S(Ak) = 73702
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _subset {
	int num[15];
	int size;
	int sum;
	unsigned int mask;
	struct _subset *next;
} SUBSET;

typedef struct _set {
	int num[15];
	int size;
	int sum;
	SUBSET *subsets[15];
} SET;

void freeSubsets (SUBSET *s) {
	SUBSET *sp;

	while (s != NULL) {
		sp = s->next;
		free(s);
		s = sp;
	}
}

void freeSet (SET *s) {
	int i;

	for (i = 0; i < s->size; i++)
		freeSubsets(s->subsets[i]);
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
 * return true only if it has uniq members
 */
bool checkSet (SET *s) {
	int i, j, size = s->size;
	bool b = true;

	for (i = 0; (i < size-1) && b; i++) {
		for (j = i+1; (j < size) && b; j++)
			if (s->num[i] == s->num[j])
				b = false;
	}

	return b;
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
	char line[100], dlm[2] = ",";
	FILE *fp;
	char *token;
	int sum = 0, bad = 0, size;
	SET set;

	fp = fopen("./sets.txt", "r");
	while ((fgets(line, 99, fp))!= NULL) {
		memset(&set, 0, sizeof(SET));
		size = 0;
		token = strtok(line, dlm);
		while (token != NULL) {
			set.num[size] = atoi(token);
			set.sum += set.num[size];
			size++;
			token = strtok(NULL, dlm);
		}
		set.size = size;

		if (checkSet(&set)) {
			if (findSubsets(&set, set.size) != NULL) {
				if (validSet(&set)) {
					sum += set.sum;
				}
				else {
					bad++;
				}				
			}
			else {
				bad++;
			}
		}
		else {
			bad++;
		}
		freeSet(&set);
	}

	printf("SUM: %d\n", sum);

	fclose(fp);
}
