#ifndef SETS_H_
#define SETS_H_

#include <sys/types.h>
#include <stdbool.h>

typedef struct block_ {
    struct block_ *b_next;
    unsigned int n_space;
    unsigned int e_left;
    unsigned char data[];    
} BLOCK;

typedef struct s_element_ {
    unsigned int  e_off;
    BLOCK   *block_ptr;
    struct s_element_ *e_next;
} S_ELEMENT;

typedef struct {
    unsigned int  e_size;
    unsigned int  hash_n;
    size_t  e_amount;
    unsigned int  (*i_func)(const unsigned int n, const void *item,  const unsigned int i_size);
    bool  (*is_eq_func)(const void *e_item, const void *n_item,  const unsigned int i_size);
    BLOCK   *block_ptr;
    S_ELEMENT set[];
} SET;

typedef struct {
    SET *set;
    unsigned int hash_i;
    S_ELEMENT *s_e;
} S_GET_NEXT;

SET *set_create (const size_t max_items, const unsigned int element_size);    // create a new set
void set_delete (SET *set);      // delete a set
void set_clear (SET *set);      // clear set content
int set_insert (SET *set, void *item);    // insert new item to set. return 1 on success, 0 if item already in set, -1 on error
bool set_e_in_set (SET *set, void *item); // return true of element in set
void *set_get_next_e (S_GET_NEXT *g_n);    // get next element in set
S_GET_NEXT set_reset_get (SET *set);   // reset intem retrival from a set

#define set_amount(s) ((s)->e_amount)

#endif /* SETS_H_ */