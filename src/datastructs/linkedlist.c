#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linkedlist.h"

#include <stdio.h>

struct linked_list* llist_new() {
    struct linked_list* new_llist;

    new_llist = malloc(sizeof(struct linked_list));
    printf("%s\n", "malloc: new linked list.");
    
    new_llist->head = NULL;
    new_llist->tail = NULL;
    new_llist->count = 0;

    return new_llist;
}

void llist_destroy(struct linked_list* llist) {
    struct llist_node* next;
    next = llist->head;

    /* Destroys all nodes */
    while (next != NULL) {
        next = next->next;
        free(next);
        printf("%s\n", "free: node destroyed.");
    }
      
    /* Then the linked list itself */
    free(llist); 
    printf("%s\n", "free: list destroyed.");
    llist = NULL;
}

/* TODO: avoid code repetition on llist_add_last and llist_add_first */
void llist_add_last(struct linked_list* llist, void* data) {
    struct llist_node* new;
    new = malloc(sizeof(struct llist_node));
    printf("%s\n", "malloc: new node (last).");
    
    new->data = data;
    new->next = NULL;
    
    if (llist->head == NULL) {
        llist->head = new;
        llist->tail = new;
    }
    
    new->prev = llist->tail;
    llist->tail = new;

    llist->count++;
}

void llist_add_first(struct linked_list* llist, void* data) {
    struct llist_node* new;
    new = malloc(sizeof(struct llist_node));
    printf("%s\n", "malloc: new node (first).");
    
    new->data = data;
    new->prev= NULL;
    
    if (llist->head == NULL) {
        llist->head = new;
        llist->tail = new;
    }
    
    new->next = llist->head;
    llist->head = new;

    llist->count++;
}

struct llist_iter* llist_iter_new(struct linked_list* llist) {
    struct llist_iter* iter;

    iter = malloc(sizeof(struct linked_list));
    printf("%s\n", "malloc: new iter.");
    iter->cur_list = llist;
    llist_iter_rewind(iter);

    return iter;
}

void llist_iter_destroy(struct llist_iter* iter) {
    free(iter);
    printf("%s\n", "free: iter destroyed.");
    iter = NULL;
}

void* llist_iter_next(struct llist_iter* iter) {
    void* to_ret = NULL;

    if (llist_iter_hasnext(iter)) {
        to_ret = iter->cur_node->data;
        iter->cur_node = iter->cur_node->next;
    }

    return to_ret;
}

void* llist_iter_prev(struct llist_iter* iter) {
    void* to_ret = NULL;

    if (llist_iter_hasprev(iter)) {
        to_ret = iter->cur_node->data;
        iter->cur_node = iter->cur_node->prev;
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
    return iter->cur_node->next != NULL;
}

bool llist_iter_hasprev(struct llist_iter* iter) {
    return iter->cur_node->prev != NULL;
}


