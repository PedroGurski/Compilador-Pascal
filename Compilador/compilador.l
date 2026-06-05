%option yylineno 
%option noyywrap
%option case-insensitive

%{
#include "ast.h"
#include "compilador.tab.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern void yyerror(const char *s);

%}

DIGITO [0-9]
LETRA [a-zA-Z]

%%

[ \t\r\n]+          { /* ignorar espacos em branco */ }

"program"         { return PROGRAM; }
"var"             { return VAR; }
"integer"         { return INTEIRO; }
"real"            { return REAL; }
"function"        { return FUNCTION; }
"procedure"       { return PROCEDURE; }
"begin"           { return BEGIN_TOKEN; }
"end"             { return END; }
"if"              { return IF; }
"then"            { return THEN; }
"else"            { return ELSE; }
"while"           { return WHILE; }
"do"              { return DO; }

":="              { return OPERADOR_ATRIBUICAO; }

"="|"<>"|">"|">="|"<"|"<=" { 
    yylval.str_val = strdup(yytext);
    return OPERADOR_RELACIONAL; 
}

"+"               { return MAIS; }
"-"               { return MENOS; }
"or"              { return OR; }

"*"|"/"|"div"|"mod"|"and" { 
    yylval.str_val = strdup(yytext);
    return OPERADOR_MULTIPLICATIVO; 
}

"("               { return ABRE_PARENTESES; }
")"               { return FECHA_PARENTESES; }
"."               { return PONTO_FINAL; }
";"               { return PONTO_VIRGULA; }
","               { return VIRGULA; }
":"               { return DOIS_PONTOS; }

{DIGITO}+("."{DIGITO}+)?([Ee][+-]?{DIGITO}+)? {
    yylval.str_val = strdup(yytext);
    return NUM;
}

{LETRA}({LETRA}|{DIGITO})* {
    for (int i = 0; yytext[i]; i++) {
        yytext[i] = tolower(yytext[i]);
    }
    yylval.str_val = strdup(yytext);
    return ID;
}

.                 { printf("Erro lexico na linha %d: %s\n", yylineno, yytext); exit(1); }

%%
