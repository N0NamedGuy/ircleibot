#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libircclient.h>

#include "external.h"

#include "datastructs/linkedlist.h"
#include "globals.h"
#include "shell.h"
#include "strutils.h"

struct external_entry {
    char botcmd[512];
    char shellcmd[512];
    char level; /*  * -> all
                    + -> voice  
                    @ -> operator */
};

extern void exec_cmd(irc_session_t* session, const char* inputf, const char* args, const char* send_to);
extern bool has_perm(const char* nick, const char level);

struct linked_list* ext_entries;

void exec_cmd(irc_session_t* session, const char* cmd, const char* args, const char* send_to) {
    char to_exec[512];
    char env_vars[512];
    char names[2048];
    char esc_args[1024];
    unsigned int i;
    unsigned int j;

    /* Escape all the args... Remove all the " */
    j = 0;
    for (i = 0; i < strlen(args); i++) {
        if (args[i] == '\"') {
            esc_args[j++] = '\\';
        }
        esc_args[j] = args[i];  
        j++;
    }
    esc_args[j] = 0;

    /* Print out nicknames separeted by : */
    names[0] = 0;
    for (i = 0; i < name_count; i++) {
        strcat(names, name_list[i]);
        strcat(names, ":");
    }

    /* Build the environment variables */
    env_vars[0] = 0;
    sprintf(env_vars, "export BOTARGS=\"%s\"; export BOTNAMES=\"%s\";", esc_args, names);

    to_exec[0] = 0;
    sprintf(to_exec, "%s %s", env_vars, cmd);
    shell_send(session, send_to, to_exec);

}

bool has_perm(const char* nick, const char level) {
    bool op;
    bool voice;

    op = is_op(nick);
    voice = is_voice(nick);

    return 
        (level == '*')
        || ((op || voice) && level == '+')
        || (op && level == '@');
}

void external_init() {
    FILE* fd;
    char buf[512];

    struct external_entry* to_add;
    struct external_entry tmp;
     

    ext_entries = llist_new();

    fd = fopen(EXTERNAL_CONF, "r");

    if (fd == NULL) {
        printf("No external plugins configuration file found...\n");
        return;
    }

    while (fgets(buf, 512, fd)) {
        if (sscanf(buf, "!%s %c %[^\n]s", tmp.botcmd, &(tmp.level), tmp.shellcmd) == 3) {
            to_add = malloc(sizeof(struct external_entry));
            memcpy(to_add, &tmp, sizeof(tmp));
            llist_add_last(ext_entries, (void*)to_add);
        }
    }

    fclose(fd);
}

bool external_check(irc_session_t* session, const char* input, const char* sender, const char* send_to) {
    
    struct llist_iter* iter;
    struct external_entry* cur_entry;
    char tmp_fmt[512];
    char* args;
    char bot_cmd[512];
    bool to_ret;
    int i;

    iter = llist_iter_new(ext_entries);

    to_ret = false;
    while (llist_iter_hasnext(iter)) {
        cur_entry = (struct external_entry*)llist_iter_next(iter);

        /* Lets find the bot command */
        for (i = 1; i < strlen(input); i++) {
            if (input[i] == ' ' || input[i] == '\0') {
                bot_cmd[i - 1] = '\0';
                break;
            } else {
                bot_cmd[i - 1] = input[i];
            }
        }
        bot_cmd[i] = '\0';


        if (strcmp(bot_cmd, cur_entry->botcmd) == 0 &&  has_perm(sender, cur_entry->level)) {
            /* Lets get us the argument */
            args = &input[strlen(bot_cmd) + 1];

            exec_cmd(session, cur_entry->shellcmd, args, send_to);
            to_ret = true;
            break;
        }
    }

    llist_iter_destroy(iter);

    return to_ret;
}

void external_help(irc_session_t* session, const char* input, const char* sender, const char* send_to) {
    
}

void external_destroy() {
    llist_destroy(ext_entries);
}
