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
#include <string.h>

#include "shell.h"

void shell_send(irc_session_t* session, const char* send_to, const char* cmd) {
    FILE* fd;
    char res[256];

    res[0] = 0;

    printf("Invoking:  %s\n", cmd);

    fflush(NULL); 
    fd = popen(cmd, "r");

    fgets(res, 256, fd);
    pclose(fd);

    if (strcmp(res, "")) {
        irc_cmd_msg(session, send_to, res);
    }
}
