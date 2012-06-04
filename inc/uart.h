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

	~clUart();
	clUart(eUART_PORT port, uint8_t size);

	volatile uint8_t *udr;

	void 	init	(uint16_t baudrate);
	void 	trByte	(uint8_t byte);
	uint8_t trData	(const uint8_t *buf, uint8_t size);

	/**	Обработчик прерывания опустошения буфера передачика.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	inline void isrUDR()
	{
		if (cnt > 0)
		{
			*udr = *ptr++;
			cnt--;
		}
		else
			*ucsrb &= ~(1 << UDRIE1);
	}

	/**	Обработчик прерывания окончания передачи.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	inline void isrTX()
	{
		*ucsrb |= (1 << RXCIE1);
		*ucsrb &= ~(1 << TXCIE1);
		ptr = buf;
		cnt = 0;
	}

	/**	Обработчик прерывания по прихожду байта в буфер приемника.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	inline void isrRX()
	{
		uint8_t tmp;

		tmp = *udr;
		*udr = tmp;
		*ptr++ = tmp;
		cnt++;
	}

private:
	uint8_t	*buf;
	uint8_t size;
	volatile uint8_t *ptr;
	volatile uint8_t cnt;

	volatile uint8_t *ubbrh;
	volatile uint8_t *ubbrl;
	volatile uint8_t *ucsrc;
	volatile uint8_t *ucsrb;
};

#endif /* UART_H_ */
