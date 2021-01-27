%{
#include "lexer.h"

%}

%token TYPE_INT TYPE_VOID
%token WHILE 
%token IF ELSE
%token NUM ID
%token COMMAND_W COMMAND_W2 COMMAND_S COMMAND_S2 COMMAND_A COMMAND_D COMMAND_X COMMAND_M COMMAND_DELAY
%right ASGN  
%left LOR
%left LAND
%left EQ NE 
%left LE GE LT GT
%left '+' '-' 
%left '*' '/'

%nonassoc IFX IFX1
%nonassoc ELSE
  
%%

pgmstart 		: TYPE ID '(' ')' STMTS
				;

STMTS 			: '{' STMT1 '}'|
				;
STMT1			: STMT  STMT1
				|
				;

STMT 			: STMT_DECLARE    //all types of statements
				| STMT_ASSGN  
				| STMT_IF
				| STMT_WHILE
				| COMMAND
				| ';'
				;

COMMAND 		: COMMAND_W '(' EXP ')' ';'	{command_w_func();}
                | COMMAND_W2 '(' EXP ',' EXP ')' ';'	{command_w_func2();}
                | COMMAND_S '(' EXP ')' ';'	{command_s_func();}
                | COMMAND_S2 '(' EXP ',' EXP ')'	';' {command_s_func2();}
                | COMMAND_A '(' EXP ')' ';'	{command_a_func();}
                | COMMAND_D '(' EXP ')' ';'	{command_d_func();}
				| COMMAND_X '(' ')' ';'	{command_x_func();}
                | COMMAND_DELAY '(' EXP ')' ';'	{command_delay_func();}
				;


EXP 			: EXP LT{push();} EXP {codegen_logical();}
				| EXP LE{push();} EXP {codegen_logical();}
				| EXP GT{push();} EXP {codegen_logical();}
				| EXP GE{push();} EXP {codegen_logical();}
				| EXP NE{push();} EXP {codegen_logical();}
				| EXP EQ{push();} EXP {codegen_logical();}
				| EXP '+'{push();} EXP {codegen_algebric();}
				| EXP '-'{push();} EXP {codegen_algebric();}
				| EXP '*'{push();} EXP {codegen_algebric();}
				| EXP '/'{push();} EXP {codegen_algebric();}
                | EXP '%'{push();} EXP {codegen_algebric();}
                | EXP {push();} LOR EXP {codegen_logical();}
				| EXP {push();} LAND EXP {codegen_logical();}
				| '(' EXP ')'
				| ID {check();push();}
				| NUM {push();}
				| COMMAND_M '(' ')'	{command_m_func();}
				;


STMT_IF 		: IF '(' EXP ')'  {if_label1();} STMTS ELSESTMT 
				;

ELSESTMT		: ELSE {if_label2();} STMTS {if_label3();}
				| {if_label3();}
				;


STMT_WHILE		:{while_start();} WHILE '(' EXP ')' {while_rep();} WHILEBODY  
				;

WHILEBODY		: STMTS {while_end();}
				| STMT {while_end();}
				;

STMT_DECLARE 	: TYPE {setType();}  ID {STMT_DECLARE();} IDS   //setting type for that line
				;


IDS 			: ';'
				| ','  ID {STMT_DECLARE();} IDS 
				;


STMT_ASSGN		: ID {push();} ASGN {push();} EXP {codegen_assign();} ';'
				;


TYPE			: TYPE_INT
                | TYPE_VOID
				;

%%

void yyerror(char *s) {
    sprintf(error_message,"Syntex Error in line number : %d : %s %s\n", yylineno, s, yytext);
    error_flag = 1;
}
