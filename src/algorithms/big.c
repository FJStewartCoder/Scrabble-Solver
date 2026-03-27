#include "big.h"
#include "base.h"

#include <string.h>
#include <stdio.h>


static short big_score_array[256 * 256 * 256];


int create_big_score_array() {
    // this array needs to be initialised before creating this one
    create_score_array();

    memset(&big_score_array, 0, sizeof(big_score_array));

    size_t big_array_size = 256 * 256 * 256;
    
    // iterate each possible combination of values up to the total size
    for (size_t i = 0; i < big_array_size; i++) {
        // break up the index into the different character components
        char c1 = i & 0xff;
        char c2 = (i >> 8) & 0xff;
        char c3 = (i >> 16) & 0xff;

        // get the score and store it
        int score = get_letter_score(c1) + get_letter_score(c2) + get_letter_score(c3);
        big_score_array[i] = score;
    }

    return 0;
}

const int get_big_word_score(char *word) {
    int score = 0;

    size_t length = strlen(word);

    int remainder = length % 3;

    if (remainder != 0) {
        for (int i = 0; i < remainder; i++) {
            score += get_letter_score(word[i]);
        }
    }

    for (size_t idx = remainder; idx < length; idx += 3) {
        unsigned int *cur = (unsigned int*)(word + idx);

        size_t i = (*cur) & 0xffffff;

        score += big_score_array[i];
    }

    return score;
}