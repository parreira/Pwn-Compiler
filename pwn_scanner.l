%option c++ yyclass="pwn_scanner" outfile="pwn_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: pwn_scanner.l,v 1.27 2015/05/18 17:36:37 ist175814 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <iostream>
#include "pwn_scanner.h"
#include "pwn_parser.tab.h"

// don't change this
#define yyerror LexerError

%}
HEXADECIMAL 	0x[0-9a-fA-F]+
DECIMALINTEGER	[0-9]|[1-9][0-9]+
DOUBLES		[0-9]+\.[0-9]*([Ee][-+]?[0-9]+)? 
HEXA_1		[1-9a-fA-F]
HEXA_2		[0-9a-fA-F][0-9a-fA-F]
IDENTIFIER1 	[_]+[A-Za-z0-9]+
IDENTIFIER2	[A-Za-z][A-Za-z0-9_]*
IDENTIFIER	({IDENTIFIER1}|{IDENTIFIER2})*
WHITESPACE	[ \n\r\t]*

%x X_STRING X_COMMENT X_SPECIAL_CHAR X_COMMENT_LINE X_END_STRING
%%
">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"<>"                   return tNE; 
"repeat"		return tREPEAT;
"if"                   	return tIF; 
"else"                 	return tELSE; 
"local"			return tLOCAL;
"import"		return tIMPORT;
"next"			return tNEXT;
"stop"			return tSTOP;
"return"		return tRETURN;
"noob"			return tNOOB;
"!!"			return tPRINTLN;
"apply"			return tAPPLY;
"from"			return tFROM;

\"                      {yy_push_state(X_STRING); yylval.s = new std::string();}
<X_STRING>\"           	{yy_pop_state(); 			return tSTRING;}
<X_STRING>\\		yy_push_state(X_SPECIAL_CHAR);
<X_STRING>\/\/		yy_push_state(X_COMMENT_LINE);
<X_STRING>\/*		yy_push_state(X_COMMENT);
<X_STRING>\n		{yyerror("New line in string");}
<X_STRING>.  	        *yylval.s += yytext;


\/\*			yy_push_state(X_COMMENT);
<X_COMMENT>\/\/		yy_push_state(X_COMMENT_LINE);
<X_COMMENT>\*\/		yy_pop_state();
<X_COMMENT>.|\n		;

\/\/			yy_push_state(X_COMMENT_LINE);
<X_COMMENT_LINE>.	;
<X_COMMENT_LINE>\n	yy_pop_state();

<X_SPECIAL_CHAR>n	{yy_pop_state(); *yylval.s += std::string(1,10);}
<X_SPECIAL_CHAR>r	{yy_pop_state(); *yylval.s += std::string(1,13);}
<X_SPECIAL_CHAR>t	{yy_pop_state(); *yylval.s += std::string(1,9);}
<X_SPECIAL_CHAR>\"	{yy_pop_state(); *yylval.s += std::string(1,34);}
<X_SPECIAL_CHAR>{HEXA_1} {yy_pop_state(); *yylval.s+=std::string(1,(strtol(yytext,nullptr,16)));}
<X_SPECIAL_CHAR>{HEXA_2} {yy_pop_state(); *yylval.s+=std::string(1,(strtol(yytext,nullptr,16)));}
<X_SPECIAL_CHAR>0	 {yy_pop_state(); yy_push_state(X_END_STRING);}
<X_SPECIAL_CHAR>.	{yyerror("Invalid lexical operation");}

<X_END_STRING>\"	{yy_pop_state(); yy_pop_state(); return tSTRING;}
<X_END_STRING>.|\n	{yyerror("Invalid lexical operation");}

{HEXADECIMAL}		{
    yylval.i = strtol(yytext,nullptr,16);
    if(yylval.i >= 0){
      return tINTEGER;
    }
    else{ yyerror("Stackoverflow");}
}
{DOUBLES} 		{yylval.d = std::stod(yytext); return tREAL;}
{DECIMALINTEGER}        {
    yylval.i = strtoul(yytext, nullptr, 10);
    if(yylval.i < 0){
      yyerror("Stackoverflow");
    }
    else{
     return tINTEGER;
    }
}
{IDENTIFIER} 		{yylval.s = new std::string(yytext); 	return tIDENTIFIER;}


[-()<>=+*/%#$;,!{}?]       					return *yytext;
[[\]|@~&]							return *yytext;
{WHITESPACE}             ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
//
// Very, very dirty hack: flex is a mess generating C++ scanners.

int pwn_scanner::yywrap() { return 1; }
