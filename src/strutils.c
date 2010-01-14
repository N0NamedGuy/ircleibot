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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "strutils.h"

char *str_replace(const char *search, const char *replace, const char *subject) {
    char *replaced = (char*)calloc(1, 1), *temp = NULL;
    char *p = (char*)subject, *p3 = (char*)subject, *p2;
    int  found = 0;
    
    if (search == NULL || replace == NULL || subject == NULL) return NULL;
        if (strlen(search) == 0 || strlen(replace) == 0 || strlen(subject) == 0) return NULL;
             
        while ( (p = strstr(p, search)) != NULL) {
            found = 1;
            temp = realloc(replaced, strlen(replaced) + (p - p3) + strlen(replace));
            if (temp == NULL) {
                free(replaced);
                return NULL;
            }
            replaced = temp;
            strncat(replaced, p - (p - p3), p - p3);
            strcat(replaced, replace);
            p3 = p + strlen(search);
            p += strlen(search);
            p2 = p;
        }
                                                                                    
        if (found == 1) {
            if (strlen(p2) > 0) {
                temp = realloc(replaced, strlen(replaced) + strlen(p2) + 1);
                if (temp == NULL) {
                    free(replaced);
                return NULL;
            }
            replaced = temp;
                strcat(replaced, p2);
            }
        } else {
            temp = realloc(replaced, strlen(subject) + 1);
            if (temp != NULL) {
            replaced = temp;
            strcpy(replaced, subject);
        }
    }
    return replaced;
}

void str_trim(char* str) {
    int i;
    
    for (i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] != ' ') {
            str[i + 1] = '\0';
            break;
        }
    }
}

int str_count(const char* needle, const char* haystack) {
    unsigned int j;
    unsigned int i;
    int count;

    count = 0;
    if (strlen(needle) > strlen(haystack)) {
        return count;
    }

    j = 0; 
    for (i = 0; i < strlen(haystack); i++) {
     
        if (needle[j] == haystack[i]) {
            j++;    
        } else {
            j = 0;
        }
        
        if (j == strlen(needle)) {
            count++;
            j = 0;
        }
    }

    return count;
}

int str_word_count(const char* str) {
    int count;
    bool in_word;
    unsigned int i;

    count = 0;
    in_word = false;

    for (i = 0; i < strlen(str); i++) {
        if (isalpha(str[i]) && !in_word) {
            count++;
            in_word = true;
        
        } else if (!isalpha(str[i]) && in_word) {
            in_word = false;
        }
    }

    return count;
}
