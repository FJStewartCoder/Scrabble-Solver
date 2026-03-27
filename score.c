#include "score.h"

#include <string.h>
#include <stdio.h>


// score for each letter from a - z
const static short letter_scores[26] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};

// the score for each character
static short score_array[256];


int create_score_array() {
    memset(&score_array, 0, sizeof(score_array));

    // for each character between 'a' and 'z' set the scores
    for (char c = 'a'; c <= 'z'; c++) {
        // set the score array at the index
        // for the character in the letter scores
        score_array[c] = letter_scores[c - 'a'];
    }

    // for each character between 'A' and 'Z' set the scores
    for (char c = 'A'; c <= 'Z'; c++) {
        // set the score array at the index
        // for the character in the letter scores
        score_array[c] = letter_scores[c - 'A'];
    }

    return 0;
}

const int get_letter_score(char c) {
    return score_array[c];
}


// --------------------------------------------------------------------------------------------

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

// --------------------------------------------------------------------------------------------

static short big_score_array[256][256][256];

int create_big_score_array() {
    memset(&big_score_array, 0, sizeof(big_score_array));

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            for (int k = 0; k < 256; k++) {
                big_score_array[i][j][k] = get_letter_score(i) + get_letter_score(j) + get_letter_score(k);
            }
        }
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

// --------------------------------------------------------------------------------------------

static short different_score_array[256 * 256];

const int chars_to_int(char c1, char c2) {
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

// --------------------------------------------------------------------------------------------

typedef unsigned char byte_t;

// will be malloced
static byte_t *ram_abuse_score_array;

const size_t four_chars_to_int(byte_t c1, byte_t c2, byte_t c3, byte_t c4) {
    return c4 | (c3 << 8) | (c2 << 16) | (c1 << 24);
}

int create_ram_abuse_score_array() {
    size_t array_size = 0xffffffff;
    ram_abuse_score_array = calloc(array_size, sizeof(byte_t));

    printf("%u\n", array_size);

    if ( ram_abuse_score_array == NULL ) {
        puts("Failed to allocate score array");
        return 1;
    }

    puts("Allocated score array");

    FILE *score_fp = fopen("score_sheet.txt", "r");

    if ( score_fp != NULL ) {
        puts("Score sheet exists");

        fread(ram_abuse_score_array, sizeof(byte_t), array_size, score_fp);

        int res = 0;

        if (ferror(score_fp) != 0) {
            res = 1;
            puts("File loading had an error");
        }

        fclose(score_fp);
        return res;
    }

    puts("Score sheet does not exist");
    fclose(score_fp);

    for (size_t i = 0; i < array_size; i++) {
        byte_t c1 = i & 0xff;
        byte_t c2 = (i >> 8) & 0xff;
        byte_t c3 = (i >> 16) & 0xff;
        byte_t c4 = (i >> 24) & 0xff;

        byte_t score = get_letter_score(c1) + get_letter_score(c2) + get_letter_score(c3) + get_letter_score(c4);
        ram_abuse_score_array[i] = score;

        if ( (i % 1000000) == 0 ) {
            printf("%u\n", i);
        }
    }

    FILE *fp = fopen("score_sheet.txt", "wb");

    // write to the file
    fwrite(ram_abuse_score_array, sizeof(byte_t), array_size, fp);

    fclose(fp);

    return 0;
}

int decreate_ram_abuse_score_array() {
    free(ram_abuse_score_array);
    return 0;
}

const int get_ram_abuse_word_score(char *word) {
    int score = 0;

    size_t length = strlen(word);

    int remainder = length % 4;

    if (remainder != 0) {
        for (int i = 0; i < remainder; i++) {
            score += get_letter_score(word[i]);
        }
    }

    for (size_t idx = remainder; idx < length; idx += 4) {
        unsigned int *cur = (unsigned int*)(word + idx);

        score += ram_abuse_score_array[*cur];
    }

    return score;
}