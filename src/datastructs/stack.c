#include "linkedlist.h"
#include "stack.h"

struct stack* stack_new() {
    return (struct stack*)llist_new();
}

void stack_destroy(struct stack* st) {
    llist_destroy((struct linked_list*)st);
}

void stack_push(struct stack* st, void* data) {
    llist_add_last((struct linked_list*)st, data);
}

void* stack_pop(struct stack* st) {
    void* to_ret;

    to_ret = llist_get_last((struct linked_list*)st);
    llist_remove_last((struct linked_list*)st);

    return to_ret;
}
