/*
 *  uart.cpp
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"
#include "debug.h"

// �����������
TUart::TUart(TUart::ePORT port, uint8_t * const buf, uint8_t size) :
		buf_(buf), size_(size) {
	if (port == PORT_UART0) {
		udr_ = &UDR0;
		ubbrh_ = &UBRR0H;
		ubbrl_ = &UBRR0L;
		ucsrc_ = &UCSR0C;
		ucsrb_ = &UCSR0B;
		ucsra_ = &UCSR0A;
	} else if (port == PORT_UART1) {
		udr_ = &UDR1;
		ubbrh_ = &UBRR1H;
		ubbrl_ = &UBRR1L;
		ucsrc_ = &UCSR1C;
		ucsrb_ = &UCSR1B;
		ucsra_ = &UCSR1A;
	}

	numTrByte_ = 0;
	cnt_ = 0;
}

// ��������� ����
bool TUart::open(uint16_t baud, TDataBits::DATA_BITS databits,
		TParity::PARITY parity, TStopBits::STOP_BITS stopbits) {
	bool sost = true;

	uint8_t ucsra = 0;
	uint8_t ucsrb = 0;
	uint8_t ucsrc = 0;

	// ��������� �������� ������


	if (baud > 19200) {
		baud = 19200;
		sost = false;
	}

	// ��� ������� �������������� ���� U2X
	// UBBRH = F_CPU / (8 * baudrate) - 1
	// ��� ���������� � ������� ������� ������� 4*baudrate
	uint32_t ibaud = baud;
	ibaud = ((F_CPU + 4*ibaud) / (8*ibaud)) - 1;
	ucsra |= (1 << U2X);					// ���. �������� �������� ������

	if (parity >= TParity::MAX) {
		parity = TParity::NONE;
		sost = false;
	}

	switch(parity)
	{
		case TParity::NONE:
			ucsrc |= (0 << UPM1) | (0 << UPM0);
			break;
		case TParity::EVEN:
			ucsrc |= (1 << UPM1) | (0 << UPM0);
			break;
		case TParity::ODD:
			ucsrc |= (1 << UPM1) | (1 << UPM0);
			break;
		case TParity::MAX:		// ��������
			break;
	}

	if (stopbits >= TStopBits::MAX) {
		stopbits = TStopBits::TWO;
		sost = false;
	}

	switch(stopbits)
	{
		case TStopBits::ONE:
			ucsrc |= (0 << USBS);
			break;
		case TStopBits::TWO:
			ucsrc |= (1 << USBS);
			break;
		case TStopBits::MAX:	// ��������
			break;
	}

	if (databits >= TDataBits::MAX) {
		databits = TDataBits::_8;
		sost = false;
	}

	switch(databits)
	{
		case TDataBits::_8:
			ucsrb |= (0 << UCSZ2);
			ucsrc |= (1 << UCSZ1) | (1 << UCSZ0);
			break;
		case TDataBits::MAX:	// ��������
			break;
	}

	*ubbrh_ = (uint8_t) (ibaud >> 8);
	*ubbrl_ = (uint8_t) (ibaud);
	*ucsra_ = ucsra;
	*ucsrb_ = ucsrb;
	*ucsrc_ = ucsrc;

	// ���������� ������ �����������/��������� � ��������� �� ������
	*ucsrb_ |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);

	return sost;
}

// ��������� ����
void TUart::close() {
	// ������ ������ �����������/��������� � ���� ����������
	*ucsrb_ &= ~((1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) |
			(1 << UDRIE1) | (1 << TXCIE1));
}

// �������� ��������� ���-�� ������, ������� ���������� � �����
void TUart::trData(uint8_t size) {
	if (size != 0) {
		// �������� �����
		*ucsrb_ &= ~(1 << RXCIE);

		// �������� ������ ����
		*udr_ = buf_[0];
		this->cnt_ = 1;

		// �������� ���������� �� ����������� ������ UART � ��������� ��������
		*ucsrb_ |= (1 << UDRIE1) | (1 << TXCIE1);
	}

	numTrByte_ = size;
}
