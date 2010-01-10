#ifndef AGENDA_H
#define AGENDA_H

#define AGENDA_PATH             "agenda.txt"

#include <stdbool.h>
#include <libircclient.h>
#include <time.h>

struct agenda_entry {
    struct tm date;
    char subject[256];
};

extern void                     agenda_init();
extern void                     agenda_add(const struct agenda_entry* entry); 
extern struct agenda_entry*     agenda_get(unsigned long pos);
extern bool                     agenda_del(unsigned long pos);
extern void                     agenda_list(irc_session_t* session, char* send_to, struct tm* date);
extern void                     agenda_save();
extern void                     agenda_destroy();

#endif
