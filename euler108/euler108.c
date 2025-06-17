/*
https://projecteuler.net/problem=108 

Solution: 1/x+1/y=1/n => n*x+n*y=x*y => 0 = n*x+n*y-x*y => 
n^2 = x*y-n*x-n*y+n^2 => n^2=(x-n)(y-n) => n^2=a*b

If we find n such that total number of all the possible factors for n^2 is greater than
1,000 we find the solution. This solution has optimized factors() function by "jay_kay"

n: 180180

real    0m0.069s
user    0m0.061s
sys     0m0.008s
*/

#include <stdio.h>

unsigned int factors (unsigned int number) {
    unsigned int result = 1;
    unsigned int exp = 0;
    while(!(number & 1)) {
        exp++;
        number >>= 1;
    }
    result *= ((exp << 1) + 1);
    for(int i = 3; i * i <= number; i += 2) {
        exp = 0;
        while(!(number % i)) {
            exp++;
            number /= i;
        }
        result *= ((exp << 1) + 1);
    }

    //  if number > 1 then number is the final prime factor raised to the first power
    //  since we are calculating the number of factors of number's square
    //  we multiple by 3 (1 doubled + 1)
    if(number > 1) result *= 3;
    return (result + 1) >> 1;
}


int main () {
    unsigned int n, c;

    c = 2;
    n = 1000;
    while(c <= 1000) {
        n++;
        c = factors(n); 
    }

    printf("n: %u\n", n);

    return 0;
}