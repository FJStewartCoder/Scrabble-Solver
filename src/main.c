#include <stdio.h>
#include <stdlib.h>


#include "algorithms/base.h"

#include "algorithms/big.h"
#include "algorithms/ram_abuse.h"
#include "algorithms/shortptr.h"
#include "algorithms/simple.h"


#include "tests/test.h"


int main(int argc, char **argv) {
	// make the score arrays

	// create_score_array();
	// create_different_score_array();
	create_big_score_array();

	// if there are more than 1 arg, get the score for each arg and print it out
	if (argc > 1) {
		int score;

		for (int i = 1; i < argc; i++) {
			score = get_big_word_score(argv[i]);
			printf("%s -> %d\n", argv[i], score);
		}
	}
	// if no args, perform the tests
	else {
		test();
	}

	return 0;
}
