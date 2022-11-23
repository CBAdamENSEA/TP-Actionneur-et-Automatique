/*
 * pwm.c
 *
 *  Created on: Oct 24, 2022
 *      Author: Adam CHEIKH BRAHIM and Karim Abdellaziz and Sami Asfary
 */

#include "pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;
extern uint8_t alpha_now;
extern uint8_t alpha_dest;

void start_command(uint8_t alpha)
{


	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	htim1.Instance->CCR1=ARR*alpha/100;
	htim1.Instance->CCR2=ARR*(100-alpha)/100;
}

void stop_command()
{

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

void change_speed(uint8_t alpha)
{

	htim1.Instance->CCR1=ARR*alpha/100;
	htim1.Instance->CCR2=ARR*(100-alpha)/100;

}
void change_speed_progressively(uint8_t alpha)
{
	HAL_TIM_Base_Start_IT(&htim6);
	alpha_dest=alpha;

}


void start_up()
{
	HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim7); // Wait for 2.5 us to turn it off

}


