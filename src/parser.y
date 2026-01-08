%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;
extern FILE *yyin;

int pc = 0;

void emit_byte(unsigned char byte) {
    // In a real assembler, this would write to a file or buffer
    printf("0x%02x ", byte); // Corrected: escaped the double quote inside printf
    pc++;
}

void emit_int(int value) {
    // In a real assembler, this would write to a file or buffer
    printf("0x%08x ", value); // Corrected: escaped the double quote inside printf
    pc += 4;
}

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
    T_PUSH T_INTEGER { emit_byte(0x01); emit_int($2); } 
    | T_POP { emit_byte(0x02); } 
    | T_DUP { emit_byte(0x03); } 
    | T_HALT { emit_byte(0xFF); } 
    | T_ADD { emit_byte(0x10); } 
    | T_SUB { emit_byte(0x11); } 
    | T_MUL { emit_byte(0x12); } 
    | T_DIV { emit_byte(0x13); } 
    | T_CMP { emit_byte(0x14); } 
    | T_AND { emit_byte(0x15); } 
    | T_OR { emit_byte(0x16); } 
    | T_XOR { emit_byte(0x17); } 
    | T_NOT { emit_byte(0x18); } 
    | T_SHL { emit_byte(0x19); } 
    | T_SHR { emit_byte(0x1A); } 
    | T_JMP T_ID { emit_byte(0x20); /* handle label later */ pc += 4; } 
    | T_JZ T_ID { emit_byte(0x21); /* handle label later */ pc += 4; } 
    | T_JNZ T_ID { emit_byte(0x22); /* handle label later */ pc += 4; } 
    | T_STORE T_INTEGER { emit_byte(0x30); emit_int($2); } 
    | T_LOAD T_INTEGER { emit_byte(0x31); emit_int($2); } 
    | T_CALL T_ID { emit_byte(0x40); /* handle label later */ pc += 4; } 
    | T_RET { emit_byte(0x41); } 
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