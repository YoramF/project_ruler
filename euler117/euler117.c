/*
https://projecteuler.net/problem=117

The Problem: Find the number of ways to form a sequence of blocks (R, G, B, E) such that the total sum of 
their sizes is exactly N.

Solution using Dynamic Programming:
Let dp[i] be the number of ways to fill exactly i slots using the available block types (Red, Green, Blue, 
Empty).

Recurrence Relation:
To fill i slots, the last block placed could be:
An Empty slot (E): This block takes 1 slot. So, the previous i−1 slots must have been filled in dp[i−1] ways.
A Red block (R): This block takes 2 slots. So, the previous i−2 slots must have been filled in dp[i−2] ways.
A Green block (G): This block takes 3 slots. So, the previous i−3 slots must have been filled in dp[i−3] ways.
A Blue block (B): This block takes 4 slots. So, the previous i−4 slots must have been filled in dp[i−4] ways.
Therefore, the recurrence relation is:
dp[i]=dp[i−1]+dp[i−2]+dp[i−3]+dp[i−4]

Base Cases:
dp[0]=1: There is one way to fill 0 slots, which is to do nothing (an empty sequence). This is crucial for 
the recurrence to work correctly for dp[1],dp[2], etc.
For any k<0, we define dp[k]=0, as you cannot fill a negative number of slots.

Solution:
100808458960497

real    0m0.004s
user    0m0.001s
sys     0m0.003s

*/

#include <stdio.h>

int main () {
    int n = 50;
    long unsigned int dp[4] = {1L, 1L, 2L, 4L};
    int i, ind;
    long unsigned sum;

    for (i = 4; i <= n; i++) {
        ind = i % 4;
        sum = dp[0]+dp[1]+dp[2]+dp[3];
        dp[ind] = sum;
    }

    printf("%lu\n", sum);

}