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

/// буфер текста выводимого на ЖКИ
static char vLCDbuf[SIZE_BUF_STRING];

clMenu::clMenu(stMNUparam *param) : sParam(param)
{
	lvlMenu = &clMenu::lvlStart;
	lineParam = 3;

	lvlCreate = true;

	sParam->voltCF = 16;
	sParam->voltDef = 10;
	sParam->voltNoise = 5;
	sParam->voltOutInt = 10;
	sParam->voltOutFract = 0;
	sParam->curOut = 133;

	cursorEnable= false;
	cursorLine = 0;
	key = KEY_NO;
	typeDevice = AVANT_R400;

	vLCDsetLED(LED_ON);
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

	// очистим текстовый буфер и перейдем на нужный уровень меню
	clearTextBuf();
	(this->*lvlMenu) ();
	key = KEY_NO;

	// Преобразование строки символов в данные для вывода на экран
	vLCDputchar(vLCDbuf, lineParam);

	// запуск обновления инф-ии на ЖКИ
	vLCDrefresh();
	PORT_DBG &= ~PIN_TP2;
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

/** Уровень начальный
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlStart()
{
	if (lvlCreate)
	{
		PORT_DBG |= PIN_TP2;

		lvlCreate = false;

		cursorEnable = false;
		lineParam = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	if (typeDevice == AVANT_RZSK)//snprintf_P(&vLCDbuf[0] , 10, fcDate, sParam.day , sParam.month , sParam.year);
	{
		// вывод параметров
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam->hour, sParam->minute, sParam->second);
		snprintf_P(&vLCDbuf[12], 11, fcUz, sParam->voltDef);
		snprintf_P(&vLCDbuf[20], 11, fcUout, sParam->voltOutInt, sParam->voltOutFract);
		snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam->voltCF);
		snprintf_P(&vLCDbuf[40], 11, fcIout, sParam->curOut);
		snprintf_P(&vLCDbuf[52], 11, fcUn, sParam->voltNoise);

		// вывод режима\состояния устройств
		snprintf_P(&vLCDbuf[60], 21, fcDef, fcRegime[2], fcDefSost[1]);
		snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[2], fcPrmSost[1]);
		snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[2], fcPrdSost[1]);
	}
	else if (typeDevice == AVANT_K400)
	{
		// вывод параметров
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam->hour, sParam->minute, sParam->second);
		snprintf_P(&vLCDbuf[12], 11, fcDate, sParam->day, sParam->month, sParam->year);
		snprintf_P(&vLCDbuf[20], 11, fcUout, sParam->voltOutInt, sParam->voltOutFract);
		snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam->voltCF);
		snprintf_P(&vLCDbuf[40], 11, fcIout, sParam->curOut);
		snprintf_P(&vLCDbuf[52], 11, fcUn, sParam->voltNoise);

		// вывод режима\состояния устройств
		snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[2], fcPrmSost[1]);
		snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[2], fcPrdSost[1]);
	}
	else if (typeDevice == AVANT_K400_OPTIC)
	{
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam->hour, sParam->minute, sParam->second);
		snprintf_P(&vLCDbuf[12], 11, fcDate, sParam->day, sParam->month, sParam->year);

		// вывод режима\состояния устройств
		snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[2], fcPrmSost[1]);
		snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[2], fcPrdSost[1]);
	}


	switch(key)
	{
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
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
	static char title[] PROGMEM = "Меню";
	static char punkt[] [21] PROGMEM =
	{
		"1. Журнал",
		"2. Управление",
		"3. Настройка",
		"4. Тест",
		"5. Информация"
	};


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_RIGHT:
			switch(cursorLine)
			{
				case 1:
					lvlMenu = &clMenu::lvlJournal;
					lvlCreate = true;
					break;
				case 2:
					lvlMenu = &clMenu::lvlControl;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetup;
					lvlCreate = true;
					break;
				case 5:
					lvlMenu = &clMenu::lvlInfo;
					lvlCreate = true;
					break;
			}
			break;


		case KEY_LEFT:
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate = true;
			break;

		default:
			break;
	}
}

/** Уровень меню. Информация об аппарате.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlInfo()
{
	static char title[] PROGMEM = "Меню\\Информация";
	static char punkt[] [21] PROGMEM =
	{
			"БСП MCU : v1.11",
			"БСП DSP : v1.78",
			"ПИ  MCU : v1.49"
	};


	if (lvlCreate)
	{
		lvlCreate = false;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	switch(key)
	{
		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Журналы.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlJournal()
{
	static char title[] PROGMEM = "Меню\\Журнал";
	static PGM_P punkt[4];
	static uint8_t numPunkt = 0;

	static char punkt1[] PROGMEM = "%d. Событий";
	static char punkt2[] PROGMEM = "%d. Защты";
	static char punkt3[] PROGMEM = "%d. Приемника";
	static char punkt4[] PROGMEM = "%d. Передатчика";

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);

		if (typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlJournalEvent;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt2)
			{

				lvlMenu = &clMenu::lvlJournalDef;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlJournalPrm;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlJournalPrd;
				lvlCreate = true;
			}
			break;
		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Журнал событий.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlJournalEvent()
{
	static char title[] PROGMEM = "Журнал\\События";
	static char punkt0[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 2",
			"Дата:  04.07.12",
			"Время: 09:45:12.357",
			"Тип: Перезапуск",
			"Значение: Выведен"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 2",
			"Дата:  04.07.12",
			"Время: 09:47:14.512",
			"Тип: Перезапуск",
			"Значение: Введен"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		else
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Журнал защиты.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlJournalDef()
{
	static char title[] PROGMEM = "Журнал\\Защита";
	static char punkt0[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 1",
			"Дата:  04.07.12",
			"Время: 09:53:45.012",
			"Тип: Нет РЗ",
			"Значение: 000 000"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 1)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Журнал приемника.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlJournalPrm()
{
	static char title[] PROGMEM = "Журнал\\Приемник";
	static char punkt0[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 2",
			"Дата:  05.07.12",
			"Время: 14:13:45.987",
			"Тип: Команда 2",
			"Значение: Начало"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 2",
			"Дата:  05.07.12",
			"Время: 14:13:46.033",
			"Тип: Команда 2",
			"Значение: Конец"
	};


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Журнал передатчика.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlJournalPrd()
{
	static char title[] PROGMEM = "Журнал\\Передатчик";
	static char punkt0[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 2",
			"Дата:  05.07.12",
			"Время: 14:13:45.964",
			"Тип: Команда 2",
			"Значение: Начало"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 2",
			"Дата:  05.07.12",
			"Время: 14:13:46.0014",
			"Тип: Команда 2",
			"Значение: Конец"
	};


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/**	Уровень меню. Управление.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlControl()
{
	static char title[] PROGMEM = "Меню\\Управление";
	static PGM_P punkt[10];
	static uint8_t numPunkt;

	static char punkt1[] PROGMEM= "%d. Пуск наладочный";
	static char punkt2[] PROGMEM= "%d. Пуск удаленного";
	static char punkt3[] PROGMEM= "%d. Сброс своего";
	static char punkt4[] PROGMEM= "%d. Сброс удаленного";
	static char punkt5[] PROGMEM= "%d. Вызов";

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);

		if (typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
			punkt[numPunkt++] = punkt5;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}

}

/** Уровень меню. Настройка.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetup()
{
	static char title[] PROGMEM = "Меню\\Настройка";
	static char punkt[] [21] PROGMEM =
	{
			"1. Режим",
			"2. Время и дата",
			"3. Параметры",
			"4. Пароль"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			switch(cursorLine)
			{
				case 2:
					lvlMenu = &clMenu::lvlSetupDT;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetupParam;
					lvlCreate = true;
					break;
			}
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Настройка параметров.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupParam()
{
	static char title[] PROGMEM = "Настройка\\Параметры";
	static PGM_P punkt[4];
	static uint8_t numPunkt = 0;

	static char punkt1[] PROGMEM = "%d. Защиты";
	static char punkt2[] PROGMEM = "%d. Приемника";
	static char punkt3[] PROGMEM = "%d. Передатчика";
	static char punkt4[] PROGMEM = "%d. Общие";

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);

		if (typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlSetupParamDef;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupParamPrm;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParamPrd;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate = true;
			}
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}

}

/** Уровень меню. Настройка параметров защиты.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupParamDef()
{
	static char title[] PROGMEM = "Параметры\\Защиты";

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 7",
			"Тип защиты",
			"Значение: ДФЗ-ПрПД",
			"Диапазон: список"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 7",
			"Тип линии",
			"Значение: 2 концевая",
			"Диапазон: список"
	};
	static char punkt3[] [21] PROGMEM =
	{
			"Номер: 3  Всего: 7",
			"Доп. время без ман.",
			"Значение: 0ч",
			"Диапазон: 0..99ч"
	};
	static char punkt4[] [21] PROGMEM =
	{
			"Номер: 4  Всего: 7",
			"Порог предупр по РЗ",
			"Значение: 6дБ",
			"Диапазон: 0..20дБ"
	};
	static char punkt5[] [21] PROGMEM =
	{
			"Номер: 5  Всего: 7",
			"Перекрытие импульсов",
			"Значение: 0град",
			"Диапазон: 0..54град"
	};
	static char punkt6[] [21] PROGMEM =
	{
			"Номер: 6  Всего: 7",
			"Компенсация задержки",
			"Значение: 0град",
			"Диапазон: 0..18град"
	};
	static char punkt7[] [21] PROGMEM =
	{
			"Номер: 7  Всего: 7",
			"Загруб.чувств. по РЗ",
			"Значение: 0дБ",
			"Диапазон: 0..32дБ"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 7)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Настройка параметров приемника.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupParamPrm()
{
	static char title[] PROGMEM = "Параметры\\Приемник";

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 2",
			"Время включения ком.",
			"Значение: 5мс",
			"Диапазон: 0..10мс"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 2",
			"Задержка на выкл.ком",
			"Значение: 100мс",
			"Диапазон: 0..1000мс"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Настройка параметров передатчика.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupParamPrd()
{
	static char title[] PROGMEM = "Параметры\\Передатчик";

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 2",
			"Время включения ком.",
			"Значение: 5мс",
			"Диапазон: 0..10мс"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 2",
			"Длительность команд",
			"Значение: 50мс",
			"Диапазон: 20..100мс"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Настройка параметров общих.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupParamGlb()
{
	static char title[] PROGMEM = "Параметры\\Общие";

	static char punkt1[] [21] PROGMEM =
	{
			"Номер: 1  Всего: 7",
			"Контроль вых.сигнала",
			"Значение: вкл.",
			"Диапазон: список"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Номер: 2  Всего: 7",
			"Время перезапуска",
			"Значение: 5сек",
			"Диапазон: 0..5сек"
	};
	static char punkt3[] [21] PROGMEM =
	{
			"Номер: 3  Всего: 7",
			"Порог предупр. по КЧ",
			"Значение: 10дБ",
			"Диапазон: 0..15дБ"
	};
	static char punkt4[] [21] PROGMEM =
	{
			"Номер: 4  Всего: 7",
			"Удерж. реле ком. ПРД",
			"Значение: выкл.",
			"Диапазон: список"
	};
	static char punkt5[] [21] PROGMEM =
	{
			"Номер: 5  Всего: 7",
			"Удерж. реле ком. ПРМ",
			"Значение: выкл.",
			"Диапазон: список"
	};
	static char punkt6[] [21] PROGMEM =
	{
			"Номер: 6  Всего: 7",
			"Загруб.чувств. по РЗ",
			"Значение: 0дБ",
			"Диапазон: 0..32дБ"
	};
	static char punkt7[] [21] PROGMEM =
	{
			"Номер: 7  Всего: 7",
			"Протокол связи",
			"Значение: стандарт",
			"Диапазон: список"
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		if (typeDevice == AVANT_K400_OPTIC)
			cursorLine = 2;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);



	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;

			if (typeDevice == AVANT_K400)
			{
				if (cursorLine == 6)
					cursorLine = 5;
			}
			else if (typeDevice == AVANT_K400_OPTIC)
			{
				if (cursorLine <= 2)
					cursorLine = 2;

				if ((cursorLine == 3) || (cursorLine == 6))
				{
					cursorLine--;
				}
			}

			break;
		case KEY_DOWN:
			if (cursorLine < 7)
				cursorLine++;

			if (typeDevice == AVANT_K400)
			{
				if (cursorLine == 6)
					cursorLine = 7;
			}
			else if (typeDevice == AVANT_K400_OPTIC)
			{
				if ((cursorLine == 3) || (cursorLine == 6))
				{
					cursorLine++;
				}
			}
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** Уровень меню. Настройка дата/время.
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlSetupDT()
{
	static char title[] PROGMEM = "Настройка\\Время&дата";
	static char punkt[] [21] PROGMEM =
	{
			"1. Дата",
			"2. Время",
	};

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}
