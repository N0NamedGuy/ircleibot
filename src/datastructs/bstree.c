#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "bstree.h"

extern void bst_free_nodes(struct bst_node* node);

/* Returns 0 if it's the node */
/* Returns <0 if it's a left child */
/* Returns >0 if it's a right child  */
/* The actual node is returned in ret_node */
extern int bst_find_node(struct bs_tree* bst, void* key, struct bst_node** ret_node, struct bst_node** prev);

/* TODO: Make this iterative */
void bst_free_nodes(struct bst_node* node) {
    if (node == NULL) {
        return;
    }
    
    bst_free_nodes(node->left);
    bst_free_nodes(node->right);

    free(node->key);
    if (node->data != NULL) {
        free(node->data);
    }
    free(node);
}

int bst_find_node(struct bs_tree* bst, void* key, struct bst_node** ret_node, struct bst_node** prev) {
    int comp;
    bool stop;
    struct bst_node* cur_node;

    cur_node = bst->root;
    comp = 0;
    stop = false;
    *ret_node = NULL;
    
    while (cur_node != NULL || stop) {
        comp = bst->comp(key, cur_node->key);
      
        if (prev != NULL) { 
            *prev = *ret_node;
        }

        *ret_node = cur_node;
        if (comp < 0) {
            cur_node = cur_node->left;
        } else if (comp > 0) {
            cur_node = cur_node->right;
        } else {
            stop = true;
        }
    }

    return comp;
}

struct bs_tree* bst_new(comparator comp) {
    struct bs_tree* new;

    new = malloc(sizeof(struct bs_tree));
    new->root = NULL;
    new->count = 0;
    new->comp = comp;

    return new;
}

void bst_destroy(struct bs_tree* bst) {
    bst_free_nodes(bst->root);
    free(bst);   
}

bool bst_add(struct bs_tree* bst, void* key, void* data) {
    struct bst_node* node;
    struct bst_node* insnode;
    int side;

    node = malloc(sizeof(struct bst_node));
    node->left = NULL;
    node->right = NULL;
    node->key = key;
    node->data = data;

    if (bst->root != NULL) {
        insnode = NULL;
        side = bst_find_node(bst, key, &insnode, NULL);

        if (side < 0) {
            insnode->left = node;
        } else if (side > 0) {
            insnode->right = node;
        } else {
            return false;
        }
    } else {
        bst->root = node;
    }

    bst->count++;
    return true;
}

bool bst_remove(struct bs_tree* bst, void* key) {

    struct bst_node* to_remove;
    struct bst_node* prev_node;

    if (bst_find_node(bst, key, &to_remove, &prev_node) == 0) {
        
        /* This is a very, VERY ugly hack, but it should do the trick */
        if (prev_node != NULL) {
            if (prev_node->left == to_remove) {
                prev_node->left = NULL;
            } else {
                prev_node->right = NULL;
            }
        }

        if (to_remove->data != NULL) {
            free(to_remove->data);
        }
        free(to_remove->key);
        free(to_remove);
        
    } else {
        return false;
    }

    return true;
}

int bst_comp_str(void* data1, void* data2) {
    return strcmp((char*)data1, (char*)data2);
}

int bst_comp_int(void* data1, void* data2) {
    return *((int*)data1) - *((int*)data2);
}

int bst_comp_uint(void* data1, void* data2) {
    return *((unsigned int*)data1) - *((unsigned int*)data2);
}

int bst_comp_long(void* data1, void* data2) {
    return *((long*)data1) - *((long*)data2);
}
int bst_comp_ulong(void* data1, void* data2) {
    return *((unsigned long*)data1) - *((unsigned long*)data2);
}

int bst_comp_char(void* data1, void* data2) {
    return *((char*)data1) - *((char*)data2);
}
