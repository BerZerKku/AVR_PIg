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

/** �����������
 * 	����� �����-������ ������ ���� ��������� �������
 * 	@param port ��� �����
 * 	@arg UART0
 * 	@arg UART1
 * 	@param *buf ��������� �� ����� ������
 * 	@param size ������ ������
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

/*	�������� ����� ������
 * 	@param byte ������
 * 	@return ���
 */
void
clUart::trByte(uint8_t byte)
{
	// �������� �����
	*ucsrb &= ~(1 << RXCIE);

	cnt = 0;
	*udr = byte;

	// �������� ���������� �� ��������� ��������
	*ucsrb |= (1 << TXCIE1);
}

/**	�������� ��������� ���-�� ������, ������� ���������� � �����
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clUart::trData(uint8_t size)
{
	if (size != 0)
	{
		// �������� �����
		*ucsrb &= ~(1 << RXCIE);

		numTrByte = size;

		// �������� ������ ����
		*udr = buf[0];
		this->cnt = 1;

		// �������� ���������� �� ����������� ������ UART � ��������� ��������
		*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);
	}
	else
		numTrByte = 0;

	return numTrByte;
}

/**	�������� ��������� ���-�� ������ �� �������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clUart::trData(uint8_t size, const uint8_t *buf)
{
	uint8_t *tmp;
	uint8_t cnt = 0;

	// ��������� ������ � �����
	if (size != 0)
	{
		// �������� �����
		*ucsrb &= ~(1 << RXCIE);

		tmp = this->buf;
		for(; cnt < size; cnt++)
			this->buf[cnt] = buf[cnt];

		numTrByte = cnt;

		// �������� ������ ����
		*udr = buf[0];
		this->cnt = 1;

		// �������� ���������� �� ����������� ������ UART � ��������� ��������
		*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);
	}
	else
	{
		numTrByte = 0;
	}

	return numTrByte;
}

/** �������� ������ ������
 * 	@param ���
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clUart::trBuf()
{
	// �������� �����
	*ucsrb &= ~(1 << RXCIE);

	numTrByte = size;

	// �������� ������ ����
	*udr = buf[cnt++];

	// �������� ���������� �� ����������� ������ UART � ��������� ��������
	*ucsrb |= (1 << UDRIE1) | (1 << TXCIE1);

	return numTrByte;
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

	// ������ �������� ��� �������������� ���� ���������
	tmp = (uint16_t) ((F_CPU + baudrate * 8L) / (baudrate * 16L) - 1);
	//tmp = (uint16_t) ( F_CPU / (16 * (uint32_t) baudrate)) - 1;
	*ubbrh = (uint8_t) tmp >> 8;
	*ubbrl = (uint8_t) tmp;

	// ����������� ������, 8-��� ������, 2 ����-����
	*ucsrc = (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);

	// ���������� ������ �����������/��������� � ��������� �� ������
	*ucsrb = (1<<TXEN1) | (1 << RXEN1) | (1 << RXCIE1);
}
