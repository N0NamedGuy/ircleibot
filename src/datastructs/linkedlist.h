#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

#define LLIST_FREE_DATA

#define LLIST_TYPE void* 
struct llist_node {
    struct llist_node* next;
    struct llist_node* prev;
    LLIST_TYPE data;
};

struct linked_list {
    struct llist_node* head;
    struct llist_node* tail;
    unsigned long count;
};

struct llist_iter {
    struct linked_list* cur_list;
    struct llist_node* cur_node;
};


/* Creates a new/Destroys linked list */
extern struct linked_list*  llist_new();
extern void                 llist_destroy(struct linked_list* llist);

/* Adds a nodes to a list */
extern void     llist_add_last(struct linked_list* llist, LLIST_TYPE data);
/* extern void     llist_add_first(struct linked_list* llist, LLIST_TYPE data); */ /* Not implemented */

/* Creates a new/Destroys linked list iterator */
extern struct llist_iter*   llist_iter_new(struct linked_list* llist);
void                        llist_iter_destroy(struct llist_iter* iter);

/* Navigation on an linked list iterator */
extern LLIST_TYPE llist_iter_next(struct llist_iter* iter);
/* extern LLIST_TYPE llist_iter_prev(struct llist_iter* iter); */ /* Not implemented */
extern void llist_iter_rewind(struct llist_iter* iter);
/* extern void llist_iter_forward(struct llist_iter* iter); */ /* Not implemented */

extern bool llist_iter_hasnext(struct llist_iter* iter);
/* extern bool llist_iter_hasprev(struct llist_iter* iter); */ /* Not implemented */

#endif

