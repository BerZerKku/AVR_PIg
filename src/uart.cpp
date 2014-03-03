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
TUart::TUart(eUART_PORT port, uint8_t * const buf, uint8_t size) :
		buf_(buf), size_(size) {
	if (port == UART_UART0) {
		udr_ = &UDR0;
		ubbrh_ = &UBRR0H;
		ubbrl_ = &UBRR0L;
		ucsrc_ = &UCSR0C;
		ucsrb_ = &UCSR0B;
		ucsra_ = &UCSR0A;
	} else if (port == UART_UART1) {
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

/**	��������� ����.
 * 	@param ���
 * 	@return True - � ������ ��������� ��������
 */
bool TUart::open(eUART_BAUD_RATE baud, eUART_DATA_BITS databits,
		eUART_PARITY parity, eUART_STOP_BITS stopbits) {
	bool sost = true;

	uint8_t ucsra = 0;
	uint8_t ucsrb = 0;
	uint8_t ucsrc = 0;

	// ��������� �������� ������
	uint32_t ibaud = 0;
	switch(baud) {
	case UART_BAUD_RATE_1200:
		ibaud = 1200;
		break;
	case UART_BAUD_RATE_2400:
		ibaud = 2400;
		break;
	case UART_BAUD_RATE_4800:
		ibaud = 4800;
		break;
	case UART_BAUD_RATE_9600:
		ibaud = 9600;
		break;
	case UART_BAUD_RATE_19200:
		ibaud = 19200;
		break;
	case UART_BAUD_RATE_38400:
		ibaud = 38400;
		break;
	case UART_BAUD_RATE_57600:
		ibaud = 57600;
		break;
	case UART_BAUD_RATE_MAX:	// ��������� �������
		break;
	}
	// UBBRH = F_CPU / (16 * baudrate) - 1
	// ��� ������� �������������� ���� U2X
	// ��� ���������� � ������� ������� ������� 4*val
	ibaud = ((F_CPU + 4*ibaud) / (8*ibaud)) - 1;
	*ubbrh_ = (uint8_t) (ibaud >> 8);		//
	*ubbrl_ = (uint8_t) (ibaud);			//
	ucsra |= (1 << U2X);					// ���. �������� �������� ������

	switch(parity)
	{
	case UART_PARITY_NONE:
		ucsrc |= (0 << UPM1) | (0 << UPM0);
		break;
	case UART_PARITY_EVEN:
		ucsrc |= (1 << UPM1) | (0 << UPM0);
		break;
	case UART_PARITY_ODD:
		ucsrc |= (1 << UPM1) | (1 << UPM0);
		break;
	case UART_PARITY_MAX:
		sost = false;
		break;
	}

	switch(stopbits)
	{
	case UART_STOP_BITS_ONE:
		ucsrc |= (0 << USBS);
		break;
	case UART_STOP_BITS_TWO:
		ucsrc |= (1 << USBS);
		break;
	case UART_STOP_BITS_MAX:
		sost = false;
		break;
	}

	switch(databits)
	{
	case UART_DATA_BITS_8:
		ucsrb |= (0 << UCSZ2);
		ucsrc |= (1 << UCSZ1) | (1 << UCSZ0);
		break;
	case UART_DATA_BITS_MAX:
		sost = false;
		break;
	}

	*ucsra_ = ucsra;
	*ucsrb_ = ucsrb;
	*ucsrc_ = ucsrc;

	// ���������� ������ �����������/��������� � ��������� �� ������
	*ucsrb_ |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);

	return sost;
}

void TUart::close() {
	// ������ ������ �����������/��������� � ��������� �� ������
	*ucsrb_ &= ~((1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1));
}

bool TUart::isOpen() const {
	return (*ucsrb_ && ((1 << TXEN1) | (1 << RXEN1)));
}

/*	�������� ����� ������
 * 	@param byte ������
 * 	@return ���
 */
void TUart::trByte(uint8_t byte) {
	// �������� �����
	*ucsrb_ &= ~(1 << RXCIE);

	cnt_ = 0;
	*udr_ = byte;

	// �������� ���������� �� ��������� ��������
	*ucsrb_ |= (1 << TXCIE1);
}

/**	�������� ��������� ���-�� ������, ������� ���������� � �����
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t TUart::trData(uint8_t size) {
	if (size != 0) {
		// �������� �����
		*ucsrb_ &= ~(1 << RXCIE);

		numTrByte_ = size;

		// �������� ������ ����
		*udr_ = buf_[0];
		this->cnt_ = 1;

		// �������� ���������� �� ����������� ������ UART � ��������� ��������
		*ucsrb_ |= (1 << UDRIE1) | (1 << TXCIE1);
	} else
		numTrByte_ = 0;


	return numTrByte_;
}

/**	�������� ��������� ���-�� ������ �� �������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t TUart::trData(uint8_t size, const uint8_t *buf) {
	uint8_t cnt = 0;

	// ��������� ������ � �����
	if (size != 0) {
		// �������� �����
		*ucsrb_ &= ~(1 << RXCIE);

		for (; cnt < size; cnt++)
			this->buf_[cnt] = buf[cnt];

		numTrByte_ = cnt;

		// �������� ������ ����
		*udr_ = buf[0];
		this->cnt_ = 1;

		// �������� ���������� �� ����������� ������ UART � ��������� ��������
		*ucsrb_ |= (1 << UDRIE1) | (1 << TXCIE1);
	} else {
		numTrByte_ = 0;
	}

	return numTrByte_;
}

/** �������� ������ ������
 * 	@param ���
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t TUart::trBuf() {
	// �������� �����
	*ucsrb_ &= ~(1 << RXCIE);

	numTrByte_ = size_;

	// �������� ������ ����
	*udr_ = buf_[cnt_++];

	// �������� ���������� �� ����������� ������ UART � ��������� ��������
	*ucsrb_ |= (1 << UDRIE1) | (1 << TXCIE1);

	return numTrByte_;
}
