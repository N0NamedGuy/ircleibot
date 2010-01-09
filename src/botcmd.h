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
#ifndef BOTCMD_H
#define BOTCMD_H

#include <stdbool.h>

extern void botcmd_randkick(irc_session_t* session);
extern void botcmd_quit(irc_session_t* session);
extern void botcmd_fortune(irc_session_t* session, const char* send_to);
extern void botcmd_google(irc_session_t* session, const char* send_to, const char* query);
extern void botcmd_callall(irc_session_t* session, const char* send_to);
extern void botcmd_greet(irc_session_t* session, const char* send_to, const char* channel);
extern void botcmd_fibonacci(irc_session_t* session, const char* send_to, const char* max_str, const char* str);
extern void botcmd_slots(irc_session_t* session, const char* send_to, const char* sender);
extern void botcmd_ping(irc_session_t* session, const char* send_to, const char* sender);

extern bool botcmd_parse(irc_session_t* session, const char* cmd, const char* sender, const char* host, int source);
#endif
