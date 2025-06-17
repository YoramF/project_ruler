/*
Euler problem 23.
Solution: 4179871
*/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define VAL_LIMIT 28123

static int last = 0;
static long long abundant_arr [VAL_LIMIT];
static bool non_ab_sum [VAL_LIMIT];

long long divisors_sum(int n) {
   int i=2;
   int sq = sqrt(n);
   long long sum = 1L;
   int k;

   while(i <= sq) {
        if(n%i == 0) {
            sum += i;
			k = n/i;
            if (i != k) {
                sum += k;
            }
        } 
        i++;
    }

	return sum;
}


int main () {
    long long s, s_a = 0L;
    int i, j;

    // reset non_ab_sum array
    memset(non_ab_sum, true, VAL_LIMIT*sizeof(bool));

    // fill abundant_arr
	for (i = 1; i < VAL_LIMIT; i++) {
		if ((s = divisors_sum(i)) > i) {
			abundant_arr[last] = i;
            last++;
        }
	}

    // now go over abundant_arr and mark all integers below 28123 that are the sum of
    // two abundant numbers;
    for (i = 0; i < last; i++) {
        for (j = i; j < last; j++) {
            s = abundant_arr[i] + abundant_arr[j];
            if (s < VAL_LIMIT) {
               non_ab_sum[s] = false; 
            }
        }
    }

    // calculate sum
    for (i = 0; i < VAL_LIMIT; i++) {
        if (non_ab_sum[i]) {
            s_a += i;
        }
    }

    printf("%lld\n", s_a);
	
	return 0;
}