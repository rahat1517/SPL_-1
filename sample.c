#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 1024

bool isValidPrintf(const char *line, int line_number) {
    // Basic check for printf existence
    char *printfPos = strstr(line, "printf");
    if (!printfPos) return true; // If no printf, it's valid
    
    // Allow printf containing scanf
    if (strstr(printfPos, "scanf")) return true;
    
    // Check for case-sensitive printf usage
    if (strstr(line, "prinTff") != NULL) {
        printf("Error: 'printf' is case-sensitive, incorrect usage at line %d\n", line_number);
        printf("Fix: Use 'printf' in lowercase.\n");
    }
    
    // Check for correct printf syntax
    char *start = strchr(line, '"');  // Find first "
    char *end = strrchr(line, '"');   // Find last "
    if (start && end && start != end) { // Ensure valid format string
        int contains_percent = strstr(start, "%") != NULL; 
        char *after_quote = end + 1; 

        // Detect missing comma after format string
        if (after_quote && *after_quote != ')' && *after_quote != ';' && *after_quote != ',' && *after_quote != '\0') {
            printf("Error: Missing comma between format string and arguments in printf at line %d\n", line_number);
            printf("Fix: Add a comma after the format string.\n");
        }

        // Detect missing format specifier when arguments are present
        char *comma = strchr(end, ',');
        if (comma && !contains_percent) {
            printf("Error: Missing format specifier in printf at line %d\n", line_number);
            printf("Fix: Use a format specifier like \"%%d\", \"%%f\", etc., when passing variables.\n");
        }
    } else {
        printf("Error: Missing or incomplete format string in printf at line %d\n", line_number);
        printf("Fix: Ensure printf contains a valid format string in double quotes.\n");
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

    // Check for missing semicolon
    int len = strlen(line);
    while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\n' || line[len - 1] == '\t')) {
        len--; 
    }
    if (len > 0 && line[len - 1] != ';') {
        printf("Warning: Missing semicolon at the end of printf statement at line %d\n", line_number);
        printf("Fix: Add ';' at the end.\n");
    }

    return false;
}

void checkFileSyntax(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    int lineNumber = 0;
    while (fgets(line, MAX_LINE, file)) {
        lineNumber++;
        if (!isValidPrintf(line, lineNumber)) {
            printf("Syntax Error in printf at line %d: %s", lineNumber, line);
        }
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    checkFileSyntax(argv[1]);
    return 0;
}
