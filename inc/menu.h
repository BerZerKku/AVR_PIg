/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "glbDefine.h"

/// время до переинициализации дисплея (* время цикла ЖКИ)
#define TIME_TO_REINIT_LCD 50

class clMenu
{
public:
	clMenu();

	/// Тело
	void main();

	/// Установка типа аппарата
	void setTypeDevice(eGB_TYPE_DEVICE device) { sParam.typeDevice = device; }

	/// Возвращает имеющуюся команду на исполнение
	uint8_t txCommand();

	/// Возвращает текущее состояние связи с БСП
	bool isConnectionBsp() const { return connectionBsp; }

	/// Установка флага наличия связи с БСП, True - есть
	void setConnectionBsp(bool f) { connectionBsp = f; }

	/// Возвращает адрес структуры параметров меню
	stGBparam* getParamStruct() { return &sParam; }

private:
	// код кнопки
	eKEY key;

	// флаг текущего сосотояния связи с БСП, True - есть
	bool connectionBsp;

	// состояние курсора
	bool cursorEnable;

	// положение курсора
	uint8_t cursorLine;

	// кол-во отображаемых параметров
	uint8_t lineParam;

	// создание уровня меню
	bool lvlCreate;

	// структура параметров
	stGBparam sParam;

	// возвращает текущий номер неисправности/предупреждения
	uint8_t getNumError(uint16_t val);

	// очистка текстового буфера
	void clearTextBuf();

	// вывод на экран измеряемых параметров
	void printMeasParam(eGB_MEAS_PARAM par1 = GB_MEAS_PARAM_NO,
						eGB_MEAS_PARAM par2 = GB_MEAS_PARAM_NO);

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
