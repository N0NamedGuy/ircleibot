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
