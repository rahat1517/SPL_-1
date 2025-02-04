#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

// Structure to store file lines with details
typedef struct {
    int lineNumber;
    int lineLength;
    char lineContent[MAX_LINE_LENGTH];
} fileline;

// Function to convert string to lowercase for case-insensitive comparison
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to check printf-related errors and suggest fixes
void check_printf_errors(fileline fl) {
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, fl.lineContent);
    to_lowercase(temp); // Convert to lowercase for case sensitivity check

    int line_number = fl.lineNumber;
    char *line = fl.lineContent;

    // Check if the line contains any printf-like function
    if (strstr(temp, "printf") != NULL || strstr(temp, "printff") != NULL || strstr(temp, "prntf") != NULL) {
        
        // Case sensitivity check
        if (strstr(line, "printf") == NULL) {
            printf("Error: 'printf' is case-sensitive, incorrect usage at line %d\n", line_number);
            printf("Fix: Use 'printf' in lowercase.\n");
        }

        // Check for incorrect spelling of printf
        if (strstr(temp, "printf") == NULL) {
            printf("Error: Possible misspelling of 'printf' at line %d\n", line_number);
            printf("Fix: Correct spelling to 'printf'.\n");
        }

        // Check if format specifier is missing
        if (strstr(line, "\"") == NULL) {
            printf("Error: Missing format specifier in printf at line %d\n", line_number);
            printf("Fix: Add a format specifier like \"%%d\", \"%%f\", etc.\n");
        }

        // Check for mismatched parentheses
        int openParen = 0, closeParen = 0;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(') openParen++;
            if (line[i] == ')') closeParen++;
        }
        if (openParen != closeParen) {
            printf("Error: Mismatched parentheses in printf at line %d\n", line_number);
            printf("Fix: Ensure correct '(' and ')' usage.\n");
        }

        // Check if semicolon is missing at the end
        int len = strlen(line);
        if (len > 0 && line[len - 2] != ';' && line[len - 1] != ';') {
            printf("Warning: Missing semicolon at the end of printf statement at line %d\n", line_number);
            printf("Fix: Add ';' at the end.\n");
        }

        // Data type mismatch check
        if (strstr(line, "%d") != NULL && strstr(line, "float") != NULL) {
            printf("Warning: Using '%%d' for float at line %d\n", line_number);
            printf("Fix: Use '%%f' for float variables.\n");
        }
        if (strstr(line, "%f") != NULL && strstr(line, "int") != NULL) {
            printf("Warning: Using '%%f' for int at line %d\n", line_number);
            printf("Fix: Use '%%d' for int variables.\n");
        }
    }
}

int main() {
    char filename[100];
    printf("Enter the C program filename: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return 1;
    }

    fileline fl;
    int line_number = 0;

    printf("\nChecking for printf-related errors...\n");

    while (fgets(fl.lineContent, sizeof(fl.lineContent), file)) {
        line_number++;
        fl.lineNumber = line_number;
        fl.lineLength = strlen(fl.lineContent);
        check_printf_errors(fl);
    }

    fclose(file);
    printf("Scan complete.\n");

    return 0;
}
