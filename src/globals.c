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
#include "globals.h"
#include "string.h"

char ns_pass[256];
char bot_channel[256];
char bot_nick[256];

char name_list[1000][256];
bool name_listing = false;
unsigned int name_count;

bool greeter_on = true;
bool bot_exiting = false;
bool bot_identified = false;

bool is_op(const char* nick) {
    unsigned int i;

    for (i = 0; i < name_count; i++) {
        if (name_list[i][0] == '@' 
            && strcmp(nick, &name_list[i][1]) == 0) {
            return true;
        }
    }

    return false;
}

bool is_voice(const char* nick) {
    unsigned int i;

    for (i = 0; i < name_count; i++) {
        if (name_list[i][0] == '+' 
            && strcmp(nick, &name_list[i][1]) == 0) {
            return true;
        }
    }

    return false;
}

