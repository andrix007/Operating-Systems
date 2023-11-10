#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "quiz.h"

int play(quiz_t *quiz) {
    quiz->max += 8; // Increment max possible score

    printf("%s\n", quiz->question);
    for (int i = 0; i < 4; i++) {
        printf("[%c] %s\n", 'a' + i, quiz->choices[i]);
    }

    char input[10];
    printf("(8pt) > ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1; // EOF or error
    }

    input[0] = tolower(input[0]); // Convert to lowercase
    if (input[0] < 'a' || input[0] > 'd') {
        printf("Invalid choice.\n");
        return 0;
    }

    int chosen_index = input[0] - 'a'; // Calculate index of chosen answer
    if (strcmp(quiz->choices[chosen_index], quiz->answer) == 0) {
        printf("Congratulations, answer [%c] is correct.\n", input[0]);
        quiz->score += 8;
    } else {
        printf("Sorry, answer [%c] is incorrect.\n", input[0]);
    }

    printf("Your current score is %u/%u points.\n", quiz->score, quiz->max);
    return 0;
}
