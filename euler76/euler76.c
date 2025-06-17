/*

I chose to solve this problem using brute force approach.
Later I found out that it can be solved using "Integer partition" function.

f(100) = 190569291

real    0m19.088s
user    0m0.030s
sys     0m0.060s
*/


#include <stdio.h>
#include <string.h>

void split (int n, int *c_d_ind, int d[]) {
    int i, c_d, tn, c_ind;

    c_ind = *c_d_ind;

    if (d[c_ind] > 1) {
        c_d = d[c_ind] = d[c_ind] - 1;

        for (i = 0; i <= c_ind; i++)
            n -= d[i];

        tn = n;
        c_d = d[c_ind] < tn? d[c_ind]: tn;
        while (c_d > 0) {
            d[i] = c_d;
            tn -= c_d;
            c_d = tn > c_d? c_d: tn;
            i++;  
        }

        c_ind = i - 1;

        while (d[c_ind] == 1)
            c_ind--;
    }
    else
        c_ind--;

    *c_d_ind = c_ind >= 0? c_ind: 0;
}

int main () {
    int n, cnt, c_d_ind;
    int d[100];

    memset(d, 0, 100);

    n = 100;
    d[0] = n-1;
    d[1] = 1;
    c_d_ind = 0;
    cnt = 0;

    while (d[c_d_ind] > 1) {
        cnt++;
        split(n, &c_d_ind, d);
    }

    cnt++;

    printf("%d: %d\n", n, cnt);

    return 0;
}