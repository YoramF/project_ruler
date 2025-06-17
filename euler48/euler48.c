/**
The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.
Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.

Propose solution:
mul(n, p) : function that calculate n*p but returns only last 10 digits of the results
add(n1, n2) : function that add n1 to n2 and return only the last 10 digits of the result

loop from 1 to 1000 and use mul() and add() to get the result

**/

#include <stdio.h>

long long int mul (long long int n, long int p) {
	long long int r = n * p;
	return (r % 10000000000);
}

long long add (long long n1, long long n2) {
	return ((n1 + n2) % 10000000000);
}

int main () {
	long long int r = 0LL, m;

	for (int i = 1; i <= 1000; i++) {
		m = i;
		for (int j = 1; j < i && m > 0; j++) {
			m = mul(m, i);
		}
		r = add(r, m);
	}

	printf("%lld\n", r);
}