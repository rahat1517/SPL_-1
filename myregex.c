#include "myregex.h"

// Main matching function
int match(const char *text, const char *pattern) {
    if (pattern[0] == '^') {
        return match_here(text, pattern + 1);
    }
    do {
        if (match_here(text, pattern)) {
            return 1;
        }
    } while (*text++ != '\0');
    return 0;
}

// Recursive matching function
int match_here(const char *text, const char *pattern) {
    if (pattern[0] == '\0') return 1;

    if (pattern[1] == '*') {
        return match_star(pattern[0], text, pattern + 2);
    }

    if (pattern[0] == '$' && pattern[1] == '\0') {
        return *text == '\0';
    }

    if (*text != '\0' && (pattern[0] == '.' || pattern[0] == *text)) {
        return match_here(text + 1, pattern + 1);
    }

    return 0;
}

// Function to handle '*' operator
int match_star(char c, const char *text, const char *pattern) {
    do {
        if (match_here(text, pattern)) {
            return 1;
        }
    } while (*text != '\0' && (*text++ == c || c == '.'));

    return 0;
}
