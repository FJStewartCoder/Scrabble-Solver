#ifndef BASE_H
#define BASE_H


#include <stdlib.h>


typedef unsigned char byte_t;

typedef int init_func_t();
typedef int destruct_func_t();
typedef int score_func_t(char*, size_t);

typedef struct {
    char *name;

    init_func_t *init;
    destruct_func_t *destruct;
    score_func_t *score;
} algorithm_t;


int create_score_array();
const byte_t get_letter_score(byte_t c);

const int get_word_score(score_func_t *score_func, char *word);


#endif