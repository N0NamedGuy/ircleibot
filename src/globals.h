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
#define BOT_PORT 6667
/* #define BOT_LISTEN_TO "N0_Named_Guy" */ /* Depecrated in favor of ops have access*/
#define NS_PASSWD_CORRECT "Password Aceite"
/* #define BOT_CHANNEL "#bot" */
#define BOT_QUIT_MSG "Adeus, até à vista..."

#define AUTO_REJOIN
#define AUTO_RECONNECT
#define CLI_INPUT

#include <stdbool.h>

extern char ns_pass[256];
extern char bot_channel[256];
extern char bot_nick[256];

extern char name_list[1000][256];
extern bool name_listing;
extern unsigned int name_count;

extern bool greeter_on;

extern bool is_op(const char* nick);
extern bool is_voice(const char* nick);
