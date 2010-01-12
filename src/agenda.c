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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <libircclient.h>

#include "datastructs/linkedlist.h"
#include "agenda.h"

extern void send_entry(irc_session_t* session, const char* send_to, const struct agenda_entry* entry, unsigned long pos);

struct linked_list* agenda;

void send_entry(    irc_session_t* session,
                    const char* send_to,
                    const struct agenda_entry* entry,
                    unsigned long pos) {
    char buf[256];
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

    send_entry(session, send_to, entry, pos);
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
    struct timestamp query_t; 
    struct tm* cur_time;
    struct llist_iter* iter;
    struct agenda_entry* cur_entry;
    bool found;
    time_t rawtime;

    found = false;
    query_t.day = -1;
    query_t.month = -1;
    query_t.year = -1;
    query_t.hour = -1;
    query_t.min = -1;

    time(&rawtime);
    cur_time = gmtime(&rawtime);

    if (strcmp(args, "today") == 0 || strcmp(args, "") == 0) {
        query_t.day = cur_time->tm_mday;
        query_t.month = cur_time->tm_mon + 1;
        query_t.year = cur_time->tm_year + 1900;
    
    } else if (strcmp(args, "month") == 0) {
        query_t.month = cur_time->tm_mon + 1;
        query_t.year = cur_time->tm_year + 1900;

    } else if (strcmp(args, "year") == 0) {
        query_t.month = cur_time->tm_year + 1900;

    } else if (!(
        
        strcmp(args, "all") == 0

        ||

        (sscanf(args, "%d-%d-%d",
            &(query_t.day),
            &(query_t.month),
            &(query_t.year)) == 3
        )
        
        ||

        (sscanf(args, "%d:%d",
            &(query_t.hour),
            &(query_t.min)) == 2
        )

        ||

        (sscanf(args, "%d-%d-%d %d:%d",
            &(query_t.day),
            &(query_t.month),
            &(query_t.year),
            &(query_t.hour),
            &(query_t.min)) == 5
        )

        )) {
       
        irc_cmd_notice(session, send_to, "Invalid arguments...");
        return;
    }
    
    iter = llist_iter_new(agenda);

    while (llist_iter_hasnext(iter)) {
        cur_entry = (struct agenda_entry*)llist_iter_next(iter);

        if (   (cur_entry->date.day   == query_t.day   || query_t.day   == -1) 
            && (cur_entry->date.month == query_t.month || query_t.month == -1)
            && (cur_entry->date.year  == query_t.year  || query_t.year  == -1)
            && (cur_entry->date.hour  == query_t.hour  || query_t.hour  == -1)
            && (cur_entry->date.min   == query_t.min   || query_t.min   == -1)) {
            
            found = true;
            send_entry(session, send_to, cur_entry, llist_iter_pos(iter));
        
        }
    }
    llist_iter_destroy(iter);
    
    if (!found) {
        irc_cmd_notice(session, send_to, "No entries were found...");
    }

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

/* This code is just for teh lulz! */
void agenda_export_ics(irc_session_t* session, const char* send_to) {
    struct llist_iter* iter;
    struct agenda_entry* entry;

    iter = llist_iter_new(agenda); 
    
    printf("BEGIN:VCALENDAR\n");
    printf("VERSION:2.0\n");
    printf("PRODID:-//David Serrano//IRC-LEI BOT//EN");
    
    while (llist_iter_hasnext(iter)) {
        printf("BEGIN:VEVENT\n");
        
        entry = (struct agenda_entry*)llist_iter_next(iter);
        
        printf("DTSTART:%04d%02d%02dT%02d%02d00Z\n",
            entry->date.year,
            entry->date.month,
            entry->date.day,
            entry->date.hour,
            entry->date.min);

        printf("SUMMARY:%s\n", entry->subject);

        printf("END:VEVENT\n");
    }

    
    printf("END:VCALENDAR\n");

    llist_iter_destroy(iter);
}


