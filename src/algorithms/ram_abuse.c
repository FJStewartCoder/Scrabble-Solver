#include "ram_abuse.h"
#include "base.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// will be malloced
static byte_t *ram_abuse_score_array;


int create_ram_abuse_score_array() {
    create_score_array();

    size_t array_size = 0xffffffff;
    ram_abuse_score_array = calloc(array_size, sizeof(byte_t));

    printf("%lu\n", array_size);

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

    // don't need to close if it never opened
    // fclose(score_fp);

    for (size_t i = 0; i < array_size; i++) {
        byte_t c1 = i & 0xff;
        byte_t c2 = (i >> 8) & 0xff;
        byte_t c3 = (i >> 16) & 0xff;
        byte_t c4 = (i >> 24) & 0xff;

        byte_t score = get_letter_score(c1) + get_letter_score(c2) + get_letter_score(c3) + get_letter_score(c4);
        ram_abuse_score_array[i] = score;

        if ( (i % 1000000) == 0 ) {
            printf("\r%lu", i);
        }
    }

    // put a new line after outputting each size
    putc('\n', stdout);

    FILE *fp = fopen("score_sheet.txt", "wb");

    // write to the file
    fwrite(ram_abuse_score_array, sizeof(byte_t), array_size, fp);

    fclose(fp);

    return 0;
}

int destruct_ram_abuse_score_array() {
    free(ram_abuse_score_array);
    return 0;
}

const int ram_abuse_score(char *word, size_t length) {
    int score = 0;

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