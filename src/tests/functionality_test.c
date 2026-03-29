#include "functionality_test.h"

#include <stdio.h>
#include <string.h>


typedef struct {
    char *word;
    size_t length;
    size_t expected;
} test_case_t;


const int test_algorithm(score_func_t *score_func) {
    int success = 1;

    int score;

    test_case_t tests[] = {
        {"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!%^&*()_+-=`,./<>?[]{};'#:@~|\\", 0, 174},
        {"ABC", 3, 7},
        {"ABCD", 4, 9},
        {"ABCDE", 5, 10},
        {"ABCDEF", 6, 14},
        {"ABCDEFG", 7, 16},
        {"ABCDEFGHIJKLM", 7, 16}
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        test_case_t *test = &tests[i];
        
        // calculate the length of the string if it does not have a length
        if ( test->length == 0 ) {
            test->length = strlen(test->word);
        }

        // get the score
        score = score_func(test->word, test->length);

        // check for test success
        const int test_successful = score == test->expected;

        if (test_successful) {
            printf("Successfully scored word %s (%d) as %lu\n", test->word, score, test->expected);
        }
        else {
            printf("Unsuccessfully scored word %s (%d) as %lu\n", test->word, score, test->expected);
            success = 0;
        }
    }

    return success;
}