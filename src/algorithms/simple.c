#include "simple.h"
#include "base.h"


const int simple_score(char *word, size_t length) {
    int score = 0;

    for ( int i = 0; i < length; i++ ) {
        score += get_letter_score(word[i]);
    }

    return score;
}