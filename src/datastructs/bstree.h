#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>

typedef int(*comparator)(void* data1, void* data2);

struct bst_node {
    struct bst_node* left;
    struct bst_node* right;
    void* data;
    void* key;
};

struct bs_tree {
    struct bst_node*    root;
    unsigned long       count;
    comparator          comp;
};

struct bst_iter {
    struct bs_tree*     cur_tree;
    struct bst_node*    cur_node;
    unsigned long       pos;
};

extern struct bs_tree*  bst_new         (comparator comp); 
extern void             bst_destroy     (struct bs_tree*    bst);
extern bool             bst_add         (struct bs_tree*    bst, void* key, void* data);
extern bool             bst_remove      (struct bs_tree*    bst, void* key);
extern void*            bst_get         (struct bs_tree*    bst, void* key);
extern unsigned long    bst_count       (struct bs_tree*    bst);

extern struct bst_iter* bst_iter_new    (struct bs_tree*    bst);
extern void             bst_iter_destroy(struct bst_iter*   bst);
extern void*            bst_iter_next   (struct bst_iter*   iter);
extern bool             bst_iter_hasnext(struct bst_iter*   iter);
extern void             bst_iter_rewind (struct bst_iter*   iter);

/* Comparators */
extern int bst_comp_str(void* data1, void* data2);
extern int bst_comp_int(void* data1, void* data2);
extern int bst_comp_uint(void* data1, void* data2);
extern int bst_comp_long(void* data1, void* data2);
extern int bst_comp_ulong(void* data1, void* data2);
extern int bst_comp_char(void* data1, void* data2);

#endif
