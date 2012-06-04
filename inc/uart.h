/*
 * uart.h
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */

#ifndef UART_H_
#define UART_H_

class clUart
{
public:
	clUart(uint16_t port, uint16_t size);
	clUart(uint8_t port);

	void init	(uint16_t baudrate);
	void trByte	(uint8_t byte);

};

#endif /* UART_H_ */
