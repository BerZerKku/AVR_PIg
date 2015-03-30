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
#include "paramBsp.h"
#include "LocalParams.h"

/// время одно цикла работы с меню, мс
#define MENU_TIME_CYLCE 200

/// время до первой инициализации дисплея, мс
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)
/// время до переинициализации дисплея, мс
#define TIME_TO_REINIT_LCD (10000 / MENU_TIME_CYLCE)

/// максимальное кол-во отображаемых на экране параметров
#define MAX_NUM_MEAS_PARAM 6

/// время вывода измеряемого параметра на экран, мс
#define TIME_MEAS_PARAM (3000 / MENU_TIME_CYLCE)

/// время вывода доп.сообщения на экран, мс
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)

/// время вывода текста на экран, мс (например авария/код аварии)
#define TIME_TEXT (1000 / MENU_TIME_CYLCE)

/// время до выхода из Тест 1 / Тест 2 и т.д. если режим != данному тесту, мс
#define TIME_TEST_EXIT (1000 / MENU_TIME_CYLCE)

/// Измеряемые параметры
enum eMENU_MEAS_PARAM {
	MENU_MEAS_PARAM_NO,			///< нет измеряемого параметра
	MENU_MEAS_PARAM_DATE,		///< дата
	MENU_MEAS_PARAM_TIME,		///< время
	MENU_MEAS_PARAM_UZ,			///< запас по зщаите
	MENU_MEAS_PARAM_UZ1,		///< запас по защите 1 (для 3-х концевой)
	MENU_MEAS_PARAM_UZ2,		///< запас по защите 2 (для 3-х концевой)
	MENU_MEAS_PARAM_UC,			///< запас по КЧ
	MENU_MEAS_PARAM_UC1,		///< запас по КЧ 1 (для 3-х концевой)
	MENU_MEAS_PARAM_UC2,		///< запас по КЧ 2 (для 3-х концевой)
	MENU_MEAS_PARAM_UN,			///< уровень шумов
	MENU_MEAS_PARAM_UN1,		///< уровень шумов 1 (для 3-х концевой)
	MENU_MEAS_PARAM_UN2,		///< уровень шумов 2 (для 3-х концевой)
	MENU_MEAS_PARAM_UOUT,		///< напряжени выхода
	MENU_MEAS_PARAM_IOUT,		///< ток выхода
	MENU_MEAS_PARAM_ROUT,		///< сопротивление линии
	MENU_MEAS_PARAM_SD			///< просечки в сигнале
};

/// Режим работы функции ввода параметров
enum eMENU_ENTER_PARAM {
	MENU_ENTER_PARAM_NO,		///< отмена изменения параметра
	MENU_ENTER_PARAM_INT,		///< изменение параметра, целое значение
	MENU_ENTER_PARAM_LIST,		///< изменение параметра, выбор из списка
	MENU_ENTER_PARAM_LIST_2,	///< изменение параметра, выбор из списка значений
	MENU_ENTER_PARAM_U_COR,		///< изменение параметра, коррекция напряжения
	MENU_ENTER_PASSWORD,		///< ввод пароля
	MENU_ENTER_PASSWORD_NEW,	///< ввод нового пароля
	MENU_ENTER_PASSWORD_READY,	///< введен верный пароль
	MENU_ENTER_PASSWORD_N_READY,///< новый пароль введен корректно
	MENU_ENTER_PARAM_READY,		///< необходимо изменить параметр
	MENU_ENTER_PARAM_MESSAGE,	///< вывод сообщения на экран при вводе параметра
};

// структура параметров для ввода значений
class TEnterParam {
public:
	TEnterParam() {
		setDisable();
	}

	/**	Проверка текущего статуса работы с параметром.
	 * 	@param Нет
	 * 	@return True - если идет ввод нового значения, иначе False.
	 */
	bool isEnable() {
		// проверка текущего статуса на достоверное значение
		if ((status_ < MENU_ENTER_PARAM_NO)
				|| (status_ > MENU_ENTER_PARAM_MESSAGE))
			status_ = MENU_ENTER_PARAM_NO;
		return (status_ != MENU_ENTER_PARAM_NO);
	}

	// начало работы, передается способ ввода переменной
	// если ввод из списка, автоматически дискретность и делитель равны 1
	// для пароля автоматически выставляются диапазон ввода и начальное значение
	void setEnable(eMENU_ENTER_PARAM s = MENU_ENTER_PARAM_INT) {
		if ((s >= MENU_ENTER_PARAM_INT) && (s <= MENU_ENTER_PASSWORD_NEW)) {
			if ((s == MENU_ENTER_PARAM_LIST)
					|| (s == MENU_ENTER_PARAM_LIST_2)) {
				disc_ = 1;
				fract_ = 1;
			} else if ((s == MENU_ENTER_PASSWORD)
					|| (s == MENU_ENTER_PASSWORD_NEW)) {
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
	void setDisable() {
		status_ = MENU_ENTER_PARAM_NO;
		cnt_ = TIME_MESSAGE;
		com = GB_COM_NO;
		disc_ = 1;
	}

	// диапазон значений
	void setValueRange(uint16_t min, uint16_t max) {
		uint8_t num = 0;
		max_ = max;
		min_ = min;
		// нахождение максимального кол-ва символов при вводе
		while (max > 0) {
			num++;
			max /= 10;
		}
		numSymbols_ = num;
	}
	uint16_t getValueMin() const {
		return min_;
	}
	uint16_t getValueMax() const {
		return max_;
	}

	// кол-во вводимых символов
	uint16_t getValueNumSymbols() const {
		return numSymbols_;
	}

	// установка текущего значения, диапазон значений должен быть задан до !
	void setValue(uint16_t val) {
		if ((val < min_) || (val > max_))
			val = min_;
		val_ = val;
	}

	// возвращает текущее значение
	uint16_t getValue() const {
		return val_;
	}

	// возвращает введеное значение с учетом дискретности и делителя
	uint8_t getValueEnter() const {
		return ((val_ / disc_) * disc_) / fract_;
	}

	/** Увеличение текущего значения.
	 * 	@param velocity Скорость изменения значения (для ввода целых значений
	 * 	и коррекции напряжения).
	 * 	@argval 0 Увеличение на шаг заданный дискретностью.
	 * 	@argval 1 Увеличение на шаг в 10 раз больше заданной дискретности.
	 * 	@argval 2 Увеличение на шаг в 50 раз больше заданной дискретности.
	 */
	uint16_t incValue(uint8_t velocity = 0) {
		eMENU_ENTER_PARAM s = status_;
		if ((s == MENU_ENTER_PARAM_INT) || (s == MENU_ENTER_PARAM_U_COR)) {
			// увеличение значения
//			val_ = (val_ <= (max_ - disc_)) ? val_ + disc_ : min_;
			uint16_t disc = disc_;
			if (velocity >= 1) {
				if ((max_ / disc) >= 10) {
					disc *= 10;
				}
			}
			if (velocity >= 2) {
				if ((max_ / disc) >= 10) {
					disc *= 5;
				}
			}
			val_ = (val_ <= (max_ - disc)) ? val_ + disc : min_;
		} else if ((s == MENU_ENTER_PARAM_LIST) ||
				   (s == MENU_ENTER_PARAM_LIST_2)) {
			// в списке порядок обратный (уменьшение индекса массива)
			val_ = (val_ > min_) ? val_ - 1 : max_;
		} else if ((s == MENU_ENTER_PASSWORD) ||
				   (s == MENU_ENTER_PASSWORD_NEW)) {
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
	uint16_t decValue(uint8_t velocity=0) {
		eMENU_ENTER_PARAM s = status_;
		if ((s == MENU_ENTER_PARAM_INT)
				|| (s == MENU_ENTER_PARAM_U_COR)) {
			// уменьшение значние
			uint16_t disc = disc_;
			if (velocity >= 1) {
				if ((max_ / disc) >= 10) {
					disc *= 10;
				}
			}
			if (velocity >= 2) {
				if ((max_ / disc) >= 10) {
					disc *= 5;
				}
			}
			val_ = (val_ >= (min_ + disc)) ? val_ - disc : max_;
		} else if ((s == MENU_ENTER_PARAM_LIST)
				|| (s == MENU_ENTER_PARAM_LIST_2)) {
			// в списке порядок обратный (увеличие индекса массива)
			val_ = (val_ < max_) ? val_ + 1 : min_;
		} else if ((s == MENU_ENTER_PASSWORD)
				|| (s == MENU_ENTER_PASSWORD_NEW)) {

		}
		return val_;
	}

	// запись/считывание дополнительного байта
	void setDopValue(uint16_t byte) {
		dopValue_ = byte;
	}
	uint16_t getDopValue() const {
		return dopValue_;
	}

	// запись/считывание дискретности
	void setDisc(uint16_t disc) {
		disc_ = disc;
	}
	uint16_t getDisc() const {
		return disc_;
	}

	// запись/считывание делителя
	void setFract(uint8_t fract) {
		fract_ = fract;
	}
	uint8_t getFract() const {
		return fract_;
	}

	// вывод сообщения на экран
	// по умолчанию работает для функции ввода параметра
	void printMessage() {
		status_ = MENU_ENTER_PARAM_MESSAGE;
		cnt_ = 0;
	}

	// считывание текущиего статуса
	eMENU_ENTER_PARAM getStatus() const {
		return status_;
	}

	// установка флага окончания ввода параметра
	void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY) {
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

/// Структура пункта меню
class TMenuPunkt {
	/// максимальное кол-во пунктов в меню
	static const uint8_t  MAX_NUM_PUNKTS = 20;
public:
	/**	Конструктор.
	 */
	TMenuPunkt() {
		clear();

		for (uint_fast8_t i = 0; i < MAX_NUM_PUNKTS; i++)
			com_[i] = GB_COM_NO;
	}

	/**	Добавление пункта меню, через указатель.
	 * 	@param name Имя пункта.
	 * 	@param com Команда для запроса из БСП. По умолчанию - нет.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, переполнен массив.
	 */
	bool add(PGM_P name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			name_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/**	Добавление пункта меню, через номер элемента массива.
	 *
	 * 	@param name	Номер пункта (имя).
	 * 	@param com Команда для запроса из БСП. По умолчанию - нет.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, переполнен массив.
	 */
	bool add(uint8_t name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			number_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/** Очистка текущего списка пунктов меню.
	 */
	void clear() {
		cnt_ = 0;
	}

	/** Замена указанного номера пункта меню.
	 * 	@param name Имя пунка.
	 * 	@param com Команда для запроса из БСП.
	 * 	@param num Номер пункта.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, не верный номер.
	 */
	bool change(PGM_P name, eGB_COM com, uint8_t num) {
		bool stat = false;
		if (num < cnt_) {
			name_[num] = name;
			com_[num] = com;
			stat = true;
		}
		return stat;
	}

	/** Возвращает название укзанного пункта меню.
	 *	@param num	Номер пункта.
	 * 	@return Название пункта.
	 */
	PGM_P getName(uint8_t num) {
//		if (num >= cnt_)
//			num = 0;
//		return name_[num];
		return ((num < cnt_) ? name_[num] : 0);
	}

	/**	Возвращает номер эелемента массива с именем указанного пункта меню.
	 *	@param num	Номер пункта.
	 * 	@return Название пункта.
	 */
	uint8_t getNumber(uint8_t num) {
		return ((num < cnt_) ? number_[num] : 0);
	}

	/** Возвращает команду указанного пункта меню.
	 * 	@param num Номер пункта.
	 * 	@return Команда для запроса из БСП.
	 */
	eGB_COM getCom(uint8_t num) {
		if (num >= cnt_)
			num = 0;
		return com_[num];
	}

	/** Возвращает текущее кол-во пункто меню.
	 * 	@return Текущее кол-во пунктов меню.
	 * 	@retval 0 - Пусто.
	 */
	uint8_t getMaxNumPunkts() {
		return cnt_;
	}

private:
	/// текущее кол-во пунктов
	uint8_t cnt_;
	/// указатель на имя пункта
	PGM_P name_[MAX_NUM_PUNKTS];
	/// номер пункта, используется с массивами
	uint8_t number_[MAX_NUM_PUNKTS];
	/// команда для запроса из БСП, необходимая для данного пункта меню
	eGB_COM com_[MAX_NUM_PUNKTS];
};

// класс меню
class clMenu {
public:

	/**	Конструктор.
	 * 	Устнанавливается по умолчанию неопределеныый тип аппарата.
	 * 	Заполняются необходимые для работы массивы.
	 * 	Производится переход на стартовый уровень меню.
	 */
	clMenu();

	/**	Работа с меню.
	 */
	void main();

	/** Установка типа аппарата и настройка меню с его учетом.
	 * 	По умолчанию будет сделан выбор исходя из меющейся информации в
	 * 	настройках отдельных устройств.
	 * 	@param device Тип устройства
	 * 	@return False в случае ошибки, иначе True
	 */
	bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

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
	eGB_COM getTxCommand();

	/** Возвращает текущее состояние связи с БСП.
	 * 	@retval True - связь с БСП есть.
	 * 	@retval False - связи с БСП нет.
	 */
	bool isConnectionBsp() const {
		return connectionBsp_;
	}

	/** Устнаовка флага наличия связи с БСП.
	 * 	@retval True - связь есть.
	 * 	@retval False - связи нет.
	 */
	void setConnectionBsp(bool f) {
		connectionBsp_ = f;
	}

	/** Параметры
	 */
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
	eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM * 2];

	// текущие пункты меню
	TMenuPunkt Punkts_;

	// параметры для ввода новых значений
	TEnterParam EnterParam;

	//  настройки для соответствующих аппаратов
	bool setDeviceK400();
	bool setDeviceRZSK();
	bool setDeviceR400M();
	bool setDeviceOPTO();

	// очистка текстового буфера
	void clearTextBuf();

	// очистка строки
	void clearLine(uint8_t line);

	// вывод сообщения на экран
	void printMessage() {
		delay_ = 0;
	}

	// возвращает true - в случае необходимости вывода сообщения
	bool isMessage() const {
		return (delay_ < TIME_MESSAGE);
	}

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
	void lvlTest();
	void lvlTest1();
	void lvlTest2();
	void lvlSetupInterface();

	// ввод параметра
	eMENU_ENTER_PARAM enterValue();

	eMENU_ENTER_PARAM enterPassword();

	// перемещение курсора вверх
	void cursorLineUp() {
		cursorLine_=(cursorLine_>1)? cursorLine_-1 : Punkts_.getMaxNumPunkts();
	}

	// пермещение курсора вниз
	void cursorLineDown() {
		cursorLine_=(cursorLine_<Punkts_.getMaxNumPunkts())? cursorLine_+1 : 1;
	}

	// вывод на экран текущих пунктов меню и курсора
	void printPunkts();

	/// Вывод на экран текущего параметра.
	void printParam();

	/// Вывод на экран названия параметра.
	void printName();

	///	Вывод на экран текущего номера и их колчиество для однотипных пар-ов.
	void printSameNumber(uint8_t pos);

	///	Вывод на экран диапазона значений параметра.
	void printRange(uint8_t pos);

	/**	Вывод на экран текущего значения параметра.
	 *
	 * 	Если значение параметра записано с ошибкой, будет поочердено выводится
	 * 	предупреждающая надпись и текущее значение параметра.
	 *
	 * 	Для строковых параметров учитывается минимальное значение. Т.е. из
	 * 	текущего значения параметра вычитается минимальное значение, а затем
	 * 	осуществляется выбор из массива строк значений параметра.
	 */
	void printValue(uint8_t pos);

	/**	Настройка параметров для ввода значения с клавиатуры.
	 *
	 *	Для параметров типа \a Param::PARAM_NO функция ввода значения не
	 *	вызывается.
	 *
	 * 	Для параметров типа \a Param::PARAM_LIST вызывается функция выбора
	 * 	значения из списка.
	 *
	 * 	Для параметров типа \a Param::PARAM_INT вызывается функция ввода
	 * 	целого числа.
	 *
	 * 	Для параметров типа \a Param::PARAM_U_COR вызывается функция ввода
	 * 	коррекции напряжения, а также минимальное значение подменяется на 0.
	 *
	 * 	Для параметров типа \a Param::PARAM_I вызывается функция ввода целого
	 * 	числа, при этом минимальное значение подменяется на 0.
	 */
	void enterParameter();

	// текущая функция ввода
	eMENU_ENTER_PARAM (clMenu::*enterFunc)();

	// текущий уровень меню
	void (clMenu::*lvlMenu)();
};

//static char punkt1[] PROGMEM = "Задержка на фикс.ком";
//static char punkt2[] PROGMEM = "Блокиров. команды";
//static char punkt3[] PROGMEM = "Задержка на выкл.ком";
//static char punkt4[] PROGMEM = "Трансляция ЦС";
//static char punkt5[] PROGMEM = "Блокиров. команды ЦС";
//static char punkt6[] PROGMEM = "Команда ВЧ в ЦС";
//static char punkt7[] PROGMEM = "Количество команд";		// К400

#endif /* MENU_H_ */
