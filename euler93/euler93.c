/*
https://projecteuler.net/problem=93

Max possible targets is 6*7*8*9=3024
Max possible 4 digits results = 4 digits selection * 24 permutations
of these 4 digits * 3 op out of 4 selections * 9 brackets combinations =
       126 * 24 * 91 * 9 < 2.5M;
Trying to solve using brute Force approach;

longest set of consecutive positive integers:  51, diigts: 1258

real    0m0.852s
user    0m0.846s
sys     0m0.005s

*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TARGETS 3024
#define MAX_INT 0x7fffffff
#define is_digit(d) ((d) >= '0' && (d) <= '9')

static char op [4] = {'+', '-', '*', '/'};

// order input array - naive algorithm since len <= 3
void order (int a[], int len) {
    int i, t, k = 0;
    bool done = false;

    while (!done) {
        done = true;
        for (i = 0; i < len-1; i++) {
            if (a[i] > a[i+1]) {
                t = a[i];
                a[i] = a[i+1];
                a[i+1] = t;
                done = false;
            }
        } 
    }
}

// get array of integers and gen next ordered permutation
// if new permutation was generated return true, otherwise false
// assumption: reverse ordered array will return false;
bool n_permutation (int a[], int len) {
    int i, j, k, d, t;
    bool s = false;

    // scan from right to left for first cell that is smaller than
    // the one before
    for (i = len-2; i >= 0; i--)
        if (a[i] < a[i+1]) {
            s = true;
            break;
        }
    
    if (s) {
        // find the first eleme right to the previous found element that is the smallest
        // but greater than the found element
        d = MAX_INT;
        for (j = i+1; j < len; j++) {
            if (a[j] > a[i]) {
                if (a[j]-a[i] < d) {
                    d = a[j]-a[i];
                    k = j;
                }
            }
        }

        // swap a[i] with a[k]
        t = a[k];
        a[k] = a[i];
        a[i] = t;

        // order array frmo a[i] to end
        order(&a[i+1], len-i-1);
    }

    return s;
}


float calc (float n1, float n2, char op) {
    switch (op) {
        case '+': return (n1 + n2);
        case '-': return (n1 - n2);
        case '*': return (n1 * n2);
        case '/': return (n1 / n2);
    }
}

// Function to return precedence of operators
int prec(char c) {
    switch (c) {
        case '/':
        case '*': return 2;
        case '+':
        case '-': return 1;
        default: return -1;
    }
}

// Function to perform infix to postfix conversion
char  *infixToPostfix(char *exp, char *result) {
       int len = strlen(exp);
       char stack[len];
       int j = 0;
       int top = -1;

       for (int i = 0; i < len; i++) {
              char c = exp[i];

              // If the scanned character is 
              // an operand, add it to the output string.
              if (is_digit(c))
                     result[j++] = c;

              // If the scanned character#include <stdbool.h> is
              // an ‘(‘, push it to the stack.
              else if (c == '(')
                     stack[++top] = '(';

              // If the scanned character is an ‘)’,
              // pop and add to the output string from the stack 
              // until an ‘(‘ is encountered.
              else if (c == ')') {
                     while (top != -1 && stack[top] != '(') {
                     result[j++] = stack[top--];
                     }
                     top--; 
              }

              // If an operator is scanned
              else {
                     while (top != -1 && (prec(c) < prec(stack[top]) ||
                                          prec(c) == prec(stack[top]))) {
                     result[j++] = stack[top--];
                     }
                     stack[++top] = c;
              }
       }

       // Pop all the remaining elements from the stack
       while (top != -1) {
              result[j++] = stack[top--];
       }

       result[j] = '\0';
       return result;
}

float parse (char *postfix) {
    int r;
    float f1, f2, fr, stack [strlen(postfix)];
    char op;
    int top = -1;
    char *sp = postfix;

    while (*sp != '\0') {
        if (is_digit(*sp))
            stack[++top] = (*sp-'0');
        else {
            // if it is an operator, we must have at least 2 numbers in the stack
            f2 = stack[top--];
            f1 = stack[top--];
            fr = calc(f1, f2, *sp);

            // push the result back to stack
            stack[++top] = fr;
        }

        sp++;
    }

    // result is on top of stack
    return stack[top];        
}

//check if target result is an integer. if it does, mark targets[] 
void check (char *inf, char *postf, char targets[]) {
       float f;
       int d;

       f = parse(infixToPostfix(inf, postf));
       d = f;

       if (d == f && d > 0) {
              if (targets[d] == 0)
                     targets[d] = 1;
       }
}

// generate all operation and brackets permutations per given set of 4 digits
void op_c (int i1, int i2, int i3, int i4, char targets[]) {
       char inf[20], postf[20];

       for (int o1 = 0; o1 < 2; o1++)
              for (int o2 = o1; o2 < 4; o2++)
                     for (int o3 = o1; o3 < 4; o3++)
                            for (int o4 = o1; o4 < 4; o4++) {
                                   sprintf(inf, "%d%c%d%c%d%c%d", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "(%d%c%d)%c%d%c%d",
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "%d%c(%d%c%d)%c%d", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "%d%c%d%c(%d%c%d)",
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "((%d%c%d)%c%d)%c%d", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "(%d%c(%d%c%d))%c%d", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "%d%c(%d%c(%d%c%d))", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "%d%c((%d%c%d)%c%d)", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "%d%c(%d%c%d%c%d)", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                                   sprintf(inf, "(%d%c%d%c%d)%c%d", 
                                          i1, op[o2], i2, op[o3],
                                          i3, op[o4], i4);
                                   check (inf, postf, targets);
                            }
}


void main () {
       int digits [4];
       char maxts [5];
       bool next;
       int i1, i2, i3, i4;
       char targets [MAX_TARGETS+1];
       int sum, max_sum = 0;

       for (i1 = 1; i1 < 7; i1++)
              for (i2 = i1+1; i2 < 8; i2++)
                     for (i3 = i2+1; i3 < 9; i3++)
                            for (i4 = i3+1; i4 < 10; i4++) {
                                   // insert digits to digits array and loop over all permutations
                                   digits[0] = i1;
                                   digits[1] = i2;
                                   digits[2] = i3;
                                   digits[3] = i4;

                                   // inint targets
                                   memset(targets, 0, sizeof(targets));

                                   // now loop over all 24 permutations
                                   do {
                                          op_c(digits[0], digits[1], digits[2], digits[3], targets);
                                          next = n_permutation(digits, 4);
                                   } while (next);

                                   // count number of successful targets
                                   sum = 0;
                                   for (int i = 1; i <= MAX_TARGETS; i++) {
                                          if (targets[i] > 0) {
                                                 sum++;
                                          }
                                          else
                                                 break;
                                   }

                                   if (sum > max_sum) {
                                          sprintf(maxts, "%d%d%d%d", i1, i2, i3, i4);
                                          max_sum = sum;
                                   }                                

                            }

       printf("longest set of consecutive positive integers: % d, diigts: %s\n", max_sum, maxts);

}