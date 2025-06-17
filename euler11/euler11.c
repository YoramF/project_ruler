/**
 * Problem 11
 * In the 20×20 grid below, four numbers along a diagonal line have been marked in red.

08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08
49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00
81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65
52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91
22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80
24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50
32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70
67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21
24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72
21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95
78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92
16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57
86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58
19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40
04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66
88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69
04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36
20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16
20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54
01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48

The product of these numbers is 26 × 63 × 78 × 14 = 1788696.

What is the greatest product of four adjacent numbers in the same direction (up, down, left, right, or diagonally) in the 20×20 grid?

The solution:
Scan the grid from top left to bottom right. For each coordinate calculate a vector
of 4 numbers in the followind directions: RIGHT, DOWN, DIAGONAL-RIGHT & DIAGONAL-LEFT
No need to check reverse directions since product operation has no order preferences.
Create a descanding sorted lits of product results where the first element in the list is the
requested solution.
 * 
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// In order to represent the input grid correct (were x moves from left to right
// and y from top to bottom) with a 2 dimention array,
// we need to enter it by columns instead of rows.
const int ar_20_20[20][20] = {
	{8,49,81,52,22,24,32,67,24,21,78,16,86,19,04,88,04,20,20,01},
	{02,49,49,70,31,47,98,26,55,36,17,39,56,80,52,36,42,69,73,70},
	{22,99,31,95,16,32,81,20,58,23,53,05,00,81,8,68,16,36,35,54},
	{97,40,73,23,71,60,28,68,05,9,28,42,48,68,83,87,73,41,29,71},
	{38,17,55,04,51,99,64,02,66,75,22,96,35,05,97,57,38,72,78,83},
	{15,81,79,60,67,03,23,62,73,00,75,35,71,94,35,62,25,30,31,51},
	{00,18,14,11,63,45,67,12,99,76,31,31,89,47,99,20,39,23,90,54},
	{40,57,29,42,89,02,10,20,26,44,67,47,07,69,16,72,11,88,01,69},
	{00,60,93,69,41,44,26,95,97,20,15,55,05,28,07,03,24,34,74,16},
	{75,87,71,24,92,75,38,63,17,45,94,58,44,73,97,46,94,62,31,92},
	{04,17,40,68,36,33,40,94,78,35,03,88,44,92,57,33,72,99,49,33},
	{05,40,67,56,54,53,67,39,78,14,80,24,37,13,32,67,18,69,71,48},
	{07,98,53,01,22,78,59,63,96,00,04,00,44,86,16,46,8,82,48,61},
	{78,43,88,32,40,36,54,8,83,61,62,17,60,52,26,55,46,67,86,43},
	{52,69,30,56,40,84,70,40,14,33,16,54,21,17,26,12,29,59,81,52},
	{12,48,03,71,28,20,66,91,88,97,14,24,58,77,79,32,32,85,16,01},
	{50,04,49,37,66,35,18,66,34,34,9,36,51,04,33,63,40,74,23,89},
	{77,56,13,02,33,17,38,49,89,31,53,29,54,89,27,93,62,04,57,19},
	{91,62,36,36,13,12,64,94,63,33,56,85,17,55,98,53,76,36,05,67},
	{8,00,65,91,80,50,70,21,72,95,92,57,58,40,66,69,36,16,54,48}
};

#define RIGHT 1
#define DOWN 2
#define DIAG_RIGHT 3
#define DIAG_LEFT 4
#define ARRAY_DIM 20

typedef struct _list {
	int xs, xe, ys, ye;		// list coordinates
	int numbers [4];		// 4 numbers in the list
	int product;			// calculated product value
	struct _list *nxt;		// pointer to next _list items
} LIST;

/*
 * x - X coordinate
 * y - Y coordinate
 * direction - RIGHT, DOWN, DIAG_RIGHT & DIAG_LEFT
 * returns: TRUE if valid coordinates, FALSE otherwise
*/
bool coordinatesOK (int x, int y, int direction) {
	switch (direction) {
		case RIGHT:
			return (x + 4) <= ARRAY_DIM;
		case DOWN:
			return (y + 4) <= ARRAY_DIM;
		case DIAG_RIGHT:
			return ((x + 4) <= ARRAY_DIM) && ((y + 4) <= ARRAY_DIM);
		case DIAG_LEFT:
			return ((x - 4) >= 0) && ((y + 4) <= ARRAY_DIM);
		default:
			return false;
	}
};

/*
 * x - X coordinate
 * y - Y coordinate
 * direction - RIGHT, DOWN, DIAG_RIGHT & DIAG_LEFT
 * returns: new entry if valid coordinates, NULL otherwise
*/
LIST *getEntry (const int x, const int y, const int direction) {
	LIST *newEntry = NULL;
	int i, v = 1;

	// first check if current coordiantes are valid and allocte new entry if they are
	if (coordinatesOK(x, y, direction)) {
		if ((newEntry = malloc(sizeof(LIST))) == NULL) {
			printf("failed to allocae memry for new entry\n");
			return NULL;
		};

		// initiate entry
		newEntry->xs = x;
		newEntry->ys = y;
		newEntry->xe = direction == DOWN? x: (direction == DIAG_LEFT? x - 3: x + 3);
		newEntry->ye = direction == RIGHT? y: y + 3;
		newEntry->nxt = NULL;

		switch (direction) {
			case RIGHT:
				for (i = 0; i < 4; i++) {
					v *= ar_20_20[x + i][y];
					newEntry->numbers[i] = ar_20_20[x + i][y];
				}
				break;
			case DOWN:
				for (i = 0; i < 4; i++) {
					v *= ar_20_20[x][y + i];
					newEntry->numbers[i] = ar_20_20[x][y + i];
				}
				break;
			case DIAG_RIGHT:
				for (i = 0; i < 4; i++) {
					v *= ar_20_20[x + i][y + i];
					newEntry->numbers[i] = ar_20_20[x + i][y + i];
				}
				break;
			case DIAG_LEFT:
				for (i = 0; i < 4; i++) {
					v *= ar_20_20[x - i][y + i];
					newEntry->numbers[i] = ar_20_20[x - i][y + i];
				}
				break;
		};
		newEntry->product = v;
	}

	return newEntry;
}

/*
* enter new entry to a descending sorted list
* listPrt - pointer to first entry in linked list
* newEntry - pointer to new entry
* returns: updated pointer to first entry in linked list
*/
LIST *getToList (LIST *list, LIST *newEntry) {
	LIST *ptr = list, *pPtr;

	// if list is empty
	if (list == NULL)
		return newEntry;

	// search for the right place to enter newEntry
	while (ptr->product >= newEntry->product) {
		// save current entry ptr
		pPtr = ptr;
		ptr = ptr->nxt;

		// if we reached list last item then add mewEntry as its last item
		if (ptr == NULL) {
			pPtr->nxt = newEntry;
			return list;
		}
	}

	// if newEntry needs to entered as first entry
	if (ptr == list) {
		newEntry->nxt = list;
		return newEntry;
	}

	// newEntry is being entered bteewn two existing entries
	newEntry->nxt = ptr;
	pPtr->nxt = newEntry;
	return list;
}

/*
* listPrt - pointer to first entry in linked list
* release all memory allocated for the list
*/
void freeList (LIST *list) {
	LIST *ptr = list, *nPtr;

	while (ptr != NULL) {
		nPtr = ptr;
		ptr = ptr->nxt;
		free(nPtr);
	}
}

/**
 * print linked list
*/
void printList (LIST *list) {
	LIST *ptr = list;
	int i = 1;

	while (ptr != NULL) {
		printf(
			"[%4d] xy: %02d, %02d, %02d, %02d numbers: %02d, %02d, %02d, %02d => %d\n",
			i,
			ptr->xs,
			ptr->ys,
			ptr->xe,
			ptr->ye,
			ptr->numbers[0],
			ptr->numbers[1],
			ptr->numbers[2],
			ptr->numbers[3],
			ptr->product);
		ptr = ptr->nxt;
		i++;
	}
}

int main (int argc, char *argv[]) {
	int i,j;
	LIST *list = NULL, *newEntry;

	for (i = 0; i < ARRAY_DIM; i++) {
		for (j = 0; j < ARRAY_DIM; j++) {
			if ((newEntry = getEntry(i, j, RIGHT)) != NULL)
				list = getToList(list, newEntry);
			if ((newEntry = getEntry(i, j, DOWN)) != NULL)
				list = getToList(list, newEntry);
			if ((newEntry = getEntry(i, j, DIAG_RIGHT)) != NULL)
				list = getToList(list, newEntry);
			if ((newEntry = getEntry(i, j, DIAG_LEFT)) != NULL)
				list = getToList(list, newEntry);
		}
	}
	printList(list);
	freeList(list);
	printf("Done\n\n");
}