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

	//snprintf_P(&vLCDbuf[0] , 10, fcDate, sParam.day , sParam.month , sParam.year);
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
	static char punkt[] [21] PROGMEM =
	{
			"1. Событий",
			"2. Защиты",
			"3. Приемника",
			"4. Передатчика"
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
				case 1:
					lvlMenu = &clMenu::lvlJournalEvent;
					lvlCreate = true;
					break;
				case 2:
					lvlMenu = &clMenu::lvlJournalDef;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlJournalPrm;
					lvlCreate = true;
					break;
				case 4:
					lvlMenu = &clMenu::lvlJournalPrd;
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
	static char punkt[] [21] PROGMEM =
	{
			"1. Пуск наладочный",
			"2. Пуск удаленного",
			"3. Сброс своего",
			"4. Сброс удаленного",
			"5. Вызов"
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
	static char punkt[] [21] PROGMEM =
	{
			"1. Защиты",
			"2. Приемника",
			"3. Передатчика",
			"4. Общие",
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
			switch(cursorLine)
			{
				case 1:
					lvlMenu = &clMenu::lvlSetupParamDef;
					lvlCreate;
					break;
				case 2:
					lvlMenu = &clMenu::lvlSetupParamPrm;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetupParamPrd;
					lvlCreate = true;
					break;
				case 4:
					lvlMenu = &clMenu::lvlSetupParamGlb;
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
