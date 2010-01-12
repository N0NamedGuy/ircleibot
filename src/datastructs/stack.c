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
