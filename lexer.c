#include "lexer.h"

// Lexer Function Implementation
TokenType lexer(char *line, char **token) {
    static const char *keywords[] = {
        "int", "float", "char", "void", "if", "else", "while", "for", "return",
        "include", "define", "struct", "printf", "scanf"
    };
    static const TokenType keywordTokens[] = {
        TOKEN_INT, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_VOID, TOKEN_IF, TOKEN_ELSE,
        TOKEN_WHILE, TOKEN_FOR, TOKEN_RETURN, TOKEN_INCLUDE, TOKEN_DEFINE,
        TOKEN_STRUCT, TOKEN_PRINTF, TOKEN_SCANF
    };

    // Skip whitespace
    while (isspace(*line)) line++;
    // Check for #include directive
    if (strncmp(line, "#include", 8) == 0) {
        *token = strdup("#include");
        return TOKEN_INCLUDE;
    }

    // Check for return keyword
    if (strncmp(line, "return", 6) == 0 && !isalnum(line[6])) {
        *token = strdup("return");
        return TOKEN_RETURN;
    }

    // Check for symbols
    if (*line == '}') {
        *token = strdup("}");
        return TOKEN_RBRACE;
    }

    // Check for keywords
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strncmp(line, keywords[i], strlen(keywords[i])) == 0 &&
            !isalnum(line[strlen(keywords[i])])) {
            *token = strdup(keywords[i]);
            return keywordTokens[i];
        }
    }

    // Check for identifiers
    if (isalpha(*line) || *line == '_') {
        *token = malloc(NAME_LENGTH);
        int i = 0;
        while (isalnum(line[i]) || line[i] == '_') {
            (*token)[i] = line[i];
            i++;
        }
        (*token)[i] = '\0';
        return TOKEN_IDENTIFIER;
    }

    // Check for numbers
    if (isdigit(*line)) {
        *token = malloc(NAME_LENGTH);
        int i = 0;
        while (isdigit(line[i])) {
            (*token)[i] = line[i];
            i++;
        }
        (*token)[i] = '\0';
        return TOKEN_NUMBER;
    }

    // Check for operators and symbols
    switch (*line) {
        case '=': *token = strdup("="); return TOKEN_ASSIGN;
        case ';': *token = strdup(";"); return TOKEN_SEMICOLON;
        case '(': *token = strdup("("); return TOKEN_LPAREN;
        case ')': *token = strdup(")"); return TOKEN_RPAREN;
        case '{': *token = strdup("{"); return TOKEN_LBRACE;
        case '}': *token = strdup("}"); return TOKEN_RBRACE;
        case ',': *token = strdup(","); return TOKEN_COMMA;
        case '[': *token = strdup("["); return TOKEN_LBRACKET;
        case ']': *token = strdup("]"); return TOKEN_RBRACKET;
        case '+': *token = strdup("+"); return TOKEN_PLUS;
        case '-': *token = strdup("-"); return TOKEN_MINUS;
        case '*': *token = strdup("*"); return TOKEN_MULT;
        case '/': *token = strdup("/"); return TOKEN_DIV;
        case '<': *token = strdup("<"); return TOKEN_LT;
        case '>': *token = strdup(">"); return TOKEN_GT;
        case '&': *token = strdup("&&"); return TOKEN_AND;
        case '|': *token = strdup("||"); return TOKEN_OR;
        case '!': *token = strdup("!="); return TOKEN_NEQ;
        case '"': *token = strdup("\""); return TOKEN_STRING;
    }

    // Unknown token
    *token = strdup(line);
    return TOKEN_UNKNOWN;
}