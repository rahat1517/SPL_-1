#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "analysis.h"
#include "types.h"
#include "printf_checker.h"

#define MAX_LINE_LENGTH 1024

// Case-insensitive strstr implementation
const char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return haystack; // Empty needle matches everything

    for (; *haystack; haystack++) {
        if (tolower(*haystack) == tolower(*needle)) {
            const char *h = haystack + 1;
            const char *n = needle + 1;
            while (*h && *n && tolower(*h) == tolower(*n)) {
                h++;
                n++;
            }
            if (!*n) return haystack; // Found a match
        }
    }
    return NULL; // No match found
}

int main() {
    // Open the input file
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 1;
    int stdFuncCount = 0;
    int userFuncCount = 0;
    int libFuncCount = 0;

    while (fgets(line, MAX_LINE_LENGTH, input)) {
        // Remove comments from the line
        removeComments(line);

        // Check for variable declarations and add to symbol table
        if (strstr(line, "int") != NULL) {
            char varName[NAME_LENGTH];
            if (sscanf(line, "int %s", varName) == 1) { // Extract variable name
                addSymbol(varName, "int", 1, 0, 0, NULL, sizeof(int)); // Add to symbol table
            }
        }
        if (strstr(line, "double") != NULL) {
            char varName[NAME_LENGTH];
            if (sscanf(line, "double %s", varName) == 1) { // Extract variable name
                addSymbol(varName, "double", 1, 0, 0, NULL, sizeof(double)); // Add to symbol table
            }
        }
        if (strstr(line, "char") != NULL) {
            char varName[NAME_LENGTH];
            if (sscanf(line, "char %s", varName) == 1) { // Extract variable name
                addSymbol(varName, "char", 1, 0, 0, NULL, sizeof(char)); // Add to symbol table
            }
        }

        // Check for function declarations and add to symbol table
        if (strstr(line, "int main()") != NULL) {
            addSymbol("main", "int", 0, 1, 0, NULL, 0); // Add main function to symbol table
        }

        // Process the line for other tasks (e.g., syntax checking, keyword identification)
        checkSyntaxErrors(line, lineNumber);
        countFunctions(line, &stdFuncCount, &userFuncCount);
        identifyKeywords(line);
        trackLibraryFunctions(line, &libFuncCount);
        analyzeVariableDeclarations(line);
        detectTypeMismatches(line, lineNumber);

        if (isPrintfTypo(line)) {
            printf("Typo found at line %d.\n", lineNumber);
        }

        if (strcasestr(line, "printf") != NULL) {
            validatePrintfFormat(line, lineNumber);
        }

        if (isComplexExpression(line)) {
            printf("Complex expression found at line %d.\n", lineNumber);
        }

        parse(line, lineNumber);

        lineNumber++;
    }

    // Close the input file
    fclose(input);

    // Print summary
    printf("Standard Library Functions: %d\n", stdFuncCount);
    printf("User-defined Functions: %d\n", userFuncCount);
    printf("Library Functions: %d\n", libFuncCount);

    // Calculate and print memory usage
    calculateMemoryUsage();

    // Print the symbol table
    printSymbolTable();

    return 0;
}