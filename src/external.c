#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libircclient.h>

#include "datastructs/linkedlist.h"
#include "globals.h"
#include "shell.h"

struct external_entry {
    char botcmd[512];
    char shellcmd[512];
    bool nargs;
    char level; /*  * -> all
                    + -> voice  
                    @ -> operator */
}

extern exec_cmd(const char* input_fmt, const char* args, char* output);

struct linked_list ext_entries;

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
        if (sscanf("!%s %c %[^\n]s"), &tmp.botcmd, &tmp.level, &tmp.shellcmd == 3) {
            to_add = malloc(sizeof(struct external_entry));
            llist_add(ext_entries, (void*)to_add);
        }
    }

    fclose(fd);
}

void external_check(irc_session_t* session, const char* input, const char* sender, const char* send_to) {
    
    struct llist_iter* iter;
    struct external_entry cur_entry;
    char tmp_fmt[512];
    char arg[512];
    char cmd[512];

    iter = llist_iter_new(ext_entries);

    while (llist_iter_hasnext(iter)) {
        cur_entry = (struct external_entry*)llist_iter_next(iter);
        tmp_fmt[0] = '\0';
        sprintf(tmp_fmt, "%s \%[^\\n]s", cur_entry->botcmd);
        
        if (sscanf(tmp_fmt, arg) == 1  && has_perm(send_to, cur_entry->level)) {
            /* TODO: Me */
        }
    }

    llist_iter_destroy(iter);

    return false;
}

void external_help() {

}

void external_destroy() {
    llist_destroy(ext_entries);
}
