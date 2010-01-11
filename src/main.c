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
#include <time.h>
#include <signal.h>
#include <string.h>
#include <libircclient.h>

#include "globals.h"
#include "botcmd.h"
#include "events.h"
#include "greeter.h"
#include "cli.h"
#include "agenda.h"

irc_callbacks_t callbacks;
irc_session_t* bot_session;

void init() {
    greeter_init();
    agenda_init();
}

void quit() {
    botcmd_quit(bot_session);
}

int main(int argc, char** argv) {

    if (argc != 5) {
        printf("%s <server> <nick> <#channel> <nicksev_pw>\n", argv[0]);
        printf("%d", argc);
        return 1;
    }

    srandom(time(NULL));

    strcpy(bot_nick, argv[2]);
    strcpy(bot_channel, argv[3]);
    strcpy(ns_pass, argv[4]);

    init();
    events_init(&callbacks);
    
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, quit);

    do {
        bot_session = irc_create_session(&callbacks);
#ifdef CLI_INPUT  
        cli_init(bot_session);
#endif        
        bot_identified = false;
        printf("Connecting to server...\n");

        if (irc_connect(bot_session, argv[1], BOT_PORT, NULL, argv[2], NULL, NULL) != 0) {
            printf("Err %d: %s\n", irc_errno(bot_session), irc_strerror(irc_errno(bot_session)));
            return 1;
        }


        if (irc_run(bot_session) == 0) {
            printf("All ok! =D\n");
        } else {
            printf("Err %d: %s\n", irc_errno(bot_session), irc_strerror(irc_errno(bot_session)));
        }

#ifdef CLI_INPUT
        cli_destroy();
#endif
        irc_destroy_session(bot_session);
#ifdef AUTO_RECONNECT
    } while(!bot_exiting);        
#else
    } while(0);
#endif

    greeter_destroy();
    agenda_destroy();

    printf("All done!\n");
    return 0;
}
