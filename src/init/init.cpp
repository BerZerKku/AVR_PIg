/*
 * setup.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include "debug.h"

void
low_level_init() __attribute__((__naked__)) __attribute__((section(".init3")));

/**	������������� ���������
 * 	�������������� ����� �� ��������� ��������� �� ���� � ��������� � +
 * 	@param ���
 * 	@return ���
 */
void
low_level_init()
{
	// ��������� ��������
	SFIOR &= ~(1 << PUD);

	// ���� �
	// ��������������� ����
	DDRA = 0xFF;
	PORTA= 0x00;

	// ���� B
	// PORTB.0 = 		- ���� +
	// PORTB.1 = 		- ���� +
	// PORTB.2 = 		- ���� +
	// PORTB.3 = 		- ���� +
	// PORTB.4 = 		- ���� +
	// PORTB.5 = E_STR	- ����� +
	// PORTB.6 = RS		- ����� +
	// PORTB.7 = 		- ���� +
	DDRB = (1 << PB5) | (1 << PB6);
	PORTB = 0xFF;

	// ���� �
	// PORTC.0 = ROW1	- ����
	// PORTC.1 = ROW2	- ����
	// PORTC.2 = ROW3	- ����
	// PORTC.3 = ROW4	- ����
	// PORTC.4 = COL1	- ����
	// PORTC.5 = COL2	- ����
	// PORTC.6 = COL3	- ����
	// PORTC.7 = COl4	- ����
	DDRC = 0;
	PORTC= 0;

	// ���� D
	// PORTD.0 = 		- ���� +.
	// PORTD.1 = 	 	- ���� +
	// PORTD.2 = RXD1	- ������.
	// PORTD.3 = TXD1	- ������.
	// PORTD.4 = MUX	- ����� 0 (������������ ����������� 0-USB, 1-485)
	// PORTD.5 = 		- ���� +
	// PORTD.6 = 		- ���� +
	// PORTD.7 = 		- ���� +
	DDRD = (1 << DDD4);
	PORTD= ~(1 << PD4);

	// ���� �
	// PORTE.0 = RXD0	- ������.
	// PORTE.1 = TXD0	- ������.
	// PORTE.2 = 		- ���� +
	// PORTE.3 = 		- ���� +
	// PORTE.4 = 		- ���� +
	// PORTE.5 = 		- ���� +
	// PORTE.6 = 		- ���� +
	// PORTE.7 = 		- ���� +
	DDRE = 0x00;
	PORTE = 0xFF;

	// ���� F
	// PORTF.0 = 		- ���� +
	// PORTF.1 = 		- ���� +
	// PORTF.2 = 		- ���� (�������� ������ E_STR)
	// PORTF.3 = 		- ���� (�������� ������ RS)
	// PORTF.4 = 		- ���� +
	// PORTF.5 = 		- ���� +
	// PORTF.6 = 		- ���� +
	// PORTF.7 = 		- ���� +
	DDRF = 0x00;
	PORTF = 0xFC;

	// ���� G
	// PORTG.0 = 		- ���� +
	// PORTG.1 = 		- ���� +
	// PORTG.2 = 		- ���� +
	// PORTG.3 = 		- ���� +
	// PORTG.4 = 		- ���� +
	DDRG = 0x00;
	PORTG= ~0x00;

#ifdef DEBUG
	DDR_DBG |= (PIN_TP);
#endif

	// ��������� ��������
	TIMSK = 0;

	// ������ 0
	// ����� ����� �� ����������
	// �������� N = 32, ���� �� OCR = 24
	// ������� = F_CPU / (2 * N * (1 + OCR)
	// ��� F_CPU = 16��� ������� 10000��
	TCCR0 = (1 << WGM01) | (0 << WGM00) |
			(0 << CS02) | (1 << CS01) | (1 << CS00);
	OCR0  = 24;
	TIMSK |= (1 << OCIE0);

	// ������ 1
	// ����� ����� �� ����������
	// �������� N = 256, ���� �� OCR = 624
	// ������� = F_CPU / (2 * N * (1 + OCR)
	// ��� F_CPU = 16��� ������� 50��
	TCCR1A 	= (0 << WGM11) | (0 << WGM10);
	TCCR1B 	= (0 << WGM13) | (1 << WGM12) |
				(1 << CS12) | (0 << CS11) | (0 << CS10);
	TCCR1C 	= 0;
	TCNT1 	= 0;
	OCR1A 	= 624;
	TIMSK 	|= (1 << OCIE1A);

	wdt_enable(WDTO_250MS);
}



