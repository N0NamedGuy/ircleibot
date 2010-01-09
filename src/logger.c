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
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "logger.h"
#include "shell.h"

FILE* fd = NULL;

void get_time_str(char* time_str) {
    struct tm *now = NULL;
    time_t time_value = 0;

    time_value = time(NULL);          /* Get time value              */
    now = localtime(&time_value);     /* Get time and date structure */
    
    sprintf(time_str, "%02d-%02d-%04d %02d:%02d:%02d",
         now->tm_mday,
         now->tm_mon + 1,
         now->tm_year + 1900,
         now->tm_hour,
         now->tm_min,
         now->tm_sec);
}

void send_to_log(const char* from, const char* msg) {
    char time_str[16];
    char buf[512];

    if (fd == NULL) {
        fd = fopen(LOG_PATH, "a+");
    }

    get_time_str(time_str);
    sprintf(buf, "(%s) <%s> %s\n", time_str, from, msg);
    printf("%s", buf);
    fputs(buf, fd);
    
    fflush(fd);

}

void grep_log(irc_session_t* session, const char* send_to, const char* grep_str) {
    char buf[256];
    unsigned int i;

    buf[0] = 0;

    /* Ugly escape string */
    for (i = 0; i < strlen(grep_str); i++) {
        if (grep_str[i] == '#' || grep_str[i] == '\'') {
            irc_cmd_msg(session, send_to, "VAI HACKAR A PUTA QUE TE PARIU!");
            return;
        }
    }

    sprintf(buf, GREP_PROG, grep_str, LOG_PATH);
    shell_send(session, send_to, buf); 
}
