#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define MAX_LINE_LENGTH 1024

typedef struct{
    int lineNumber;
    int lineLength;
    char lineContent[MAX_LINE_LENGTH];
} fileline;

int readFile(fileline arr[]);
//int scanfCheck(char arr[], int length);
void keyword(fileline arr[], int len);
int printcheck(char arr[] , int length);
void printFile(fileline arr[], int length);
int specialCharacter(char arr[], int size);
int findCommentStart(char arr[], int length);
int calculateLength(char *arr);
//int scancheck( char arr[] , int length);
int fprintfcheck(char arr[] , int length);
void varCount(fileline arr[], int len);
int forcheck(char arr[], int size);
//void checkFileLines(fileline arr[], int length);
void checkFileFunctions(fileline arr[], int len);
void countBuiltInFunctions(fileline arr[], int length);


// Function to trim spaces
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace((unsigned char)str[start])) start++;
    while (end > start && isspace((unsigned char)str[end])) end--;
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

// Function to check if 'for' is written with incorrect case
int containsIncorrectFor(char *str) {
    if (strncasecmp(str, "for", 3) == 0 && strncmp(str, "for", 3) != 0) {
        return 1; // "for" exists, but incorrect case
    }
   /* if (strstr(str, "forr") != NULL || strstr(str, "foor") != NULL ||
     strstr(str, "fOor") != NULL || strstr(str, "fOOr") != NULL || 
     strstr(str, "fOOR") != NULL || strstr(str, "FOor") != NULL || 
     strstr(str, "FOOr") != NULL || strstr(str, "FOOR") != NULL || 
     strstr(str, "fOr") != NULL || strstr(str, "fOr") != NULL || 
     strstr(str, "fOR") != NULL || strstr(str, "fOR") != NULL || strstr(str, "FOR") != NULL) {
        return 1;
    } */
    if (strstr(str, "for") != NULL) {
        return 1; // 'for' exists but incorrect (e.g., 'ffor', 'foR', etc.)
    }
    return 0;
}

// Function to check if a string contains invalid operators like "==="
int containsInvalidOperators(char *str) {
    if (strstr(str, "===") != NULL) {
        return 1;
    }
    return 0;
}

// Function to check for loop syntax errors
void checkForLoopErrors(char *code, int lineNum) {
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, code);
    trim(temp);

    // Check if 'for' has incorrect case
    if (containsIncorrectFor(temp)) {
        printf("Error on line %d: Incorrect capitalization of 'for'. Use 'for' in lowercase.\n", lineNum);
    }

    // Check if 'for' is missing completely
    if (strstr(temp, "for") == NULL) {
        return;
    }

    // Check for missing parentheses
    char *start = strchr(temp, '(');
    char *end = strchr(temp, ')');
    if (!start || !end || end < start) {
        printf("Error on line %d: Missing or misplaced parentheses in 'for' loop.\n", lineNum);
        return;
    }

    // Extract inside of for loop
    char inside[MAX_LINE_LENGTH];
    strncpy(inside, start + 1, end - start - 1);
    inside[end - start - 1] = '\0';

    // Split into three parts
    char *part1 = strtok(inside, ";");
    char *part2 = strtok(NULL, ";");
    char *part3 = strtok(NULL, ";");

    // Check if all three parts exist
    if (!part1 || !part2 || !part3) {
        printf("Error on line %d: DEclaration", lineNum);
        return;
    }

    trim(part1);
    trim(part2);
    trim(part3);

    // Check for invalid operators in initialization
    if (containsInvalidOperators(part1)) {
        printf("Error on line %d: Invalid operator '===' used in initialization.\n", lineNum);
    }

    // Check if initialization is missing or incorrect
    if (strlen(part1) == 0) {
        printf("Error on line %d: Missing initialization in 'for' loop.\n", lineNum);
    } else if (!strchr(part1, '=')) {
        printf("Error on line %d: Initialization is incorrect in 'for' loop.\n", lineNum);
    }

    // Check if condition is missing or incorrect
    if (strlen(part2) == 0) {
        printf("Error on line %d: Missing condition in 'for' loop (infinite loop risk).\n", lineNum);
    } else if (!strpbrk(part2, "<>!=")) {
        printf("Error on line %d: Condition is incorrect in 'for' loop (missing comparison operator).\n", lineNum);
    }

    // Check if increment is missing or incorrect
    if (strlen(part3) == 0) {
        printf("Error on line %d: Missing increment/decrement in 'for' loop.\n", lineNum);
    } else if (!strpbrk(part3, "+-*/")) {
        printf("Error on line %d: Increment/decrement is incorrect in 'for' loop.\n", lineNum);
    }
}


void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}



void check_scanf_errors(const char *line, int line_number) {
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    to_lowercase(temp);

    if (strstr(temp, "scanf") != NULL || strstr(temp, "scanff") != NULL 
        || strstr(temp, "sanf") != NULL || strstr(temp, "scnaf") != NULL 
        || strstr(temp, "scaanff") != NULL || strstr(temp, "sCnff") != NULL 
        || strstr(temp, "scnff") != NULL || strstr(temp, "scnff") != NULL 
        || strstr(temp, "SCANF") != NULL || strstr(temp, "scnff") != NULL 
        || strstr(temp, "scnfff") != NULL || strstr(temp, "scnff") != NULL 
        || strstr(temp, "scnff") != NULL || strstr(temp, "scAnff") != NULL 
        || strstr(temp, "SCnfff") != NULL || strstr(temp, "scNFf") != NULL 
        || strstr(temp, "SCANF") != NULL || strstr(temp, "sCANff") != NULL 
        || strstr(temp, "SCANf") != NULL || strstr(temp, "scnaf") != NULL 
        || strstr(temp, "sCaNf") != NULL || strstr(temp, "scnff") != NULL 
        || strstr(temp, "scnff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "SCANF") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnfff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnff") != NULL || strstr(temp, "scAnff") != NULL
        || strstr(temp, "SCnfff") != NULL || strstr(temp, "scNFf") != NULL
        || strstr(temp, "SCANF") != NULL || strstr(temp, "sCANff") != NULL
        || strstr(temp, "SCANf") != NULL || strstr(temp, "scnaf") != NULL
        || strstr(temp, "sCaNf") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "SCANF") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnfff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnff") != NULL || strstr(temp, "scAnff") != NULL
        || strstr(temp, "SCnfff") != NULL || strstr(temp, "scNFf") != NULL
        || strstr(temp, "SCANF") != NULL || strstr(temp, "sCANff") != NULL
        || strstr(temp, "SCANf") != NULL || strstr(temp, "scnaf") != NULL
        || strstr(temp, "sCaNf") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "SCANF") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnfff") != NULL || strstr(temp, "scnff") != NULL
        || strstr(temp, "scnff") != NULL || strstr(temp, "scAnff") != NULL){
        if (strstr(line, "scanf") == NULL) {
            printf("Error: 'scanf' is case-sensitive, incorrect usage at line %d\n", line_number);
            printf("Fix: Use 'scanf' in lowercase.\n");
        }
        if (strstr(temp, "scanf") == NULL) {
            printf("Error: Possible misspelling of 'scanf' at line %d\n", line_number);
            printf("Fix: Correct spelling to 'scanf'.\n");
        }
        if (strstr(line, "\"") == NULL) {
            printf("Error: Missing format specifier in scanf at line %d\n", line_number);
            printf("Fix: Add a format specifier like \"%%d\", \"%%f\", etc.\n");
        }
        if (strstr(line, "&") == NULL && strstr(line, "%d") != NULL) {
            printf("Warning: Missing '&' for integer variable in scanf at line %d\n", line_number);
            printf("Fix: Use '&' before the variable (e.g., &var).\n");
        }
        int openParen = 0, closeParen = 0;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(') openParen++;
            if (line[i] == ')') closeParen++;
        }
        if (openParen != closeParen) {
            printf("Error: Mismatched parentheses in scanf at line %d\n", line_number);
            printf("Fix: Ensure correct '(' and ')' usage.\n");
        }
        int len = strlen(line);
        if (len > 0 && line[len - 2] != ';' && line[len - 1] != ';') {
            printf("Warning: Missing semicolon at the end of scanf statement at line %d\n", line_number);
            printf("Fix: Add ';' at the end.\n");
        }
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

void check_printf_errors(const char *line, int line_number) {
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    to_lowercase(temp);
    

    if (strstr(temp, "printf") != NULL || strstr(temp,"print")
        || strstr(temp,"Primtf") != NULL || strstr(temp,"Print") != NULL
        || strstr(temp,"prinf") != NULL || strstr(temp,"prinff") != NULL 
        || strstr(temp,"prinfff") != NULL || strstr(temp,"prinffff") != NULL
        || strstr(temp,"PRinf") != NULL || strstr(temp,"priff") != NULL
        || strstr(temp,"PRINTF") != NULL || strstr(temp,"PRINT") != NULL 
        || strstr(temp,"PRINFF") != NULL || strstr(temp,"PRINFFF") != NULL
        || strstr(temp,"PRintf") != NULL || strstr(temp,"prinF") != NULL
        || strstr(temp,"prinFf") != NULL || strstr(temp,"prinFff") != NULL
        || strstr(temp,"prinFfff") != NULL || strstr(temp,"PRinF") != NULL
        ||strstr(temp,"PrIntf") != NULL || strstr(temp,"prInF") != NULL
        || strstr(temp,"prInFf") != NULL || strstr(temp,"prInFff") != NULL
        || strstr(temp,"prInFfff") != NULL || strstr(temp,"PRInF") != NULL
        || strstr(temp,"PrinTF") != NULL || strstr(temp,"prinTF") != NULL
        || strstr(temp,"prinTFf") != NULL || strstr(temp,"prinTFff") != NULL
        || strstr(temp,"prinTFfff") != NULL || strstr(temp,"PRinTF") != NULL
        || strstr(temp,"PrIntF") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinTf") != NULL || strstr(temp,"prinTf") != NULL
        || strstr(temp,"prinTff") != NULL || strstr(temp,"prinTfff") != NULL
        || strstr(temp,"prinTffff") != NULL || strstr(temp,"PRinTf") != NULL
        || strstr(temp,"PrIntf") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInTf") != NULL
        || strstr(temp,"PrIntF") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"primtf") != NULL || strstr(temp,"PRInF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTF") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL
        || strstr(temp,"prinTfff") != NULL || strstr(temp,"prinTffff") != NULL
        || strstr(temp,"PRinT") != NULL || strstr(temp,"PrInt") != NULL
        || strstr(temp,"prInT") != NULL || strstr(temp,"prInTf") != NULL
        || strstr(temp,"prInTff") != NULL || strstr(temp,"prInTfff") != NULL
        || strstr(temp,"prInTffff") != NULL || strstr(temp,"PRInT") != NULL
        || strstr(temp,"PrInt") != NULL || strstr(temp,"prInTF") != NULL
        || strstr(temp,"prInTFf") != NULL || strstr(temp,"prInTFff") != NULL
        || strstr(temp,"prInTFfff") != NULL || strstr(temp,"PRInTF") != NULL
        || strstr(temp,"PrinT") != NULL || strstr(temp,"prinT") != NULL
        || strstr(temp,"prinTf") != NULL || strstr(temp,"prinTff") != NULL)  {
    if (strstr(line, "printf") == NULL) {
            printf("Error: 'printf' is case-sensitive, incorrect usage at line %d\n", line_number);
            printf("Fix: Use 'printf' in lowercase.\n");
        }

        
        
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

        // 3. Check for mismatched parentheses
        int openParen = 0, closeParen = 0;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(') openParen++;
            if (line[i] == ')') closeParen++;
        }
        if (openParen != closeParen) {
            printf("Error: Mismatched parentheses in printf at line %d\n", line_number);
            printf("Fix: Ensure correct '(' and ')' usage.\n");
        }


        int len = strlen(line);
        while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\n' || line[len - 1] == '\t')) {
            len--; 
        }
        if (len > 0 && line[len - 1] != ';') {
            printf("Warning: Missing semicolon at the end of printf statement at line %d\n", line_number);
            printf("Fix: Add ';' at the end.\n");
        }
    }
}

int main(){
    
    int option;
    FILE *input;
    fileline lines[100]; 
    char buffer[MAX_LINE_LENGTH]; 
    int lineCount = 0; 

   input = fopen("input.txt.txt","r");
   if(input == NULL){
       printf("Error : Could not open file.\n");
       return 1;
   }

   printf("\nChecking for scanf and printf-related errors...\n");

    while (fgets(buffer, MAX_LINE_LENGTH, input) != NULL) {
        if (buffer[0] == '\n') continue;
        
        lines[lineCount].lineNumber = lineCount + 1;
        lines[lineCount].lineLength = strlen(buffer);
        strcpy(lines[lineCount].lineContent, buffer);
        
    
        checkForLoopErrors(buffer, lineCount);
    

        // Check for errors
        check_scanf_errors(buffer, lineCount + 1);
        check_printf_errors(buffer, lineCount + 1);
        
        lineCount++;
    }
    

    fclose(input);
    printf("\nScan complete.\n");
    
    
   

   while(fgets(buffer, 100, input) != NULL) {
       int length = calculateLength(buffer);  
       int commentPos = findCommentStart(buffer, length); 

       if(buffer[0] == '\n'){
           continue;
       }

       lines[lineCount].lineNumber = lineCount + 1; 

       if(commentPos == -1){
           
           lines[lineCount].lineLength = length;
           strcpy(lines[lineCount].lineContent, buffer);
       } else {
           
           strncpy(lines[lineCount].lineContent, buffer, commentPos);
           lines[lineCount].lineContent[commentPos] = '\n'; 
           lines[lineCount].lineContent[commentPos + 1] = '\0'; 
           lines[lineCount].lineLength = strlen(lines[lineCount].lineContent);
       }
       
       lineCount++; 
   }

   fclose(input); 

   printf("\n __________________________________________________\n|");
   printf("          Syntax Checker - Main Menu             |\n");
   printf("|-------------------------------------------------|\n");
   printf("| 1. Check Syntax                                 |\n");
   printf("| 2. View Errors                                  |\n");
   printf("| 3. Analyze Code                                 |\n");
   printf("| 4. Exit                                         |\n");
   printf("|_________________________________________________|\n");

   printf("What do you want? Please choose an option\n");

   scanf("%d",&option);

   switch (option)
   {
   case 1:
        printf("You Selected: Check Syntax\n");
        //checkFileLines(lines, lineCount);
        checkFileFunctions(lines, lineCount);
        break;

    case 2:
        printf("You Selected: View Errors\n");
        break;

    case 3:
        printf("You Selected: Analyze Code\n");

        countBuiltInFunctions(lines, lineCount);

        printf("\nProcessed Lines:\n");

        for (int i = 0; i < lineCount; i++) {
             printf("Line %d (Length %d): %s", 
              lines[i].lineNumber, 
              lines[i].lineLength, 
              lines[i].lineContent);
        }
        break;

    case 4:
        printf("Exiting the program. Allah Hafez!\n");
        break;
   
    default:
        printf("Invalid Option. Please Choose a valid option from the menu\n");
        break;
   }
   printf("\n\n\n\t\t\tKEYWORDS with LINE NUMBER\n\n\n");
	keyword(lines,lineCount);
   

   return 0;
}

// Function to calculate the length of a string
int calculateLength(char *arr){
    int length = 0;
    while(arr[length] != '\0'){
        length++;
    }
    return length;
}

int findCommentStart(char arr[], int length){
    for(int i = 0; i < length - 1; i++) {
        if(arr[i] == '/' && arr[i + 1] == '/'){
            return i;
        }
    }
    return -1;
}

// Function to count built-in functions
void countBuiltInFunctions(fileline arr[], int length){
    char currentLine[1000], wordBuffer[100];
    char builtInFunctions[30][10] = {"printf", "scanf", "gets", "puts", "fscanf", "fprintf", "fgets", "fputs", "fputc", "fgetc", "fopen", "fclose", "feof", "fflush", "malloc", "calloc", "rand", "strcmp", "strlen", "strcpy", "strncpy", "strncmp", "tolower", "toupper", "strrev", "getch", "strcat",  "strncat", "sqrt", "pow"};
    int builtInCount = 0;

    for (int lineIndex = 0; lineIndex < length; lineIndex++) {
        strcpy(currentLine, arr[lineIndex].lineContent);
        int bufferIndex = 0;

        for (int charIndex = 0; charIndex <= arr[lineIndex].lineLength; charIndex++) {
            if (currentLine[charIndex] == ' ' || currentLine[charIndex] == '(' || 
                currentLine[charIndex] == '\0' || currentLine[charIndex] == '\n' || 
                currentLine[charIndex] == '\t') {

                if (bufferIndex > 0) {
                    wordBuffer[bufferIndex] = '\0'; 
                    for (int functionIndex = 0; functionIndex < 30; functionIndex++) {
                        if (strcmp(wordBuffer, builtInFunctions[functionIndex]) == 0) {
                            builtInCount++;
                        }
                    }
                    bufferIndex = 0; 
                }
            } else {
                wordBuffer[bufferIndex] = currentLine[charIndex];
                bufferIndex++;
            }
        }
    }

   printf("\nTotal Built-in Functions: %d\n", builtInCount);
}

int printcheck(char arr[], int length) {
    
    int pcount = 0;
    int ccount = 0;
    int icount = 0;
    int f = 0 ;

    for( int i = 0; i< length ; i++){
        if( arr[i] == '%'){
            pcount ++;
        }
        if(arr[i] == ','){
            ccount ++;
        }
        if(arr[i] == '\"'){
            icount ++;
        }
    }

    if(pcount != ccount){
        f =1;
    }
    if(icount % 2 != 0){
        f =1;
    }

    return f;
}

int checkWhileLoopSyntax(char arr[] , int length){

    int semicolonFlag = 0;
    int singleQuoteCount = 0;
    int doubleQuoteCount = 0;

    for(int i = 0; i < length; i++){
        if(arr[i]== ';'){
            semicolonFlag = 1;
        }

        if(arr[i] =='\''){
            singleQuoteCount++;
        }

        if(arr[i] == '"'){
            doubleQuoteCount++;
        }

        if(singleQuoteCount % 2 != 0 || doubleQuoteCount % 2 != 0){
            semicolonFlag = 1;
        }
    }

    return semicolonFlag;
}

void checkFileFunctions(fileline arr[], int len) {
    
    const char keywords[8][10] = {"printf", "scanf", "gets", "puts", "fscanf", "fprintf", "for", "while"};
    char word[100];
    char line[100];

    for (int i = 0; i < len; i++) {
        strcpy(line, arr[i].lineContent);
        int wordIndex = 0;               

        for (int j = 0; j <= arr[i].lineLength; j++) {
            
            if (line[j] == ' ' || line[j] == '(' || line[j] == '\0' || line[j] == '\t') {
                word[wordIndex] = '\0'; // 

                
                for (int k = 0; k < 8; k++) {
                    if (strcmp(word, keywords[k]) == 0) {
                        int errorFlag = 0;

                        if (strcmp(word, "printf") == 0) {
                            errorFlag = printcheck(line, arr[i].lineLength);
                        } 
    
                      /* else if (strcmp(word, "scanf") == 0) {
                            errorFlag = scanfCheck(line, arr[i].lineLength);
                        } */
                        
                        else if (strcmp(word, "fprintf") == 0) {
                            errorFlag = fprintfcheck(line, arr[i].lineLength);
                        } 

                         else if (strcmp(word, "for") == 0) {
                            errorFlag = forcheck(line, arr[i].lineLength);
                        } 

                        else if (strcmp(word, "while") == 0) {
                            errorFlag = checkWhileLoopSyntax(line, arr[i].lineLength);
                        } 

                        /*else if (strcmp(word, "gets") == 0) {
                            errorFlag = getscheck(line, arr[i].lineLength);
                        } 

                        else if (strcmp(word, "puts") == 0) {
                            errorFlag = putscheck(line, arr[i].lineLength);
                        }

                        else if (strcmp(word, "fscanf") == 0) {
                            errorFlag = fscanfcheck(line, arr[i].lineLength);
                        } 

                        

                         */

                    }
                }

                wordIndex = 0; 
            } 
            else {
                word[wordIndex++] = line[j]; 
            }
        }
    }
} 


int fprintfcheck(char arr[] , int length){

    int pcount = 0;
    int ccount = 0;
    int icount = 0;
    int f = 0;
    int i ;

    for( i = 0; i < length ; i++){
        
        if(arr[i] == '%'){
            pcount++;
        }

        if(arr[i]==','){
            ccount++;
        }

        if(arr[i] = '\"'){
            icount++;
        }

    }

    if(pcount != ccount - 1){
        f = 1;
    }

    if(icount % 2 !=0){
        f = 1;
    }

    return f;
}

int forcheck(char arr[], int size) {
    int semicolonCount = 0;  
    int quoteCount = 0;      
    int errorFlag = 0;       

    for (int i = 0; i < size; i++) {
        
        if (arr[i] == ';') {
            semicolonCount++;
        }
        
        if (arr[i] == '\'') {
            quoteCount++;
        }

        if(arr[i] =='f' && arr[i+1] == 'o' && arr[i+2] != 'r'){
        errorFlag = 1;
        }

        else if(arr[i] != 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] != '('){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] != 'o' && arr[i+2] == 'r' && arr[i+3] != '('){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] != ' '){
            errorFlag = 1;
        }

        else if(arr[i] != 'f' && arr[i+1] == 'o' && arr[i+2] != 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] != 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }   

        else if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] != 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] != 'n'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] != 't'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] != ' '){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] != 'i'){
            errorFlag = 1;
        }  

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] != 'i'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] != ' '){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] != '=' ){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] != '0'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] != ';' ){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] != '<'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] != 'n'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] != ';'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] != ' '){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] == ' ' && arr[i+20] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] == ' ' && arr[i+20] == 'i' && arr[i+21] != ' '){
            errorFlag = 1;
        }

    }

    if (semicolonCount != 2) {
        errorFlag = 1;
    }

    if (quoteCount % 2 != 0) {
        errorFlag = 1;
    }
    
    return errorFlag;
}
 
void keyword(fileline arr[], int len) {
    
    char keywords[32][10] = {
        "auto", "double", "int", "struct", "break", "else", "long", "switch",
        "case", "enum", "register", "typedef", "char", "extern", "return", "union",
        "const", "float", "short", "unsigned", "continue", "for", "signed", "void",
        "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"
    };
    char str[100], word[100];
        int k = 0; 
    
    
    for (int i = 0; i < len; i++) {
        
        strcpy(str, arr[i].lineContent);

        for (int j = 0; j <= arr[i].lineLength; j++) {
            
            if (str[j] == ' ' || str[j] == '(' || str[j] == '\t' || str[j] == '\0') {
                if (k > 0) { 
                    word[k] = '\0'; 
                    
                    
                    for (int l = 0; l < 32; l++) {
                        if (strcmp(word, keywords[l]) == 0) {
                            printf("Line %d: %s\n", arr[i].lineNumber, word);
                        }
                    }
                    k = 0; 
                }
            } else {
            
                word[k++] = str[j];
            }
        }
    }
}
