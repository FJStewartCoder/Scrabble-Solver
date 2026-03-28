#include "shortptr.h"
#include "base.h"

#include <string.h>


static byte_t different_score_array[256 * 256];


int create_different_score_array() {
    // base score array is required for calculations
    create_score_array();

    // set all of the memory to 0s by default
    memset(&different_score_array, 0, sizeof(different_score_array));

    // get the size of the array
    const size_t array_size = sizeof(different_score_array) / sizeof(different_score_array[0]);

    // iterate every index of the array
    for (size_t i = 0; i < array_size; i++) {
        // break the index up into two characters as the components
        byte_t c1 = i & 0xff;
        byte_t c2 = (i >> 8) & 0xff;

        // calculate the score of the two characters and get the sum
        const byte_t score = get_letter_score(c1) + get_letter_score(c2);

        // set the index to the score (the index created the characters)
        different_score_array[i] = score;
    }

    return 0;
}

static const int calc_score(char *word, size_t length) {
    // initialise the score
    int score = 0;

    // calculate the remainder (prevents segfault)
    int remainder = length % 2;

    // if there is a remainder, calculate the score of the first character
    if (remainder) { score += get_letter_score(word[0]); }

    // start at the remainder and iterate the word 2 characters at a time
    for (size_t idx = remainder; idx < length; idx += 2) {
        // word is list of bytes
        // short is two bytes
        // convert the char* to ushort* to combine the two characters into a single number
        // dereference to get the number which is the index in the array
        unsigned short *cur = (unsigned short*)(word + idx);

        // add, to the score, the score in the score array at the index
        score += different_score_array[*cur];
    }

    // return the score
    return score;
}

// wrapper for the calc_score function because it requires the word length
// this could allow for multithreading
const int get_different_word_score(char *word) {
    const int length = strlen(word);

    return calc_score(word, length);
}