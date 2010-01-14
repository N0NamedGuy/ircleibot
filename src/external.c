#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libircclient.h>

#include "datastructs/linkedlist.h"

struct external_entry {
    char botcmd[512];
    char shellcmd[512];
    char level; /*  * -> all
                    + -> voice  
                    @ -> operator */
}

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

void external_check() {

}

void external_help() {

}

void external_destroy() {
    llist_destroy(ext_entries);
}
