/*
    https://projecteuler.net/problem=122

    For k <= 200 using Breadth-First Search (BFS) is good enough.
    I implemented all exponent sets using link lists. Once moving to next stage, I released all
    lists of the previous stage.

sum m(k) = 1582

real    0m0.048s
user    0m0.037s
sys     0m0.011s

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_K   201

typedef struct _exp {
    int exp;
    struct _exp *next;
} EXP;

typedef struct _exp_list {
    EXP *list;
    EXP *last;
    struct _exp_list *next;
} EXP_LIST;

typedef struct _list_heads {
    EXP_LIST *head;
    EXP_LIST *last;
} LIST_HEADS;

/*
    add new exponent to tree
*/
LIST_HEADS *add_exp (LIST_HEADS *l_head, EXP *c_exp_list, int exp) {
    EXP *n_e_list, *bt;
    EXP_LIST *n_list;

    // create new list head
    if ((n_list = malloc(sizeof(EXP_LIST))) == NULL) {
        perror("Failed to allocate RAM for new list header\n");
        exit(EXIT_FAILURE);
    }
    n_list->next = NULL;

    if (l_head == NULL) {
        if ((l_head = malloc(sizeof(LIST_HEADS))) == NULL) {
            perror("Failed to allocate RAM for list_heads\n");
            exit(EXIT_FAILURE);
        }
        l_head->head = n_list;
    }
    else
        l_head->last->next = n_list;

    l_head->last = n_list;

    // now constract the new n_list
    if ((n_e_list = malloc(sizeof(EXP))) == NULL) {
        perror("Failed to allocate RAM for new exponent\n");
        exit(EXIT_FAILURE);
    }
    n_list->list = n_e_list;

    // first copy all exponents form c_exp_list to new list
    while (c_exp_list != NULL) {
        n_e_list->exp = c_exp_list->exp;
        n_e_list->next = NULL;
        c_exp_list = c_exp_list->next;
        bt = n_e_list;

        if ((n_e_list = malloc(sizeof(EXP))) == NULL) {
            perror("Failed to allocate RAM for new exponent\n");
            exit(EXIT_FAILURE);
        }
        bt->next = n_e_list;
    }

    n_e_list->next = NULL;
    n_e_list->exp = exp;
    n_list->last = n_e_list;

    return l_head;
}

#ifdef DEBUG
/*
    print list of exponents
*/
void p_list (EXP *h, int i) {

    printf("[%d] { ", i);
    while (h != NULL) {
        printf("%d ", h->exp);
        h = h->next;
    }
    printf("}\n");
}

/*
    print all exponents set for a given stage
*/
void p_stage (LIST_HEADS *l_head) {
    EXP_LIST *e_list;
    EXP *exp;
    int i = 1;

    e_list = l_head->head;
    printf("------\n");
    while (e_list != NULL) {
        exp = e_list->list;
        p_list(e_list->list, i++);
        e_list = e_list->next;
    }
}
#endif

/*
    free an exponent set
*/
void f_list (EXP *h) {
    EXP *bt;

    bt = h;
    while (h) {
        h = h->next;
        free(bt);
        bt = h;
    }
}

/*
    free list headers
*/
void f_heads (LIST_HEADS *l_head) {
    EXP_LIST *e_list, *e_list_bt;

    if (l_head != NULL) {
        e_list = l_head->head;
        while (e_list != NULL) {
            e_list_bt = e_list;
            f_list(e_list->list);
            e_list = e_list->next;
            free(e_list_bt);     
        }

        free(l_head);
    }
}

int main () {
    int min_exp [MAX_K];
    LIST_HEADS *stage [MAX_K];
    int r, done, ai, ar, ak;
    EXP_LIST *e_lp;
    EXP *ep, *ep_last;
    int sum;

    // Init min_exp
    min_exp[0] = 0;
    min_exp[1] = 0;
    stage[0] = NULL;
    stage[1] = NULL;
    for (int i = 2; i < MAX_K; i++) {
        min_exp[i] = 0x7FFFFFFF;
        stage[i] = NULL;
    }

    // init stage[1] to contain single exp list {1}
    stage[1] = add_exp(NULL, NULL, 1);
    done = 0;
    r = 1;

    while (r < MAX_K && !done) {
        done = 1;

        if (stage[r] != NULL) {
            e_lp = stage[r]->head;

            while (e_lp != NULL) {
                ep = e_lp->list;
                ep_last = e_lp->last;

                // generate new exponent sets based on current set
                if (ep_last != NULL) {
                    ar = ep_last->exp;

                    while (ep != NULL) {
                        ai = ep->exp;
                        ak = ai + ar;

                        // check if new exponent is already registered in min_exp[] and if its value is less
                        // we can prune only chains only if min_exp[ak] < r
                        if (ak < MAX_K && min_exp[ak] >= r) {
                            done = 0;
                            min_exp[ak] = r;
                            stage[r+1] = add_exp(stage[r+1], e_lp->list, ak);
                        }

                        ep = ep->next;
                    }
                }

                e_lp = e_lp->next;
            }

            #ifdef DEBUG
            p_stage(stage[r]);
            #endif
            f_heads(stage[r]);
        }     
        
        r++;   
    }

    #ifdef DEBUG
    printf("min_exp====, max r: %d\n", r);
    for (int i = 1; i < MAX_K; i++)
        printf("[%d] %d\n", i, min_exp[i]);
    #endif
    sum = 0;
    for (int i = 1; i < MAX_K; i++)
        sum += min_exp[i];

    printf("sum m(k) = %d\n", sum);
    f_heads(stage[r]);    

    return 0;
} 