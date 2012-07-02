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

//static void (*level []) (eKEY key) =
//{
//	lvlStart,
//	lvlParam,
//	lvlFirst
//};

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING];

clMenu::clMenu(stMNUparam *param) : sParam(param)
{
	lvlMenu = &clMenu::lvlStart;
	lineParam = 3;

	lvlCreate = true;
}


/**	������ � ������� ������� ����
 * 	@param ���
 * 	@return ���
 */
void
clMenu::main(void)
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

	// ������� ��� � ����������, ���� ������ ������ ������ ���������
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		vLCDsetLED(LED_SWITCH);
		key = tmp;
	}

	// ������� ����� � �������� �������
	// ��� ������������� �������� ������ ����
	if (lvlCreate)
	{
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	// ������� ��������� ����� � �������� �� ������ ������� ����
	clearTextBuf();
	(this->*lvlMenu) ();
	key = KEY_NO;

	// �������������� ������ �������� � ������ ��� ������ �� �����
	//vLCDputchar(vLCDbuf, lineParam);

	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
}

/** ������� ���������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlStart()
{
	if (lvlCreate)
	{
		lvlCreate = false;

	}

	//snprintf_P(&vLCDbuf[0] , 10, fcDate, sParam.day , sParam.month , sParam.year);
	snprintf_P(&vLCDbuf[00], 11, fcTime, sParam->hour, sParam->minute, sParam->second);
	snprintf_P(&vLCDbuf[12], 11, fcUz, sParam->voltDef);
	snprintf_P(&vLCDbuf[20], 11, fcUout, sParam->voltOutInt, sParam->voltOutFract);
	snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam->voltCF);
	snprintf_P(&vLCDbuf[40], 11, fcIout, sParam->curOut);
	snprintf_P(&vLCDbuf[52], 11, fcUn, sParam->voltNoise);

	// ����� ������\��������� ���������
	snprintf_P(&vLCDbuf[60], 21, fcDef, fcRegime[0], fcDefSost[0]);
	snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[1], fcPrmSost[1]);
	snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[2], fcPrdSost[2]);

	switch(key)
	{
		case KEY_UP:
			if (lineParam > 0)
				lineParam--;
			lvlCreate = true;
			break;
		case KEY_DOWN:
			if (lineParam < 6)
				lineParam++;
			lvlCreate = true;
			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlParam;
			break;

		default:
			break;
	}
}

/** ������� ����������� ����������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlParam()
{
	if (lvlCreate)
	{
		lvlCreate = false;

		vLCDdrawBoard(lineParam);
	}

	uint16_t uUout = 251;

	snprintf_P(&vLCDbuf[42], 10, fcUout, uUout/10, uUout%10);
	snprintf_P(&vLCDbuf[53], 10, fcIout, 399);

	snprintf_P(&vLCDbuf[63], 10, fcDate, 1, 5, 12);
	snprintf_P(&vLCDbuf[74], 10, fcTime, 19, 15, 54);


	switch(key)
	{
		case KEY_LEFT:
		case KEY_RIGHT:

			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlStart;
			break;

		default:
			break;
	}
}

/** ������� ���� ������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlFirst()
{
	if (lvlCreate)
	{
		lvlCreate = false;
	}
}

/** ������� ���������� ������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::clearTextBuf()
{
	for(uint_fast8_t i = 0; i < sizeof(vLCDbuf);i++)
		vLCDbuf[i] = ' ';
}
