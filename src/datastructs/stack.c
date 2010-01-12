#include <stdlib.h>
#include "stack.h"

stack* stack_new() {
    return (stack*)llist_new();
}

void stack_destroy(stack* st) {
    llist_destroy((struct linked_list*)st);
}

void stack_push(stack* st, void* data) {
    llist_add_last((struct linked_list*)st, data);
}

void* stack_pop(stack* st) {
    void* to_ret;
    
    if (st->count == 0) {
        return NULL;
    }

    to_ret = st->tail->data;

    if (st->count > 1) {
        st->tail = st->tail->prev;
        free(st->tail->next);
        st->tail->next = NULL;
    } else {
        free(st->head);
        st->head = NULL;
    }

    st->count--;
    
    return to_ret;
}

bool is_empty(stack* st) {
    return st->count == 0;
}
