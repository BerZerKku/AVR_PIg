/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
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

/// буфер текста выводимого на ЖКИ
static char vLCDbuf[SIZE_BUF_STRING];

clMenu::clMenu(stMNUparam *param) : sParam(param)
{
	lvlMenu = &clMenu::lvlStart;
	lineParam = 3;

	lvlCreate = true;
}


/**	Работа с текущим уровнем меню
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::main(void)
{
	// Счетчик времени до переинициализации ЖКИ
	static uint8_t reInit = false;

	// Переинициализация диссплея
	if (reInit > 0)
	{
		reInit--;
	}
	else
	{
		reInit = TIME_TO_REINIT_LCD - 1;
		vLCDinit();
	}

	// Считаем код с клавиатуры, если кнопка нажата зажгем подсветку
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		vLCDsetLED(LED_SWITCH);
		key = tmp;
	}

	// очистим буфер и нарисуем границы
	// при необходимости создания уровня меню
	if (lvlCreate)
	{
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	// очистим текстовый буфер и перейдем на нужный уровень меню
	clearTextBuf();
	(this->*lvlMenu) ();
	key = KEY_NO;

	// Преобразование строки символов в данные для вывода на экран
	//vLCDputchar(vLCDbuf, lineParam);

	// запуск обновления инф-ии на ЖКИ
	vLCDrefresh();
}

/** Уровень начальный
 * 	@param Нет
 * 	@return Нет
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

	// вывод режима\состояния устройств
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

/** Уровень отображения параметров
 * 	@param Нет
 * 	@return Нет
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

/** Уровень меню первый
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlFirst()
{
	if (lvlCreate)
	{
		lvlCreate = false;
	}
}

/** Очистка текстового буфера
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::clearTextBuf()
{
	for(uint_fast8_t i = 0; i < sizeof(vLCDbuf);i++)
		vLCDbuf[i] = ' ';
}
