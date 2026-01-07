%{
#include "parser.tab.h"
%}

%option noyywrap
%option yylineno

%%

"PUSH"      { return T_PUSH; }
"POP"       { return T_POP; }
"DUP"       { return T_DUP; }
"HALT"      { return T_HALT; }
"ADD"       { return T_ADD; }
"SUB"       { return T_SUB; }
"MUL"       { return T_MUL; }
"DIV"       { return T_DIV; }
"CMP"       { return T_CMP; }
"AND"       { return T_AND; }
"OR"        { return T_OR; }
"XOR"       { return T_XOR; }
"NOT"       { return T_NOT; }
"SHL"       { return T_SHL; }
"SHR"       { return T_SHR; }
"JMP"       { return T_JMP; }
"JZ"        { return T_JZ; }
"JNZ"       { return T_JNZ; }
"STORE"     { return T_STORE; }
"LOAD"      { return T_LOAD; }
"CALL"      { return T_CALL; }
"RET"       { return T_RET; }

[ \t]+      { /* ignore whitespace */ }
\n          { return '\n'; }
;.*         { /* ignore comments */ }

.           { fprintf(stderr, "Line %d: Unexpected character '%c'\n", yylineno, *yytext); }

%%