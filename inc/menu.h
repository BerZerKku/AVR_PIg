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

/// Измеряемые параметры
enum eMENU_MEAS_PARAM
{
	MENU_MEAS_PARAM_NO,
	MENU_MEAS_PARAM_DATE,
	MENU_MEAS_PARAM_TIME,
	MENU_MEAS_PARAM_UZ,
	MENU_MEAS_PARAM_UC,
	MENU_MEAS_PARAM_UN,
	MENU_MEAS_PARAM_UOUT,
	MENU_MEAS_PARAM_IOUT,
	MENU_MEAS_PARAM_ROUT
};

class clMenu
{
public:
	clMenu();

	/// Тело
	void main();

	/// Установка типа аппарата
	bool setTypeDevice(eGB_TYPE_DEVICE device);

	/// Возвращает имеющуюся команду на исполнение
	uint8_t txCommand();

	/// Возвращает текущее состояние связи с БСП
	bool isConnectionBsp() const { return connectionBsp_; }

	/// Установка флага наличия связи с БСП, True - есть
	void setConnectionBsp(bool f) { connectionBsp_ = f; }

	// структура параметров
	stGBparam sParam;

private:
	// код кнопки
	eKEY key_;

	// флаг текущего сосотояния связи с БСП, True - есть
	bool connectionBsp_;

	// состояние курсора
	bool cursorEnable_;

	// положение курсора
	uint8_t cursorLine_;

	// кол-во отображаемых параметров
	uint8_t lineParam_;

	// создание уровня меню
	bool lvlCreate_;

	// измеряемые параметры
	eMENU_MEAS_PARAM measParam[6];

	// возвращает текущий номер неисправности/предупреждения
	uint8_t getNumError(uint16_t val);

	// очистка текстового буфера
	void clearTextBuf();

	// вывод на экран измеряемого параметра
	void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

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
