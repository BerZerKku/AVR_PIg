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

/// режим подсветки по умолчанию
#define LED_REGIME LED_SWITCH

/// буфер текста выводимого на ЖКИ
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// кол-во строк данных отображаемых на экране
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)

clMenu::clMenu() {
	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;
	lvlCreate_ = true;
	blink_ = false;
	blinkMeasParam_ = false;
	curCom_ = 0;
	delay_ = TIME_MESSAGE;

	// курсор неактивен
	cursorEnable_ = false;
	cursorLine_ = 0;

	// нажатой кнопки еще нет
	key_ = KEY_NO;

	// тип устройства еще не известен
	setDevice(AVANT_NO);

	// связи с БСП еще нет
	connectionBsp_ = false;

	// заполним массивы сост1ояний работы для всех устройств
	// массив должен быть заполнен полностью и последним всегда должно
	// быть fcUnknownSost
	sParam.def.status.stateText[0] = fcDefSost00;
	sParam.def.status.stateText[1] = fcDefSost01;
	sParam.def.status.stateText[2] = fcDefSost02;
	sParam.def.status.stateText[3] = fcDefSost03;
	sParam.def.status.stateText[4] = fcDefSost04;
	sParam.def.status.stateText[5] = fcDefSost05;
	sParam.def.status.stateText[6] = fcDefSost06;
	sParam.def.status.stateText[7] = fcDefSost07;
	sParam.def.status.stateText[8] = fcDefSost08;
	sParam.def.status.stateText[9] = fcDefSost09;
	sParam.def.status.stateText[10] = fcDefSost10;
	sParam.def.status.stateText[11] = fcDefSost11;
	sParam.def.status.stateText[12] = fcDefSost12;
	sParam.def.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	sParam.prm.status.stateText[0] = fcPrmSost00;
	sParam.prm.status.stateText[1] = fcPrmSost01;
	sParam.prm.status.stateText[2] = fcPrmSost02;
	sParam.prm.status.stateText[3] = fcPrmSost03;
	sParam.prm.status.stateText[4] = fcPrmSost04;
	sParam.prm.status.stateText[5] = fcPrmSost05;
	sParam.prm.status.stateText[6] = fcPrmSost06;
	sParam.prm.status.stateText[7] = fcPrmSost07;
	sParam.prm.status.stateText[8] = fcPrmSost08;
	sParam.prm.status.stateText[9] = fcPrmSost09;
	sParam.prm.status.stateText[10] = fcPrmSost10;
	sParam.prm.status.stateText[11] = fcPrmSost11;
	sParam.prm.status.stateText[12] = fcPrmSost12;
	sParam.prm.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	sParam.prd.status.stateText[0] = fcPrdSost00;
	sParam.prd.status.stateText[1] = fcPrdSost01;
	sParam.prd.status.stateText[2] = fcPrdSost02;
	sParam.prd.status.stateText[3] = fcPrdSost03;
	sParam.prd.status.stateText[4] = fcPrdSost04;
	sParam.prd.status.stateText[5] = fcPrdSost05;
	sParam.prd.status.stateText[6] = fcPrdSost06;
	sParam.prd.status.stateText[7] = fcPrdSost07;
	sParam.prd.status.stateText[8] = fcPrdSost08;
	sParam.prd.status.stateText[9] = fcPrdSost09;
	sParam.prd.status.stateText[10] = fcPrdSost10;
	sParam.prd.status.stateText[11] = fcPrdSost11;
	sParam.prd.status.stateText[12] = fcPrdSost12;
	sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	// назначим имена устройствам
	sParam.def.status.name = fcDeviceName00;
	sParam.prm.status.name = fcDeviceName01;
	sParam.prd.status.name = fcDeviceName02;

#ifdef DEBUG
	// в режиме отладки включена постоянная подсветка
	vLCDsetLed(LED_ON);
#else
	// установка режима работы подсветки
	vLCDsetLed(LED_REGIME);
#endif
}

void clMenu::main(void) {

	static const char fcNoConnectBsp[] PROGMEM = " Нет связи с БСП!!! ";

	// Счетчик времени до переинициализации ЖКИ
	static uint8_t cntInitLcd = 0;
	static uint8_t cntBlinkMeas = 0;
	static uint8_t cntBlinkText = 0;
	// предыдущее состояние флага наличия связи с БСП
	static bool lastConnection = false;

	if (++cntBlinkMeas >= TIME_MEAS_PARAM) {
		blinkMeasParam_ = !blinkMeasParam_;
		cntBlinkMeas = 0;
	}

	if (++cntBlinkText >= TIME_TEXT) {
		blink_ = !blink_;
		cntBlinkText = 0;
	}

	if (++cntInitLcd >= TIME_TO_INIT_LCD) {
		vLCDinit();
		cntInitLcd = 0;
	}

	// настройка аппарата, если произошла смена:
	// кол-ва аппаратов в линии
	// типа совместимости (Р400м)
	if (!sParam.device) {
		setDevice(sParam.typeDevice);
	}

	// Считаем код с клавиатуры
	// Если нажата любая кнопка - включится кратковременная подсветка
	eKEY tmp = eKEYget(sParam.typeDevice);
	if (tmp != KEY_NO) {
		if (tmp == KEY_EMPTY)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLed(LED_SWITCH);
	}

	// подсветка включена всегда, если режим не "Введен"
	if (sParam.glb.status.getRegime() != GB_REGIME_ENABLED) {
		vLCDsetLed(LED_SWITCH);
	}

	// счетчик вывода сообщения
	if (delay_ < TIME_MESSAGE) {
		delay_++;
	}

	// вывод в буфер содержимого текущего меню
	// либо сообщения что тип аппарата не определен
	clearTextBuf();
	(this->*lvlMenu)();
	key_ = KEY_NO;

#ifdef VIEW_DEBUG_PARAM
	// вывод отладочной информации
	if (this->lvlMenu == &clMenu::lvlStart) {
		snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
		snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
		snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
		snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
		snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
		snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
		snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
	} else {
		snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
	}
#endif

	// вывод сообщения в случае отсутствия связи с БСП
	bool connection = isConnectionBsp();
	if (!connection) {
		if (blink_) {
			// если связи нет, периодически вместо измеряемых параметров
			// выводится предупреждающая надпись
			snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
		}
	} else if (!lastConnection) {
		// если связь с БСП только восстановилась
		// дважды пошлем команду опроса версии
		sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
		sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
	}
	lastConnection = connection;

	// преобразование строки символов в данные для вывода на экран
	vLCDputchar(vLCDbuf, lineParam_);
	// запуск обновления инф-ии на ЖКИ
	vLCDrefresh();
}

/**	Выполнение настроек для К400.
 * 	@retval True - всегда.
 */
bool clMenu::setDeviceK400() {

	sParam.typeDevice = AVANT_K400;

	// дата и время выводятся во всех вариантах
	measParam[0] = MENU_MEAS_PARAM_TIME; // дата <-> время
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
	measParam[1] = MENU_MEAS_PARAM_DATE;
	measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

	if (!sParam.prd.status.isEnable()) {
		// Если чистый приемник, не нужны U и I
		// а в трех концевой при этом надо выводить Uк1, Uk2, Uш1, Uш2
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			measParam[2] = MENU_MEAS_PARAM_UC1;
			measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC1;
			measParam[3] = MENU_MEAS_PARAM_UC2;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
			measParam[4] = MENU_MEAS_PARAM_UN1;
			measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN1;
			measParam[5] = MENU_MEAS_PARAM_UN2;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
		} else {
			measParam[3] = MENU_MEAS_PARAM_UC;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
			measParam[5] = MENU_MEAS_PARAM_UN;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;
		}
	} else if (!sParam.prm.status.isEnable()) {
		// Если чистый передатчик, не нужны Uk/Uш
		measParam[2] = MENU_MEAS_PARAM_UOUT;
		measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
		measParam[4] = MENU_MEAS_PARAM_IOUT;
		measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
	} else {
		// Если есть приемник и передатчик выводим U,I,Uk,Uш
		// в 3-х концевой при этом выводятся Uк1/2, Uш1/2
		measParam[2] = MENU_MEAS_PARAM_UOUT;
		measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
		measParam[4] = MENU_MEAS_PARAM_IOUT;
		measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			measParam[3] = MENU_MEAS_PARAM_UC1;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
			measParam[5] = MENU_MEAS_PARAM_UN1;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
		} else {
			measParam[3] = MENU_MEAS_PARAM_UC;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
			measParam[5] = MENU_MEAS_PARAM_UN;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;

		}
	}

	// заполнение массива общих неисправностей
	sParam.glb.status.faultText[0] = fcGlbFault0001;
	sParam.glb.status.faultText[1] = fcGlbFault0002;
	sParam.glb.status.faultText[2] = fcGlbFault0004;
	sParam.glb.status.faultText[3] = fcGlbFault0008;
	sParam.glb.status.faultText[4] = fcGlbFault0010;
	// 5-7 нет
	sParam.glb.status.faultText[8] = fcGlbFault0100;
	sParam.glb.status.faultText[9] = fcGlbFault0200;
	// 10 нет
	sParam.glb.status.faultText[11] = fcGlbFault0800;
	sParam.glb.status.faultText[12] = fcGlbFault1000;
	// 13-15 нет
	// заполнение массива общих предупреждений
	sParam.glb.status.warningText[0] = fcGlbWarning01;
	// 1-15 нет

	// отключение ЗАЩИТЫ
	sParam.def.status.setEnable(false);

	// ПРИЕМНИК
	// заполнение массива неисправностей приемника
	sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
	sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
	sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
	// 3-7 нет
	sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
	sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
	sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
	sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений приемника
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
	sParam.prm.status.warningText[1] = fcPrmWarning02k400;
	sParam.prm.status.warningText[2] = fcPrmWarning04k400;
	// 3-15 нет

	// ПЕРЕДАТЧИК
	// заполнение массива неисправностей передатчика
	sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
	sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
	// 2-7 нет
	sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
	sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
	sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
	sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений передатчика
	// 0 нет
	sParam.prd.status.warningText[1] = fcPrdWarning02k400;
	sParam.prd.status.warningText[2] = fcPrdWarning04k400;
	// 3-15 нет

	return true;
}

/**	Выполнение настроек для РЗСК.
 * 	@retval True - всегда.
 */
bool clMenu::setDeviceRZSK() {

	sParam.typeDevice = AVANT_RZSK;

	// первый столбец параметров
	measParam[0] = MENU_MEAS_PARAM_TIME;	// дата <-> время
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
	measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
	measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;

	// второй столбец параметров
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		measParam[1] = MENU_MEAS_PARAM_UZ1;
		measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ2;
		measParam[3] = MENU_MEAS_PARAM_UC1;
		measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
		measParam[5] = MENU_MEAS_PARAM_UN1;
		measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
	} else {
		measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
		measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
		measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;
	}

	// заполнение массива общих неисправностей
	sParam.glb.status.faultText[0] = fcGlbFault0001;
	sParam.glb.status.faultText[1] = fcGlbFault0002;
	sParam.glb.status.faultText[2] = fcGlbFault0004;
	sParam.glb.status.faultText[3] = fcGlbFault0008;
	sParam.glb.status.faultText[4] = fcGlbFault0010;
	// 5-7 нет
	sParam.glb.status.faultText[8] = fcGlbFault0100;
	sParam.glb.status.faultText[9] = fcGlbFault0200;
	// 10 нет
	sParam.glb.status.faultText[11] = fcGlbFault0800;
	sParam.glb.status.faultText[12] = fcGlbFault1000;
	// 13-15 нет
	// заполнение массива общих предупреждений
	// 1-9 нет
	sParam.glb.status.warningText[10] = fcGlbWarning01;
	// 11-15 нет

	// ЗАЩИТА
	// заполнение массива неисправностей защиты
	sParam.def.status.faultText[0] = fcDefFault0001;
	sParam.def.status.faultText[1] = fcDefFault0002;
	sParam.def.status.faultText[2] = fcDefFault0004;
	// 3-7 нет
	sParam.def.status.faultText[8] = fcDefFault0100;
	sParam.def.status.faultText[9] = fcDefFault0200;
	// 10 нет
	sParam.def.status.faultText[11] = fcDefFault0800;
	// 12 нет
	sParam.def.status.faultText[13] = fcDefFault2000;
	sParam.def.status.faultText[14] = fcDefFault4000rzsk;
	// 15 нет
	// заполнение массива предупреждений защиты
	sParam.def.status.warningText[0] = fcDefWarning01rzsk;
	sParam.def.status.warningText[1] = fcDefWarning02;
	// 2-15 нет

	// ПРИЕМНИК
	// заполнение массива неисправностей защиты
	sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
	sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
	sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
	// 3-7 нет
	sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
	sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
	sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
	sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений защиты
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
	sParam.prm.status.warningText[1] = fcPrmWarning02k400;
	sParam.prm.status.warningText[2] = fcPrmWarning04k400;
	// 3-15 нет

	// ПЕРЕДАТЧИК
	// заполнение массива неисправностей передатчика
	sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
	sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
	// 2-7 нет
	sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
	sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
	sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
	sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений передатчика
	// 0 нет
	sParam.prd.status.warningText[1] = fcPrdWarning02k400;
	sParam.prd.status.warningText[2] = fcPrdWarning04k400;
	// 3-15 нет

	return true;
}

/**	Выполнение настроек для Р400м.
 * 	@retval True - всегда.
 */
bool clMenu::setDeviceR400M() {

	sParam.typeDevice = AVANT_R400M;

	// первый столбец параметров
	measParam[0] = MENU_MEAS_PARAM_TIME;	// дата <-> время
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
	measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
	measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;

	// второй столбец параметров
	measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
	if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
		measParam[3] = MENU_MEAS_PARAM_UC1;
		measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
	} else {
		measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
	}
	measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_SD;

	// заполнение массива общих неисправностей
	sParam.glb.status.faultText[0] = fcGlbFault0001;
	sParam.glb.status.faultText[1] = fcGlbFault0002;
	sParam.glb.status.faultText[2] = fcGlbFault0004;
	sParam.glb.status.faultText[3] = fcGlbFault0008;
	sParam.glb.status.faultText[4] = fcGlbFault0010;
	sParam.glb.status.faultText[5] = fcGlbFault0020;
	sParam.glb.status.faultText[6] = fcGlbFault0040;
	sParam.glb.status.faultText[7] = fcGlbFault0080;
	sParam.glb.status.faultText[8] = fcGlbFault0100;
	sParam.glb.status.faultText[9] = fcGlbFault0200;
	sParam.glb.status.faultText[10] = fcGlbFault0400;
	sParam.glb.status.faultText[11] = fcGlbFault0800;
	sParam.glb.status.faultText[12] = fcGlbFault1000;
	sParam.glb.status.faultText[13] = fcGlbFault2000;
	sParam.glb.status.faultText[14] = fcGlbFault4000;
	// 15 нет
	// заполнение массива общих предупреждений
	sParam.glb.status.warningText[0] = fcGlbWarning01;
	// 1-15 - нет

	// ЗАЩИТА
	// заполнение массива неисправностей защиты
	sParam.def.status.faultText[0] = fcDefFault0001;
	sParam.def.status.faultText[1] = fcDefFault0002;
	sParam.def.status.faultText[2] = fcDefFault0004;
	// 3 нет
	sParam.def.status.faultText[4] = fcDefFault0010;
	// 5 нет
	sParam.def.status.faultText[6] = fcDefFault0040;
	sParam.def.status.faultText[7] = fcDefFault0080;
	sParam.def.status.faultText[8] = fcDefFault0100;
	sParam.def.status.faultText[9] = fcDefFault0200;
	sParam.def.status.faultText[10] = fcDefFault0400;
	sParam.def.status.faultText[11] = fcDefFault0800;
	sParam.def.status.faultText[12] = fcDefFault1000;
	sParam.def.status.faultText[13] = fcDefFault2000;
	sParam.def.status.faultText[14] = fcDefFault4000;
	sParam.def.status.faultText[15] = fcDefFault8000;
	// заполнение массива предупреждений защиты
	sParam.def.status.warningText[0] = fcDefWarning01;
	sParam.def.status.warningText[1] = fcDefWarning02;
	sParam.def.status.warningText[2] = fcDefWarning04;
	sParam.def.status.warningText[3] = fcDefWarning08;
	// 4-15 нет

	// отключение приемника
	sParam.prm.status.setEnable(false);
	// отключение передатчика
	sParam.prd.status.setEnable(false);

	return true;
}

/**	Выполнение настроек для ОПТИКИ.
 * 	@retval True - всегда.
 */
bool clMenu::setDeviceOPTO() {

	sParam.typeDevice = AVANT_OPTO;

	measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
	measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

	// заполнение массива общих неисправностей
	sParam.glb.status.faultText[0] = fcGlbFault0001;
	sParam.glb.status.faultText[1] = fcGlbFault0002;
	sParam.glb.status.faultText[2] = fcGlbFault0004;
	sParam.glb.status.faultText[3] = fcGlbFault0008;
	sParam.glb.status.faultText[4] = fcGlbFault0010;
	// 5-8 нет
	sParam.glb.status.faultText[9] = fcGlbFault0200;
	// 10-15 нет
	// заполнение массива общих предупреждений
	sParam.glb.status.warningText[0] = fcGlbWarning01;
	sParam.glb.status.warningText[1] = fcGlbWarning02;
	// 2-3 нет
	sParam.glb.status.warningText[4] = fcGlbWarning10;
	sParam.glb.status.warningText[5] = fcGlbWarning20;
	sParam.glb.status.warningText[6] = fcGlbWarning40;
	// 7-15 нет

	// ЗАЩИТА
	// заполнение массива неисправностей защиты
	sParam.def.status.faultText[0] = fcDefFault0001;
	sParam.def.status.faultText[1] = fcDefFault0002;
	sParam.def.status.faultText[2] = fcDefFault0004;
	// 3
	sParam.def.status.faultText[4] = fcDefFault0010rzsko;
	// 5-7 нет
	sParam.def.status.faultText[8] = fcDefFault0100;
	sParam.def.status.faultText[9] = fcDefFault0200;
	// 10 нет
	sParam.def.status.faultText[11] = fcDefFault0800;
	// 12 нет
	sParam.def.status.faultText[13] = fcDefFault2000;
	// 14-15 нет
	// заполнение массива предупреждений защиты
	sParam.def.status.warningText[0] = fcDefWarning01rzsko;
	sParam.def.status.warningText[1] = fcDefWarning02;
	// 2-15 нет

	// ПРИЕМНИК
	// заполнение массива неисправностей приемника
	sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
	sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
	sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
	// 3-7 нет
	sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
	sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
	sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
	sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений защиты
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsko;
	// 1-15 нет

	// ПЕРЕДАТЧИК
	// заполнение массива неисправностей передатчика
	sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
	sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
	// 2-7 нет
	sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
	sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
	sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
	sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
	// 12-15 нет
	// заполнение массива предупреждений передатчика
	// 0-15 нет

	return true;
}

bool clMenu::setDevice(eGB_TYPE_DEVICE device) {
	bool status = false;

	sParam.glb.status.setEnable(true);

	// если необходимый тип аппарата небыл передан, сделаем вывод исходя
	// из текущих настроек.
	if (device == AVANT_NO) {

		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM) {
			// ВЧ вариант
			// если есть защита + команды (прм и/или прд) - РЗСК
			// если есть только команды (прм и/или прд) - К400
			// если есть защита и (версия прошивки & 0xF000) = 0xF000 - Р400М

			if (sParam.def.status.isEnable()) {
				if ((sParam.prm.status.isEnable())
						|| (sParam.prd.status.isEnable())) {
					device = AVANT_RZSK;
				} else {
					uint16_t vers = sParam.glb.getVersProgIC(GB_IC_BSP_MCU);
					if ((vers & 0xF000) == 0xF000) {
						device = AVANT_R400M;
					}
				}
			} else {
				if ((sParam.prd.status.isEnable())
						|| (sParam.prm.status.isEnable())) {
					device = AVANT_K400;
				}
			}
		} else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO) {
			device = AVANT_OPTO;
		}

		// если текущее устройство совпадает с новым, то ничего не делаем
		// иначе прыгаем на начальный уровень
		if (device == sParam.typeDevice) {
			status = true;
		} else {
			sParam.typeDevice = device;
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
		}
	}

	if (!status) {
		// предварительная "очистка" массивов неисправностей
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.glb.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.def.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.prm.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.prd.status.faultText[i] = fcUnknownFault;

		// предварительная "очистка" массивов предупреждений
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.glb.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.def.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.prm.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.prd.status.warningText[i] = fcUnknownWarning;

		// предварительная очистка массива отображаемых параметров
		for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
			measParam[i] = MENU_MEAS_PARAM_NO;

		if (device == AVANT_K400) {
			status = setDeviceK400();
		} else if (device == AVANT_RZSK) {
			status = setDeviceRZSK();
		} else if (device == AVANT_R400M) {
			status = setDeviceR400M();
		} else if (device == AVANT_OPTO) {
			status = setDeviceOPTO();
		}
	}

	if ((!status) || (device == AVANT_NO)) {
		// если полученные данные не подходят ни под один имеющийся тип
		// на экране отображается ошибка
		sParam.typeDevice = AVANT_NO;
		// в случае неизвестного типа устройства, отключим все
		for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
			measParam[i] = MENU_MEAS_PARAM_NO;
		measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
		measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

		sParam.def.status.setEnable(false);
		sParam.prm.status.setEnable(false);
		sParam.prd.status.setEnable(false);

		lvlMenu = &clMenu::lvlError;
	}

	// "сброс" флага необходимости проверки типа аппарата
	sParam.device = true;
	// обнавление текущего уровня меню
	lvlCreate_ = true;

	return status;
}

eGB_COM clMenu::getTxCommand() {
	static uint8_t cnt = 0;
	eGB_COM com = sParam.txComBuf.getFastCom();

	if (com == GB_COM_NO) {
		cnt++;
		if (cnt == 1)
			com = GB_COM_GET_SOST;
		else if (cnt == 2)
			com = sParam.txComBuf.getCom1();

		// если нет команды, посылаем команду из буфера 2
		if (com == GB_COM_NO)
			com = sParam.txComBuf.getCom2();

		// начинаем цикл сначала, если отправлено 4 посылки
		if (cnt >= 4)
			cnt = 0;
	}

	return com;
}

/** Очистка текстового буфера
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::clearTextBuf() {
	for (uint_fast8_t i = 0; i < sizeof(vLCDbuf); i++)
		vLCDbuf[i] = ' ';
}

/** Очистка строки
 * 	@param line Номер строки 1..NUM_TEXT_LINES
 * 	@retrun Нет
 */
void clMenu::clearLine(uint8_t line) {
	if ((line > 0) && (line <= NUM_TEXT_LINES)) {
		line = (line - 1) * 20;
		for (uint_fast8_t i = 0; i < 20; i++)
			vLCDbuf[line++] = ' ';
	}
}

/** Уровень "Ошибочный тип аппарата"
 * 	Изначально на экран выводится надпись "Инициализация". Но если тип аппарата
 * 	не будет определен n-ое кол-во времени, то надпись сменится на
 * 	"Тип аппарата не определен!!!"
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlError() {
	static uint8_t time = 0;

	static const char fcNoTypeDevice0[] PROGMEM = "    Тип аппарата    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   не определен!!!  ";
	static const char fcNoTypeDevice3[] PROGMEM = "    Инициализация   ";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		vLCDclear();
		// только одна строка отводится под вывод параметров
		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_VERS);
	}

	// вывод на экран измеряемых параметров
	printMeasParam(0, measParam[0]);
	printMeasParam(1, measParam[1]);

	// Проверка времени нахождения в неизвестном состоянии типа аппарата
	if (time >= 25) {
		snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
		snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);
	} else {
		snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice3);
	}
	time++;

	if (sParam.typeDevice != AVANT_NO) {
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		time = 0;
	}
}

/** Уровень начальный
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlStart() {

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorEnable_ = false;
		lineParam_ = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();
		// буфер 1
		// дополнительные команды
		// время измеряемые параметры (в ВЧ)
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

		// буфер 2
		// неисправности
		// в Р400м + АК + кол-во аппаратов в линии
		sParam.txComBuf.addCom1(GB_COM_GET_FAULT);
		if (sParam.typeDevice == AVANT_R400M)
			sParam.txComBuf.addCom1(GB_COM_DEF_GET_TYPE_AC);
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
	}

	// вывод на экран измеряемых параметров
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (blinkMeasParam_)
			printMeasParam(i, measParam[i]);
		else
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
	}

	uint16_t val = sParam.glb.status.getWarnings();

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;

		// в Р400м выводится АК и время до АК
		if (sParam.typeDevice == AVANT_R400M) {
			uint16_t time = sParam.def.getTimeToAC();
			eGB_TYPE_AC ac = sParam.def.getTypeAC();

			uint8_t t = poz + 20;
			t += snprintf_P(&vLCDbuf[t], 11,
					fcAcType[static_cast<uint8_t>(ac)]);

			// время до АК
			// выводится если соблюдаются условия:
			// 1. АК не выключен
			// 2. Режим = введен
			// 3. Состояние = Контроль
			if (ac != GB_TYPE_AC_OFF) {
				if (sParam.def.status.getRegime() == GB_REGIME_ENABLED) {
					if (sParam.def.status.getState() == 1) {
						uint8_t hour = time / 3600;
						uint8_t min = (time % 3600) / 60;
						uint8_t sec = time % 60;
						snprintf_P(&vLCDbuf[t + 1], 11, fcTimeToAc, hour, min,
								sec);
						poz += 20;
					}
				}
			}
		}
	}
	if (sParam.prm.status.isEnable()) {
		printDevicesStatus(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable()) {
		printDevicesStatus(poz, &sParam.prd.status);
//		poz += 20;
	}

	switch (key_) {
	case KEY_MENU:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_CALL:
		sParam.txComBuf.setInt8(GB_CONTROL_CALL);
		sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		break;

	case KEY_PUSK_UD:
		if (sParam.def.status.isEnable()) {
			if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			}
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_AC_PUSK_UD:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_PUSK_NALAD:
		if (sParam.def.status.isEnable()) {
			if (sParam.def.status.getState() == 7) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
		}
		break;

	case KEY_AC_RESET:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_AC_PUSK:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		break;

	case KEY_AC_REGIME:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_REG_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_RESET_IND:
		if (sParam.prd.status.isEnable() || sParam.prm.status.isEnable()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND);
		}
		break;

	case KEY_PUSK:
		if (sParam.prm.status.isEnable()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER);
		}
		break;

	case KEY_RESET:
		sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
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
void clMenu::lvlFirst() {
	static char title[] PROGMEM = "Меню";
	static char punkt1[] PROGMEM = "%d. Журнал";
	static char punkt2[] PROGMEM = "%d. Управление";
	static char punkt3[] PROGMEM = "%d. Настройка";
	static char punkt4[] PROGMEM = "%d. Тесты";
	static char punkt5[] PROGMEM = "%d. Информация";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		lineParam_ = 1;
		cursorEnable_ = true;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_ENTER:
		switch (cursorLine_) {
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
		case 4:
			lvlMenu = &clMenu::lvlTest;
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

	case KEY_MENU:
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
void clMenu::lvlInfo() {
	static char title[]  PROGMEM = "Меню\\Информация";
	static char versProg[] 	PROGMEM = "%S: %02X.%02X";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;
		cursorLine_ = 0;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(GB_IC_BSP_MCU);
		if (sParam.typeDevice != AVANT_OPTO) {
			Punkts_.add(GB_IC_BSP_DSP);				// только в оптике нет DSP
		}
		Punkts_.add(GB_IC_PI_MCU);
		if (sParam.prd.status.isEnable()) {
			Punkts_.add(GB_IC_BSK_PLIS_PRD1);
			if (sParam.prd.getNumCom() > 16) {
				Punkts_.add(GB_IC_BSK_PLIS_PRD2);
			}
		}
		if (sParam.prm.status.isEnable()) {
			Punkts_.add(GB_IC_BSK_PLIS_PRM1);
			if (sParam.prm.getNumCom() > 16) {
				Punkts_.add(GB_IC_BSK_PLIS_PRM2);
			}
		}
		if (sParam.def.status.isEnable()) {
			Punkts_.add(GB_IC_BSZ_PLIS);
		}

		// доплнительные команды
		// обновляется версия прошивок (на случай перепрошивки)
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_VERS);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	uint8_t cntPunkts = cursorLine_;
	for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++) {
		uint8_t  ic = Punkts_.getNumber(cntPunkts);
		uint16_t vers = sParam.glb.getVersProgIC((eGB_IC) ic);
		snprintf_P(	&vLCDbuf[20*line], 21, versProg,
				fcIC[ic],
				(uint8_t) (vers >> 8),
				(uint8_t) vers);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts()) {
				break;
		}
	}

	switch (key_) {
	case KEY_UP:
		if (cursorLine_ > 0) {
			cursorLine_--;
		}
		break;
	case KEY_DOWN:
		if ((cursorLine_ + numLines) < Punkts_.getMaxNumPunkts()) {
			cursorLine_++;
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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
void clMenu::lvlJournal() {
	static char title[] PROGMEM = "Меню\\Журнал";
	static char punkt1[] PROGMEM = "%d. События";
	static char punkt2[] PROGMEM = "%d. Защита";
	static char punkt3[] PROGMEM = "%d. Приемник";
	static char punkt4[] PROGMEM = "%d. Передатчик";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();

		// активация необходимых пунктов меню и соответствующих им команд
		Punkts_.clear();
		Punkts_.add(punkt1);

		if (sParam.def.status.isEnable()) {
			Punkts_.add(punkt2);
		}

		if (sParam.prm.status.isEnable()) {
			Punkts_.add(punkt3);
		}

		if (sParam.prd.status.isEnable()) {
			Punkts_.add(punkt4);
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlJournalEvent;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlJournalDef;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlJournalPrm;
			lvlCreate_ = true;
		} else if (name == punkt4) {
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
void clMenu::lvlJournalEvent() {
	static char title[] PROGMEM = "Журнал\\События";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_EVENT_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = GLB_JRN_EVENT_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_EVENT_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_EVENT_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// номер текущей записи в архиве и максимальное кол-во записей
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// вывод названия текущего пункта меню
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;

	// вывод номер текущей записи и их кол-ва
	if (device == AVANT_OPTO) {
		// в оптике дополнительно выводится кол-во событий в одной записи
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}
	poz += 20;

	if (num_entries == 0) {
		// вывод сообщения об отсутствии записей в журнале
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ифнорация о текущей записи еще не получена
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// вывод режима
		snprintf_P(&vLCDbuf[poz], 21, fcRegimeJrn);
		snprintf_P(&vLCDbuf[poz + 7], 13,
				fcRegime[sParam.jrnEntry.getRegime()]);
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;

		// вывод события
		// в оптике в одной записи может быть много событий, поэтому
		// считывается код события в записи
		uint8_t event = 0;
		if (device == AVANT_OPTO) {
			// проверка текущего номера событий с кол-вом событий в записи
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			event = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			event = sParam.jrnEntry.getEventType();
		}

		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_R400M) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
		} else if (device == AVANT_K400) {
			if (event <= MAX_JRN_EVENT_VALUE) {
				uint8_t device = (uint8_t) sParam.jrnEntry.getDeviceJrn();
				snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event],
						fcDevicesK400[device]);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
			}
		} else if (device == AVANT_RZSK) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventRZSK[event], event);
		} else if (device == AVANT_OPTO) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTO[event], event);
		}

	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// поместим в сообщение для БСП адрес необходимой записи
	// размещен в конце, чтобы не терять время до следующего обращения к
	// данному пункту меню
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** Уровень меню. Журнал защиты.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlJournalDef() {
	static char title[] PROGMEM = "Журнал\\Защита";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_DEF);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_R400M) {
			t = GLB_JRN_DEF_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_DEF_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_DEF_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_DEF_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	// номер текущей записи в архиве и максимальное кол-во записей
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// вывод названия текущего пункта меню
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// вывод номер текущей записи и их кол-ва
	snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	poz += 20;

	if (num_entries == 0) {
		// вывод сообщения об отсутствии записей в журнале
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ифнорация о текущей записи еще не получена
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// вывод состояния
		snprintf_P(&vLCDbuf[poz], 21, fcStateJrn);
		snprintf_P(&vLCDbuf[poz + 11], 10,
				sParam.def.status.stateText[sParam.jrnEntry.getEventType()]);
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());

		poz += 20;
//		uint8_t signals = sParam.jrnEntry.getSignalDef();
//		snprintf(&vLCDbuf[poz], 21, "%x",signals);
		snprintf_P(&vLCDbuf[poz], 21, fcSignalDefJrn,
				sParam.jrnEntry.getSignalPusk(),
				sParam.jrnEntry.getSignalStop(), sParam.jrnEntry.getSignalMan(),
				sParam.jrnEntry.getSignalPrd(), sParam.jrnEntry.getSignalPrm(),
				sParam.jrnEntry.getSignalOut());
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;
	default:
		break;
	}

	// поместим в сообщение для БСП адрес необходимой записи
	// размещен в конце, чтобы не терять время до следующего обращения к
	// данному пункту меню
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** Уровень меню. Журнал приемника.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlJournalPrm() {
	static char title[] PROGMEM = "Журнал\\Приемник";
	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRM_K400_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRM_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRM_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// номер текущей записи в архиве и максимальное кол-во записей
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// вывод названия текущего пункта меню
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// вывод номер текущей записи и их кол-ва
	if (device == AVANT_OPTO) {
		// в оптике дополнительно выводится кол-во событий в одной записи
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}
	poz += 20;

	if (num_entries == 0) {
		// вывод сообщения об отсутствии записей в журнале
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ифнорация о текущей записи еще не получена
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// вывод номера команды
		uint8_t com = 0;
		if (device == AVANT_OPTO) {
			// в оптике в каждой записи каждый бит отвечает за свою команду
			// если 1 - передается, 0 - нет.
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			com = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			com = sParam.jrnEntry.getNumCom();
		}
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;
		// вывод события
		if (device == AVANT_OPTO) {
			// в оптике если есть записи - то это наличие команды
			// иначе - команд на передачу нет
			if (com != 0) {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComYes);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
			}
		} else {
			uint8_t event = sParam.jrnEntry.getEventType();
			snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
		}
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// поместим в сообщение для БСП адрес необходимой записи
	// размещен в конце, чтобы не терять время до следующего обращения к
	// данному пункту меню
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** Уровень меню. Журнал передатчика.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlJournalPrd() {
	static char title[] PROGMEM = "Журнал\\Передатчик";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRD_K400_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRD_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRD_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// номер текущей записи в архиве и максимальное кол-во записей
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// вывод названия текущего пункта меню
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;

	// вывод номер текущей записи и их кол-ва
	if (device == AVANT_OPTO) {
		// в оптике дополнительно выводится кол-во событий в одной записи
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}

	poz += 20;
	if (num_entries == 0) {
		// вывод сообщения об отсутствии записей в журнале
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ифнорация о текущей записи еще не получена
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {

		// вывод номера команды
		uint8_t com = 0;
		if (device == AVANT_OPTO) {
			// в оптике в одной записи может быть много команд
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			com = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			com = sParam.jrnEntry.getNumCom();
		}
		uint8_t t = snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);

		// для команднеой ВЧ-аппаратуры, выведем источник формирования команды
		if ((device == AVANT_K400) || (device == AVANT_RZSK)) {
			uint8_t s = sParam.jrnEntry.getSourceCom();
			snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
		}

		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;
		// вывод события
		if (device == AVANT_OPTO) {
			// в оптике если есть записи - то это наличие команды
			// иначе - команд на передачу нет
			if (com != 0) {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComYes);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
			}
		} else {
			uint8_t event = sParam.jrnEntry.getEventType();
			snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
		}
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// поместим в сообщение для БСП адрес необходимой записи
	// размещен в конце, чтобы не терять время до следующего обращения к
	// данному пункту меню
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/**	Уровень меню. Управление.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlControl() {
	static char title[] PROGMEM = "Меню\\Управление";
	// %d - может быть двухзначным, учесть для макс. кол-ва символов !
	//							   	"01234567890123456789"
	static char punkt02[] PROGMEM = "%d. Пуск удаленного";
	static char punkt03[] PROGMEM = "%d. Сброс своего";
	static char punkt04[] PROGMEM = "%d. Сброс удаленного";
	static char punkt05[] PROGMEM = "%d. Вызов";
	static char punkt06[] PROGMEM = "%d. Пуск налад. вкл.";
	static char punkt07[] PROGMEM = "%d. Пуск налад. выкл";
	static char punkt08[] PROGMEM = "%d. АК пуск";
	static char punkt09[] PROGMEM = "%d. Пуск удален. МАН";
	static char punkt10[] PROGMEM = "%d. АК контр.провер.";
	static char punkt11[] PROGMEM = "%d. Сброс АК";
	static char punkt12[] PROGMEM = "%d. Пуск АК свой";
	static char punkt13[] PROGMEM = "%d. Пуск АК удаленн.";
	static char punkt14[] PROGMEM = "%d. Пуск ПРД";
//	static char punkt15[] PROGMEM = "%d. АК автоматическ.";
	static char punkt16[] PROGMEM = "%d. АК ускоренный";
	static char punkt17[] PROGMEM = "%d. АК выключен";
	static char punkt18[] PROGMEM = "%d. АК испытания";
	static char punkt19[] PROGMEM = "%d. АК нормальный";
	static char punkt20[] PROGMEM = "%d. АК беглый";
//	static char punkt21[] PROGMEM = "%d. АК односторонний";
	static char punkt22[] PROGMEM = "%d. Сброс удаленных";
	static char punkt23[] PROGMEM = "%d. Пуск удаленн. 1";
	static char punkt24[] PROGMEM = "%d. Пуск удаленн. 2";
	static char punkt25[] PROGMEM = "%d. Пуск удаленн. 3";
	static char punkt26[] PROGMEM = "%d. Пуск удаленных";
	static char punkt27[] PROGMEM = "%d. Пуск удал. МАН 1";
	static char punkt28[] PROGMEM = "%d. Пуск удал. МАН 2";
	static char punkt29[] PROGMEM = "%d. Пуск удал. МАН 3";
	static char punkt30[] PROGMEM = "%d. Пуск удал-ых МАН";
	static char punkt31[] PROGMEM = "%d. АК включен";

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		if (device == AVANT_R400M) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			eGB_COMPATIBILITY compatibility = sParam.glb.getCompatibility();
			// первым всегда идет пуск наладочный
			Punkts_.add(punkt07);	// далее выбирается в зависимости от текущего
			// сброс своего есть во всех аппаратах и совместимостях
			Punkts_.add(punkt03);
			if (compatibility == GB_COMPATIBILITY_AVANT) {
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt04);
					Punkts_.add(punkt02);
					Punkts_.add(punkt11);
					Punkts_.add(punkt12);
					Punkts_.add(punkt13);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt17);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt22);
					Punkts_.add(punkt23);
					Punkts_.add(punkt24);
					Punkts_.add(punkt26);
					Punkts_.add(punkt11);
					Punkts_.add(punkt12);
					Punkts_.add(punkt13);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt17);
				}
			} else if (compatibility == GB_COMPATIBILITY_PVZ90) {
				Punkts_.add(punkt04);
				Punkts_.add(punkt19);
				Punkts_.add(punkt16);
				Punkts_.add(punkt17);
				Punkts_.add(punkt18);
				Punkts_.add(punkt08);
			} else if (compatibility == GB_COMPATIBILITY_PVZUE) {
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt02);
					Punkts_.add(punkt09);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt23);
					Punkts_.add(punkt24);
					Punkts_.add(punkt26);
					Punkts_.add(punkt27);
					Punkts_.add(punkt28);
					Punkts_.add(punkt30);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				}
			} else if (compatibility == GB_COMPATIBILITY_AVZK80) {
				Punkts_.add(punkt19);
				Punkts_.add(punkt16);
				Punkts_.add(punkt17);
				Punkts_.add(punkt18);
				Punkts_.add(punkt08);
			} else if (compatibility == GB_COMPATIBILITY_PVZL) {
				Punkts_.add(punkt11);
				Punkts_.add(punkt12);
				Punkts_.add(punkt13);
				Punkts_.add(punkt14);
				Punkts_.add(punkt31);
				Punkts_.add(punkt17);
			}
			// Вызов есть во всех совместимостях
			Punkts_.add(punkt05);
		} else if (device == AVANT_RZSK) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			if (numDevices == GB_NUM_DEVICES_2) {
				Punkts_.add(punkt07);	// далее выбирается в зависимости от текущего
				Punkts_.add(punkt03);
				Punkts_.add(punkt04);
				Punkts_.add(punkt02);
				Punkts_.add(punkt05);
			} else if (numDevices == GB_NUM_DEVICES_3) {
				Punkts_.add(punkt07);	// далее выбирается в зависимости от текущего
				Punkts_.add(punkt03);
				Punkts_.add(punkt22);
				Punkts_.add(punkt23);	// далее выбирается в зависимости от номера
				Punkts_.add(punkt24);	// далее выбирается в зависимости от номера
				Punkts_.add(punkt26);
				Punkts_.add(punkt05);
			}
		} else if (device == AVANT_K400) {
			Punkts_.add(punkt03);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt03);
			Punkts_.add(punkt04);
		}

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_R400M) {
			// совместимость
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// кол-во аппаратов в линии
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// номер текущего аппарата
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		} else if (sParam.typeDevice == AVANT_RZSK) {
			// кол-во аппаратов в линии
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// номер текущего аппарата
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	if (sParam.def.status.isEnable()) {
		// выбор вкл./выкл. наладочного пуска
		if (sParam.def.status.getState() != 7)
			Punkts_.change(punkt06, GB_COM_NO, 0);
		else
			Punkts_.change(punkt07, GB_COM_NO, 0);

		// выбор пуск удаленного
		if (device == AVANT_RZSK) {
			if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
				uint8_t num = sParam.glb.getDeviceNum();
				if (num == 1) {
					Punkts_.change(punkt24, GB_COM_NO, 3);
					Punkts_.change(punkt25, GB_COM_NO, 4);
				} else if (num == 2) {
					Punkts_.change(punkt23, GB_COM_NO, 3);
					Punkts_.change(punkt25, GB_COM_NO, 4);
				} else if (num == 3) {
					Punkts_.change(punkt23, GB_COM_NO, 3);
					Punkts_.change(punkt24, GB_COM_NO, 4);
				}
			}
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();

	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER: {
		if (name == punkt02) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt03) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt04) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt05) {
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt06) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt07) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt09) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt10) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt11) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt12) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt13) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt14) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
//		else if (p == punkt15)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_FAST);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (name == punkt16) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_FAST);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt17) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt18) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt19) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt20) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		} else if (p == punkt21) {
//			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt22) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt23) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt24) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt25) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt26) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt27) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt28) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt29) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt30) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt31) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
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
void clMenu::lvlSetup() {
	static char title[] PROGMEM = "Меню\\Настройка";
	static char punkt1[] PROGMEM = "%d. Режим";
	static char punkt2[] PROGMEM = "%d. Время и дата";
	static char punkt3[] PROGMEM = "%d. Параметры";
	static char punkt4[] PROGMEM = "%d. Пароль";
	static char punkt5[] PROGMEM = "%d. Интерфейс";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();
	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();

		// выбор функции ввода : пароль или параметр
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PASSWORD_READY) {
			uint16_t val = EnterParam.getValue();

			if (sParam.password.check(val)) {
				EnterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
			} else
				EnterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_N_READY) {
			uint16_t val = EnterParam.getValue();

			sParam.password.set(val);
		}
	}

	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER: {
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlRegime;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlSetupDT;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
		} else if (name == punkt4) {
			enterFunc = &clMenu::enterPassword;
			EnterParam.setEnable(MENU_ENTER_PASSWORD);
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt5) {
			lvlMenu = &clMenu::lvlSetupInterface;
			lvlCreate_ = true;
		}
	}
		break;

	default:
		break;
	}
}

void clMenu::lvlRegime() {
	static char title[] PROGMEM = "Настройка\\Режим";
	eGB_REGIME reg = sParam.glb.status.getRegime();

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// доплнительные команды
		sParam.txComBuf.clear();
		// кол-во аппаратов в линии
		sParam.txComBuf.addCom1(GB_COM_GET_DEVICE_NUM);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesRegime(poz, &sParam.def.status);
		poz += 20;
	}
	if (sParam.prm.status.isEnable()) {
		printDevicesRegime(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable()) {
		printDevicesRegime(poz, &sParam.prd.status);
//		poz += 20;
	}

	// Ввод нового значения параметра.
	// Сначала выбирается требуемый режим работы.
	// При попытке перейти в режим "Выведен" из "Введен" или "Готов",
	// происходит запрос пароля. При ошибочном пароле выводится сообщение.
	if (EnterParam.isEnable()) {
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();
		eGB_REGIME_ENTER val = GB_REGIME_ENTER_MAX;

		// выбор функции ввода : пароль или параметр
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PARAM_READY) {
			// проверим пароль, если пытаемся перейти в режим "Выведен"
			// из режимов "Введен" и "Готов"
			val = (eGB_REGIME_ENTER) EnterParam.getValueEnter();

			if ((reg == GB_REGIME_ENABLED) || (reg == GB_REGIME_READY)) {
				if (val == GB_REGIME_ENTER_DISABLED) {
					enterFunc = &clMenu::enterPassword;
					EnterParam.setEnable(MENU_ENTER_PASSWORD);
					EnterParam.setDopValue(static_cast<uint16_t>(val));
					val = GB_REGIME_ENTER_MAX;
				}
			}
		}

		if (stat == MENU_ENTER_PASSWORD_READY) {
			// проверка введеного пароля
			if (sParam.password.check(EnterParam.getValue())) {
				val = static_cast<eGB_REGIME_ENTER>(EnterParam.getDopValue());
			} else {
				EnterParam.printMessage();
			}
		}

		if (val != GB_REGIME_ENTER_MAX) {
			eGB_COM com = GB_COM_NO;

			if (val == GB_REGIME_ENTER_DISABLED)
				com = GB_COM_SET_REG_DISABLED;
			else if (val == GB_REGIME_ENTER_ENABLED)
				com = GB_COM_SET_REG_ENABLED;
			sParam.txComBuf.addFastCom(com);
			EnterParam.setDisable();
		}
	}

	switch (key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER: {
			uint8_t min = GB_REGIME_ENTER_DISABLED;
			uint8_t max = GB_REGIME_ENTER_DISABLED;
			uint8_t val = GB_REGIME_ENTER_DISABLED;

			// "Введен" 	-> "Выведен"
			// "Выведен" 	-> "Введен"
			// "Готов" 		-> "Введен"	 / "Выведен"
			// остальные 	-> "Выведен" / "Введен"
			enterFunc = &clMenu::enterValue;
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			switch(reg) {
				case GB_REGIME_ENABLED:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_DISABLED;
					val = GB_REGIME_ENTER_DISABLED;
					break;
				case GB_REGIME_ENTER_DISABLED:
					min = GB_REGIME_ENTER_ENABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_ENABLED;
					break;
				case GB_REGIME_READY:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_ENABLED;
					break;
				default:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_DISABLED;
					break;
			}
			EnterParam.setValueRange(min, max);
			EnterParam.setValue(val);
			EnterParam.list = fcRegimeEnter[0];
			EnterParam.com = GB_COM_NO;
		}
		break;

		default: {
		}
		break;
	}
}

/** Уровень меню. Настройка параметров.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupParam() {
	static char title[] PROGMEM = "Настройка\\Параметры";
	static char punkt1[] PROGMEM = "%d. Защита";
	static char punkt2[] PROGMEM = "%d. Приемник";
	static char punkt3[] PROGMEM = "%d. Передатчик";
	static char punkt4[] PROGMEM = "%d. Общие";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// настройка меню, в зависимости от текущего устройства
		Punkts_.clear();
		if (sParam.def.status.isEnable()) {
			Punkts_.add(punkt1);
		}
		if (sParam.prm.status.isEnable()) {
			Punkts_.add(punkt2);
		}
		if (sParam.prd.status.isEnable()) {
			Punkts_.add(punkt3);
		}
		Punkts_.add(punkt4);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 20, title);
	printPunkts();

	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlSetupParamDef;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlSetupParamPrm;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlSetupParamPrd;
			lvlCreate_ = true;
		} else if (name == punkt4) {
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
void clMenu::lvlSetupParamDef() {
	//								 12345678901234567890
	static char title[]   PROGMEM = "Параметры\\Защита";
	static char punkt1[]  PROGMEM = "Тип защиты";
	static char punkt2[]  PROGMEM = "Тип линии";
	static char punkt3[]  PROGMEM = "Доп. время без ман.";
	static char punkt4[]  PROGMEM = "Перекрытие импульсов";
	static char punkt5[]  PROGMEM = "Компенсация задержки";
	static char punkt6[]  PROGMEM = "Порог предупр. по РЗ";
	static char punkt7[]  PROGMEM = "Загрубление чувствит";
	static char punkt8[]  PROGMEM = "Тип приемника";
	static char punkt9[]  PROGMEM = "Снижение уровня АК";
	static char punkt10[] PROGMEM = "Частота ПРД";
	static char punkt11[] PROGMEM = "Частота ПРМ";
	static char punkt12[] PROGMEM = "Загрубл. чувств. РЗ";	// ==punkt7 для РЗСК

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		sParam.txComBuf.clear();
		Punkts_.clear();

		// для переформирования меню добавляется команда опроса:
		// кол-ва аппаратов в линии
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);

		if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_DEF_GET_DEF_TYPE);
			Punkts_.add(punkt2, GB_COM_DEF_GET_LINE_TYPE);
			Punkts_.add(punkt3, GB_COM_DEF_GET_T_NO_MAN);
			Punkts_.add(punkt4, GB_COM_DEF_GET_OVERLAP);
			Punkts_.add(punkt5, GB_COM_DEF_GET_DELAY);
			Punkts_.add(punkt6, GB_COM_DEF_GET_RZ_THRESH);
			Punkts_.add(punkt12, GB_COM_DEF_GET_RZ_DEC);
			Punkts_.add(punkt8, GB_COM_DEF_GET_PRM_TYPE);
		} else if (device == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();
			// для переформирования меню добавляется команда опроса:
			// совместимости
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			Punkts_.add(punkt1, GB_COM_DEF_GET_DEF_TYPE);
			Punkts_.add(punkt2, GB_COM_DEF_GET_LINE_TYPE);
			Punkts_.add(punkt3, GB_COM_DEF_GET_T_NO_MAN);
			Punkts_.add(punkt4, GB_COM_DEF_GET_OVERLAP);
			Punkts_.add(punkt5, GB_COM_DEF_GET_DELAY);
			Punkts_.add(punkt7, GB_COM_DEF_GET_RZ_DEC);
			if (comp == GB_COMPATIBILITY_AVANT) {
				// Снижение уровня АК есть только в совместимости АВАНТ
				Punkts_.add(punkt9, GB_COM_DEF_GET_PRM_TYPE);
			}
			Punkts_.add(punkt10, GB_COM_DEF_GET_FREQ_PRD);
			Punkts_.add(punkt11, GB_COM_DEF_GET_RZ_THRESH);
		}
	}

	// подмена команды, на команду текущего уровня меню.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	// номер текущего параметра и их количество
	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	// название параметра
	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// отображение доп.номера, для однотипных параметров
	poz = 60;
	if (device == AVANT_R400M) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt5) {
				if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT) {
					snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_, 2);
				}
			}
		}
	} else if (device == AVANT_RZSK) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if ((name == punkt5) || (name == punkt12)) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_, 2);
			}
		}
	}

	//  диапазон значений параметра
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_T_NO_MAN_MIN,
				DEF_T_NO_MAN_MAX, "час");
	} else if (name == punkt4) {
		uint8_t min = (sParam.typeDevice == AVANT_R400M) ?
				DEF_OVERLAP_MIN2 : DEF_OVERLAP_MIN;
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, min, DEF_OVERLAP_MAX, "град");
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_DELAY_MIN, DEF_DELAY_MAX,
				"град");
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_THRESH_MIN,
				DEF_RZ_THRESH_MAX, "дБ");
	} else if ((name == punkt7) || (name == punkt12)) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_DEC_MIN,
				DEF_RZ_DEC_MAX, "дБ");
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП
			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt2) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt4) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt5) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt7) || (name == punkt12)) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt9) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt10) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt11) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}

			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {

		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = sParam.def.getDefType();
			snprintf_P(&vLCDbuf[poz], 11, fcDefType[val]);
		} else if (name == punkt2) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		} else if (name == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%dчас", sParam.def.getTimeNoMan());
		} else if (name == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%dград", sParam.def.getOverlap());
		} else if (name == punkt5) {
			uint8_t val = sParam.def.getDelay(curCom_);
			snprintf(&vLCDbuf[poz], 11, "%dград", val);
		} else if (name == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzThreshold());
		} else if ((name == punkt7) || (name == punkt12)) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzDec(curCom_));
		} else if (name == punkt8) {
			snprintf_P(&vLCDbuf[poz], 11, fcPrmType[sParam.def.getPrmType()]);
		} else if (name == punkt9) {
			uint8_t val = sParam.def.getAcDec() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt10) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrd()]);
		} else if (name == punkt11) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrm()]);
		}
	}

	switch (key_) {
	case KEY_UP:
		curCom_ = 1;
		cursorLineUp();
		break;
	case KEY_DOWN:
		curCom_ = 1;
		cursorLineDown();
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.glb.getNumDevices() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.glb.getNumDevices() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.def.status.getRegime() != GB_REGIME_DISABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			EnterParam.printMessage();
		} else {

			if (name == punkt1) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(DEF_TYPE_MIN, DEF_TYPE_MAX - 1);
				EnterParam.setValue(sParam.def.getDefType());
				EnterParam.list = fcDefType[0];
				EnterParam.com = GB_COM_DEF_SET_DEF_TYPE;
			} else if (name == punkt2) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_NUM_DEVICES_MIN,
						GB_NUM_DEVICES_MAX - 1);
				EnterParam.setValue(sParam.def.getNumDevices());
				EnterParam.list = fcNumDevices[0];
				EnterParam.com = GB_COM_DEF_SET_LINE_TYPE;
			} else if (name == punkt3) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_T_NO_MAN_MIN, DEF_T_NO_MAN_MAX);
				EnterParam.setValue(sParam.def.getTimeNoMan());
				EnterParam.setDisc(DEF_T_NO_MAN_DISC);
				EnterParam.setFract(DEF_T_NO_MAN_FRACT);
				EnterParam.com = GB_COM_DEF_SET_T_NO_MAN;
			} else if (name == punkt4) {
				EnterParam.setEnable();
				if (sParam.typeDevice == AVANT_R400M) {
					EnterParam.setValueRange(DEF_OVERLAP_MIN2, DEF_OVERLAP_MAX);
				} else {
					EnterParam.setValueRange(DEF_OVERLAP_MIN, DEF_OVERLAP_MAX);
				}
				EnterParam.setValue(sParam.def.getOverlap());
				EnterParam.setDisc(DEF_OVERLAP_DISC);
				EnterParam.setFract(DEF_OVERLAP_FRACT);
				EnterParam.com = GB_COM_DEF_SET_OVERLAP;
			} else if (name == punkt5) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_DELAY_MIN, DEF_DELAY_MAX);
				EnterParam.setValue(sParam.def.getDelay(curCom_));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(DEF_DELAY_DISC);
				EnterParam.setFract(DEF_DELAY_FRACT);
				EnterParam.com = GB_COM_DEF_SET_DELAY;
			} else if (name == punkt6) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_RZ_THRESH_MIN, DEF_RZ_THRESH_MAX);
				EnterParam.setValue(sParam.def.getRzDec(curCom_));
				EnterParam.setDisc( DEF_RZ_THRESH_DISC);
				EnterParam.setFract(DEF_RZ_THRESH_FRACT);
				EnterParam.com = GB_COM_DEF_SET_RZ_THRESH;
			} else if ((name == punkt7) || (name == punkt12)) {
				// TODO Р400М в 3-х концевой 2 разных
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_RZ_DEC_MIN, DEF_RZ_DEC_MAX);
				EnterParam.setValue(sParam.def.getRzDec(curCom_));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(DEF_RZ_DEC_DISC);
				EnterParam.setFract(DEF_RZ_DEC_FRACT);
				EnterParam.com = GB_COM_DEF_SET_RZ_DEC;
			} else if (name == punkt8) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(DEF_PRM_TYPE_MIN, DEF_PRM_TYPE_MAX -1);
				EnterParam.setValue(sParam.def.getPrmType());
				EnterParam.list = fcPrmType[0];
				EnterParam.com = GB_COM_DEF_SET_PRM_TYPE;
			} else if (name == punkt9) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.def.getAcDec() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_DEF_SET_PRM_TYPE;
			} else if (name == punkt10) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_PVZL_FREQ_MIN,
						GB_PVZL_FREQ_MAX - 1);
				EnterParam.setValue(sParam.def.getFreqPrd());
				EnterParam.list = fcPvzlFreq[0];
				EnterParam.com = GB_COM_DEF_SET_FREQ_PRD;
			} else if (name == punkt11) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_PVZL_FREQ_MIN,
						GB_PVZL_FREQ_MAX - 1);
				EnterParam.setValue(sParam.def.getFreqPrm());
				EnterParam.list = fcPvzlFreq[0];
				EnterParam.com = GB_COM_DEF_SET_RZ_THRESH;
			}
		}
		break;

	default:
		break;
	}
}

/** Уровень меню. Настройка параметров приемника.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupParamPrm() {
	static char title[] PROGMEM = "Параметры\\Приемник";

	static char punkt1[] PROGMEM = "Задержка на фикс.ком";
	static char punkt2[] PROGMEM = "Блокиров. команды";
	static char punkt3[] PROGMEM = "Задержка на выкл.ком";
	static char punkt4[] PROGMEM = "Трансляция ЦС";
	static char punkt5[] PROGMEM = "Блокиров. команды ЦС";
	static char punkt6[] PROGMEM = "Команда ВЧ в ЦС";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (device == AVANT_K400) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
			Punkts_.add(punkt4, GB_COM_PRM_GET_DR_STATE);
			Punkts_.add(punkt5, GB_COM_PRM_GET_DR_BLOCK);
			Punkts_.add(punkt6, GB_COM_PRM_GET_DR_COM);
		} else if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
		}
	}

	// подмена команды, на команду текущего уровня меню.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	snprintf_P(&vLCDbuf[poz], 21, name);

	// при необходимости, вывод подномера данного параметра
	poz = 60;
	if ((name == punkt2) || (name == punkt3)
			|| (name == punkt5) || (name == punkt6)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prm.getNumCom());
	}

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_ON_MIN,
				PRM_TIME_ON_MAX, "мс");
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_OFF_MIN,
				PRM_TIME_OFF_MAX, "мс");
	} else if (name == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_COM_DR_MIN,
				PRM_COM_DR_MAX, "ком");
	}

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt2) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt4) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt5) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockComDR8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prm.getTimeOn());
		} else if (name == punkt2) {
			uint8_t val = (sParam.prm.getBlockCom(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%dмс",
					sParam.prm.getTimeOff(curCom_ - 1));
		} else if (name == punkt4) {
			uint8_t val = sParam.prm.getStateDR() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt5) {
			uint8_t val = (sParam.prm.getBlockComDR(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%dком",
					sParam.prm.getComDR(curCom_ - 1));
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		curCom_ = 1;
		break;
	case KEY_DOWN:
		cursorLineDown();
		curCom_ = 1;
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.prm.getNumCom() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.prm.getNumCom() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			EnterParam.printMessage();
		} else {
			if (name == punkt1) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_TIME_ON_MIN, PRM_TIME_ON_MAX);
				EnterParam.setValue(sParam.prm.getTimeOn());
				EnterParam.setDisc(PRM_TIME_ON_DISC);
				EnterParam.setFract(PRM_TIME_ON_FRACT);
				EnterParam.com = GB_COM_PRM_SET_TIME_ON;
			} else if (name == punkt2) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.setDopValue(curCom_);
				EnterParam.com = GB_COM_PRM_SET_BLOCK_COM;
			} else if (name == punkt3) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_TIME_OFF_MIN, PRM_TIME_OFF_MAX);
				EnterParam.setValue(sParam.prm.getTimeOff(curCom_ - 1));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(PRM_TIME_OFF_DISC);
				EnterParam.setFract(PRM_TIME_OFF_FRACT);
				EnterParam.com = GB_COM_PRM_SET_TIME_OFF;
			} else if (name == punkt4) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getStateDR() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRM_SET_DR_STATE;
			} else if (name == punkt5) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockComDR(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRM_SET_DR_BLOCK;
			} else if (name == punkt6) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_COM_DR_MIN, PRM_COM_DR_MAX);
				EnterParam.setValue(sParam.prm.getComDR(curCom_ - 1));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(PRM_COM_DR_DISC);
				EnterParam.setFract(PRM_COM_DR_FRACT);
				EnterParam.com = GB_COM_PRM_SET_DR_COM;
			}
		}
		break;

	default:
		break;
	}
}

/** Уровень меню. Настройка параметров передатчика.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupParamPrd() {
	static char title[]  PROGMEM = "Параметры\\Передатчик";
	static char punkt1[] PROGMEM = "Задержка срабат. ПРД";
	static char punkt2[] PROGMEM = "Длительность команды";	// ВЧ
	static char punkt2o[] PROGMEM= "Длительность команды";	// Оптика
	static char punkt3[] PROGMEM = "Тестовая команда";
	static char punkt4[] PROGMEM = "Следящие команды";
	static char punkt5[] PROGMEM = "Блокиров. команды";
	static char punkt6[] PROGMEM = "Трансляция ЦС";
	static char punkt7[] PROGMEM = "Блокиров. команды ЦС";
	static char punkt8[] PROGMEM = "Количество команд А";


	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (device == AVANT_K400) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt3, GB_COM_PRD_GET_TEST_COM);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
			Punkts_.add(punkt8, GB_COM_PRD_GET_COM_A);
			Punkts_.add(punkt6, GB_COM_PRD_GET_DR_STATE);
			Punkts_.add(punkt7, GB_COM_PRD_GET_DR_BLOCK);
		} else if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2o, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
		}
	}

	// подмена команды, на команду текущего параметра
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	snprintf_P(&vLCDbuf[poz], 21, name);

	// при необходимости выводим подномер данного параметра
	poz = 60;
	if ((name == punkt4) || (name == punkt5) || (name == punkt7)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prd.getNumCom());
	}

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_TIME_ON_MIN,
				PRD_TIME_ON_MAX, "мс");
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURAT_L_MIN,
				PRD_DURAT_L_MAX, "мс");
	} else if (name == punkt2o) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURAT_O_MIN,
				PRD_DURAT_O_MAX, "мс");
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_COM_A_MIN,
				PRD_COM_A_MAX, "");
	}

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt2) || (name == punkt2o)) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt4) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getLongCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt5) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt7) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockComDR8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getTimeOn());
		} else if (name == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getDurationL());
		} else if (name == punkt2o) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getDurationO());
		} else if (name == punkt3) {
			uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt4) {
			uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt5) {
			uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt6) {
			uint8_t val = sParam.prd.getStateDR() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt7) {
			uint8_t val = sParam.prd.getBlockComDR(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt8) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.prd.getNumComA());
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		curCom_ = 1;
		break;
	case KEY_DOWN:
		cursorLineDown();
		curCom_ = 1;
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.prd.getNumCom() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.prd.getNumCom() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			EnterParam.printMessage();
		} else {

			if (name == punkt1) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_TIME_ON_MIN, PRD_TIME_ON_MAX);
				EnterParam.setValue(sParam.prd.getTimeOn());
				EnterParam.setDisc(PRD_TIME_ON_DISC);
				EnterParam.setFract(PRD_TIME_ON_FRACT);
				EnterParam.com = GB_COM_PRD_SET_TIME_ON;
			} else if (name == punkt2) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_DURAT_L_MIN, PRD_DURAT_L_MAX);
				EnterParam.setValue(sParam.prd.getDurationL());
				EnterParam.setDisc(PRD_DURAT_L_DISC);
				EnterParam.setFract(PRD_DURAT_L_FRACT);
				EnterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (name == punkt2o) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_DURAT_O_MIN, PRD_DURAT_O_MAX);
				EnterParam.setValue(sParam.prd.getDurationO());
				EnterParam.setDisc(PRD_DURAT_O_DISC);
				EnterParam.setFract(PRD_DURAT_O_FRACT);
				EnterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (name == punkt3) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_TEST_COM;
			} else if (name == punkt4) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_LONG_COM;
			} else if (name == punkt5) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_BLOCK_COM;
			} else if (name == punkt6) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getStateDR() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_DR_STATE;
			} else if (name == punkt7) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockComDR(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_DR_BLOCK;
			} else if (name == punkt8) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_COM_A_MIN, PRD_COM_A_MAX);
				EnterParam.setValue(sParam.prd.getNumComA());
				EnterParam.setDisc(PRD_COM_A_DISC);
				EnterParam.setFract(PRD_COM_A_FRACT);
				EnterParam.com = GB_COM_PRD_SET_COM_A;
			}
		}
		break;

	default:
		break;
	}
}

/** Уровень меню. Настройка параметров общих.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupParamGlb() {
	static char title[] PROGMEM = "Параметры\\Общие";
	//								   12345678901234567890
	static char punkt1[] PROGMEM 	= "Синхронизация часов";
	static char punkt2[] PROGMEM 	= "Номер аппарата";
	static char punkt3[] PROGMEM 	= "Контроль вых.сигнала";
	static char punkt4[] PROGMEM 	= "Порог предупреждения";
	static char punkt4rzsk[] PROGMEM= "Порог предупр. по КЧ";
	static char punkt5[] PROGMEM 	= "Время перезапуска";
	static char punkt6[] PROGMEM 	= "Удерж. реле ком. ПРД";
	static char punkt7[] PROGMEM 	= "Удерж. реле ком. ПРМ";
	static char punkt8[] PROGMEM 	= "Загрубл. чувств. ПРМ";
	static char punkt9[] PROGMEM 	= "Сетевой адрес";
	static char punkt10[] PROGMEM 	= "Uвых номинальное";
	static char punkt11[] PROGMEM 	= "Частота";
	static char punkt12[] PROGMEM 	= "Совместимость";			// защита
	static char punkt13[] PROGMEM 	= "Снижение ответа АК";
	static char punkt14[] PROGMEM 	= "Тип детектора";
	static char punkt15[] PROGMEM 	= "Коррекция напряжения";
	static char punkt16[] PROGMEM 	= "Коррекция тока";
	static char punkt17[] PROGMEM 	= "Протокол обмена";
	static char punkt18[] PROGMEM 	= "Признак четности";
	static char punkt19[] PROGMEM 	= "Допустимые провалы";
	static char punkt20[] PROGMEM 	= "Порог по помехе";
	static char punkt21[] PROGMEM 	= "Допустимая помеха";
	static char punkt22[] PROGMEM 	= "Тип автоконтроля";
	static char punkt23[] PROGMEM 	= "Резервирование";
	static char punkt24[] PROGMEM 	= "Совместимость";
	static char punkt25[] PROGMEM 	= "Тип линии";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (type == AVANT_K400) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt4, GB_COM_GET_CF_THRESHOLD);
				Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
				Punkts_.add(punkt8, GB_COM_GET_CF_THRESHOLD);
			}
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
//			Punkts_.add(punkt24, GB_COM_GET_COM_PRD_KEEP);	// TODO К400 команда "Совместимость"
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
				Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
			}
			Punkts_.add(punkt25, GB_COM_DEF_GET_LINE_TYPE);
		} else if (type == AVANT_RZSK) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			Punkts_.add(punkt4rzsk, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
			Punkts_.add(punkt8, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
			Punkts_.add(punkt14, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
			Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();

			// для переформирования меню добавляются команды опроса:
			// совместимости и кол-ва аппаратов в линии
			// измеряемые параметры для коррекции
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			if (comp == GB_COMPATIBILITY_AVANT) {
				Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			}
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			Punkts_.add(punkt4, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt10, GB_COM_GET_COM_PRM_KEEP);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
			Punkts_.add(punkt12, GB_COM_GET_COM_PRD_KEEP);
			if (comp == GB_COMPATIBILITY_PVZL) {
				Punkts_.add(punkt13, GB_COM_GET_TIME_RERUN);
			}
			Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
			Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
			if (comp == GB_COMPATIBILITY_PVZUE) {
				Punkts_.add(punkt17, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt18, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt19, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt20, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt21, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt22, GB_COM_GET_TIME_RERUN);
			}
		} else if (type == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
			}
			Punkts_.add(punkt23, GB_COM_GET_COR_U_I);
		}
	}

	// подмена команды, на команду текущего уровня меню.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt,
			cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// отображение доп.номера, для однотипных параметров
	poz = 60;
	if (sParam.typeDevice == AVANT_K400) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt8) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumPrm, curCom_, 2);
			}
		}
	} else if (sParam.typeDevice == AVANT_RZSK) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt8) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumPrm, curCom_, 2);
			}
		}
	}

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DEV_NUM_MIN,
				sParam.glb.getMaxNumDevices(), "");
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if ((name == punkt4) || (name == punkt4rzsk)) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CF_THRESH_MIN,
				GLB_CF_THRESH_MAX, "дБ");
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_T_RERUN_MIN,
				GLB_T_RERUN_MAX, "c");
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_IN_DEC_MIN,
				GLB_IN_DEC_MAX, "дБ");
	} else if (name == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_NET_ADR_MIN,
				GLB_NET_ADR_MAX, "");
	} else if (name == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_U_OUT_NOM_MIN,
				GLB_U_OUT_NOM_MAX, "В");
	} else if (name == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_FREQ_MIN, GLB_FREQ_MAX,
				""); // "кГц" не влезает
	} else if (name == punkt12) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt13) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_AC_IN_DEC_MIN,
				GLB_AC_IN_DEC_MAX, "дБ");
	} else if (name == punkt14) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DETECTOR_MIN,
				GLB_DETECTOR_MAX, "");
	} else if (name == punkt15) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_COR_U_DEC_MIN / 10,
				GLB_COR_U_DEC_MAX / 10, "В");
	} else if (name == punkt16) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, 0, GLB_COR_I_DEC_MAX, "мА");
	} else if (name == punkt17) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt18) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt19) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_FAIL_MIN,
				GLB_PVZUE_FAIL_MAX, "град");
	} else if (name == punkt20) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_TH_MIN,
				GLB_PVZUE_N_TH_MAX, "сек");
	} else if (name == punkt21) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_LVL_MIN,
				GLB_PVZUE_N_LVL_MAX, "град");
	} else if (name == punkt22) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt23) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt24) {
		// TODO K400 диапазон для параметра "Совместимость"
	} else if (name == punkt25) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП
			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt2) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt4) || (name == punkt4rzsk)) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt5) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt7) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt9) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt10) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt11) {
				uint16_t t = EnterParam.getValue();
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t >> 8), 0);
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t), 1);
			} else if (name == punkt12) {
				uint8_t t = EnterParam.getValueEnter();
				sParam.txComBuf.setInt8(t);
			} else if (name == punkt13) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt14) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt15) {
				// если текущее значение коррекции тока равно 0
				// то передается сообщение с под.байтом равным 4
				// означающим сброс коррекции
				int16_t t = static_cast<int16_t>(EnterParam.getValue());
				uint8_t dop = EnterParam.getDopValue();
				if (t == 0)
					dop = 4;
				else {
					// новая коррекция =
					// напряжение прибора - (напряжение с БСП - коррекция)
					t -= static_cast<int16_t>(sParam.measParam.getVoltageOut());
					t += sParam.glb.getCorU();
				}
				sParam.txComBuf.setInt8(dop, 0);
				sParam.txComBuf.setInt8(t / 10, 1);
				sParam.txComBuf.setInt8((t % 10) * 10, 2);
			} else if (name == punkt16) {
				// если текущее значение коррекции тока равно 0
				// то передается сообщение с под.байтом равным 5
				// означающим сброс коррекции
				int16_t t = static_cast<int16_t>(EnterParam.getValue());
				uint8_t dop = EnterParam.getDopValue();
				if (t == 0)
					dop = 5;
				else {
					// новая коррекция = ток прибора - (ток с БСП - коррекция)
					t -= static_cast<int16_t>(sParam.measParam.getCurrentOut());
					t += sParam.glb.getCorI();
				}
				sParam.txComBuf.setInt8(dop, 0);
				sParam.txComBuf.setInt8((t >> 8), 1);
				sParam.txComBuf.setInt8((t), 2);
			} else if (name == punkt17) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt18) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt19) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt20) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt21) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt22) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt23) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt24) {
				// TODO K400 команда на передачу для параметра "Совместимость"
			} else if (name == punkt25) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getDeviceNum());
		} else if (name == punkt3) {
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if ((name == punkt4) || (name == punkt4rzsk)) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getCfThreshold());
		} else if (name == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%dс", sParam.glb.getTimeRerun());
		} else if (name == punkt6) {
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt7) {
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt8) {
			uint8_t val = sParam.glb.getInDecrease(curCom_);
			snprintf(&vLCDbuf[poz], 11, "%dдБ", val);
		} else if (name == punkt9) {
			snprintf(&vLCDbuf[poz], 11, "%u", sParam.glb.getNetAddress());
		} else if (name == punkt10) {
			snprintf(&vLCDbuf[poz], 11, "%uВ", sParam.glb.getUoutNom());
		} else if (name == punkt11) {
			snprintf(&vLCDbuf[poz], 11, "%uкГц", sParam.glb.getFreq());
		} else if (name == punkt12) {
			uint8_t val = static_cast<uint8_t>(sParam.glb.getCompatibility());
			snprintf_P(&vLCDbuf[poz], 11, fcCompatibility[val]);
		} else if (name == punkt13) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getAcInDec());
		} else if (name == punkt14) {
			snprintf(&vLCDbuf[poz], 11, "%u", sParam.glb.getDetector());
		} else if (name == punkt15) {
			int16_t val = sParam.glb.getCorU();
			int8_t f = val % 10;
			if (f < 0)
				f = -f;
			// вывоится целая/дробная часть напряжения коррекции
			// в дробной части дополнительно отсекается знак числа
			snprintf(&vLCDbuf[poz], 11, "%d.%dВ", val / 10, f);
		} else if (name == punkt16) {
			snprintf(&vLCDbuf[poz], 11, "%dмА", sParam.glb.getCorI());
		} else if (name == punkt17) {
			uint8_t val = sParam.glb.getPvzueProtocol();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueProtocol[val]);
		} else if (name == punkt18) {
			uint8_t val = sParam.glb.getPvzueParity();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueParity[val]);
		} else if (name == punkt19) {
			snprintf(&vLCDbuf[poz], 11, "%uград", sParam.glb.getPvzueFail());
		} else if (name == punkt20) {
			snprintf(&vLCDbuf[poz], 11, "%uсек", sParam.glb.getPvzueNoiseTH());
		} else if (name == punkt21) {
			snprintf(&vLCDbuf[poz], 11, "%uград",
					sParam.glb.getPvzueNoiseLvl());
		} else if (name == punkt22) {
			uint8_t val = sParam.glb.getPvzueTypeAC();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueTypeAC[val]);
		} else if (name == punkt23) {
			uint8_t val = sParam.glb.getBackup() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt24) {
			// TODO K400 вывод параметра "Совместимость"
		} else if (name == punkt25) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.glb.getNumDevices() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.glb.getNumDevices() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// коррекция тока и напряжения вводятся в любом режиме
		// а также настройка интерфейса связи
		// остальные параметры только в "Выведен"
		if (name == punkt15) {
			// вводится реальное значение напряжения
			// по умолчанию стоит 0, что значит сброс настроек
			EnterParam.setEnable(MENU_ENTER_PARAM_U_COR);
			EnterParam.setValueRange(0, GLB_COR_U_DEC_MAX);
			EnterParam.setValue(0);
			EnterParam.setDisc(GLB_COR_U_DEC_DISC);
			EnterParam.setFract(GLB_COR_U_DEC_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (name == punkt16) {
			// вводится реальное значение тока
			// по умолчанию стоит 0, что значит сброс настроек
			EnterParam.setEnable();
			EnterParam.setValueRange(0, GLB_COR_I_DEC_MAX);
			EnterParam.setValue(0);
			EnterParam.setDisc(GLB_COR_I_DEC_DISC);
			EnterParam.setFract(GLB_AC_IN_DEC_FRACT);
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			EnterParam.printMessage();
		} else if (name == punkt1) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (name == punkt2) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_DEV_NUM_MIN,
					sParam.glb.getMaxNumDevices());
			EnterParam.setValue(sParam.glb.getDeviceNum());
			EnterParam.setDisc(GLB_DEV_NUM_DISC);
			EnterParam.setFract(GLB_DEV_NUM_FRACT);
			EnterParam.com = GB_COM_SET_DEVICE_NUM;
		} else if (name == punkt3) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_OUT_CHECK;
		} else if ((name == punkt4) || (name == punkt4rzsk)) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_CF_THRESH_MIN, GLB_CF_THRESH_MAX);
			EnterParam.setValue(sParam.glb.getCfThreshold());
			EnterParam.setDisc(GLB_CF_THRESH_DISC);
			EnterParam.setFract(GLB_CF_THRESH_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (name == punkt5) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_T_RERUN_MIN, GLB_T_RERUN_MAX);
			EnterParam.setValue(sParam.glb.getTimeRerun());
			EnterParam.setDisc(GLB_T_RERUN_DISC);
			EnterParam.setFract(GLB_T_RERUN_FRACT);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt6) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (name == punkt7) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (name == punkt8) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_IN_DEC_MIN, GLB_IN_DEC_MAX);
			EnterParam.setValue(sParam.glb.getInDecrease(curCom_));
			EnterParam.setDisc(GLB_IN_DEC_DISC);
			EnterParam.setFract(GLB_IN_DEC_FRACT);
			EnterParam.setDopValue(curCom_ + 1);
			EnterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (name == punkt9) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_NET_ADR_MIN, GLB_NET_ADR_MAX);
			EnterParam.setValue(sParam.glb.getNetAddress());
			EnterParam.setDisc(GLB_NET_ADR_DISC);
			EnterParam.setFract(GLB_NET_ADR_FRACT);
			EnterParam.com = GB_COM_SET_NET_ADR;
		} else if (name == punkt10) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_U_OUT_NOM_MIN, GLB_U_OUT_NOM_MAX);
			EnterParam.setValue(sParam.glb.getUoutNom());
			EnterParam.setDisc(GLB_U_OUT_NOM_DISC);
			EnterParam.setFract(GLB_U_OUT_NOM_FRACT);
			EnterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (name == punkt11) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_FREQ_MIN, GLB_FREQ_MAX);
			EnterParam.setValue(sParam.glb.getFreq());
			EnterParam.setDisc(GLB_FREQ_DISC);
			EnterParam.setFract(GLB_FREQ_FRACT);
			EnterParam.com = GB_COM_SET_FREQ;
		} else if (name == punkt12) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_COMPATIBILITY_MIN,
					GB_COMPATIBILITY_MAX - 1);
			uint8_t val = (uint8_t) sParam.glb.getCompatibility();
			EnterParam.setValue(val);
			EnterParam.list = fcCompatibility[0];
			EnterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (name == punkt13) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_AC_IN_DEC_MIN, GLB_AC_IN_DEC_MAX);
			EnterParam.setValue(sParam.glb.getAcInDec());
			EnterParam.setDisc(GLB_AC_IN_DEC_DISC);
			EnterParam.setFract(GLB_AC_IN_DEC_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt14) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_DETECTOR_MIN, GLB_DETECTOR_MAX);
			EnterParam.setValue(sParam.glb.getDetector());
			EnterParam.setDisc(GLB_DETECTOR_DISC);
			EnterParam.setFract(GLB_DETECTOR_FRACT);
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (name == punkt17) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_PROTOCOL_MIN,
					GB_PVZUE_PROTOCOL_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueProtocol());
			EnterParam.list = fcPvzueProtocol[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt18) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_PARITY_MIN,
					GB_PVZUE_PARITY_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueParity());
			EnterParam.list = fcPvzueParity[0];
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt19) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_FAIL_MIN, GLB_PVZUE_FAIL_MAX);
			EnterParam.setValue(sParam.glb.getPvzueFail());
			EnterParam.setDisc(GLB_PVZUE_FAIL_DISC);
			EnterParam.setFract(GLB_PVZUE_FAIL_FRACT);
			EnterParam.setDopValue(3);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt20) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_N_TH_MIN, GLB_PVZUE_N_TH_MAX);
			EnterParam.setValue(sParam.glb.getPvzueNoiseTH());
			EnterParam.setDisc(GLB_PVZUE_N_TH_DISC);
			EnterParam.setFract(GLB_PVZUE_N_TH_FRACT);
			EnterParam.setDopValue(4);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt21) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_N_LVL_MIN, GLB_PVZUE_N_LVL_MAX);
			EnterParam.setValue(sParam.glb.getPvzueNoiseLvl());
			EnterParam.setDisc(GLB_PVZUE_N_LVL_DISC);
			EnterParam.setFract(GLB_PVZUE_N_LVL_FRACT);
			EnterParam.setDopValue(5);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt22) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_TYPE_AC_MIN,
					GB_PVZUE_TYPE_AC_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueTypeAC());
			EnterParam.list = fcPvzueTypeAC[0];
			EnterParam.setDopValue(6);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt23) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getBackup() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (name == punkt24) {
			// TODO K400 ввод параметра с клавиатуры "Совместимость"
		} else if (name == punkt25) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_NUM_DEVICES_MIN,
					GB_NUM_DEVICES_MAX - 1);
			EnterParam.setValue(sParam.def.getNumDevices());
			EnterParam.list = fcNumDevices[0];
			EnterParam.com = GB_COM_DEF_SET_LINE_TYPE;
		}
		break;
	default:
		break;
	}
}

/** Уровень меню. Настройка дата/время.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupDT() {
	static char title[] PROGMEM = "Настройка\\Время&дата";
	static char punkt1[] PROGMEM = "%d. Год";
	static char punkt2[] PROGMEM = "%d. Месяц";
	static char punkt3[] PROGMEM = "%d. День";
	static char punkt4[] PROGMEM = "%d. Часы";
	static char punkt5[] PROGMEM = "%d. Минуты";
	static char punkt6[] PROGMEM = "%d. Секунды";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 2;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);
		Punkts_.add(punkt6);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[20], 21, title);

	printMeasParam(0, MENU_MEAS_PARAM_DATE);
	printMeasParam(1, MENU_MEAS_PARAM_TIME);

	if (EnterParam.isEnable()) {
		// вывод текущего пункта
		snprintf_P(&vLCDbuf[20 * lineParam_], 21, name, cursorLine_);
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// копирование введеного значение на свое место
//			sParam.txComBuf.setInt8(BIN_TO_BCD(EnterParam.getValueEnter()),
//					EnterParam.getDopValue());
			uint8_t t = EnterParam.getDopValue();
			uint8_t val = EnterParam.getValueEnter();
			if (t <= 2) {
				// ввод даты
				// подменим сохраненное время на текущее
				if (t == 0) {
					// ввод года, проверим дату, т.к. может быть високосный
					uint8_t month = BCD_TO_BIN(sParam.txComBuf.getInt8(1));
					uint8_t day = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
					if (day > sParam.dateTime.getNumDaysInMonth(month, val)) {
						sParam.txComBuf.setInt8(0x01, 2);
					}
				} else if (t == 1) {
					// ввод месяца, проверим кол-во установленных дней
					uint8_t day = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
					if (day > sParam.dateTime.getNumDaysInMonth(val)) {
						sParam.txComBuf.setInt8(0x01, 2);
					}
				}
				sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getHour()), 3);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMinute()), 4);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getSecond()), 5);
				sParam.txComBuf.addFastCom(EnterParam.com);
			} else if (t <= 5) {
				// ввод времени
				// подменим сохраненную дату на текущую
				sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getYear()), 0);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMonth()), 1);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getDay()), 2);
				sParam.txComBuf.addFastCom(EnterParam.com);
			}
			EnterParam.setDisable();
		}
	} else
		printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (name == punkt1) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 99);
			EnterParam.setValue(sParam.dateTime.getYear());
			EnterParam.setDopValue(0);
		} else if (name == punkt2) {
			EnterParam.setEnable();
			EnterParam.setValueRange(1, 12);
			EnterParam.setValue(sParam.dateTime.getMonth());
			EnterParam.setDopValue(1);
		} else if (name == punkt3) {
			EnterParam.setEnable();
			uint8_t max = sParam.dateTime.getNumDaysInMonth();
			EnterParam.setValueRange(1, max);
			EnterParam.setValue(sParam.dateTime.getDay());
			EnterParam.setDopValue(2);
		} else if (name == punkt4) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 23);
			EnterParam.setValue(sParam.dateTime.getHour());
			EnterParam.setDopValue(3);
		} else if (name == punkt5) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 59);
			EnterParam.setValue(sParam.dateTime.getMinute());
			EnterParam.setDopValue(4);
		} else if (name == punkt6) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 59);
			EnterParam.setValue(sParam.dateTime.getSecond());
			EnterParam.setDopValue(5);
		}
		EnterParam.com = GB_COM_SET_TIME;
		EnterParam.setDisc(1);
		EnterParam.setFract(1);
		// сохраним текущие значения даты и времени
		// байты расположены в порядке передачи в БСП
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getYear()), 0);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMonth()), 1);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getDay()), 2);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getHour()), 3);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMinute()), 4);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getSecond()), 5);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** Уровень меню. Интерфейс
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlSetupInterface() {
	static char title[] PROGMEM = "Настройка\\Интерфейс";
	static char punkt1[] PROGMEM = "Интерфейс связи";
	// Для RS-485 возможны следующие настройки:
	// протокола: стандарт, modbus...
	// скорость бит/с: 1200, 4800, 9600, 19200, 38400, 57600
	// биты данных: всегда 8
	// четность: нет,нечет, чет, маркер(1), пробел(0)
	// стоповые биты: 2, 1.5, 1
	// в RS-232 всегда: стандарт, 19200 бит/с, 8 бит, 2 стоп-бита, четность-нет
	static char punkt2[] PROGMEM = "Протокол";
	static char punkt3[] PROGMEM = "Скорость передачи";
	static char punkt4[] PROGMEM = "Биты данных";
	static char punkt5[] PROGMEM = "Четность";
	static char punkt6[] PROGMEM = "Стоповые биты";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		// если установлена связь по Локальной сети
		// появляются настройки портов
		// в RS-232 всегда: 19200 бит/с, 8 бит, 2 стоп-бита, четность-нет
		// (но во время отлдаки можно настраивать)
#ifndef DEBUG
		if (sParam.Uart.Interface.get() == GB_INTERFACE_RS485) {
			Punkts_.add(punkt2);
			Punkts_.add(punkt3);
			Punkts_.add(punkt4);
			Punkts_.add(punkt5);
			Punkts_.add(punkt6);
		}
#else
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);
		Punkts_.add(punkt6);
#endif

		// дополнительные команды
		sParam.txComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// вывод надписи "Диапазон:" и переход к выводу самого диапазона
	// ВСЕ СПИСКИ
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	snprintf_P(&vLCDbuf[poz], 11, fcRangeList);

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			if (name == punkt1) {
				uint8_t tmp = EnterParam.getValueEnter();
				TInterface::INTERFACE val;
				val = static_cast<TInterface::INTERFACE> (tmp);
				// если интерфейс сменился, обновим меню
				if (val != sParam.Uart.Interface.get()) {
					sParam.Uart.Interface.set(val);
					lvlCreate_ = true;
				}
			} else if (name == punkt2) {
				uint8_t tmp = EnterParam.getValueEnter();
				TProtocol::PROTOCOL val;
				val = static_cast<TProtocol::PROTOCOL> (tmp);
				sParam.Uart.Protocol.set(val);
			} else if (name == punkt3) {
				uint8_t tmp = EnterParam.getValueEnter();
				TBaudRate::BAUD_RATE val;
				val = static_cast<TBaudRate::BAUD_RATE> (tmp);
				sParam.Uart.BaudRate.set(val);
			} else if (name == punkt4) {
				uint8_t tmp = EnterParam.getValueEnter();
				TDataBits::DATA_BITS val;
				val = static_cast<TDataBits::DATA_BITS> (tmp);
				sParam.Uart.DataBits.set(val);
			} else if (name == punkt5) {
				uint8_t tmp = EnterParam.getValueEnter();
				TParity::PARITY val;
				val = static_cast<TParity::PARITY> (tmp);
				sParam.Uart.Parity.set(val);
			} else if (name == punkt6) {
				uint8_t tmp = EnterParam.getValueEnter();
				TStopBits::STOP_BITS val;
				val = static_cast<TStopBits::STOP_BITS> (tmp);
				sParam.Uart.StopBits.set(val);
			}
			EnterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Interface.get());
			snprintf_P(&vLCDbuf[poz], 11, fcInterface[val]);
		} else if (name == punkt2) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Protocol.get());
			snprintf_P(&vLCDbuf[poz], 11, fcProtocol[val]);
		} else if (name == punkt3) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.BaudRate.get());
			snprintf_P(&vLCDbuf[poz], 11, fcBaudRate[val]);
		} else if (name == punkt4) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.DataBits.get());
			snprintf_P(&vLCDbuf[poz], 11, fcDataBits[val]);
		} else if (name == punkt5) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Parity.get());
			snprintf_P(&vLCDbuf[poz], 11, fcParity[val]);
		} else if (name == punkt6) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.StopBits.get());
			snprintf_P(&vLCDbuf[poz], 11, fcStopBits[val]);
		}
	}

	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// а также настройка интерфейса связи
		// остальные параметры только в "Выведен"
		if (name == punkt1) {
			// интерфейс связи
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TInterface::MIN, TInterface::MAX - 1);
			EnterParam.setValue(sParam.Uart.Interface.get());
			EnterParam.list = fcInterface[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt2) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TProtocol::MIN, TProtocol::MAX - 1);
			EnterParam.setValue(sParam.Uart.Protocol.get());
			EnterParam.list = fcProtocol[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt3) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TBaudRate::MIN, TBaudRate::MAX - 1);
			EnterParam.setValue(sParam.Uart.BaudRate.get());
			EnterParam.list = fcBaudRate[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt4) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TDataBits::MIN, TDataBits::MAX - 1);
			EnterParam.setValue(sParam.Uart.DataBits.get());
			EnterParam.list = fcDataBits[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt5) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TParity::MIN, TParity::MAX - 1);
			EnterParam.setValue(sParam.Uart.Parity.get());
			EnterParam.list = fcParity[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt6) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TStopBits::MIN, TStopBits::MAX - 1);
			EnterParam.setValue(sParam.Uart.StopBits.get());
			EnterParam.list = fcStopBits[0];
			EnterParam.com = GB_COM_NO;
		}
		break;
	default:
		break;
	}
}

/** Уровень меню. Тест 1.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlTest() {
	static char title[] PROGMEM 		= "Настройка\\Тесты";
	static char punkt1[] PROGMEM 		= "%d. Тест передатчика";
	static char punkt2[] PROGMEM 		= "%d. Тест приемника";
	static char message[][21] PROGMEM 	= {
		   //12345678901234567890
			"    Перейдите в     ",		//
			"   режим ВЫВЕДЕН    " 		//
	};

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		// тест передатчика при наличии передатчика или защиты
		if ((sParam.prd.status.isEnable()) || (sParam.def.status.isEnable())) {
			Punkts_.add(punkt1);
		}
		// тест приемника при наличии приемника или защиты
		if ((sParam.prm.status.isEnable()) || (sParam.def.status.isEnable())) {
			Punkts_.add(punkt2);
		}

		// доплнительные команды
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_R400M) {
			// совместимость в Р400М
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 20, title);
	if (isMessage()) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		uint8_t poz = 40;
		for (uint_fast8_t i = 0; i < 2; i++, poz += 20)
			snprintf_P(&vLCDbuf[poz], 21, message[i]);
	} else
		printPunkts();

	eGB_REGIME reg = sParam.glb.status.getRegime();
	switch (key_) {
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_2)) {
				//
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_1);
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			} else if (reg == GB_REGIME_TEST_1) {
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			}
		} else if (name == punkt2) {
			if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_1)) {
				sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_2);
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			} else if (reg == GB_REGIME_TEST_2) {
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			}
		}
		// если уровень меню (т.е. стоит неверный режим) не изменился,
		// выведем сообщение
		if (lvlMenu == &clMenu::lvlTest)
			printMessage();
		break;

	default:
		break;
	}

}

/** Уровень меню. Тест 1.
 *  Если в течении 1 секунды небыл получен режим совпадающий с данным тестом
 * 	происходи выход в предыдущий пункт меню.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlTest1() {
	static char title[] PROGMEM = "Тесты\\Передатчик";
	static char punkt1[] PROGMEM = "Сигналы передатчика";
	static uint8_t cnt = 0;		// счетчик до выхода при ошибочном режиме
	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {


		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		// в оптике не надо выводить доп.параметры
		lineParam_ = (device == AVANT_OPTO) ? 1 : 2;

		vLCDclear();
		vLCDdrawBoard(lineParam_);
		cnt = 0;

		// дополнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_MEAS);	// измерения
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// сигналы

		// сигналы для тестов
		sParam.test.clear();
		if (device == AVANT_R400M) {
			sParam.test.addSignalToList(GB_SIGNAL_RZ);
			if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT)
				sParam.test.addSignalToList(GB_SIGNAL_CF);
		} else if (device == AVANT_RZSK) {
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF2_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);
		} else if (device == AVANT_K400) {
			sParam.test.addSignalToList(GB_SIGNAL_CF1);
			sParam.test.addSignalToList(GB_SIGNAL_CF2);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}
		} else if (device == AVANT_OPTO) {
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}
		}
	}

	// вывод на экран измеряемых параметров, если это не оптика
	if (device != AVANT_OPTO) {
		printMeasParam(2, MENU_MEAS_PARAM_UOUT);
		printMeasParam(3, MENU_MEAS_PARAM_IOUT);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	if (EnterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// текущий сигнал(ы)
			uint8_t sig = EnterParam.listValue[EnterParam.getValue()];
			uint8_t rz = 0;
			uint8_t cf = 0;

			sParam.test.getBytes(cf, rz, (eGB_TEST_SIGNAL) sig);
			// т.к. у нас для установки сигналов есть две разные группы
			// для каждой из которых требуется отправка своей команды
			// добавим в буфере команду для каждой из групп
			// !!! при передаче команды надо проверять данные в буфере
			// КЧ
			sParam.txComBuf.setInt8(1, 0);				// группа КЧ
			sParam.txComBuf.setInt8(cf, 1);				// текущий сигнал КЧ
			sParam.txComBuf.addFastCom(EnterParam.com);
			// РЗ
			sParam.txComBuf.setInt8(2, 2);				// группа РЗ
			sParam.txComBuf.setInt8(rz, 3);				// текущий сигнал РЗ
			sParam.txComBuf.addFastCom(EnterParam.com);

			EnterParam.setDisable();
		}
	} else {
		uint8_t poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// выход из теста при несоответствии режима
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_1) {
		if (++cnt >= TIME_TEST_EXIT) {
			key_ = KEY_CANCEL;
		}
	} else {
		cnt = 0;
	}

	switch (key_) {
		case KEY_CANCEL:
			sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
			lvlMenu = &clMenu::lvlTest;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			EnterParam.setValue(0);
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST_2);
			EnterParam.setValueRange(0, sParam.test.getNumSignals() - 1);
			EnterParam.listValue = sParam.test.signalList;
			EnterParam.list = fcTest1K400[0];
			EnterParam.com = GB_COM_SET_REG_TEST_1;
			break;

		default:
			break;
	}
}

/** Уровень меню. Тест 2.
 * 	Если в течении 1 секунды небыл получен режим совпадающий с данным тестом
 * 	происходи выход в предыдущий пункт меню.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlTest2() {
	static char title[] 	PROGMEM = "Тесты\\Приемник";
	static char punkt1[] 	PROGMEM = "Сигналы приемника";
	static char prm1[] 	PROGMEM = "ПРМ1: ";
	static char prm2[]	PROGMEM = "ПРМ2: ";

	static uint8_t cnt = 0;		// счетчик до выхода при ошибочном режиме
	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;


		if (device == AVANT_OPTO) {
			// В оптике измеряемые параметры выводить не надо
			lineParam_ = 1;
		} else if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			// В трех концевой линии 2 строки параметров
			lineParam_ = 3;
		} else {
			// Иначе одна строка параметров
			lineParam_ = 2;
		}
		vLCDclear();
		vLCDdrawBoard(lineParam_);
		cnt = 0;

		// дополнительные команды
		sParam.txComBuf.clear();
		if (device != AVANT_OPTO) {
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);	// измерения
		}
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// сигналы
	}

	// вывод на экран измеряемых параметров, если это не оптика
	if (device != AVANT_OPTO) {
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			if (sParam.def.status.isEnable()) {
				printMeasParam(2, MENU_MEAS_PARAM_UC1);
				printMeasParam(3, MENU_MEAS_PARAM_UZ1);
				printMeasParam(4, MENU_MEAS_PARAM_UC2);
				printMeasParam(5, MENU_MEAS_PARAM_UZ2);
			} else {
				printMeasParam(2, MENU_MEAS_PARAM_UC1);
				printMeasParam(3, MENU_MEAS_PARAM_UN1);
				printMeasParam(4, MENU_MEAS_PARAM_UC2);
				printMeasParam(5, MENU_MEAS_PARAM_UN2);
			}
		} else {
			printMeasParam(2, MENU_MEAS_PARAM_UC);
			if (sParam.def.status.isEnable()) {
				printMeasParam(3, MENU_MEAS_PARAM_UZ);
			} else {
				printMeasParam(3, MENU_MEAS_PARAM_UN);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	uint8_t poz = 100;
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		poz = 80;
		poz += snprintf_P(&vLCDbuf[poz], 11, prm1);
		snprintf_P(&vLCDbuf[poz], 11,
					fcTest1K400[sParam.test.getCurrentSignal()]);
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, prm2);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal2()]);
	} else {
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// выход из теста при несооответствии режима
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_2) {
		if (++cnt >= TIME_TEST_EXIT) {
			key_ = KEY_CANCEL;
		}
	} else {
		cnt = 0;
	}

	switch (key_) {
		case KEY_CANCEL:
			sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
			lvlMenu = &clMenu::lvlTest;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		default:
			break;
	}
}

/** Ввод параметра.
 * 	@param нет
 * 	@return True - по окончанию
 */
eMENU_ENTER_PARAM clMenu::enterValue() {
	static char enterList[] PROGMEM = "Ввод: %S";
	static char enterInt[] PROGMEM = "Ввод: %01u";
	static char enterUcor[] PROGMEM = "Ввод: %01u.%01u";

	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// вывод сообщения до тех пор, пока счетчик времени не обнулится
		// затем возврат в исходный пункт меню
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					" Изменить параметр  ",//
					"  можно только в    ",		//
					"  режиме ВЫВЕДЕН    " };

			EnterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else if (status == MENU_ENTER_PARAM_INT) {
		uint16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// если кол-во символов выходит за допустимые значения, закончим ввод
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterInt, val);
		}
	} else if (status == MENU_ENTER_PARAM_U_COR) {
		uint16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// если кол-во символов выходит за допустимые значения, закончим ввод
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterUcor, val / 10, val % 10);
		}
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = EnterParam.getValue();
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				EnterParam.list + STRING_LENGHT * val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = EnterParam.listValue[EnterParam.getValue()];
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				EnterParam.list + STRING_LENGHT * val);
	} else
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		EnterParam.setDisable();
		break;

	case KEY_UP:
		EnterParam.incValue(timePressKey());
		break;
	case KEY_DOWN:
		EnterParam.decValue(timePressKey());
		break;

	case KEY_ENTER:
		EnterParam.setEnterValueReady();
		break;
	default:
		break;
	}

	key_ = KEY_NO;
	return EnterParam.getStatus();
}

/** Ввод пароля
 * 	@param нет
 * 	@return True - по окончанию
 */
eMENU_ENTER_PARAM clMenu::enterPassword() {
	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// вывод сообщения до тех пор, пока счетчик времени не обнулится
		// затем возврат в исходный пункт меню
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					"       Введен       ",//
					"    неправильный    ",		//
					"       пароль       " };

			EnterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else {

		uint8_t poz = 100;
		clearLine(NUM_TEXT_LINES);

		uint16_t val = EnterParam.getValue();

		if (status == MENU_ENTER_PASSWORD) {
			static char enter[] PROGMEM = "Пароль: %04u";
			snprintf_P(&vLCDbuf[poz], 21, enter, val);
		} else if (status == MENU_ENTER_PASSWORD_NEW) {
			static char enterNew[] PROGMEM = "Новый пароль: %04u";
			snprintf_P(&vLCDbuf[poz], 21, enterNew, val);
		} else {
			key_ = KEY_CANCEL;
		}
	}

	switch (key_) {
	case KEY_CANCEL:
		EnterParam.setDisable();
		break;

	case KEY_ENTER:
		if (status == MENU_ENTER_PASSWORD)
			EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_READY);
		else if (status == MENU_ENTER_PASSWORD_NEW)
			EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_N_READY);
		else
			EnterParam.setDisable();
		break;

	case KEY_UP:
		EnterParam.setDisc(1000);
		EnterParam.incValue();
		break;
	case KEY_RIGHT:
		EnterParam.setDisc(100);
		EnterParam.incValue();
		break;
	case KEY_DOWN:
		EnterParam.setDisc(10);
		EnterParam.incValue();
		break;
	case KEY_LEFT:
		EnterParam.setDisc(1);
		EnterParam.incValue();
		break;

	default:
		break;
	}
	key_ = KEY_NO;

	return EnterParam.getStatus();
}

/**	Вывод на экран текущих пунктов меню и курсора
 *	@param Нет
 *	@return Нет
 */
void clMenu::printPunkts() {
	// вывод на экран пунктов текущего меню
	// вывод заканчивается на последней строчке экрана,
	// либо последнем пункте

	// кол-во отображаемых строк на экране
	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	// если номер текущей строки больше, чем вмещается строк на экране
	// то выводить на экран начинаем с (текущий пункт - кол.во строк)
	// иначе с первой
	uint8_t cntPunkts = (cursorLine_ > numLines) ? cursorLine_ - numLines : 0;

	for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++) {
		snprintf_P(&vLCDbuf[20 * line], 21, Punkts_.getName(cntPunkts),
				cntPunkts + 1);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts())
			break;
	}

	// при необходиомости, вывод курсора на экран
	if (cursorEnable_) {
		if (cursorLine_ > numLines)
			vLCDbuf[20 * (NUM_TEXT_LINES - 1) + 2] = '*';
		else {
			vLCDbuf[20 * (cursorLine_ + lineParam_ - 1) + 2] = '*';
		}
	}
}


/**	Вывод в указанном месте отображаемого параметра.
 * 	В одной строке выводятся два параметра.
 * 	@param poz Текущая позиция
 * 	@arg 0..eMENU_MEAS_PARAM, 0 первая строка слева, 5 - третья справа
 * 	@param par Отображаемый параметр
 * 	@return Нет
 */
void clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par) {
	// смещение в буфере
	if (poz < MAX_NUM_MEAS_PARAM) {
		// 10 - кол-во символов отведенное на экране под 1 параметр
		poz = (poz * 10) + 1;

		switch (par) {
		case MENU_MEAS_PARAM_DATE:
			snprintf_P(&vLCDbuf[poz], 11, fcDate, sParam.dateTime.getDay(),
					sParam.dateTime.getMonth(), sParam.dateTime.getYear());
			break;
		case MENU_MEAS_PARAM_TIME:
			snprintf_P(&vLCDbuf[poz], 11, fcTime, sParam.dateTime.getHour(),
					sParam.dateTime.getMinute(), sParam.dateTime.getSecond());
			break;
		case MENU_MEAS_PARAM_UZ:
			snprintf_P(&vLCDbuf[poz], 11, fcUz,
					sParam.measParam.getVoltageDef());
			break;

			// в 3-х концевой может быть Uz1 == Uz, Uz2
		case MENU_MEAS_PARAM_UZ1:
			snprintf_P(&vLCDbuf[poz], 11, fcUz1,
					sParam.measParam.getVoltageDef());
			break;
		case MENU_MEAS_PARAM_UZ2:
			snprintf_P(&vLCDbuf[poz], 11, fcUz2,
					sParam.measParam.getVoltageDef2());
			break;

		case MENU_MEAS_PARAM_UC:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf,
					sParam.measParam.getVoltageCf());
			break;

			// в 3-х концевой может быть Uk1 == Uk, Uk2
		case MENU_MEAS_PARAM_UC1:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf1,
					sParam.measParam.getVoltageCf());
			break;
		case MENU_MEAS_PARAM_UC2:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf2,
					sParam.measParam.getVoltageCf2());
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
			// в 3-х концевой может быть Uш1 == Uш, Uш2
		case MENU_MEAS_PARAM_UN1:
			snprintf_P(&vLCDbuf[poz], 11, fcUn1,
					sParam.measParam.getVoltageNoise());
			break;
		case MENU_MEAS_PARAM_UN2:
			snprintf_P(&vLCDbuf[poz], 11, fcUn2,
					sParam.measParam.getVoltageNoise2());
			break;

		case MENU_MEAS_PARAM_SD:
			snprintf_P(&vLCDbuf[poz], 11, fcSd,
					sParam.measParam.getPulseWidth());
			// ничего не делаем
			break;

		default:
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
void clMenu::printDevicesStatus(uint8_t poz, TDeviceStatus *device) {
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
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'l', y);
		}
	}
	else
	{
		text = device->stateText;
		poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
		snprintf_P(&vLCDbuf[poz], 9,
				text[device->getState()], device->getDopByte());
	}
}

/**	Вывод в пунтке меню "Режим" текущего режима устройств
 * 	@param poz Начальная позиция в буфере данных ЖКИ
 * 	@param device Данные для текущего устройства
 *	@return Нет
 */
void clMenu::printDevicesRegime(uint8_t poz, TDeviceStatus *device) {
	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;
	snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}


///**	Возвращает кол-во ошибок в коде, т.е. кол-во установленных битов.
// * 	@param Код ошибки
// * 	@return Кол-во ошибок
// */
//uint8_t clMenu::getNumError(uint16_t val) {
//	uint_fast8_t tmp = 0;
//
//	while ((val & 0x0001) == 0) {
//		tmp++;
//		val >>= 1;
//	}
//
//	return tmp;
//}
