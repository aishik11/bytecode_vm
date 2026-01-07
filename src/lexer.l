%{
#include "parser.tab.h"
%}

%option noyywrap
%option yylineno

%%

[ 	]+      { /* ignore whitespace */ }
\n          { return '\n'; }
;.*         { /* ignore comments */ }

.           { fprintf(stderr, "Line %d: Unexpected character '%c'\n", yylineno, *yytext); }

%%