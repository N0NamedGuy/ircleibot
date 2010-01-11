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
#include <pthread.h>
#include <stdbool.h>
#include <libircclient.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "cli.h"
#include "globals.h"
#include "botcmd.h"

pthread_t cli_th;
bool run;

void* do_cli(void* session) {
    
    char *buf;
    
    while (run && !bot_exiting) {
        buf = readline("> ");
        
        if (buf != NULL) {
            add_history(buf);
    
            
            if (strcmp(buf, "/quit") == 0) {
                botcmd_quit((irc_session_t*)session);
            
            } else if (buf[0] == '/' && buf[1] != '/') {
                irc_send_raw((irc_session_t*)session, &buf[1]);
        
            } else if (buf[0] == '!') {
                botcmd_parse((irc_session_t*)session, buf, "?", "?", 0);

            } else {
                irc_cmd_msg((irc_session_t*)session, bot_channel, buf);
            }
        
            free(buf);
        }
        
    }

    return NULL;
}

void cli_destroy() {
    run = false;
    pthread_cancel(cli_th);
}

void cli_init(irc_session_t* session) {
    run = true;
    pthread_create(&cli_th, NULL, do_cli, (void*)session);
}
