import random as r
from time import perf_counter as pc


a= [1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10]
b = [0 for _ in range(65)]
letter_scores = b + a + [0 for _ in range(6)] + a + b + b

def get_score(word):
    score = 0

    for chr in word:
        score += letter_scores[ord(chr)]
	
    return score


def test():
    score = get_score("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!%^&*()_+-=`,./<>?[]{};'#:@~|\\")
    print(score)

    if (score != 87 * 2):
        print("Fail!")
    else:
        print("Success!")


def speed_test(word_length, num_words):
    # ------------------------------------------------------------------------------------
    # create random words

    print("Started creating words...")

    start = pc()

    letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
    word_list = ["".join(r.choices(letters, k=word_length)) for _ in range(num_words)]

    end = pc()

    print(f"Generating words complete in {end - start}s!")

    # ------------------------------------------------------------------------------------
    # testing 

    print("Test started!")

    start = pc()

    for word in word_list:
        get_score(word)

    end = pc()

    time = end - start

    print(f"Time taken ({num_words:,} {word_length:,} letter words) -> {time}s")


def main():
    test()
    speed_test(100, 100 * 1000)


if __name__ == '__main__':
    main()
