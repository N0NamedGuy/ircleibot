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
