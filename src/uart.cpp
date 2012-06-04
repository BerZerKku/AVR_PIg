/*
 *  uart.cpp
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

clUart::clUart(uint8_t port)
{

}

clUart::clUart(uint16_t port, uint16_t size)
{

}

void clUart::trByte(uint8_t byte)
{
	UDR1 = byte;
}

/**	Настройка uart1
 * 	Не забыть сконфигурировать пины ввода/вывода !!!
 * 	@param baudrate Скорость работы порта
 * 	@return Нет
 */
void clUart::init(uint16_t baudrate)
{
	uint16_t tmp;

	// Настройка портов ввода-вывода
	// без этого работает не корректно
//	DDRD&=~(1<<PORTD2);
//	DDRD|=(1<<PORTD3);
//	PORTD&=~(1<<PORTD2);
//	PORTD|=(1<<PORTD3);

	// рассчет скорости без установленного бита ускорения
	tmp = (uint16_t) ( F_CPU / (16 * (uint32_t) baudrate)) - 1;
	UBRR1H = (uint8_t) tmp >> 8;
	UBRR1L = (uint8_t) tmp;

	// асинхронная работа, 8-бит данных, 2 стоп-бита
	UCSR1C = (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);

	// разрешение работы передатчика
	// прерывания по окончанию передачи
	UCSR1B = (1 << TXCIE1) | (1<<TXEN1);
}

/**	Прерывание по опустошению передающего буфера UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_UDRE_vect)
{
	UDR1 = 0x55;
}

/** Прерывание по окончанию передачи данных UART1
 * 	Запретим прерывание по опустошению буфера
 * 	Разрешим прерывание по приему.
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_TX_vect)
{
	UCSR1B &= ~(1 << UDRIE1);
	UCSR1B |= (1 << RXCIE1);
}
