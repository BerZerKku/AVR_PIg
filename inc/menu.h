/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "protocolS.h"

/// время до переинициализации дисплея (* время цикла ЖКИ)
#define TIME_TO_REINIT_LCD 50

/// Структура параметров БСП
struct stMNUparam
{
	// дата/время
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

	// измеряемые параметры
	int8_t voltDef;
	int8_t voltCF;
	int8_t voltNoise;
	uint8_t voltOutInt;
	uint8_t voltOutFract;
	uint16_t curOut;
	uint16_t resistOut;

	// состояние общее
	uint16_t glb_avar;
	uint16_t glb_warn;

	// состояние защиты
	uint16_t def_avar;
	uint16_t def_warn;
	uint8_t def_regime;
	uint8_t def_sost;
	uint8_t def_dop;

	// состояние приемника
	uint16_t prm_avar;
	uint16_t prm_warn;
	uint8_t prm_regime;
	uint8_t prm_sost;
	uint8_t prm_dop;

	// состояние передатчика
	uint16_t prd_avar;
	uint16_t prd_warn;
	uint8_t prd_regime;
	uint8_t prd_sost;
	uint8_t prd_dop;


};

enum eMNU_TYPE_DEVICE
{
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_RZSK_OPTIC,
	AVANT_K400,
	AVANT_K400_OPTIC
};

enum eMNU_TYPE_LINE
{
	UM,
	OPTIC
};


class clMenu
{
public:
	clMenu(clProtocolS *protocol);

	// тело
	void main();

	// возвращает кол-во линий отведенных для параметров
	uint8_t getLineParam() const
	{
		return lineParam;
	}

	// установка типа аппарата
	void setTypeDevice(eMNU_TYPE_DEVICE device)
	{
		typeDevice = device;
	}

	// возвращает текущий тип аппарата
	eMNU_TYPE_DEVICE getTypeDevice() const
	{
		return typeDevice;
	}

	// возвращает имеющуюся команду на исполнение
	uint8_t txCommand();

	// структура параметров аппарата
	stMNUparam sParam;

	// протокол работы с БСП
	const clProtocolS *pProtocol;

private:
	// код кнопки
	eKEY key;

	// кол-во отображаемых параметров
	uint8_t lineParam;

	// создание уровня меню
	bool lvlCreate;

	// положение курсора
	uint8_t cursorLine;

	// состояние курсора
	bool cursorEnable;

	// тип аппарата
	eMNU_TYPE_DEVICE typeDevice;

	// команда на исполнение
	uint8_t com;

	// возвращает текущий номер неисправности/предупреждения
	uint8_t getNumError(uint16_t val);

	// очистка текстового буфера
	void clearTextBuf();
	// Уровни меню
	void lvlStart();
	void lvlFirst();
	void lvlJournal();
	void lvlJournalEvent();
	void lvlJournalDef();
	void lvlJournalPrm();
	void lvlJournalPrd();
	void lvlControl();
	void lvlSetup();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupDT();
	void lvlInfo();

	// текущий уровень меню
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
