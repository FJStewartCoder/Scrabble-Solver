#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_THREADS 10

// array that stores all of the letter scores for each ASCII (8bit) character
// majority of this array is 0s
// a lookup is faster than if statement
int letter_scores[256];


// function to get the score of any word
int get_score(char *word) {
	// the current character's index in the given word
	int char_idx = 0;
	// the score
	int score = 0;
	
	// iterate the word until the string null character
	while (word[char_idx] != '\0') {
		// since characters is just an int
		// get the numberer in the letter_scores array at the number that the character represents
		score += letter_scores[word[char_idx]];
		// increment character index
		char_idx++;
	}
	
	return score;
}


void make_score_array() {
	// array of all scores for the alphabet
	const int alphabet_score[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
	
	// the index of the current letter_score
	int cur_let = 0;

	// set the first 65 characters to 0
	for (; cur_let < 65; cur_let++) {
		letter_scores[cur_let] = 0;
	}

	// one set of the alphabet
	for (int i = 0; i < 26; i++) {
		letter_scores[cur_let + i] = alphabet_score[i];
	}

	// six 0s
	for (cur_let = 91; cur_let < 97; cur_let++) {
		letter_scores[cur_let] = 0;	
	}

	// another alphabet
	for (int i = 0; i < 26; i++) {
		letter_scores[cur_let + i] = alphabet_score[i];
	}

	// extra padding for rest of ascii characters of 0s
	for (cur_let = 123; cur_let < 256; cur_let++) {
		letter_scores[cur_let] = 0;	
	}
}


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


typedef struct test_args {
	char *word_list;
	unsigned int word_length;
	unsigned int word_start;
	unsigned int word_count;
} test_args_t;


DWORD WINAPI word_score_loop(void *args_) {
	test_args_t *args = (test_args_t*)args_;

	// pass a pointer to the start of the word for each word to the get_score function
	// pass pointer from idx 0 each word_length + 1
	// e.g a  a  0  a  a  0  a  a  0
	//     0, 1, 2, 3, 4, 5, 6, 7, 8
	// so pass 0, 3, 6 etc. Each word_len + 1 starting at 0

	for (unsigned int word = args->word_start; word < args->word_count; word++) {
		get_score(&args->word_list[word * (args->word_length + 1)]);
	}

	return 0;
}


void perform_test(char *word_list, unsigned int word_length, unsigned int word_count, unsigned int num_threads) {
	// list of threads at size of max threads
	HANDLE threads[MAX_THREADS];

	// dont allow more than max threads
	if (num_threads > MAX_THREADS) {
		return;
	}

	printf("Test started!\n");

	clock_t start = clock();

	// tests args which need to be cast as void*
	test_args_t args = {word_list, word_length, 0, 0};

	// iterate num threads time and create the thread
	// store the thread in the thread list
	for (int i = 0; i < num_threads; i++) {
		// change the args for each one
		args.word_start = args.word_count;
		args.word_count = word_count * ((float)(i + 1) / num_threads);

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

	printf("Time taken (%d %d letter words) -> %fs, %ld clock cycles\n", word_count, word_length, (double)ticks / CLOCKS_PER_SEC, ticks);
}


void create_words(char *words, unsigned int memory_len, unsigned int word_length, unsigned int word_count) {
	// define all letters and the number of letters
	const char letters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	int num_letters = sizeof(letters) / sizeof(letters[0]) - 1;	
	
	// the current random number
	int letter_number;

	// assign all characters in long array to a random character
	for (unsigned int letter = 0; letter < memory_len; letter++) {
		letter_number = rand() % num_letters;
		words[letter] = letters[letter_number]; 
	}
	
	// every word_length + 1 characters should be a null terminator to define the end of each string
	// e.g a  a  0  a  a  0  a  a  0
	//     0, 1, 2, 3, 4, 5, 6, 7, 8
	// so word len here is 2 so we need null terminators each word_len + 1 starting at the index word length
	for (unsigned int null_pos = word_length; null_pos < memory_len; null_pos += (word_length + 1)) {
		words[null_pos] = '\0';
	}
}


// the main speed test
int speed_test(unsigned int word_length, unsigned int num_words) {
	// ------------------------------------------------------------------------------------
	// allocate memory
	
	printf("Started allocating memory...\n");
	
	// need to do this because it will be one long array of characters but each word length
	// characters we will have a null character to end the string
	// so we need word length + 1 per word so 100 letters of word + 1 for null
	unsigned int word_size = num_words * (word_length + 1);

	// allocate some memory to store a long string of characters
	char *words = malloc(word_size * sizeof(char));
	
	// check if memory is valid
	if (words == NULL) {
		printf("Failed to allocate memory\n");
		return 1;
	}

	printf("Memory allocation complete!\n");

	// ------------------------------------------------------------------------------------
	// create random words
	
	printf("Started creating words...\n");

	create_words(words, word_size, word_length, num_words);

	printf("Word creation complete!\n");
	
	// ------------------------------------------------------------------------------------
	// testing 
	
	for (int i = 0; i < 3; i++) {
		perform_test(words, word_length, num_words, 2);
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
