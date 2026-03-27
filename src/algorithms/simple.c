#include "simple.h"
#include "base.h"


const int get_word_score(char *word) {
    int score = 0;

    // while the character is not null terminator
    while (*word != '\0') {
        // add the current score to the total score
        score += get_letter_score(*word);

        // move the pointer for the word along
        word++;
    }

    return score;
}