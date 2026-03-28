#include "shortptr.h"
#include "base.h"

#include <string.h>


static byte_t different_score_array[256 * 256];


int create_different_score_array() {
    memset(&different_score_array, 0, sizeof(different_score_array));

    const size_t array_size = 256 * 256;

    for (int i = 0; i < array_size; i++) {
        byte_t c1 = i & 0xff;
        byte_t c2 = (i >> 8) & 0xff;

        const byte_t score = get_letter_score(c1) + get_letter_score(c2);
        different_score_array[i] = score;
    }

    return 0;
}

static const int calc_score(char *word, size_t length) {
    int score = 0;

    int remainder = length % 2;

    if (remainder) {
        score += get_letter_score(word[0]);
    }

    for (size_t idx = remainder; idx < length; idx += 2) {
        // word is list of bytes
        // short is two bytes
        // convert the pointer to char to pointer to character to combine the two characters
        // dereference to get the number which is the index in the array
        unsigned short *cur = (unsigned short*)(word + idx);

        score += different_score_array[*cur];
    }

    return score;
}

const int get_different_word_score(char *word) {
    int length = strlen(word);
    return calc_score(word, length);
}