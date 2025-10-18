#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int letter_scores[256];

int get_score(char *word) {
	int cur_char = 0;
	int cur_score = 0;
	
	while (word[cur_char] != '\0') {
		cur_score += letter_scores[word[cur_char]];
		cur_char++;
	}
	
	return cur_score;
}

void make_score_array() {
	const int alphabet_score[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
	
	int cur_let = 0;

	for (; cur_let < 65; cur_let++) {
		letter_scores[cur_let] = 0;
	}

	for (int i = 0; i < 26; i++) {
		letter_scores[cur_let + i] = alphabet_score[i];
	}

	for (cur_let = 91; cur_let < 97; cur_let++) {
		letter_scores[cur_let] = 0;	
	}

	for (int i = 0; i < 26; i++) {
		letter_scores[cur_let + i] = alphabet_score[i];
	}

	for (cur_let = 123; cur_let < 256; cur_let++) {
		letter_scores[cur_let] = 0;	
	}
}

void test() {
	// £ and $ don't work
	int score = get_score("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!%^&*()_+-=`,./<>?[]{};'#:@~|\\");
	
	printf("%d\n", score);

	if (score != 87 * 2) {
		printf("Fail!\n");
	}
	else {
		printf("Success!\n");	
	}
}

int speed_test(unsigned int word_length, unsigned int num_words) {
	// ------------------------------------------------------------------------------------
	// allocate memory
	
	printf("Started allocating memory...\n");

	char **words = malloc(num_words * sizeof(char*));
	
	if (words == NULL) {
		printf("Failed to allocate memory\n");
		return 1;
	}

	for (int i = 0; i < num_words; i++) {
		words[i] = malloc(word_length * sizeof(char));

		if (words[i] != NULL) {
			continue;
		}
		
		printf("Failed to allocate memory! Freeing memory.\n");
		for (int j = 0; j < i; j++) {
			free(words[j]);
		}
		free(words);

		return 1;
	}
	
	printf("Memory allocation complete!\n");

	// ------------------------------------------------------------------------------------
	// create random words
	

	printf("Word creation complete!\n");

	// ------------------------------------------------------------------------------------
	// testing 
	
	printf("Test started!\n");

	time_t start = clock();

	for (unsigned int word = 0; word < num_words; word++) {
		get_score(words[word]);
	}

	time_t end = clock();
	
	time_t ticks = end - start;

	printf("Time taken -> %ld clock cycles\n", ticks);

	// ------------------------------------------------------------------------------------
	// free arrays

	for (int i = 0; i < num_words; i++) {
		free(words[i]);
	}
		
	free(words);

	return 0;
}

int main(int argc, char **argv) {
	make_score_array();

	if (argc > 1) {
		int score;

		for (int i = 1; i < argc; i++) {
			score = get_score(argv[i]);
			printf("%s -> %d\n", argv[i], score);
		}
	}
	else {
		test();
		speed_test(100, 10000000);
	}

	return 0;
}
