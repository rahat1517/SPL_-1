#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

Symbol symbolTable[MAX_SYMBOLS];    // Array to store symbols
int symbolCount = 0;                // Total number of symbols

// Add a symbol to the symbol table
int addSymbol(const char *name, const char *type, int scope, int isFunction, int paramCount, char paramTypes[][NAME_LENGTH], int memorySize) {
    // Check if the symbol already exists
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && symbolTable[i].scope == scope) {
            return 0; // Symbol already exists
        }
    }

    // Add the new symbol to the symbol table
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolTable[symbolCount].scope = scope;
    symbolTable[symbolCount].isFunction = isFunction;
    symbolTable[symbolCount].paramCount = paramCount;
    symbolTable[symbolCount].memorySize = memorySize;

    // Add parameter types for functions
    if (isFunction) {
        for (int i = 0; i < paramCount; i++) {
            strcpy(symbolTable[symbolCount].paramTypes[i], paramTypes[i]);
        }
    }

    symbolCount++; // Increment the symbol count
    return 1;
}

// Lookup a symbol in the symbol table
int lookupSymbol(const char *name, int scope, Symbol *result) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && (symbolTable[i].scope == scope || symbolTable[i].scope == 0)) {
            if (result) *result = symbolTable[i];
            return 1;
        }
    }
    return 0;
}

// Print the symbol table
void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("Name\tType\tScope\tIsFunction\tParamCount\tMemorySize\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\t%s\t%s\t%d\t%d\t%d\n", symbolTable[i].name, symbolTable[i].type,
               symbolTable[i].scope == 0 ? "Global" : "Local",
               symbolTable[i].isFunction, symbolTable[i].paramCount, symbolTable[i].memorySize);
    }
}

// Calculate memory usage
void calculateMemoryUsage() {
    int totalMemory = 0;
    printf("\nDebug: Symbol Table Contents:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("Name: %s, Type: %s, Scope: %d, IsFunction: %d, MemorySize: %d\n",
               symbolTable[i].name, symbolTable[i].type, symbolTable[i].scope,
               symbolTable[i].isFunction, symbolTable[i].memorySize);
    }
    for (int i = 0; i < symbolCount; i++) {
        if (!symbolTable[i].isFunction) { // Only calculate memory for variables, not functions
            if (strcmp(symbolTable[i].type, "int") == 0) {
                totalMemory += sizeof(int); // int typically uses 4 bytes
            } else if (strcmp(symbolTable[i].type, "double") == 0) {
                totalMemory += sizeof(double); // double typically uses 8 bytes
            } else if (strcmp(symbolTable[i].type, "char") == 0) {
                totalMemory += sizeof(char); // char typically uses 1 byte
            }
        }
    }
    printf("\nTotal Memory Used: %d bytes\n", totalMemory);
}