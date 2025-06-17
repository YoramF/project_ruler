#include <stdio.h>
#include <time.h>	// for clock_t, clock()
#include <unistd.h>	// for sleep()

#include "divisors.h"

int main () {
	unsigned long num = 1, adder = 2;
	int targetDivisors = 500;
	int d = 1, cd;
	clock_t start, end;
	double time_spent = 0.0;

	start = clock();

	while (d < targetDivisors) {
		cd = divisors(num);
		if (cd > d) {
			d = cd;
			printf("[%u][%d]             \r", num, d);
		}
		else {
			num += adder;
			adder++;
		}
	}
	printf("\n");

	end = clock();
	time_spent += (double)(end - start) / CLOCKS_PER_SEC * 1000;

	printf("%u has %d divisors\n", num, d);
	printf("Time elpased is %f ms (clock)\n", time_spent);
}