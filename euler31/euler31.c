/*
Solution: use recursive+ memoization approach 
answer: 73682

real    0m0.175s
user    0m0.015s
sys     0m0.045s
*/

#include <stdio.h>


static int coins [8] = {200, 100, 50, 20, 10, 5, 2, 1};
static int sum_v [200+1][8+1];

int sum_c (int val, int left) {
    int sum, c, coin, v = val;

    // if this val+left combination has been calculated, use saved result 
    if (sum_v[v][left] >= 0)
        return(sum_v[v][left]);

    coin = coins[8-left];

    if (left <= 1)
        return 1;

    sum = 0;

    while (val >= 0) {
        sum += sum_c(val, left-1);
        val -= coin;
    }

    // save result for later use
    sum_v[v][left] = sum;

    return sum;

}

int main () {

    for (int i = 0; i <= 200; i++)
        for (int j = 0; j <= 8; j++)
            sum_v[i][j] = -1;

    printf("%d\n", sum_c(200, 8));
}