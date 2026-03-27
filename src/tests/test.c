#include "test.h"

#include "speed_test.h"
#include "functionality_test.h"


#include "algorithms/base.h"

#include "algorithms/big.h"
#include "algorithms/ram_abuse.h"
#include "algorithms/shortptr.h"
#include "algorithms/simple.h"

// -------------------------------------------------------------------------------------------

#include <stdio.h>

// -------------------------------------------------------------------------------------------

typedef struct {
    char *name;

    int (*create)();
    int (*destruct)();

    int (*score)(char*);
} algorithm_t;


const algorithm_t algs[] = {
    {"Default", create_score_array, NULL, get_word_score},
    {"Big", create_big_score_array, NULL, get_big_word_score},
    {"Different", create_different_score_array, NULL, get_different_word_score},
    {"Ram Abuse", create_ram_abuse_score_array, destruct_ram_abuse_score_array, get_ram_abuse_word_score}
};

const size_t num_algs = sizeof(algs) / sizeof(algs[0]);

// -------------------------------------------------------------------------------------------

int test() {
    for (size_t i = 0; i < num_algs; i++) {
        algorithm_t *alg = &algs[i];

        char *alg_name;

        if (alg->name == NULL) { alg_name = "Unknown Algorithm"; }
        else { alg_name = alg->name; }

        printf("\nTesting %s\n", alg_name);

        // check if creation alg exists and run it if it does
        if (alg->create == NULL) {
            printf("%s does not have a creation function\n", alg_name);
            continue;
        }

        // if creation fails, also continue
        if (alg->create() != 0) {
            printf("%s had an error whilst initialising\n", alg_name);
            continue;
        }

        printf("%s successfully initialised\n", alg_name);

        // if a scoring alg exists, test it
        if (alg->score != NULL) {
            printf("%s has a scoring function\n", alg_name);

            test_algorithm(alg->score);

            double time_taken = speed_test_algorithm(alg->score);
            printf("%s took %lfs to score the test word\n", alg_name, time_taken);
        }
        else {
            printf("%s does not have a scoring function\n", alg_name);
        }

        // if we can destruct, do it
        if (alg->destruct == NULL) { 
            printf("%s does not have a destruction function\n", alg_name);
            continue;
        }

        alg->destruct();

        printf("%s has been destructed\n", alg_name);
    }
}