#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libircclient.h>

#include "datastructs/linkedlist.h"
#include "agenda.h"

struct linked_list* agenda;

void agenda_init() {
    FILE* fd;
    
    char buf[256];
    struct agenda_entry* entry;
    struct timestamp cur_date;
    char subject[256];

    fd = fopen(AGENDA_PATH, "r");
    
    agenda = llist_new();
    
    if (fd != NULL) {
        while (fgets(buf, 256, fd) != NULL) {
                
            if (sscanf(buf, "%d %d %d %d %d %[^\n]s ",
                &cur_date.day,
                &cur_date.month,
                &cur_date.year,
                &cur_date.hour,
                &cur_date.min,
                subject) == 6) {
                
                entry = malloc(sizeof(struct agenda_entry));
                entry->date = cur_date;
                strcpy(entry->subject, subject);

                llist_add_last(agenda, (void*)entry);
            }
            
        }

    }

}

void agenda_add(irc_session_t* session, const char* send_to, const char* args) {
    struct agenda_entry* entry;
    char buf[256];
    int day, year, month, hour, min;

    if (sscanf(args, "%d-%d-%d %d:%d %[^\n]s",
        &day,
        &month,
        &year,
        &hour,
        &min,
        buf) != 6) {
        
        irc_cmd_notice(session, send_to, "Invalid agenda entry.");
        return;
    }

    entry = malloc(sizeof(struct agenda_entry));
    
    strcpy(entry->subject, buf);
    entry->date.day = day;
    entry->date.month = month;
    entry->date.year = year;
    entry->date.hour = hour;
    entry->date.min = min;

    llist_add_last(agenda, (void*)entry);
    sprintf(buf, "Entry added to agenda. Index: [%04d]\n", llist_count(agenda));
    irc_cmd_msg(session, send_to, buf);
}

void agenda_get(irc_session_t* session, const char* send_to, const char* args) {
    struct agenda_entry* entry;
    unsigned long pos;
    char buf[256];

    pos = atol(args);

    if (pos < 1) {
        irc_cmd_notice(session, send_to, "Index must be equal or above 1");
        return;
    }

    entry = (struct agenda_entry*)llist_get(agenda, pos - 1);

    if (entry == NULL) {
        irc_cmd_notice(session, send_to, "Agenda entry not found.");
        return;
    }

    sprintf(buf, "[%04ld] %02d-%02d-%04d %02d:%02d %s\n",
        pos,
        entry->date.day,
        entry->date.month,
        entry->date.year,
        entry->date.hour,
        entry->date.min,
        entry->subject);

    irc_cmd_msg(session, send_to, buf);
}

void agenda_del(irc_session_t* session, const char* send_to, const char* args) {
    unsigned long pos;
    char buf[256];

    pos = atol(args);

    if (pos < 1) {
        irc_cmd_notice(session, send_to, "Index must be equal or above 1");
        return;
    }

    if (!llist_remove(agenda, pos - 1)) {
        irc_cmd_notice(session, send_to, "Agenda entry not found.");
        return;
    }

    sprintf(buf, "Agenda entry number %04ld removed", pos);
    irc_cmd_msg(session, send_to, buf);
}

void agenda_list(irc_session_t* session, const char* send_to, const char* args) {
    irc_cmd_notice(session, send_to, "Coming soon...");
}

void agenda_save() {
    FILE* fd;
    struct llist_iter* iter;
    struct agenda_entry* cur_entry;

    fd = fopen(AGENDA_PATH, "w");
    char buf[256];

    if (fd == NULL) {
        printf("Error while saving the agenda data...\n");
        return;
    }

    iter = llist_iter_new(agenda);
    while(llist_iter_hasnext(iter)) {
        cur_entry = (struct agenda_entry*)llist_iter_next(iter);
        sprintf(buf, "%d %d %d %d %d %s\n",
            cur_entry->date.day,
            cur_entry->date.month,
            cur_entry->date.year,
            cur_entry->date.hour,
            cur_entry->date.min,
            cur_entry->subject);

        fputs(buf, fd);
    }
    llist_iter_destroy(iter);

    fclose(fd);

    printf("Agenda has been saved to disk...\n");
}

void agenda_destroy() {
    agenda_save();
    llist_destroy(agenda);
}
