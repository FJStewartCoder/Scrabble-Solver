#include "speed_test.h"

#include <time.h>
#include <stdlib.h>


#define WORD_LENGTH 1000 
#define WORD_COUNT 1000 * 1000


const double speed_test_algorithm(const int (*score_func)(char *)) {
    srand(time(NULL));

    const char alpha[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

    char *random_word = malloc((WORD_LENGTH + 1) * sizeof(char));

    for (size_t i = 0; i < WORD_LENGTH; i++) {
        char rand_char = alpha[rand() % 52];
        random_word[i] = rand_char;
    }

    random_word[WORD_LENGTH] = '\0';

    int score;

    time_t start = clock();

    for (size_t i = 0; i < WORD_COUNT; i++) {
        score = score_func(random_word);
    }

    time_t end = clock();

    free(random_word);

    return (double)(end - start) / CLOCKS_PER_SEC;
}