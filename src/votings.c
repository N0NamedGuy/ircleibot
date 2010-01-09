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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "votings.h"
bool vote_open = false;

char vote_opts[1000][256];
int  vote_count[1000];
int  vote_cursor = -1;

char voters[1000][256];
int  voters_cursor;

char reason[256];

#ifdef VOTE_TIMEOUT
irc_session_t* alarm_session;
char alarm_send_to[256];
#endif

void swap_vote(int a, int b) {
    char tmp_opt[256];
    int tmp_count;

    strcpy(tmp_opt, vote_opts[a]);
    tmp_count = vote_count[a];

    strcpy(vote_opts[a], vote_opts[b]);
    vote_count[a] = vote_count[b];

    strcpy(vote_opts[b], tmp_opt);
    vote_count[b] = tmp_count;
}

void voting_add(irc_session_t* session, const char* option, const char* voter, const char* host) {
    int i;
    int j;
    char buf[256];
    
    if (!vote_open) {
        irc_cmd_notice(session, voter, "There's no open voting");
        return;
    }

    /* Check if voted */

    for (i = 0; i < voters_cursor; i++) {
        if (strcmp(voters[i], host) == 0) {
            irc_cmd_notice(session, voter, "You already voted...");
            return;
        }
    }

    strcpy(voters[voters_cursor++], host);

    for (i = 0; i < vote_cursor; i++) {
        if (strcmp(vote_opts[i], option) == 0) {
            vote_count[i]++;
            break;
        }
    }

    if (i == vote_cursor) {
        strcpy(vote_opts[i], option);
        vote_count[i] = 1;
        vote_cursor++;
    }

     /* Sort in place... */
    for (j = i - 1; j >= 0; j--) {
        if (vote_count[j] < vote_count[i]) {
            swap_vote(i,j);
        }
    }

    sprintf(buf, "Voted for %s [Vote count: %i]", option, vote_count[i]);
    irc_cmd_notice(session, voter, buf);
    printf("%s\n", buf);
}

#ifdef VOTE_TIMEOUT
void voting_timeout() {
    signal(SIGALRM, SIG_IGN);
    irc_cmd_msg(alarm_session, alarm_send_to, "Voting timeout for:");
    voting_report(alarm_session, alarm_send_to);
    vote_open = false;
}
#endif


void voting_init(irc_session_t* session, const char* send_to, char* motive) {
    vote_cursor = 0;
    voters_cursor = 0;
    
    if (vote_open) {
        voting_close(session, send_to);
    }


    strcpy(reason, motive);
    vote_open = true;

#ifdef VOTE_TIMEOUT
    alarm_session = session;
    strcpy(alarm_send_to, send_to);
#endif

    irc_cmd_msg(session, send_to, "Opened voting for:");
    irc_cmd_msg(session, send_to, reason);

#ifdef VOTE_TIMEOUT
    signal(SIGALRM, voting_timeout); 
    alarm(120);
#endif
}

void voting_close(irc_session_t* session, const char* send_to) {
#ifdef VOTE_TIMEOUT
    signal(SIGALRM, SIG_IGN);
#endif
    irc_cmd_msg(session, send_to, "Closed voting for:");
    voting_report(session, send_to);
    vote_open = false;
}

void voting_report(irc_session_t* session, const char* send_to) {
    char buf[256];
    int i;

    if (!vote_open) {
        if (vote_cursor == -1) {
            irc_cmd_notice(session, send_to, "There's no open voting");
            return;
        } else {
            irc_cmd_notice(session, send_to, "The voting is closed");
        }
    }

    irc_cmd_msg(session, send_to, reason);

    for (i = 0; i < vote_cursor; i++) {
        sprintf(buf, "%d - <%s> %d votes", i + 1, vote_opts[i], vote_count[i]);
        irc_cmd_msg(session, send_to, buf);
    }
}
