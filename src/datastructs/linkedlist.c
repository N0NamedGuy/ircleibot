#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linkedlist.h"

extern void                 free_nodes(struct llist_node* node);
extern struct llist_node*   find_node(struct linked_list* llist, unsigned int index);

/* TODO: Make this iterative... */
void free_nodes(struct llist_node* node) {
    
    if (node != NULL) {
        free_nodes(node->next);
    } else {
        return;
    }

#ifdef LLIST_FREE_DATA
    free(node->data);
#endif
    free(node);
}

struct llist_node* find_node(struct linked_list* llist, unsigned int index) {
    struct llist_node* cur_node;
    unsigned int cnt;
    
    cur_node = llist->head;
    for (cnt = 0; cur_node != NULL; cnt++) {
        if (cnt == index) {
            return cur_node;
        }
    }

    return NULL;
}

struct linked_list* llist_new() {
    struct linked_list* new_llist;

    new_llist = malloc(sizeof(struct linked_list));
    
    new_llist->head = NULL;
    new_llist->tail = NULL;
    new_llist->count = 0;

    return new_llist;
}

void llist_destroy(struct linked_list* llist) {
    if (llist->count == 1) {
        free(llist->head->data);
        free(llist->head);
    } else if (llist->count > 1) {
        free_nodes(llist->head);
    }

    /* Then the linked list itself */
    free(llist);
    llist = NULL;
}

void llist_add_last(struct linked_list* llist, LLIST_TYPE data) {
    struct llist_node* new;
    new = malloc(sizeof(struct llist_node));
    
    new->data = data;
    new->next = NULL;
    
    if (llist->head != NULL) {
        
        if (llist->tail != NULL) {
            new->prev = llist->tail;
            llist->tail->next = new;
        } else {
            new->prev = llist->head;
            llist->head->next = new;
        }
        llist->tail = new;
    
    } else {
        new->prev = NULL;
        llist->head = new;
    }
    

    llist->count++;
}

bool llist_remove_first(struct linked_list* llist) {
    struct llist_node* to_remove;
    
    if (llist->head == NULL) {
        return false;
    }

    to_remove = llist->head;
    llist->head = llist->head->next;
    if (llist->head != NULL) {
        llist->head->prev = NULL;
    }
    
    if (llist->count == 1) {
        llist->tail = NULL;
    }

#ifdef LLIST_FREE_DATA
    free(to_remove->data);
#endif
    
    free(to_remove);
    llist->count--;
    
    return true;
}

bool llist_remove_last(struct linked_list* llist) {
    struct llist_node* to_remove;
    
    if (llist->tail == NULL) {
        if (llist->head != NULL) {
           free(llist->head);
           llist->head = NULL;
            return true;
        } else {
            return false;
        }
    }

    to_remove = llist->tail;
    llist->tail = llist->tail->prev;
    if (llist->tail != NULL) {
        llist->tail->next = NULL;
    }

    if (llist->count == 1) {
        llist->tail = NULL;
    }

#ifdef LLIST_FREE_DATA
    free(to_remove->data);
#endif
    
    free(to_remove);
    llist->count--;

    return true;
}

extern bool llist_remove(struct linked_list* llist, unsigned int index) {
    struct llist_node* to_remove;
    struct llist_node* prev;
    struct llist_node* next;

    to_remove = find_node(llist, index);
    if (to_remove == NULL) {
        return false;
    }

    prev = to_remove->prev;
    next = to_remove->next;

    if (prev != NULL) {
        prev->next = next;
    } else {
        llist->head = next;
    }

    if (next != NULL) {
        next->prev = prev;
    } else {
        llist->tail = prev;
    }

#ifdef LLIST_FREE_DATA
    free(to_remove->data);
#endif
    free(to_remove);
    
    llist->count--;

    if (llist->count <= 1) {
        llist->head->next = NULL;
    }

    return true;    
}

struct llist_iter* llist_iter_new(struct linked_list* llist) {
    struct llist_iter* iter;

    iter = malloc(sizeof(struct linked_list));
    iter->cur_list = llist;
    llist_iter_rewind(iter);

    return iter;
}

void llist_iter_destroy(struct llist_iter* iter) {
    free(iter);
    iter = NULL;
}

LLIST_TYPE llist_iter_next(struct llist_iter* iter) {
    void* to_ret = NULL;

    if (llist_iter_hasnext(iter)) {
        to_ret = iter->cur_node->data;
        iter->cur_node = iter->cur_node->next;
    }

    return to_ret;
}

void llist_iter_rewind(struct llist_iter* iter) {
    iter->cur_node = iter->cur_list->head;
}

void llist_iter_forward(struct llist_iter* iter) {
    iter->cur_node = iter->cur_list->tail;
}

bool llist_iter_hasnext(struct llist_iter* iter) {
    return iter->cur_node != NULL;
}

bool llist_iter_hasprev(struct llist_iter* iter) {
    return iter->cur_node->prev != NULL;
}


