#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "glbDefine.h"

//typedef struct Param Param_t PROGMEM;

struct Param {

	/// Названия параметров
	enum NAME {
		TIME_SYNCH,			///<
		NUM_OF_DEVICES_2X,
		NUM_OF_DEVICES_3X,
	};

	/// Размернось параметра (связана с fcDimension)
	enum DIMENSION {
		DIM_NO = 0,		///< Нет
		DIM_MA,			///< мА
		DIM_A,			///< А
		DIM_KA,			///< кА
		DIM_V,			///< В
		DIM_KV,			///< кВ
		DIM_MSEC,		///< мс
		DIM_SEC,		///< с
		DIM_HOUR,		///< час
		DIM_DB,			///< дБ
		DIM_HZ,			///< Гц
		DIM_KHZ,		///< кГц
		DIM_DEG			///< град
	};

	/// Тип строки для отображения диапазона значений параметра.
	enum RANGE_TYPE {
		RANGE_LIST = 0,	///< Список.
		RANGE_ON_OFF,	///< Список вкл./выкл.
		RANGE_INT,		///< <min>..<max><dim>
		RANGE_INT_NO_DIM,///< <min>..<max>
		RANGE_U_COR,	///< 0..±<max/10><dim>
		RANGE_I_COR		///< 0..±<max><dim>
	};

	/// Тип параметра, в плане ввода / отображения.
	enum PARAM_TYPE {
		PARAM_NO = 0,	///< Неизменяемый параметр.
		PARAM_LIST,		///< Список.
		PARAM_INT,		///< Целое значение jn -32768..32767.
		PARAM_U_COR,	///< Коррекция напряжения ±ХХ.Х.
		PARAM_I_COR,	///< Коррекция тока ±XXX.
		PARAM_BITES		///< Намор битовых значений.
	};

	/// Зависимость максимума параметра.
	enum DEPEND_MAX {
		DEPEND_MAX_NO,
		DEPEND_MAX_ON_COM_PRD,
		DEPEND_MAX_ON_NUM_DEVS
	};

	/// Зависимость повторов параметра
	enum DEPEND_SAME {
		DEPEND_SAME_NO,
		DEPEND_SAME_ON_NUM_DEVS,
		DEPEND_SAME_ON_COM_PRD,
		DEPEND_SAME_ON_COM_PRM
	};

	char name[NAME_PARAM_LENGHT];	///< Имя параметра.
	eGB_COM com;		///< Команда считывания для стандартного протокола.

	PARAM_TYPE param;	///< Тип параметра.
	RANGE_TYPE range;	///< Тип строки для вывода диапазона значений.
	DIMENSION dim;		///< Размерность параметра.
	PGM_P listValues;	///< Массив параметров для строковых переменных.

	uint8_t num;		///< Количество подобных параметров.
	int16_t min;		///< Минимальное значение параметра.
	int16_t max;		///< Максимальное значение параметра.
	uint8_t disc;		///< Дискретность.
	uint8_t fract;		///< Множитель для обмена по протоколу с БСП.

	eGB_SEND_TYPE send;	///< Тип параметра для сохранения нового значения.
	uint8_t sendDop;	///< Байт дополнительной информации для сохранения.
	DEPEND_MAX dependMax;	///< Зависимость максимума текущего параметра.
	DEPEND_SAME dependSame;	///< Зависимость повторений текущего параметра.
};

#endif /* PARAM_H_ */
