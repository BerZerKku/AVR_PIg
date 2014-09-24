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

// конструктор
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

// открывает порт
bool TUart::open(eUART_BAUD_RATE baud, eUART_DATA_BITS databits,
		eUART_PARITY parity, eUART_STOP_BITS stopbits) {
	bool sost = true;

	uint8_t ucsra = 0;
	uint8_t ucsrb = 0;
	uint8_t ucsrc = 0;

	// установка скорости работы
	uint32_t ibaud = 0;

	if (baud >= UART_BAUD_RATE_MAX) {
		baud = UART_BAUD_RATE_19200;
		sost = false;
	}

	switch(baud) {
		case UART_BAUD_RATE_300:
			ibaud = 300;
			break;
		case UART_BAUD_RATE_600:
			ibaud = 600;
			break;
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
		case UART_BAUD_RATE_MAX:	// заглушка
			break;
	}

	// при условии установленного бита U2X
	// UBBRH = F_CPU / (8 * baudrate) - 1
	// для округления в большую сторону добавим 4*baudrate
	ibaud = ((F_CPU + 4*ibaud) / (8*ibaud)) - 1;
	ucsra |= (1 << U2X);					// вкл. удвоения скорости работы

	if (parity >= UART_PARITY_MAX) {
		parity = UART_PARITY_NONE;
		sost = false;
	}

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
		case UART_PARITY_MAX:		// заглушка
			break;
	}

	if (stopbits >= UART_STOP_BITS_MAX) {
		stopbits = UART_STOP_BITS_TWO;
		sost = false;
	}

	switch(stopbits)
	{
		case UART_STOP_BITS_ONE:
			ucsrc |= (0 << USBS);
			break;
		case UART_STOP_BITS_TWO:
			ucsrc |= (1 << USBS);
			break;
		case UART_STOP_BITS_MAX:	// заглушка
			break;
	}

	if (databits >= UART_DATA_BITS_MAX) {
		databits = UART_DATA_BITS_8;
		sost = false;
	}

	switch(databits)
	{
		case UART_DATA_BITS_8:
			ucsrb |= (0 << UCSZ2);
			ucsrc |= (1 << UCSZ1) | (1 << UCSZ0);
			break;
		case UART_DATA_BITS_MAX:	// заглушка
			break;
	}

	*ubbrh_ = (uint8_t) (ibaud >> 8);
	*ubbrl_ = (uint8_t) (ibaud);
	*ucsra_ = ucsra;
	*ucsrb_ = ucsrb;
	*ucsrc_ = ucsrc;

	// разрешение работы передатчика/приемника и прервания по приему
	*ucsrb_ |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);

	return sost;
}

// Закрывает порт
void TUart::close() {
	// запрет работы передатчика/приемника и всех прерываний
	*ucsrb_ &= ~((1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) |
			(1 << UDRIE1) | (1 << TXCIE1));
}

// Передача заданного кол-ва данных, заранее помещенных в буфер
void TUart::trData(uint8_t size) {
	if (size != 0) {
		// Запретим прием
		*ucsrb_ &= ~(1 << RXCIE);

		// отправим первый байт
		*udr_ = buf_[0];
		this->cnt_ = 1;

		// Разрешим прерывание по опустошению буфера UART и окончанию передачи
		*ucsrb_ |= (1 << UDRIE1) | (1 << TXCIE1);
	}

	numTrByte_ = size;
}
