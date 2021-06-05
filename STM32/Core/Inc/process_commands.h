#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "QMC5883L.h"
#include "HCSR04.h"

#define COMMAND_END 		"*end               "
#define COMMAND_FORWARD  	"*w"
#define COMMAND_BACKWARD 	"*s"
#define COMMAND_RIGHT 	 	"*d"
#define COMMAND_LEFT		"*a"
#define COMMAND_STOP		"*x"
#define COMMAND_DELAY		"*delay"
#define COMMAND_METER 		"*m"
#define COMMAND_ASGN		"="
#define COMMAND_MOD			"%"
#define COMMAND_PLUS		"+"
#define COMMAND_MINUS		"-"
#define COMMAND_MULT		"*"
#define COMMAND_DIV			"/"
#define COMMAND_EQ			"=="
#define COMMAND_NE			"!="
#define COMMAND_LE			"<="
#define COMMAND_GE			">="
#define COMMAND_GT			">"
#define COMMAND_LT			"<"
#define COMMAND_LOR 		"||"
#define COMMAND_LAND 		"&&"
#define COMMAND_IF			"if"
#define COMMAND_GOTO		"goto"

#define COMMAND_SIZE 20

#define DISTANCE_TO_MS(x,y) (x / (0.019 * y / 100.0 )) + (210.5 * y / 100.0) / 2
#define DISTANCE_TO_MS2(x,y) (x * 50) * (100.0 / ((float) y))
#define DISTANCE_TO_MS3(x,y) (x / (0.0215 * y / 100.0 )) + (186.0465116 * y / 100.0) / 2.0
#define ROUND_TO_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
//#define DEGREE_TO_MS_v0(x) ((x + 0.057 * 35.1 / 2) / 0.057)
#define DEGREE_TO_MS(x) ((x / 0.05032618) + (23.33333345 / 2))
#define DEGREE_TO_MS2(x) ((x / 0.047368421) + 100)

typedef struct {
	char id[10];
	int value;
}variable;

variable variables[100];
int variable_count;

char commands[1000][COMMAND_SIZE];
char loops_command[COMMAND_SIZE];
int command_count;
char delimeter[2];
char *command;
char *parameter;
char *parameter2;

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;

void read_commands(UART_HandleTypeDef* uart);
void process_commands();
variable* find_variable(char* name);
void find_parameters(char* parameter,int * num1,int* num2);
