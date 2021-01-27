#include "lexer.h"

int count=0;

char st[1000][10];
int top=0;
int i=0;
char temp[10]="t";

int label[200];
int lnum=0;
int ltop=0;
int switch_stack[1000];
int stop=0;
char type[10];
struct Table
{
	char id[20];
	char type[10];
}table[10000];
int tableCount=0;

FILE * f1;
extern FILE *fp;

char commands[1000][20];
int command_counter = 0;

void clear(){
    yyset_lineno(0);
    yylex_destroy();
    error_flag = 0;
    command_counter = 0;
    tableCount = 0;
    count=0;
    top=0;
    i=0;
    lnum=0;
    ltop=0;
    stop=0;
}
int lexer(const char* filename){
    clear();
    yyin = fopen(filename, "r");
    if(yyin == NULL){
        return -1;
    }
   if(!yyparse()){
       fclose(yyin);
       return 1;
    }
   else
    {
       fclose(yyin);
       return 0;
    }
}
    
void push()
{
  	strcpy(st[++top],yytext);
}

void codegen_logical()
{
    sprintf(temp,"$t%d",i);
    sprintf(commands[command_counter++],"= %s %s %s %s",temp,st[top-1],st[top-2],st[top]);
  	top-=2;
 	strcpy(st[top],temp);
 	i++;
}

void codegen_algebric()
{
    sprintf(temp,"$t%d",i); // converts temp to reqd format
    sprintf(commands[command_counter++],"= %s %s %s %s",temp,st[top-1],st[top-2],st[top]);
  	top-=2;
 	strcpy(st[top],temp);
 	i++;
}
void codegen_assign()
{
    int flag=0;
    for(i=0;i<tableCount;i++)
    {
        if(!strcmp(table[i].id,st[top-2]))
        {
            flag=1;
            break;
        }
    }
    if(!flag){
        strcpy(yytext,st[top-2]);
        yyerror("Variable not declared:");
    }


    sprintf(commands[command_counter++],"= %s %s",st[top-2],st[top]);
 	top-=3;
}
 
void if_label1()
{
 	lnum++;
    sprintf(commands[command_counter++],"if not %s",st[top]);
    sprintf(commands[command_counter++],"goto $L%d",lnum);
 	label[++ltop]=lnum;
}

void if_label2()
{
	int x;
	lnum++;
	x=label[ltop--]; 
    sprintf(commands[command_counter++],"goto $L%d",lnum);
    sprintf(commands[command_counter++],"$L%d: ",x);
	label[++ltop]=lnum;
}

void if_label3()
{
	int y;
	y=label[ltop--];
    sprintf(commands[command_counter++],"$L%d:",y);
	top--;
}
void while_start()
{
	lnum++;
	label[++ltop]=lnum;
    sprintf(commands[command_counter++],"$L%d:",lnum);
}
void while_rep()
{
	lnum++;
    sprintf(commands[command_counter++],"if not %s",st[top]);
    sprintf(commands[command_counter++],"goto $L%d",lnum);
 	label[++ltop]=lnum;
}
void while_end()
{
	int x,y;
	y=label[ltop--];
	x=label[ltop--];
    sprintf(commands[command_counter++],"goto $L%d",x);
    sprintf(commands[command_counter++],"$L%d:",y);
	top--;
}

/* for symbol table*/

void check()
{
	char temp[20];
	strcpy(temp,yytext);
	int flag=0;
	for(i=0;i<tableCount;i++)
	{
		if(!strcmp(table[i].id,temp))
		{
			flag=1;
			break;
		}
	}
	if(!flag)
	{
        yyerror("Variable not declared:");
	}
}

void setType()
{
	strcpy(type,yytext);
}


void STMT_DECLARE()
{
	char temp[20];
	int i,flag;
	flag=0;
	strcpy(temp,yytext);
	for(i=0;i<tableCount;i++)
	{
		if(!strcmp(table[i].id,temp))
			{
			flag=1;
			break;
            }
	}
	if(flag)
        yyerror("redefinition of ");

	else
	{
		strcpy(table[tableCount].id,temp);
		strcpy(table[tableCount].type,type);
		tableCount++;
	}
}

void command_w_func(){
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0 || atoi(st[top]) > 100){
            yyerror("speed value must be in range 0-100 ");
        }
    }
    sprintf(commands[command_counter++],"*w %s",st[top]);
    top--;
}

void command_w_func2(){
    if(isdigit(st[top-1][0])){
        if(atoi(st[top-1]) < 0 || atoi(st[top-1]) > 100){
            yyerror("speed value must be in range 0-100 ");
        }
    }
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0){
            yyerror("distance value must be positive ");
        }
    }
    sprintf(commands[command_counter++],"*w %s %s",st[top-1],st[top]);
    top-=2;
}

void command_s_func(){
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0 || atoi(st[top]) > 100){
            yyerror("speed value must be in range 0-100 ");
        }
    }
    sprintf(commands[command_counter++],"*s %s",st[top]);
    top--;
}

void command_s_func2(){
    if(isdigit(st[top-1][0])){
        if(atoi(st[top-1]) < 0 || atoi(st[top-1]) > 100){
            yyerror("speed value must be in range 0-100 ");
        }
    }
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0){
            yyerror("distance value must be positive ");
        }
    }
    sprintf(commands[command_counter++],"*s %s %s",st[top-1],st[top]);
    top-=2;
}

void command_a_func(){
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0 || atoi(st[top]) > 180){
            yyerror("angle must be in range 0-180 ");
        }
    }
    sprintf(commands[command_counter++],"*a %s",st[top]);
    top--;
}

void command_d_func(){
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0 || atoi(st[top]) > 180){
            yyerror("angle must be in range 0-180 ");
        }
    }
    sprintf(commands[command_counter++],"*d %s",st[top]);
    top--;

}

void command_x_func(){
    sprintf(commands[command_counter++],"*x");
}

void command_m_func(){

	sprintf(temp,"$t%d",i);
    sprintf(commands[command_counter++],"= %s %s",temp,"*m");
  	strcpy(st[++top],temp); 	
 	i++;
}

void command_delay_func(){
    if(isdigit(st[top][0])){
        if(atoi(st[top]) < 0){
            yyerror("delay value must positive ");
        }
    }
    sprintf(commands[command_counter++],"*delay %s",st[top]);
    top--;
}
