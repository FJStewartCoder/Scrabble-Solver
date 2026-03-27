#include <stdlib.h>

int create_score_array();
const int get_letter_score(char c);
const int get_word_score(char *word);

int create_big_score_array();
const int get_big_word_score(char *word);

int create_different_score_array();
const int get_different_word_score(char *word, size_t length);

int create_ram_abuse_score_array();
const int get_ram_abuse_word_score(char *word);
int decreate_ram_abuse_score_array();