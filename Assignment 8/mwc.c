#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

void count(const char *data, size_t len, size_t *lines, size_t *words, size_t *bytes) {
    int in_word = 0;
    *lines = *words = *bytes = 0;
    for(size_t i = 0; i < len; i++) {
        (*bytes)++;
        if(data[i] == '\n') (*lines)++;
        if(isspace(data[i])) in_word = 0;
        else if(!in_word) {
            in_word = 1;
            (*words)++;
        }
    }
}

void process_file(const char *filename) {
    size_t lines, words, bytes;
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        perror("open");
        return;
    }
    
    struct stat st;
    if(fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return;
    }
    
    char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    count(data, st.st_size, &lines, &words, &bytes);
    printf("%zu %zu %zu %s\n", lines, words, bytes, filename);
    
    munmap(data, st.st_size);
    close(fd);
}

void process_stdin() {
    size_t lines, words, bytes;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    lines = words = bytes = 0;
    while((nread = getline(&line, &len, stdin)) != -1) {
        count(line, nread, &lines, &words, &bytes);
    }
    free(line);
    printf("%zu %zu %zu\n", lines, words, bytes);
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        process_stdin();
    } else {
        for(int i = 1; i < argc; i++) {
            process_file(argv[i]);
        }
    }
    return 0;
}
