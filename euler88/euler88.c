/*
https://projecteuler.net/problem=88

My initial solution was based on brute force approach that took forever.
So I took stephan-brumme c++ solution and changed it to c

sum of all prod-sum: 7587457

real    0m0.047s
user    0m0.038s
sys     0m0.009s
*/

#include <stdio.h>
#include <stdbool.h>

#define MAX_K 12000
#define MAX_VAL 0xffffffff

// found a better solution with k terms for number n ?
bool valid(unsigned int n, unsigned int k, unsigned int *f_k) {
  // too many terms ? (more than 12000)
  if (k > MAX_K)
    return false;

  // found a smaller number n with the same number of terms k ?
  if (f_k[k] > n)
  {
    f_k[k] = n; 
    return true;
  }

  return false;
}

// return number of minimum k found (a number may be responsible for multiple minimum k, e.g. 8 => k=4 and k=5)
// n: the initial number
// product:   n divided by removed numbers
// sum:       n minus      removed numbers
// depth:     count        removed numbers
// minFactor: skip checking factors smalled than this
unsigned int getMinK(unsigned int n, unsigned int product, unsigned int sum,
  unsigned int *f_k, unsigned int depth, unsigned int minFactor)
{
  // already removed all factors > 1 ?
  // => add a bunch of 1s to the sum
  if (product == 1)
     return valid(n, depth + sum - 1, f_k)? 1 : 0;

  unsigned int result = 0;
  if (depth > 1)
  {
    // perfect match ?
    if (product == sum)
      return valid(n, depth, f_k) ? 1 : 0;

    // try to finish sequence
    if (valid(n, depth + sum - product, f_k))
      result++;
  }

  // and now all remaining factors
  for (unsigned int i = minFactor; i*i <= product; i++)
      if (product % i == 0) // found a factor ? let's dig deeper ...
          result += getMinK(n, product / i, sum - i, f_k, depth + 1, i);

  return result;
}

int main()
{

    unsigned int f_k[MAX_K+1];

    // init f_k
    for (int i = 0; i <= MAX_K; i++)
      f_k[i] = MAX_VAL;


    // k(2) = 4
    unsigned int n = 4;

    // result
    unsigned int sum = 0;

    // 0 and 1 are not used, still 11999 to go ...
    unsigned int todo = MAX_K - 1;
    while (todo > 0)
    {
        // analyze n
        unsigned int found = getMinK(n, n, n, f_k, 1, 2);
        // at least one new k(n) found ?
        if (found > 0)
        {
            todo -= found;
            sum  += n;
        }

        // next number
        n++;
    }

    // print result
    printf("sum of all prod-sum: %u\n", sum);

    return 0;
}
