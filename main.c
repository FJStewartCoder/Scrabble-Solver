#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "score.h"

#define MAX_THREADS 10


typedef struct test_args {
	// pointer to an array of characters
	char *word_list;
	// how much memory has been allocated
	unsigned int memory_len;

	// length of each word
	unsigned int word_length;
	// number of words
	unsigned int word_count;
	// where to start processing the words
	unsigned int word_start;

	// how many characters to move along in the window
	unsigned int window_shift;
} test_args_t;


// function to ensure that the get_score function is accurate
void test() {
	// £ and $ don't work
	// gets score of 2 alphabets (lower and upper) then some bonus chars which should be ignored
	int score = get_score("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!%^&*()_+-=`,./<>?[]{};'#:@~|\\");
	
	// prints score
	printf("%d\n", score);

	// one alphabet's sum is 87; so, we expect 87 * 2
	if (score != 87 * 2) {
		printf("Fail!\n");
	}
	else {
		printf("Success!\n");	
	}
}


DWORD WINAPI word_score_loop(void *args_) {
	test_args_t *args = (test_args_t*)args_;

	// where a particular word starts in the character list
	// scenario:
	// abcdefghijklmno
	// word 0 len 3 shift 2 = abc
	// word 1 = cde
	// word n = shift * n
	unsigned int start_pointer = args->word_start * args->window_shift;

	// gets the index of the end
	unsigned int end_pointer = start_pointer + ((args->word_count - 1) * args->window_shift);

	// iterate words from start to count incrementing in size of window shift
	for (unsigned int word = 0; word < args->word_count; word++) {
		// pass pointer to start of word and then pass length as well
		get_score_fast(&args->word_list[start_pointer + (word * args->window_shift)], args->word_length);
	}

	return 0;
}


void perform_test(test_args_t *args_, unsigned int num_threads) {
	// list of threads at size of max threads
	HANDLE threads[MAX_THREADS];

	// dont allow more than max threads
	if (num_threads > MAX_THREADS) {
		return;
	}

	printf("Test started!\n");

	clock_t start = clock();

	// tests args which need to be cast as void*
	test_args_t args = *args_;
	args.word_count = (float)args_->word_count / num_threads;

	// iterate num threads time and create the thread
	// store the thread in the thread list
	for (int i = 0; i < num_threads; i++) {
		// change the args for each one
		args.word_start = args.word_count * i;

		HANDLE thread = CreateThread(NULL, 0, word_score_loop, (void*)(&args), 0, NULL);

		// thread start message
		printf("Started thread %d - (%d, %d)\n", i + 1, args.word_start, args.word_count);

		threads[i] = thread;
	}

	// wait for threads to finish before stopping timer
	for (int i = 0; i < num_threads; i++) {
		WaitForSingleObject(threads[i], INFINITE);
	}

	clock_t end = clock();
	
	clock_t ticks = end - start;

	printf("Time taken (%d %d letter words) -> %fs, %ld clock cycles\n", args_->word_count, args_->word_length, (double)ticks / CLOCKS_PER_SEC, ticks);
}


void create_words(test_args_t *args) {
	// define all letters and the number of letters
	const char letters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	int num_letters = sizeof(letters) / sizeof(letters[0]) - 1;	
	
	// the current random number
	int letter_number;

	// assign all characters in long array to a random character
	for (unsigned int letter = 0; letter < args->memory_len; letter++) {
		letter_number = rand() % num_letters;
		// assign the characters in the word_list to a character
		args->word_list[letter] = letters[letter_number];
	}
}


int get_window_size(test_args_t *args) {
	return args->word_length + ((args->word_count - 1) * args->window_shift);
}


// the main speed test
int speed_test(unsigned int word_length, unsigned int num_words, unsigned int window_size) {
	if (window_size > word_length) {
		return 1;
	}

	test_args_t args = {NULL, 0, word_length, num_words, 0, window_size};

	// ------------------------------------------------------------------------------------
	// allocate memory
	
	printf("Started allocating memory...\n");
	
	// get overall size
	unsigned int word_size = get_window_size(&args);

	// allocate some memory to store a long string of characters
	char *words = malloc(word_size * sizeof(char));
	
	// check if memory is valid
	if (words == NULL) {
		printf("Failed to allocate memory\n");
		return 1;
	}

	// set the word list in the args
	args.word_list = words;
	args.memory_len = word_size;

	printf("Allocated memory of size %d characters!\n", word_size);
	printf("Memory allocation complete!\n");

	// ------------------------------------------------------------------------------------
	// create random words
	
	printf("Started creating words...\n");

	create_words(&args);

	printf("Word creation complete!\n");
	
	// ------------------------------------------------------------------------------------
	// testing 
	
	for (int i = 0; i < 3; i++) {
		perform_test(&args, 2);
	}

	// ------------------------------------------------------------------------------------
	// free arrays

	free(words);

	return 0;
}


int main(int argc, char **argv) {
	// make the score array
	make_score_array();

	// if there are more than 1 arg, get the score for each arg and print it out
	if (argc > 1) {
		int score;

		for (int i = 1; i < argc; i++) {
			score = get_score(argv[i]);
			printf("%s -> %d\n", argv[i], score);
		}
	}
	// if no args, perform the tests
	else {
		test();
		speed_test(1000, 1000 * 1000, 1);
	}

	return 0;
}
