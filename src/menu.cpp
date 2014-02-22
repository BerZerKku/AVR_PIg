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
	numPunkts_ = 0;

	// нажатой кнопки еще нет
	key_ = KEY_NO;

	// тип устройства еще не известен
	setTypeDevice(AVANT_NO);

	// связи с БСП еще нет
	connectionBsp_ = false;

	// заполним массивы состояний работы для всех устройств
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

/**	Работа с текущим уровнем меню
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::main(void) {
	// Счетчик времени до переинициализации ЖКИ
	static uint8_t cntInitLcd = 0;
	static uint8_t cntBlinkMeas = 0;
	static uint8_t cntBlinkText = 0;

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

	if (!sParam.device)
		setTypeDevice();

	// Считаем код с клавиатуры
	// Если нажата любая кнопка - включится кратковременная подсветка
	eKEY tmp = eKEYget(sParam.typeDevice);
	if (tmp != KEY_NO) {
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLed(LED_SWITCH);
	}

	// счетчик вывода сообщения
	if (delay_ < TIME_MESSAGE)
		delay_++;

	// вывод в буфер содержимого текущего меню
	// либо сообщения что тип аппарата не определен
	clearTextBuf();
	(this->*lvlMenu)();
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
	if (!isConnectionBsp() && (blink_)) {
		static const char fcNoConnectBsp[] PROGMEM = " Нет связи с БСП!!! ";
		snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
	}

	// преобразование строки символов в данные для вывода на экран
	vLCDputchar(vLCDbuf, lineParam_);
	// запуск обновления инф-ии на ЖКИ
	vLCDrefresh();
}

/** Установка типа аппарата и настройка меню с его учетом.
 * 	@param device Тип устройства
 * 	@return False в случае ошибки, иначе True
 */
bool clMenu::setTypeDevice(eGB_TYPE_DEVICE device) {
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
					uint16_t vers = sParam.glb.getVersBsp() & 0xF000;
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
		}

		// если текущее устройство совпадает с новым, то ничего не делаем
		// иначе прыгаем на начальный уровень
		if (device == sParam.typeDevice)
			status = true;
		else {
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
			sParam.typeDevice = device;

			// TODO ВСЕ в 3-х концевой Uк1/2, Uш1/2
			// первый столбец параметров
			measParam[0] = MENU_MEAS_PARAM_TIME; // дата <-> время
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;
			// второй столбец параметров
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

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

			// включение ПРИЕМНИКА
			sParam.prm.status.setEnable(true);
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
			// 1-15 нет

			// включение ПЕРЕДАТЧИКА
			// заполнение массива неисправностей передатчика
			sParam.prd.status.setEnable(true);
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

			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF1);
			sParam.test.addSignalToList(GB_SIGNAL_CF2);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}

			status = true;
		} else if (device == AVANT_RZSK) {
			sParam.typeDevice = device;

			// TODO ВСЕ в 3-х концевой Uк1/2, Uз1/2, Uш1/2
			// первый столбец параметров
			measParam[0] = MENU_MEAS_PARAM_TIME;	// дата <-> время
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;

			// второй столбец параметров
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UZ;
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

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

			// включение ЗАЩИТЫ
			sParam.def.status.setEnable(true);
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

			// включение ПРИЕМНИКА
			sParam.prm.status.setEnable(true);
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
			// 1-15 нет

			// включение ПЕРЕДАТЧИКА
			sParam.prd.status.setEnable(true);
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

			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1);
			sParam.test.addSignalToList(GB_SIGNAL_COM2);
			sParam.test.addSignalToList(GB_SIGNAL_COM3);
			sParam.test.addSignalToList(GB_SIGNAL_COM4);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);

			status = true;
		} else if (device == AVANT_R400M) {
			sParam.typeDevice = device;

			// TODO ВСЕ в 3-х концевой Uk1/2
			// первый столбец параметров
			measParam[0] = MENU_MEAS_PARAM_TIME;	// дата <-> время
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;

			// второй столбец параметров
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UZ;
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

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

			// включение ЗАЩИТЫ
			sParam.def.status.setEnable(true);
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

			status = true;
		} else if (device == AVANT_OPTIC) {
			sParam.typeDevice = device;

			// TODO ОПТИКА
			// дополнить список измеряемых параметров, если есть ?!
			measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_TIME;
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_DATE;

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
			// 1-15 нет

			// включение ЗАЩИТЫ
			sParam.def.status.setEnable(true);
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

			// включение ПРИЕМНИКА
			sParam.prm.status.setEnable(true);
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
			sParam.prm.status.warningText[0] = fcPrmWarning01rzsko;
			// 1-15 нет

			// включение ПЕРЕДАТЧИКА
			sParam.prd.status.setEnable(true);
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

			// TODO ОПТИКА надо заполнить сигналы ТЕСТов
			// с учетом текущего набора устройств
			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1);
			sParam.test.addSignalToList(GB_SIGNAL_COM2);
			sParam.test.addSignalToList(GB_SIGNAL_COM3);
			sParam.test.addSignalToList(GB_SIGNAL_COM4);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);
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

	return status;
}

/**	Возвращает имеющуюся команду на исполнение.
 * 	Сначала проверяется срочная команда, если ее нет идет перебор текущих.
 * 	Каждый цикл (состоящий из 4-х посылок) опрашиваются:
 * 	текущее состояние
 * 	команда из буфера 1
 * 		первым всегда идет текущий параметр, если есть
 * 		затем команды необходимые для настройки меню (совместимость и т.д.)
 * 		если команд в буфере нет, то посылается команда из буфера 2
 * 	команда из буфера 2
 * 		команды опроса остальных параметров в данном уровне меню
 * 	@param Нет
 * 	@return Команда
 */
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
			snprintf_P(&vLCDbuf[poz], 11, fcDate, sParam.dataTime.getDay(),
					sParam.dataTime.getMonth(), sParam.dataTime.getYear());
			break;
		case MENU_MEAS_PARAM_TIME:
			snprintf_P(&vLCDbuf[poz], 11, fcTime, sParam.dataTime.getHour(),
					sParam.dataTime.getMinute(), sParam.dataTime.getSecond());
			break;
		case MENU_MEAS_PARAM_UZ:
			snprintf_P(&vLCDbuf[poz], 11, fcUz,
					sParam.measParam.getVoltageDef());
			break;

			// TODO ВСЕ для Uз1 и Uз2 нужны свои параметры
		case MENU_MEAS_PARAM_UZ1:
			snprintf_P(&vLCDbuf[poz], 11, fcUz1,
					sParam.measParam.getVoltageDef());
			break;
		case MENU_MEAS_PARAM_UZ2:
			snprintf_P(&vLCDbuf[poz], 11, fcUz2,
					sParam.measParam.getVoltageDef());
			break;

		case MENU_MEAS_PARAM_UC:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf,
					sParam.measParam.getVoltageCf());
			break;

			// TODO ВСЕ для Uк1 и Uк2 нужны свои параметры
		case MENU_MEAS_PARAM_UC1:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf1,
					sParam.measParam.getVoltageCf());
			break;
		case MENU_MEAS_PARAM_UC2:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf2,
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

			// TODO ВСЕ для Uш1 и Uш2 нужны свои параметры
		case MENU_MEAS_PARAM_UN1:
			snprintf_P(&vLCDbuf[poz], 11, fcUn1,
					sParam.measParam.getVoltageNoise());
			break;
		case MENU_MEAS_PARAM_UN2:
			snprintf_P(&vLCDbuf[poz], 11, fcUn2,
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
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
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
	poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}

/** Уровень "Ошибочный тип аппарата"
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlError() {
	static const char fcNoTypeDevice0[] PROGMEM = "    Тип аппарата    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   не определен!!!  ";

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

	snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
	snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);

	if (sParam.typeDevice != AVANT_NO) {
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
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
		numPunkts_ = 0;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();
		// дополнительные команды
		// буфер 1
		// неисправности, измеряемые параметры (в ВЧ)
		sParam.txComBuf.addCom1(GB_COM_GET_FAULT);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);

		// буфер 2
		// время, АК в Р400
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
		if (sParam.typeDevice == AVANT_R400M)
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_TYPE_AC);
	}

	// вывод на экран измеряемых параметров
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (blinkMeasParam_)
			printMeasParam(i, measParam[i]);
		else
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
	}

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;

		// в Р400м выводится АК и время до АК
		if (sParam.typeDevice == AVANT_R400M) {
			uint16_t time = sParam.def.getTimeToAC();
			eGB_TYPE_AC ac = sParam.def.getTypeAC();
			uint8_t t = poz + 20;
			t += snprintf_P(&vLCDbuf[t],11,fcAcType[static_cast<uint8_t>(ac)]);

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
		poz += 20;
	}

	eGB_TYPE_DEVICE type = sParam.typeDevice;
	if (sParam.typeDevice == AVANT_R400M) {
		switch (key_) {
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_CALL:
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_PRD:
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_AC_UD:
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_NALAD:
			if (sParam.def.status.getState() == 7) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			break;

		case KEY_FUNC_RESET_AC:
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_AC_PUSK:
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
			break;

		case KEY_FUNC_REG_AC:
			sParam.txComBuf.setInt8(GB_CONTROL_REG_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		default:
			break;
		}
	} else {
		switch (key_) {
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_RES_IND:
			if ((type == AVANT_K400) || (type == AVANT_RZSK))
				sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND);
			break;

		case KEY_FUNC_ENTER:
			if ((type == AVANT_K400) || (type == AVANT_RZSK))
				sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER);
			break;

		case KEY_FUNC_RESET:
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		default:
			break;
		}
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
	static char punkt4[] PROGMEM = "%d. Тест";
	static char punkt5[] PROGMEM = "%d. Информация";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		lineParam_ = 1;
		cursorEnable_ = true;

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

	snprintf_P(&vLCDbuf[0], 21, title);

	if (isMessage()) {
		static char message[][21] PROGMEM = { "    Перейдите в     ",
				"   тестовый режим   " };

		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		uint8_t poz = 40;
		for (uint_fast8_t i = 0; i < 2; i++, poz += 20)
			snprintf_P(&vLCDbuf[poz], 21, message[i]);
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
			if (sParam.glb.status.getRegime() == GB_REGIME_TEST_1) {
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			} else if (sParam.glb.status.getRegime() == GB_REGIME_TEST_2) {
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			} else
				printMessage();
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
void clMenu::lvlInfo() {
	static char title[] PROGMEM = "Меню\\Информация";
	static char bspMcu[] PROGMEM = "БСП MCU : %02X.%02x";
	static char bspDsp[] PROGMEM = "БСП DSP : %02X.%02x";
	static char piMcu[] PROGMEM = "ПИ  MCU : %02X.%02x";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// доплнительные команды
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint16_t vers = sParam.glb.getVersBsp();
	snprintf_P(&vLCDbuf[20], 21, bspMcu, (uint8_t) (vers >> 8), (uint8_t) vers);
	vers = sParam.glb.getVersDsp();
	snprintf_P(&vLCDbuf[40], 21, bspDsp, (uint8_t) (vers >> 8), (uint8_t) vers);
	snprintf_P(&vLCDbuf[60], 21, piMcu, (uint8_t) (VERS >> 8), (uint8_t) VERS);

	switch (key_) {
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

		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();

		// активация необходимых пунктов меню и соответствующих им команд
		// по умолчанию только журнал событий
		punkt_[num++] = punkt1;
		if (type == AVANT_R400M) {
			punkt_[num++] = punkt2;
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		} else if (type == AVANT_K400) {
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		numPunkts_ = num;
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

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;
	case KEY_ENTER:
		if (punkt_[cursorLine_ - 1] == punkt1) {
			lvlMenu = &clMenu::lvlJournalEvent;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			lvlMenu = &clMenu::lvlJournalDef;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			lvlMenu = &clMenu::lvlJournalPrm;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
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

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
		uint16_t t = 0;
		if (sParam.typeDevice == AVANT_K400)
			t = GLB_JRN_EVENT_K400_MAX;
		else if (sParam.typeDevice == AVANT_R400M)
			t = GLB_JRN_EVENT_R400_MSK_MAX;
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
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
		// вывод режима
		snprintf_P(&vLCDbuf[poz], 21, fcRegimeJrn);
		snprintf_P(&vLCDbuf[poz + 7], 13,
				fcRegime[sParam.jrnEntry.getRegime()]);
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// вывод события
		uint8_t event = sParam.jrnEntry.getEventType();

		if (sParam.typeDevice == AVANT_R400M) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
		} else
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
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
		if (sParam.typeDevice == AVANT_R400M)
			t = GLB_JRN_DEF_R400_MSK_MAX;
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
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());

		poz += 20;
//		uint8_t signals = sParam.jrnEntry.getSignalDef();
//		snprintf(&vLCDbuf[poz], 21, "%x",signals);
		snprintf_P(&vLCDbuf[poz], 21, fcSignalDefJrn,
				sParam.jrnEntry.getSignalPusk(),
				sParam.jrnEntry.getSignalStop(), sParam.jrnEntry.getSignalMan(),
				sParam.jrnEntry.getSignalPrd(), sParam.jrnEntry.getSignalPrm(),
				sParam.jrnEntry.getSignalOut());
//		if (sParam.typeDevice == AVANT_R400_MSK)
//		{
//			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
//		}
//		else
//			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
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

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
		if (sParam.typeDevice == AVANT_K400)
			sParam.jrnEntry.setMaxNumJrnEntries(GLB_JRN_PRM_K400_MAX);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
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
		// вывод номера команды
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, sParam.jrnEntry.getNumCom());
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// вывод события
		uint8_t event = sParam.jrnEntry.getEventType();
		snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
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

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// установка текущего журнала и максимального кол-во записей в нем
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
		if (sParam.typeDevice == AVANT_K400)
			sParam.jrnEntry.setMaxNumJrnEntries(GLB_JRN_PRD_K400_MAX);

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_ENTRY);
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
		// вывод номера команды
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, sParam.jrnEntry.getNumCom());
		poz += 20;
		// вывод даты
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		// вывод времени
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// вывод события
		uint8_t event = sParam.jrnEntry.getEventType();
		snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
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
	static char punkt15[] PROGMEM = "%d. АК автоматическ.";
	static char punkt16[] PROGMEM = "%d. АК ускоренный";
	static char punkt17[] PROGMEM = "%d. АК выключен";
	static char punkt18[] PROGMEM = "%d. АК испытания";
	static char punkt19[] PROGMEM = "%d. АК нормальный";
	static char punkt20[] PROGMEM = "%d. АК беглый";
	static char punkt21[] PROGMEM = "%d. АК односторонний";
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

		// сброс флага смены совместимости
		sParam.glb.isCompatibilityRefresh();
		// сброс флага смены кол-ва аппаратов
		sParam.glb.isNumDevicesRefresh();

		uint8_t num = 0;
		if (device == AVANT_R400M) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			eGB_COMPATIBILITY compatibility = sParam.glb.getCompatibility();

			// первым всегда идет пуск наладочный
			punkt_[num++] = punkt07;
			// сброс своего есть во всех аппаратах и совместимостях
			punkt_[num++] = punkt03;
			if (compatibility == GB_COMPATIBILITY_AVANT) {
				if (numDevices == GB_NUM_DEVICES_2) {
					punkt_[num++] = punkt04;
					punkt_[num++] = punkt02;
					punkt_[num++] = punkt11;
					punkt_[num++] = punkt12;
					punkt_[num++] = punkt13;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt17;
				} else if (numDevices == GB_NUM_DEVICES_3) {
					punkt_[num++] = punkt22;
					punkt_[num++] = punkt23;
					punkt_[num++] = punkt24;
					punkt_[num++] = punkt26;
					punkt_[num++] = punkt11;
					punkt_[num++] = punkt12;
					punkt_[num++] = punkt13;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt17;
				}
			} else if (compatibility == GB_COMPATIBILITY_PVZ90) {
				punkt_[num++] = punkt04;
				punkt_[num++] = punkt19;
				punkt_[num++] = punkt16;
				punkt_[num++] = punkt17;
				punkt_[num++] = punkt18;
				punkt_[num++] = punkt08;
			} else if (compatibility == GB_COMPATIBILITY_PVZUE) {
				if (numDevices == GB_NUM_DEVICES_2) {
					punkt_[num++] = punkt02;
					punkt_[num++] = punkt09;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt20;
					punkt_[num++] = punkt10;
					punkt_[num++] = punkt17;
				} else if (numDevices == GB_NUM_DEVICES_3) {
					punkt_[num++] = punkt23;
					punkt_[num++] = punkt24;
					punkt_[num++] = punkt26;
					punkt_[num++] = punkt27;
					punkt_[num++] = punkt28;
					punkt_[num++] = punkt30;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt20;
					punkt_[num++] = punkt10;
					punkt_[num++] = punkt17;
				}
			} else if (compatibility == GB_COMPATIBILITY_AVZK80) {
				punkt_[num++] = punkt19;
				punkt_[num++] = punkt16;
				punkt_[num++] = punkt17;
				punkt_[num++] = punkt18;
				punkt_[num++] = punkt08;
			} else if (compatibility == GB_COMPATIBILITY_PVZL) {
				punkt_[num++] = punkt11;
				punkt_[num++] = punkt12;
				punkt_[num++] = punkt13;
				punkt_[num++] = punkt14;
				punkt_[num++] = punkt31;
				punkt_[num++] = punkt17;
			}
			// Вызов есть во всех совместимостях
			punkt_[num++] = punkt05;
		} else if (sParam.typeDevice == AVANT_RZSK) {
			punkt_[num++] = punkt07;
			punkt_[num++] = punkt03;
			punkt_[num++] = punkt05;
		} else if (sParam.typeDevice == AVANT_K400) {
			punkt_[num++] = punkt03;
		}
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		// TODO Р400м Если анализировать команды переданные с ПК, то не надо
		if (sParam.typeDevice == AVANT_R400M) {
			// совместимость
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// кол-во аппаратов в линии
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// номер текущего аппарата
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		}
	}


	// в Р400М меню управление зависит от кол-ва аппаратов в линии
	// и типа совместимости
	// поэтому при их изменении обновим уровень меню
	if (device == AVANT_R400M) {
		if (sParam.glb.isCompatibilityRefresh())
			lvlCreate_ = true;

		if (sParam.glb.isNumDevicesRefresh())
			lvlCreate_ = true;
	}

	// в РЗСК/Р400м
	if ((device == AVANT_R400M) || (device == AVANT_RZSK))
		punkt_[0] = (sParam.def.status.getState() != 7) ? punkt06 : punkt07;



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

	case KEY_ENTER: {
		PGM_P p = punkt_[cursorLine_ - 1];

		if (p == punkt02)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt03)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt04)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt05)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt06)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt07)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt11)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt12)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt13)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt14)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
//		else if (p == punkt15)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_FAST);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (p == punkt16)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_FAST);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt17)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt19)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
//		else if (p == punkt21)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (p == punkt22) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt23) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt24) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt25) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt26) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt27) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt28) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt29) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt30) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt31) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
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

	if (lvlCreate_) {
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
	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterParam.getStatus();

		// выбор функции ввода : пароль или параметр
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PASSWORD_READY) {
			uint16_t val = enterParam.getValue();

			if (sParam.password.check(val)) {
				enterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
			} else
				enterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_N_READY) {
			uint16_t val = enterParam.getValue();

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
	case KEY_ENTER: {
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
		else if (p == punkt4)
		{
			enterFunc = &clMenu::enterPassword;
			enterParam.setEnable(MENU_ENTER_PASSWORD);
			enterParam.com = GB_COM_NO;
		}
	}
	break;

	default:
	break;
}
}

void clMenu::lvlRegime() {
	static char title[] PROGMEM = "Настройка\\Режим";
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
		poz += 20;
	}

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterParam.getStatus();

		// выбор функции ввода : пароль или параметр
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			eGB_REGIME_ENTER val =
					(eGB_REGIME_ENTER) enterParam.getValueEnter();
			eGB_COM com = GB_COM_NO;

			if (val == GB_REGIME_ENTER_DISABLED)
				com = GB_COM_SET_REG_DISABLED;
			else if (val == GB_REGIME_ENTER_ENABLED)
				com = GB_COM_SET_REG_ENABLED;
			else if (val == GB_REGIME_ENTER_TEST_1) {
				com = GB_COM_SET_REG_TEST_1;
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 1);
			} else if (val == GB_REGIME_ENTER_TEST_2)
				com = GB_COM_SET_REG_TEST_2;

			sParam.txComBuf.addFastCom(com);
			enterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_READY) {
			// пароль введен верно
			// переход к вводу режима работы (можно только вывести)
			if (sParam.password.check(enterParam.getValue())) {
				enterFunc = &clMenu::enterValue;
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_REGIME_ENTER_DISABLED,
						GB_REGIME_ENTER_DISABLED);
				enterParam.setValue(GB_REGIME_ENTER_DISABLED);
				enterParam.list = fcRegimeEnter[0];
				enterParam.com = GB_COM_NO;
			} else {
				enterParam.printMessage();
			}
		}
	}

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// если хоть одно из имеющихся устройств введено
			// на изменение режима требуется пароль
			enterFunc = &clMenu::enterPassword;
			enterParam.setEnable(MENU_ENTER_PASSWORD);
		} else {
			// если аппарат выведен, доступны тесты
			enterFunc = &clMenu::enterValue;
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_REGIME_ENTER_DISABLED,
					GB_REGIME_ENTER_MAX - 1);
			enterParam.setValue(GB_REGIME_ENTER_ENABLED);
			enterParam.list = fcRegimeEnter[0];
			enterParam.com = GB_COM_NO;

		}
		break;

	default:
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

		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_R400M) {
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt4;
		} else if (sParam.typeDevice == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		} else if (sParam.typeDevice == AVANT_K400) {
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
	case KEY_ENTER:
		if (punkt_[cursorLine_ - 1] == punkt1) {
			lvlMenu = &clMenu::lvlSetupParamDef;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			lvlMenu = &clMenu::lvlSetupParamPrm;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			lvlMenu = &clMenu::lvlSetupParamPrd;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
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
	static char title[] PROGMEM = "Параметры\\Защиты";
	static char punkt1[] PROGMEM = "Тип защиты";
	static char punkt2[] PROGMEM = "Тип линии";
	static char punkt3[] PROGMEM = "Доп. время без ман.";
	static char punkt4[] PROGMEM = "Перекрытие импульсов";
	static char punkt5[] PROGMEM = "Компенсация задержки";
	static char punkt6[] PROGMEM = "Порог предупр по РЗ";
	static char punkt7[] PROGMEM = "Загруб.чувств. по РЗ";
	static char punkt8[] PROGMEM = "Тип приемника";
	static char punkt9[] PROGMEM = "Снижение уровня АК";
	static char punkt10[] PROGMEM = "Частота ПРД";
	static char punkt11[] PROGMEM = "Частота ПРМ";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		enterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// сброс флага изменения совместимости
		sParam.glb.isCompatibilityRefresh();

		// заполнение массивов параметров и команд
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DEF_TYPE);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_T_NO_MAN);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_OVERLAP);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DELAY);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_THRESH);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_DEC);
			punkt_[num++] = punkt8;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_PRM_TYPE);
		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY t = sParam.glb.getCompatibility();

			// добавляется опрос совместимости, для переформирования меню
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);

			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DEF_TYPE);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_T_NO_MAN);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_OVERLAP);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DELAY);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_DEC);
			if (t == GB_COMPATIBILITY_AVANT) {
				punkt_[num++] = punkt9;
				sParam.txComBuf.addCom2(GB_COM_DEF_GET_PRM_TYPE);
			}
			punkt_[num++] = punkt10;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_FREQ_PRD);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_THRESH);
		}
		numPunkts_ = num;
	}

	// обновление уровня, при наличии смены совместимости
	if (sParam.glb.isCompatibilityRefresh())
		lvlCreate_ = true;

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_T_NO_MAN_MIN,
				DEF_T_NO_MAN_MAX, "час");
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_OVERLAP_MIN,
				DEF_OVERLAP_MAX, "град");
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_DELAY_MIN, DEF_DELAY_MAX,
				"град");
	} else if (p == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_THRESH_MIN,
				DEF_RZ_THRESH_MAX, "дБ");
	} else if (p == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_DEC_MIN,
				DEF_RZ_DEC_MAX, "дБ");
	} else if (p == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt4) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt5) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt6) {
				// !!! TODO РЗСК
			} else if (p == punkt7) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt8) {
				// !!! TODO РЗСК
			} else if (p == punkt9) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt10) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt11) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			}

			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {

		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			uint8_t val = sParam.def.getDefType();
			snprintf_P(&vLCDbuf[poz], 11, fcDefType[val]);
		} else if (p == punkt2) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		} else if (p == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%dчас", sParam.def.getTimeNoMan());
		} else if (p == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%dград", sParam.def.getOverlap());
		} else if (p == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%dград", sParam.def.getDelay());
		} else if (p == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzThreshold());
		} else if (p == punkt7) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.def.getRzDec());
		} else if (p == punkt8) {
			snprintf_P(&vLCDbuf[poz], 11, fcPrmType[sParam.def.getPrmType()]);
		} else if (p == punkt9) {
			uint8_t val = sParam.def.getAcDec() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt10) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrd()]);
		} else if (p == punkt11) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrm()]);
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
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.def.status.getRegime() == GB_REGIME_ENABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			enterParam.printMessage();
		} else {

			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(DEF_TYPE_MIN, DEF_TYPE_MAX - 1);
				enterParam.setValue(sParam.def.getDefType());
				enterParam.list = fcDefType[0];
				enterParam.com = GB_COM_DEF_SET_DEF_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_NUM_DEVICES_MIN,
						GB_NUM_DEVICES_MAX - 1);
				enterParam.setValue(sParam.def.getNumDevices());
				enterParam.list = fcNumDevices[0];
				enterParam.com = GB_COM_DEF_SET_LINE_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_T_NO_MAN_MIN, DEF_T_NO_MAN_MAX);
				enterParam.setValue(sParam.def.getTimeNoMan());
				enterParam.setDisc(DEF_T_NO_MAN_DISC);
				enterParam.setFract(DEF_T_NO_MAN_FRACT);
				enterParam.com = GB_COM_DEF_SET_T_NO_MAN;
			} else if (punkt_[cursorLine_ - 1] == punkt4) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_OVERLAP_MIN, DEF_OVERLAP_MAX);
				enterParam.setValue(sParam.def.getOverlap());
				enterParam.setDisc(DEF_OVERLAP_DISC);
				enterParam.setFract(DEF_OVERLAP_FRACT);
				enterParam.com = GB_COM_DEF_SET_OVERLAP;
			} else if (punkt_[cursorLine_ - 1] == punkt5) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_DELAY_MIN, DEF_DELAY_MAX);
				enterParam.setValue(sParam.def.getDelay());
				enterParam.setDisc(DEF_DELAY_DISC);
				enterParam.setFract(DEF_DELAY_FRACT);
				enterParam.com = GB_COM_DEF_SET_DELAY;
			} else if (punkt_[cursorLine_ - 1] == punkt6) {
				// !!! РЗСК надо сделать
			} else if (punkt_[cursorLine_ - 1] == punkt7) {
				// !!! Р400 в 3-х концевой 2 разных
				enterParam.setEnable();
				enterParam.setValueRange(DEF_RZ_DEC_MIN, DEF_RZ_DEC_MAX);
				enterParam.setValue(sParam.def.getRzDec());
				enterParam.setDisc(DEF_RZ_DEC_DISC);
				enterParam.setFract(DEF_RZ_DEC_FRACT);
				enterParam.com = GB_COM_DEF_SET_RZ_DEC;
			} else if (punkt_[cursorLine_ - 1] == punkt8) {
				// !!! РЗСК надо сделать
			} else if (punkt_[cursorLine_ - 1] == punkt9) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.def.getAcDec() ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_SET_PRM_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt10) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_PVZL_FREQ_MIN, GB_PVZL_FREQ_MAX-1);
				enterParam.setValue(sParam.def.getFreqPrd());
				enterParam.list = fcPvzlFreq[0];
				enterParam.com = GB_COM_DEF_SET_FREQ_PRD;
			} else if (punkt_[cursorLine_ - 1] == punkt11) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_PVZL_FREQ_MIN, GB_PVZL_FREQ_MAX-1);
				enterParam.setValue(sParam.def.getFreqPrm());
				enterParam.list = fcPvzlFreq[0];
				enterParam.com = GB_COM_DEF_SET_RZ_THRESH;
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

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_BLOCK_COM);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_ON);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_OFF);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_BLOCK_COM);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_ON);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_OFF);
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	// при необходимости, вывод подномера данного параметра
	poz = 60;
	if ((p == punkt2) || (p == punkt3)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prm.getNumCom());
	}

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_ON_MIN,
				PRM_TIME_ON_MAX, "мс");
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_OFF_MIN,
				PRM_TIME_OFF_MAX, "мс");
	}

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prm.getTimeOn());
		} else if (p == punkt2) {
			uint8_t val = (sParam.prm.getBlockCom(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%dмс",
					sParam.prm.getTimeOff(curCom_ - 1));
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

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			enterParam.printMessage();
		} else {
			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable();
				enterParam.setValueRange(PRM_TIME_ON_MIN, PRM_TIME_ON_MAX);
				enterParam.setValue(sParam.prm.getTimeOn());
				enterParam.setDisc(PRM_TIME_ON_DISC);
				enterParam.setFract(PRM_TIME_ON_FRACT);
				enterParam.com = GB_COM_PRM_SET_TIME_ON;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.setDopValue(curCom_);
				enterParam.com = GB_COM_PRM_SET_BLOCK_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable();
				enterParam.setValueRange(PRM_TIME_OFF_MIN, PRM_TIME_OFF_MAX);
				enterParam.setValue(sParam.prm.getTimeOff(curCom_ - 1));
				enterParam.setDopValue(curCom_);
				enterParam.setDisc(PRM_TIME_OFF_DISC);
				enterParam.setFract(PRM_TIME_OFF_FRACT);
				enterParam.com = GB_COM_PRM_SET_TIME_OFF;
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
	static char title[] PROGMEM = "Параметры\\Передатчик";
	static char punkt1[] PROGMEM = "Задержка срабат. ПРД";
	static char punkt2[] PROGMEM = "Длительность команды";
	static char punkt3[] PROGMEM = "Тестовая команда";
	static char punkt4[] PROGMEM = "Следящие команды";
	static char punkt5[] PROGMEM = "Блокиров. команды";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TIME_ON);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_DURATION);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TEST_COM);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_LONG_COM);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_BLOCK_COM);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TIME_ON);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_DURATION);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_LONG_COM);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_BLOCK_COM);
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	// при необходимости выводим подномер данного параметра
	poz = 60;
	if ((p == punkt4) || (p == punkt5)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prd.getNumCom());
	}

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_TIME_ON_MIN,
				PRD_TIME_ON_MAX, "мс");
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURATION_MIN,
				PRD_DURATION_MAX, "мс");
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	}

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());

			} else if (p == punkt4) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getLongCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (p == punkt5) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {

		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getTimeOn());
		} else if (p == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%dмс", sParam.prd.getDuration());
		} else if (p == punkt3) {
			uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt4) {
			uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt5) {
			uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
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

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			enterParam.printMessage();
		} else {

			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable();
				enterParam.setValueRange(PRD_TIME_ON_MIN, PRD_TIME_ON_MAX);
				enterParam.setValue(sParam.prd.getTimeOn());
				enterParam.setDisc(PRD_TIME_ON_DISC);
				enterParam.setFract(PRD_TIME_ON_FRACT);
				enterParam.com = GB_COM_PRD_SET_TIME_ON;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable();
				enterParam.setValueRange(PRD_DURATION_MIN, PRD_DURATION_MAX);
				enterParam.setValue(sParam.prd.getDuration());
				enterParam.setDisc(PRD_DURATION_DISC);
				enterParam.setFract(PRD_DURATION_FRACT);
				enterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_TEST_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt4) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.setDopValue(curCom_);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_LONG_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt5) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.setDopValue(curCom_);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_BLOCK_COM;
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
	static char punkt1[] PROGMEM = "Синхронизация часов";
	static char punkt2[] PROGMEM = "Номер аппарата";
	static char punkt3[] PROGMEM = "Контроль вых.сигнала";
	static char punkt4[] PROGMEM = "Порог предупр. по КЧ";
	static char punkt5[] PROGMEM = "Время перезапуска";
	static char punkt6[] PROGMEM = "Удерж. реле ком. ПРД";
	static char punkt7[] PROGMEM = "Удерж. реле ком. ПРМ";
	static char punkt8[] PROGMEM = "Загрубл. чувств. ПРМ";
	static char punkt9[] PROGMEM = "Сетевой адрес";
	static char punkt10[] PROGMEM = "Uвых номинальное";
	static char punkt11[] PROGMEM = "Частота";
	static char punkt12[] PROGMEM = "Совместимость";		// защита
	static char punkt13[] PROGMEM = "Снижение ответа АК";
	static char punkt14[] PROGMEM = "Тип детектора";
	static char punkt15[] PROGMEM = "Коррекция напряжения";
	static char punkt16[] PROGMEM = "Коррекция тока";
	static char punkt17[] PROGMEM = "Протокол обмена";
	static char punkt18[] PROGMEM = "Признак четности";
	static char punkt19[] PROGMEM = "Допустимые провалы";
	static char punkt20[] PROGMEM = "Порог по помехе";
	static char punkt21[] PROGMEM = "Допустимая помеха";
	static char punkt22[] PROGMEM = "Тип автоконтроля";
	static char punkt23[] PROGMEM = "Резервирование";
	static char punkt24[] PROGMEM = "Совместимость";		// упаск

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		enterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// заполнение массивов параметров и команд
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		sParam.glb.isCompatibilityRefresh();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt8;
			// sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);					// т.к. уже есть такая команда
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt24;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);// TODO К400 команда "Совместимость"
			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I	);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt8;
			// sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);					// т.к. уже есть такая команда
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt14;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);		// TODO РСЗК команда
			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I);

		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY t = sParam.glb.getCompatibility();

			// TODO Р400м Если ананлизировать команды с ПК, то не надо
			// опрашиваем совместимость
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// измеряемые параметры для коррекции
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);

			// в совместимостях отличия минимальные

			if (t == GB_COMPATIBILITY_AVANT) {
				punkt_[num++] = punkt1;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			}

			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt10;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt12;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			if (t == GB_COMPATIBILITY_PVZL) {
				punkt_[num++] = punkt13;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			}

			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I);

			if (t == GB_COMPATIBILITY_PVZUE) {
				punkt_[num++] = punkt17;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt18;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt19;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt20;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt21;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt22;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			}
		}
		numPunkts_ = num;
	}

	// обновление уровня, при наличии смены совместимости
	if (sParam.glb.isCompatibilityRefresh())
		lvlCreate_ = true;

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	//  вывод надписи "Диапазон:" и переход к выводу самого диапазона
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DEV_NUM_MIN,
				sParam.glb.getMaxNumDevices(), "");
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CF_THRESH_MIN,
				GLB_CF_THRESH_MAX, "дБ");
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_T_RERUN_MIN,
				GLB_T_RERUN_MAX, "c");
	} else if (p == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_IN_DEC_MIN,
				GLB_IN_DEC_MAX, "дБ");
	} else if (p == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_NET_ADR_MIN,
				GLB_NET_ADR_MAX, "");
	} else if (p == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_U_OUT_NOM_MIN,
				GLB_U_OUT_NOM_MAX, "В");
	} else if (p == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_FREQ_MIN, GLB_FREQ_MAX,
				"кГц");
	} else if (p == punkt12) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt13) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_AC_IN_DEC_MIN,
				GLB_AC_IN_DEC_MAX, "дБ");
	} else if (p == punkt15) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_COR_U_DEC_MIN / 10,
				GLB_COR_U_DEC_MAX / 10, "В");
	} else if (p == punkt16) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, 0, GLB_COR_I_DEC_MAX, "мА");
	} else if (p == punkt17) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt18) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt19) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_FAIL_MIN,
				GLB_PVZUE_FAIL_MAX, "град");
	} else if (p == punkt20) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_TH_MIN,
				GLB_PVZUE_N_TH_MAX, "сек");
	} else if (p == punkt21) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_LVL_MIN,
				GLB_PVZUE_N_LVL_MAX, "град");
	} else if (p == punkt22) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// новое значение введено, надо передать в БСП

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt4) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt5) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt6) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt7) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt8) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt9) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt10) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt11) {
				uint16_t t = enterParam.getValue();
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t >> 8), 0);
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t), 1);
			} else if (p == punkt12) {
				uint8_t t = enterParam.getValueEnter();
				sParam.txComBuf.setInt8(t);
			} else if (p == punkt13) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt15) {
				// если текущее значение коррекции тока равно 0
				// то передается сообщение с под.байтом равным 4
				// означающим сброс коррекции
				int16_t t = static_cast<int16_t>(enterParam.getValue());
				uint8_t dop = enterParam.getDopValue();
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
			} else if (p == punkt16) {
				// если текущее значение коррекции тока равно 0
				// то передается сообщение с под.байтом равным 5
				// означающим сброс коррекции
				int16_t t = static_cast<int16_t>(enterParam.getValue());
				uint8_t dop = enterParam.getDopValue();
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
			} else if (p == punkt17) {
				// TODO Р400м ПВЗУ-Е
			} else if (p == punkt18) {
				// TODO Р400м ПВЗУ-Е
			} else if (p == punkt19) {
				// TODO Р400м ПВЗУ-Е
			} else if (p == punkt20) {
				// TODO Р400м ПВЗУ-Е
			} else if (p == punkt21) {
				// TODO Р400м ПВЗУ-Е
			} else if (p == punkt22) {
				// TODO Р400м ПВЗУ-Е
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		// вывод надписи "Значение:" и переход к выводу самого значения
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getDeviceNum());
		} else if (p == punkt3) {
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getCfThreshold());
		} else if (p == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%dс", sParam.glb.getTimeRerun());
		} else if (p == punkt6) {
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt7) {
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt8) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getInDecrease());
		} else if (p == punkt9) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getNetAddress());
		} else if (p == punkt10) {
			snprintf(&vLCDbuf[poz], 11, "%dВ", sParam.glb.getUoutNom());
		} else if (p == punkt11) {
			snprintf(&vLCDbuf[poz], 11, "%dкГц", sParam.glb.getFreq());
		} else if (p == punkt12) {
			uint8_t val = static_cast<uint8_t>(sParam.glb.getCompatibility());
			snprintf_P(&vLCDbuf[poz], 11, fcCompatibility[val]);
		} else if (p == punkt13) {
			snprintf(&vLCDbuf[poz], 11, "%dдБ", sParam.glb.getAcInDec());
		} else if (p == punkt15) {
			int16_t val = sParam.glb.getCorU();
			int8_t f = val % 10;
			if (f < 0)
				f = -f;
			// вывоится целая/дробная часть напряжения коррекции
			// в дробной части дополнительно отсекается знак числа
			snprintf(&vLCDbuf[poz], 11, "%d.%dВ", val / 10, f);
		} else if (p == punkt16) {
			snprintf(&vLCDbuf[poz], 11, "%dмА", sParam.glb.getCorI());
		} else if (p == punkt17) {
			uint8_t val = sParam.glb.getPvzueProtocol();
			snprintf(&vLCDbuf[poz], 11, fcPvzueProtocol[val]);
		} else if (p == punkt18) {
			uint8_t val = sParam.glb.getPvzueParity();
			snprintf(&vLCDbuf[poz], 11, fcPvzueParity[val]);
		} else if (p == punkt19) {
			snprintf(&vLCDbuf[poz], 11, "%uград", sParam.glb.getPvzueFail());
		} else if (p == punkt20) {
			snprintf(&vLCDbuf[poz], 11, "%uсек", sParam.glb.getPvzueNoiseTH());
		} else if (p == punkt21) {
			snprintf(&vLCDbuf[poz], 11, "%uград",
					sParam.glb.getPvzueNoiseLvl());
		} else if (p == punkt22) {
			uint8_t val = sParam.glb.getPvzueTypeAC();
			snprintf(&vLCDbuf[poz], 11, fcPvzueTypeAC[val]);
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
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// коррекция тока и напряжения вводятся в любом режиме
		// остальные параметры только в "Выведен"
		if (punkt_[cursorLine_ - 1] == punkt15) {
			// вводится реальное значение напряжения
			// по умолчанию стоит 0, что значит сброс настроек
			enterParam.setEnable(MENU_ENTER_PARAM_U_COR);
			enterParam.setValueRange(0, GLB_COR_U_DEC_MAX);
			enterParam.setValue(0);
			enterParam.setDisc(GLB_COR_U_DEC_DISC);
			enterParam.setFract(GLB_COR_U_DEC_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_COR_U_I;
		} else if (punkt_[cursorLine_ - 1] == punkt16) {
			// вводится реальное значение тока
			// по умолчанию стоит 0, что значит сброс настроек
			enterParam.setEnable();
			enterParam.setValueRange(0, GLB_COR_I_DEC_MAX);
			enterParam.setValue(0);
			enterParam.setDisc(GLB_COR_I_DEC_DISC);
			enterParam.setFract(GLB_AC_IN_DEC_FRACT);
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_SET_COR_U_I;
		} else if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// если хоть одно из имеющихся устройств введено
			// изменение параметров запрещено
			enterParam.printMessage();
		} else if (punkt_[cursorLine_ - 1] == punkt1) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_DEV_NUM_MIN,
					sParam.glb.getMaxNumDevices());
			enterParam.setValue(sParam.glb.getDeviceNum());
			enterParam.setDisc(GLB_DEV_NUM_DISC);
			enterParam.setFract(GLB_DEV_NUM_FRACT);
			enterParam.com = GB_COM_SET_DEVICE_NUM;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_OUT_CHECK;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_CF_THRESH_MIN, GLB_CF_THRESH_MAX);
			enterParam.setValue(sParam.glb.getCfThreshold());
			enterParam.setDisc(GLB_CF_THRESH_DISC);
			enterParam.setFract(GLB_CF_THRESH_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (punkt_[cursorLine_ - 1] == punkt5) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_T_RERUN_MIN, GLB_T_RERUN_MAX);
			enterParam.setValue(sParam.glb.getTimeRerun());
			enterParam.setDisc(GLB_T_RERUN_DISC);
			enterParam.setFract(GLB_T_RERUN_FRACT);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt6) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt7) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt8) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_IN_DEC_MIN, GLB_IN_DEC_MAX);
			enterParam.setValue(sParam.glb.getInDecrease());
			enterParam.setDisc(GLB_IN_DEC_DISC);
			enterParam.setFract(GLB_IN_DEC_FRACT);
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (punkt_[cursorLine_ - 1] == punkt9) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_NET_ADR_MIN, GLB_NET_ADR_MAX);
			enterParam.setValue(sParam.glb.getNetAddress());
			enterParam.setDisc(GLB_NET_ADR_DISC);
			enterParam.setFract(GLB_NET_ADR_FRACT);
			enterParam.com = GB_COM_SET_NET_ADR;
		} else if (punkt_[cursorLine_ - 1] == punkt10) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_U_OUT_NOM_MIN, GLB_U_OUT_NOM_MAX);
			enterParam.setValue(sParam.glb.getUoutNom());
			enterParam.setDisc(GLB_U_OUT_NOM_DISC);
			enterParam.setFract(GLB_U_OUT_NOM_FRACT);
			enterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt11) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_FREQ_MIN, GLB_FREQ_MAX);
			enterParam.setValue(sParam.glb.getFreq());
			enterParam.setDisc(GLB_FREQ_DISC);
			enterParam.setFract(GLB_FREQ_FRACT);
			enterParam.com = GB_COM_SET_FREQ;
		} else if (punkt_[cursorLine_ - 1] == punkt12) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_COMPATIBILITY_MIN,
					GB_COMPATIBILITY_MAX - 1);
			uint8_t val = (uint8_t) sParam.glb.getCompatibility();
			enterParam.setValue(val);
			enterParam.list = fcCompatibility[0];
			enterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt13) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_AC_IN_DEC_MIN, GLB_AC_IN_DEC_MAX);
			enterParam.setValue(sParam.glb.getAcInDec());
			enterParam.setDisc(GLB_AC_IN_DEC_DISC);
			enterParam.setFract(GLB_AC_IN_DEC_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt17) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_PROTOCOL_MIN,
					GB_PVZUE_PROTOCOL_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueProtocol());
			enterParam.list = fcPvzueProtocol[0];
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_GET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt18) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_PARITY_MIN,
					GB_PVZUE_PARITY_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueParity());
			enterParam.list = fcPvzueParity[0];
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_GET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt19) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_FAIL_MIN, GLB_PVZUE_FAIL_MAX);
			enterParam.setValue(sParam.glb.getPvzueFail());
			enterParam.setDisc(GLB_PVZUE_FAIL_DISC);
			enterParam.setFract(GLB_PVZUE_FAIL_FRACT);
			enterParam.setDopValue(3);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt20) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_N_TH_MIN, GLB_PVZUE_N_TH_MAX);
			enterParam.setValue(sParam.glb.getPvzueNoiseTH());
			enterParam.setDisc(GLB_PVZUE_N_TH_DISC);
			enterParam.setFract(GLB_PVZUE_N_TH_FRACT);
			enterParam.setDopValue(4);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt21) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_N_LVL_MIN, GLB_PVZUE_N_LVL_MAX);
			enterParam.setValue(sParam.glb.getPvzueNoiseLvl());
			enterParam.setDisc(GLB_PVZUE_N_LVL_DISC);
			enterParam.setFract(GLB_PVZUE_N_LVL_FRACT);
			enterParam.setDopValue(5);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt22) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_TYPE_AC_MIN,
					GB_PVZUE_TYPE_AC_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueTypeAC());
			enterParam.list = fcPvzueTypeAC[0];
			enterParam.setDopValue(6);
			enterParam.com = GB_COM_GET_TIME_RERUN;
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

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		punkt_[num++] = punkt5;
		punkt_[num++] = punkt6;
		numPunkts_ = num;

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
	}

	snprintf_P(&vLCDbuf[20], 21, title);

	printMeasParam(0, MENU_MEAS_PARAM_DATE);
	printMeasParam(1, MENU_MEAS_PARAM_TIME);

	if (enterParam.isEnable()) {
		// вывод текущего пункта
		snprintf_P(&vLCDbuf[20 * lineParam_], 21, punkt_[cursorLine_ - 1],
				cursorLine_);

		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// копирование введеного значение на свое место
			sParam.txComBuf.setInt8(BIN_TO_BCD(enterParam.getValueEnter()),
					enterParam.getDopValue());
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
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
		if (punkt_[cursorLine_ - 1] == punkt1) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 99);
			enterParam.setValue(sParam.dataTime.getYear());
			enterParam.setDopValue(0);
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			enterParam.setEnable();
			enterParam.setValueRange(1, 12);
			enterParam.setValue(sParam.dataTime.getMonth());
			enterParam.setDopValue(1);
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			enterParam.setEnable();
			uint8_t max = sParam.dataTime.getNumDaysInMonth();
			enterParam.setValueRange(1, max);
			enterParam.setValue(sParam.dataTime.getDay());
			enterParam.setDopValue(2);
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 23);
			enterParam.setValue(sParam.dataTime.getHour());
			enterParam.setDopValue(3);
		} else if (punkt_[cursorLine_ - 1] == punkt5) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 59);
			enterParam.setValue(sParam.dataTime.getMinute());
			enterParam.setDopValue(4);
		} else if (punkt_[cursorLine_ - 1] == punkt6) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 59);
			enterParam.setValue(sParam.dataTime.getSecond());
			enterParam.setDopValue(5);
		}
		enterParam.com = GB_COM_SET_TIME;
		enterParam.setDisc(1);
		enterParam.setFract(1);
		// сохраним текущие значения даты и времени
		// байты расположены в порядке передачи в БСП
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getYear()), 0);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getMonth()), 1);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getDay()), 2);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getHour()), 3);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getMinute()), 4);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getSecond()), 5);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** Уровень меню. Тест 1.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlTest1() {
	static char title[] PROGMEM = "Меню\\Тест 1";
	static char punkt1[] PROGMEM = "Сигнал на выходе";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		numPunkts_ = 1;

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);

		// сигналы для тестов
		if (sParam.typeDevice == AVANT_R400M)
		{
			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_RZ);
			if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT)
				sParam.test.addSignalToList(GB_SIGNAL_CF);
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	if (enterParam.isEnable()) {
		// ввод нового значения параметра
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// текущий сигнал(ы)
			uint8_t sig = enterParam.listValue[enterParam.getValue()];
			uint8_t rz = 0;
			uint8_t cf = 0;

			sParam.test.getBytes(cf, rz, (eGB_TEST_SIGNAL) sig);
			// TODO РЗСК может быть два сигнала КЧ и РЗ одновременно
			// первый байт - номер группы (1 - кч, 2 - рз)
			// второй байт - номер сигнала (0 - выкл.)
			if (rz > 0)
			{
				sParam.txComBuf.setInt8(2, 0);
				sParam.txComBuf.setInt8(rz, 1);
			} else if (cf > 0) {
				sParam.txComBuf.setInt8(1, 0);
				sParam.txComBuf.setInt8(cf, 1);
			} else {
				// иначе запишим 0, чтобы ничего не отработалось
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 1);
			}

			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		uint8_t poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// выход из теста при несооответствии режима
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_1)
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterParam.setValue(0);
		enterParam.setEnable(MENU_ENTER_PARAM_LIST_2);
		enterParam.setValueRange(0, sParam.test.getNumSignals() - 1);
		enterParam.listValue = sParam.test.signalList;
		enterParam.list = fcTest1K400[0];
		enterParam.com = GB_COM_SET_REG_TEST_1;
		break;

	default:
		break;
	}
}

/** Уровень меню. Тест 2.
 * 	@param Нет
 * 	@return Нет
 */
void clMenu::lvlTest2() {
	static char title[] PROGMEM = "Меню\\Тест 2";
	static char punkt1[] PROGMEM = "Сигнал на входе";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		numPunkts_ = 1;

		// доплнительные команды
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	uint8_t poz = 100;
	poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
	snprintf_P(&vLCDbuf[poz], 11, fcTest1K400[sParam.test.getCurrentSignal()]);

	// выход из теста при несооответствии режима
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_2)
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
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

	eMENU_ENTER_PARAM status = enterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// вывод сообщения до тех пор, пока счетчик времени не обнулится
		// затем возврат в исходный пункт меню
		if (enterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = { " Изменить параметр  ",
					"  можно только в    ", "  режиме ВЫВЕДЕН    " };

			enterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else if (status == MENU_ENTER_PARAM_INT) {
		uint16_t val = enterParam.getValue();
		uint8_t num = enterParam.getValueNumSymbols();

		// если кол-во символов выходит за допустимые значения, закончим ввод
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterInt, val);
		}
	} else if (status == MENU_ENTER_PARAM_U_COR) {
		uint16_t val = enterParam.getValue();
		uint8_t num = enterParam.getValueNumSymbols();

		// если кол-во символов выходит за допустимые значения, закончим ввод
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterUcor, val / 10, val % 10);
		}
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = enterParam.getValue();
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				enterParam.list + STRING_LENGHT * val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = enterParam.listValue[enterParam.getValue()];
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				enterParam.list + STRING_LENGHT * val);
	} else
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		enterParam.setDisable();
		break;

	case KEY_UP:
		enterParam.incValue();
		break;
	case KEY_DOWN:
		enterParam.decValue();
		break;

	case KEY_ENTER:
		enterParam.setEnterValueReady();
		break;
	default:
		break;
	}

	key_ = KEY_NO;
	return enterParam.getStatus();
}

/** Ввод пароля
 * 	@param нет
 * 	@return True - по окончанию
 */
eMENU_ENTER_PARAM clMenu::enterPassword() {
	eMENU_ENTER_PARAM status = enterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// вывод сообщения до тех пор, пока счетчик времени не обнулится
		// затем возврат в исходный пункт меню
		if (enterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					"       Введен       ",//
					"    неправильный    ",		//
					"       пароль       " };

			enterParam.cnt_++;
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

		uint16_t val = enterParam.getValue();

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
		enterParam.setDisable();
		break;

	case KEY_ENTER:
		if (status == MENU_ENTER_PASSWORD)
			enterParam.setEnterValueReady(MENU_ENTER_PASSWORD_READY);
		else if (status == MENU_ENTER_PASSWORD_NEW)
			enterParam.setEnterValueReady(MENU_ENTER_PASSWORD_N_READY);
		else
			enterParam.setDisable();
		break;

	case KEY_UP:
		enterParam.setDisc(1000);
		enterParam.incValue();
		break;
	case KEY_RIGHT:
		enterParam.setDisc(100);
		enterParam.incValue();
		break;
	case KEY_DOWN:
		enterParam.setDisc(10);
		enterParam.incValue();
		break;
	case KEY_LEFT:
		enterParam.setDisc(1);
		enterParam.incValue();
		break;

	default:
		break;
	}
	key_ = KEY_NO;

	return enterParam.getStatus();
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
		snprintf_P(&vLCDbuf[20 * line], 21, punkt_[cntPunkts], cntPunkts + 1);

		if (++cntPunkts >= numPunkts_)
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

/**	Возвращает кол-во ошибок в коде, т.е. кол-во установленных битов.
 * 	@param Код ошибки
 * 	@return Кол-во ошибок
 */
uint8_t clMenu::getNumError(uint16_t val) {
	uint_fast8_t tmp = 0;

	while ((val & 0x0001) == 0) {
		tmp++;
		val >>= 1;
	}

	return tmp;
}
