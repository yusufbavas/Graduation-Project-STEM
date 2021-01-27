#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stem_lang.tab.h"

extern int yylex_destroy(void);
extern int yyparse(void);
extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;
extern int yylineno;

char error_message[100];
int error_flag;

void yyerror(char *s);

int  yylex(void);

void push();
void codegen_logical();
void codegen_algebric();
void codegen_assign();
void if_label1();
void if_label2();
void if_label3();
void while_start();
void while_rep();
void while_end();
void check();
void setType();
void STMT_DECLARE();
void command_w_func();
void command_w_func2();
void command_s_func();
void command_s_func2();
void command_a_func();
void command_d_func();
void command_x_func();
void command_m_func();
void command_delay_func();

int lexer(const char* filename);
void clear();
