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

/// время одно цикла работы с меню, мс
#define MENU_TIME_CYLCE 200

/// время до первой инициализации дисплея, мс
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)
/// время до переинициализации дисплея, мс
#define TIME_TO_REINIT_LCD (10000 / MENU_TIME_CYLCE)

/// максимальное кол-во пунктов в меню
#define MAX_NUM_PUNKTS 15

/// максимальное кол-во отображаемых на экране параметров
#define MAX_NUM_MEAS_PARAM 6

/// время вывода измеряемого параметра на экран, мс
#define TIME_MEAS_PARAM (3000 / MENU_TIME_CYLCE)

/// время вывода доп.сообщения на экран, мс
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)

/// время вывода текста на экран, мс (например авария/код аварии)
#define TIME_TEXT (1000 / MENU_TIME_CYLCE)

/// Измеряемые параметры
enum eMENU_MEAS_PARAM
{
	MENU_MEAS_PARAM_NO,
	MENU_MEAS_PARAM_DATE,
	MENU_MEAS_PARAM_TIME,
	MENU_MEAS_PARAM_UZ,
	MENU_MEAS_PARAM_UZ1,
	MENU_MEAS_PARAM_UZ2,
	MENU_MEAS_PARAM_UC,
	MENU_MEAS_PARAM_UC1,
	MENU_MEAS_PARAM_UC2,
	MENU_MEAS_PARAM_UN,
	MENU_MEAS_PARAM_UN1,
	MENU_MEAS_PARAM_UN2,
	MENU_MEAS_PARAM_UOUT,
	MENU_MEAS_PARAM_IOUT,
	MENU_MEAS_PARAM_ROUT
};

/// Режим работы функции ввода параметров
enum eMENU_ENTER_PARAM
{
	MENU_ENTER_PARAM_NO,		// отмена изменения параметра
	MENU_ENTER_PARAM_INT,		// изменение параметра, целое значение
	MENU_ENTER_PARAM_LIST,		// изменение параметра, выбор из списка
	MENU_ENTER_PARAM_LIST_2,	// изменение параметра, выбор из списка значений
	MENU_ENTER_PARAM_U_COR,		// изменение параметра, коррекция напряжения
	MENU_ENTER_PASSWORD,		// ввод пароля
	MENU_ENTER_PASSWORD_NEW,	// ввод нового пароля
	MENU_ENTER_PASSWORD_READY,	// введен верный пароль
	MENU_ENTER_PASSWORD_N_READY,// новый пароль введен корректно
	MENU_ENTER_PARAM_READY,		// необходимо изменить параметр
	MENU_ENTER_PARAM_MESSAGE,	// вывод сообщения на экран при вводе параметра
};

// структура параметров для ввода значений
class TEnterParam
{
public:
	TEnterParam()
	{
		setDisable();
	}


	/**	Проверка текущего статуса работы с параметром.
	 * 	@param Нет
	 * 	@return True - если идет ввод нового значения, иначе False.
	 */
	bool isEnable()
	{
		// проверка текущего статуса на достоверное значение
		if ( (status_ <  MENU_ENTER_PARAM_NO) ||
				(status_ > MENU_ENTER_PARAM_MESSAGE) )
			status_ = MENU_ENTER_PARAM_NO;
		return (status_ != MENU_ENTER_PARAM_NO);
	}

	// начало работы, передается способ ввода переменной
	// если ввод из списка, автоматически дискретность и делитель равны 1
	// для пароля автоматически выставляются диапазон ввода и начальное значение
	void setEnable(eMENU_ENTER_PARAM s=MENU_ENTER_PARAM_INT)
	{
		if ((s>=MENU_ENTER_PARAM_INT) && (s<=MENU_ENTER_PASSWORD_NEW))
		{
			if ( (s == MENU_ENTER_PARAM_LIST) ||
					(s == MENU_ENTER_PARAM_LIST_2) )
			{
				disc_ = 1;
				fract_ = 1;
			}
			else if ( (s == MENU_ENTER_PASSWORD) ||
					(s == MENU_ENTER_PASSWORD_NEW) )
			{
				val_ = 0;
				min_ = 0;
				max_ = 9999;
			}
			status_ = s;
		}
	}

	/**	Отключение изменения параметра.
	 * 	@param Нет
	 * 	@return Нет
	 */
	void setDisable()
	{
		status_ = MENU_ENTER_PARAM_NO;
		cnt_ = TIME_MESSAGE;
		com = GB_COM_NO;
	}

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
	uint8_t getValueEnter() const
	{
		return ((val_ / disc_) * disc_) / fract_;
	}

	// увеличение текущего значения
	uint16_t incValue()
	{

		if ( (status_ == MENU_ENTER_PARAM_INT) ||
				(status_ == MENU_ENTER_PARAM_U_COR) )
		{
			// увеличение значения
			val_ = (val_ <= (max_ - disc_)) ? val_ + disc_ : min_;
		}
		else if ( (status_ == MENU_ENTER_PARAM_LIST) ||
				(status_ == MENU_ENTER_PARAM_LIST_2) )
		{
			// в списке порядок обратный (уменьшение индекса массива)
			val_ = (val_ > min_) ? val_ - 1 : max_;
		}
		else if ( (status_ == MENU_ENTER_PASSWORD) ||
				(status_ == MENU_ENTER_PASSWORD_NEW) )
		{
			uint16_t t = 0;

			// находится разряд заданный дискретностью
			// например для числа 1234 и дискрету 100, получается 2
			t = val_ % (disc_ * 10);
			t = t / disc_;

			if (t == 9)
				val_ -= 9 * disc_;
			else
				val_ += disc_;
		}
		return val_;
	}
	// уменьшение текущего значения
	uint16_t decValue()
	{
		eMENU_ENTER_PARAM s = status_;
		if ( (status_ == MENU_ENTER_PARAM_INT) ||
				(status_ == MENU_ENTER_PARAM_U_COR) )
		{
			// уменьшение значние
			val_ = (val_ >= (min_ + disc_)) ? val_ - disc_ : max_;
		}
		else if ( (s == MENU_ENTER_PARAM_LIST) ||
				(s == MENU_ENTER_PARAM_LIST_2) )
		{
			// в списке порядок обратный (увеличие индекса массива)
			val_ = (val_ < max_) ? val_ + 1 : min_;
		}
		else if ( (s == MENU_ENTER_PASSWORD) ||
				(s == MENU_ENTER_PASSWORD_NEW) )
		{

		}
		return val_;
	}

	// запись/считывание дополнительного байта
	void setDopValue(uint16_t byte) { dopValue_ = byte; }
	uint16_t getDopValue() const { return dopValue_; }

	// запись/считывание дискретности
	void setDisc(uint16_t disc) { disc_ = disc; }
	uint16_t getDisc() const { return disc_; }

	// запись/считывание делителя
	void setFract(uint8_t fract) { fract_ = fract; }
	uint8_t getFract() const { return fract_; }

	// вывод сообщения на экран
	// по умолчанию работает для функции ввода параметра
	void printMessage()
	{
		status_ = MENU_ENTER_PARAM_MESSAGE;
		cnt_ = 0;
	}

	// считывание текущиего статуса
	eMENU_ENTER_PARAM getStatus() const { return status_; }

	// установка флага окончания ввода параметра
	void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY)
	{
		status_ = status;
	}

	// указатель на первый элемент списка
	PGM_P list;

	// указатель на массив значений
	uint8_t *listValue;

	// команда на передачу
	eGB_COM com;

	// счетчик времени
	uint8_t cnt_;

private:
	// текущее значение
	uint16_t val_;

	// максимальное значение
	uint16_t max_;

	// минимальное значение
	uint16_t min_;

	// кол-во символов
	uint8_t numSymbols_;

	// байт с дополнительой информацией
	uint16_t dopValue_;

	// дискретность
	uint16_t disc_;

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

	// флаг мигания надписей
	bool blink_;

	// флаг смены измеряемых параметров
	bool blinkMeasParam_;

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
	eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM*2];

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

	// вывод сообщения на экран
	void printMessage() { delay_ = 0; }

	// возвращает true - в случае необходимости вывода сообщения
	bool isMessage() const { return (delay_ < TIME_MESSAGE); }

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
	void lvlInfo();
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
	void lvlTest1();
	void lvlTest2();

	// ввод параметра
	eMENU_ENTER_PARAM enterValue();

	eMENU_ENTER_PARAM enterPassword();

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

	// текущая функция ввода
	eMENU_ENTER_PARAM (clMenu:: *enterFunc) ();

	// текущий уровень меню
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
