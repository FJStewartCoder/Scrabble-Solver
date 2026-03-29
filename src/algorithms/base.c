#include "base.h"

// required for memset
#include <string.h>


// score for each letter from a - z
const static byte_t letter_scores[26] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};

// the score for each character
static byte_t score_array[256];


int create_score_array() {
    memset(&score_array, 0, sizeof(score_array));

    // for each character between 'a' and 'z' set the scores
    for (byte_t c = 'a'; c <= 'z'; c++) {
        // set the score array at the index
        // for the character in the letter scores
        score_array[c] = letter_scores[c - 'a'];
    }

    // for each character between 'A' and 'Z' set the scores
    for (byte_t c = 'A'; c <= 'Z'; c++) {
        // set the score array at the index
        // for the character in the letter scores
        score_array[c] = letter_scores[c - 'A'];
    }

    return 0;
}

const byte_t get_letter_score(byte_t c) {
    return score_array[c];
}

const int get_word_score(score_func_t *score_func, char *word) {
    return score_func(word, strlen(word));
}