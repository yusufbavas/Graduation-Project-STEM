#include "process_commands.h"

void read_commands(UART_HandleTypeDef* uart){
	command_count = 0;
	variable_count = 0;

	while(1){
		if (HAL_UART_Receive(uart, commands[command_count], COMMAND_SIZE, 100) == HAL_OK){
			if(strcmp(commands[command_count],COMMAND_END) == 0)
				break;
			command_count++;
		}
	}
}

void process_commands(){

	strcpy(delimeter," ");

	for(int i=0;i<command_count;++i){
		memset(loops_command,0,COMMAND_SIZE);
		strcpy(loops_command,commands[i]);

		command = strtok(loops_command,delimeter);
		// Forward
		if(strcmp(command,COMMAND_FORWARD) == 0){
			int speed;
			parameter = strtok(NULL, delimeter);
			if(isdigit(parameter[0])){
				speed = atoi(parameter);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed);
			}
			else{
				variable *v2 = find_variable(parameter);
				speed = v2->value;
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed);
			}

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);

			parameter = strtok(NULL, delimeter);
			// forward with 2 argument
			if (parameter != NULL){
				int param;
				if(isdigit(parameter[0])){
					param = atoi(parameter);
				}
				else{
					variable *v2 = find_variable(parameter);
					param = v2->value;
				}
				HAL_Delay(ROUND_TO_INT(DISTANCE_TO_MS2(param,(speed < 100 ? speed: 100))));

				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
			}
		}
		// backward
		else if(strcmp(command,COMMAND_BACKWARD) == 0){
			int speed;
			parameter = strtok(NULL, delimeter);

			if(isdigit(parameter[0])){
				speed = atoi(parameter);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed);
			}
			else{
				variable *v2 = find_variable(parameter);
				speed = v2->value;
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed);
			}

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);

			parameter = strtok(NULL, delimeter);
			// backward with second argument
			if (parameter != NULL){
				int param;
				if(isdigit(parameter[0])){
					param = atoi(parameter);
				}
				else{
					variable *v2 = find_variable(parameter);
					param = v2->value;
				}
				HAL_Delay(ROUND_TO_INT(DISTANCE_TO_MS2(param,(speed < 100 ? speed: 100))));

				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
			}
		}
		else if(strcmp(command,COMMAND_RIGHT) == 0){
			parameter = strtok(NULL, delimeter);

			int param;
			if(isdigit(parameter[0])){
				param = atoi(parameter);
			}
			else{
				variable *v2 = find_variable(parameter);
				param = v2->value;
			}
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,100);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,100);

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);

			//HAL_Delay(param);
			HAL_Delay(ROUND_TO_INT(DEGREE_TO_MS(param)));

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
		}
		else if(strcmp(command,COMMAND_LEFT) == 0){
			parameter = strtok(NULL, delimeter);

			int param;
			if(isdigit(parameter[0])){
				param = atoi(parameter);
			}
			else{
				variable *v2 = find_variable(parameter);
				param = v2->value;
			}

			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,100);
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,100);

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_Delay(ROUND_TO_INT(DEGREE_TO_MS(param)));
			//HAL_Delay(param);

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
		}
		else if(strcmp(command,COMMAND_STOP) == 0){

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
		}
		else if(strcmp(command,COMMAND_DELAY) == 0){
			parameter = strtok(NULL, delimeter);
			int param;
			if(isdigit(parameter[0])){
				param = atoi(parameter);
			}
			else{
				variable *v2 = find_variable(parameter);
				param = v2->value;
			}

			HAL_Delay(param);
		}
		else if(strcmp(command,COMMAND_ASGN) == 0){

			parameter = strtok(NULL, delimeter);
			variable *v = find_variable(parameter);

			int num1,num2;
			parameter = strtok(NULL, delimeter);


			if(isdigit(parameter[0])){
				v->value = atoi(parameter);

			}
			else{

				if(strcmp(parameter,COMMAND_PLUS) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 + num2;
				}
				else if(strcmp(parameter,COMMAND_MINUS) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 - num2;
				}
				else if(strcmp(parameter,COMMAND_MULT) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 * num2;
				}
				else if(strcmp(parameter,COMMAND_DIV) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 / num2;
				}
				else if(strcmp(parameter,COMMAND_EQ) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 == num2;
				}
				else if(strcmp(parameter,COMMAND_NE) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 != num2;
				}
				else if(strcmp(parameter,COMMAND_LE) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 <= num2;
				}
				else if(strcmp(parameter,COMMAND_GE) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 >= num2;
				}
				else if(strcmp(parameter,COMMAND_GT) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 > num2;
				}
				else if(strcmp(parameter,COMMAND_LT) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 < num2;
				}
				else if(strcmp(parameter,COMMAND_LOR) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 || num2;
				}
				else if(strcmp(parameter,COMMAND_LAND) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 && num2;
				}
				else if(strcmp(parameter,COMMAND_MOD) == 0){

					find_parameters(parameter, &num1, &num2);
					v->value = num1 % num2;
				}
				else if(strcmp(parameter,COMMAND_METER) == 0){
					HCSR04_Read();
					HAL_Delay(10);

					int dis = Distance;
					v->value = dis;
				}
				else{
					variable *v2 = find_variable(parameter);
					v->value = v2->value;
				}
			}

		}
		else if(strcmp(command,COMMAND_IF) == 0){
			parameter = strtok(NULL, delimeter);
			if(strcmp(parameter,"not") == 0){
				parameter = strtok(NULL, delimeter);
				variable *v2 = find_variable(parameter);
				if(v2->value != 0)
					i++;
			}
			else{
				variable *v2 = find_variable(parameter);
				if(v2->value == 0)
					i++;
			}

		}
		// jump to given labels index
		else if(strcmp(command,COMMAND_GOTO) == 0){
			parameter = strtok(NULL, delimeter);
			char *temp;
			for(int j=0;j<command_count;++j){
				temp = strtok(commands[j],":");
				if(strcmp(temp,parameter) == 0){
					i = j;

					break;
				}
			}
		}
	}

	// stop motors
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
}
// parse input and find 2 parameter
// if parameter is a variable name return its value
void find_parameters(char* parameter,int * num1,int* num2){
	parameter = strtok(NULL, delimeter);
	if(isdigit(parameter[0])){
		*num1 = atoi(parameter);
	}
	else{
		variable *v2 = find_variable(parameter);
		*num1 = v2->value;
	}

	parameter = strtok(NULL, delimeter);
	if(isdigit(parameter[0])){
		*num2 = atoi(parameter);
	}
	else{
		variable *v2 = find_variable(parameter);
		*num2 = v2->value;
	}
}
// find variable according to given name
// if variable is not exist create it.
variable* find_variable(char* name){

	for(int i=0;i<variable_count;++i){
		if(strcmp(variables[i].id,name) == 0 )
			return &variables[i];
	}
	strcpy(variables[variable_count].id,name);
	variable_count++;
	return &variables[variable_count-1];
}
