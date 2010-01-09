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
#ifndef VOTINGS_H
#define VOTINGS_H

#include <libircclient.h>

extern void voting_add(irc_session_t* session, const char* option, const char* voter, const char* host);
extern void voting_init(irc_session_t* session, const char* send_to, char* motive);
extern void voting_report(irc_session_t* session, const char* send_to);
extern void voting_close(irc_session_t* session, const char* send_to);

#define VOTE_TIMEOUT
#endif
