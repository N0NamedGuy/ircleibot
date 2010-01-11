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
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

#define LLIST_FREE_DATA

struct llist_node {
    struct llist_node* next;
    struct llist_node* prev;
    void* data;
};

struct linked_list {
    struct llist_node* head;
    struct llist_node* tail;
    unsigned long count;
};

struct llist_iter {
    struct linked_list* cur_list;
    struct llist_node* cur_node;
    unsigned long cur_pos;
};


/* Creates a new/Destroys linked list */
extern struct linked_list*  llist_new();
extern void                 llist_destroy(struct linked_list* llist);

/* Adds a node to a list */
extern void                 llist_add_last(struct linked_list* llist, void* data);
extern void                 llist_add_first(struct linked_list* llist, void* data);

/* Removes nodes from a list */
extern bool                 llist_remove_first(struct linked_list* llist);
extern bool                 llist_remove_last(struct linked_list* llist);
extern bool                 llist_remove(struct linked_list* llist, unsigned long pos);

/* Gets nodes from a list */
extern void*           llist_get_first(struct linked_list* llist);
extern void*           llist_get_last(struct linked_list* llist);
extern void*           llist_get(struct linked_list* llist, unsigned long pos);

extern unsigned int         llist_count(struct linked_list* llist);

/* Creates a new/Destroys linked list iterator */
extern struct llist_iter*   llist_iter_new(struct linked_list* llist);
extern void                 llist_iter_destroy(struct llist_iter* iter);

/* Navigation on an linked list iterator */
extern void*                llist_iter_next(struct llist_iter* iter);
/* extern void*             llist_iter_prev(struct llist_iter* iter); */ /* Not implemented */
extern void                 llist_iter_rewind(struct llist_iter* iter);
/* extern void              llist_iter_forward(struct llist_iter* iter); */ /* Not implemented */

extern bool                 llist_iter_hasnext(struct llist_iter* iter);
/* extern bool              llist_iter_hasprev(struct llist_iter* iter); */ /* Not implemented */

extern unsigned long        llist_iter_pos(struct llist_iter* iter);
#endif

