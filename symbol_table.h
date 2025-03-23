#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 1000
#define NAME_LENGTH 100
#define MAX_PARAMS 100

// Symbol Table Structure
typedef struct {
    char name[NAME_LENGTH];          // Name of the symbol (variable or function)
    char type[NAME_LENGTH];         // Data type (e.g., int, char, double)
    int scope;                      // Scope level (0 for global, 1 for local)
    int isFunction;                 // 1 if it's a function, 0 otherwise
    int paramCount;                 // Number of parameters (for functions)
    char paramTypes[MAX_PARAMS][NAME_LENGTH]; // Types of parameters (for functions)
    int memorySize;                 // Memory used by the variable in bytes
} Symbol;

extern Symbol symbolTable[MAX_SYMBOLS];    // Array to store symbols
extern int symbolCount;                    // Total number of symbols

// Function prototypes
int addSymbol(const char *name, const char *type, int scope, int isFunction, int paramCount, char paramTypes[][NAME_LENGTH], int memorySize);
int lookupSymbol(const char *name, int scope, Symbol *result);
void printSymbolTable();
void calculateMemoryUsage();

#endif // SYMBOL_TABLE_H