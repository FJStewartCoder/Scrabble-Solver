#ifndef RAM_ABUSE_H
#define RAM_ABUSE_H

#include <stdlib.h>

int create_ram_abuse_score_array();
const int ram_abuse_score(char *word, size_t length);
int destruct_ram_abuse_score_array();

#endif