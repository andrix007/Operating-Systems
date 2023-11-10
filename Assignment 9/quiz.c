#include <stdio.h>
#include <stdlib.h>
#include "quiz.h"

int main() {
    quiz_t quiz;
    quiz.n = 0;
    quiz.score = 0;
    quiz.max = 0;

    while (1) {
        char *json_data = fetch("https://opentdb.com/api.php?amount=1&category=18&difficulty=easy&type=multiple");
        if (json_data == NULL) {
            fprintf(stderr, "Failed to fetch data\n");
            continue;
        }

        if (parse(&quiz, json_data) == -1) {
            fprintf(stderr, "Failed to parse JSON\n");
            free(json_data);
            continue;
        }

        free(json_data);

        if (play(&quiz) == -1) {
            break; // User ended input or error
        }

        quiz.n++;
    }

    printf("Thanks for playing today.\n");
    printf("Your final score is %u/%u points.\n", quiz.score, quiz.max);
    return 0;
}
