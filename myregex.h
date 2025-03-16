#ifndef MYREGEX_H
#define MYREGEX_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATTERN_LENGTH 100
#define MAX_TEXT_LENGTH 1000

// Function Prototypes
int match(const char *text, const char *pattern);
int match_here(const char *text, const char *pattern);
int match_star(char c, const char *text, const char *pattern);

#endif // MYREGEX_H
