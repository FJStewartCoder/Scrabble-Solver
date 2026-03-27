#include "shortptr.h"
#include "base.h"

#include <string.h>


static short different_score_array[256 * 256];

static const int chars_to_int(char c1, char c2) {
    return 0 | c2 | (c1 << 8);
}

int create_different_score_array() {
    memset(&different_score_array, 0, sizeof(different_score_array));

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            different_score_array[chars_to_int(i, j)] = get_letter_score(i) + get_letter_score(j);
        }
    }

    return 0;
}

const int get_different_word_score(char *word, size_t length) {
    int score = 0;

    // size_t length = strlen(word);

    // this magically does not go over the length of the string due to the null terminator
    // for odd or even it works differently but null terminator saves us
    for (size_t idx = 0; idx < length; idx += 2) {
        // word is list of bytes
        // short is two bytes
        // convert the pointer to char to pointer to character to combine the two characters
        // dereference to get the number which is the index in the array
        short *cur = (short*)(word + idx);

        score += different_score_array[*cur];
    }

    return score;
}