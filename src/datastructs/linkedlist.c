#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linkedlist.h"

extern void                 free_nodes(struct llist_node* node);
extern struct llist_node*   find_node(struct linked_list* llist, unsigned int index);

void free_nodes(struct llist_node* node) {
    struct llist_node* i;
    struct llist_node* next;
    
    for (i = node; i != NULL; i = next) {
#ifdef LLIST_FREE_DATA
        free(i->data);
#endif
        next = i->next;
        free(i);
    }

}

struct llist_node* find_node(struct linked_list* llist, unsigned int index) {
    struct llist_node* node;
    unsigned int i;
    
    if (index <= (llist->count - 1) / 2) {
        node = llist->head;
        for (i = 0; i < index; i++) {
            node = node->next;
        }
    } else {
        node = llist->tail;
        for (i = llist->count - 1; i > index; i--) {
            node = node->prev;
        }
    }

    return node;
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
    free_nodes(llist->head);

    free(llist);
    llist = NULL;
}

void llist_add_first(struct linked_list* llist, LLIST_TYPE data) {
    struct llist_node* new;
    new = malloc(sizeof(struct llist_node));

    new->data = data;
    new->next = llist->head;
    new->prev = NULL;

    if (llist->count == 0) {
        llist->head = new;
    } else {
        llist->head->prev = new;
    }
    
    llist->head = new;
    llist->count++;
}

void llist_add_last(struct linked_list* llist, LLIST_TYPE data) {
    struct llist_node* new;
    new = malloc(sizeof(struct llist_node));
    
    new->data = data;
    new->next = NULL;
    new->prev = llist->tail;

    if (llist->count == 0) {
        llist->head = new;        
    } else {
        llist->tail->next = new;
    }

    llist->tail = new;
    llist->count++;
}

bool llist_remove_first(struct linked_list* llist) {
    
    if (llist->count == 0) {
        return false;
    }

#ifdef LLIST_FREE_DATA
    free(llist->head->data);
#endif

    if (llist->count > 1) {
        llist->head = llist->head->next;
        free(llist->head->prev);
    } else {
        free(llist->head);
        llist->head = NULL;
    }

    llist->count--;
    
    return true;
}

bool llist_remove_last(struct linked_list* llist) {
    
    if (llist->count == 0) {
        return false;
    }

#ifdef LLIST_FREE_DATA
    free(llist->tail->data);
#endif

    if (llist->count > 1) {
        llist->tail = llist->tail->prev;
        free(llist->tail->next);
        llist->tail->next = NULL;
    } else {
        free(llist->head);
        llist->head = NULL;
    }

    llist->count--;
    
    return true;
}

void remove_middle_node(struct llist_node* node) {
    struct llist_node* prev;
    struct llist_node* next;

    prev = node->prev;
    next = node->next;

    prev->next = next;
    next->prev = prev;

#ifdef LLIST_FREE_DATA
    free(node->data);
#endif

    free(node); 
}

extern bool llist_remove(struct linked_list* llist, unsigned int index) {
    struct llist_node* to_remove;

    if (index >= llist->count) {
        return false;
    }

    if (index == 0) {
        return llist_remove_first(llist);

    } else if (index == llist->count - 1) {
        return llist_remove_last(llist);
    
    } else {
        to_remove = find_node(llist, index);
        remove_middle_node(to_remove);
        llist->count--;
        return true;
    }

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


