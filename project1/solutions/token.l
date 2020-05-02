%{
#include <stdio.h>
#include <string>
#include "IR.h"
using namespace Boost::Internal;
#include "grammar.h"

/* Put value onto Bison stack. */
#define SAVE_TOKEN  yylval.string = new std::string(yytext)
#define TOKEN(t)    (yylval.token = t)

static FILE *yyparse_file_ptr;

void set_yyparse_file_ptr(FILE *file_ptr){
    yyparse_file_ptr = file_ptr;
    yyin = yyparse_file_ptr;
}

%}

/* don't invoke function "yywrap" when encountering EOF */
%option noyywrap

/* regular definitions */
delim       [ \t\r\n]
ws          {delim}+
letter      [a-zA-Z]
digit       [0-9]
id          [a-zA-Z_][a-zA-Z0-9_]*
float       {digit}+\.{digit}+
integer     {digit}+

%%

{ws}                        { /* no action and no return */ }

{id}                        { SAVE_TOKEN; return TIDENTIFIER; }
{float}                     { SAVE_TOKEN; return TFLOAT; }
{integer}                   { SAVE_TOKEN; return TINTEGER; }

";"                         { return TOKEN(TSEMICOLON); }
"="                         { return TOKEN(TEQUAL); }
","                         { return TOKEN(TCOMMA); }
"["                         { return TOKEN(TLBRACKET); }
"]"                         { return TOKEN(TRBRACKET); }
"("                         { return TOKEN(TLPAREN); }
")"                         { return TOKEN(TRPAREN); }
"<"                         { return TOKEN(TCLT); }
">"                         { return TOKEN(TCGT); }
"+"                         { return TOKEN(TPLUS); }
"-"                         { return TOKEN(TMINUS); }
"*"                         { return TOKEN(TMUL); }
"//"                        { return TOKEN(TINTDIV); }
"/"                         { return TOKEN(TDIV); }
"%"                         { return TOKEN(TMOD); }
.                           { fprintf(stderr, "Unknown token:%s\n", yytext); yyterminate();}

%%