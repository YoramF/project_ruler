/*
https://projecteuler.net/problem=100

The solution:
r: Red
b: Blue
P(bb) = b/(r+b)*(b-1)/((r+b)-1) = 1/2

2*b*(b-1) = (r+b)((r+b)-1)
r+b = y
2*b*(b-1)-y(y-1) = 0

applying Pell Equation Approach: 
2b^2-2b+y-y^2 = 0
b = (2+sqrt(4+8y^2-8y))/4 = (1+sqrt(1+2y^2-2y))/2
we need to find an integer d so that 2y^2-2y+1 = d^2
solving b for the integer d we found we get:
y = (2+sqrt(8d^2-4))/4 = (1+sqrt(2d^2-1))/2
so it is suffices to find values of d such that d^2-2c^2 = -1 for some integer c
we need to find pair(d,c) where d is number of blue disks and c is the total of all disks
The first three solutions to this are (1,1), (5,7), It follows that 
if (d,c) is a solution, (3*d+4*c,2*d+3*c).
we pairs until c is larger than 10^12

*/

#include <stdio.h>


void main () {
    long unsigned c, d;
    long unsigned limit = 1000000000000;
    long unsigned m_c, t_d, t_c;

    c = 7;
    d = 5;
    m_c = 2*limit-1;
    while(c <= m_c) {
        t_d = 2*c+3*d;
        t_c = 3*c+4*d;
        d = t_d;
        c = t_c;
    }

    printf("x=%lu\n", (d+1)/2);
}