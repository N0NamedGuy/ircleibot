#ifndef AGENDA_H
#define AGENDA_H

#define AGENDA_PATH             "agenda.txt"

#include <stdbool.h>
#include <libircclient.h>

struct timestamp {
    int day;
    int month;
    int year;
    int hour;
    int min;
};

struct agenda_entry {
    struct timestamp date;
    char subject[256];
};

extern void agenda_init();
extern void agenda_add(irc_session_t* session, const char* send_to, const char* args); 
extern void agenda_get(irc_session_t* session, const char* send_to, const char* args);
extern void agenda_del(irc_session_t* session, const char* send_to, const char* args);
extern void agenda_list(irc_session_t* session, const char* send_to, const char* args);
extern void agenda_save();
extern void agenda_destroy();

#endif
