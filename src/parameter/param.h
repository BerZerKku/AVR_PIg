#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "glbDefine.h"

//typedef struct Param Param_t PROGMEM;

struct Param {
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
		RANGE_FLOAT10,	///< <min/10>..<max/10>
		RANGE_FLOAT100	///< <min/100>..<max/100>
	};

	/// Тип параметра, в плане ввода / отображения.
	enum PARAM_TYPE {
		PARAM_NO = 0,	///< Неизменяемый параметр.
		PARAM_LIST,		///< Список.
		PARAM_INT,		///< Целое значение -32768..32767.
		PARAM_BITES,	///< Набор битовых значений для однотипных параметров.
		PARAM_BITE,		///< Один бит в принятом байте данных.
		PARAM_FLOAT10,	///< Дробное значение <val/10>.<val%10>.
		PARAM_FLOAT100	///< Дробное значение <val/100>.<val%100>.
	};

	/// Зависимость максимума параметра.
	enum DEPEND_MAX {
		DEPEND_MAX_NO,				///< Нет зависимости.
		DEPEND_MAX_ON_COM_PRD,		///< Зависит от количества команд на ПРД.
		DEPEND_MAX_ON_NUM_DEVS		///< Зависит от количества команд на ПРМ.
	};

	/// Зависимость повторов параметра.
	enum DEPEND_SAME {
		DEPEND_SAME_NO,				///< Нет зависимости.
		DEPEND_SAME_ON_NUM_DEVS,	///< Зависит от количества аппаратов в линии.
		DEPEND_SAME_ON_COM_PRD,		///< Зависит от количества команд на ПРД.
		DEPEND_SAME_ON_COM_PRM		///< Зависит от количества команд на ПРМ.
	};

	/// Условие для изменения параметра.
	enum CHANGE_COND {
		CHANGE_COND_NO,				///< Нет условий.
		CHANGE_COND_REG_DISABLE		///< Изменить можно только в режиме "Выведен".
	};

	char name[NAME_PARAM_LENGHT];	///< Имя параметра.
	eGB_COM com;		///< Команда считывания для стандартного протокола.

	PARAM_TYPE param;	///< Тип параметра.
	RANGE_TYPE range;	///< Тип строки для вывода диапазона значений.
	DIMENSION dim;		///< Размерность параметра.
	PGM_P listValues;	///< Массив параметров для строковых переменных.

	uint8_t num;		///< Количество подобных параметров.
	int16_t min;		///< Минимальное значение параметра.
	int16_t max;		///< Максимальное значение параметра или кол-во элементов в списке.
	uint8_t disc;		///< Дискретность.
	uint8_t fract;		///< Множитель для обмена по протоколу с БСП.

	eGB_SEND_WR_TYPE sendWr;	///< Тип команды для записи параметра.
	eGB_SEND_RD_TYPE sendRd;	///< Тип команды для чтение параметра.
	uint8_t sendDop;		///< Байт дополнительной информации для сохранения.
	uint8_t posBite;		///< Позиция первого бита для битовых не однородных данныхю.
	DEPEND_MAX dependMax;	///< Зависимость максимума текущего параметра.
	DEPEND_SAME dependSame;	///< Зависимость повторений текущего параметра.
	CHANGE_COND changeCond;	///< Условие для изменения параметра.
};

#endif /* PARAM_H_ */
