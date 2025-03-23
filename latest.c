/**
 * C Syntax Checker - A Lightweight Tool for Error Detection in C Programs
 * 
 * This program analyzes C source code to detect common syntax errors and
 * provides detailed statistics about the code structure.
 * 
 * Features:
 * - Detects unbalanced braces, parentheses, and brackets
 * - Identifies missing semicolons
 * - Finds unmatched quotes
 * - Flags undefined variables
 * - Counts functions (both user-defined and standard library)
 * - Calculates memory usage by variables
 * - Identifies C language keywords and standard library functions
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <stdbool.h>
 
 /* Maximum lengths for various elements */
 #define MAX_LINE_LENGTH 1024
 #define MAX_FILENAME_LENGTH 256
 #define MAX_IDENTIFIER_LENGTH 256
 #define MAX_ERROR_MSG_LENGTH 512
 #define MAX_TOKENS 10000
 #define MAX_STACK_SIZE 1000
 #define MAX_VARIABLES 1000
 #define MAX_FUNCTIONS 1000
 #define MAX_KEYWORDS 32
 
 /* Token types */
 typedef enum {
     TOKEN_IDENTIFIER,
     TOKEN_KEYWORD,
     TOKEN_NUMBER,
     TOKEN_STRING,
     TOKEN_CHAR,
     TOKEN_OPERATOR,
     TOKEN_SEPARATOR,
     TOKEN_PREPROCESSOR,
     TOKEN_COMMENT,
     TOKEN_EOF
 } TokenType;
 
 /* Token structure */
 typedef struct {
     TokenType type;
     char lexeme[MAX_IDENTIFIER_LENGTH];
     int line;
     int column;
 } Token;
 
 /* Variable information structure */
 typedef struct {
     char name[MAX_IDENTIFIER_LENGTH];
     char type[MAX_IDENTIFIER_LENGTH];
     int size;
     int line;
     bool isArray;
     int arraySize;
 } Variable;
 
 /* Function information structure */
 typedef struct {
     char name[MAX_IDENTIFIER_LENGTH];
     bool isUserDefined;
     bool isPrototype;
     int line;
 } Function;
 
 /* Error information structure */
 typedef struct {
     char message[MAX_ERROR_MSG_LENGTH];
     int line;
     int column;
 } Error;
 
 /* Global variables */
 Token tokens[MAX_TOKENS];
 int tokenCount = 0;
 char sourceCode[MAX_LINE_LENGTH * 1000]; // Buffer for the entire source code
 Variable variables[MAX_VARIABLES];
 int variableCount = 0;
 Function functions[MAX_FUNCTIONS];
 int functionCount = 0;
 Error errors[MAX_TOKENS];
 int errorCount = 0;
 
 /* C Keywords */
 const char *keywords[] = {
     "auto", "break", "case", "char", "const", "continue", "default", "do",
     "double", "else", "enum", "extern", "float", "for", "goto", "if",
     "int", "long", "register", "return", "short", "signed", "sizeof", "static",
     "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
 };
 
 /* Standard library functions */
 const char *stdLibFunctions[] = {
     "printf", "scanf", "gets", "puts", "fprintf", "fscanf", "fgets", "fputs",
     "malloc", "calloc", "realloc", "free", "memcpy", "memmove", "memset",
     "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp", "strlen"
 };
 
 /* Function prototypes */
 void readSourceFile(const char* filename);
 void tokenize();
 void analyzeCode();
 void checkBalancedDelimiters();
 void checkMissingSemicolons();
 void checkUnmatchedQuotes();
 void identifyFunctions();
 void trackVariables();
 void calculateMemoryUsage();
 void identifyKeywords();
 void checkPrintfErrors();
 void checkScanfErrors();
 bool isKeyword(const char *identifier);
 bool isStdLibFunction(const char *identifier);
 int getTypeSize(const char *type);
 void reportError(const char *message, int line, int column);
 void printResults();
 
 /**
  * Main function - Entry point of the program
  */
 int main(int argc, char *argv[]) {
     if (argc < 2) {
         printf("Usage: %s <source_file>\n", argv[0]);
         return 1;
     }
     
     readSourceFile(argv[1]);
     tokenize();
     analyzeCode();
     printResults();
     
     return 0;
 }
 
 /**
  * Reads the source code file into memory
  */
 void readSourceFile(const char* filename) {
     FILE *file = fopen(filename, "r");
     if (!file) {
         printf("Error: Cannot open file %s\n", filename);
         exit(1);
     }
     
     char line[MAX_LINE_LENGTH];
     sourceCode[0] = '\0';
     
     while (fgets(line, MAX_LINE_LENGTH, file)) {
         strcat(sourceCode, line);
     }
     
     fclose(file);
     printf("File read successfully: %s\n", filename);
 }
 
 /**
  * Tokenizes the source code
  */
 void tokenize() {
     char *current = sourceCode;
     int line = 1;
     int column = 1;
     char lexeme[MAX_IDENTIFIER_LENGTH];
     int lexemeIndex;
     
     while (*current != '\0') {
         // Skip whitespace
         if (isspace(*current)) {
             if (*current == '\n') {
                 line++;
                 column = 1;
             } else {
                 column++;
             }
             current++;
             continue;
         }
         
         // Handle comments
         if (*current == '/' && *(current + 1) == '/') {
             // Single-line comment
             while (*current != '\0' && *current != '\n') {
                 current++;
             }
             continue;
         }
         
         if (*current == '/' && *(current + 1) == '*') {
             // Multi-line comment
             current += 2;
             column += 2;
             while (*current != '\0' && !(*current == '*' && *(current + 1) == '/')) {
                 if (*current == '\n') {
                     line++;
                     column = 1;
                 } else {
                     column++;
                 }
                 current++;
             }
             if (*current != '\0') {
                 current += 2;
                 column += 2;
             }
             continue;
         }
         
         // Handle preprocessor directives
         if (*current == '#') {
             lexemeIndex = 0;
             lexeme[lexemeIndex++] = *current++;
             column++;
             
             while (*current != '\0' && *current != '\n') {
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 current++;
                 column++;
             }
             
             lexeme[lexemeIndex] = '\0';
             
             tokens[tokenCount].type = TOKEN_PREPROCESSOR;
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - lexemeIndex;
             tokenCount++;
             
             continue;
         }
         
         // Handle string literals
         if (*current == '"') {
             lexemeIndex = 0;
             lexeme[lexemeIndex++] = *current++;
             column++;
             
             while (*current != '\0' && *current != '"') {
                 if (*current == '\\' && *(current + 1) != '\0') {
                     // Skip escaped characters
                     if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                         lexeme[lexemeIndex++] = *current;
                     }
                     current++;
                     column++;
                 }
                 
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 
                 if (*current == '\n') {
                     line++;
                     column = 1;
                 } else {
                     column++;
                 }
                 
                 current++;
             }
             
             if (*current == '"') {
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 current++;
                 column++;
             }
             
             lexeme[lexemeIndex] = '\0';
             
             tokens[tokenCount].type = TOKEN_STRING;
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - lexemeIndex;
             tokenCount++;
             
             continue;
         }
         
         // Handle character literals
         if (*current == '\'') {
             lexemeIndex = 0;
             lexeme[lexemeIndex++] = *current++;
             column++;
             
             while (*current != '\0' && *current != '\'') {
                 if (*current == '\\' && *(current + 1) != '\0') {
                     // Skip escaped characters
                     if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                         lexeme[lexemeIndex++] = *current;
                     }
                     current++;
                     column++;
                 }
                 
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 
                 current++;
                 column++;
             }
             
             if (*current == '\'') {
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 current++;
                 column++;
             }
             
             lexeme[lexemeIndex] = '\0';
             
             tokens[tokenCount].type = TOKEN_CHAR;
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - lexemeIndex;
             tokenCount++;
             
             continue;
         }
         
         // Handle numbers
         if (isdigit(*current)) {
             lexemeIndex = 0;
             
             while (isdigit(*current) || *current == '.' || *current == 'e' || *current == 'E' ||
                    *current == 'f' || *current == 'F' || *current == 'l' || *current == 'L' ||
                    ((*current == '+' || *current == '-') && (*(current - 1) == 'e' || *(current - 1) == 'E'))) {
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 current++;
                 column++;
             }
             
             lexeme[lexemeIndex] = '\0';
             
             tokens[tokenCount].type = TOKEN_NUMBER;
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - lexemeIndex;
             tokenCount++;
             
             continue;
         }
         
         // Handle identifiers and keywords
         if (isalpha(*current) || *current == '_') {
             lexemeIndex = 0;
             
             while (isalnum(*current) || *current == '_') {
                 if (lexemeIndex < MAX_IDENTIFIER_LENGTH - 1) {
                     lexeme[lexemeIndex++] = *current;
                 }
                 current++;
                 column++;
             }
             
             lexeme[lexemeIndex] = '\0';
             
             if (isKeyword(lexeme)) {
                 tokens[tokenCount].type = TOKEN_KEYWORD;
             } else {
                 tokens[tokenCount].type = TOKEN_IDENTIFIER;
             }
             
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - lexemeIndex;
             tokenCount++;
             
             continue;
         }
         
         // Handle operators and separators
         if (strchr("+-*/%%=<>!&|^~?:.,;()[]{}", *current)) {
             // Handle multi-character operators
             if ((*current == '+' && *(current + 1) == '+') ||
                 (*current == '-' && *(current + 1) == '-') ||
                 (*current == '=' && *(current + 1) == '=') ||
                 (*current == '!' && *(current + 1) == '=') ||
                 (*current == '<' && *(current + 1) == '=') ||
                 (*current == '>' && *(current + 1) == '=') ||
                 (*current == '&' && *(current + 1) == '&') ||
                 (*current == '|' && *(current + 1) == '|') ||
                 (*current == '+' && *(current + 1) == '=') ||
                 (*current == '-' && *(current + 1) == '=') ||
                 (*current == '*' && *(current + 1) == '=') ||
                 (*current == '/' && *(current + 1) == '=') ||
                 (*current == '%' && *(current + 1) == '=') ||
                 (*current == '&' && *(current + 1) == '=') ||
                 (*current == '|' && *(current + 1) == '=') ||
                 (*current == '^' && *(current + 1) == '=') ||
                 (*current == '-' && *(current + 1) == '>')) {
                 
                 lexeme[0] = *current++;
                 lexeme[1] = *current++;
                 lexeme[2] = '\0';
                 column += 2;
             } else {
                 lexeme[0] = *current++;
                 lexeme[1] = '\0';
                 column++;
             }
             
             tokens[tokenCount].type = (strchr(".,;()[]{}", lexeme[0])) ? TOKEN_SEPARATOR : TOKEN_OPERATOR;
             strcpy(tokens[tokenCount].lexeme, lexeme);
             tokens[tokenCount].line = line;
             tokens[tokenCount].column = column - strlen(lexeme);
             tokenCount++;
             
             continue;
         }
         
         // Skip unrecognized characters
         current++;
         column++;
     }
     
     // Add EOF token
     tokens[tokenCount].type = TOKEN_EOF;
     strcpy(tokens[tokenCount].lexeme, "EOF");
     tokens[tokenCount].line = line;
     tokens[tokenCount].column = column;
     tokenCount++;
     
     printf("Tokenization complete. Found %d tokens.\n", tokenCount);
 }
 
 /**
  * Runs all analysis passes on the code
  */
 void analyzeCode() {
     checkBalancedDelimiters();
     checkMissingSemicolons();
     checkUnmatchedQuotes();
     identifyFunctions();
     trackVariables();
     calculateMemoryUsage();
     identifyKeywords();
     checkPrintfErrors();
     checkScanfErrors();
 }
 
 /**
  * Checks for balanced delimiters (parentheses, braces, brackets)
  */
 void checkBalancedDelimiters() {
     char stack[MAX_STACK_SIZE];
     int stackTop = -1;
     int positions[MAX_STACK_SIZE];
     int lines[MAX_STACK_SIZE];
     int columns[MAX_STACK_SIZE];
     
     printf("Checking for balanced delimiters...\n");
     
     for (int i = 0; i < tokenCount; i++) {
         if (tokens[i].type == TOKEN_SEPARATOR) {
             if (strcmp(tokens[i].lexeme, "(") == 0 || 
                 strcmp(tokens[i].lexeme, "{") == 0 || 
                 strcmp(tokens[i].lexeme, "[") == 0) {
                 
                 if (stackTop >= MAX_STACK_SIZE - 1) {
                     printf("Stack overflow during delimiter checking\n");
                     return;
                 }
                 
                 // Push onto stack
                 stack[++stackTop] = tokens[i].lexeme[0];
                 positions[stackTop] = i;
                 lines[stackTop] = tokens[i].line;
                 columns[stackTop] = tokens[i].column;
             } else if (strcmp(tokens[i].lexeme, ")") == 0 || 
                        strcmp(tokens[i].lexeme, "}") == 0 || 
                        strcmp(tokens[i].lexeme, "]") == 0) {
                 
                 if (stackTop == -1) {
                     // No matching opening delimiter
                     char message[MAX_ERROR_MSG_LENGTH];
                     sprintf(message, "Unmatched closing delimiter '%s'", tokens[i].lexeme);
                     reportError(message, tokens[i].line, tokens[i].column);
                     continue;
                 }
                 
                 char expected;
                 switch (stack[stackTop]) {
                     case '(': expected = ')'; break;
                     case '{': expected = '}'; break;
                     case '[': expected = ']'; break;
                     default: expected = '\0';
                 }
                 
                 char actual = tokens[i].lexeme[0];
                 
                 if (actual != expected) {
                     // Mismatched delimiter
                     char message[MAX_ERROR_MSG_LENGTH];
                     sprintf(message, "Mismatched delimiter: expected '%c' but found '%c'. Opening delimiter at line %d, column %d",
                             expected, actual, lines[stackTop], columns[stackTop]);
                     reportError(message, tokens[i].line, tokens[i].column);
                 }
                 
                 // Pop from stack
                 stackTop--;
             }
         }
     }
     
     // Check for unclosed delimiters
     while (stackTop >= 0) {
         char message[MAX_ERROR_MSG_LENGTH];
         sprintf(message, "Unclosed delimiter '%c'", stack[stackTop]);
         reportError(message, lines[stackTop], columns[stackTop]);
         stackTop--;
     }
 }
 
 /**
  * Checks for missing semicolons
  */
 void checkMissingSemicolons() {
     printf("Checking for missing semicolons...\n");
     
     for (int i = 0; i < tokenCount - 1; i++) {
         // Skip preprocessor directives and their contents
         if (tokens[i].type == TOKEN_PREPROCESSOR) {
             while (i < tokenCount - 1 && tokens[i].line == tokens[i+1].line) {
                 i++;
             }
             continue;
         }
         
         // Skip function declarations/definitions (they don't need semicolons)
         if (i + 2 < tokenCount && 
             tokens[i].type == TOKEN_IDENTIFIER && 
             strcmp(tokens[i+1].lexeme, "(") == 0) {
             // Find the matching closing parenthesis
             int j = i + 2;
             int parenCount = 1;
             
             while (j < tokenCount && parenCount > 0) {
                 if (strcmp(tokens[j].lexeme, "(") == 0) parenCount++;
                 if (strcmp(tokens[j].lexeme, ")") == 0) parenCount--;
                 j++;
             }
             
             // Check if this is followed by a block (function definition) or semicolon (prototype)
             if (j < tokenCount && strcmp(tokens[j].lexeme, "{") == 0) {
                 // This is a function definition, skip to the end of the function
                 int braceCount = 1;
                 j++;
                 
                 while (j < tokenCount && braceCount > 0) {
                     if (strcmp(tokens[j].lexeme, "{") == 0) braceCount++;
                     if (strcmp(tokens[j].lexeme, "}") == 0) braceCount--;
                     j++;
                 }
                 
                 i = j - 1;
                 continue;
             }
         }
         
         // Check for statements that should end with semicolons
         if ((tokens[i].type == TOKEN_IDENTIFIER || 
              tokens[i].type == TOKEN_NUMBER || 
              strcmp(tokens[i].lexeme, ")") == 0 || 
              strcmp(tokens[i].lexeme, "]") == 0 || 
              strcmp(tokens[i].lexeme, "++") == 0 || 
              strcmp(tokens[i].lexeme, "--") == 0 || 
              tokens[i].type == TOKEN_STRING || 
              tokens[i].type == TOKEN_CHAR) && 
             tokens[i+1].line > tokens[i].line && 
             strcmp(tokens[i].lexeme, ";") != 0 && 
             strcmp(tokens[i].lexeme, "{") != 0 && 
             strcmp(tokens[i].lexeme, "}") != 0) {
             
             // Check if the previous line ended with a semicolon
             bool hasEndingSemicolon = false;
             int j = i;
             while (j >= 0 && tokens[j].line == tokens[i].line) {
                 if (strcmp(tokens[j].lexeme, ";") == 0) {
                     hasEndingSemicolon = true;
                     break;
                 }
                 j--;
             }
             
             if (!hasEndingSemicolon) {
                 reportError("Missing semicolon at end of statement", tokens[i].line, tokens[i].column + strlen(tokens[i].lexeme));
             }
         }
     }
 }
 
 /**
  * Checks for unmatched quotes in string and character literals
  */
 void checkUnmatchedQuotes() {
     printf("Checking for unmatched quotes...\n");
     
     for (int i = 0; i < tokenCount; i++) {
         if (tokens[i].type == TOKEN_STRING) {
             const char* lexeme = tokens[i].lexeme;
             int len = strlen(lexeme);
             
             // A properly formed string token should start and end with double quotes
             if (len < 2 || lexeme[0] != '"' || lexeme[len-1] != '"') {
                 reportError("Unterminated string literal", tokens[i].line, tokens[i].column);
             }
         } else if (tokens[i].type == TOKEN_CHAR) {
             const char* lexeme = tokens[i].lexeme;
             int len = strlen(lexeme);
             
             // A properly formed character token should start and end with single quotes
             if (len < 2 || lexeme[0] != '\'' || lexeme[len-1] != '\'') {
                 reportError("Unterminated character literal", tokens[i].line, tokens[i].column);
             }
             
             // Character literals should contain exactly one character or escape sequence
             if (len == 2) {
                 reportError("Empty character literal", tokens[i].line, tokens[i].column);
             } else if (len > 4 && lexeme[1] != '\\') {
                 reportError("Multi-character literal (too many characters)", tokens[i].line, tokens[i].column);
             }
         }
     }
 }
 
 /**
  * Identifies functions (both user-defined and standard library)
  */
 void identifyFunctions() {
     printf("Identifying functions...\n");
     
     // First pass: identify function declarations and definitions
     for (int i = 0; i < tokenCount - 2; i++) {
         // Look for patterns like: type identifier(params)
         // or: type * identifier(params)
         if ((i > 0 && tokens[i-1].type == TOKEN_KEYWORD && tokens[i].type == TOKEN_IDENTIFIER) || 
             (i > 1 && tokens[i-2].type == TOKEN_KEYWORD && tokens[i-1].type == TOKEN_OPERATOR && 
              strcmp(tokens[i-1].lexeme, "*") == 0 && tokens[i].type == TOKEN_IDENTIFIER)) {
             
             if (i+1 < tokenCount && strcmp(tokens[i+1].lexeme, "(") == 0) {
                 // Found potential function declaration or definition
                 Function func;
                 strcpy(func.name, tokens[i].lexeme);
                 func.line = tokens[i].line;
                 
                 // Determine if it's a prototype or definition
                 // Find closing parenthesis
                 int j = i + 2;
                 int parenCount = 1;
                 
                 while (j < tokenCount && parenCount > 0) {
                     if (strcmp(tokens[j].lexeme, "(") == 0) parenCount++;
                     if (strcmp(tokens[j].lexeme, ")") == 0) parenCount--;
                     j++;
                 }
                 
                 if (j < tokenCount) {
                     // Check what follows the closing parenthesis
                     while (j < tokenCount && (tokens[j].type == TOKEN_OPERATOR || isspace(*tokens[j].lexeme))) {
                         j++;
                     }
                     
                     if (j < tokenCount && strcmp(tokens[j].lexeme, "{") == 0) {
                         // This is a function definition
                         func.isUserDefined = true;
                         func.isPrototype = false;
                     } else {
                         // This is a function prototype
                         func.isUserDefined = true;
                         func.isPrototype = true;
                     }
                     
                     // Add to functions array if not already present
                     bool alreadyExists = false;
                     for (int k = 0; k < functionCount; k++) {
                         if (strcmp(functions[k].name, func.name) == 0) {
                             alreadyExists = true;
                             break;
                         }
                     }
                     
                     if (!alreadyExists && functionCount < MAX_FUNCTIONS) {
                         functions[functionCount++] = func;
                     }
                 }
             }
         }
     }
     
     // Second pass: identify function calls
     for (int i = 0; i < tokenCount - 1; i++) {
         if (tokens[i].type == TOKEN_IDENTIFIER && 
             i+1 < tokenCount && strcmp(tokens[i+1].lexeme, "(") == 0) {
             
             // Check if this is a function call (not a declaration)
             if (i == 0 || tokens[i-1].type != TOKEN_KEYWORD) {
                 const char* funcName = tokens[i].lexeme;
                 
                 // Check if it's a standard library function
                 if (isStdLibFunction(funcName)) {
                     // Add to functions array if not already present
                     bool alreadyExists = false;
                     for (int j = 0; j < functionCount; j++) {
                         if (strcmp(functions[j].name, funcName) == 0) {
                             alreadyExists = true;
                             break;
                         }
                     }
                     
                     if (!alreadyExists && functionCount < MAX_FUNCTIONS) {
                         Function func;
                         strcpy(func.name, funcName);
                         func.isUserDefined = false;
                         func.isPrototype = false;
                         func.line = tokens[i].line;
                         functions[functionCount++] = func;
                     }
                 }
             }
         }
     }
 }
 
 /**
  * Tracks variable declarations and usage
  */
 void trackVariables() {
     printf("Tracking variables...\n");
     
     // Keep track of scope level
     int scopeLevel = 0;
     
     for (int i = 0; i < tokenCount - 1; i++) {
         // Track scope changes
         if (tokens[i].type == TOKEN_SEPARATOR) {
             if (strcmp(tokens[i].lexeme, "{") == 0) {
                 scopeLevel++;
             } else if (strcmp(tokens[i].lexeme, "}") == 0) {
                 scopeLevel--;
             }
         }
         
         // Look for variable declarations
         if (tokens[i].type == TOKEN_KEYWORD && 
             (strcmp(tokens[i].lexeme, "int") == 0 || 
              strcmp(tokens[i].lexeme, "char") == 0 || 
              strcmp(tokens[i].lexeme, "float") == 0 || 
              strcmp(tokens[i].lexeme, "double") == 0 || 
              strcmp(tokens[i].lexeme, "void") == 0 || 
              strcmp(tokens[i].lexeme, "long") == 0 || 
              strcmp(tokens[i].lexeme, "short") == 0)) {
             
             // Skip function declarations
             bool isFunction = false;
             for (int j = i + 1; j < tokenCount && j < i + 10; j++) {
                 if (tokens[j].type == TOKEN_SEPARATOR && strcmp(tokens[j].lexeme, "(") == 0) {
                     isFunction = true;
                     break;
                 }
                 if (tokens[j].type == TOKEN_SEPARATOR && strcmp(tokens[j].lexeme, ";") == 0) {
                     break;
                 }
             }
             
             if (!isFunction) {
                 // This is a variable declaration
                 int j = i + 1;
                 
                 // Skip modifiers and pointer asterisks
                 while (j < tokenCount && 
                        (tokens[j].type == TOKEN_KEYWORD || 
                         (tokens[j].type == TOKEN_OPERATOR && strcmp(tokens[j].lexeme, "*") == 0))) {
                     j++;
                 }
                 
                 if (j < tokenCount && tokens[j].type == TOKEN_IDENTIFIER) {
                     Variable var;
                     strcpy(var.name, tokens[j].lexeme);
                     strcpy(var.type, tokens[i].lexeme);
                     var.size = getTypeSize(tokens[i].lexeme);
                     var.line = tokens[j].line;
                     var.isArray = false;
                     var.arraySize =.1;
                     
                     // Check if it's an array
                     if (j+1 < tokenCount && strcmp(tokens[j+1].lexeme, "[") == 0) {
                         var.isArray = true;
                         
                         // Try to get array size
                         if (j+2 < tokenCount && tokens[j+2].type == TOKEN_NUMBER) {
                             var.arraySize = atoi(tokens[j+2].lexeme);
                         }
                     }
                     
                     // Add to variables array if not already present
                     bool alreadyExists = false;
                     for (int k = 0; k < variableCount; k++) {
                         if (strcmp(variables[k].name, var.name) == 0) {
                             alreadyExists = true;
                             break;
                         }
                     }
                     
                     if (!alreadyExists && variableCount < MAX_VARIABLES) {
                         variables[variableCount++] = var;
                     }
                 }
             }
         }
     }
     
     // Check for undefined variables
     for (int i = 0; i < tokenCount; i++) {
         if (tokens[i].type == TOKEN_IDENTIFIER) {
             // Skip function names
             bool isFunction = false;
             for (int j = 0; j < functionCount; j++) {
                 if (strcmp(functions[j].name, tokens[i].lexeme) == 0) {
                     isFunction = true;
                     break;
                 }
             }
             
             if (!isFunction) {
                 // Check if this identifier is a declared variable
                 bool isDeclared = false;
                 for (int j = 0; j < variableCount; j++) {
                     
                     if (strcmp(variables[j].name, tokens[i].lexeme) == 0) {
                        isDeclared = true;
                        break;
                    }
                }
                
                if (!isDeclared) {
                    // Check if this is a variable use, not a declaration
                    bool isBeingDeclared = false;
                    
                    // Check if it's part of a declaration (looking backward)
                    for (int j = i - 1; j >= 0 && j >= i - 5; j--) {
                        if (tokens[j].type == TOKEN_KEYWORD && 
                            (strcmp(tokens[j].lexeme, "int") == 0 || 
                             strcmp(tokens[j].lexeme, "char") == 0 || 
                             strcmp(tokens[j].lexeme, "float") == 0 || 
                             strcmp(tokens[j].lexeme, "double") == 0 || 
                             strcmp(tokens[j].lexeme, "void") == 0 || 
                             strcmp(tokens[j].lexeme, "long") == 0 || 
                             strcmp(tokens[j].lexeme, "short") == 0)) {
                            isBeingDeclared = true;
                            break;
                        }
                        
                        if (tokens[j].type == TOKEN_SEPARATOR && strcmp(tokens[j].lexeme, ";") == 0) {
                            break;
                        }
                    }
                    
                    if (!isBeingDeclared) {
                        // Flag undefined variable
                        char message[MAX_ERROR_MSG_LENGTH];
                        sprintf(message, "Undefined variable '%s'", tokens[i].lexeme);
                        reportError(message, tokens[i].line, tokens[i].column);
                    }
                }
            }
        }
    }
}

/**
 * Calculates memory usage by variables
 */
void calculateMemoryUsage() {
    int totalMemory = 0;
    int typeMemory[5] = {0}; // For int, char, float, double, other
    
    printf("Calculating memory usage...\n");
    
    for (int i = 0; i < variableCount; i++) {
        int size = variables[i].size;
        
        if (variables[i].isArray) {
            size *= variables[i].arraySize;
        }
        
        totalMemory += size;
        
        // Categorize by type
        if (strcmp(variables[i].type, "int") == 0) {
            typeMemory[0] += size;
        } else if (strcmp(variables[i].type, "char") == 0) {
            typeMemory[1] += size;
        } else if (strcmp(variables[i].type, "float") == 0) {
            typeMemory[2] += size;
        } else if (strcmp(variables[i].type, "double") == 0) {
            typeMemory[3] += size;
        } else {
            typeMemory[4] += size;
        }
    }
    
    printf("Total memory usage: %d bytes\n", totalMemory);
    printf("  - Int variables: %d bytes\n", typeMemory[0]);
    printf("  - Char variables: %d bytes\n", typeMemory[1]);
    printf("  - Float variables: %d bytes\n", typeMemory[2]);
    printf("  - Double variables: %d bytes\n", typeMemory[3]);
    printf("  - Other variables: %d bytes\n", typeMemory[4]);
}

/**
 * Identifies C language keywords and counts their usage
 */
void identifyKeywords() {
    int keywordCount[MAX_KEYWORDS] = {0};
    
    printf("Identifying keywords...\n");
    
    for (int i = 0; i < tokenCount; i++) {
        if (tokens[i].type == TOKEN_KEYWORD) {
            // Find the keyword in the keywords array
            for (int j = 0; j < MAX_KEYWORDS; j++) {
                if (strcmp(tokens[i].lexeme, keywords[j]) == 0) {
                    keywordCount[j]++;
                    break;
                }
            }
        }
    }
    
    // Print keyword usage
    printf("Keyword usage:\n");
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (keywordCount[i] > 0) {
            printf("  - %s: %d occurrences\n", keywords[i], keywordCount[i]);
        }
    }
}
/**
 * Checks for common printf-related errors
 */ 
void checkPrintfErrors() {
    printf("Checking for printf errors...\n");
    
    for (int i = 0; i < tokenCount - 2; i++) {
        // Look for printf function calls
        if (tokens[i].type == TOKEN_IDENTIFIER) {
            // Check for case sensitivity errors
            if (strcmp(tokens[i].lexeme, "prinTf") == 0 || strcmp(tokens[i].lexeme, "Printf") == 0 || strcmp(tokens[i].lexeme, "pRintf") == 0 || strcmp(tokens[i].lexeme, "prIntf") == 0 ||
            strcmp(tokens[i].lexeme, "priNtf") == 0 || strcmp(tokens[i].lexeme, "prinTf") == 0 || strcmp(tokens[i].lexeme, "printF") == 0 || strcmp(tokens[i].lexeme, "PRintf") == 0 ||
            strcmp(tokens[i].lexeme, "PRIntf") == 0 || strcmp(tokens[i].lexeme, "PRIntf") == 0 || strcmp(tokens[i].lexeme, "PRINtf") == 0 || strcmp(tokens[i].lexeme, "PRINTf") == 0 ||
            strcmp(tokens[i].lexeme, "PRINTF") == 0 || strcmp(tokens[i].lexeme, "pRINTF") == 0 || strcmp(tokens[i].lexeme, "prINTF") == 0 || strcmp(tokens[i].lexeme, "priNTF") == 0 ||
            strcmp(tokens[i].lexeme, "prinTF") == 0 || strcmp(tokens[i].lexeme, "printF") == 0 || strcmp(tokens[i].lexeme, "PrINTF") == 0 || strcmp(tokens[i].lexeme, "PRinTF") == 0 ||
            strcmp(tokens[i].lexeme, "PRiNTF") == 0 || strcmp(tokens[i].lexeme, "PRInTF") == 0 || strcmp(tokens[i].lexeme, "PRIeNTF") == 0 || strcmp(tokens[i].lexeme, "PRIentf") == 0 ||
            strcmp(tokens[i].lexeme, "PRintF") == 0 || strcmp(tokens[i].lexeme, "PRinTf") == 0 || strcmp(tokens[i].lexeme, "PRiNtf") == 0 || strcmp(tokens[i].lexeme, "PrIntF") == 0 ||
            strcmp(tokens[i].lexeme, "PrIntf") == 0 || strcmp(tokens[i].lexeme, "PrinTF") == 0 || strcmp(tokens[i].lexeme, "PrintF") == 0 || strcmp(tokens[i].lexeme, "pRintF") == 0 ||
            strcmp(tokens[i].lexeme, "pRinTF") == 0 || strcmp(tokens[i].lexeme, "pRInTf") == 0 || strcmp(tokens[i].lexeme, "pRINtf") == 0 || strcmp(tokens[i].lexeme, "prINTf") == 0 ||
            strcmp(tokens[i].lexeme, "priNTf") == 0 || strcmp(tokens[i].lexeme, "prinTF") == 0 || strcmp(tokens[i].lexeme, "printF") == 0 || strcmp(tokens[i].lexeme, "PrINtf") == 0 ||
            strcmp(tokens[i].lexeme, "PrIntF") == 0 || strcmp(tokens[i].lexeme, "PrinTF") == 0 || strcmp(tokens[i].lexeme, "PrintF") == 0 || strcmp(tokens[i].lexeme, "pRinTf") == 0 ||
            strcmp(tokens[i].lexeme, "pRintF") == 0 || strcmp(tokens[i].lexeme, "pRinTF") == 0 || strcmp(tokens[i].lexeme, "pRINTF") == 0 || strcmp(tokens[i].lexeme, "prINtf") == 0 ||
            strcmp(tokens[i].lexeme, "prIntF") == 0 || strcmp(tokens[i].lexeme, "priNTF") == 0 || strcmp(tokens[i].lexeme, "priNtF") == 0 || strcmp(tokens[i].lexeme, "priNtf") == 0 ||
            strcmp(tokens[i].lexeme, "prinTf") == 0 || strcmp(tokens[i].lexeme, "printF") == 0
             )
                {
                reportError("Case sensitivity error. Did you mean 'printf'?", tokens[i].line, tokens[i].column);
                continue;
            }
            
            if (strcmp(tokens[i].lexeme, "printf") == 0 && 
                i+1 < tokenCount && strcmp(tokens[i+1].lexeme, "(") == 0) {
                
                // Found printf call - look for the format string
                int j = i + 2;
                int formatStringIndex = -1;
                
                // Find the format string (should be the first argument)
                while (j < tokenCount && strcmp(tokens[j].lexeme, ")") != 0) {
                    if (tokens[j].type == TOKEN_STRING) {
                        formatStringIndex = j;
                        break;
                    }
                    j++;
                }
                
                if (formatStringIndex == -1) {
                    reportError("printf call missing format string", tokens[i].line, tokens[i].column);
                    continue;
                }
                
                // Extract format string
                char formatString[MAX_IDENTIFIER_LENGTH];
                strcpy(formatString, tokens[formatStringIndex].lexeme);
                
                // Remove surrounding quotes
                int fmtLen = strlen(formatString);
                if (fmtLen >= 2) {
                    memmove(formatString, formatString + 1, fmtLen - 2);
                    formatString[fmtLen - 2] = '\0';
                }
                
                // Count format specifiers
                int formatSpecifiers = 0;
                bool inFormatSpecifier = false;
                
                for (int k = 0; k < strlen(formatString); k++) {
                    if (formatString[k] == '%') {
                        // Check for %% which is not a format specifier
                        if (k + 1 < strlen(formatString) && formatString[k+1] == '%') {
                            k++; // Skip the second %
                            continue;
                        }
                        
                        inFormatSpecifier = true;
                        formatSpecifiers++;
                    } else if (inFormatSpecifier) {
                        // Check for invalid format specifiers
                        if (strchr("diuoxXfFeEgGaAcspn", formatString[k])) {
                            inFormatSpecifier = false;
                        } else if (!isdigit(formatString[k]) && 
                                  !strchr(".-+# hlLz", formatString[k])) {
                            char errMsg[MAX_ERROR_MSG_LENGTH];
                            sprintf(errMsg, "Invalid format specifier '%%%c' in printf", formatString[k]);
                            reportError(errMsg, tokens[formatStringIndex].line, tokens[formatStringIndex].column);
                            inFormatSpecifier = false;
                        }
                    }
                }
                
                // Count arguments (excluding the format string)
                int arguments = 0;
                int commaCount = 0;
                j = formatStringIndex + 1;
                
                while (j < tokenCount && strcmp(tokens[j].lexeme, ")") != 0) {
                    if (strcmp(tokens[j].lexeme, ",") == 0) {
                        commaCount++;
                    }
                    j++;
                }
                
                arguments = commaCount;
                
                // Check for mismatch in format specifiers and arguments
                if (formatSpecifiers > arguments) {
                    reportError("Too few arguments for printf format string", tokens[i].line, tokens[i].column);
                } else if (formatSpecifiers < arguments) {
                    reportError("Too many arguments for printf format string", tokens[i].line, tokens[i].column);
                }
                
                // Now check each argument with corresponding format specifier
                if (formatSpecifiers > 0) {
                    int currentArgIndex = formatStringIndex + 2; // Start after format string and comma
                    int currentFmtIndex = 0;
                    
                    for (int specifierIdx = 0; specifierIdx < formatSpecifiers; specifierIdx++) {
                        // Find next format specifier
                        while (currentFmtIndex < strlen(formatString)) {
                            if (formatString[currentFmtIndex] == '%' && 
                                (currentFmtIndex + 1 >= strlen(formatString) || 
                                 formatString[currentFmtIndex + 1] != '%')) {
                                break;
                            }
                            currentFmtIndex++;
                        }
                        
                        if (currentFmtIndex < strlen(formatString)) {
                            // Find the format specifier type
                            int specStart = currentFmtIndex;
                            currentFmtIndex++; // Skip the %
                            
                            // Skip modifiers
                            while (currentFmtIndex < strlen(formatString) && 
                                   !strchr("diuoxXfFeEgGaAcspn", formatString[currentFmtIndex])) {
                                currentFmtIndex++;
                            }
                            
                            char formatType = '\0';
                            if (currentFmtIndex < strlen(formatString)) {
                                formatType = formatString[currentFmtIndex];
                                currentFmtIndex++; // Move past this format specifier
                            }
                            
                            // Find and check the corresponding argument
                            while (currentArgIndex < tokenCount && 
                                   strcmp(tokens[currentArgIndex].lexeme, ",") != 0 && 
                                   strcmp(tokens[currentArgIndex].lexeme, ")") != 0) {
                                
                                // Check if the argument matches the format type
                                if (tokens[currentArgIndex].type == TOKEN_IDENTIFIER) {
                                    // If it's a variable, check if it's declared
                                    bool isDeclared = false;
                                    for (int v = 0; v < variableCount; v++) {
                                        if (strcmp(variables[v].name, tokens[currentArgIndex].lexeme) == 0) {
                                            isDeclared = true;
                                            
                                            // Check type compatibility
                                            if (formatType) {
                                                bool typeMatch = false;
                                                
                                                switch (formatType) {
                                                    case 'd': case 'i': case 'u': case 'o': case 'x': case 'X':
                                                        // Integer types
                                                        if (strcmp(variables[v].type, "int") == 0 || 
                                                            strcmp(variables[v].type, "short") == 0 || 
                                                            strcmp(variables[v].type, "long") == 0) {
                                                            typeMatch = true;
                                                        }
                                                        break;
                                                        
                                                    case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':
                                                        // Floating-point types
                                                        if (strcmp(variables[v].type, "float") == 0 || 
                                                            strcmp(variables[v].type, "double") == 0) {
                                                            typeMatch = true;
                                                        }
                                                        break;
                                                        
                                                    case 'c':
                                                        // Character type
                                                        if (strcmp(variables[v].type, "char") == 0) {
                                                            typeMatch = true;
                                                        }
                                                        break;
                                                        
                                                    case 's':
                                                        // String (char array) type
                                                        if (strcmp(variables[v].type, "char") == 0 && variables[v].isArray) {
                                                            typeMatch = true;
                                                        }
                                                        break;
                                                        
                                                    case 'p':
                                                        // Pointer type
                                                        // Simplified check
                                                        typeMatch = true;
                                                        break;
                                                }
                                                
                                                if (!typeMatch) {
                                                    char errMsg[MAX_ERROR_MSG_LENGTH];
                                                    sprintf(errMsg, "Format specifier '%%%c' incompatible with variable type '%s'", 
                                                            formatType, variables[v].type);
                                                    reportError(errMsg, tokens[currentArgIndex].line, tokens[currentArgIndex].column);
                                                }
                                            }
                                            
                                            break;
                                        }
                                    }
                                    
                                    if (!isDeclared) {
                                        char errMsg[MAX_ERROR_MSG_LENGTH];
                                        sprintf(errMsg, "Undefined variable '%s' used in printf", tokens[currentArgIndex].lexeme);
                                        reportError(errMsg, tokens[currentArgIndex].line, tokens[currentArgIndex].column);
                                    }
                                } 
                                // Check literals for compatibility
                                else if (tokens[currentArgIndex].type == TOKEN_NUMBER) {
                                    if (formatType == 's' || formatType == 'c') {
                                        char errMsg[MAX_ERROR_MSG_LENGTH];
                                        sprintf(errMsg, "Format specifier '%%%c' incompatible with numeric literal", formatType);
                                        reportError(errMsg, tokens[currentArgIndex].line, tokens[currentArgIndex].column);
                                    }
                                } 
                                else if (tokens[currentArgIndex].type == TOKEN_STRING) {
                                    if (formatType != 's') {
                                        char errMsg[MAX_ERROR_MSG_LENGTH];
                                        sprintf(errMsg, "Format specifier '%%%c' incompatible with string literal", formatType);
                                        reportError(errMsg, tokens[currentArgIndex].line, tokens[currentArgIndex].column);
                                    }
                                }
                                else if (tokens[currentArgIndex].type == TOKEN_CHAR) {
                                    if (formatType != 'c' && formatType != 'd') {
                                        char errMsg[MAX_ERROR_MSG_LENGTH];
                                        sprintf(errMsg, "Format specifier '%%%c' incompatible with character literal", formatType);
                                        reportError(errMsg, tokens[currentArgIndex].line, tokens[currentArgIndex].column);
                                    }
                                }
                                
                                currentArgIndex++;
                            }
                            
                            // Move to next argument
                            if (currentArgIndex < tokenCount && strcmp(tokens[currentArgIndex].lexeme, ",") == 0) {
                                currentArgIndex++;
                            }
                        }
                    }
                }
            }
        }
    }
} 

/**
 * Checks for common scanf function usage errors
 */
void checkScanfErrors() {
    printf("Checking for scanf errors...\n");
    
    for (int i = 0; i < tokenCount - 2; i++) {
        // Look for scanf function calls
        if (tokens[i].type == TOKEN_IDENTIFIER && 
            strcmp(tokens[i].lexeme, "scanf") == 0 &&
            i+1 < tokenCount && strcmp(tokens[i+1].lexeme, "(") == 0) {
            
            // Find the format string and parameters
            int j = i + 2;
            
            // Ensure scanf has parameters
            if (j < tokenCount && strcmp(tokens[j].lexeme, ")") == 0) {
                reportError("scanf called with no arguments", tokens[i].line, tokens[i].column);
                continue;
            }
            
            // Check if the first parameter is a string literal (format string)
            if (j < tokenCount && tokens[j].type != TOKEN_STRING) {
                reportError("scanf first argument should be a format string", tokens[j].line, tokens[j].column);
                continue;
            }
            
            // Extract the format string
            char formatString[MAX_IDENTIFIER_LENGTH];
            strncpy(formatString, tokens[j].lexeme + 1, strlen(tokens[j].lexeme) - 2); // Remove quotes
            formatString[strlen(tokens[j].lexeme) - 2] = '\0';
            
            // Count format specifiers in the format string
            int formatSpecCount = 0;
            char formatSpecifiers[MAX_IDENTIFIER_LENGTH];
            int formatSpecIndex = 0;
            
            for (int k = 0; k < strlen(formatString); k++) {
                if (formatString[k] == '%') {
                    // Skip %% which is literal %
                    if (k+1 < strlen(formatString) && formatString[k+1] == '%') {
                        k++;
                    } else {
                        formatSpecCount++;
                        // Capture the specifier type (d, f, c, s, etc.)
                        if (k+1 < strlen(formatString)) {
                            formatSpecifiers[formatSpecIndex++] = formatString[k+1];
                        }
                    }
                }
            }
            formatSpecifiers[formatSpecIndex] = '\0';
            
            // Count actual arguments after format string
            int argCount = 0;
            j++; // Move past format string
            
            if (j < tokenCount && strcmp(tokens[j].lexeme, ",") != 0) {
                reportError("Expected comma after format string in scanf", tokens[j].line, tokens[j].column);
            } else {
                j++; // Move past comma
            }
            
            // Count arguments (should be addresses with &, except for array names)
            while (j < tokenCount && strcmp(tokens[j].lexeme, ")") != 0) {
                if (tokens[j].type == TOKEN_OPERATOR && strcmp(tokens[j].lexeme, "&") == 0) {
                    if (j+1 < tokenCount && tokens[j+1].type == TOKEN_IDENTIFIER) {
                        // Check if the variable is declared
                        bool isDeclared = false;
                        for (int k = 0; k < variableCount; k++) {
                            if (strcmp(variables[k].name, tokens[j+1].lexeme) == 0) {
                                isDeclared = true;
                                
                                // Check if variable type matches format specifier
                                if (argCount < formatSpecIndex) {
                                    char specifier = formatSpecifiers[argCount];
                                    // Check type compatibility
                                    if ((specifier == 'd' || specifier == 'i') && 
                                        strcmp(variables[k].type, "int") != 0) {
                                        char message[MAX_ERROR_MSG_LENGTH];
                                        sprintf(message, "Format specifier %%d doesn't match variable type %s", variables[k].type);
                                        reportError(message, tokens[j+1].line, tokens[j+1].column);
                                    } else if (specifier == 'f' && 
                                              strcmp(variables[k].type, "float") != 0 && 
                                              strcmp(variables[k].type, "double") != 0) {
                                        char message[MAX_ERROR_MSG_LENGTH];
                                        sprintf(message, "Format specifier %%f doesn't match variable type %s", variables[k].type);
                                        reportError(message, tokens[j+1].line, tokens[j+1].column);
                                    } else if (specifier == 'c' && 
                                              strcmp(variables[k].type, "char") != 0) {
                                        char message[MAX_ERROR_MSG_LENGTH];
                                        sprintf(message, "Format specifier %%c doesn't match variable type %s", variables[k].type);
                                        reportError(message, tokens[j+1].line, tokens[j+1].column);
                                    } else if (specifier == 's' && 
                                              (strcmp(variables[k].type, "char") != 0 || !variables[k].isArray)) {
                                        reportError("Format specifier %s requires char array", tokens[j+1].line, tokens[j+1].column);
                                    }
                                }
                                
                                break;
                            }
                        }
                        
                        if (!isDeclared) {
                            char message[MAX_ERROR_MSG_LENGTH];
                            sprintf(message, "Undeclared variable '%s' used in scanf", tokens[j+1].lexeme);
                            reportError(message, tokens[j+1].line, tokens[j+1].column);
                        }
                        
                        argCount++;
                        j += 2; // Skip over & and variable name
                    } else {
                        reportError("Expected variable name after & in scanf", tokens[j].line, tokens[j].column);
                        j++;
                    }
                } else if (tokens[j].type == TOKEN_IDENTIFIER) {
                    // This might be an array name (no & needed)
                    bool isArray = false;
                    for (int k = 0; k < variableCount; k++) {
                        if (strcmp(variables[k].name, tokens[j].lexeme) == 0 && variables[k].isArray) {
                            isArray = true;
                            break;
                        }
                    }
                    
                    if (!isArray) {
                        reportError("Missing & operator for non-array variable in scanf", tokens[j].line, tokens[j].column);
                    }
                    
                    argCount++;
                    j++;
                } else if (tokens[j].type == TOKEN_SEPARATOR && strcmp(tokens[j].lexeme, ",") == 0) {
                    j++;
                } else {
                    j++;
                }
            }
            
            // Check if format specifier count matches argument count
            if (formatSpecCount != argCount) {
                char message[MAX_ERROR_MSG_LENGTH];
                sprintf(message, "scanf format specifier count (%d) doesn't match argument count (%d)", 
                        formatSpecCount, argCount);
                reportError(message, tokens[i].line, tokens[i].column);
            }
        }
        
        // Check for case sensitivity errors - "Scanf" instead of "scanf"
        if (tokens[i].type == TOKEN_IDENTIFIER && (
            strcmp(tokens[i].lexeme, "scanF") == 0 ||  strcmp(tokens[i].lexeme, "Scanf") == 0 || 
            strcmp(tokens[i].lexeme, "sCanf") == 0 || strcmp(tokens[i].lexeme, "scAnf") == 0 || 
            strcmp(tokens[i].lexeme, "scaNf") == 0 || strcmp(tokens[i].lexeme, "scanF") == 0 || 
            strcmp(tokens[i].lexeme, "SCanf") == 0 || strcmp(tokens[i].lexeme, "ScAnf") == 0 || 
            strcmp(tokens[i].lexeme, "ScaNf") == 0 ||  strcmp(tokens[i].lexeme, "ScanF") == 0 || 
            strcmp(tokens[i].lexeme, "sCAnf") == 0 || strcmp(tokens[i].lexeme, "sCaNf") == 0 || 
            strcmp(tokens[i].lexeme, "sCanF") == 0 ||  strcmp(tokens[i].lexeme, "scANf") == 0 || 
            strcmp(tokens[i].lexeme, "scAnF") == 0 ||  strcmp(tokens[i].lexeme, "scaNF") == 0 || 
            strcmp(tokens[i].lexeme, "SCAnf") == 0 ||  strcmp(tokens[i].lexeme, "SCaNf") == 0 || 
            strcmp(tokens[i].lexeme, "SCanF") == 0 ||  strcmp(tokens[i].lexeme, "ScANf") == 0 || 
            strcmp(tokens[i].lexeme, "ScAnF") == 0 ||  strcmp(tokens[i].lexeme, "ScaNF") == 0 || 
            strcmp(tokens[i].lexeme, "sCANf") == 0 || strcmp(tokens[i].lexeme, "sCAnF") == 0 || 
            strcmp(tokens[i].lexeme, "sCaNF") == 0 || strcmp(tokens[i].lexeme, "scANF") == 0 || 
            strcmp(tokens[i].lexeme, "SCANf") == 0 || strcmp(tokens[i].lexeme, "ScANF") == 0 || 
            strcmp(tokens[i].lexeme, "sCANF") == 0 || strcmp(tokens[i].lexeme, "SCANF") == 0)) 
            {
            
            reportError("Case sensitivity error: 'scanf' is lowercase in C", tokens[i].line, tokens[i].column);
        }
    }
}

/**
 * Checks if an identifier is a C keyword
 */
bool isKeyword(const char *identifier) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(identifier, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Checks if an identifier is a standard library function
 */
bool isStdLibFunction(const char *identifier) {
    for (int i = 0; i < sizeof(stdLibFunctions) / sizeof(stdLibFunctions[0]); i++) {
        if (strcmp(identifier, stdLibFunctions[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Get the size in bytes of a C data type
 */
int getTypeSize(const char *type) {
    if (strcmp(type, "char") == 0) {
        return 1;
    } else if (strcmp(type, "short") == 0) {
        return 2;
    } else if (strcmp(type, "int") == 0) {
        return 4;
    } else if (strcmp(type, "long") == 0) {
        return 8;
    } else if (strcmp(type, "float") == 0) {
        return 4;
    } else if (strcmp(type, "double") == 0) {
        return 8;
    } else if (strcmp(type, "void") == 0) {
        return 0;
    } else {
        return 4; // Default for unknown types
    }
}

/**
 * Reports an error, storing it in the errors array
 */
void reportError(const char *message, int line, int column) {
    if (errorCount < MAX_TOKENS) {
        strcpy(errors[errorCount].message, message);
        errors[errorCount].line = line;
        errors[errorCount].column = column;
        errorCount++;
        
        printf("Error at line %d, column %d: %s\n", line, column, message);
    }
}

/**
 * Prints the analysis results
 */
void printResults() {
    printf("\n=== C SYNTAX CHECKER RESULTS ===\n\n");
    
    // Print errors
    printf("ERRORS FOUND: %d\n", errorCount);
    if (errorCount > 0) {
        printf("---------------------------\n");
        for (int i = 0; i < errorCount; i++) {
            printf("Error #%d at line %d, column %d: %s\n", 
                   i + 1, errors[i].line, errors[i].column, errors[i].message);
        }
        printf("---------------------------\n\n");
    }
    
    // Print function statistics
    int userDefinedCount = 0;
    int userDefinedPrototypes = 0;
    int stdLibCount = 0;
    
    for (int i = 0; i < functionCount; i++) {
        if (functions[i].isUserDefined) {
            if (functions[i].isPrototype) {
                userDefinedPrototypes++;
            } else {
                userDefinedCount++;
            }
        } else {
            stdLibCount++;
        }
    }
    
    printf("FUNCTION ANALYSIS:\n");
    printf("---------------------------\n");
    printf("Total functions found: %d\n", functionCount);
    printf("User-defined functions: %d\n", userDefinedCount);
    printf("User-defined prototypes: %d\n", userDefinedPrototypes);
    printf("Standard library functions: %d\n", stdLibCount);
    printf("---------------------------\n\n");
    
    // Print function details
    printf("FUNCTION DETAILS:\n");
    printf("---------------------------\n");
    printf("%-30s %-15s %-10s %-10s\n", "Name", "Type", "Prototype", "Line");
    printf("---------------------------\n");
    for (int i = 0; i < functionCount; i++) {
        printf("%-30s %-15s %-10s %-10d\n", 
               functions[i].name, 
               functions[i].isUserDefined ? "User-defined" : "Standard Lib",
               functions[i].isPrototype ? "Yes" : "No",
               functions[i].line);
    }
    printf("---------------------------\n\n");
    
    // Print variable statistics
    printf("VARIABLE ANALYSIS:\n");
    printf("---------------------------\n");
    printf("Total variables found: %d\n", variableCount);
    
    int intCount = 0, charCount = 0, floatCount = 0, doubleCount = 0, otherCount = 0;
    int arrayCount = 0;
    
    for (int i = 0; i < variableCount; i++) {
        if (strcmp(variables[i].type, "int") == 0) {
            intCount++;
        } else if (strcmp(variables[i].type, "char") == 0) {
            charCount++;
        } else if (strcmp(variables[i].type, "float") == 0) {
            floatCount++;
        } else if (strcmp(variables[i].type, "double") == 0) {
            doubleCount++;
        } else {
            otherCount++;
        }
        
        if (variables[i].isArray) {
            arrayCount++;
        }
    }
    
    printf("  int variables: %d\n", intCount);
    printf("  char variables: %d\n", charCount);
    printf("  float variables: %d\n", floatCount);
    printf("  double variables: %d\n", doubleCount);
    printf("  other variables: %d\n", otherCount);
    printf("  array variables: %d\n", arrayCount);
    printf("---------------------------\n\n");
    
    // Print variable details
    printf("VARIABLE DETAILS:\n");
    printf("---------------------------\n");
    printf("%-20s %-10s %-10s %-10s %-10s\n", "Name", "Type", "Size", "Array", "Line");
    printf("---------------------------\n");
    for (int i = 0; i < variableCount; i++) {
        printf("%-20s %-10s %-10d %-10s %-10d\n", 
               variables[i].name, 
               variables[i].type, 
               variables[i].size * (variables[i].isArray ? variables[i].arraySize : 1),
               variables[i].isArray ? "Yes" : "No",
               variables[i].line);
    }
    printf("---------------------------\n\n");
    
    // Print token statistics
    int idCount = 0, kwCount = 0, numCount = 0, strCount = 0, charLitCount = 0, 
        opCount = 0, sepCount = 0, ppCount = 0, commCount = 0;
    
    for (int i = 0; i < tokenCount; i++) {
        switch (tokens[i].type) {
            case TOKEN_IDENTIFIER: idCount++; break;
            case TOKEN_KEYWORD: kwCount++; break;
            case TOKEN_NUMBER: numCount++; break;
            case TOKEN_STRING: strCount++; break;
            case TOKEN_CHAR: charLitCount++; break;
            case TOKEN_OPERATOR: opCount++; break;
            case TOKEN_SEPARATOR: sepCount++; break;
            case TOKEN_PREPROCESSOR: ppCount++; break;
            case TOKEN_COMMENT: commCount++; break;
            default: break;
        }
    }
    
    printf("TOKEN STATISTICS:\n");
    printf("---------------------------\n");
    printf("Total tokens: %d\n", tokenCount - 1); // Exclude EOF token
    printf("  Identifiers: %d\n", idCount);
    printf("  Keywords: %d\n", kwCount);
    printf("  Numbers: %d\n", numCount);
    printf("  String literals: %d\n", strCount);
    printf("  Character literals: %d\n", charLitCount);
    printf("  Operators: %d\n", opCount);
    printf("  Separators: %d\n", sepCount);
    printf("  Preprocessor directives: %d\n", ppCount);
    printf("  Comments: %d\n", commCount);
    printf("---------------------------\n\n");
    
    // Summary statistics
    printf("CODE SUMMARY:\n");
    printf("---------------------------\n");
    printf("Memory usage: %d bytes\n", 0); // Calculated in calculateMemoryUsage
    printf("Error density: %.2f errors per 100 tokens\n", 
           (float)errorCount / (tokenCount - 1) * 100);
    printf("Function to variable ratio: %.2f\n", 
           (float)functionCount / (variableCount > 0 ? variableCount : 1));
    printf("---------------------------\n");
    
    // Overall verdict
    printf("\nVERDICT: ");
    if (errorCount == 0) {
        printf("No syntax errors detected. Code appears to be syntactically correct.\n");
    } else if (errorCount < 5) {
        printf("Minor syntax issues detected. Consider addressing them before compilation.\n");
    } else {
        printf("Significant syntax issues detected. Please fix the errors before proceeding.\n");
    }
}