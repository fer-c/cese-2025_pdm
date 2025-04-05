/*
 * API_uart.h
 *
 *  Created on: Apr 3, 2025
 *      Author: fcoronel
 */

#ifndef INC_API_UART_H_
#define INC_API_UART_H_

#include "API_delay.h"

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* INC_API_UART_H_ */
