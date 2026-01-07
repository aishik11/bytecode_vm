%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;
extern FILE *yyin;

%}

%union {
    int ival;
    char *sval;
}

%token <sval> T_ID 
%token <ival> T_INTEGER 

%token T_PUSH T_POP T_DUP T_HALT 
%token T_ADD T_SUB T_MUL T_DIV T_CMP 
%token T_AND T_OR T_XOR T_NOT T_SHL T_SHR 
%token T_JMP T_JZ T_JNZ 
%token T_STORE T_LOAD 
%token T_CALL T_RET
%token <sval> T_LABEL

%type <sval> label_def 

%%

program:
    | program line 
    ;

line:
    '\n' 
    | instruction '\n' 
    | label_def '\n' 
    ;

label_def:
    T_LABEL { $$ = $1; } 
    ;

instruction:
    T_PUSH T_INTEGER 
    | T_POP 
    | T_DUP 
    | T_HALT 
    | T_ADD 
    | T_SUB 
    | T_MUL 
    | T_DIV 
    | T_CMP 
    | T_AND 
    | T_OR 
    | T_XOR 
    | T_NOT 
    | T_SHL 
    | T_SHR 
    | T_JMP T_ID 
    | T_JZ T_ID 
    | T_JNZ T_ID 
    | T_STORE T_INTEGER 
    | T_LOAD T_INTEGER 
    | T_CALL T_ID 
    | T_RET 
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", yylineno, s); 
}

int main(int argc, char **argv) {
    if (argc < 2) { 
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]); 
        return 1;
    }

    yyin = fopen(argv[1], "r"); 
    if (!yyin) { 
        perror(argv[1]); 
        return 1;
    }

    yyparse(); 
    fclose(yyin); 
    return 0;
}