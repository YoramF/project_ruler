/** 
 * implement set of long long int
 * 
 **/

#include <stdlib.h>

typedef struct _table {
	long long int *values;
	char *next_values;
	struct _table *next;
} table;

int blocks = 0;

static table *allocate_table (int hash) {
	table *t;

	if ((t = calloc(1, sizeof(table))) == NULL) {
		perror("[set-insert] failed to allocate memory");
		return NULL;
	}
	if ((t->values = calloc(hash, sizeof(long long int))) == NULL) {
		perror("[set-insert] failed to allocate memory");
		return NULL;			
	}
	if ((t->next_values = calloc(hash, sizeof(char))) == NULL) {
		perror("[set-insert] failed to allocate memory");
		return NULL;			
	}
	blocks++;
	return t;
}

/*
*	insert value into set.
	return -1 in case of error
	return 0 if value is already in the set
	return 1 if value was not in set and was saved
*/
int insert (table **set, long long int value, int hash) {
	table *t, *nt;
	int hv = value % hash;

	t = *set;
	// if first call to insert() then allocate first block, save value and return 1
	if (t == NULL) {
		if ((t = allocate_table(hash)) == NULL) {
			// update set with new value
			return -1;
		}
		
		t->values[hv] = value;
		*set = t;
		return 1;
	}

	// value was not found in first block of numbers
	// loop to find the right block of numbers
	while ((t->values[hv] != value) && (t->next_values[hv])) {
		t = t->next;
	}

	if (t->values[hv] == value)
		return 0;

	// we found empty hash entry
	if (t->values[hv] == 0) {
		t->values[hv] = value;
		return 1;
	}

	// if we got here it means we have a new value and it needs to be saved on next block
	// of number if exists. if note allocate a new block
	if (t->next == NULL) {
		if ((nt = allocate_table(hash)) == NULL) {
			return -1;
		}
		t->next = nt;
	}
	else {
		nt = t->next;
	}
	
	nt->values[hv] = value;	
	t->next_values[hv] = 1;
	return 1;
}