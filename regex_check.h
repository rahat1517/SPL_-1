#ifndef REGEX_CHECK_H
#define REGEX_CHECK_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function to check if the #include statement is valid (case-sensitive)
bool isValidInclude(const char *line) {
    // #include<stdio.h> বা #include <stdio.h> এর মতো স্টেটমেন্ট চেক করবে
    
    if (line == NULL) return false;

    // প্রথমে #include চেক করা (case-sensitive)
    if (strncmp(line, "#include", 8) != 0) {
        return false; // #include নয়
    }

    // তারপর সাদা স্পেস (space) চেক করা
    const char *start = line + 8;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // যদি < বা " না থাকে, তবে ভুল
    if (*start != '<' && *start != '"') {
        return false;
    }

    // '>' বা '"' এর পর কোন লেখা বা অক্ষর আছে কিনা চেক করা
    start++;
    while (*start != '>' && *start != '"' && *start != '\0') {
        start++;
    }

    // '>' বা '"' এর পর কিছু থাকতে হবে, অথবা স্ট্রিং শেষ হয়ে যাবে
    return (*start == '>' || *start == '"') && *(start + 1) == '\0';
}

// Function to check if the #include statement is invalid (does not match valid format)
bool isInvalidInclude(const char *line) {
    // #include<stdio.h> বা #include <stdio.h> ছাড়া অন্য কিছু চেক করবে
    return !isValidInclude(line); // যদি valid না হয় তবে invalid
}

#endif
