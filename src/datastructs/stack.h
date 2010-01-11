#ifdef STACK_H
#define STACK_H

#include "linkedlist.h"

extern typedef struct linked_list stack;

extern stack*   stack_new();
extern void     stack_destroy(stack* st);
extern void     stack_push(stack* st, void* data);
extern void*    stack_pop(stack* st);

#endif
