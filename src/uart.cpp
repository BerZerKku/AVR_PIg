/*
 *  uart.cpp
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

/** �����������
 * 	����� �����-������ ������ ���� ��������� �������
 * 	@param port ��� �����
 * 	@arg UART0
 * 	@arg UART1
 * 	@param size ������ ������
 */
clUart::clUart(eUART_PORT port, uint8_t size)
{
	if (port == UART0)
	{
		udr = &UDR0;
		ubbrh = &UBRR0H;
		ubbrl = &UBRR0L;
		ucsrc = &UCSR0C;
		ucsrb = &UCSR0B;
	}
	else if (port == UART1)
	{
		udr = &UDR1;
		ubbrh = &UBRR1H;
		ubbrl = &UBRR1L;
		ucsrc = &UCSR1C;
		ucsrb = &UCSR1B;
	}

	// ������� ����� ��� ������ ���������� �������
//	this->size = size;
//	buf = (uint8_t *) malloc(sizeof(uint8_t) * size);
}

/**	����������
 * 	��������� �����
 * 	@param ���
 * 	@return ���
 */
//clUart::~clUart()
//{
////	free(buf);
//}

/*	�������� ����� ������
 * 	@param byte ������
 * 	@return ���
 */
void
clUart::trByte(uint8_t byte)
{
	*udr = byte;
}

/**	��������� �����
 * 	@param baudrate �������� ������ �����
 * 	@return ���
 */
void
clUart::init(uint16_t baudrate)
{
	uint16_t tmp;

	// ��������� ������ �����-������
	// ��� ����� �������� �� ���������
//	DDRD&=~(1<<PORTD2);
//	DDRD|=(1<<PORTD3);
//	PORTD&=~(1<<PORTD2);
//	PORTD|=(1<<PORTD3);

	// ������� �������� ��� �������������� ���� ���������
	tmp = (uint16_t) ( F_CPU / (16 * (uint32_t) baudrate)) - 1;
	*ubbrh = (uint8_t) tmp >> 8;
	*ubbrl = (uint8_t) tmp;

	// ����������� ������, 8-��� ������, 2 ����-����
	*ucsrc = (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);

	// ���������� ������ �����������
	// ���������� �� ��������� ��������
	*ucsrb = (1 << TXCIE1) | (1<<TXEN1) | (1 << UDRIE1);
}
