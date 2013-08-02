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

/// кол-во строк, отображаемых на экране

/// время до переинициализации дисплея (* время цикла ЖКИ)
#define TIME_TO_REINIT_LCD 50

/// максимальное кол-во пунктов в меню
#define MAX_NUM_PUNKTS 12

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
	/// По умолчанию будет сделан выбор исходя из имеющейся информации в
	/// настройках отдельных устройств
	bool setTypeDevice(eGB_TYPE_DEVICE device = AVANT_NO);

	/// Возвращает имеющуюся команду на исполнение
	eGB_COM getTxCommand();

	/// Возвращает текущее состояние связи с БСП
	bool isConnectionBsp() const { return connectionBsp_; }

	/// Установка флага наличия связи с БСП, True - есть
	void setConnectionBsp(bool f) { connectionBsp_ = f; }

	// структура параметров
	stGBparam sParam;

private:
	// код кнопки
	eKEY key_;

	// флаг мигания надписи
	bool blink_;

	// флаг текущего сосотояния связи с БСП, True - есть
	bool connectionBsp_;

	// true - необходимо вывести на экран курсор
	bool cursorEnable_;

	// текущее положение курсора (номер строки)
	uint8_t cursorLine_;

	// кол-во отображаемых параметров
	uint8_t lineParam_;

	// если true - необходимо создать уровень меню
	bool lvlCreate_;

	// измеряемые параметры
	eMENU_MEAS_PARAM measParam[6];

	// кол-во пунктов в текущем меню
	uint8_t numPunkts_;

	// пункты в текущем меню
	PGM_P punkt_[MAX_NUM_PUNKTS];

	// очистка текстового буфера
	void clearTextBuf();

	// вывод на экран измеряемого параметра
	void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

	// вывод на экран текущего состояния устройств
	void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

	// вывод в пунтке меню "Режим" текущего режима устройств
	void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

	// Уровни меню
	void lvlError();
	void lvlStart();
	void lvlFirst();
	void lvlJournal();
	void lvlJournalEvent();
	void lvlJournalDef();
	void lvlJournalPrm();
	void lvlJournalPrd();
	void lvlControl();
	void lvlSetup();
	void lvlRegime();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupDT();
	void lvlInfo();

	// перемещение курсора вверх
	void cursorLineUp()
	{
		cursorLine_ = (cursorLine_ > 1) ? cursorLine_ - 1 : numPunkts_;
	};

	// пермещение курсора вниз
	void cursorLineDown()
	{
		cursorLine_ = (cursorLine_ < numPunkts_) ? cursorLine_ + 1 : 1;
	}

	// вывод на экран текущих пунктов меню и курсора
	void printPunkts();

	// возвращает текущий номер неисправности/предупреждения
	uint8_t getNumError(uint16_t val);

	// текущий уровень меню
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
