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
#define MAX_NUM_PUNKTS 15

/// время вывода доп.сообщения на экран
#define TIME_MESSAGE 10

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

/// Режим работы функции ввода параметров
enum eMENU_ENTER_PARAM
{
	MENU_ENTER_PARAM_NO,		// отмена изменения параметра
	MENU_ENTER_PARAM_WORK,		// изменение параметра
	MENU_ENTER_PARAM_READY,		// необходимо изменить параметр
	MENU_ENTER_PARAM_MESSAGE	// вывод сообщения на экран
};

// структура параметров для ввода значений
class TEnterParam
{
public:
	TEnterParam()
	{
		setDisable();
	}

	bool isEnable()
	{
		// проверка текущего статуса на достоверное значение
		if ( (status_ <  MENU_ENTER_PARAM_NO) ||
				(status_ > MENU_ENTER_PARAM_MESSAGE) )
			status_ = MENU_ENTER_PARAM_NO;
		return (status_ != MENU_ENTER_PARAM_NO);
	}
	void setEnable() { status_ = MENU_ENTER_PARAM_WORK; }
	void setDisable() { status_ = MENU_ENTER_PARAM_NO; }

	// диапазон значений
	void setValueRange(uint16_t min, uint16_t max)
	{
		uint8_t num = 0;
		max_ = max;
		min_ = min;
		// нахождение максимального кол-ва символов при вводе
		while (max > 0)
		{
			num++;
			max /= 10;
		}
		numSymbols_ = num;
	}
	uint16_t getValueMin() const { return min_; }
	uint16_t getValueMax() const { return max_; }

	// кол-во вводимых символов
	uint16_t getValueNumSymbols() const { return numSymbols_; }

	// установка текущего значения, диапазон значений должен быть задан до !
	void setValue(uint16_t val)
	{
		if ( (val < min_) || (val > max_) )
			val = min_;
		val_ = val;
	}

	// возвращает текущее значение
	uint16_t getValue() const { return val_; }

	// возвращает введеное значение с учетом дискретности и делителя
	uint8_t getValueEnter()
	{
		return ((val_ / disc_) * disc_) / fract_;
	}

	// увеличение/уменьшение текущего значения
	uint16_t incValue()
	{
		if (val_ <= (max_ - disc_))
			val_ += disc_;
		else
			val_ = min_;
		return val_;
	}
	uint16_t decValue()
	{
		if (val_ >= (min_ + disc_))
			val_ -= disc_;
		else
			val_ = max_;
		return val_;
	}

	// запись/считывание дополнительного байта
	void setDopByte(uint8_t byte) { dopByte_ = byte; }
	uint8_t getDopByte() const { return dopByte_; }

	// запись/считывание дискретности
	void setDisc(uint8_t disc) { disc_ = disc; }
	uint8_t getDisc() const { return disc_; }

	// запись/считывание делителя
	void setFract(uint8_t fract) { fract_ = fract; }
	uint8_t getFract() const { return fract_; }

	// вывод сообщения на экран
	void printMessage() { status_ = MENU_ENTER_PARAM_MESSAGE; cnt_ = 0; }

	// считывание текущиего статуса
	eMENU_ENTER_PARAM getStatus() const { return status_; }
	void setEnterValueReady() { status_ = MENU_ENTER_PARAM_READY; }

	// команда на передачу
	eGB_COM com;

	// счетчик времени
	uint8_t cnt_;
private:
	// true - идет ввод значения
	bool enable_;

	// текущее значение
	uint16_t val_;

	// максимальное значение
	uint16_t max_;

	// минимальное значение
	uint16_t min_;

	// кол-во символов
	uint8_t numSymbols_;

	// байт с дополнительой информацией
	uint8_t dopByte_;

	// дискретность
	uint8_t disc_;

	// делитель
	uint8_t fract_;



	// текущий статус устройства
	eMENU_ENTER_PARAM status_;
};

// класс меню
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

	// текущий подпункт (например номер текущей команды параметра, начиная с 1)
	uint8_t curCom_;

	// если true - необходимо создать уровень меню
	bool lvlCreate_;

	// время вывода доп.сообщения на экран (например сообщения об ошибке)
	uint8_t delay_;

	// измеряемые параметры
	eMENU_MEAS_PARAM measParam[6];

	// кол-во пунктов в текущем меню
	uint8_t numPunkts_;

	// пункты в текущем меню
	PGM_P punkt_[MAX_NUM_PUNKTS];

	// параметры для ввода новых значений
	TEnterParam enterParam;

	// очистка текстового буфера
	void clearTextBuf();

	// очистка строки
	void clearLine(uint8_t line);

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

	// ввод целого значения
	eMENU_ENTER_PARAM enterValue();

	// заглушка для ввода значений
	eMENU_ENTER_PARAM enterNo() { return MENU_ENTER_PARAM_NO; }

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

	// текущая функция ввода значения
	eMENU_ENTER_PARAM (clMenu:: *enterFunc_) ();

	// текущий уровень меню
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
