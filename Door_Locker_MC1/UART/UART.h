/*
 * UART.h
 *
 *  Created on: Feb 7, 2020
 *      Author: Rana
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "../Util/std_types.h"
#include "../Util/micro_config.h"
#include "../Util/common_macros.h"

/* UART Driver Baud Rate */
#define USART_BAUDRATE 9600

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(void);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */

