#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"

int parse(quiz_t *quiz, char *msg) {
    struct json_object *parsed_json;
    struct json_object *results;
    struct json_object *result;
    struct json_object *question;
    struct json_object *correct_answer;
    struct json_object *incorrect_answers;
    size_t n_incorrect_answers;

    parsed_json = json_tokener_parse(msg);
    if (parsed_json == NULL) {
        return -1;
    }

    json_object_object_get_ex(parsed_json, "results", &results);
    result = json_object_array_get_idx(results, 0);

    json_object_object_get_ex(result, "question", &question);
    json_object_object_get_ex(result, "correct_answer", &correct_answer);
    json_object_object_get_ex(result, "incorrect_answers", &incorrect_answers);

    n_incorrect_answers = json_object_array_length(incorrect_answers);

    quiz->question = strdup(json_object_get_string(question));
    quiz->answer = strdup(json_object_get_string(correct_answer));

    for (size_t i = 0; i < n_incorrect_answers; i++) {
        quiz->choices[i] = strdup(json_object_get_string(json_object_array_get_idx(incorrect_answers, i)));
    }
    quiz->choices[n_incorrect_answers] = strdup(json_object_get_string(correct_answer));

    json_object_put(parsed_json);

    return 0;
}
