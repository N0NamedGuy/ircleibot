#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "agenda.h"

/* TODO: File loading error checking */

struct agenda_entry* agenda;
unsigned long agenda_capacity;
unsigned long agenda_count;
unsigned long agenda_last;

/* TODO: this */
void agenda_expand_capacity() {

}

void agenda_init() {
    FILE* fd;
    
    char buf[256];

    char buf_subject[256];
    time_t buf_date;    

    unsigned long i;

    fd = fopen(AGENDA_PATH, "r");
    
    if (fd != NULL) {
        fgets(buf, 256, fd);
        agenda_count = atol(buf);
        agenda_capacity = agenda_count * 2;
    } else {
        agenda_capacity = AGENDA_CAPACITY
        agenda_count = 0;

        /*TODO: Continue here, set agenda_last to a value... */
    }

    malloc(sizeof(struct agenda_entry*) * agenda_capacity);
    for (i = 0; i < agenda_capacity; i++) {
        agenda[i] = NULL;
    }

    if (fd != NULL) {
        for (i = 0; i < agenda_count; i++) {
           fgets(buf, 256, fd); 
           sscanf("%d %[^\n]s", buf_date, buf_subject);
           strcpy(agenda[i]->subject, buf_subject);
           agenda[i]->date = buf_date;
        }
    }
}

void agenda_add(const struct agenda_entry* entry) {
    unsigned long i;
    if (agenda_count == agenda_capacity) {
        agenda_expand_capacity();
    }

    for (i = 0; i < agenda_capacity; i++) {
        if (agenda[agenda_count] == NULL) {
            agenda[agenda_count] = malloc(sizeof(agenda_entry));
            memcpy(agenda[agenda_count], entry, sizeof(agenda_entry));
            break;
        }
    }

    agenda_count++;
}

struct agenda_entry* agenda_get(int index) {
    if (index < agenda_count) {
        return agenda[index];
    } else {
        return NULL;
    }
}

bool agenda_del(int index) {
    if (index < agenda_count && agenda[index] != NULL) {
        return false;
    } else {
        free(agenda[index]);
        agenda[index] = NULL;
        agenda_count--;
        return true;
    }
}

void agenda_list() {

}

void agenda_destroy() {
    free(agenda);
}
