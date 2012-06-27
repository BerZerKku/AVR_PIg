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

//	~clUart();
	clUart(eUART_PORT port, uint8_t *buf, uint8_t size);

	volatile uint8_t *udr;

	void 	init	(uint16_t baudrate);
	void 	trByte	(uint8_t byte);
	uint8_t trData	(const uint8_t *buf, uint8_t size);

	/**	Обработчик прерывания опустошения буфера передачика.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	void isrUDR()
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
	 * 	Запрещаем передачу, разрешаем прием.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	void isrTX()
	{
		ptr = buf;
		cnt = 0;
		*ucsrb |= (1 << RXCIE1);
		*ucsrb &= ~(1 << TXCIE1);

	}

	/**	Обработчик прерывания по прихожду байта в буфер приемника.
	 * 	Не забыть расположить в нужном прерывании !!!
	 */
	void isrRX()
	{
		uint8_t tmp  = *udr;

		if (cnt < size)
		{
			*ptr++ = tmp;
			cnt++;
		}
	}

private:
	// кол-во принятых/полученных данных
	volatile uint8_t cnt;
	// указатель на буфер
	uint8_t	* const buf;
	// размер буфера
	const uint8_t size;
	//
	volatile uint8_t *ptr;


	// регистры настройки
	volatile uint8_t *ubbrh;
	volatile uint8_t *ubbrl;
	volatile uint8_t *ucsrc;
	volatile uint8_t *ucsrb;
};

#endif /* UART_H_ */
