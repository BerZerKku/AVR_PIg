/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#include <avr/io.h>
#include "../inc/keyboard.h"
#include "../inc/debug.h"

/// ��� ������� ������
static eKEY keyPressed;

/**	���������� �������� ������� ������
 * 	@param ���
 * 	@return eKEY ��� ������� ������
 */
eKEY
eKEYget(void)
{
	eKEY key_tmp = keyPressed;

	keyPressed = KEY_NO;
	return key_tmp;
}


/**	������� ����������� ������� ������
 *	�������� ���� ��� � 10��
 *	������� ����� ������ "�������" ������������
 *	��������� ������������ ������ TIME_DELAY_REPEAT * (������ ������ �-��)
 *	����� ������������ TIME_DELAY * (������ ������ �-��)
 * 	@param ���
 * 	@return ���
 */
void
vKEYmain(void)
{
	static uint_fast8_t delay = TIME_DELAY;	// ������� ������������
	static uint_fast8_t keyPrev = KEY_NO;	// ���������� �������� ������
	uint_fast8_t tmp;						// ��������� ����������
	uint_fast8_t tmpKey = KEY_NO;			// ��������� ����������

	tmp = (~PIND) & 0xC0;
	// ������� �������� ������� ������ "�������"
	if (tmp & 0x80)
		tmpKey = KEY_FUNC;

	// ��������� ����������, �� ����������� ������ ������� ������
	if (tmp & 0x40)
		tmpKey |= KEY_CANCEL;
	else
	{
		tmp = (~PING) & 0x03;
		if (tmp)
		{
			if (tmp & 0x01)
				tmpKey |= KEY_LEFT;
			else if (tmp & 0x02)
				tmpKey |= KEY_UP;
		}
		else
		{
			tmp = ~(PINC) & 0x1F;
			if (tmp)
			{
				if (tmp & 0x01)
					tmpKey |= KEY_RESET_IND;
				else if (tmp & 0x02)
					tmpKey |= KEY_ENTER;
				else if (tmp & 0x04)
					tmpKey |= KEY_RIGHT;
				else if (tmp & 0x08)
					tmpKey |= KEY_DOWN;
				else if (tmp & 0x10)
					tmpKey |= KEY_RESET;
			}
		}
	}

	// �������� �� ���������� � ���������� ���������
	// ���� �� �������, �������� ������� ������������
	// ���� ������ �� ������, ����������� �� ���������
	// ���� ������ ������������ ���������� �����,
	// ������������ ������������ � �������� TIME_DELAY_REPEAT
	if (tmpKey != keyPrev)
	{
		keyPrev = tmpKey;
		tmpKey = KEY_NO;
		delay = TIME_DELAY;
	}
	else
	{
		if (delay > 0)
		{
			if (tmpKey != KEY_NO)
			{
				tmpKey = KEY_NO;
				delay--;
			}
		}
		else
		{
			delay = TIME_DELAY_REPEAT;
		}
	}

	// ���� ���������� ������� ��� ����������, ������ ����� �������
	// ������� ����� ������ ������� - ������������
	if (keyPressed == KEY_NO)
		keyPressed = (eKEY) tmpKey;
}







