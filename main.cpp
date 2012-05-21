/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <math.h>
//#include <util/delay.h>

#include "debug.h"
#include "setup.h"
#include "ks0108.h"
#include "menu.h"
#include "keyboard.h"

/// ������ ���������� ������ * 100 ��
#define LCD_REFRESH_PERIOD 2

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

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


