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

	// iterate words from start to count
	for (unsigned int word = args->word_start; word < args->word_count; word++) {
		// pass pointer to start of word and then pass length as well
		get_score_fast(&args->word_list[word * args->word_length], args->word_length);
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
	test_args_t args = {args_->word_list, 0, args_->word_length, 0, 0, 0};

	// iterate num threads time and create the thread
	// store the thread in the thread list
	for (int i = 0; i < num_threads; i++) {
		// change the args for each one
		args.word_start = args.word_count;
		args.word_count = args_->word_count * ((float)(i + 1) / num_threads);

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

	int memory_len = args->word_count * args->word_length;

	// assign all characters in long array to a random character
	for (unsigned int letter = 0; letter < memory_len; letter++) {
		letter_number = rand() % num_letters;
		// assign the characters in the word_list to a character
		args->word_list[letter] = letters[letter_number];
	}
}


// the main speed test
int speed_test(unsigned int word_length, unsigned int num_words) {
	test_args_t args = {NULL, 0, word_length, num_words, 0, 1};

	// ------------------------------------------------------------------------------------
	// allocate memory
	
	printf("Started allocating memory...\n");
	
	// get overall size
	unsigned int word_size = num_words * word_length;

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
		speed_test(1000, 1000 * 1000);
	}

	return 0;
}
