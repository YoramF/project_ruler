/*
https://projecteuler.net/problem=89

Roma's number rules:
https://projecteuler.net/about=roman_numerals


diff: 743

real    0m0.029s
user    0m0.008s
sys     0m0.021s

*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct {
    int val;
    char c;
} ROMAN_D;

static char R_n [80];
static ROMAN_D R_d [7] = {
    {1000, 'M'}, {500, 'D'}, {100, 'C'}, {50, 'L'}, {10, 'X'}, {5, 'V'}, {1, 'I'}
};

int g_r_v (char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return -1;
    }
}

int check_sc (int l_c, int c) {

    switch (l_c) {
        case 10:
        case 5:
            if (c == 1)
                return -c;
            break;
        case 100:
        case 50:
            if (c == 10)
                return -c;
            break;
        case 1000:
        case 500:
            if (c == 100)
                return -c;
            break;
    }

    return c;
}

int check_dlv (int sc, int *D, int *L, int *V) {

    switch (sc) {
        case 500:
            (*D)++;
            if (*D > 1)
                return -1;
            break;
        case 50:
            (*L)++;
            if (*L > 1)
                return -1;
            break;
        case 5:
            (*V)++;
            if (*V > 1)
                return -1;
            break;
    }

    return sc;
}

/* parse a Roman number string and retuen its decimal value.
    input: s - pointer to string
    output: evaluated number.
    if the Roman number string is invalid, the function return -1
*/
int parse_r_n (char *s, int *len) {
    int D = 0, L = 0, V = 0;
    char *sp;
    int r, v, l_c, c, sc;

    sp = s + strlen(s) - 1;
    if (*sp == '\n') {
        *sp = '\0';
        sp--;
    }

    *len = sp - s + 1;

    l_c = g_r_v(*sp);
    r = l_c;
    sp--;

    while (sp >= s) {
        c = g_r_v(*sp);

        // check if we are in subtractive combinations state
        sc = check_sc(l_c, c);

        // if are not in subtractive combinations state,
        // check that D, L, and V can each only appear once.
        if (sc == c) {
            if (check_dlv(sc, &D, &L, &V) < 0)
                return -1;
        }

        // check that Numerals must be arranged in descending order of size
        if (sc > 0 && sc < l_c)
            return -1;

        r += sc;
        l_c = c;
        sp--;
    }

    return r;
}

inline static void add_4_9 (int r, int b, int d, int *len, int *num, char **sp) {
    **sp = R_d[b+d].c;
    (*sp)++;
    **sp = R_d[b].c;
    (*sp)++;
    *num -= r*R_d[b+d].val;
    *len += 2;         
}


/*
 generare a string representing Roman number based on the input num
 return the string length or -1 if error for some reason
*/
int gen_r_n (int num) {
    char *sp = R_n;
    int d = 0, r, r4, r9, i, len = 0;

    while (num > 0) {

        // first get rid from all 'M' symbolds
        r = num / R_d[d].val;
        for (i = 0; i < r; i++) {
            *sp = R_d[d].c;
            sp++;
            num -= R_d[d].val;
        }
        len += r;               

        // now handle the rest
        if (d == 0 || d == 4 || d == 2) {
            r9 = num / R_d[d+2].val;
            r4 = num / R_d[d+1].val;
            if (r9 == 9) {
                add_4_9(r9, d, 2, &len, &num, &sp);             
            }
            else if (r4 == 4) {
                add_4_9(r9, d, 1, &len, &num, &sp);   
            }
        }
        else if (d == 1 || d == 3 || d == 5) {
            r4 = num / R_d[d+1].val;
            if (r4 == 4) {
                add_4_9(r9, d, 1, &len, &num, &sp);   
            }                        
        }
        d++;
    }

    *sp = '\0';
    return len;
}


void main () {
    FILE *f;
    char line [80];
    int v, c = 0, len_s, len_t, t_len = 0;;

    if ((f = fopen("roman.txt", "r")) == NULL) {
        printf("Failed to open file, roman.txt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (fgets(line, 80, f) != NULL) {
        c++;
        v = parse_r_n(line, &len_s);

        if (v > -1) {
            printf("[%d][%d] %d, %s.......", c, len_s, v, line);
            len_t = gen_r_n(v);
            printf("[%d], %s\n", len_t, R_n);
            t_len += (len_s - len_t);
        }
    }

    printf("diff: %d\n", t_len);
}