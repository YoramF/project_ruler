/*
	Solution based on Prim's algorithm (https://en.wikipedia.org/wiki/Prim%27s_algorithm)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAX_NUM 1000

struct _edge;

typedef struct _vertex {
	struct _vertex *prev, *next;	// list list in Q
	struct _edge **edges;			// array of edges
	int n_edges;					// number of edges
	int val;						// C[v]
	int in_tree;					// false if vertex still in Q
	int edge;						// E[v]
} VERTEX;

typedef struct _edge {
	VERTEX *v1, *v2;				// pointers to vertecs
	int val;						// edge value
} EDGE;


/*
	scan line into network raw
*/
void scan (char *b, int *net, int size) {
	char *tok;
	int i = 0;
	char s[2] = ",";

	tok = strtok(b, s);

	while (tok != NULL && i < size) {
		if (tok[0] == '-')
			net[i] = -1;
		else 
			net[i] = atoi(tok);
		i++;

		tok = strtok(NULL, s);
	}
}

/*
	find an edge based on vertecs address
*/
EDGE *f_edge (EDGE *e, VERTEX *v1, VERTEX *v2, int n_edges) {
	int i;

	for (i = 0; i < n_edges; i++) {
		if ((e[i].v1 == v1 && e[i].v2 == v2) || (e[i].v1 == v2 && e[i].v2 == v1))
			return &e[i]; 
	}

	return NULL;
}

/*
	retrieve vertecs with smallest C[v] in Q. If all vertecs have same value return the first one.
	if retrieved vertex was the last one, set Q to NULL
*/
VERTEX *r_vertex (VERTEX **Q) {
	VERTEX *v = *Q, *mv = *Q;
	int val = MAX_NUM;

	// find v with smallest C[v]
	if (v != NULL) {
		do {
			if (v->val < val) {
				mv = v;
				val = v->val;
			}
			v = v->next;
		} while (v != *Q);
	}

	// remove mv from Q
	if (mv->next != mv) {
		if (mv == *Q)
			*Q = (*Q)->next;	// update Q to point to other vertex
		mv->next->prev = mv->prev;
		mv->prev->next = mv->next;
	}
	else
		(*Q = NULL);	// last vertex has been removed. 

	return mv;
}

/*
	insert vertix v into tree F and return edges value (E[v])
*/
int in_tree (VERTEX **F, VERTEX *v) {
	int val = 0;

	v->in_tree = 1;

	if (*F == NULL) // tree is empty
		*F = v;
	else
		val = v->val;

	return val;
}

int main (int argc, char *argv[]) {
	FILE *fp;
	char b [200];
	int *network;
	char  opt;
	int size = -1;
	char *fname = NULL;
	int sum_nets = 0;
	int n_edges = 0;
	int min_net = 0;
	VERTEX *vertecs, *F = NULL, *Q = NULL, *v, *w;
	EDGE *edges, *vw;
	int i, j, k;

	// get commad line input
	if (argc > 1) {
		while ((opt = getopt(argc, argv, "f:s:")) > 0) {

			switch (opt) {
				case 'f':
					fname = optarg;
					break;
				case 's':
					size = atoi(optarg);
					break;
				default:
					printf("wrong input\n");
					return 1;
			}
		}
	}

	if (size <= 0) {
		printf("-s SIZE is missing\n");
		return 2;		
	}

	if (fname == NULL) {
		printf("-f FILE_NAME is missing\n");
		return 2;		
	}

	if ((fp = fopen(fname, "r")) == NULL) {
		printf("error opening file %s, error: %s\n", fname, strerror(errno));
		return 3;
	}

	if ((network = calloc(size*size, sizeof(int))) == NULL) {
		printf("error allocating memory\n", strerror(errno));
		return 4;	
	}

	// scan file into network matrix
	i = 0;
	while (!feof(fp) && i < size) {
		fscanf(fp, "%s", b);
		scan (b, network+(i*size), size);
		i++;
	}

	fclose(fp);

	// allocate edges
	if ((vertecs = calloc(size, sizeof(VERTEX))) == NULL) {
		printf("error allocating memory\n", strerror(errno));
		return 4;	
	}

	// find sum of all edgees and number of edges and edges per vertex
	for (i = 0; i < size; i++) {
		k = 0;
		for (j = 0; j < size; j++)
			if (network[i*size+j] >= 0) {
				k++;
				if (j < i) {
					sum_nets += network[i*size+j];
					n_edges++;
				}
			}

		if ((vertecs[i].edges = calloc(k, sizeof(void *))) == NULL) {
			printf("error allocating memory\n", strerror(errno));
			return 4;	
		}
		vertecs[i].n_edges = k;
		vertecs[i].val = MAX_NUM;
	}

	// allocate edges
	if ((edges = calloc(n_edges, sizeof(EDGE))) == NULL) {
		printf("error allocating memory for edges\n", strerror(errno));
		return 4;	
	}

	// construct initial graph
	for (i = 0, k = 0; i < size; i++) {
		int e;
		EDGE *ep;
		for (j = 0, e = 0; j < size; j++) {
			if (network[i*size+j] >= 0) {
				if ((ep = f_edge(edges, &vertecs[i], &vertecs[j], n_edges)) == NULL) {
					vertecs[i].edges[e] = &edges[k];
					edges[k].val = network[i*size+j];
					edges[k].v1 = &vertecs[i];
					edges[k].v2 = &vertecs[j];
					k++;
				}
				else {
					vertecs[i].edges[e] = ep;
				}
				e++;
			}
		}
	}

	// network can be released
	free(network);

	// insert all vertecs into Q
	for (i = 0; i < size-1; i++) {
		vertecs[i].next = &vertecs[i+1];
		vertecs[i+1].prev = &vertecs[i];
	}
	vertecs[0].prev = &vertecs[i];
	vertecs[i].next = &vertecs[0];
	Q = &vertecs[0];

	// main loop
	while (Q != NULL) {
		v = r_vertex(&Q);

		// insert v into F
		min_net += in_tree(&F, v);

		// loop over all edges in v
		for (i = 0; i < v->n_edges; i++) {
			// get adjacent vertex
			vw = v->edges[i];
			if (vw->v1 == v)
				w = vw->v2;
			else
				w = vw->v1;				
			if (w->in_tree == 0) { 		// w still in Q
				if (w->val > vw->val) {
					w->val = vw->val;	// set C[v]
					v->edge = i;		// set E[v]
				}
			}
		}
	}

	// free everything;
	for (i = 0; i < size; i++)
		free(vertecs[i].edges);
	
	free(vertecs);
	free(edges);

	printf("Max/Min network = %d/%d, saving: %d\n", sum_nets, min_net, sum_nets-min_net);

	return 0;
}