/*
https://projecteuler.net/problem=98

Solution approach: 
1) first find all anagram sets (first assumption is that the in the attched list
there might have several words that are anagram to each other). If we find out that
there are only pairs of anagram words I will optimize the list
2) I guess that by step one, the number of words that need to be checked in next stap
will be reduced
3) Once I have the sets, I need to find the correct digits replacments to get square 
numbers. This replacement process is unique per set.
4) Find the largest sqaure number

largest square: 18769

real    0m0.009s
user    0m0.007s
sys     0m0.002s
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>

#define MAX_WORD 15

typedef struct _word {
    long unsigned val;
    char s[MAX_WORD];
    struct _word *next;
} WORD;

typedef struct _anagrams{
    WORD *w[2];
    int len;
    struct _anagrams *next;
} ANAGRAMS;

static char _ch ['Z'-'A'+1];

#define set_ch(c,d) (_ch[(c)-'@']=(d))
#define get_d_ch(c) _ch[(c)-'@']
#define is_d_ch(c,d) (_ch[(c)-'@']==(d))
#define reset_ch() (memset(_ch,-1,sizeof(_ch)))

int q_sort (const void *c1, const void *c2) {
    return *(char *)c1 - *(char *)c2;
}

// calculate value to word
long unsigned add_w_val (char *word, int len) {
    long unsigned val = 0L;
    int i;

    // sort the characters in word
    qsort(word, len, 1, q_sort);

    for (i = 0; i < len; i++) {
        val = 26*val + (word[i] - '@');
    }

    return val;
}

// add new word to list
WORD *add_word (WORD *wp, char *word, int len) {
    WORD *nw, *fwp, *bwp;

    if ((nw = malloc(sizeof(WORD))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM for new WORD: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    strncpy(nw->s, &word[1], len); // skip first and last \""
    nw->val = add_w_val(&word[1], len);

    if (wp != NULL) {
        // insert new word and keep words values in order
        fwp = wp;
        bwp = NULL;
        while (fwp != NULL && fwp->val < nw->val) {
            bwp = fwp;
            fwp = fwp->next;
        }

        if (bwp == NULL) {
            // wp->val > nw->val and nw is first in chain
            nw->next = wp;
        }
        else {
            // either fwp->val >= nw->val,or fwp = NULL
            // insert nw between bwp and fwp
            nw->next = bwp->next;
            bwp->next = nw;
            
            // wp is not going to change
            return wp;
        }
    }
    else
        // wp == NULL
        nw->next = NULL;

    return nw;
}

void read_file (WORD *words[]) {
    char buf [20*1024];
    char *tok = NULL;
    char *del = ",";
    int len;
    FILE *f;

    memset(words, 0, MAX_WORD*sizeof(WORD *));

    f = fopen("./words.txt", "r");

    fgets(buf, sizeof(buf), f);
    if (strlen(buf) == sizeof(buf))
        exit (EXIT_FAILURE);

    tok = strtok(buf, del);
    while (tok != NULL) {
        len = strlen(tok)-2;
        words[len] = add_word(words[len], tok, len);
        tok = strtok(NULL, del);
    }

    fclose(f);
}


ANAGRAMS *add_anagrams (ANAGRAMS *a, WORD *w1, WORD *w2, int len) {
    ANAGRAMS *n_a;

    if ((n_a = malloc(sizeof(ANAGRAMS))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM for new ANAGRAM: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    n_a->len = len;
    n_a->w[0] = w1;
    n_a->w[1] = w2;
    
    n_a->next = a;
    
    return n_a;
}

// find all anagrams in scaned words and generate a link list from
// longest words to shortest
ANAGRAMS *find_anagrams (WORD *words[]) {
    ANAGRAMS *a_h = NULL;
    int i, is_second;
    WORD *wp, *cwp, *bwp;
    long unsigned val;

    // scan words array from largest words to smallest
    for (i = 0; i < MAX_WORD; i++) {
        // scan all words in each index.
        // if two successive words have same values, these two words are anagrams words
        if (words[i] != NULL) {
            wp = words[i];
            is_second = 0;
            val = wp->val;
            bwp = wp;
            cwp = wp;
            while (wp != NULL) {
                if (val == wp->val) {
                    if (is_second) {
                        a_h = add_anagrams(a_h, cwp, wp, i);
                        is_second = 0;

                        // check special case were the first two words are anagrams
                        // in case these are the only words we must make sure
                        // words[i] == NULL
                        if (bwp == words[i])
                            words[i] = wp->next;
                        bwp->next = wp->next;   // remove cwp and wp from words[]
                    }
                    else
                        is_second ^= 0x01;    
                }
                else
                    is_second = 1;

                val = wp->val;
                bwp = cwp; 
                cwp = wp;
                wp = wp->next;          
            }
        }
    }

    return a_h;
}

void release_a (ANAGRAMS *a) {
    ANAGRAMS *n;

    while (a != NULL) {
        n = a;
        free(a);
        a = n->next;
    }
}

void release_w (WORD *words[]) {
    int i;
    WORD *nw, *bw;

    for (i = 0; i < MAX_WORD; i++) {
        if (words[i] != NULL) {
            nw = words[i];
            while (nw != NULL) {
                bw = nw;
                free(nw);
                nw = bw->next;
            }
        }
    }
}

// check if a number is square
bool is_square (long unsigned n) {
    long unsigned q;

    q = (long unsigned)sqrtl((long double)n);

    return (q*q == n);
}

// generate a number out of keys and new word
long unsigned f_squar(char *w) {
    long unsigned sq = 0;
    char d;
    int i;

    while (*w != '\0') {
        if ((d = get_d_ch(*w)) > -1) {
            sq = 10*sq + (long unsigned)d;

            // we can't allow leading zeroes
            if (sq == 0)
                break;
        }
        else
            break;
        w++;
    }

    return sq;
}

// check if we can generate a sequance key out of a word and a number
// if we can, update key[] and return true
bool f_key (long unsigned n, char *w, int len) {
    int i, d, j;
    char digits [10];

    i = 1;
    memset(digits, 0, sizeof(digits));
    reset_ch();

    while (n) {
        d = n % 10;
        n /= 10;
        j = len-i;

        if (is_d_ch(w[j], -1) && digits[d] == 0) {
            set_ch(w[j], d);
            digits[d] = 1;
        }

        else if (!is_d_ch(w[j], d))
            return false;

        i++;
    }

    return true;
}

// find a match between pair of anagrams and two square numbers
// return the bigger sqare number
long unsigned matched_sq(ANAGRAMS *a_h, long unsigned *sqp, int sqn) {
    long unsigned sq, l_sq = 0L;

    sqn--;  // to match array indexes

    while (sqn >= 0) {

        sq = 0L;

        // check if we can constract a key from sqare number and word
        if (f_key(sqp[sqn], a_h->w[0]->s, a_h->len)) {

            // gen a sqare from a_h other word and keys
            sq = f_squar(a_h->w[1]->s);

            if (sq > 0 && is_square(sq)) {
                sq =  sqp[sqn] > sq? sqp[sqn]: sq;
                if (sq > l_sq)
                    l_sq = sq;
            }
        }
        
        sqn--;
    }

    return l_sq;
}

// generate list of square numbers with given number of digits
long unsigned *gen_square (int digits, int *n_sqr) {
    long unsigned *sqp = NULL;
    long unsigned sq_l, sq_m, sq, sqr;

    sq_l = (long unsigned)powl(10.0,(long double)(digits))-1;
    sq_m = (long unsigned)powl(10.0,(long double)(digits-1));

    sq = (long unsigned)sqrt((double)sq_m);

    if ((sqp = malloc((sq_l-sq_m+1)*sizeof(long unsigned))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    *n_sqr = 0;
    sqr = 0L;
    while (sqr < sq_l) {
        sqr = sq*sq;
        sqp[*n_sqr] = sqr;
        (*n_sqr)++;
        sq++;
    }

    return sqp;    
}

// find the largest square value withing the given anagrams pair list
long unsigned largest_square (ANAGRAMS *a_h) {
    long unsigned l_sq = 0L, sq;
    int len = 0, sqn;
    long unsigned *sqp = NULL;

    while (a_h != NULL) {

        // get sqaure limits based on word length
        if (len != a_h->len) {
            if (l_sq == 0L) {

                if (sqp != NULL)
                    free(sqp);

                len = a_h->len;

                sqp = gen_square(len, &sqn);
            }
            else
                // no need to keep looking since we scan from largest squares to smaller ones
                break;
        }

        // see if we can find matched sqares to current anagrams pair
        if ((sq = matched_sq(a_h, sqp, sqn)) > 0) {
            if (l_sq < sq)
                l_sq = sq;
        }

        a_h = a_h->next;
    }

    if (sqp != NULL)
       free(sqp);

    return l_sq;
}

int main () {
    WORD *words[MAX_WORD];
    WORD *wp;
    int i, j, k = 0;
    ANAGRAMS *a_h, *a_n;
    long unsigned l_sq;

    read_file(words);

    for (i = 0; i < MAX_WORD; i++) {
        wp = words[i];
        j = 0;
        while (wp != NULL) {
            wp = wp->next;
            k++;
        }
    }

    // print_sq();

    a_h = find_anagrams(words);
    a_n = a_h;
    release_w(words);   // no need for remaining words[]

    l_sq = largest_square(a_h);
    release_a(a_h);

    printf("largest square: %lu\n", l_sq);

    return 0;
}