/*
 *  uart.cpp
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../inc/uart.h"
#include "../inc/debug.h"

/** Конструктор
 * 	Порты ввода-вывода должны быть настроены заранее
 * 	@param port Имя порта
 * 	@arg UART0
 * 	@arg UART1
 * 	@param *buf Указатель на буфер данных
 * 	@param size Размер буфера
 */
clUart::clUart(eUART_PORT port, uint8_t * const buf, uint8_t size):
														buf(buf), size(size)
{
	if (port == UART_UART0)
	{
		udr = &UDR0;
		ubbrh = &UBRR0H;
		ubbrl = &UBRR0L;
		ucsrc = &UCSR0C;
		ucsrb = &UCSR0B;
	}
	else if (port == UART_UART1)
	{
		udr = &UDR1;
		ubbrh = &UBRR1H;
		ubbrl = &UBRR1L;
		ucsrc = &UCSR1C;
		ucsrb = &UCSR1B;
	}

	numTrByte = 0;
	cnt = 0;
}

/*	Отправка байта данных
 * 	@param byte Данные
 * 	@return Нет
 */
void
clUart::trByte(uint8_t byte)
{
	// запретим прием
	*ucsrb &= ~(1 << RXCIE);

	cnt = 0;
	*udr = byte;

	// разрешим прерывание по окончанию передачи
	*ucsrb |= (1 << TXCIE1);
}

/**	Отправка заданного кол-ва данных, заранее помещенных в буфер
 * 	@param *buf Указатель на начало данных
 * 	@param size Кол-во байт данных
 * 	@return Кол-во отправляемых байт данных
 */
uint8_t
clUart::trData(uint8_t size)
{
	if (size != 0)
	{
		// Запретим прием
		*ucsrb &= ~(1 << RXCIE);

		numTrByte = size;

		// отправим первый байт
		*udr = buf[0];
		this->cnt = 1;

		// Разрешим прерывание по опустошению буфера UART и окончанию передачи
		*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);
	}
	else
		numTrByte = 0;

	return numTrByte;
}

/**	Отправка заданного кол-ва данных из массива
 * 	@param *buf Указатель на начало данных
 * 	@param size Кол-во байт данных
 * 	@return Кол-во отправляемых байт данных
 */
uint8_t
clUart::trData(uint8_t size, const uint8_t *buf)
{
	uint8_t *tmp;
	uint8_t cnt = 0;

	// Скопируем данные в буфер
	if (size != 0)
	{
		// Запретим прием
		*ucsrb &= ~(1 << RXCIE);

		tmp = this->buf;
		for(; cnt < size; cnt++)
			this->buf[cnt] = buf[cnt];

		numTrByte = cnt;

		// отправим первый байт
		*udr = buf[0];
		this->cnt = 1;

		// Разрешим прерывание по опустошению буфера UART и окончанию передачи
		*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);
	}
	else
	{
		numTrByte = 0;
	}

	return numTrByte;
}

/** Отправка буфера данных
 * 	@param Нет
 * 	@return Кол-во передаваемых байт данных
 */
uint8_t
clUart::trBuf()
{
	// Запретим прием
	*ucsrb &= ~(1 << RXCIE);

	numTrByte = size;

	// отправим первый байт
	*udr = buf[cnt++];

	// Разрешим прерывание по опустошению буфера UART и окончанию передачи
	*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);

	return numTrByte;
}

/**	Настройка порта
 * 	@param baudrate Скорость работы порта
 * 	@return Нет
 */
void
clUart::init(uint16_t baudrate)
{
	uint16_t tmp;

	// Настройка портов ввода-вывода
	// без этого работает не корректно
//	DDRD&=~(1<<PORTD2);
//	DDRD|=(1<<PORTD3);
//	PORTD&=~(1<<PORTD2);
//	PORTD|=(1<<PORTD3);

	// расчет скорости без установленного бита ускорения
	tmp = (uint16_t) ((F_CPU + baudrate * 8L) / (baudrate * 16L) - 1);
	//tmp = (uint16_t) ( F_CPU / (16 * (uint32_t) baudrate)) - 1;
	*ubbrh = (uint8_t) tmp >> 8;
	*ubbrl = (uint8_t) tmp;

	// асинхронная работа, 8-бит данных, 2 стоп-бита
	*ucsrc = (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);

	// разрешение работы передатчика/приемника и прервания по приему
	*ucsrb = (1<<TXEN1) | (1 << RXEN1) | (1 << RXCIE1);
}
