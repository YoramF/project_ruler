#include <stdio.h>

#include "sets.h"

void dump_d(BLOCK *b, unsigned int off, unsigned int size) {
    // for (int i = 0; i < size; i++)
        // printf("%d,", b->data[off*size+i]);
        printf("%llu", *(long long unsigned *)&b->data[off*size]);
}

void dump_e (S_ELEMENT *e, SET *s) {
    if (e->block_ptr == NULL)
        printf("EMPTY\n");
    else {
        printf("\n");
        do {
            dump_d(e->block_ptr, e->e_off, s->e_size);
            e = e->e_next;
            printf("\n");
        }
        while (e != NULL);
    }
}

void dump_set(SET *s) {
    printf("set-> e_size: %d\n", s->e_size);
    printf("e_amount: %d\n", s->e_amount);
    printf("set:\n");
    for (int i = 0; i < s->hash_n; i++) {
        printf("[%d] ", i);
        dump_e(&s->set[i], s);
    }

}

int main () {
    SET *s;
    long long unsigned int lli = 6L;
    int k;

    s = set_create(51, sizeof(lli));

    for (int i = 1; i <= 50; i++) {
        lli *= i;
        k = set_insert(s, &lli);
        printf("inset %llu, returned %d\n", lli, k);
    }

    printf("set amount: %lld\n", set_amount(s));

    lli = 6L;
    for (int i = 1; i <= 50; i++) {
        lli *= i;
        printf("value: %llu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }

    dump_set(s);

    set_clear(s);

    dump_set(s);
    lli = 6L;
    for (int i = 1; i <= 50; i++) {
        lli *= i;
        printf("value: %llu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        k = set_insert(s, &lli);
        printf("inset %llu, returned %d\n", lli, k);
    }
    printf("set amount: %lld\n", set_amount(s));
    dump_set(s);

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        printf("value: %llu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }
    set_clear(s);
    dump_set(s);

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        printf("value: %llu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }
    
    return 0;
}