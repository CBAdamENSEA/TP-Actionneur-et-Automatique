/*
 * shell.c
 *
 *  Created on: Oct 24, 2022
 *      Author: Adam CHEIKH BRAHIM and Karim Abdellaziz
 */

#include "shell.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
extern uint8_t *prompt;
extern uint8_t *newline;

uint8_t help[]="Usage:\r\n"
		"pinout (to show all the connected pins and their functions\r\n"
		"start (to turn on the motor)\r\n"
		"stop (to turn off the motor)\r\n";
uint8_t pinout[]="Pinout:\r\n"
		"PA2: USART2_TX: to transfer data via STLink to the laptop\r\n"
		"PA3: USART2_RX: to receive data via STLink from the laptop\r\n"
		"PA5: LED: To turn ON or OFF the LED\r\n"
		"PA8: TIM1_CH1: to control the motor: PWM signal\r\n"
		"PA9: TIM1_CH2: to control the motor: PWM shifted signal\r\n"
		"PA11: TIM1_CH1N: to control the motor: PWM complementary signal\r\n"
		"PA12: TIM1_CH1N: to control the motor: PWM complementary shifted signal\r\n"
		"PA13: T_SWDIO: to debug the stm32 (serial wire)\r\n"
		"PA14: T_SWCLK: to debug the stm32 (serial wire)\r\n"
		"PC13: BUTTON: to detect the pressing of the button\r\n";
uint8_t start[]="Power ON\r\n";
uint8_t stop[]="Power OFF\r\n";
uint8_t cmdNotFound[]="Command not found\r\n";

/*
 *
 *
 */
void shell(int * newCmdReady,char cmdBuffer[CMD_BUFFER_SIZE])
{
	if (*newCmdReady)
	{
		if(strcmp(cmdBuffer,"help")==0)
		{
			HAL_UART_Transmit(&huart2, help, sizeof(help), HAL_MAX_DELAY);
		}
		else if (strcmp(cmdBuffer,"pinout")==0)
		{
			HAL_UART_Transmit(&huart2, pinout, sizeof(pinout), HAL_MAX_DELAY);
		}
		else if (strcmp(cmdBuffer,"start")==0)
		{
			HAL_UART_Transmit(&huart2, start, sizeof(start), HAL_MAX_DELAY);
		}
		else if (strcmp(cmdBuffer,"stop")==0)
		{
			HAL_UART_Transmit(&huart2, stop, sizeof(stop), HAL_MAX_DELAY);
		}
		else
		{
			HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
		}

		HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
		memset(cmdBuffer,NULL,CMD_BUFFER_SIZE*sizeof(char)); // to clear the reception buffer cmdBuffer
		*newCmdReady = 0; // to wait for another reception
	}
}
