#ifndef AGENDA_H
#define AGENDA_H

#define AGENDA_PATH             "agenda.txt"
#define AGENDA_CAPACITY         31
#define AGENDA_MAX_CAPACITY     1000000000

#include <stdbool.h>
#include <time.h>

struct agenda_entry {
    time_t date;
    char subject[256];
};

extern void                     agenda_init();
extern void                     agenda_add(struct agenda_entry* entry); 
extern struct agenda_entry*     agenda_get(int index);
extern bool                     agenda_del(int index);
extern struct agenda_entry*     agenda_list(time_t date);
extern void                     agenda_destroy();

#endif
