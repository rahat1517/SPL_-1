#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

// Function to check if the #include statement is valid using regex
bool isValidInclude(const char *line) {
    regex_t regex;
    int reti;

    // Valid patterns:
    // 1. #include<stdio.h>
    // 2. #include "stdio.h"
    const char *pattern = "^#include[[:space:]]*[<\"]([a-zA-Z0-9_./]+)[>\"]$";

    // Compile regex
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return false;
    }

    // Match regex
    reti = regexec(&regex, line, 0, NULL, 0);
    regfree(&regex);

    return (reti == 0);
}

// Function to check if the main function is valid using regex
bool isValidMainFunction(const char *line) {
    regex_t regex;
    int reti;

    // Valid pattern:
    // 1. int main() {
    // 2. int main (void) {
    const char *pattern = "^[[:space:]]*(?i:int)[[:space:]]+main[[:space:]]*\\([[:space:]]*(void)?[[:space:]]*\\)[[:space:]]*\\{";

    // Compile regex
    reti = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return false;
    }

    // Match regex
    reti = regexec(&regex, line, 0, NULL, 0);
    regfree(&regex);

    return (reti == 0);
}

int main() {
    // Example test cases
    char *testIncludes[] = {
        "#include<stdio.h>",
        "#include <stdio.h>",
        "#include \"myheader.h\"",
        "# include <stdio.h>",   // Invalid
        "#include stdio.h",       // Invalid
        "#include<>",             // Invalid
        "#include\"stdio.h",      // Invalid
        "#include<stdio.h> extra" // Invalid
    };

    char *testMains[] = {
        "int main() {",
        "int main(void) {",
        "Int Main() {",     // Invalid
        "intmain() {",      // Invalid
        "void main() {",    // Invalid
        "int main) {",      // Invalid
        "int main() ;",     // Invalid
        "int main( {",      // Invalid
        "int main(){}",     // Valid (single-line function body)
    };

    printf("Checking #include statements:\n");
    for (int i = 0; i < sizeof(testIncludes) / sizeof(testIncludes[0]); i++) {
        printf("%s --> %s\n", testIncludes[i], isValidInclude(testIncludes[i]) ? "Valid" : "Invalid");
    }

    printf("\nChecking main function declarations:\n");
    for (int i = 0; i < sizeof(testMains) / sizeof(testMains[0]); i++) {
        printf("%s --> %s\n", testMains[i], isValidMainFunction(testMains[i]) ? "Valid" : "Invalid");
    }

    return 0;
}
