/**
 * 
 * The Fibonacci sequence is defined by the recurrence relation:
 *
 * Fn = Fn−1 + Fn−2, where F1 = 1 and F2 = 1.
 * Hence the first 12 terms will be:
 * 
 * 1 = 1
 * F1 = 1
 * F2 = 1
 * F3 = 2
 * F4 = 3
 * F5 = 5
 * F6 = 8
 * F7 = 13
 * F8 = 21
 * F9 = 34
 * F10 = 55
 * F11 = 89
 * F12 = 144
 * The 12th term, F12, is the first term to contain three digits.
 * 
 * What is the index of the first term in the Fibonacci sequence to contain 1000 digits?
 * 
 * Solution: Yoram Finder
 * ======================
 * In order to calculate Fibonacci up to 1000 digits number there is a need to represnet
 * these huge numbers as an array of 56 * 64bit numbers (struct _lli56). And to implement an add() function 
 * for these huge numbers. I implemented lli56_add (lli56 *n1, lli56 *n2, lli56 *res) which return
 * the number of digits in of the result number.
 * The main loop is simple just keep calcukating Fn+1 until we get a 1000 digits result
 * 
 */

#include <stdio.h>
#include <math.h>

#define MAX_LLS	 1000000000000000000L
#define MAX_DIGITS	18
#define LLI56	56

typedef struct _lli56 {
	long long int num[LLI56];
	int n_items;
} lli56;

void lli56_zero (lli56 *n) {
	int i;

	n->n_items = 0;
	for (i = 0; i < LLI56; i++)
		n->num[i] = 0;
}

int lli56_add (lli56 *n1, lli56 *n2, lli56 *res) {
	int i;
	int o = 0;
	int digits;
	long long int j, r;

	for (i = 0; i < LLI56; i++) {
		r = n1->num[i] + n2->num[i] + o;
		o = r / MAX_LLS;
		res->num[i] = r % MAX_LLS;
	}

	// find most significant 18 digits number item
	i = LLI56 - 1;
	while (res->num[i] == 0)
		i--;

	res->n_items = i;
	digits = (i+1) * MAX_DIGITS;

	j = MAX_LLS - 1;
	while (j >= res->num[i]) {
		digits--;
		j /= 10;
	}

	// the above loop pass most significant digit
	digits++;

	return digits;
}

char *p_lli56 (lli56 *n, int d, char *buf) {
	char *p = buf;
	int i = d, k = 0, j = 0;
	long long e;

	p[i--] = '\0';

	// loop on all digits in non significant number items
	while (k < n->n_items) {
		e = n->num[k];
		for (j = 0; j < MAX_DIGITS; j++) {
			p[i] = (e % 10) + '0';
			e /= 10;
			i--;
		}
		k++;	
	}

	// loop on most significant number item
	e = n->num[k];
	while (i >= 0) {
		p[i] = (e % 10) + '0';
		e /= 10;
		i--;
	}

	return p;
}

void main () {
	int i;
	long l;
	char str [1200];
	lli56 numbers[3];
	int togle = 0, st;
	int digits = 0;

	lli56_zero(&numbers[0]);
	lli56_zero(&numbers[1]);
	lli56_zero(&numbers[2]);

	numbers[0].num[0] = 0; //inint first number
	numbers[1].num[0] = 1;

	i = 1;
	do {
		i++;
		digits = lli56_add(&numbers[togle], &numbers[(togle+1)%3], &numbers[(togle+2)%3]);		
		st = (togle+2)%3;
		togle = (togle + 1) % 3;
	}
	while (digits < 1000);

	printf("Result: %d\n", i);
	printf("%d! = (%d) %s\n", i, digits, p_lli56(&numbers[st], digits, str));
}