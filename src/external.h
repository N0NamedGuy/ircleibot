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
#ifndef EXTERNAL_H
#define EXTERNAL_H

#define EXTERNAL_CONF "external.conf"

#include <stdbool.h>
#include <libircclient.h>

extern void external_init();
extern bool external_check(irc_session_t* session, const char* input, const char* sender, const char* send_to);
extern void external_help(irc_session_t* session, const char* input, const char* sender, const char* send_to);
extern void external_destroy();
#endif
