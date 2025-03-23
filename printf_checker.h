#ifndef PRINTF_CHECKER_H
#define PRINTF_CHECKER_H

#include <stdbool.h>

// Maximum length of a line in the input file
#define MAX_LINE_LENGTH 1024

// Function prototypes

/**
 * @brief Checks for case-sensitive mismatches in the `printf` keyword.
 * 
 * @param line The input line to check.
 * @return true If a case-sensitive mismatch is found.
 * @return false If the `printf` keyword is correct.
 */
bool isPrintfCaseSensitiveError(const char *line);

/**
 * @brief Checks for additional variations of `printf` (e.g., `printf_s`, `sprintf`, `fprintf`).
 * 
 * @param line The input line to check.
 * @return true If an unsupported `printf` variation is found.
 * @return false If no unsupported variation is found.
 */
bool isUnsupportedPrintfVariation(const char *line);

/**
 * @brief Validates the format string of a `printf` statement.
 * 
 * @param line The input line containing the `printf` statement.
 * @param lineNumber The line number in the input file.
 * @return int 0 if the format string is valid, otherwise an error code.
 */
int validatePrintfFormat(const char *line, int lineNumber);

/**
 * @brief Checks for invalid escape sequences in the format string.
 * 
 * @param line The input line to check.
 * @param lineNumber The line number in the input file.
 * @return int 0 if no invalid escape sequences are found, otherwise an error code.
 */
int checkEscapeSequences(const char *line, int lineNumber);

/**
 * @brief Checks for unbalanced quotes in the format string.
 * 
 * @param line The input line to check.
 * @param lineNumber The line number in the input file.
 * @return int 0 if the format string is balanced, otherwise an error code.
 */
int checkUnbalancedQuotes(const char *line, int lineNumber);
bool isPrintfTypo(const char *line);

#endif // PRINTF_CHECKER_H