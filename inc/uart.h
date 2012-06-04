/*
 * uart.h
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */

#ifndef UART_H_
#define UART_H_

enum eUART_PORT
{
	UART0,
	UART1
};

class clUart
{
public:
	clUart(eUART_PORT port);
//	~clUart();

	volatile uint8_t* udr;

	void init	(uint16_t baudrate);
	void trByte	(uint8_t byte);

	uint8_t	*buf;
	uint8_t size;

private:
	volatile uint8_t *ubbrh;
	volatile uint8_t *ubbrl;
	volatile uint8_t *ucsrc;
	volatile uint8_t *ucsrb;
};

#endif /* UART_H_ */
