/*
 * flashParams.h
 *
 *  Created on: 27.03.2015
 *      Author: Shcheblykin
 */

#ifndef FLASHPARAMS_H_
#define FLASHPARAMS_H_

#include <avr/pgmspace.h>
#include "glbDefine.h"
#include "param.h"

static const char fcNullBuf[] PROGMEM= "";

/// массив строк размерностей, связан с Param::DIMENSION
static const char fcDimension[] [5] PROGMEM = {
		"",		// Param::DIM_NO
		"мА",	// Param::DIM_MA
		"А",	// Param::DIM_A
		"кА",	// Param::DIM_KA
		"В",	// Param::DIM_V
		"кВ",	// Param::DIM_KV
		"мс",	// Param::MSEC
		"c",	// Param::DIM_SEC
		"час",	// Param::DIM_HOUR
		"дБ",	// Param::DIM_DB
		"Гц",	// Param::DIM_HZ
		"кГц",	// Param::DIM_KHZ
		"град"	// Param::DIM_DEG
};

/// Возможные значения параметра.
static const char fcOnOff[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"выкл.",		///< 0
		"вкл."			///< 1
};

/// Интерфейс связи (параметр Интерфейс).
static char fcInterface[TInterface::MAX][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"USB",			///< TInterface::USB
		"RS485"			///< TInterface::RS485
};

// Протокол (параметр Интерфейс).
static char fcProtocol[TProtocol::MAX ][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"СТАНДАРТ",		///< TProtocol::STANDART
		"MODBUS",		///< TProtocol::MODBUS
		"МЭК-101"		///< TProtocol::IEC_101
};

// Скорость, бит/с (параметр Интерфейс).
static char fcBaudRate[TBaudRate::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"600",			///< TBaudRate::_600
		"1200",			///< TBaudRate::_1200
		"2400",			///< TBaudRate::_2400
		"4800",			///< TBaudRate::_4800
		"9600",			///< TBaudRate::_9600
		"19200"			///< TBaudRate::_19200
};

/// Количество бит данных (параметр Интерфейс).
static char fcDataBits[TDataBits::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"8"				///< TDataBits::_8
};

/// Четность (параметр Интерфейс).
static char fcParity[TParity::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"нет",			///< TParity::NONE
		"чет",			///< TParity::EVEN
		"нечет"			///< TParity::ODD
};

/// Стоповые биты (параметр Интерфейс).
static char fcStopBits[TStopBits::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",			///< TStopBits::ONE
		"2"				///< TStopBits::TWO
};

// параметр заглушка
static const Param fNullParam PROGMEM = {
		"",						// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_NO,		// тип параметра
		Param::RANGE_ON_OFF,	// диапазон измнения
		Param::DIM_NO,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		0,						// минимальное значение
		1,						// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// частота
static const Param fFreq PROGMEM = {
		"Частота",				// название параметра
		GB_COM_GET_FREQ,		// команда стандартного протокола
		Param::PARAM_INT,		// тип параметра
		Param::RANGE_INT_NO_DIM,// диапазон измнения
		Param::DIM_KHZ,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		16,						// минимальное значение
		1000,					// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_INT16_BE,		// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// компенсация задержки
static const Param fCompDelay PROGMEM = {
		"Компенсация задержки",	// название параметра
		GB_COM_GET_COMP_DELAY,	// команда стандартного протокола
		Param::PARAM_COMP_D,	// тип параметра
		Param::RANGE_COMP_D,	// диапазон измнения
		Param::DIM_MSEC,		// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		0,						// минимальное значение
		10000,					// максимальное значение
		125,					// дискретность
		125,					// множитель для стандартного протокола
		GB_SEND_INT8,			// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// максимальный ток
static const Param fCurrMax PROGMEM = {
		"Максимальный ток",		// название параметра
		GB_COM_GET_CURR_MAX,	// команда стандартного протокола
		Param::PARAM_INT,		// тип параметра
		Param::RANGE_INT,		// диапазон измнения
		Param::DIM_KA,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		0,						// минимальное значение
		250,					// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_INT8,			// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// минимальный ток
static const Param fCurrMin PROGMEM = {
		"Минимальный ток",		// название параметра
		GB_COM_GET_CURR_MAX,	// команда стандартного протокола
		Param::PARAM_INT,		// тип параметра
		Param::RANGE_INT,		// диапазон измнения
		Param::DIM_KA,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		0,						// минимальное значение
		250,					// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_INT8,			// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// минимальное напряжение
static const Param fVoltMin PROGMEM = {
		"Миним. напряжение",	// название параметра
		GB_COM_GET_VOLT_MIN,	// команда стандартного протокола
		Param::PARAM_INT,		// тип параметра
		Param::RANGE_INT,		// диапазон измнения
		Param::DIM_KV,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		0,						// минимальное значение
		1020,					// максимальное значение
		4,						// дискретность
		4,						// множитель для стандартного протокола
		GB_SEND_INT8,			// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// интерфейс связи
static const Param fIntfInterface PROGMEM = {
		"Интерфейс связи",		// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcInterface[0],			// массив значений
		1,						// кол-во повторений параметра
		TInterface::MIN,		// минимальное значение
		SIZE_OF(fcInterface),	// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// четность - интерфейс связи
static const Param fIntfProtocol PROGMEM = {
		"Протокол",				// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcProtocol[0],			// массив значений
		1,						// кол-во повторений параметра
		TProtocol::MIN,			// минимальное значение
		SIZE_OF(fcProtocol),	// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// скорость связи - интерфейс связи
static const Param fIntfBaudrate PROGMEM = {
		"Скорость передачи",	// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcBaudRate[0],			// массив значений
		1,						// кол-во повторений параметра
		TBaudRate::MIN,			// минимальное значение
		SIZE_OF(fcBaudRate),	// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// количество битов данных - интерфейс связи
static const Param fIntfDataBits PROGMEM = {
		"Биты данных",			// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcDataBits[0],			// массив значений
		1,						// кол-во повторений параметра
		TDataBits::MIN,			// минимальное значение
		SIZE_OF(fcDataBits),	// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// четность - интерфейс связи
static const Param fIntfParity PROGMEM = {
		"Четность",				// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcParity[0],			// массив значений
		1,						// кол-во повторений параметра
		TParity::MIN,			// минимальное значение
		SIZE_OF(fcParity),		// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// число стоповых битов - интерфейс связи
static const Param fIntfStopBits PROGMEM = {
		"Стоповые биты",		// название параметра
		GB_COM_NO,				// команда стандартного протокола
		Param::PARAM_LIST,		// тип параметра
		Param::RANGE_LIST,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcStopBits[0],			// массив значений
		1,						// кол-во повторений параметра
		TStopBits::MIN,			// минимальное значение
		SIZE_OF(fcStopBits),	// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_NO,				// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_NO 	// условие для изменения параметра
};

// адрес в локальной сети
static const Param fNetAddress PROGMEM = {
		"Сетевой адрес",		// название параметра
		GB_COM_NO,				// команда стандартного протокола	// TODO
		Param::PARAM_INT,		// тип параметра
		Param::RANGE_INT,		// диапазон измнения
		Param::DIM_NO,			// размерность
		fcNullBuf,				// массив значений
		1,						// кол-во повторений параметра
		1,						// минимальное значение
		247,					// максимальное значение
		1,						// дискретность
		1,						// множитель для стандартного протокола
		GB_SEND_INT8,			// тип параметра для сохранения новго значения
		0,						// байт дополнительной информации для сохранения
		Param::DEPEND_MAX_NO,	// заивимость максимума
		Param::DEPEND_SAME_NO,	// зависимость повторений
		Param::CHANGE_COND_REG_DISABLE // условие для изменения параметра
};

// Массив параметров (связан с eGB_PARAM)
static const Param* fParams[] PROGMEM  = {
		&fNullParam,
		&fFreq,
		&fCompDelay,
		&fCurrMax,
		&fCurrMin,
		&fVoltMin,
		// Параметры интерфейса
		&fIntfInterface,
		&fIntfProtocol,
		&fIntfBaudrate,
		&fIntfDataBits,
		&fIntfParity,
		&fIntfStopBits,
		&fNetAddress
};

#endif /* FLASHPARAMS_H_ */
