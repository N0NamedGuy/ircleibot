/* IRC-LEI Bot
 * Copyright (C) 2010 David Serrano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
