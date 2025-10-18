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
	
	// need to do this because it will be one long array of characters but each word length
	// characters we will have a null character to end the string
	// so we need word length + 1 per word so 100 letters of word + 1 for null
	unsigned int word_size = num_words * (word_length + 1);	
	char *words = malloc(word_size * sizeof(char));
	
	if (words == NULL) {
		printf("Failed to allocate memory\n");
		return 1;
	}

	printf("Memory allocation complete!\n");

	// ------------------------------------------------------------------------------------
	// create random words
	
	printf("Started creating words...\n");

	const char letters[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	int num_letters = sizeof(letters) / sizeof(letters[0]) - 1;	
	
	int letter_number;

	for (unsigned int letter = 0; letter < word_size; letter++) {
		letter_number = rand() % num_letters;
		words[letter] = letters[letter_number]; 
	}
	
	for (unsigned int null_pos = word_length; null_pos < word_size; null_pos += (word_length + 1)) {
		words[null_pos] = '\0';
	}

	printf("Word creation complete!\n");
	
	// ------------------------------------------------------------------------------------
	// testing 
	
	printf("Test started!\n");

	clock_t start = clock();

	for (unsigned int word = 0; word < num_words; word++) {
		get_score(&words[word * (word_length + 1)]);
	}

	clock_t end = clock();
	
	clock_t ticks = end - start;

	printf("Time taken (%d %d letter words) -> %fs, %ld clock cycles\n", num_words, word_length, (double)ticks / CLOCKS_PER_SEC, ticks);

	// ------------------------------------------------------------------------------------
	// free arrays

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
		speed_test(100, 100 * 1000);
	}

	return 0;
}
