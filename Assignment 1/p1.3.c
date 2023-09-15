// Andrei Bancila
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

void print_env() {
    char **current = environ;
    while (*current) {
        puts(*current);
        current++;
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *name_to_remove = NULL;
    int verbose = 0;

    while ((opt = getopt(argc, argv, "vu:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'u':
                name_to_remove = optarg;
                if (verbose) {
                    fprintf(stderr, "Removing %s from environment\n", name_to_remove);
                }
                unsetenv(name_to_remove);
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [-u name] [name=value]... [command [args]...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Process name=value pairs
    for (; optind < argc; optind++) {
        char *arg = argv[optind];
        if (strchr(arg, '=') != NULL) {
            if (verbose) {
                fprintf(stderr, "Adding %s to environment\n", arg);
            }
            putenv(arg);
        } else {
            break;
        }
    }

    // If no command is specified, print environment
    if (optind == argc) {
        print_env();
        return 0;
    }

    // Execute remaining command
    char **cmd = argv + optind;
    if (verbose) {
        fprintf(stderr, "Executing command %s\n", cmd[0]);
    }
    execvp(cmd[0], cmd);

    // execvp only returns if there was an error
    perror("execvp");
    return EXIT_FAILURE;
}
