#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"

// Global Variables
Scope scopes[MAX_SCOPES];
int currentScope = 0;
int scopeCount = 1; // Start with global scope (scope 0)

int main() {
    FILE *input = fopen("input.txt.txt", "r");
    if (!input) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 1;
    while (fgets(line, MAX_LINE_LENGTH, input)) {
        parse(line, lineNumber); // Pass line number to parser
        lineNumber++;
    }

    fclose(input);
    printSymbolTable();
    return 0;
}