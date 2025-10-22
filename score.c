#include "score.h"
#include <stdio.h>

// array that stores all of the letter scores for each ASCII (8bit) character
// majority of this array is 0s
// a lookup is faster than if statement
int letter_scores[256];


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

// -------------------------------------------------------------------------------------------------------------------------

// function to get the score of any word
int get_score_fast(char *word, unsigned int word_length) {
	// the score
	int score = 0;
	
	// iterate the word until the string null character
	for (unsigned int i = 0; i < word_length; i++) {
		// since characters is just an int
		// get the numberer in the letter_scores array at the number that the character represents
		score += letter_scores[word[i]];
	}
	
	return score;
}


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