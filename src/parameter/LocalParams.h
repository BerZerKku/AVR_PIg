/*
 * LocalParams.h
 *
 *  Created on: 25.03.2015
 *      Author: Shcheblykin
 */

#ifndef LOCALPARAMS_H_
#define LOCALPARAMS_H_

#include "param.h"

class LocalParams {
	/// Максимальное количество параметров в списке.
	static const uint8_t MAX_NUM_OF_PARAMS = 20;

public:

	/// Текущее состояние значения параметра
	enum STATE {
		STATE_NO_ERROR = 0,	///< Значение корректно.
		STATE_READ_PARAM,	///< Идет считывание параметра.
		STATE_ERROR			///< Значение ошибочное.
	};


	/// Конструктор
	LocalParams();

	/** Установка нового значения параметра и его проверка на корректность.
	 *
	 * 	@param val Новое значение параметра.
	 */
	void setValue(int16_t val);

	/**	Возвращает текущее значение параметра.
	 *
	 * 	@return Текущее значение параметра.
	 */
	int16_t getValue() const { return val; }

	/**	Возвращает состояние флага ошибки текущего значения.
	 *
	 * 	@retval True Ошибочное значение.
	 * 	@retval False Корректное значение.
	 */
	STATE isError() { return state; }

	/**	Переход к следующему параметру.
	 *
	 * 	Если достигли конца списка, происходит прыжок на первый параметр.
	 * 	Если был осуществлен переход (кол-во параметров больше 1), то ждем
	 * 	считывания значения параметра.
	 */
	void nextParam();

	/**	Переход к предыдущему параметру.
	 *
	 * 	Если достигли начала списка, происходит прыжок на последний параметр.
	 * 	Если был осуществлен переход (кол-во параметров больше 1), то ждем
	 * 	считывания значения параметра.
	 */
	void prevParam();

	/** Переход к следующему однотипному параметру.
	 *
	 *	Если достигли конца списка , происходит прыжок на первый параметр.
	 * 	Если был осуществлен переход (кол-во однотипных параметров больше 1),
	 * 	то ждем считывания значения параметра.
	 */
	void nextSameParam();

	/** Переход к предыдущему однотипному параметру.
	 *
	 *	Если достигли начала списка, происходит прыжок на последний параметр.
	 *	Если был осуществлен переход (кол-во однотипных параметров больше 1),
	 * 	то ждем считывания значения параметра.
	 */
	void prevSameParam();

	/** Добавление в список нового параметра.
	 *
	 * 	В случае переполнения буфера параметров, будет возвращена ошибка.
	 *
	 * 	@param *newParam Указатель на структуру параметра.
	 * 	@retval True Параметр добавлен.
	 * 	@retval False Буфер переполнен, параметр небыл добавлен.
	 *
	 */
	bool addParam(eGB_PARAM newParam);

	///	Очистка списка параметров.
	void clearParams();

	/**	Возвращает минимальное значение параметра ?!
	 *
	 * 	@return Минимальное значение параметра.
	 */
	int16_t getMin() const { return pgm_read_word(&fp[param[currParam]].min);	}

	/**	Возвращает максимальное значение параметра ?!
	 *
	 * 	@return Максимальное значение параметра.
	 */
	int16_t getMax() const { return pgm_read_word(&fp[param[currParam]].max); }

	/**	Возвращает дискретность параметра ?!
	 *
	 * 	@param Дискретность параметра.
	 */
	uint8_t getDisc() const { return pgm_read_byte(&fp[param[currParam]].disc); }

	/**	Возвращает множитель параметра.
	 *
	 * 	@param Множитель параметра.
	 */
	uint8_t getFract() const { return pgm_read_byte(&fp[param[currParam]].fract); }

	/** Отправка команды на считывание по стандартному протоколу.
	 *
	 * 	@param *buf Указатель на буфер передатчика.
	 * 	@param *val Дополнительный байт данных.
	 * 	@return Возвращает кол-во байт данных на передачу.
	 */
	uint8_t  (*txComByProtocolS) (uint8_t *buf, uint8_t val);

	/** Формирование посылки стандартного протокола.
	 *
	 * 	@param *buf Указатель на буфер передатчика.
	 * 	@param *val Указатель на значение параметра.
	 * 	@return Возвращает кол-во байт данных на передачу.
	 */
	uint8_t  (*txParamByProtocolS) (uint8_t *buf, uint8_t *val);

	/** Извлечение данных из посылки стандартного протокола.
	 *
	 * 	@param *buf Указатель на буфер приемника.
	 * 	@return Возвращает значение параметра.
	 */
	uint16_t (*rxParamByProtocolS) (uint8_t *buf);

	/**	Возвращает номер текущего параметра.
	 *
	 * 	@return Номер текущего параметра.
	 */
	uint8_t getNumOfCurrParam() const {
		return currParam + 1;
	}

	/**	Возвращает количество текущих параметров.
	 *
	 * 	@return Количество текущих параметров.
	 */
	uint8_t getNumOfParams() const {
		return numOfParams;
	}

	/**	Возвращает указатель на сторку с названием параметра.
	 *
	 * 	Строка находится во FLASH.
	 *
	 * 	@return Указатель на строку с названием параметра.
	 *
	 */
	PGM_P getNameOfParam() const {
		return (PGM_P) pgm_read_word(&fp[param[currParam]].name);
	}

	/**	Возвращает указатель на первую строку массива значений параметра.
	 *
	 * 	Актуально для переменных значение которых выбирается из списка. Сам
	 * 	список находится во FLASH.
	 *
	 * 	@return Указатель на первую строку массива значений параметра.
	 *
	 */
	PGM_P getListOfValues() const {
		return (PGM_P) pgm_read_word(&fp[param[currParam]].listValues);
	}

	/**	Возвращает количество однотипных парметров.
	 *
	 * 	Например, если для каждой из команд возможно установить свое значение
	 * 	"Длительность команды", то тут будет количество команд.
	 *
	 * 	@return Количество однотипных параметров.
	 */
	uint8_t getNumOfCurrSameParam() const {
		return currSameParam + 1;
	}

	/**	Возвращает текущий номер однотипного параметра.
	 *
	 * 	@return Текущий номер однотипного параметра.
	 */
	uint8_t getNumOfSameParams() const {
		return numOfSameParam;
	}

	/**	Возвращает тип текущего параметра.
	 *
	 * 	@return Тип текущего параметра.
	 */
	Param::PARAM_TYPE getParamType() const {
		return (Param::PARAM_TYPE) pgm_read_byte(&fp[param[currParam]].param);
	}

	/**	Возвращает тип диапазона значений декущего параметра.
	 *
	 * 	@return Тип диапазона значений текущего параметра.
	 */
	Param::RANGE_TYPE getRangeType() const {
		return (Param::RANGE_TYPE) pgm_read_byte(&fp[param[currParam]].range);
	}

	/**	Возвращает размерность текущего параметра.
	 *
	 * 	@return Размерность текущего параметра.
	 */
	Param::DIMENSION getDim() const {
		return (Param::DIMENSION) pgm_read_byte(&fp[param[currParam]].dim);
	}

	/**	Возвращает команду стандартного протокола для текущего параметра.
	 *
	 * 	@return Команда стандартного протокола для текущего параметра.
	 */
	eGB_COM getCom() const {
		return (eGB_COM) pgm_read_byte(&fp[param[currParam]].com);
	}

	/**	Установка массива значений параметров.
	 *
	 * 	Сам массив находится во FLASH, но это учитывается далее.
	 *
	 * 	@param *ptr Указатель на массив значений параметров.
	 */
	void setFlashParams(const Param *ptr) {
		fp = ptr;
	};

	/**	Возвращает текущий параметр.
	 *
	 * 	@return Текущий параметр.
	 */
	eGB_PARAM getParam() const {
		return param[currParam];
	}

private:

	const Param* fp;		///< Массив значений параметров.

	eGB_PARAM param[MAX_NUM_OF_PARAMS]; ///< Массив параметров.

//	const Param* param[MAX_NUM_OF_PARAMS];	///< Массив параметров.

	uint16_t val;			///< Значение текущего параметра.

	uint8_t currParam;		///< Номер текущего параметра.

	uint8_t numOfParams;	///< Количество параметров в текущем списке.

	uint8_t currSameParam;	///< Номер текущего однотипного параметра.

	uint8_t numOfSameParam;	///< Количество однотипных параметров.

	STATE state;			///< Флаг ошибки в текущем значении.

	/**	Проверка установленного значения параметра на корректность.
	 *
	 * 	В случа ошибочного значения в состояние \a state будет записано
	 * 	значение STATE_ERROR, если все верно - \a STATE_NO_ERROR.
	 *
	 *	@see STATE_ERROR
	 *	@see STATE_NO_ERROR
	 *
	 * 	@param val Новое значение параметра.
	 *
	 */
	void check(int16_t val);

	/**	Обновление параметра.
	 *
	 * 	Текущий статус \a state сбрасывается в ожидание чтения STATE_READ_PARAM,
	 * 	а значение параметра сбрасывается в 0.
	 *
	 */
	void refreshParam();
};

#endif /* LOCALPARAMS_H_ */
