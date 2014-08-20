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
	static char stateRPS[] PROGMEM = "RPS: In Operation";
	static char stateRX[]  PROGMEM = "RX : Rx Guard";
	static char stateTX[]  PROGMEM = "TX : Tx Guard ";

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
//	snprintf_P(&vLCDbuf[60], 21, fcDef, fcRegime[2], fcDefSost[1]);
//	snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[2], fcPrmSost[1]);
//	snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[2], fcPrdSost[1]);
	snprintf_P(&vLCDbuf[60], 21, stateRPS);
	snprintf_P(&vLCDbuf[80], 21, stateRX);
	snprintf_P(&vLCDbuf[100],21, stateTX);

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
	static char title[] PROGMEM = "Menu";
	static char punkt[] [21] PROGMEM =
	{
		"1. Event Logs",
		"2. Control",
		"3. Settings",
		"4. Test mode",
		"5. Info"
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
	static char title[] PROGMEM = "Menu\\Info";
	static char punkt[] [21] PROGMEM =
	{
			"MCU1 : v1.11",
			"MCU2 : v1.49",
			"DSP  : v1.78"
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
	static char title[] PROGMEM = "Menu\\Event Logs";
	static char punkt[] [21] PROGMEM =
	{
			"1. Events",
			"2. RPS",
			"3. Receiver",
			"4. Transmitter"
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
	static char title[] PROGMEM = "Event Logs\\Events";
	static char punkt0[] [21] PROGMEM =
	{
			"Number 1 of 2",
			"Date:  04.07.14",
			"Time:  09:45:12.357",
			"Event: Restart",
			"State: Service"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Number 2 of 2",
			"Date:  04.07.14",
			"Time:  09:47:14.512",
			"Event: Restart",
			"State: Operate"
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
	static char title[] PROGMEM = "Event Logs\\RPS";
	static char punkt0[] [21] PROGMEM =
	{
			"Number 1 of 1",
			"Date:  04.07.14",
			"Time:  09:53:45.012",
			"Event: No Signal",
			"Code:  000 000"
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
	static char title[] PROGMEM = "Event Logs\\Receiver";
	static char punkt0[] [21] PROGMEM =
	{
			"Number 1 of 2",
			"Date:  05.07.14",
			"Time:  14:13:45.987",
			"Com#:  2",
			"Event: Start"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Number 2 of 2",
			"Date:  05.07.14",
			"Time:  14:13:46.033",
			"Com#:  2",
			"Event: Stop"
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
	static char title[] PROGMEM = "Event Logs\\Transmitter";
	static char punkt0[] [21] PROGMEM =
	{
			"Number 1 of 2",
			"Date:  05.07.14",
			"Time:  14:13:45.964",
			"Com#:  2",
			"Event: Start"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"Number 2 of 2",
			"Date:  05.07.14",
			"Time:  14:13:46.0014",
			"Com#:  2",
			"Event: Stop"
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
	static char title[] PROGMEM = "Menu\\Control";
	static char punkt[] [21] PROGMEM =
	{
			"1. Manual Blocking",
			"2. Remote Blocking",
			"3. Reset",
			"4. Remote Reset",
			"5. Call"
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
	static char title[] PROGMEM = "Menu\\Settings";
	static char punkt[] [21] PROGMEM =
	{
			"1. State",
			"2. Time/Date",
			"3. Parameters",
			"4. Password"
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
	static char title[] PROGMEM = "Settings\\Parameters";
	static char punkt[] [21] PROGMEM =
	{
			"1. RPS",
			"2. Receiver",
			"3. Transmitter",
			"4. General",
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
	static char title[] PROGMEM = "Parameters\\RPS";

	static char punkt1[] [21] PROGMEM =
	{
			"Number 1 of 6",
			"Logic Type",
			"Value: Differential",
			""
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Number 2 of 6",
			"Line Type",
			"Value: 2 ends",
			""
	};
	static char punkt3[] [21] PROGMEM =
	{
			"Number 3 of 6",
			"RPS Low Level",
			"Value: 6dB",
			"Range: 0..20dB"
	};
	static char punkt4[] [21] PROGMEM =
	{
			"Number 4 of 6",
			"Pulse Overlap",
			"Value: 0deg",
			"Range: 0..54deg"
	};
	static char punkt5[] [21] PROGMEM =
	{
			"Number 5 of 6",
			"Line Delay",
			"Value: 0deg",
			"Range: 0..18deg"
	};
	static char punkt6[] [21] PROGMEM =
	{
			"Number 6 of 6",
			"RP Receiver Desense",
			"Value: 0dB",
			"Range: 0..32dB"
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
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 6)
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
	static char title[] PROGMEM = "Parameters\\Receiver";

	static char punkt1[] [21] PROGMEM =
	{
			"Number 1 of 2",
			"Bounce Time",
			"Value: 5ms",
			"Range: 0..10ms"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Number 2 of 2",
			"Prolong. Command Out",
			"Value: 100ms",
			"Range: 0..1000ms"
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
	static char title[] PROGMEM = "Parameters\\Transmitter";

	static char punkt1[] [21] PROGMEM =
	{
			"Number 1 of 2",
			"Bounce Time",
			"Value: 5ms",
			"Range: 0..10ms"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"Number 2 of 2",
			"Command Duration",
			"Value: 50ms",
			"Range: 20..100ms"
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
	static char title[] PROGMEM = "Parameters\\General";

	static char punkt1[] [21] PROGMEM =
		{
				"Number 1 of 7",
				"Amplifier Control",
				"Value: ON",
				"Range: ON, OFF"
		};
		static char punkt2[] [21] PROGMEM =
		{
				"Number 2 of 7",
				"Turn Off Time",
				"Value: 5sec",
				"Range: 0..5sec"
		};
		static char punkt3[] [21] PROGMEM =
		{
				"Number 3 of 7",
				"Guard Low Level",
				"Value: 10dB",
				"Range: 0..15dB"
		};
		static char punkt4[] [21] PROGMEM =
		{
				"Number 4 of 7",
				"Tx Com Retention",
				"Value: OFF",
				"Range: ON, OFF"
		};
		static char punkt5[] [21] PROGMEM =
		{
				"Number 5 of 7",
				"Rx Com Retention",
				"Value: OFF",
				"Range: ON, OFF"
		};
		static char punkt6[] [21] PROGMEM =
		{
				"Number 6 of 7",
				"Com Receiver Desense",
				"Value: 0dB",
				"Range: 0..32dB"
		};
		static char punkt7[] [21] PROGMEM =
		{
				"Number 7 of 7",
				"Communicat. Protocol",
				"Value: MODBUS",
				""
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
	static char title[] PROGMEM = "Settings\\Date&Time";
	static char punkt[] [21] PROGMEM =
	{
			"1. Date",
			"2. Time",
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
