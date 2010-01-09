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
#include <libircclient.h>
#include <string.h>
#include <stdbool.h>

#include "botcmd.h"
#include "globals.h"
#include "shell.h"
#include "strutils.h"
#include "votings.h"
#include "logger.h"
#include "greeter.h"

/* Credits to micah89 */
int fibo_iter(int n) {
    int i;
    int penultimate;
    int last;
    int current;

    if (n == 0 || n == 1)
        return n;
    else {
        last = 0;
        current = 1;
        for (i = 2; i <= n; i++) {
            penultimate = last;
            last = current;
            current = penultimate + last;
        }
        return current;
     }
}
/*
void botcmd_clones(irc_session_t* session, const char* send_to, const char* sender) {
    bool* checked;
    unsigned int i;
    unsigned int j;
    char buf[256];
    bool found;

    printf("Clones query requested by %s\n", sender);
    checked = malloc(name_count * sizeof(bool));

    for (i = 0; i < name_count; i++) {
        for (j = i + 1; j < name_count; j++) {
            strcpy(buf, "");
            sprintf(buf, "%s is clone of: ", name_list[i]);
            found = false;

            if (strcmp(host_list[i], host_list[j]) == 0) {
                strcat(buf, " ");
                strcat(buf, name_list[i]);
                checked[j] = true;
            }
        }
    
        if (found) {
            printf("Clone Results: %s", buf);
            irc_cmd_notice(session, send_to, buf);
        }

        checked[i] = true;
    }

    free(checked);
}
*/
void botcmd_ping(irc_session_t* session, const char* send_to, const char* sender) {
    printf("Ping requested from %s\n", sender);

    irc_cmd_notice(session, send_to, "!pong");
}

/* Credits to lastdance */
void botcmd_slots(irc_session_t* session, const char* send_to, const char* sender) {
    int option;
    char buf[256];
    
    buf[0] = 0;
    option = rand() % 10;

    printf("Doing Slots: %d came out...\n", option);

    switch (option) {
    case 1:
        irc_cmd_kick(session, sender, send_to, "WIN!!1!11!ONE");
        break;

    case 2:
        irc_cmd_msg(session, send_to, "ENORMES SEIOS!");
        break;

    case 3:
        sprintf(buf, "MODE %s -v %s", bot_channel, sender);
        irc_send_raw(session, buf);
        sprintf(buf, "MODE %s -o %s", bot_channel, sender);
        irc_send_raw(session, buf);
/*        sprintf(buf, "CHANSERV ACCESS %s DEL %s", bot_channel, sender);
        irc_send_raw(session, buf); */
        break;

    case 4:
        sprintf(buf, "MODE %s +o %s", bot_channel, sender);
        irc_send_raw(session, buf);
        irc_cmd_msg(session, send_to, "GANHASTE OP!");
        irc_cmd_kick(session, sender, bot_channel, "solved");
        break;

    case 5:
        botcmd_fortune(session, send_to);
        break;

    case 6:
        irc_cmd_msg(session, send_to, "It's time to kick bubble gum and chew some ass!"); 
        botcmd_randkick(session);
        break;

    case 7:
        irc_cmd_msg(session, send_to, ".|.");
        break;

    case 8:
        irc_cmd_msg(session, send_to, "Nao mandas em mim!");
        break;

    case 9:
        irc_cmd_msg(session, send_to, "http://www.youtube.com/watch?v=oHg5SJYRHA0");
        break;

    case 0:
        irc_cmd_msg(session, send_to, "Perdeste.");
        break;
    }
}

/*
void botcmd_slots(irc_session_t* session, const char* send_to, const char* sender) {
    int n;
    char slots_c[3];
    int i;
    char buf[5];

    for (i = 0; i < 3; i++) {
        n = rand() % 3;

        switch (n) {
        case 0:
            slots_c[i] = 'V';
            break;

        case 1:
            slots_c[i] = 'O';
            break;

        case 2:
            slots_c[i] = 'K';
            break;
        }
    }

    sprintf(buf, "%c|%c|%c", slots_c[0], slots_c[1], slots_c[2]);
    if (slots_c[0] == slots_c[1] && slots_c[0] == slots_c[2]) {
        irc_cmd_msg(session, send_to, buf);
    } else {
        irc_cmd_notice(session, sender, buf);
    }
}
*/



/* Credits to micah89 */
void botcmd_fibonacci(irc_session_t* session, const char* send_to, const char* max_str, const char* str) {
    int i;
    int j;
    char *buf;
    int max;
    int fib;

    max = atoi(max_str);

    if (max >= 10) {
        return;
    }

    for (i = 1; i <= max ;i++) {
        fib = fibo_iter(i);
        
        buf = malloc((sizeof(char) * (strlen(str) + 1) * fib) + 1);
        strcpy(buf, "");

        for (j = 0; j < fib; j++) {
            strcat(buf, str);
            strcat(buf, " ");
        }
        irc_cmd_msg(session, send_to, buf);
        free(buf);
   }
}


void botcmd_greet(irc_session_t* session, const char* send_to, const char* channel) {
    char buf[256];
    char greet[256];

    char send_to_c[256];
    
    strcpy(send_to_c, send_to);
    str_trim(send_to_c);


    if (greeter_on) {

        greeter_get(greet);

        if (strlen(greet) > 0) {
            sprintf(buf, greet, send_to_c);
            irc_cmd_msg(session, channel, buf);
        }
    }
}

void botcmd_google(irc_session_t* session, const char* send_to, const char* query) {
    char buf[256];
    char* str;
   
    sprintf(buf, "http://google.pt/search?q=%s", query);

    str = str_replace(" ", "%20", buf);
    irc_cmd_msg(session, send_to, str);

}

void botcmd_randkick(irc_session_t* session) {
    int n;
    char* to_kick;

    n = random() % name_count;
    
    if (name_list[n][0] != '+' && name_list[n][0] != '@') {
        to_kick = &name_list[n][0];
    } else {
        to_kick = &name_list[n][1];
    }

    irc_cmd_msg(session, bot_channel, "Random kick to: ");
    irc_cmd_msg(session, bot_channel, to_kick);
    irc_cmd_kick(session, to_kick, bot_channel,"Random kick");
    
}

void botcmd_fortune(irc_session_t* session, const char* send_to) {
    shell_send(session, send_to, "fortune -os | tr '\n' ' '");
}

void botcmd_quit(irc_session_t* session) {
    irc_cmd_quit(session, BOT_QUIT_MSG);

    printf("Quitting\n");
    exit(0);
}

void botcmd_callall(irc_session_t* session, const char* send_to) {
    unsigned int i;
    char buf[256];

    buf[0] = 0;
    for (i = 0; i < name_count; i++) {
        if (name_list[i][0] == '+' || name_list[i][0] == '@') {
            strcat(buf, &name_list[i][1]);
        } else {
            strcat(buf, &name_list[i][0]);
        }
        strcat(buf, " ");
    }

    irc_cmd_msg(session, send_to, buf);
}

/*
    source values:

    0 -> Public from bot_channel
    1 -> Private message to bot
*/
bool botcmd_parse(irc_session_t* session, const char* cmd, const char* sender,
    const char* host, int source) {

    char args[5][256];

    if (cmd[0] != '!') {
        return false;
    } else {
        printf("Got command: %s from %s.\n", cmd, sender);
    }

     if (sscanf(cmd, "!vote %[^\n]s", args[1]) == 1) {
        voting_add(session, args[1], sender, host);
        return true;


    } else if (strcmp(cmd, "!showvotes") == 0) {
        printf("Showing vote list\n");
        voting_report(session, sender);
        return true;


    } else if (strcmp(cmd, "!broadcastvotes") == 0 
        && is_op(sender)) {

        printf("Broadcasting vote list\n");
        voting_report(session, bot_channel);
        return true;


    } else if (sscanf(cmd, "!openvote %[^\n]s", args[0]) == 1
        && is_op(sender)) {     
    
        printf("Opening voting: %s\n", args[0]);
        voting_init(session, bot_channel, args[0]);
        return true;


    } else if (strcmp(cmd, "!closevote") == 0
        && is_op(sender)) {     
    
        printf("Closing current voting\n");
        voting_close(session, bot_channel);
        return true;


    } else if (sscanf(cmd,"!grep %[^\n]s", args[0]) == 1) {
        if (!source) {
            grep_log(session, bot_channel, args[0]);
        } else {
            grep_log(session, sender, args[0]);
        }
        return true;

    } else if (strcmp(cmd, "!fortune") == 0) {
        if (!source) {
            botcmd_fortune(session, bot_channel);
        } else {
            botcmd_fortune(session, sender);
        }
        return true;

    } else if (strcmp(cmd, "!randkick") == 0
        && is_op(sender))  {
        botcmd_randkick(session);
        return true;

    } else if (sscanf(cmd, "!google %[^\n]s", args[0]) == 1) {
        if (!source) {
            botcmd_google(session, bot_channel, args[0]);
        } else {
            botcmd_google(session, sender, args[0]);
        }
        return true;

    } else if (strcmp(cmd, "!callall") == 0) {
        if (!source) {
            botcmd_callall(session, bot_channel);
        } else {
            irc_cmd_msg(session, sender, "You can't use this command in private...");
        }
        return true;

    } else if (sscanf(cmd, "!greet %[^\n]s", args[0]) == 1) {
        if (!source) {
            botcmd_greet(session, args[0], bot_channel);
        } else {
            botcmd_greet(session, args[0], sender);
        }
        return true;

    } else if (strcmp(cmd, "!enablegreet") == 0
        && is_op(sender)) {
        printf("Greeter is now off...\n");
        greeter_on = true;
        return true;

    } else if (strcmp(cmd, "!disablegreet") == 0
        && is_op(sender)) {
        printf("Greeter is now on...\n");
        greeter_on = false;
        return true;

    } else if (sscanf(cmd, "!fibonacci %s %[^\n]s", args[0], args[1]) == 2 
        && is_op(sender)) {
        if (!source) {
            botcmd_fibonacci(session, bot_channel, args[0], args[1]);
        } else {
            botcmd_fibonacci(session, sender, args[0], args[1]);
        }
        /* irc_cmd_kick(session, sender, bot_channel, "FODE-TE"); */
        return true;

    } else if (strcmp(cmd, "!slots") == 0) {
        if (!source) {
            botcmd_slots(session, bot_channel, sender);
        } else {
            botcmd_slots(session, sender, sender);
        }
        
        return true;
   
    } else if (strcmp(cmd, "!ping") == 0) {
        botcmd_ping(session, sender, sender);

        return true;

    } else if (strcmp(cmd, "!clones") == 0) {
        /* botcmd_clones(session, sender, sender); */
        irc_cmd_notice(session, sender, "Comming soon...");
        return true;

    } else if (strcmp(cmd, "!help") == 0) {
        irc_cmd_notice(session, sender, "Command list:");
        irc_cmd_notice(session, sender, "!callall");
        irc_cmd_notice(session, sender, "!clones");
        irc_cmd_notice(session, sender, "!google <search query>");
        irc_cmd_notice(session, sender, "!grep <search query>");
        if (greeter_on) irc_cmd_notice(session, sender, "!greet <something>");
        irc_cmd_notice(session, sender, "!help");
        irc_cmd_notice(session, sender, "!ping");
        irc_cmd_notice(session, sender, "!slots");
        irc_cmd_notice(session, sender, "!showvotes");
        irc_cmd_notice(session, sender, "!vote <option>");
        
        if (is_op(sender)) {
            irc_cmd_notice(session, sender, "Op command list:");
            irc_cmd_notice(session, sender, "!broadcastvotes");
            irc_cmd_notice(session, sender, "!closevote");
            irc_cmd_notice(session, sender, "!disablegreet");
            irc_cmd_notice(session, sender, "!enablegreet");
            irc_cmd_notice(session, sender, "!fibonacci <n> <string>");
            irc_cmd_notice(session, sender, "!openvote <question to vote>");
            irc_cmd_notice(session, sender, "!randkick");
        }
        return true;

    }

    return false;
}
