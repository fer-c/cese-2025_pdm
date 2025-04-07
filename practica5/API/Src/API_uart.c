/*
 * API_uart.c
 *
 *  Created on: Apr 3, 2025
 *      Author: fcoronel
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include <string.h>

static uint8_t *UART_BAUD_RATE = (uint8_t*) "baud rate: 115200\r\n";
static uint8_t *UART_PARITY = (uint8_t*) "parity: NONE\r\n";
static uint8_t *UART_STOP_BITS = (uint8_t*) "stop bits: 1\r\n";
static uint8_t *UART_WORD_LEN = (uint8_t*) "word len: 8bits\r\n";

extern UART_HandleTypeDef huart2;

const uint32_t UART_Timeout_Escritura = 1000; /**< Timeout para la excritura de datos por UART. */
const uint32_t UART_Timeout_Lectura = 1; /**< Timeout para la lectura de datos por UART. */

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
    uartSendConfiguration();
    return true;
  } else {
    return false;
  }

}

void uartSendClearScreen() {
  uint8_t clearScreen[] = "\x1b[2J\x1b[H";  // Borrar pantalla y mover el cursor al inicio
  uartSendString(clearScreen);
}

void uartSendConfiguration() {
  uartSendString(UART_BAUD_RATE);
  uartSendString(UART_WORD_LEN);
  uartSendString(UART_STOP_BITS);
  uartSendString(UART_PARITY);
}

void uartSendString(uint8_t *pstring) {
  assert_param(pstring != NULL);
  uint16_t len = strlen((char*) pstring);
  HAL_UART_Transmit(&huart2, pstring, len, UART_Timeout_Escritura);
}

void uartSendStringSize(uint8_t *pstring, uint16_t size) {
  assert_param(pstring != NULL);
  HAL_UART_Transmit(&huart2, pstring, size, UART_Timeout_Escritura);
}

void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {
  assert_param(pstring != NULL);
  assert_param(size > 0);
  HAL_UART_Receive(&huart2, pstring, size, UART_Timeout_Lectura);
}
