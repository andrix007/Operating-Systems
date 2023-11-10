#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

char* fetch(char *url) {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];
    size_t capacity = 1024;
    size_t size = 0;

    char *response = malloc(capacity);
    if (!response) {
        perror("malloc");
        return NULL;
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        free(response);
        return NULL;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        free(response);
        return NULL;
    }

    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("curl", "curl", "-s", url, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]);
        while (1) {
            ssize_t count = read(pipefd[0], buffer, sizeof(buffer));
            if (count == -1) {
                if (errno == EINTR) {
                    continue;
                } else {
                    perror("read");
                    break;
                }
            } else if (count == 0) {
                break;
            } else {
                if (size + count > capacity) {
                    capacity *= 2;
                    response = realloc(response, capacity);
                    if (!response) {
                        perror("realloc");
                        break;
                    }
                }
                memcpy(response + size, buffer, count);
                size += count;
            }
        }
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }

    return response;
}
