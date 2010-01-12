#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "linkedlist.h"

typedef struct linked_list stack;

extern stack*   stack_new();
extern void     stack_destroy(stack* st);
extern void     stack_push(stack* st, void* data);

/* When doing a pop do not forget to free the data you get */
extern void*    stack_pop(stack* st);

extern bool     is_empty(stack* st);
#endif
