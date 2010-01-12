#ifndef ELIZA_H
#define ELIZA_H

#include <libircclient>

struct eliza_context {
    char host_name[256];
    char last_ans[256];
}

void eliza_init();
void eliza_destroy();
void eliza_answer(irc_session_t* session, char* send_to, char* sender, char* input);

#endif
