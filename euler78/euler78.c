/**
 * 
 * Problem:
Let p(n) represent the number of different ways in which n coins can be separated into piles. For example, five coins can be separated into piles in exactly seven different ways, so p(5)=7.

OOOOO
OOOO   O
OOO   OO
OOO   O   O
OO   OO   O
OO   O   O   O
O   O   O   O   O
Find the least value of n for which p(n) is divisible by one million.

 * Solution by: Yoram Finder
 * =========================
 * 
 * To find p(n) for anny given n we can follow Partition function (number theory) 
 * p(n) = p(n – 1)  + p(n – 2) – p(n – 5) – p(n – 7) + p(n – 12) + p(n – 15) – p(n – 22)…
 * where p(0) = 1 and p(n)  = 0 for n < 0.
 * The sequence to use are the pentagonal numbers which are given on the form 
 * f(k) = k(3k-1)/2 for both negative and positive k
 * k = m/2+1 for m%2 == 0 and k = -m/2-1 otherwise for m = 1,2... 


(https://en.wikipedia.org/wiki/Partition_function_(number_theory))
 * 
 * Since the problem is looking for the first n where p(n) is devisebale by 1M and
 * there is no meed to provide the result of p(n) we don't have to handle very large
 * integers, just big enough so that f(p(n)) % 1M can be calculated, where f()
 * represent lower number portion of the actual p(n) value.
 * 
 */

#include <stdio.h>

#define MAX_NUM 100000

int pn [MAX_NUM];

int calc_pn (int n) {
	int sign, k;
	int i = 0;
	int penta = 1;
	int p_n = 0;;

	while (penta <= n) {                    
		sign = (i % 4 > 1) ? -1 : 1;
		p_n += sign * pn[n - penta];
		p_n %= 1000000;
		i++;
				
		k = (i % 2 == 0) ? i / 2 + 1 : -(i / 2 + 1);
		penta = k * (3 * k - 1) / 2;
	}

	pn[n] = p_n;
	return p_n;
}

void main () {
	int i;
	int	p;

	pn[0] = 1;

	// inint array
	for (i = 6; i < MAX_NUM; i++) {
		pn[i] = 0;
	}

	// printf("%s\n", p_lli3(comb(55374), bf));

	// loop unstil we find the result
	i = 0;
	do {
		i++;
		p = calc_pn(i);
	}
	while (p != 0);

	printf("Result: p(n) for n=%d\n", i);
}