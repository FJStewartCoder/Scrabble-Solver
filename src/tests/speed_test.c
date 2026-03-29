#include "speed_test.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define WORD_LENGTH 1000 * 1000
#define WORD_COUNT 1000


typedef struct {
	// pointer to an array of characters
    char *list;

	// how much memory has been allocated
    size_t mem_size;

	// length of each word
    size_t word_length;
	// number of words
    size_t word_count;
    // number of characters between each word (because they are overlapping)
    size_t word_offset;

    // int to show if valid or not
    bool is_valid;
} word_list_t;


size_t get_required_mem_size(word_list_t *w_list) {
	return w_list->word_length + ((w_list->word_count - 1) * w_list->word_offset);
}

void create_words(word_list_t *w_list) {
    // allocate mem_size about of memory
    w_list->list = malloc(w_list->mem_size);

    const int is_valid = w_list->list != NULL;

    // if not valid, set error state and return
    if (!is_valid) {
        w_list->is_valid = false;
        return;
    }

	// define all letters and the number of letters
	const char letters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	const int num_letters = sizeof(letters) / sizeof(letters[0]) - 1;	
	
	// the current random number
	int letter_number;

	// assign all characters in long array to a random character
	for (unsigned int letter = 0; letter < w_list->mem_size; letter++) {
		letter_number = rand() % num_letters;
		// assign the characters in the word_list to a character
		w_list->list[letter] = letters[letter_number];
	}
}

word_list_t init_word_list() {
    word_list_t res;

    // default to NULL
    res.list = NULL;

    // set validity to true
    res.is_valid = true;

    // set parameters about the words
    res.word_count = WORD_COUNT;
    res.word_length = WORD_LENGTH;
    res.word_offset = 10;

    // get the memory size needed
    res.mem_size = get_required_mem_size(&res);

    // create all of the words
    create_words(&res);

    return res;
}

void del_word_list(word_list_t *w_list) {
    if (w_list->is_valid) {
        free(w_list->list);
    }
}

// get the word at any given index
char *get_word(word_list_t *w_list, size_t idx) {
    const bool invalid_index = idx < 0 || idx >= w_list->word_count;

    if (invalid_index) { return NULL; }
    
    // (number of offsets * idx) + the pointer to the first word
    char *word_ptr = w_list->list + (idx * w_list->word_offset);

    return word_ptr;
}


const double speed_test_algorithm(score_func_t *score_func) {
    srand(time(NULL));

    word_list_t words = init_word_list();

    int score;

    time_t start = clock();

    for (int i = 0; i < words.word_count; i++) {
        char *word = get_word(&words, i);

        score = score_func(word, words.word_length);
    }

    time_t end = clock();

    del_word_list(&words);

    return (double)(end - start) / CLOCKS_PER_SEC;
}