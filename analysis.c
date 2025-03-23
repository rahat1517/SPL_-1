#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "analysis.h"

// Check for syntax errors
void checkSyntaxErrors(const char *line, int lineNumber) {
    static int braceCount = 0; // Use static to track braces across multiple lines
    int quoteCount = 0;
    int semicolonCount = 0;

    // Skip empty lines
    if (strlen(line) == 0 || line[0] == '\n') {
        return;
    }

    // Check for braces and quotes
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '{') braceCount++;
        if (line[i] == '}') braceCount--;
        if (line[i] == '"') quoteCount++;
        if (line[i] == ';') semicolonCount++;
    }

    // Check for unmatched quotes
    if (quoteCount % 2 != 0) {
        printf("Error at line %d: Unmatched quotes.\n", lineNumber);
    }

    // Check for missing semicolons
    if (semicolonCount == 0) {
        // Exclude lines that do not require semicolons
        if (strstr(line, "#") != NULL) {
            // Preprocessor directives (e.g., #include)
            return;
        }
        if (strstr(line, "{") != NULL || strstr(line, "}") != NULL) {
            // Lines with braces (e.g., function definitions, blocks)
            return;
        }
        if (strstr(line, "if") != NULL || strstr(line, "for") != NULL || strstr(line, "while") != NULL) {
            // Control structures
            return;
        }
        if (strstr(line, "return") != NULL) {
            // Return statements
            return;
        }
        if (strstr(line, "int main()") != NULL || strstr(line, "void") != NULL) {
            // Function definitions
            return;
        }

        // If none of the above, flag as missing semicolon
        printf("Error at line %d: Missing semicolon.\n", lineNumber);
    }

    // Check for unbalanced braces at the end of the file
    if (braceCount != 0 && feof(stdin)) {
        printf("Error: Unbalanced braces in the file.\n");
    }
}
// Count standard and user-defined functions
void countFunctions(const char *line, int *stdFuncCount, int *userFuncCount) {
    const char *stdFuncs[] = {"printf", "scanf", "gets", "puts", "fscanf", "malloc", "free", "strlen"};
    const char *userFuncs[] = {"main", "addSymbol", "lookupSymbol", "printSymbolTable"};

    for (int i = 0; i < sizeof(stdFuncs) / sizeof(stdFuncs[0]); i++) {
        if (strstr(line, stdFuncs[i]) != NULL && 
            (line == strstr(line, stdFuncs[i]) || !isalnum(line[strstr(line, stdFuncs[i]) - line - 1]))) {
            (*stdFuncCount)++;
        }
    }

    for (int i = 0; i < sizeof(userFuncs) / sizeof(userFuncs[0]); i++) {
        if (strstr(line, userFuncs[i]) != NULL && 
            (line == strstr(line, userFuncs[i]) || !isalnum(line[strstr(line, userFuncs[i]) - line - 1]))) {
            (*userFuncCount)++;
        }
    }
}

// Identify C keywords
void identifyKeywords(const char *line) {
    const char *keywords[] = {"int", "char", "double", "float", "for", "while", "if", "else", "return", "void", "struct", "typedef"};
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strstr(line, keywords[i]) != NULL && 
            (line == strstr(line, keywords[i]) || !isalnum(line[strstr(line, keywords[i]) - line - 1]))) {
            printf("Keyword Found: %s\n", keywords[i]);
        }
    }
}

// Track standard library functions
void trackLibraryFunctions(const char *line, int *libFuncCount) {
    const char *libFuncs[] = {"printf", "scanf", "gets", "puts", "fscanf", "malloc", "free", "strlen", "strcpy", "strcat"};
    for (int i = 0; i < sizeof(libFuncs) / sizeof(libFuncs[0]); i++) {
        if (strstr(line, libFuncs[i]) != NULL && 
            (line == strstr(line, libFuncs[i]) || !isalnum(line[strstr(line, libFuncs[i]) - line - 1]))) {
            (*libFuncCount)++;
        }
    }
}

// Analyze variable declarations
void analyzeVariableDeclarations(const char *line) {
    const char *types[] = {"int", "char", "double", "float", "void", "struct", "long"};
    for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
        if (strstr(line, types[i]) != NULL && 
            (line == strstr(line, types[i]) || !isalnum(line[strstr(line, types[i]) - line - 1]))) {
            printf("Variable Declaration Found: %s\n", types[i]);
        }
    }
}

// Detect type mismatches (placeholder)
void detectTypeMismatches(const char *line, int lineNumber) {
    // Placeholder for type mismatch detection logic
    // For now, do nothing to avoid spurious messages
}

// Check for complex expressions
int isComplexExpression(const char *line) {
    // Check for arithmetic operators
    if (strstr(line, "+") || strstr(line, "-") || strstr(line, "*") || strstr(line, "/") || strstr(line, "%")) {
        return 1; // Complex expression detected
    }
    // Check for logical operators
    if (strstr(line, "&&") || strstr(line, "||") || strstr(line, "!")) {
        return 1; // Complex expression detected
    }
    // Check for nested function calls
    if (strstr(line, "(") && strstr(line, ")") && strstr(line, "(") != strstr(line, ")")) {
        return 1; // Nested function call detected
    }
    return 0; // Not a complex expression
}