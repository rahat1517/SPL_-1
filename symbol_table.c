#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000
#define NAME_LENGTH 100
#define MAX_PARAMS 100

typedef struct {
    char name[NAME_LENGTH];
    char type[NAME_LENGTH];
    int scope;  // 0 for global, 1 for local
    int isFunction;  // 1 if it's a function
    int paramCount;
    char paramTypes[MAX_PARAMS][NAME_LENGTH];
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

int addSymbol(const char *name, const char *type, int scope, int isFunction, int paramCount, char paramTypes[][NAME_LENGTH]) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && symbolTable[i].scope == scope) {
            return 0; // Symbol already exists
        }
    }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolTable[symbolCount].scope = scope;
    symbolTable[symbolCount].isFunction = isFunction;
    symbolTable[symbolCount].paramCount = paramCount;
    if (isFunction) {
        for (int i = 0; i < paramCount; i++) {
            strcpy(symbolTable[symbolCount].paramTypes[i], paramTypes[i]);
        }
    }
    symbolCount++;
    return 1;
}

int lookupSymbol(const char *name, int scope, Symbol *result) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0 && (symbolTable[i].scope == scope || symbolTable[i].scope == 0)) {
            if (result) *result = symbolTable[i];
            return 1;
        }
    }
    return 0;
}

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("Name\tType\tScope\tIsFunction\tParamCount\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\t%s\t%s\t%d\t%d\n", symbolTable[i].name, symbolTable[i].type,
               symbolTable[i].scope == 0 ? "Global" : "Local",
               symbolTable[i].isFunction, symbolTable[i].paramCount);
    }
}
