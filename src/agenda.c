#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <libircclient.h>

#include "datastructs/linkedlist.h"
#include "agenda.h"

struct linked_list* agenda;

void agenda_init() {
    FILE* fd;
    
    char buf[256];
    struct agenda_entry* entry;
    struct tm cur_date;
    char subject[256];

    fd = fopen(AGENDA_PATH, "r");
    
    agenda = llist_new();
    
    if (fd != NULL) {
        while (fgets(buf, 256, fd) != NULL) {
                
            if (sscanf(buf, "%d %d %d %d %d %[^\n]s ",
                &cur_date.tm_mday,
                &cur_date.tm_mon,
                &cur_date.tm_year,
                &cur_date.tm_hour,
                &cur_date.tm_min,
                subject) == 6) {
                
                entry = malloc(sizeof(struct agenda_entry));
                llist_add_last(agenda, (void*)entry);
            }
            
        }

    }

}

void agenda_add(const struct agenda_entry* entry) {
    llist_add_last(agenda, (void*)entry);
}

struct agenda_entry* agenda_get(unsigned long pos) {
    return llist_get(agenda, pos);
}

bool agenda_del(unsigned long pos) {
    return llist_remove(agenda, pos);
}

void agenda_list(irc_session_t* session, char* send_to, struct tm* date) {
    struct llist_iter* iter;
    struct agenda_entry* cur_entry;
    struct tm* entry_tm;

    char buf[256];

    iter = llist_iter_new(agenda);

    while (llist_iter_hasnext(iter)) {
        cur_entry = (struct agenda_entry*)llist_iter_next(iter);
        entry_tm = &(cur_entry->date);

        if (   (date->tm_mon  == entry_tm->tm_mon  || date->tm_mon  == -1) 
            && (date->tm_year == entry_tm->tm_year || date->tm_year == -1)
            && (date->tm_mday == entry_tm->tm_mday || date->tm_mday == -1)) {

            sprintf(buf, "[%04ld] (%02d-%02d-%04d) %02d:%02d %s",
                llist_iter_pos(iter),
                entry_tm->tm_mday,
                entry_tm->tm_mon + 1,
                entry_tm->tm_year + 1900,
                entry_tm->tm_hour,
                entry_tm->tm_min,
                cur_entry->subject);
            irc_cmd_msg(session, send_to, buf);           
        }
    }

    llist_iter_destroy(iter);
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
            cur_entry->date.tm_mday,
            cur_entry->date.tm_mon,
            cur_entry->date.tm_year,
            cur_entry->date.tm_hour,
            cur_entry->date.tm_min,
            cur_entry->subject);

        fputs(buf, fd);
    }
    llist_iter_destroy(iter);

    fclose(fd);
}

void agenda_destroy() {
    agenda_save();
    llist_destroy(agenda);
}
