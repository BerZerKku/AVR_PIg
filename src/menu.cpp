/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */
#include <stdio.h>

#include "../inc/menu.h"
#include "../inc/debug.h"
#include "../inc/ks0108.h"
#include "../inc/flash.h"

/// буфер текста выводимого на ЖКИ
static char vLCDbuf[SIZE_BUF_STRING];

//clMenu::clMenu(clProtocolS *protocol) : pProtocol(protocol)
clMenu::clMenu()
{
	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;

	lvlCreate_ = true;

	blink_ = false;

	cursorEnable_= false;
	cursorLine_ = 0;

	key_ = KEY_NO;

	sParam.typeDevice = AVANT_NO;

	connectionBsp_ = false;

	// по умолчанию показываем только дату и время
	measParam[0] = MENU_MEAS_PARAM_TIME;
	measParam[1] = MENU_MEAS_PARAM_DATE;
	for(uint_fast8_t i = 2; i < 6; i++)
		measParam[i] = MENU_MEAS_PARAM_NO;

	// заполним текстовые надписи неисправностей и предупреждений
	// для всех устройств
	for(uint_fast8_t i = 0; i < 16; i++)
	{
		sParam.glb.status.faultText[i] = fcUnknownFault;
		sParam.def.status.faultText[i] = fcUnknownFault;
		sParam.prm.status.faultText[i] = fcUnknownFault;
		sParam.prd.status.faultText[i] = fcUnknownFault;
	}

	for(uint_fast8_t i = 0; i < 8; i++)
	{
		sParam.def.status.warningText[i] = fcUnknownWarning;
		sParam.prm.status.warningText[i] = fcUnknownWarning;
		sParam.prd.status.warningText[i] = fcUnknownWarning;
	}

	// заполним массивы состояний работы для всех устройств
	sParam.def.status.stateText[0] =  fcDefSost00;
	sParam.def.status.stateText[1] =  fcDefSost01;
	sParam.def.status.stateText[2] =  fcDefSost02;
	sParam.def.status.stateText[3] =  fcDefSost03;
	sParam.def.status.stateText[4] =  fcDefSost04;
	sParam.def.status.stateText[5] =  fcDefSost05;
	sParam.def.status.stateText[6] =  fcDefSost06;
	sParam.def.status.stateText[7] =  fcDefSost07;
	sParam.def.status.stateText[8] =  fcDefSost08;
	sParam.def.status.stateText[9] =  fcDefSost09;
	sParam.def.status.stateText[10] = fcDefSost10;
	sParam.def.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	sParam.prm.status.stateText[0] =  fcPrmSost00;
	sParam.prm.status.stateText[1] =  fcPrmSost01;
	sParam.prm.status.stateText[2] =  fcPrmSost02;
	sParam.prm.status.stateText[3] =  fcPrmSost03;
	sParam.prm.status.stateText[4] =  fcPrmSost04;
	sParam.prm.status.stateText[5] =  fcPrmSost05;
	sParam.prm.status.stateText[6] =  fcPrmSost06;
	sParam.prm.status.stateText[7] =  fcPrmSost07;
	sParam.prm.status.stateText[8] =  fcPrmSost08;
	sParam.prm.status.stateText[9] =  fcPrmSost09;
	sParam.prm.status.stateText[10] = fcPrmSost10;
	sParam.prm.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	sParam.prd.status.stateText[0] =  fcPrdSost00;
	sParam.prd.status.stateText[1] =  fcPrdSost01;
	sParam.prd.status.stateText[2] =  fcPrdSost02;
	sParam.prd.status.stateText[3] =  fcPrdSost03;
	sParam.prd.status.stateText[4] =  fcPrdSost04;
	sParam.prd.status.stateText[5] =  fcPrdSost05;
	sParam.prd.status.stateText[6] =  fcPrdSost06;
	sParam.prd.status.stateText[7] =  fcPrdSost07;
	sParam.prd.status.stateText[8] =  fcPrdSost08;
	sParam.prd.status.stateText[9] =  fcPrdSost09;
	sParam.prd.status.stateText[10] = fcPrdSost10;
	sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	// назначим имена устройствам
	sParam.def.status.name = fcDeviceName00;
	sParam.prm.status.name = fcDeviceName01;
	sParam.prd.status.name = fcDeviceName02;

	// включение постоянной подсветки
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

	blink_ =  ((reInit % 8) < 4) ? false : true;


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

	// Считаем код с клавиатуры
	// Если нажата любая кнопка - включится кратковременная подсветка
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLED(LED_SWITCH);
	}

	// очистка текстового буфера и переход на текущий уровень меню
	clearTextBuf();
	(this->*lvlMenu) ();
	key_ = KEY_NO;


#ifdef VIEW_DEBUG_PARAM
	// вывод отладочной информации
	if (this->lvlMenu == &clMenu::lvlStart)
	{
		snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
		snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
		snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
		snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
		snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
		snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
		snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
	}
	else
	{
		snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
	}
#endif

	// вывод сообщения в случае отсутствия связи с БСП
	if (!isConnectionBsp() && (blink_))
		snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);

	// преобразование строки символов в данные для вывода на экран
	vLCDputchar(vLCDbuf, lineParam_);

	// запуск обновления инф-ии на ЖКИ
	vLCDrefresh();
}

/** Установка типа аппарата и настройка меню с его учетом.
 * 	@param device Тип устройства
 * 	@return False в случае ошибки, иначе True
 */
bool
clMenu::setTypeDevice(eGB_TYPE_DEVICE device)
{
	sParam.typeDevice = device;

	sParam.glb.status.setEnable(true);
	measParam[0] = MENU_MEAS_PARAM_TIME;
	measParam[1] = MENU_MEAS_PARAM_DATE;

	if (device == AVANT_K400)
	{
		measParam[2] = MENU_MEAS_PARAM_UOUT;
		measParam[3] = MENU_MEAS_PARAM_IOUT;
		measParam[4] = MENU_MEAS_PARAM_UZ;
		measParam[5] = MENU_MEAS_PARAM_UC;

		// заполнение массива общих неисправностей и предупреждений
		sParam.glb.status.faultText[0] =  fcGlbFault0001;
		sParam.glb.status.faultText[1] =  fcGlbFault0002;
		sParam.glb.status.faultText[2] =  fcGlbFault0004;
		sParam.glb.status.faultText[3] =  fcGlbFault0008;
		sParam.glb.status.faultText[4] =  fcGlbFault0010;
		sParam.glb.status.faultText[5] =  fcGlbFault0020rzsk;
		sParam.glb.status.faultText[6] =  fcGlbFault0040;
		sParam.glb.status.faultText[7] =  fcGlbFault0080;
		sParam.glb.status.faultText[8] =  fcGlbFault0100;
		sParam.glb.status.faultText[9] =  fcGlbFault0200;
		sParam.glb.status.faultText[10] = fcGlbFault0400rzsk;
		sParam.glb.status.faultText[11] = fcGlbFault0800;
		sParam.glb.status.faultText[12] = fcGlbFault1000;
		sParam.glb.status.faultText[13] = fcGlbFault2000;
		sParam.glb.status.faultText[14] = fcGlbFault4000;
		sParam.glb.status.faultText[15] = fcUnknownFault;

		sParam.glb.status.warningText[0] = fcGlbWarning01;
		sParam.glb.status.warningText[1] = fcGlbWarning02;
		sParam.glb.status.warningText[2] = fcUnknownWarning;
		sParam.glb.status.warningText[3] = fcUnknownWarning;
		sParam.glb.status.warningText[4] = fcGlbWarning10;
		sParam.glb.status.warningText[5] = fcGlbWarning20;
		sParam.glb.status.warningText[6] = fcGlbWarning40;
		sParam.glb.status.warningText[7] = fcUnknownWarning;

		// отключение защиты
		sParam.def.status.setEnable(false);

		// включение приемника
		// и заполнение массивов неисправностей и предупреждений
		sParam.prm.status.setEnable(true);
		sParam.prm.status.faultText[0] =  fcPrmFault0001rzsk;
		sParam.prm.status.faultText[1] =  fcPrmFault0002rzsk;
		sParam.prm.status.faultText[2] =  fcPrmFault0004rzsk;
		sParam.prm.status.faultText[3] =  fcPrmFault0008rzsk;
		sParam.prm.status.faultText[4] =  fcUnknownFault;
		sParam.prm.status.faultText[5] =  fcUnknownFault;
		sParam.prm.status.faultText[6] =  fcUnknownFault;
		sParam.prm.status.faultText[7] =  fcUnknownFault;
		sParam.prm.status.faultText[8] =  fcPrmFault0100rzsk;
		sParam.prm.status.faultText[9] =  fcPrmFault0200rzsk;
		sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
		sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
		sParam.prm.status.faultText[12] = fcPrmFault1000rzsk;
		sParam.prm.status.faultText[13] = fcUnknownFault;
		sParam.prm.status.faultText[14] = fcUnknownFault;
		sParam.prm.status.faultText[15] = fcPrmFault8000rzsk;

		sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
		sParam.prm.status.warningText[1] = fcUnknownWarning;
		sParam.prm.status.warningText[2] = fcUnknownWarning;
		sParam.prm.status.warningText[3] = fcUnknownWarning;
		sParam.prm.status.warningText[4] = fcUnknownWarning;
		sParam.prm.status.warningText[5] = fcUnknownWarning;
		sParam.prm.status.warningText[6] = fcUnknownWarning;
		sParam.prm.status.warningText[7] = fcUnknownWarning;

		// включение передатчика
		// и заполнение массивов неисправностей и предупреждений
		sParam.prd.status.setEnable(true);
		sParam.prd.status.faultText[0] =  fcPrdFault0001rzsk;
		sParam.prd.status.faultText[1] =  fcPrdFault0002rzsk;
		sParam.prd.status.faultText[2] =  fcUnknownFault;
		sParam.prd.status.faultText[3] =  fcUnknownFault;
		sParam.prd.status.faultText[4] =  fcUnknownFault;
		sParam.prd.status.faultText[5] =  fcUnknownFault;
		sParam.prd.status.faultText[6] =  fcUnknownFault;
		sParam.prd.status.faultText[7] =  fcUnknownFault;
		sParam.prd.status.faultText[8] =  fcPrdFault0100rzsk;
		sParam.prd.status.faultText[9] =  fcPrdFault0200rzsk;
		sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
		sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
		sParam.prd.status.faultText[12] = fcUnknownFault;
		sParam.prd.status.faultText[13] = fcUnknownFault;
		sParam.prd.status.faultText[14] = fcUnknownFault;
		sParam.prd.status.faultText[15] = fcUnknownFault;

		sParam.prd.status.warningText[0] = fcUnknownWarning;
		sParam.prd.status.warningText[1] = fcUnknownWarning;
		sParam.prd.status.warningText[2] = fcUnknownWarning;
		sParam.prd.status.warningText[3] = fcUnknownWarning;
		sParam.prd.status.warningText[4] = fcUnknownWarning;
		sParam.prd.status.warningText[5] = fcUnknownWarning;
		sParam.prd.status.warningText[6] = fcUnknownWarning;
		sParam.prd.status.warningText[7] = fcUnknownWarning;

	}

	return true;
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

/**	Вывод в указанном месте отображаемого параметра.
 * 	@param poz Текущая позиция (0..5, 0 первая строка слева, 5 - третья справа)
 * 	@param par Второй отображаемый параметр
 * 	@return Нет
 */
void
clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par)
{
	// смещение в буфере
	if (poz < 6)
	{
		poz = (poz * 10) + 1;

		switch(par)
		{
			case MENU_MEAS_PARAM_DATE:
				snprintf_P(&vLCDbuf[poz], 9, fcDate,
						sParam.dataTime.getDay(),
						sParam.dataTime.getMonth(),
						sParam.dataTime.getYear());
				break;
			case MENU_MEAS_PARAM_TIME:
				snprintf_P(&vLCDbuf[poz], 9, fcTime,
						sParam.dataTime.getHour(),
						sParam.dataTime.getMinute(),
						sParam.dataTime.getSecond());
				break;
			case MENU_MEAS_PARAM_UZ:
				snprintf_P(&vLCDbuf[poz], 9, fcUz,
						sParam.measParam.getVoltageDef());
				break;
			case MENU_MEAS_PARAM_UC:
				snprintf_P(&vLCDbuf[poz], 11, fcUcf,
						sParam.measParam.getVoltageCf());
				break;
			case MENU_MEAS_PARAM_UOUT:
				snprintf_P(&vLCDbuf[poz], 11, fcUout,
						sParam.measParam.getVoltageOutInt(),
						sParam.measParam.getVoltageOutFract());
				break;
			case MENU_MEAS_PARAM_IOUT:
				snprintf_P(&vLCDbuf[poz], 11, fcIout,
						sParam.measParam.getCurrentOut());
				break;
			case MENU_MEAS_PARAM_ROUT:
				snprintf_P(&vLCDbuf[poz], 11, fcRout,
						sParam.measParam.getResistOut());
				break;
			case MENU_MEAS_PARAM_UN:
				snprintf_P(&vLCDbuf[poz], 11, fcUn,
						sParam.measParam.getVoltageNoise());
				break;
			default:
				// ничего не делаем
				break;
		}
	}
}

/**	Вывод на экран текущего состояния устройства.
 * 	Если есть общие неисправности - выводится поочередно расшифровка самой
 * 	приоритетной и код (даже если одна).
 * 	Далее проверяется наличие неисправности устройства и если есть, выводится
 * 	по тому же алгоритму.
 * 	Если есть общее предупреждение - выводится почередно состояние и расшифровка
 * 	данного предупреждения. Если предупреждений несколько, выводится только код.
 * 	Далее проверяется наличие предупреждения для устройства и если есть,
 * 	выодится по тому же алгоритму.
 * 	Если небыло никаких неисправностей/предупреждений, то выводится текущее
 * 	состояние и режим работы устройства.
 * 	@param poz Начальная позиция в буфере данных ЖКИ
 * 	@param device Данные для текущего устройства
 *	@return Нет
 */
void
clMenu::printDevicesStatus(uint8_t poz, TDeviceStatus *device)
{
	PGM_P *text;
	uint_fast8_t x = 0;
	uint_fast16_t y = 0;

	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;

	if (sParam.glb.status.getNumFaults() != 0)
	{
		if (blink_)
		{
			text = sParam.glb.status.faultText;
			x = sParam.glb.status.getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = sParam.glb.status.getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'g', y);
		}
	}
	else if (device->getNumFaults() != 0)
	{
		if (blink_)
		{
			text = device->faultText;
			x = device->getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = device->getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'l', y);
		}
	}
	else if ( (sParam.glb.status.getNumWarnings() != 0) && (blink_) )
	{
		if (sParam.glb.status.getNumWarnings() == 1)
		{
			text = sParam.glb.status.warningText;
			x = sParam.glb.status.getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = sParam.glb.status.getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else if ( (device->getNumWarnings()!= 0) && (blink_) )
	{
		if (device->getNumWarnings() == 1)
		{
			text = device->warningText;
			x = device->getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = device->getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else
	{
		text = device->stateText;
		poz += snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
		snprintf_P(&vLCDbuf[poz], 9, text[device->getState()]);
	}
/*
	if (sParam.glb.status.getNumFaults() != 0)
	{
		// вывод общей неисправности для всех имеющихся устройств

		uint_fast8_t tmp =  sParam.glb.status.getFault();
		uint_fast16_t faults = sParam.glb.status.getFaults();

		if (sParam.def.status.isEnable())
		{
			if (blink_)
				snprintf_P(&vLCDbuf[60], 21, fcDef, fcGlbError[tmp], fcNull);
			else
			{
				snprintf_P(&vLCDbuf[60], 21, fcDef, fcNull, fcNull);
				snprintf_P(&vLCDbuf[64], 17, fcGlbFaults, faults, fcNull);
			}
		}

		if (sParam.prm.status.isEnable())
		{
			if (blink_)
				snprintf_P(&vLCDbuf[80], 21, fcPrm, fcGlbError[tmp], fcNull);
			else
			{
				snprintf_P(&vLCDbuf[80], 21, fcPrm, fcNull, fcNull);
				snprintf_P(&vLCDbuf[84], 17, fcGlbFaults, faults, fcNull);
			}
		}

		if (sParam.prd.status.isEnable())
		{
			if (blink_)
				snprintf_P(&vLCDbuf[100], 21, fcPrd, fcGlbError[tmp], fcNull);
			else
			{
				snprintf_P(&vLCDbuf[100], 21, fcPrd, fcNull, fcNull);
				snprintf_P(&vLCDbuf[104], 17, fcGlbFaults, faults, fcNull);
			}
		}
	}
	else if ( (sParam.glb.status.getNumWarnings() != 0) && (blink_) )
	{
		// вывод общего предупреждения для всех устройств

		if (sParam.glb.status.getNumWarnings() == 1)
		{
			uint_fast8_t warning =  sParam.glb.status.getWarning();

			if (sParam.def.status.isEnable())
				snprintf_P(&vLCDbuf[60],21,fcDef,fcGlbWarning[warning],fcNull);

			if (sParam.prm.status.isEnable())
				snprintf_P(&vLCDbuf[80],21,fcPrm,fcGlbWarning[warning],fcNull);

			if (sParam.prd.status.isEnable())
				snprintf_P(&vLCDbuf[100],21,fcPrd,fcGlbWarning[warning],fcNull);
		}
		else
		{
			uint_fast16_t warnings =  sParam.glb.status.getWarnings();

			if (sParam.def.status.isEnable())
			{
				snprintf_P(&vLCDbuf[60], 21, fcDef, fcNull, fcNull);
				snprintf_P(&vLCDbuf[64], 17, fcGlbWarnings, warnings, fcNull);

			}

			if (sParam.prm.status.isEnable())
			{
				snprintf_P(&vLCDbuf[80], 21, fcPrm, fcNull, fcNull);
				snprintf_P(&vLCDbuf[84], 17, fcGlbWarnings, warnings, fcNull);

			}

			if (sParam.prd.status.isEnable())
			{
				snprintf_P(&vLCDbuf[100], 21, fcPrd, fcNull, fcNull);
				snprintf_P(&vLCDbuf[104], 17, fcGlbWarnings, warnings, fcNull);

			}
		}
	}*/

}

/** Уровень начальный
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlStart()
{
	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorEnable_ = false;
		lineParam_ = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	// вывод на экран измеряемых параметров
	for(uint_fast8_t i = 0; i < 6; i++)
		printMeasParam(i, measParam[i]);

//	if (sParam.typeDevice == AVANT_R400)
//	{
//		// вывод режима\состояния устройств
//		snprintf_P(&vLCDbuf[60], 21, fcDef,
//				fcRegime[sParam.def.status.getRegime()],
//				fcDefSost[sParam.def.status.getState()]);
//	}
//	else if (sParam.typeDevice == AVANT_RZSK)
//	{
//		// вывод режима\состояния устройств
//		snprintf_P(&vLCDbuf[60], 21, fcDef,
//				fcRegime[sParam.def.status.getRegime()],
//				fcDefSost[sParam.def.status.getState()]);
//		snprintf_P(&vLCDbuf[80], 21, fcPrm,
//				fcRegime[sParam.prm.status.getRegime()], //
//				fcPrmSost[sParam.prm.status.getState()]);
//		snprintf_P(&vLCDbuf[100], 21, fcPrd,
//				fcRegime[sParam.prd.status.getRegime()], //
//				fcPrdSost[sParam.prd.status.getState()]);
//	}
//	else if (sParam.typeDevice == AVANT_K400)
//	{
//		// вывод режима\состояния устройств
//		snprintf_P(&vLCDbuf[80], 21, fcPrm,
//				fcRegime[sParam.prm.status.getRegime()], //
//				fcPrmSost[sParam.prm.status.getState()]);
//		snprintf_P(&vLCDbuf[100], 21, fcPrd,
//				fcRegime[sParam.prd.status.getRegime()], //
//				fcPrdSost[sParam.prd.status.getState()]);
//	}
//	else if (sParam.typeDevice == AVANT_K400_OPTIC)
//	{
//		if (sParam.glb.status.getNumFaults() != 0)
//		{
//			uint_fast8_t tmp = 0;
//
//			tmp = sParam.glb.status.getFault();
//			snprintf_P(&vLCDbuf[80], 21, fcPrm, fcGlbError[tmp], fcNull);
//			snprintf_P(&vLCDbuf[100], 21, fcPrd, fcGlbError[tmp], fcNull);
//
//		}
//		else if (sParam.glb.status.getNumWarnings() != 0)
//		{
//			uint_fast8_t tmp = 0;
//
//			tmp = sParam.glb.status.getWarning();
//			snprintf_P(&vLCDbuf[80], 21, fcPrm, fcGlbWarning[tmp], fcNull);
//			snprintf_P(&vLCDbuf[100], 21, fcPrd, fcGlbWarning[tmp], fcNull);
//		}
//		else
//		{
//			// вывод режима\состояния устройств
//			if (sParam.prm.status.getNumFaults() == 0)
//			{
//				if (sParam.prm.status.getNumWarnings() == 0)
//				{
//					snprintf_P(&vLCDbuf[80], 21, fcPrm,
//							fcRegime[sParam.prm.status.getRegime()],
//							fcPrmSost[sParam.prm.status.getState()]);
//				}
//				else
//				{
//					uint_fast8_t tmp = 0;
//
//					tmp = sParam.prm.status.getWarning();
//					snprintf_P(&vLCDbuf[80], 21, fcPrm,
//							fcPrmWarningOptic[tmp], fcNull);
//				}
//			}
//			else
//			{
//				uint_fast8_t tmp = 0;
//
//				tmp = sParam.prm.status.getFault();
//				snprintf_P(&vLCDbuf[80], 21, fcPrm, fcPrmError[tmp], fcNull);
//
//			}
//
//			if (sParam.prd.status.getNumFaults() == 0)
//			{
//				if (sParam.prd.status.getNumWarnings() == 0)
//				{
//					snprintf_P(&vLCDbuf[100], 21, fcPrd,
//							fcRegime[sParam.prd.status.getRegime()],
//							fcPrdSost[sParam.prd.status.getState()]);
//				}
//				else
//				{
//					uint_fast8_t tmp = 0;
//
//					tmp = sParam.prd.status.getWarning();
//					snprintf_P(&vLCDbuf[100], 21, fcPrd,
//							fcPrdWarning[tmp], fcNull);
//				}
//			}
//			else
//			{
//				uint_fast8_t tmp = 0;
//
//				tmp = sParam.prd.status.getFault();
//				snprintf_P(&vLCDbuf[100], 21, fcPrd, fcPrdError[tmp], fcNull);
//
//			}
//		}
//	}
	if (sParam.def.status.isEnable())
		printDevicesStatus(60, &sParam.def.status);
	if (sParam.prm.status.isEnable())
		printDevicesStatus(80, &sParam.prm.status);
	if (sParam.prd.status.isEnable())
		printDevicesStatus(100, &sParam.prd.status);

	switch(key_)
	{
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_RES_IND:
//			com = 1;
			break;

		case KEY_FUNC_ENTER:
//			com = 2;
			break;

		case KEY_FUNC_RESET:
//			com = 3;
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


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_RIGHT:
			switch(cursorLine_)
			{
				case 1:
					lvlMenu = &clMenu::lvlJournal;
					lvlCreate_ = true;
					break;
				case 2:
					lvlMenu = &clMenu::lvlControl;
					lvlCreate_ = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetup;
					lvlCreate_ = true;
					break;
				case 5:
					lvlMenu = &clMenu::lvlInfo;
					lvlCreate_ = true;
					break;
			}
			break;


		case KEY_LEFT:
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
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


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	switch(key_)
	{
		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		if (sParam.typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
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

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlJournalEvent;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt2)
			{

				lvlMenu = &clMenu::lvlJournalDef;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlJournalPrm;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlJournalPrd;
				lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		else
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 1)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);


		if ((sParam.typeDevice == AVANT_RZSK) ||
				(sParam.typeDevice == AVANT_R400))
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
			punkt[numPunkt++] = punkt5;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
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

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			switch(cursorLine_)
			{
				case 2:
					lvlMenu = &clMenu::lvlSetupDT;
					lvlCreate_ = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetupParam;
					lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		if (sParam.typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 20, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlSetupParamDef;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupParamPrm;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParamPrd;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine_ == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine_ == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine_ == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine_ == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine_ == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 7)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		if (sParam.typeDevice == AVANT_K400_OPTIC)
			cursorLine_ = 2;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine_ == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine_ == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine_ == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine_ == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine_ == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;

			if (sParam.typeDevice == AVANT_K400)
			{
				if (cursorLine_ == 6)
					cursorLine_ = 5;
			}
			else if (sParam.typeDevice == AVANT_K400_OPTIC)
			{
				if (cursorLine_ <= 2)
					cursorLine_ = 2;
				else if ((cursorLine_ == 3) || (cursorLine_ == 6))
				{
					cursorLine_--;
				}
			}
			else if (sParam.typeDevice == AVANT_R400)
			{
				if (cursorLine_ == 2)
					cursorLine_ = 1;
				else if ((cursorLine_ == 4) || (cursorLine_ == 5))
					cursorLine_ = 3;
			}

			break;
		case KEY_DOWN:
			if (cursorLine_ < 7)
				cursorLine_++;

			if (sParam.typeDevice == AVANT_K400)
			{
				if (cursorLine_ == 6)
					cursorLine_ = 7;
			}
			else if (sParam.typeDevice == AVANT_K400_OPTIC)
			{
				if ((cursorLine_ == 3) || (cursorLine_ == 6))
				{
					cursorLine_++;
				}
			}
			else if (sParam.typeDevice == AVANT_R400)
			{
				if (cursorLine_ == 2)
					cursorLine_ = 3;
				else if ((cursorLine_ == 4) || (cursorLine_ == 5))
					cursorLine_ = 6;
			}
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

uint8_t
clMenu::txCommand()
{
//	uint_fast8_t t = com;
//	com = 0;

	return 0;
}

uint8_t
clMenu::getNumError(uint16_t val)
{
	uint_fast8_t tmp = 0;

	while ((val & 0x0001) == 0)
	{
		tmp++;
		val >>= 1;
	}

	return tmp;
}
