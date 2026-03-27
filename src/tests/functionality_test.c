#include "functionality_test.h"

#include <stdio.h>


typedef struct {
    char *word;
    size_t expected;
} test_case_t;


const int test_algorithm(const int (*score_func)(char *)) {
    int success = 1;

    int score;

    test_case_t tests[] = {
        {"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!%^&*()_+-=`,./<>?[]{};'#:@~|\\", 174},
        {"ABC", 7},
        {"ABCD", 9},
        {"ABCDE", 10},
        {"ABCDEF", 14},
        {"ABCDEFG", 16}
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        test_case_t *test = &tests[i];

        // get the score
        score = score_func(test->word);

        // check for test success
        const int test_successful = score == test->expected;

        if (test_successful) {
            printf("Successfully scored word %s\n", test->word);
        }
        else {
            printf("Unsuccessfully scored word %s as %d instead of %d\n", test->word, score, test->expected);
            success = 0;
        }
    }

    return success;
}