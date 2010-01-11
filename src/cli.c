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
