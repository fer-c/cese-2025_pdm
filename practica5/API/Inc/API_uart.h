/*
 * API_uart.h
 *
 *  Created on: Apr 3, 2025
 *      Author: fcoronel
 */

#ifndef INC_API_UART_H_
#define INC_API_UART_H_

#include "API_delay.h"

/**
 * @brief inicializa el UART, en particular el UART2
 */
bool_t uartInit();

/**
 * @brief Envia un comando para limpiar la pantalla por el UART
 * @param none
 * @return: none
 */
void uartSendClearScreen();

/**
 * @brief Envia la configuracion del UART por el UART
 * @param none
 * @return: none
 */
void uartSendConfiguration();

/**
 * @brief Envia un byte por el UART
 * @param byte: byte a enviar
 * @return: none
 */
void uartSendString(uint8_t * pstring);

/**
 * @brief Recibe un byte por el UART
 * @param byte: puntero a la variable donde se guardara el byte recibido
 * @return: none
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/**
 * @brief Recibe un byte por el UART
 * @param byte: puntero a la variable donde se guardara el byte recibido
 * @return: none
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* INC_API_UART_H_ */
