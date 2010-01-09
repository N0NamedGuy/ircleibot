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
#include "greeter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned long n_greets = 0;
char** greets = NULL;

void greeter_init() {
    FILE* fd = NULL;
    char n_greets_str[13];
    unsigned long i;

    if (fd == NULL) {
        fd = fopen(GREETINGS_PATH, "r");
    }

    if (fgets(n_greets_str, 12, fd) == NULL) {
        fd = NULL;
        return;
    }

    n_greets = atol(n_greets_str);

    greets = malloc(sizeof(char*) * n_greets);
    for (i = 0; i < n_greets; i++) {
        greets[i] = malloc(sizeof(char**) * 256);
        fgets(greets[i], 256, fd);     
        
        /* Strip \n */   
        greets[i][strlen(greets[i]) - 1] = '\0';
    }

    printf("Greetings Loaded...\n");
}

void greeter_get(char* greet) {
    unsigned long n;

    if (n_greets == 0) {
        return;
    }

    n = rand() % n_greets;
    strcpy(greet, greets[n]);
}

void greeter_destroy() {
    unsigned long i;
    
    if (greets != NULL) {
        for (i = 0; i < n_greets; i++) {
            free(greets[i]);
        }
        
        free(greets);
    }
}
