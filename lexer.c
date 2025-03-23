#include "lexer.h"
#include <string.h>

// Remove comments from a line
void removeComments(char *line) {
    char *commentStart;

    // Remove single-line comments (//)
    commentStart = strstr(line, "//");
    if (commentStart) {
        *commentStart = '\0';
    }

    // Remove multi-line comments (/* ... */)
    commentStart = strstr(line, "/*");
    if (commentStart) {
        char *commentEnd = strstr(commentStart, "*/");
        if (commentEnd) {
            memmove(commentStart, commentEnd + 2, strlen(commentEnd + 2) + 1);
        } else {
            // Handle multi-line comments spanning multiple lines
            *commentStart = '\0';
        }
    }
}