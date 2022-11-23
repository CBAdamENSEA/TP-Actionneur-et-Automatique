/**
  ******************************************************************************
  * @file	pwm.c
  * @authors	Adam Cheikh Brahim, Karim Abdellaziz and Sami Asfary
  * @brief	Functions that control the motor
  ******************************************************************************
  **/


#include "pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;
extern uint8_t alpha_now;
extern uint8_t alpha_dest;


/**
  * @brief
  * @param
  * @param
  * @retval
  */
void start_command(uint8_t alpha)
{


	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	htim1.Instance->CCR1=ARR*alpha/100;
	htim1.Instance->CCR2=ARR*(100-alpha)/100;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void stop_command()
{

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void change_speed(uint8_t alpha)
{

	htim1.Instance->CCR1=ARR*alpha/100;
	htim1.Instance->CCR2=ARR*(100-alpha)/100;

}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void change_speed_progressively(uint8_t alpha)
{
	HAL_TIM_Base_Start_IT(&htim6);
	alpha_dest=alpha;

}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void start_up()
{
	HAL_GPIO_WritePin(ISO_RESET_GPIO_Port, ISO_RESET_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim7); // Wait for 2.5 us to turn it off

}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
float read_current()
{
	float current_v=0.0;
	uint32_t adc_value;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_value=HAL_ADC_GetValue(&hadc1);
	current_v=(((adc_value)/65536.0)*3.3-2.5)*11.9;
	return current_v;
}


