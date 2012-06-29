/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "menu.h"
#include "debug.h"
#include "ks0108.h"
#include "flash.h"
#include "keyboard.h"

extern stMNUparam sParam;

static enum eMNU_LVL eMenuLvl = MNU_LVL_START;

static void lvlStart		(eKEY key);
static void lvlParam		(eKEY key);
static void lvlFirst		(eKEY key);
static void clearTextBuf	(void);

static void (*level []) (eKEY key) =
{
	lvlStart,
	lvlParam,
	lvlFirst
};

/// ����� ������ ���������� �� ���
static char vLCDbuf[169];


/**	������ � ������� ������� ����
 * 	@param ���
 * 	@return ���
 */
void vMNUmain(void)
{
	// ������� ������� �� ����������������� ���
	static uint8_t reInit = false;

	// ����������������� ��������
	if (reInit > 0)
	{
		reInit--;
	}
	else
	{
		reInit = TIME_TO_REINIT_LCD - 1;
		vLCDinit();
	}

	// ���������� ���� ������� ������ � ������� � ������� ������� ����
	// ��������� ����� �������������� �������
	clearTextBuf();
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		vLCDsetLED(LED_SWITCH);
	}
	level[eMenuLvl](tmp);

#ifdef DEBUG
	static char cnt = 0;
	static eKEY lastkey = KEY_NO;
	if (tmp != KEY_NO)
	{
		if (tmp != lastkey)
		{
			lastkey = tmp;
			cnt = 0;
		}
		else
			cnt++;
	}
	// ����� ���� ������� ������
	snprintf(&vLCDbuf[84],  22, "UCSR1B = %02X", UCSR1B);
	snprintf(&vLCDbuf[105], 22, "UCSR1C = %02X", UCSR1C);
	snprintf(&vLCDbuf[126], 22, "UBRR1 = %02X%02X", UBRR1H, UBRR1L);
	snprintf(&vLCDbuf[147], 22, "������ %02X ������� %d", tmp, cnt);
#endif

	// ������� �������
	vLCDclear();
	// �������������� ������ �������� � ������ � ����� �� ���
	vLCDputchar(vLCDbuf);
	vLCDrefresh();
}

/** ������� ���������
 * 	@param ���
 * 	@return ���
 */
static void lvlStart(eKEY key)
{
	snprintf_P(&vLCDbuf[1] , 10, fcDate, sParam.day , sParam.month , sParam.year);
	snprintf_P(&vLCDbuf[11], 10, fcTime, sParam.hour, sParam.minute, sParam.second);
	snprintf_P(&vLCDbuf[22], 10, fcUout, sParam.voltOutInt, sParam.voltOutFract);
	snprintf_P(&vLCDbuf[32], 10, fcUcf, sParam.voltCF);
	snprintf_P(&vLCDbuf[43], 10, fcUn, 	sParam.voltNoise);
	snprintf_P(&vLCDbuf[53], 10, fcUz, 	sParam.voltDef);

	switch(key)
	{
		case KEY_LEFT:
		case KEY_RIGHT:
			eMenuLvl = MNU_LVL_PARAM;
			break;

		default:
			break;
	}
}

/** ������� ����������� ����������
 * 	@param ���
 * 	@return ���
 */
static void lvlParam(eKEY key)
{
	uint16_t uUout = 251;

	snprintf_P(&vLCDbuf[42], 10, fcUout, uUout/10, uUout%10);
	snprintf_P(&vLCDbuf[53], 10, fcIout, 399);

	snprintf_P(&vLCDbuf[63], 10, fcDate, 1, 5, 12);
	snprintf_P(&vLCDbuf[74], 10, fcTime, 19, 15, 54);


	switch(key)
	{
		case KEY_LEFT:
		case KEY_RIGHT:
			eMenuLvl = MNU_LVL_START;
			break;

		default:
			break;
	}
}

/** ������� ���� ������
 * 	@param ���
 * 	@return ���
 */
static void lvlFirst(eKEY key)
{
}

/** ������� ���������� ������
 * 	@param ���
 * 	@return ���
 */
static void clearTextBuf(void)
{
	for(uint_fast8_t i = 0; i < sizeof(vLCDbuf);i++)
		vLCDbuf[i] = ' ';
}
