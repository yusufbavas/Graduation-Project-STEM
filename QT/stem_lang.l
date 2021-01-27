%option noinput nounput
%option noyywrap

%{
#include <stdio.h>

#include "stem_lang.tab.h"




%}

alpha [a-zA-Z]
digit [0-9]

%%
[ \t]		;
[ \n]   { yylineno = yylineno + 1;}
"forward"     return COMMAND_W;
"forward2"    return COMMAND_W2;
"backward"      return COMMAND_S;
"backward2"     return COMMAND_S2;
"right"       return COMMAND_D;
"left"       return COMMAND_A;
"stop"       return COMMAND_X;
"distance"    return COMMAND_M;
"delay"     return COMMAND_DELAY;
"int"       return TYPE_INT;
"void"      return TYPE_VOID;
"while"     return WHILE;
"if"        return IF;
"else"      return ELSE;
{digit}+       return NUM;
{alpha}({alpha}|{digit})* return ID;
"="     return ASGN;
"<="    return LE;
">="    return GE;
"=="    return EQ;
"!="    return NE;
">"     return GT;
"<"     return LT;
"||" 	return LOR;
"&&"  	return LAND;
\/\/.* ;
\/\*(.*\n)*.*\*\/ ;
.       return yytext[0];
%%
