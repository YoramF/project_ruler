#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_FACTORS 200

// define a variable size factorial structure 
typedef struct {
    int n_items;
    unsigned int factors[];
} FACTORIALS;

typedef struct _set {
    unsigned int count;
    FACTORIALS *fc;
    struct _set *next;
} SET;

int q_func (const void *e1, const void *e2) {
    unsigned int *e1_, *e2_;

    e1_ = (unsigned int *)e1;
    e2_ = (unsigned int *)e2;

    return (*e2_ - *e1_);
}

void print_fc (FACTORIALS *fc) {
    int i;
    printf("( ");
    for (i = 0; i < fc->n_items; i++)
        printf("%u ", fc->factors[i]);
    printf(")\n");
}

// compare two elements
int set_is_eq (const void *e_item, const void *n_item) {
    FACTORIALS *e1, *e2;
    int r = 0;

    e1 = (FACTORIALS *)e_item;
    e2 = (FACTORIALS *)n_item;


    if (e1->n_items == e2->n_items) {
        return memcmp(&e1->factors, &e2->factors, e1->n_items*sizeof(unsigned int));
    }

    return (e1->n_items - e2->n_items);
}

FACTORIALS *dup_fc (FACTORIALS *fc) {
    FACTORIALS *n_fc;

    if ((n_fc = malloc(sizeof(FACTORIALS)+fc->n_items*sizeof(unsigned int))) == NULL) {
        perror("Failed to reallocate RAM for updated fc\n");
        exit (EXIT_FAILURE);        
    }

    // copy fc to nfc
    memcpy(n_fc, fc, sizeof(FACTORIALS)+fc->n_items*sizeof(unsigned int));
    return n_fc;
}





// insert new fc into set only if it is new fc
int set_insert (SET *set, FACTORIALS *fc) {
    SET *set_n = set->next, *set_b = NULL;
    SET *s_fc;
    int r;

    if ((s_fc = malloc(sizeof(SET))) == NULL) {
        perror("Failed to allocate RAM for new SET element\n");
        exit (EXIT_FAILURE);
    }

    s_fc->fc = dup_fc(fc);
    s_fc->next = NULL;

    qsort(&s_fc->fc->factors, fc->n_items, sizeof(unsigned int), &q_func);

    while (set_n != NULL) {
        if ((r = set_is_eq(set_n->fc, s_fc->fc)) == 0) {
            // fc is already in set. 
            free(s_fc->fc);
            free(s_fc);
            return 0;
        }
        else if (r > 0) {
            set->count++;
            s_fc->next = set_n;
            if (set_b != NULL)
                set_b->next = s_fc;
            else
                set->next = s_fc;
            return 1;
        }
        set_b = set_n;
        set_n = set_n->next;
    }

    // got here if set is empty, or fc is bigger than all elements in set
    if (set->next == set_n)
        set->next = s_fc;
    else {
        set_b->next = s_fc;
    }

    set->count++;
    return 1;
}




void print_factorials_sets (SET *fs) {
    int i = 1;

    fs = fs->next;
    while (fs != NULL) {
        printf("[%d] ",i);
        print_fc(fs->fc);
        i++;
        fs = fs->next;
    }
}

// this fc will be used for the recursive gen_factorials
FACTORIALS *init_fc () {
    FACTORIALS *fc;

    if ((fc = malloc(sizeof(FACTORIALS)+MAX_FACTORS*sizeof(unsigned int))) == NULL) {
        perror("Failed to allocate RAM for updated common fc\n");
        exit (EXIT_FAILURE);
    }

    fc->n_items = 0;

    return fc;
}

void append_fc (FACTORIALS *fc, unsigned int n) {
    if (fc->n_items < MAX_FACTORS) {
        fc->factors[fc->n_items] = n;
        fc->n_items++;
    }
    else {
        fprintf(stderr, "append_fc, exceeded max factors %d\n", MAX_FACTORS);
        exit (EXIT_FAILURE);
    }
}

// return last item in factorial array and remove it from factorial
unsigned int extract_fc (FACTORIALS *fc) {

    if (fc->n_items > 0) {
        fc->n_items--;
        return fc->factors[fc->n_items];
    }
    else {
        fprintf(stderr, "Failed to extract item from empty factorial\n");
        exit (EXIT_FAILURE);
    }
}

// recursive function to generate all factorials of a number
void gen_factorials (FACTORIALS *fc, SET *set) {
    unsigned int i, d, num;

    num = extract_fc(fc);   // last element

    for (i = 2; i*i <= num; i++) {
        d = num/i;
        if (d*i == num) {
            append_fc(fc, i);
            append_fc(fc, d);
            if (set_insert(set, fc)) {
                print_fc(fc);    
                gen_factorials(fc, set);
            }
            else
                extract_fc(fc);

            extract_fc(fc);         // remove d from fc
        }
    }

    printf("Set->count: %u\n", set->count);
}

void delete_factorials (SET *sfc) {
    SET *bfc, *nfc;

    if (sfc == NULL)
        return;
    
    nfc = sfc->next;
   
    while (nfc != NULL) {
        bfc = nfc;
        free(bfc->fc);
        nfc = nfc->next;
        free(bfc);
    }

    free(sfc);
}


int main (int argc, char **argv) {
    unsigned int num, i, d;
    SET *set;
    FACTORIALS *gfc;

    num = atoi(argv[1]);

    if ((set = malloc(sizeof(SET))) == NULL) {
        perror("Failed to allocate RAM for set\n");
        exit (EXIT_FAILURE);
    }
    set->next = NULL;
    set->count = 0;

    gfc = init_fc();

    // add to set (1, num) - special case
    gfc->n_items = 2;
    gfc->factors[0] = num;
    gfc->factors[1] = 1;
    set_insert(set, gfc);

    // reset gfc to insclude only num
    gfc->n_items = 1;
    gen_factorials(gfc, set);

    print_factorials_sets(set);

    delete_factorials(set);

    return 0;

}
