#ifndef BIG_NUMBERS_H_
#define BIG_NUMBERS_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifdef GEN_LIB
unsigned int DIM = 0;
typedef unsigned char BIG_NUMBER;
#else
extern unsigned int DIM;
#endif

typedef struct {
    void *num;
    void *dem;
} FRACTION;

bool is_bigger_equale_ (const void *A, const void *B, bool equal);
#define is_bigger(a,b) is_bigger_equale_((a), (b), false)
#define is_bigger_e(a,b) is_bigger_equale_((a), (b), true)

void add_sub_f_ (const FRACTION *f1, const FRACTION *f2, FRACTION *r, bool plus);
#define add_f(a,b,c) add_sub_f_((a), (b), (c), true);
#define sub_f(a,b,c) add_sub_f_((a), (b), (c), false);

#define assign(a,b) memcpy((b), (a), DIM)
#define release(a) free((a))

// this function must be called before using big_numbers function
// dim parameter will set the max number of digits in big_numbers

/* BIG NUMBER operations */
unsigned int init_big_numbers (unsigned int dim);
void *gen_number(); // create a new big number
void make_int (void *A, char *s); // put a number represented as a string into the big int A
void make_int_u (void *A, unsigned n); // put unsigned n into the big int A
void increment (void *A); // A++ 
void add (const void *A, const void *B, void *C); // C = A + B 
void sub (const void *A, const void *B, void *C); // C = A - B 
bool is_equal (const void *A, const void *B); 
void multiply (const void *A, const void *B, void *C); // C = A * B 
void divide (const void *A, const void *B, void *C); // C[] = A[] / B[] (no reminders)
void modulo (const void *A, const void *B, void *C); // C[] A[] % B[]
bool is_zero (const void *A); // check if A[] is ZERO
int digits (const void *A); // return index to most significat digit in input number 
char *number2str (const void *A, char *s); // convert BIG number A[] to a string s, return s. assuming s has enough space for all digits+1 (for '\0')

/* FRACTION operations */
FRACTION *gen_fraction(); // create new fraction
void make_fraction (FRACTION *f, long double r); // convert float number (r) into FRACTION (f)
void release_fr (FRACTION *f); // release FRACTION
void reduce_f (FRACTION *A); // reduce FRACTION A
void inverse_f (FRACTION *A); // A = 1/A
void divide_f (const FRACTION *A, const FRACTION *B, FRACTION *C); // C = A / B
void assign_f (const FRACTION *F1, FRACTION *F2); // copy one fraction to another
void multiply_f (const FRACTION *A, const FRACTION *B, FRACTION *C); // C = A * B

#endif /* BIG_NUMBERS_H_ */