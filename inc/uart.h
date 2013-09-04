/**
 * uart.h
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 *
 *     Работа с UART.
 *     Настройка портов ввода вывода производится заранее.
 */

#ifndef UART_H_
#define UART_H_

enum eUART_PORT
{
	UART_UART0,
	UART_UART1
};

class clUart
{
public:

//	~clUart();
	clUart(eUART_PORT port, uint8_t * const buf, uint8_t size);

	volatile uint8_t *udr;

	void 	init	(uint16_t baudrate);
	void 	trByte	(uint8_t byte);
	uint8_t trData	(uint8_t size);
	uint8_t trData	(uint8_t size, const uint8_t *buf);
	uint8_t trBuf	();

	/// Считывание счетчика байт данных в буфере
	uint8_t getCnt() const { return cnt; }

	/// Сбросить счетчик байт данных в буфере
	void clrCnt() {cnt = 0;}

	/**	Обработчик прерывания опустошения буфера передачика.
	 * 	Не забыть расположить в нужном прерывании !!
	 */
	void isrUDR()
	{
		if (cnt < numTrByte)
		{
			*udr = buf[cnt++];
		}
		else
		{
			*ucsrb &= ~(1 << UDRIE1);
		}
	}

	/**	Обработчик прерывания окончания передачи.
	 * 	Запрещаем передачу, разрешаем прием.
	 * 	Не забыть расположить в нужном прерывании !!
	 */
	void isrTX()
	{
		cnt = 0;
		numTrByte = 0;
		*ucsrb |= (1 << RXCIE1);
		*ucsrb &= ~(1 << TXCIE1);

	}

	/**	Обработчик прерывания по прихожду байта в буфер приемника.
	 * 	@param byte Принятый байт данных
	 * 	Не забыть расположить в нужном прерывании !!
	 */
	void isrRX(uint8_t byte)
	{
		if (cnt < size)
		{
			buf[cnt++] = byte;
		}
	}

private:
	// кол-во принятых/полученных данных
	volatile uint8_t cnt;
	// указатель на буфер
	uint8_t	* const buf;
	// размер буфера
	const uint8_t size;
	// кол-во данных на передачу
	uint8_t numTrByte;


	// регистры настройки
	volatile uint8_t *ubbrh;
	volatile uint8_t *ubbrl;
	volatile uint8_t *ucsrc;
	volatile uint8_t *ucsrb;
};

#endif /* UART_H_ */
