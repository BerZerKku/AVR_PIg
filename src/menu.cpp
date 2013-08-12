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

/// кол-во строк данных отображаемых на экране
#define NUM_TEXT_LINES 6

//clMenu::clMenu(clProtocolS *protocol) : pProtocol(protocol)
clMenu::clMenu()
{
	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;


	lvlCreate_ = true;

	blink_ = false;

	curCom_ = 0;

	// курсор неактивен
	cursorEnable_= false;
	cursorLine_ = 0;
	numPunkts_ = 0;

	// нажатой кнопки еще нет
	key_ = KEY_NO;

	// тип устройства еще не известен
	sParam.typeDevice = AVANT_NO;

	// связи с БСП еще нет
	connectionBsp_ = false;

	// измеряемые параметры
	for(uint_fast8_t i = 0; i < 6; i++)
		measParam[i] = MENU_MEAS_PARAM_NO;

	// для начала заполним текстовые надписи неисправностей и предупреждений
	// для всех устройств "неизвестными" состояниями
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
	// массив должен быть заполнен полностью и последним всегда должно
	// быть fcUnknownSost
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
	static const char fcNoConnectBsp[] PROGMEM = " Нет связи с БСП!!! ";

	// Счетчик времени до переинициализации ЖКИ
	static uint8_t reInit = false;

	blink_ =  ((reInit % 8) < 4) ? false : true;


	// Переинициализация дисплея
	if (reInit > 0)
	{
		reInit--;
	}
	else
	{
		reInit = TIME_TO_REINIT_LCD - 1;
		vLCDinit();
	}

	if (!sParam.device)
		setTypeDevice();

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

	// очистка текстового буфера
	clearTextBuf();

	// выведем сообщение, если текущий тип аппарата не определен
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
	bool status = false;


	sParam.glb.status.setEnable(true);

	// если необходимый тип аппарата небыл передан, сделаем вывод исходя
	// из текущих настроек.
	if (device == AVANT_NO)
	{
		if (sParam.def.status.isEnable())
		{
			if ((sParam.prm.status.isEnable()) ||
					(sParam.prd.status.isEnable()))
			{
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTIC)
					device = AVANT_RZSK_OPTIC;
				else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_RZSK;
			}
			else
			{
				// !!! тут еще должно быть определение аппарата для МОСКВЫ
				// !!! на наднный момент можно опреджелить по версии
				// !!! ПО для МК БСП - 0x0F
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_R400;
			}
		}
		else
		{
			if ((sParam.prd.status.isEnable()) ||
					(sParam.prm.status.isEnable()))
			{
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTIC)
					device = AVANT_K400_OPTIC;
				else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_K400;
			}
		}

		// если текущее устройство совпадает с новым, то ничего не делаем
		// иначе прыгаем на начальный уровень
		if (device == sParam.typeDevice)
			status = true;
		else
		{
			sParam.typeDevice = device;
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
		}
	}

	if (!status)
	{
		if (device == AVANT_K400)
		{
			sParam.typeDevice = device;

			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			measParam[2] = MENU_MEAS_PARAM_UOUT;
			measParam[3] = MENU_MEAS_PARAM_IOUT;
			measParam[4] = MENU_MEAS_PARAM_UN;
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

			status = true;
		}
		else if (device == AVANT_RZSK)
		{
			sParam.typeDevice = device;

			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			measParam[2] = MENU_MEAS_PARAM_UOUT;
			measParam[3] = MENU_MEAS_PARAM_IOUT;
			measParam[4] = MENU_MEAS_PARAM_UZ;
			measParam[5] = MENU_MEAS_PARAM_UC;

			// включение защиты
			// и заполнение массивов неисправностей и предупреждений
			sParam.def.status.setEnable(true);
			sParam.def.status.faultText[0] =  fcDefFault0001;
			sParam.def.status.faultText[1] =  fcDefFault0002;
			sParam.def.status.faultText[2] =  fcDefFault0004;
			sParam.def.status.faultText[3] =  fcDefFault0008;
			sParam.def.status.faultText[4] =  fcUnknownFault;
			sParam.def.status.faultText[5] =  fcUnknownFault;
			sParam.def.status.faultText[6] =  fcUnknownFault;
			sParam.def.status.faultText[7] =  fcUnknownFault;
			sParam.def.status.faultText[8] =  fcDefFault0100;
			sParam.def.status.faultText[9] =  fcDefFault0200;
			sParam.def.status.faultText[10] = fcUnknownFault;
			sParam.def.status.faultText[11] = fcDefFault0800;
			sParam.def.status.faultText[12] = fcUnknownFault;
			sParam.def.status.faultText[13] = fcDefFault2000;
			sParam.def.status.faultText[14] = fcDefFault4000;
			sParam.def.status.faultText[15] = fcUnknownFault;

			sParam.def.status.warningText[0] = fcDefWarning01rzsk;
			sParam.def.status.warningText[1] = fcDefWarning02;
			sParam.def.status.warningText[2] = fcUnknownWarning;
			sParam.def.status.warningText[3] = fcUnknownWarning;
			sParam.def.status.warningText[4] = fcUnknownWarning;
			sParam.def.status.warningText[5] = fcUnknownWarning;
			sParam.def.status.warningText[6] = fcUnknownWarning;
			sParam.def.status.warningText[7] = fcUnknownWarning;

			// заполнение массива общих неисправностей и предупреждений
			sParam.glb.status.faultText[0] =  fcGlbFault0001;
			sParam.glb.status.faultText[1] =  fcGlbFault0002;
			sParam.glb.status.faultText[2] =  fcGlbFault0004;
			sParam.glb.status.faultText[3] =  fcGlbFault0008;
			sParam.glb.status.faultText[4] =  fcGlbFault0010;
			sParam.glb.status.faultText[5] =  fcUnknownFault;
			sParam.glb.status.faultText[6] =  fcUnknownFault;
			sParam.glb.status.faultText[7] =  fcUnknownFault;
			sParam.glb.status.faultText[8] =  fcGlbFault0100;
			sParam.glb.status.faultText[9] =  fcGlbFault0200;
			sParam.glb.status.faultText[10] = fcGlbFault0400rzsk;
			sParam.glb.status.faultText[11] = fcGlbFault0800;
			sParam.glb.status.faultText[12] = fcGlbFault1000;
			sParam.glb.status.faultText[13] = fcUnknownFault;
			sParam.glb.status.faultText[14] = fcUnknownFault;
			sParam.glb.status.faultText[15] = fcUnknownFault;

			sParam.glb.status.warningText[0] = fcGlbWarning01;
			sParam.glb.status.warningText[1] = fcUnknownWarning;
			sParam.glb.status.warningText[2] = fcUnknownWarning;
			sParam.glb.status.warningText[3] = fcUnknownWarning;
			sParam.glb.status.warningText[4] = fcUnknownWarning;
			sParam.glb.status.warningText[5] = fcUnknownWarning;
			sParam.glb.status.warningText[6] = fcUnknownWarning;
			sParam.glb.status.warningText[7] = fcUnknownWarning;

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

			status = true;

		}
		else if (device == AVANT_R400)
		{
			sParam.typeDevice = device;

			sParam.prm.status.setEnable(false);
			sParam.prd.status.setEnable(false);

			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			measParam[2] = MENU_MEAS_PARAM_UOUT;
			measParam[3] = MENU_MEAS_PARAM_IOUT;
			measParam[4] = MENU_MEAS_PARAM_UZ;
			measParam[5] = MENU_MEAS_PARAM_UC;

			// включение защиты
			// и заполнение массивов неисправностей и предупреждений
			sParam.def.status.setEnable(true);
			sParam.def.status.faultText[0] =  fcDefFault0001;
			sParam.def.status.faultText[1] =  fcDefFault0002;
			sParam.def.status.faultText[2] =  fcDefFault0004;
			sParam.def.status.faultText[3] =  fcDefFault0008;
			sParam.def.status.faultText[4] =  fcDefFault0010;
			sParam.def.status.faultText[5] =  fcDefFault0020;
			sParam.def.status.faultText[6] =  fcDefFault0040;
			sParam.def.status.faultText[7] =  fcDefFault0080;
			sParam.def.status.faultText[8] =  fcDefFault0100;
			sParam.def.status.faultText[9] =  fcDefFault0200;
			sParam.def.status.faultText[10] = fcDefFault0400;
			sParam.def.status.faultText[11] = fcDefFault0800;
			sParam.def.status.faultText[12] = fcDefFault1000;
			sParam.def.status.faultText[13] = fcDefFault2000;
			sParam.def.status.faultText[14] = fcDefFault4000;
			sParam.def.status.faultText[15] = fcDefFault8000;

			sParam.def.status.warningText[0] = fcDefWarning01;
			sParam.def.status.warningText[1] = fcDefWarning02;
			sParam.def.status.warningText[2] = fcDefWarning04;
			sParam.def.status.warningText[3] = fcDefWarning08;
			sParam.def.status.warningText[4] = fcUnknownWarning;
			sParam.def.status.warningText[5] = fcUnknownWarning;
			sParam.def.status.warningText[6] = fcUnknownWarning;
			sParam.def.status.warningText[7] = fcUnknownWarning;

			status = true;
		}
	}

	if ( (!status) || (device == AVANT_NO) )
	{
		// если полученные данные не подходят ни под один имеющийся тип
		// на экране отображается ошибка
		sParam.typeDevice = AVANT_NO;
		// в случае неизвестного типа устройства, отключим все
		measParam[0] = MENU_MEAS_PARAM_TIME;
		measParam[1] = MENU_MEAS_PARAM_DATE;
		for(uint_fast8_t i = 2; i < 6; i++)
			measParam[i] = MENU_MEAS_PARAM_NO;
		sParam.def.status.setEnable(false);
		sParam.prm.status.setEnable(false);
		sParam.prd.status.setEnable(false);

		lvlMenu = &clMenu::lvlError;
	}

	// сброс флага необходимости проверки типа аппарата
	sParam.device = true;

	return status;
}

/**	Возвращает имеющуюся команду на исполнение.
 * 	Сначала проверяется срочная команда, если ее нет идет перебор текущих.
 * 	Каждый цикл опрашиваются состояния, неисправности и одна из текущих команд.
 * 	@param Нет
 * 	@return Команда
 */
eGB_COM
clMenu::getTxCommand()
{
	static uint8_t cnt = 0;
	eGB_COM com = sParam.txComBuf.getFastCom();

	if (com == GB_COM_NO)
	{
		cnt++;
		if (cnt == 1)
			com = GB_COM_GET_SOST;
		else if (cnt == 2)
			com = GB_COM_GET_FAULT;
		else
		{
			com = sParam.txComBuf.getCom();
			cnt = 0;
		}
	}

	return com;
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
		poz += snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]) + 1;
		snprintf_P(&vLCDbuf[poz], 9, text[device->getState()]);
	}
}

/**	Dывод в пунтке меню "Режим" текущего режима устройств
* 	@param poz Начальная позиция в буфере данных ЖКИ
 * 	@param device Данные для текущего устройства
 *	@return Нет
 */
void
clMenu::printDevicesRegime(uint8_t poz, TDeviceStatus *device)
{
	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;
	poz += snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]) + 1;
}

/** Уровень "Ошибочный тип аппарата"
 * 	@param Нет
 * 	@return Нет
 */
void
clMenu::lvlError()
{
	static const char fcNoTypeDevice0[] PROGMEM = "    Тип аппарата    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   не определен!!!  ";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorEnable_ = false;
		vLCDclear();
		// только одна строка отводится под вывод параметров
		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom(GB_COM_GET_VERS);
	}

	// вывод на экран измеряемых параметров
	printMeasParam(0, measParam[0]);
	printMeasParam(1, measParam[1]);

	snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
	snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);

	if (sParam.typeDevice != AVANT_NO)
	{
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
	}
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
		numPunkts_ = 0;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// доплнительные команды
		// в ВЧ варианте есть измеряемые параметры
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom(GB_COM_GET_TIME);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom(GB_COM_GET_MEAS);
		// в Р400 нужна информация по совместимости и типу АК
		if (sParam.typeDevice == AVANT_R400)
		{
			sParam.txComBuf.addCom(GB_COM_DEF_GET_TYPE_AC);
		}
	}

	// вывод на экран измеряемых параметров
	for(uint_fast8_t i = 0; i < 6; i++)
		printMeasParam(i, measParam[i]);

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable())
	{
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;
	}
	if (sParam.prm.status.isEnable())
	{
		printDevicesStatus(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable())
	{
		printDevicesStatus(poz, &sParam.prd.status);
		poz += 20;
	}

	switch(key_)
	{
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_RES_IND:
			sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND);
			break;

		case KEY_FUNC_ENTER:
			sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER);
			break;

		case KEY_FUNC_RESET:
			sParam.txComBuf.setByte(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
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
	static char punkt1[] PROGMEM = "%d. Журнал";
	static char punkt2[] PROGMEM = "%d. Управление";
	static char punkt3[] PROGMEM = "%d. Настройка";
	static char punkt4[] PROGMEM = "%d. Тест";
	static char punkt5[] PROGMEM = "%d. Информация";

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		punkt_[num++] = punkt5;
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title );
	printPunkts();

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_ENTER:
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

		case KEY_CANCEL:
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
	static char title[]  PROGMEM = "Меню\\Информация";
	static char bspMcu[] PROGMEM = "БСП MCU : %02X.%02x";
	static char bspDsp[] PROGMEM = "БСП DSP : %02X.%02x";
	static char piMcu[]  PROGMEM = "ПИ  MCU : %02X.%02x";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	uint16_t vers = sParam.glb.getVersBsp();
	snprintf_P(&vLCDbuf[20], 21, bspMcu, (uint8_t)(vers >> 8), (uint8_t) vers);
	vers = sParam.glb.getVersDsp();
	snprintf_P(&vLCDbuf[40], 21, bspDsp, (uint8_t)(vers >> 8), (uint8_t) vers);
	snprintf_P(&vLCDbuf[60], 21, piMcu,  (uint8_t)(VERS >> 8), (uint8_t) VERS);

	switch(key_)
	{
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Меню\\Журнал";
	static char punkt1[] PROGMEM = "%d. События";
	static char punkt2[] PROGMEM = "%d. Защта";
	static char punkt3[] PROGMEM = "%d. Приемник";
	static char punkt4[] PROGMEM = "%d. Передатчик";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);


		eGB_TYPE_DEVICE type = sParam.typeDevice;
		uint8_t num = 0;
		sParam.txComBuf.clear();
		// активация необходимых пунктов меню и соответствующих им команд
		if (type == AVANT_R400)
		{

			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom(GB_COM_GET_JRN_CNT);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom(GB_COM_DEF_GET_JRN_CNT);
		}
		else if (type == AVANT_RZSK)
		{
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom(GB_COM_GET_JRN_CNT);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom(GB_COM_DEF_GET_JRN_CNT);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom(GB_COM_PRM_GET_JRN_CNT);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom(GB_COM_PRD_GET_JRN_CNT);
		}
		else if (type == AVANT_K400)
		{
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom(GB_COM_GET_JRN_CNT);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom(GB_COM_PRM_GET_JRN_CNT);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom(GB_COM_PRD_GET_JRN_CNT);
		}
		else if (type == AVANT_K400_OPTIC)
		{
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom(GB_COM_GET_JRN_CNT);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom(GB_COM_PRM_GET_JRN_CNT);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom(GB_COM_PRD_GET_JRN_CNT);
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	uint16_t tmp = sParam.glb.getNumJrnEntry();
	sDebug.byte1 = tmp >> 8;
	sDebug.byte2 = tmp;

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_ENTER:
			if (punkt_[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlJournalEvent;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt2)
			{

				lvlMenu = &clMenu::lvlJournalDef;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlJournalPrm;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlJournalPrd;
				lvlCreate_ = true;
			}
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

		// доплнительные команды
		sParam.txComBuf.clear();
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

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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

		// доплнительные команды
		sParam.txComBuf.clear();
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

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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

		// доплнительные команды
		sParam.txComBuf.clear();
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

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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

		// доплнительные команды
		sParam.txComBuf.clear();
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

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Меню\\Управление";
	// %d - может быть двухзначным, учесть для макс. кол-ва символов !
	//							   	"01234567890123456789"
	static char punkt02[] PROGMEM = "%d. Пуск удаленного";
	static char punkt03[] PROGMEM = "%d. Сброс своего";
	static char punkt04[] PROGMEM = "%d. Сброс удаленного";
	static char punkt05[] PROGMEM = "%d. Вызов";
	static char punkt06[] PROGMEM = "%d. Пуск налад. вкл.";
	static char punkt07[] PROGMEM = "%d. Пуск налад. выкл";
	static char punkt08[] PROGMEM =	"%d. АК пуск";
	static char punkt09[] PROGMEM = "%d. Пуск удален. МАН";
	static char punkt10[] PROGMEM =	"%d. АК контр.провер.";
	static char punkt11[] PROGMEM = "%d. Сброс АК";
	static char punkt12[] PROGMEM =	"%d. Пуск АК свой";
	static char punkt13[] PROGMEM =	"%d. Пуск АК удаленн.";
	static char punkt14[] PROGMEM = "%d. Пуск ПРД";
	static char punkt15[] PROGMEM = "%d. АК автоматическ.";
	static char punkt16[] PROGMEM = "%d. АК ускоренный";
	static char punkt17[] PROGMEM = "%d. АК выключен";
	static char punkt18[] PROGMEM = "%d. АК испытания";
	static char punkt19[] PROGMEM = "%d. АК нормальный";
	static char punkt20[] PROGMEM = "%d. АК беглый";
	static char punkt21[] PROGMEM = "%d. АК односторонний";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_R400)
		{
			punkt_[num++] = punkt03;
			punkt_[num++] = punkt04;
			punkt_[num++] = punkt02;
			punkt_[num++] = punkt05;
			// !!! Р400
			// !!! Добавить автоконтроли и учесть совместимость
			// !!! Учесть стостояние наладочного пуска (вкл/выкл)

		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt03;
			// !!! РЗСК
			// !!! Учесть стостояние наладочного пуска (вкл/выкл)
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			punkt_[num++] = punkt03;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			punkt_[num++] = punkt03;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom(GB_COM_DEF_GET_TYPE_AC);
		sParam.txComBuf.addCom(GB_COM_GET_COM_PRD_KEEP);

		// !!! Р400.
		// !!! Добавить переформирование меню в случае изменения типа АК
		// !!! или типа удаленного аппарата
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
		{
			PGM_P p = punkt_[cursorLine_ - 1];

			if (p == punkt02)
			{
				sParam.txComBuf.setByte(GB_CONTROL_PUSK_UD_1);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			else if (p == punkt03)
			{
				sParam.txComBuf.setByte(GB_CONTROL_RESET_SELF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			else if (p == punkt04)
			{
				sParam.txComBuf.setByte(GB_CONTROL_RESET_UD_1);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			else if (p == punkt05)
			{
				sParam.txComBuf.setByte(GB_CONTROL_CALL);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			else if (p == punkt06)
			{
				sParam.txComBuf.setByte(GB_CONTROL_PUSK_ON);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			else if (p == punkt07)
			{
				sParam.txComBuf.setByte(GB_CONTROL_PUSK_OFF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
		}
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
	static char title[]  PROGMEM = "Меню\\Настройка";
	static char punkt1[] PROGMEM = "%d. Режим";
	static char punkt2[] PROGMEM = "%d. Время и дата";
	static char punkt3[] PROGMEM = "%d. Параметры";
	static char punkt4[] PROGMEM = "%d. Пароль";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_ENTER:
		{
			PGM_P p = punkt_[cursorLine_ - 1];
			if (p == punkt1)
			{
				lvlMenu = &clMenu::lvlRegime;
				lvlCreate_ = true;
			}
			else if (p == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupDT;
				lvlCreate_ = true;
			}
			else if (p == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParam;
				lvlCreate_ = true;
			}
		}
		break;

		default:
			break;
	}
}

void
clMenu::lvlRegime()
{
	static char title[] PROGMEM = "Настройка\\Режим";
	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable())
	{
		printDevicesRegime(poz, &sParam.def.status);
		poz += 20;
	}
	if (sParam.prm.status.isEnable())
	{
		printDevicesRegime(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable())
	{
		printDevicesRegime(poz, &sParam.prd.status);
		poz += 20;
	}



	switch(key_)
	{
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Настройка\\Параметры";
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

		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_R400)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 20, title);
	printPunkts();

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_ENTER:
			if (punkt_[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlSetupParamDef;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupParamPrm;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParamPrd;
				lvlCreate_ = true;
			}
			else if (punkt_[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate_ = true;
			}
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
	static char title[]  PROGMEM = "Параметры\\Защиты";
	static char punkt1[] PROGMEM = "Тип защиты";
	static char punkt2[] PROGMEM = "Тип линии";
	static char punkt3[] PROGMEM = "Доп. время без ман.";
	static char punkt4[] PROGMEM = "Перекрытие импульсов";
	static char punkt5[] PROGMEM = "Компенсация задержки";
	static char punkt6[] PROGMEM = "Порог предупр по РЗ";
	static char punkt7[] PROGMEM = "Загруб.чувств. по РЗ";
	static char punkt8[] PROGMEM = "Тип приемника";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
			punkt_[num++] = punkt5;
			punkt_[num++] = punkt6;
			punkt_[num++] = punkt7;
			punkt_[num++] = punkt8;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_RZSK)
		{
			sParam.txComBuf.addCom(GB_COM_DEF_GET_DEF_TYPE);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_LINE_TYPE);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_T_NO_MAN);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_OVERLAP);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_DELAY);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_RZ_THRESH);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_RZ_DEC);
			sParam.txComBuf.addCom(GB_COM_DEF_GET_PRM_TYPE);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);
	poz += 20;

	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);
	poz += 20;

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	snprintf_P(&vLCDbuf[poz], 11, fcRange);
	poz += 10;
	if (p == punkt1)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
		poz += 10;
	}
	else if (p == punkt2)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
		poz += 10;
	}
	else if (p == punkt3)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				DEF_T_NO_MAN_MIN, DEF_T_NO_MAN_MAX, "час");
		poz += 10;
	}
	else if (p == punkt4)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				DEF_OVERLAP_MIN, DEF_OVERLAP_MAX, "град");
		poz += 10;
	}
	else if (p == punkt5)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				DEF_DELAY_MIN, DEF_DELAY_MAX, "град");
		poz += 10;
	}
	else if (p == punkt6)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				DEF_RZ_THRESH_MIN, DEF_RZ_THRESH_MAX, "дБ");
		poz += 10;
	}
	else if (p == punkt7)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				DEF_RZ_DEC_MIN, DEF_RZ_DEC_MAX, "дБ");
		poz += 10;
	}
	else if (p == punkt8)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
		poz += 10;
	}

	// вывод надписи "Значение:" и переход к выводу самого значения
	snprintf_P(&vLCDbuf[poz], 11, fcValue);
	poz += 10;
	if (p == punkt1)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcDefType[sParam.def.getDefType()]);
	}
	else if (p == punkt2)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcTypeLine[sParam.def.getLineType()]);
	}
	else if (p == punkt3)
	{
		snprintf(&vLCDbuf[poz], 11, "%dчас", sParam.def.getTimeNoMan());
	}
	else if (p == punkt4)
	{
		snprintf(&vLCDbuf[poz], 11, "%dград", sParam.def.getOverlap());
	}
	else if (p == punkt5)
	{
		snprintf(&vLCDbuf[poz], 11, "%dград", sParam.def.getDelay());
	}
	else if (p == punkt6)
	{
		snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzThreshold());
	}
	else if (p == punkt7)
	{
		snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzDec());
	}
	else if (p == punkt8)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcPrmType[sParam.def.getPrmType()]);
	}

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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

	static char punkt1[] PROGMEM = "Время включения ком.";
	static char punkt2[] PROGMEM = "Блокиров. команды";
	static char punkt3[] PROGMEM = "Задержка на выкл.ком";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение списка параметров
		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_K400)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_K400)
		{
			sParam.txComBuf.addCom(GB_COM_PRM_GET_BLOCK_COM);
			sParam.txComBuf.addCom(GB_COM_PRM_GET_TIME_ON);
			sParam.txComBuf.addCom(GB_COM_PRM_GET_TIME_OFF);
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			sParam.txComBuf.addCom(GB_COM_PRM_GET_BLOCK_COM);
			sParam.txComBuf.addCom(GB_COM_PRM_GET_TIME_ON);
			sParam.txComBuf.addCom(GB_COM_PRM_GET_TIME_OFF);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);
	poz += 20;

	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);
	poz += 20;

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	snprintf_P(&vLCDbuf[poz], 11, fcRange);
	poz += 10;
	if (p == punkt1)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				PRM_TIME_ON_MIN, PRM_TIME_ON_MAX, "мс");
		poz += 10;
	}
	else if (p == punkt2)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_);
		poz += 20;
	}
	else if (p == punkt3)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				PRM_TIME_OFF_MIN, PRM_TIME_OFF_MAX, "мс");
		poz += 10;
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_);
		poz += 20;
	}

	// вывод надписи "Значение:" и переход к выводу самого значения
	snprintf_P(&vLCDbuf[poz], 11, fcValue);
	poz += 10;
	if (p == punkt1)
	{
		snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prm.getTimeOn());
	}
	else if (p == punkt2)
	{
		if (sParam.prm.getBlockCom(curCom_ - 1))
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt3)
	{
		snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prm.getTimeOff(curCom_ - 1));
	}

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			curCom_ = 1;
			break;
		case KEY_DOWN:
			cursorLineDown();
			curCom_ = 1;
			break;
		case KEY_LEFT:
			curCom_ = curCom_ <= 1 ? sParam.prm.getNumCom() : curCom_- 1;
			break;
		case KEY_RIGHT:
			curCom_ = curCom_ >= sParam.prm.getNumCom() ? 1 : curCom_ + 1;
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Параметры\\Передатчик";
	static char punkt1[] PROGMEM = "Время включения";
	static char punkt2[] PROGMEM = "Длительность команды";
	static char punkt3[] PROGMEM = "Тестовая команда";
	static char punkt4[] PROGMEM = "Длительные команды";
	static char punkt5[] PROGMEM = "Блокиров. команды";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение списка параметров
		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_K400)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
			punkt_[num++] = punkt5;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt4;
			punkt_[num++] = punkt5;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_K400)
		{
			sParam.txComBuf.addCom(GB_COM_PRD_GET_TIME_ON);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_DURATION);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_TEST_COM);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_LONG_COM);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_BLOCK_COM);
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			sParam.txComBuf.addCom(GB_COM_PRD_GET_TIME_ON);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_DURATION);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_LONG_COM);
			sParam.txComBuf.addCom(GB_COM_PRD_GET_BLOCK_COM);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);
	poz += 20;

	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);
	poz += 20;

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	snprintf_P(&vLCDbuf[poz], 11, fcRange);
	poz += 10;
	if (p == punkt1)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				PRD_TIME_ON_MIN, PRD_TIME_ON_MAX, "мс");
		poz += 10;
	}
	else if (p == punkt2)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				PRD_DURATION_MIN, PRD_DURATION_MAX, "мс");
		poz += 10;
	}
	else if (p == punkt3)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
	}
	else if (p == punkt4)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_);
		poz += 20;
	}
	else if (p == punkt5)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_);
		poz += 20;
	}

	// вывод надписи "Значение:" и переход к выводу самого значения
	snprintf_P(&vLCDbuf[poz], 11, fcValue);
	poz += 10;
	if (p == punkt1)
	{
		snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getTimeOn());
	}
	else if (p == punkt2)
	{
		snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getDuration());
	}
	else if (p == punkt3)
	{
		if (sParam.prd.getTestCom())
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt4)
	{
		if (sParam.prd.getLongCom(curCom_ - 1))
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt5)
	{
		if (sParam.prd.getBlockCom(curCom_ - 1))
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			curCom_ = 1;
			break;
		case KEY_DOWN:
			cursorLineDown();
			curCom_ = 1;
			break;
		case KEY_LEFT:
			curCom_ = curCom_ <= 1 ? sParam.prd.getNumCom() : curCom_- 1;
			break;
		case KEY_RIGHT:
			curCom_ = curCom_ >= sParam.prd.getNumCom() ? 1 : curCom_ + 1;
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Параметры\\Общие";
	static char punkt1[] PROGMEM = "Синхронизация часов";
	static char punkt2[] PROGMEM = "Номер аппарата";
	static char punkt3[] PROGMEM = "Контроль вых.сигнала";
	static char punkt4[] PROGMEM = "Порог предупр. по КЧ";
	static char punkt5[] PROGMEM = "Время перезапуска";
	static char punkt6[] PROGMEM = "Удерж. реле ком. ПРД";
	static char punkt7[] PROGMEM = "Удерж. реле ком. ПРМ";
	static char punkt8[] PROGMEM = "Уменьшение усиления";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение списка параметров
		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_K400)
		{
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
			punkt_[num++] = punkt5;
			punkt_[num++] = punkt6;
			punkt_[num++] = punkt7;
			punkt_[num++] = punkt8;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
			punkt_[num++] = punkt5;
			punkt_[num++] = punkt6;
			punkt_[num++] = punkt7;
			punkt_[num++] = punkt8;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_K400)
		{
			sParam.txComBuf.addCom(GB_COM_GET_TIME_SINCHR);
			sParam.txComBuf.addCom(GB_COM_GET_DEVICE_NUM);
			sParam.txComBuf.addCom(GB_COM_GET_OUT_CHECK);
			sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);
			sParam.txComBuf.addCom(GB_COM_GET_TIME_RERUN);
			sParam.txComBuf.addCom(GB_COM_GET_COM_PRD_KEEP);
			sParam.txComBuf.addCom(GB_COM_GET_COM_PRM_KEEP);
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			sParam.txComBuf.addCom(GB_COM_GET_OUT_CHECK);
			sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);
			sParam.txComBuf.addCom(GB_COM_GET_TIME_RERUN);
			sParam.txComBuf.addCom(GB_COM_GET_COM_PRD_KEEP);
			sParam.txComBuf.addCom(GB_COM_GET_COM_PRM_KEEP);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);
	poz += 20;

	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);
	poz += 20;

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	snprintf_P(&vLCDbuf[poz], 11, fcRange);
	poz += 10;
	if (p == punkt1)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
	}
	else if (p == punkt2)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				GLB_DEV_NUM_MIN, GLB_DEV_NUM_MAX, "");
		poz += 10;
	}
	else if (p == punkt3)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
	}
	else if (p == punkt4)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				GLB_CF_THRESH_MIN, GLB_CF_THRESH_MAX, "дБ");
		poz += 10;
	}
	else if (p == punkt5)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				GLB_T_RERUN_MIN, GLB_T_RERUN_MAX,  "c");
		poz += 10;
	}
	else if (p == punkt6)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
	}
	else if (p == punkt7)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
		poz += 10;
	}
	else if (p == punkt8)
	{
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec,
				GLB_IN_DEC_MIN, GLB_IN_DEC_MAX, "дБ");
		poz += 10;
	}

	// вывод надписи "Значение:" и переход к выводу самого значения
	snprintf_P(&vLCDbuf[poz], 11, fcValue);
	poz += 10;
	if (p == punkt1)
	{
		if (sParam.glb.getTimeSinchr())
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt2)
	{
		snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getDeviceNum());
	}
	else if (p == punkt3)
	{
		if (sParam.glb.getOutCheck())
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt4)
	{
		snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getCfThreshold());
	}
	else if (p == punkt5)
	{
		snprintf(&vLCDbuf[poz], 11, "%dс", sParam.glb.getTimeRerun());
	}
	else if (p == punkt6)
	{
		if (sParam.glb.getComPrdKeep())
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt7)
	{
		if (sParam.glb.getComPrmKeep())
			snprintf_P(&vLCDbuf[poz], 11, fcOn);
		else
			snprintf_P(&vLCDbuf[poz], 11, fcOff);
	}
	else if (p == punkt8)
	{
		snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getInDecrease());
	}

	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
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
	static char title[]  PROGMEM = "Настройка\\Время&дата";
	static char punkt1[] PROGMEM = "%d. Дата";
	static char punkt2[] PROGMEM = "%d. Время";


	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();


	switch(key_)
	{
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;

		default:
			break;
	}
}

/**	Вывод на экран текущих пунктов меню и курсора
 *	@param Нет
 *	@return Нет
 */
void
clMenu::printPunkts()
{
	// вывод на экран пунктов текущего меню
	// вывод заканчивается на последней строчке экрана,
	// либо последнем пункте

	// кол-во отображаемых строк на экране
	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	// если номер текущей строки больше, чем вмещается строк на экране
	// то выводить на экран начинаем с (текущий пункт - кол.во строк)
	// иначе с первой
	uint8_t cntPunkts = (cursorLine_ > numLines) ? cursorLine_-numLines: 0;

	for(uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++)
	{
		snprintf_P(&vLCDbuf[20 * line], 21, punkt_[cntPunkts], cntPunkts + 1);

		if (++cntPunkts >= numPunkts_)
			break;
	}

	// при необходиомости, вывод курсора на экран
	if (cursorEnable_)
	{
		if (cursorLine_ > numLines)
			vLCDbuf[20 * (NUM_TEXT_LINES - 1) + 2] = '*';
		else
		{
			vLCDbuf[20 * (cursorLine_ + lineParam_ - 1) + 2] = '*';
		}
	}
}

/**	Возвращает кол-во ошибок в коде, т.е. кол-во установленных битов.
 * 	@param Код ошибки
 * 	@return Кол-во ошибок
 */
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
