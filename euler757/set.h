typedef struct _table {
	long long int *values;
	char *next_values;
	struct _table *next;
} table;

extern int blocks;
int insert (table **set, long long int value, int hash);