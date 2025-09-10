%{
#include "ast.h"
#include "microc.tab.h"
#include <stdlib.h>
#include <string.h>
extern void yyerror(const char *s);
int yywrap(void) {
    return 1;
}
%}
%%
"int"         { return INT; }
"void"        { return VOID; }
"if"          { return IF; }
"else"        { return ELSE; }
"while"       { return WHILE; }
"for"         { return FOR; }
"return"      { return RETURN; }
[a-zA-Z][a-zA-Z0-9]* { yylval.identifier = strdup(yytext); return IDENTIFIER; }
[0-9]+        { yylval.number = atoi(yytext); return NUMBER; }
"+"           { return PLUS; }
"-"           { return MINUS; }
"*"           { return MULT; }
"/"           { return DIVIDE; }
"="           { return ASG_OP; }
"=="          { return EQ_OP; }
"!="          { return NOT_EQ_OP; }
"<"           { return LESS_THAN_OP; }
">"           { return GREATER_THAN_OP; }
"<="          { return LESS_EQ_OP; }
">="          { return GREATER_EQ_OP; }
"&&"          { return AND_OP; }
"||"          { return OR_OP; }
"!"           { return NOT_OP; }
"("           { return LPAR; }
")"           { return RPAR; }
"{"           { return LBRACE; }
"}"           { return RBRACE; }
";"           { return SCOLON; }
","           { return COMMA; }
[ \t\n]+      ;
.             { fprintf(stderr, "Carattere non riconosciuto: %s\n", yytext); }
%%
