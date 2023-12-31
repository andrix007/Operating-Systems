 // Andrei Bancila
 #define _DEFAULT_SOURCE

 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>

 /*
 * Count the number of characters in string that are an element of the
 * character set delim. Returns 0 if none of the characters in string
 * is in the character set delim or the string is empty.
 */

 int f[100];
 int k = 2;

 size_t strcnt(const char *string, const char *delim)
 {
    size_t cnt = 0;

    for (const char *s = string; *s; s++) {
        for (const char *d = delim; *d; d++) {
            if (*s == *d) {
                cnt++;
                break;
            }
        }
    }
    return cnt;
 }

 /*
 * Split the string whenever a character appears that is in the
 * character set delim. Return a NULL terminated vector of pointers to
 * the sub-strings.
 */

 char ** strsplit(char **string, const char *delim)
 {
    char *token;
    size_t cnt = strcnt(*string, delim);

    char **splitv = calloc(cnt + 2, sizeof(char));
    if (splitv) {
        int i;
        for (i = 0; (token = strsep(string, delim)); i++) {
            splitv[i] = token;
            f[k]++;
        }
    }
    k++;
    return splitv;
 }

 int main(int argc, char *argv[])
 {  
    for(int i = 2 ; i < argc ; i++) {
        f[i] = 0;
    }
    for (int i = 2; i < argc; i++) {
        char **splitv = strsplit(&argv[i], " ");
        if (splitv) {
            for (int j = 0 ; j < f[i] ; j++) {
                (void) puts(splitv[j]);
            }
            (void) free(splitv);
        }
    }

    return EXIT_SUCCESS;
 }