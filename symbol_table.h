#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000
#define NAME_LENGTH 100
#define MAX_PARAMS 100
#define MAX_SCOPES 100

// Symbol Table Structure
typedef struct {
    char name[NAME_LENGTH];
    char type[NAME_LENGTH];
    int scope;  // 0 for global, 1+ for local
    int isFunction;  // 1 if it's a function
    int paramCount;
    char paramTypes[MAX_PARAMS][NAME_LENGTH];
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} Scope;

extern Scope scopes[MAX_SCOPES];
extern int currentScope;
extern int scopeCount;

// Symbol Table Functions
int addSymbol(const char *name, const char *type, int isFunction, int paramCount, char paramTypes[][NAME_LENGTH]);
void printSymbolTable();

#endif // SYMBOL_TABLE_H