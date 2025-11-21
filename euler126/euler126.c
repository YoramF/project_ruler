/*
https://projecteuler.net/problem=126

I used brute force approach in which I calculate the number of cubes per layer for initial cuboid of
different dimensions. I store tha resuls in cubic_count array. Once all cubids are calculated
I scan the cubic_count array to find the first index where its value is equal 1000.
The function that count number of cubes which are require to cover a given cuboid calls itself to calculate
the number of cubes which are required to cover the sides of new layer. It appears that the side cover
is a the N-2 layer of cuoid with dimentions of 2x2xC where C is the 3rd dimention of the original cuboid.

The least value of n for which C(n) = 1000 is: 18522

real    14m39.303s
user    14m38.339s
sys     0m0.098s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LAYERS 1000
#define MAX_DIM 10000
#define MAX_CUBES 100000
#define TARGET 1000

int layer_cubes(int a, int b, int c, int n) {

    switch (n) {
        case 0:
            return  a * b * c;
        case 1:
            return (2 * (a * b + a * c + b * c));
        default:
            return (2 * (a * b + (((n - 1) * 2) + c) * a + (((n - 1) * 2) + c) * b) + layer_cubes(2, 2, c, n - 2));
    }
}

int main() {
    int cube_count[MAX_CUBES + 1] = {0};
    int a, b, c, n;
    long cubes;

    for (a = 1; a <= MAX_DIM; a++) {
        for (b = a; b <= MAX_DIM; b++) {
            for (c = b; c <= MAX_DIM; c++) {
                n = 1;
                while (n < MAX_LAYERS) {
                    cubes = layer_cubes(a, b, c, n);
                    if (cubes > MAX_CUBES) break;
                    cube_count[cubes]++;
                    n++;
                }
            }
        }
    }

    for (int i = 1; i <= MAX_CUBES; i++) {
        if (cube_count[i] == TARGET) {
            printf("The least value of n for which C(n) = %d is: %d\n",TARGET, i);
            return 0;
        }
    }

    return 0;
}
