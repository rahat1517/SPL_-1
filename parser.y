%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.c"

int yylex();
void yyerror(const char *s);
extern int yylineno;
int current_scope = 0;
char current_function_type[NAME_LENGTH];
%}

%token INT FLOAT CHAR VOID IF ELSE WHILE FOR RETURN INCLUDE HASH DEFINE STRUCT
%token NUMBER IDENTIFIER ASSIGN SEMICOLON LPAREN RPAREN LBRACE RBRACE COMMA
%token LBRACKET RBRACKET PLUS MINUS MULT DIV LT GT EQ NEQ AND OR

%%
program:
    program statement
    | /* empty */
    ;

statement:
    declaration
    | function_definition
    | function_call
    | preprocessor
    ;

declaration:
    type IDENTIFIER SEMICOLON {
        if (!addSymbol($2, $1, current_scope, 0, 0, NULL)) {
            printf("Error: Variable '%s' already declared at line %d\n", $2, yylineno);
        }
    }
    ;

type:
    INT | FLOAT | CHAR | VOID
    ;

function_definition:
    type IDENTIFIER LPAREN parameter_list RPAREN LBRACE program RBRACE {
        strcpy(current_function_type, $1);
        if (!addSymbol($2, $1, 0, 1, $4, (char(*)[NAME_LENGTH])$5)) {
            printf("Error: Function '%s' already declared at line %d\n", $2, yylineno);
        }
        current_scope = 0;
    }
    ;

parameter_list:
    /* empty */ { $$ = 0; }
    | parameter_list_item { $$ = 1; }
    | parameter_list COMMA parameter_list_item { $$ = $1 + 1; }
    ;

parameter_list_item:
    type IDENTIFIER {
        addSymbol($2, $1, 1, 0, 0, NULL);
    }
    ;

function_call:
    IDENTIFIER LPAREN argument_list RPAREN SEMICOLON {
        Symbol func;
        if (!lookupSymbol($1, 0, &func)) {
            printf("Error: Function '%s' not declared at line %d\n", $1, yylineno);
        } else if (func.isFunction) {
            if ($3 != func.paramCount) {
                printf("Error: Function '%s' expects %d arguments, but %d provided at line %d\n",
                       $1, func.paramCount, $3, yylineno);
            }
        }
    }
    ;

argument_list:
    /* empty */ { $$ = 0; }
    | expression { $$ = 1; }
    | argument_list COMMA expression { $$ = $1 + 1; }
    ;

expression:
    NUMBER
    | IDENTIFIER
    | IDENTIFIER LBRACKET expression RBRACKET
    | expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    ;
%%
int main() {
    yyparse();
    printSymbolTable();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
