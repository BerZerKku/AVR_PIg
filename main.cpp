/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <math.h>
//#include <util/delay.h>

#include "debug.h"
#include "setup.h"
#include "ks0108.h"
#include "menu.h"
#include "keyboard.h"
#include "uart.h"


/// ������ ���������� ������ * 100 ��
#define LCD_REFRESH_PERIOD 2

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;


clUart uartPC(UART1);

/**	main.c
 * 	@param ���
 * 	@return ���
 */
int __attribute__ ((OS_main)) main (void)
{
	// ������� 1�
	uint_fast8_t cnt_lcd = 0;

	vSETUP();

	sei();

	vLCDinit();
	vLCDclear();
	uartPC.init(19200);
	uartPC.trByte(0x1);

	while(1)
	{

		if (b100ms)
		{
			// ������ ����������� ��� � 100��

			// ������ ����������� ��� � 1�

			// ���������� ������
			// ����������� � �������� LCD_REFRESH_PERIOD * 100��
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				vMNUmain();
				cnt_lcd = 0;
			}

			b100ms = false;
		}
	}
}

/**	���������� �� ���������� ������0
 * 	����������� ��� � 100 ���
 * 	@param ���
 * 	@return ���
 */
ISR(TIMER0_COMP_vect)
{
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	if (cnt >  0)
		cnt--;
	else
	{
		cnt = 100 - 1;
		vKEYmain();
	}

	// ���������� ��� �������� ��� � 100���
	vLCDmain();
}

/**	���������� �� ���������� � ������1
 * 	����������� ��� � 100 ��
 * 	@param ���
 * 	@return ���
 */
ISR(TIMER1_COMPA_vect)
{
	b100ms = true;
}

/**	���������� �� ����������� ����������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_UDRE_vect)
{
	// *uartPC.udr = 0x55;
	// UDR1 = 0x55;
	*uartPC.udr = 33;

}

/** ���������� �� ��������� �������� ������ UART1
 * 	�������� ���������� �� ����������� ������
 * 	�������� ���������� �� ������.
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_TX_vect)
{
	//UCSR1B &= ~(1 << UDRIE1);
	//UCSR1B |= (1 << RXCIE1);
}

