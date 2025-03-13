#include "regex_check.h"  // Custom header file

int main() {
    const char *test_cases[] = {
        "#iNclude <stdio.h>",   // ✅ সঠিক
        "#include <math.h>",    // ✅ সঠিক
        "#include<stdio.h>",    // ✅ সঠিক
        "#include<string.h>",   // ✅ সঠিক
        "#include \"myfile.h\"", // ✅ সঠিক
        "#include myfile.h"     // ❌ ভুল
    };

    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    for (int i = 0; i < num_cases; i++) {
        if (isInvalidInclude(test_cases[i])) {
            printf("❌ INVALID: %s\n", test_cases[i]);
        } else {
            printf("✔ VALID: %s\n", test_cases[i]);
        }
    }

    return 0;
}
