#include "parser.h"
#include "symbol_table.h"
#include <string.h>
#include <ctype.h>


// Custom function for case-insensitive string search
const char *strcasestr_custom(const char *haystack, const char *needle) {
    if (!*needle) return haystack; // If needle is empty, return haystack

    for (; *haystack; haystack++) {
        if (tolower(*haystack) == tolower(*needle)) {
            const char *h = haystack + 1;
            const char *n = needle + 1;
            while (*h && *n && tolower(*h) == tolower(*n)) {
                h++;
                n++;
            }
            if (!*n) return haystack; // If needle is fully matched, return haystack
        }
    }
    return NULL; // If needle is not found, return NULL
}

// Function to check case sensitivity for printf and scanf
void checkCaseSensitivity(const char *line, int lineNumber) {
    const char *printfPos = strcasestr_custom(line, "printf"); // Case-insensitive search
    const char *scanfPos = strcasestr_custom(line, "scanf");   // Case-insensitive search

    if (printfPos && strncmp(printfPos, "printf", 6) != 0) {
        printf("Error at line %d: 'printf' is case-sensitive. Found: '%.6s'. Use 'printf' in lowercase.\n", lineNumber, printfPos);
    }

    if (scanfPos && strncmp(scanfPos, "scanf", 5) != 0) {
        printf("Error at line %d: 'scanf' is case-sensitive. Found: '%.5s'. Use 'scanf' in lowercase.\n", lineNumber, scanfPos);
    }
}

// Function to check #include directives
void checkIncludeDirective(const char *line, int lineNumber) {
    const char *includePos = strstr(line, "#include");
    if (includePos) {
        const char *start = strchr(includePos, '<');
        const char *end = strchr(includePos, '>');

        if (!start || !end || start > end) {
            start = strchr(includePos, '"');
            end = strchr(includePos + 1, '"');
        }

        if (!start || !end || start > end) {
            printf("Error at line %d: Invalid #include directive.\n", lineNumber);
            return;
        }

        char headerName[NAME_LENGTH];
        strncpy(headerName, start + 1, end - start - 1);
        headerName[end - start - 1] = '\0';

        // Check for common header files
        if (strcmp(headerName, "stdio.h") != 0 &&
            strcmp(headerName, "stdlib.h") != 0 &&
            strcmp(headerName, "string.h") != 0) {
            printf("Warning at line %d: Unrecognized header file '%s'.\n", lineNumber, headerName);
        }
    }
}


// Function to check printf errors
void checkPrintfErrors(const char *line, int lineNumber) {
    const char *printfStart = strstr(line, "printf");
    if (printfStart) {
        const char *formatStart = strchr(printfStart, '"');
        const char *formatEnd = strrchr(printfStart, '"');

        // Check if format string is missing
        if (!formatStart || !formatEnd || formatStart == formatEnd) {
            printf("Error at line %d: Missing or invalid format string in printf.\n", lineNumber);
            return;
        }

        // Check for missing arguments
        const char *comma = strchr(formatEnd, ',');
        if (!comma) {
            printf("Warning at line %d: No arguments provided for format specifiers in printf.\n", lineNumber);
        }

        // Check for mismatched format specifiers and arguments
        int formatSpecifierCount = 0;
        for (const char *p = formatStart; p <= formatEnd; p++) {
            if (*p == '%' && *(p + 1) != '%') {
                formatSpecifierCount++;
            }
        }

        int argumentCount = 0;
        const char *arg = comma;
        while (arg && *arg != ')') {
            if (*arg == ',') argumentCount++;
            arg++;
        }

        if (formatSpecifierCount != argumentCount) {
            printf("Error at line %d: Mismatch between format specifiers (%d) and arguments (%d) in printf.\n",
                   lineNumber, formatSpecifierCount, argumentCount);
        }
    }
}

// Function to check for unused variables
void checkUnusedVariables(const char *line, int lineNumber) {
    const char *varDecl = strstr(line, "int ");
    if (varDecl) {
        char varName[NAME_LENGTH];
        sscanf(varDecl, "int %[^;=]", varName);

        if (strstr(line, varName) == varDecl) {
            printf("Warning at line %d: Variable '%s' is declared but not used.\n", lineNumber, varName);
        }
    }
}

// Function to check for missing return statement in main
void checkReturnStatement(const char *line, int lineNumber) {
    const char *mainStart = strstr(line, "int main");
    if (mainStart) {
        const char *returnPos = strstr(line, "return");
        if (!returnPos) {
            printf("Warning at line %d: 'main' function is missing a return statement.\n", lineNumber);
        }
    }
}

// Parser Function Implementation
void parse(char *line, int lineNumber) {
    char *token;
    TokenType tokenType = lexer(line, &token);

    switch (tokenType) {
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_CHAR:
        case TOKEN_VOID:
            printf("Type: %s\n", token);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", token);
            break;
        case TOKEN_NUMBER:
            printf("Number: %s\n", token);
            break;
        case TOKEN_PRINTF:
            printf("Function: printf\n");
            checkCaseSensitivity(line, lineNumber); // Check for case sensitivity
            checkPrintfErrors(line, lineNumber);    // Check for printf errors
            break;
        case TOKEN_SCANF:
            printf("Function: scanf\n");
            checkCaseSensitivity(line, lineNumber); // Check for case sensitivity
            break;
        case TOKEN_INCLUDE:
            printf("Directive: #include\n");
            checkIncludeDirective(line, lineNumber); // Check #include directive
            break;
        default:
            printf("Unknown Token: %s\n", token);
            break;
    }

    checkUnusedVariables(line, lineNumber); // Check for unused variables
    checkReturnStatement(line, lineNumber); // Check for missing return statement

    free(token);
}