#include "main.h"

uint32_t IC_Val1;
uint32_t IC_Val2;
uint32_t Difference;
uint8_t Is_First_Captured;  // is the first value captured ?
uint8_t Distance;

TIM_HandleTypeDef *timer;

void HCSR04_init(TIM_HandleTypeDef *tim);
void delay_us (uint16_t us);
void HCSR04_Read (void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
