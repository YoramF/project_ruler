/*
https://projecteuler.net/problem=102

The main challenge of this problem is determin if a triangle contains the origin or not.
For that I will use: 
Half-Plane Test (Same Side Test) - which is based on vector calculations. 
(Thanks to Gemini :)
Given: A triangle with vertices A(x1, y1), B(x2, y2), and C(x3, y3).

Calculate Edge Vectors:
Vector AB = (x2 - x1, y2 - y1)
Vector BC = (x3 - x2, y3 - y2)
Vector CA = (x1 - x3, y1 - y3)

Calculate Vectors to Origin:
Vector AO = (-x1, -y1)
Vector BO = (-x2, -y2)
Vector CO = (-x3, -y3)

Calculate Cross Products (2D equivalent):
Cross(AB, AO) = (x2 - x1) * (-y1) - (y2 - y1) * (-x1)
Cross(BC, BO) = (x3 - x2) * (-y2) - (y3 - y2) * (-x2)
Cross(CA, CO) = (x1 - x3) * (-y3) - (y1 - y3) * (-x3)
Check Signs:

If all three cross products have the same sign (all positive or all negative), the origin is inside the triangle.
If any two cross products have different signs, the origin is outside the triangle.

Total tiangles which include origin: 228

real    0m0.005s
user    0m0.003s
sys     0m0.002s

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

// chech if given tiangle includes the origin
bool inc_origin(int A[], int B[], int C[]) {
    int cr1, cr2, cr3;

    cr1 = (B[0]-A[0])*(-1*A[1])-(B[1]-A[1])*(-1*A[0]);
    cr2 = (C[0]-B[0])*(-1*B[1])-(C[1]-B[1])*(-1*B[0]);
    cr3 = (A[0]-C[0])*(-1*C[1])-(A[1]-C[1])*(-1*C[0]);

    return ((cr1 < 0 && cr2 < 0 && cr3 < 0) || (cr1 > 0 && cr2 > 0 && cr3 > 0));
}


void main () {
    FILE *f;
    char line [80];
    int i = 0;
    int A[2], B[2], C[2];
    int count = 0;

    if ((f = fopen("./triangles.txt", "r")) == NULL) {
        fprintf(stderr, "Failed to open file ./triangles.txt :%s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line)-1, f) != NULL) {
        sscanf(line, "%d,%d,%d,%d,%d,%d", &A[0], &A[1], &B[0], &B[1], &C[0], &C[1]);
        if (inc_origin(A, B, C))
            count++;
    }

    printf("Total tiangles which include origin: %d\n", count);

    fclose(f);

}