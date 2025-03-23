#include "printf_checker.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Check for case-sensitive mismatches in the `printf` keyword
bool isPrintfCaseSensitiveError(const char *line) {
    const char *printfKeyword = "printf"; // Correct case-sensitive `printf` keyword
    const char *printfVariants[] = {
        "Printf", "PRINTF", "prinTf", "PrIntf", "pRintf"
    };

    // Check for exact match
    if (strstr(line, printfKeyword) != NULL) {
        return false; // Exact match found, no error
    }

    // Check for case-sensitive mismatches
    for (int i = 0; i < sizeof(printfVariants) / sizeof(printfVariants[0]); i++) {
        if (strstr(line, printfVariants[i]) != NULL) {
            return true; // Case-sensitive mismatch found
        }
    }

    return false; // No case-sensitive mismatch
}

// Check for unsupported `printf` variations
bool isUnsupportedPrintfVariation(const char *line) {
    const char *unsupportedVariations[] = {
        "printf_s", "sprintf", "fprintf", "snprintf", "vfprintf", "vprintf", "vsprintf"
    };

    for (int i = 0; i < sizeof(unsupportedVariations) / sizeof(unsupportedVariations[0]); i++) {
        if (strstr(line, unsupportedVariations[i]) != NULL) {
            return true; // Unsupported variation found
        }
    }

    return false; // No unsupported variation found
}

// Validate printf format string
int validatePrintfFormat(const char *line, int lineNumber) {
    const char *validSpecifiers = "diuoxXfFeEgGaAcspn%";
    const char *formatStart = strchr(line, '"');
    const char *formatEnd = strrchr(line, '"');

    if (!formatStart || !formatEnd) {
        printf("Error at line %d: Format string not enclosed in double quotes.\n", lineNumber);
        return 1; // Error code 1: Unbalanced quotes
    }

    // Check for invalid format specifiers
    for (const char *p = formatStart; p <= formatEnd; p++) {
        if (*p == '%') {
            p++; // Skip '%'
            if (!strchr(validSpecifiers, *p)) {
                printf("Error at line %d: Invalid format specifier '%%%c'.\n", lineNumber, *p);
                return 2; // Error code 2: Invalid format specifier
            }
        }
    }

    return 0; // No errors
}

// Check for invalid escape sequences
int checkEscapeSequences(const char *line, int lineNumber) {
    const char *validEscapes = "abfnrtv\\\'\"\?";
    const char *formatStart = strchr(line, '"');
    const char *formatEnd = strrchr(line, '"');

    if (!formatStart || !formatEnd) {
        printf("Error at line %d: Format string not enclosed in double quotes.\n", lineNumber);
        return 1; // Error code 1: Unbalanced quotes
    }

    // Check for invalid escape sequences
    for (const char *p = formatStart; p <= formatEnd; p++) {
        if (*p == '\\') {
            p++; // Skip '\'
            if (!strchr(validEscapes, *p)) {
                printf("Error at line %d: Invalid escape sequence '\\%c'.\n", lineNumber, *p);
                return 3; // Error code 3: Invalid escape sequence
            }
        }
    }

    return 0; // No errors
}

// Check for `printf` typos
bool isPrintfTypo(const char *line) {
    const char *printfVariants[] = {
        "Printf", "PRINTF", "prinTf", "PrIntf", "pRintf"
    };

    for (int i = 0; i < sizeof(printfVariants) / sizeof(printfVariants[0]); i++) {
        if (strstr(line, printfVariants[i]) != NULL) {
            return true; // Typo found
        }
    }
    return false; // No typo found
}

// Check for unbalanced quotes
int checkUnbalancedQuotes(const char *line, int lineNumber) {
    int quoteCount = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '"') quoteCount++;
    }

    if (quoteCount % 2 != 0) {
        printf("Error at line %d: Unbalanced quotes.\n", lineNumber);
        return 4; // Error code 4: Unbalanced quotes
    }

    return 0; // No errors
}