/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"

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
};

enum eMNU_TYPE_DEVICE
{
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_K400,
	AVANT_K400_OPTIC
};

enum eMNU_TYPE_LINE
{
	UM,
};


class clMenu
{
public:
	clMenu(stMNUparam *param);

	// тело
	void main();

	// возвращает кол-во линий отведенных для параметров
	uint8_t getLineParam() { return lineParam; }

	// установка типа аппарата
	void setTypeDevice(eMNU_TYPE_DEVICE device)
	{
		typeDevice = device;
	}

	// возвращает текущий тип аппарата
	eMNU_TYPE_DEVICE getTypeDevice()
	{
		return typeDevice;
	}

	// возвращает имеющуюся команду на исполнение
	uint8_t txCommand();

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

	stMNUparam * const sParam;

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
