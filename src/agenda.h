#ifndef AGENDA_H
#define AGENDA_H

#define AGENDA_PATH             "agenda.txt"

#include <stdbool.h>
#include <libircclient.h>
#include <time.h>

struct agenda_entry {
    time_t date;
    char subject[256];
};

extern void                     agenda_init();
extern void                     agenda_add(struct agenda_entry* entry); 
extern struct agenda_entry*     agenda_get(unsigned long);
extern bool                     agenda_del(unsigned long);
extern void                     agenda_list(irc_session_t* session, char* send_to, struct tm* date);
extern void                     agenda_destroy();

#endif
