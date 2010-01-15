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

void exec_cmd(irc_session_t* session, const char* inputf, const char* args, const char* send_to) {
    char to_exec[256];

    if (str_count("%s", inputf) != 1) {
        printf("Invalid command format:  %s\n", inputf);
        return;
    }

    sprintf(to_exec, inputf, args);
    shell_send(session, send_to, to_exec);

}

bool has_perm(const char* nick, const char level) {
    unsigned int i;

    for (i = 0; i < name_count; i++) {
        if (strcmp(name_list[i], nick)) {
            return 
                (nick[0] == level) 
                || (nick[0] == '@'  && level == '+')
                || (level == '*');
                    
        }
    }

    return false;
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
    char args[512];

    iter = llist_iter_new(ext_entries);

    while (llist_iter_hasnext(iter)) {
        cur_entry = (struct external_entry*)llist_iter_next(iter);
        tmp_fmt[0] = '\0';
        sprintf(tmp_fmt, "!%s \%\[^\\n]s", cur_entry->botcmd);
        
        printf("Using format: %s\n", tmp_fmt);

        if (sscanf(input, tmp_fmt, args) == 1  && has_perm(sender, cur_entry->level)) {
            exec_cmd(session, cur_entry->shellcmd, args, send_to);
            return true;
        }
    }

    llist_iter_destroy(iter);

    return false;
}

void external_help(irc_session_t* session, const char* input, const char* sender, const char* send_to) {
    
}

void external_destroy() {
    llist_destroy(ext_entries);
}
