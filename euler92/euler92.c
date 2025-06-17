/*
https://projecteuler.net/problem=92

Solution approach:
Start with 2 and calcuate the number chain recurdivly until arriving to 89 or 1 (stop numbers). 
One arrived to one of the stop numbers, start returing from the call and mark each number on 
the chain with the stop number.


sum: 8581146

real    0m0.371s
user    0m0.367s
sys     0m0.003s
*/

#include <stdio.h>

#define LIMIT 10000000
#define MAX_NUM 568     // max possible numers (7*9^2)

// calculate the new number based on num
int g_n_num (int num) {
    int d, n_num = 0;

    while (num > 0) {
        d = num % 10;
        n_num += d*d;
        num /= 10;
    }

    return n_num;
}

int get_stop (int num, int s_nums[]) {
    int n_num;
    
    n_num = g_n_num(num);

    if (s_nums[n_num] > 0)
        return s_nums[n_num];
    else if (n_num == 1 || n_num == 89)
        return n_num;
    else {
        s_nums[n_num] = get_stop(n_num, s_nums);
        return s_nums[n_num];
    }
}

void main () {
    int s_nums[MAX_NUM];
    int n, s_n, sum = 0;

    // init s_nums
    for (n = 0; n < MAX_NUM; n++)
        s_nums[n] = 0;

    for (n = 2; n < LIMIT; n++) {
        if ((s_n = s_nums[g_n_num(n)]) == 0) 
            s_n = get_stop(n, s_nums);
    
        if (s_n == 89)
            sum++;
    }

    printf("sum: %d\n", sum);
}
