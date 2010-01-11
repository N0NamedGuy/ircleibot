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
#include <stdbool.h>
#include <libircclient.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "events.h"
#include "globals.h"
#include "botcmd.h"
#include "logger.h"


static void* refresh_names(void* session) {

    /* Loop forever */
    while (!bot_exiting) {
        irc_cmd_names((irc_session_t*)session, bot_channel);
        sleep(10);
    }

    return NULL;
}

static void on_event(   irc_session_t *session,
                        unsigned int event,
                        const char *origin,
                        const char **params,
                        unsigned int count) {
    
    unsigned int init_count;
    unsigned int i;

    char buf[256] = "";
    char* tok;

    switch (event) {
    case LIBIRC_RFC_RPL_NAMREPLY:
        if (name_listing == false) {
            name_listing = true;
            name_count = 0;
/*            printf("Channel listing for %s\n", bot_channel);*/
        }
        
        init_count = name_count;

        i = 0;
        strcpy(buf, params[3]);
       
        tok = strtok(buf, " ");
        if (tok != NULL) {         
            do {
                strcpy(name_list[i], tok);
/*                printf("%s\n", name_list[i]);*/
                i++;

            } while((tok = strtok(NULL, " ")));
        }

        name_count += i;
        
        break;

    case LIBIRC_RFC_RPL_ENDOFNAMES:
        name_listing = false;
        break;

    case LIBIRC_RFC_RPL_UMODEIS:
        printf("Mode has changed...\n");
        irc_cmd_names(session, bot_channel);
        break;
    }

/*    printf("Event: %d \n", event); */
}

static void on_join( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    pthread_t refresh_th;
   
    char nick[256];
    irc_target_get_nick(origin, nick, 256);

    printf("%s Joined %s\n", nick, params[0]);
    irc_cmd_names(session, params[0]);
   
    if (strcmp(params[0], bot_channel) == 0) { 
        pthread_create(&refresh_th, NULL, refresh_names, (void*)session);
    } 
    
    if (greeter_on && strcmp(bot_nick, nick) != 0) {
        printf("Greeting %s\n", nick);
        botcmd_greet(session, nick, bot_channel);
    }
}

static void on_nick( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    printf("%s changed to %s\n", origin, params[0]);
    irc_cmd_names(session, bot_channel);
}

static void on_connect() {

    printf("Connected!\n");

}

static void on_kick( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {


    printf("Kicked from %s\n", params[0]);
#ifdef AUTO_REJOIN
    irc_cmd_join(session, params[0], NULL);    
#endif
}

static void on_privmsg( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    char nick[256];
    char host[256];
    char *msg;

    
    /* printf("<%s> %s \n", origin, params[1]); */
    irc_target_get_nick(origin, nick, 256);
    irc_target_get_host(origin, host, 256);
    msg = irc_color_strip_from_mirc(params[1]);  

    if ( (!is_voice(nick) && !is_op(nick))
        || ((is_voice(nick) || is_op(nick)) && !botcmd_parse(session, msg, nick, host, 1))) {
     
        send_to_log(nick, msg);

    }

    free(msg);
}

static void on_channel( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    char nick[256];
    char host[256];
    char *msg;

    /* printf("<%s> %s \n", origin, params[1]); */
    irc_target_get_nick(origin, nick, 256);
    irc_target_get_host(origin, host, 256);
    msg = irc_color_strip_from_mirc(params[1]);  
    
    if ( (!is_voice(nick) && !is_op(nick))
        || ((is_voice(nick) || is_op(nick)) && !botcmd_parse(session, msg, nick, host, 0))) {
     
        send_to_log(nick, msg);

    }

    free(msg);
}


static void on_notice( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    printf("<%s> %s \n", origin, params[1]);

    if (strcmp(NS_PASSWD_CORRECT, params[1]) == 0) {
        printf("Correct password entered...\n");
        irc_cmd_join(session, bot_channel, NULL);
    } else if (!bot_identified) {
        irc_send_raw(session, "NICKSERV IDENTIFY %s", ns_pass);
        bot_identified = true;
    }
    //irc_cmd_join(bot_session, "#fct", NULL);
}

static void on_ctcp( irc_session_t *session,
                 const char *event,
                 const char *origin,
                 const char **params,
                 unsigned int count) {

    printf("CTCP From <%s> %s \n", origin, params[0]);
}

void events_init(irc_callbacks_t* callbacks) {
    bot_identified = false;

    callbacks->event_numeric = on_event;
    callbacks->event_join = on_join;
    callbacks->event_kick = on_kick;
    callbacks->event_nick = on_nick;
    callbacks->event_notice = on_notice;
    callbacks->event_connect = on_connect;
    callbacks->event_ctcp_req = on_ctcp;
    callbacks->event_privmsg = on_privmsg;
    callbacks->event_channel = on_channel;
}
