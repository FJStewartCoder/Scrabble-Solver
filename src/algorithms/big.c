#include "big.h"
#include "base.h"

#include <string.h>


static short big_score_array[256][256][256];

int create_big_score_array() {
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
        big_score_array[c1][c2][c3] = score;
    }

    return 0;
}

const int get_big_word_score(char *word) {
    int score = 0;

    size_t length = strlen(word);

    for (size_t idx = 0; idx < length; idx += 3) {
        char c1 = word[idx];
        char c2 = word[idx + 1];
        char c3 = word[idx + 2];

        score += big_score_array[c1][c2][c3];
    }

    return score;
}