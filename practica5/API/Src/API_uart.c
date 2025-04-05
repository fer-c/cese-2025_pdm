/*
 * API_uart.c
 *
 *  Created on: Apr 3, 2025
 *      Author: fcoronel
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

bool_t uartInit() {
	  huart2.Instance = USART2;
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart2) == HAL_OK) {
		  uartSendString((uint8_t *)"baud rate: 115200\r\n");
		  uartSendString((uint8_t *)"word len: 8bits\r\n");
		  uartSendString((uint8_t *)"stop bits: 1\r\n");
		  uartSendString((uint8_t *)"parity: NONE\r\n");
		  return true;
	  } else {
		  return false;
	  }

}

void uartSendString(uint8_t * pstring) {
	uint16_t len = strlen((char*) pstring);
	HAL_UART_Transmit(&huart2, pstring, len, 1000);
}


void uartSendStringSize(uint8_t * pstring, uint16_t size) {
	HAL_UART_Transmit(&huart2, pstring, size, 1000);
}

void uartReceiveStringSize(uint8_t * pstring, uint16_t size) {

}
