#ifndef QUIZ_H
#define QUIZ_H

#define MAX_CHOICES 4

// Structure to hold quiz data
typedef struct {
    unsigned int n;         // current question number (starting at 1)
    unsigned int score;     // current total score
    unsigned int max;       // possible max score
    char *question;         // next question (dynamically allocated)
    char *answer;           // correct answer (dynamically allocated)
    char *choices[MAX_CHOICES]; // answer choices (dynamically allocated)
} quiz_t;

// Function prototypes
extern char* fetch(char *url);
extern int parse(quiz_t *quiz, char *msg);
extern int play(quiz_t *quiz);

#endif // QUIZ_H
