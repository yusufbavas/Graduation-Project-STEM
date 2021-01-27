#include "HCSR04.h"


void HCSR04_init(TIM_HandleTypeDef *tim){
	IC_Val1 = 0;
	IC_Val2 = 0;
	Difference = 0;
	Is_First_Captured = 0;  // is the first value captured ?
	Distance  = 0;

	timer = tim;
	HAL_TIM_Base_Start(timer);
	HAL_TIM_IC_Start_IT(timer, TIM_CHANNEL_3);
}

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(timer,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(timer) < us);  // wait for the counter to reach the us input in the parameter
}

void HCSR04_Read (void)
{
	HAL_GPIO_WritePin(HC_SR04_Output_GPIO_Port, HC_SR04_Output_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay_us(10);  // wait for 10 us
	HAL_GPIO_WritePin(HC_SR04_Output_GPIO_Port, HC_SR04_Output_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(timer, TIM_IT_CC3);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // if the interrupt source is channel3
	{
		if (Is_First_Captured==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffff - IC_Val1) + IC_Val2;
			}

			Distance = Difference * .034/2;
			Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
		}
	}
}
