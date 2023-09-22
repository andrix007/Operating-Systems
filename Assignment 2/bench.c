//Copyright Andrei Bancila
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <getopt.h>

#define MAX_ITERATIONS 10000

void display_usage() {
    printf("Usage: bench [-w num_warmup_runs] [-d time_duration] command [args...]\n");
}

int main(int argc, char *argv[]) {
    int option;
    int num_warmup_runs = 0;
    int time_duration = 5;

    while ((option = getopt(argc, argv, "w:d:")) != -1) {
        switch (option) {
            case 'w':
                num_warmup_runs = atoi(optarg);
                break;
            case 'd':
                time_duration = atoi(optarg);
                break;
            default:
                display_usage();
                return EXIT_FAILURE;
        }
    }

    if (optind >= argc) {
        display_usage();
        return EXIT_FAILURE;
    }

    struct timespec start_timestamp, end_timestamp;
    double min_duration = -1, max_duration = -1, accumulated_time = 0;
    int successful_runs = 0, failed_runs = 0;

    for (int i = 0; i < num_warmup_runs; ++i) {
        if (fork() == 0) {
            execvp(argv[optind], &argv[optind]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }

    for (int i = 0; i < MAX_ITERATIONS && accumulated_time < time_duration; ++i) {
        if (clock_gettime(CLOCK_MONOTONIC, &start_timestamp) == -1) {
            perror("clock_gettime");
            return EXIT_FAILURE;
        }

        if (fork() == 0) {
            execvp(argv[optind], &argv[optind]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        if (wait(NULL) == -1) {
            perror("wait");
            failed_runs++;
        } else {
            if (clock_gettime(CLOCK_MONOTONIC, &end_timestamp) == -1) {
                perror("clock_gettime");
                return EXIT_FAILURE;
            }

            double elapsed_time = (end_timestamp.tv_sec - start_timestamp.tv_sec) +
                                  (end_timestamp.tv_nsec - start_timestamp.tv_nsec) / 1e9;

            if (min_duration == -1 || elapsed_time < min_duration)
                min_duration = elapsed_time;

            if (max_duration == -1 || elapsed_time > max_duration)
                max_duration = elapsed_time;

            accumulated_time += elapsed_time;
            successful_runs++;
        }
    }

    printf("Min:   %.6lf seconds\n", min_duration);
    printf("Avg:   %.6lf seconds\n", accumulated_time / successful_runs);
    printf("Max:   %.6lf seconds\n", max_duration);
    printf("Total: %.6lf seconds\n", accumulated_time);
    printf("Warmups: %d\n", num_warmup_runs);
    printf("Runs: %d\n", successful_runs);
    printf("Fails: %d\n", failed_runs);

    return EXIT_SUCCESS;
}
