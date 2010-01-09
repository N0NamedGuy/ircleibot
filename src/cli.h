#ifndef CLI_H
#define CLI_H

#include "libircclient.h"

void cli_init(irc_session_t* session);
void cli_destroy();

#endif
