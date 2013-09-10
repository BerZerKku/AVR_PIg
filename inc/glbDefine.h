/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 * 	Максимальное значение для строковых параметров на 1 больше возможного,
 * 	для хранения в массиве ошибочного значения и удобства пользования
 * 	перечислениями.
 * 	Так что для ввода строкового значения максимум должен быть меньше на 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "debug.h"

/// пароль администратора
#define PASSWORD_ADMIN 6352

/// пароль пользователя по умолчанию
#define PASSWORD_USER 0

/// версия текущей прошивки
#define VERS 0x0100

/// максимально кол-во команд на прием (должно быть кратно 8)
#define MAX_NUM_COM_PRM 32

/// максимальное кол-во команд на передачу (должно быть кратно 8)
#define MAX_NUM_COM_PRD 32

/// преобразование двух CHAR в INT
#define TO_INT16(high, low) (((uint16_t) high << 8) + low)

/// максимально возможное кол-во состояний устройств
#define MAX_NUM_DEVICE_STATE 12

/// максимальное кол-во команд в буфере (не считая 3-х основных)
#define MAX_NUM_COM_BUF 15

/// максимальное кол-во сигналов в тестах
#define MAX_NUM_TEST_SIGNAL 40

/// Минимальные, максимальные значения параметров приемника и их дискретность
/// ЗАЩИТА
/// тип защиты
#define DEF_TYPE_MIN		0
#define DEF_TYPE_MAX		8
/// допустимое время без манипуляции
#define DEF_T_NO_MAN_MIN	0
#define DEF_T_NO_MAN_MAX	99
#define DEF_T_NO_MAN_DISC	1
#define DEF_T_NO_MAN_FRACT	1
#define DEF_T_NO_MAN_MIN_F	(DEF_T_NO_MAN_MIN / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_MAX_F	(DEF_T_NO_MAN_MAX / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_DISC_F	(DEF_T_NO_MAN_DISC / DEF_T_NO_MAN_FRACT)
/// перекрытие импульсов
#define DEF_OVERLAP_MIN		0
#define DEF_OVERLAP_MAX		54
#define DEF_OVERLAP_DISC	2
#define DEF_OVERLAP_FRACT	1
#define DEF_OVERLAP_MIN_F	(DEF_OVERLAP_MIN / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_MAX_F	(DEF_OVERLAP_MAX / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_DISC_F	(DEF_OVERLAP_DISC / DEF_OVERLAP_FRACT)
/// компенсация задержки в линии
#define DEF_DELAY_MIN		0
#define DEF_DELAY_MAX		18
#define DEF_DELAY_DISC		2
#define DEF_DELAY_FRACT		1
#define DEF_DELAY_MIN_F		(DEF_DELAY_MIN / DEF_DELAY_FRACT)
#define DEF_DELAY_MAX_F		(DEF_DELAY_MAX / DEF_DELAY_FRACT)
#define DEF_DELAY_DISC_F	(DEF_DELAY_DISC / DEF_DELAY_FRACT)
/// порог предупреждения по РЗ
#define DEF_RZ_THRESH_MIN	0
#define DEF_RZ_THRESH_MAX	16
#define DEF_RZ_THRESH_DISC	1
#define DEF_RZ_THRESH_FRACT	1
#define DEF_RZ_THRESH_MIN_F	(DEF_RZ_THRESH_MIN / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_MAX_F	(DEF_RZ_THRESH_MAX / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_DISC_F (DEF_RZ_THRESH_DISC / DEF_RZ_THRESH_FRACT)
/// загрубление чувствительности по РЗ
#define DEF_RZ_DEC_MIN		0
#define DEF_RZ_DEC_MAX		32
#define DEF_RZ_DEC_DISC		1
#define DEF_RZ_DEC_FRACT	1
#define DEF_RZ_DEC_MIN_F	(DEF_RZ_DEC_MIN / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_MAX_F	(DEF_RZ_DEC_MAX / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_DISC_F	(DEF_RZ_DEC_DISC / DEF_RZ_DEC_FRACT)
/// тип приемника
#define DEF_PRM_TYPE_MIN	0
#define DEF_PRM_TYPE_MAX	3
/// ПРМ
/// время включения
#define PRM_TIME_ON_MIN		0
#define PRM_TIME_ON_MAX		10
#define PRM_TIME_ON_DISC	1
#define PRM_TIME_ON_FRACT	1
#define PRM_TIME_ON_MIN_F	(PRM_TIME_ON_MIN / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_MAX_F	(PRM_TIME_ON_MAX / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_DISC_F 	(PRM_TIME_ON_DISC / PRM_TIME_ON_FRACT)
/// задержка на выключение
#define PRM_TIME_OFF_MIN	0
#define PRM_TIME_OFF_MAX	1000
#define PRM_TIME_OFF_DISC	50
#define PRM_TIME_OFF_FRACT	10
#define PRM_TIME_OFF_MIN_F	(PRM_TIME_OFF_MIN / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_MAX_F	(PRM_TIME_OFF_MAX / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_DISC_F	(PRM_TIME_OFF_DISC/ PRM_TIME_OFF_FRACT)
/// ПРД
/// время включения (задержка срабатывания дискретного входа)
/// было от 0 до 10, изменено по требованию АГ
#define PRD_TIME_ON_MIN		5
#define PRD_TIME_ON_MAX		20
#define PRD_TIME_ON_DISC	1
#define PRD_TIME_ON_FRACT	1
#define PRD_TIME_ON_MIN_F	(PRD_TIME_ON_MIN / PRM_TIME_ON_FRACT)
#define PRD_TIME_ON_MAX_F	(PRD_TIME_ON_MAX / PRM_TIME_ON_FRACT)
#define PRD_TIME_ON_DISC_F	(PRD_TIME_ON_DISC / PRM_TIME_ON_FRACT)
/// длительность команды
#define PRD_DURATION_MIN	20
#define PRD_DURATION_MAX	100
#define PRD_DURATION_DISC	1
#define PRD_DURATION_FRACT	1
#define PRD_DURATION_MIN_F	(PRD_DURATION_MIN / PRD_DURATION_FRACT)
#define PRD_DURATION_MAX_F	(PRD_DURATION_MAX / PRD_DURATION_FRACT)
#define PRD_DURATION_DISC_F	(PRD_DURATION_DISC / PRD_DURATION_FRACT)
/// ОБЩИЕ
/// номер аппарата
#define GLB_DEV_NUM_MIN		1
#define GLB_DEV_NUM_MAX 	3
#define GLB_DEV_NUM_DISC 	1
#define GLB_DEV_NUM_FRACT	1
#define GLB_DEV_NUM_MIN_F	(GLB_DEV_NUM_MIN / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_MAX_F 	(GLB_DEV_NUM_MAX / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_DISC_F	(GLB_DEV_NUM_DISC / GLB_DEV_NUM_FRACT)
/// порог предупреждения по КЧ
#define GLB_CF_THRESH_MIN	0
#define GLB_CF_THRESH_MAX	22
#define GLB_CF_THRESH_DISC	1
#define GLB_CF_THRESH_FRACT	1
#define GLB_CF_THRESH_MIN_F	(GLB_CF_THRESH_MIN / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_MAX_F	(GLB_CF_THRESH_MAX / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_DISC_F (GLB_CF_THRESH_DISC / GLB_CF_THRESH_FRACT)
/// время перезапуска
#define GLB_T_RERUN_MIN		0
#define GLB_T_RERUN_MAX		5
#define GLB_T_RERUN_DISC	1
#define GLB_T_RERUN_FRACT	1
#define GLB_T_RERUN_MIN_F	(GLB_T_RERUN_MIN / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_MAX_F	(GLB_T_RERUN_MAX / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_DISC_F	(GLB_T_RERUN_DISC / GLB_T_RERUN_FRACT)
/// уменьшение усиления канала
#define GLB_IN_DEC_MIN		0
#define GLB_IN_DEC_MAX		22
#define GLB_IN_DEC_DISC 	1
#define GLB_IN_DEC_FRACT	1
#define GLB_IN_DEC_MIN_F	(GLB_IN_DEC_MIN / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_MAX_F	(GLB_IN_DEC_MAX / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_DISC_F	(GLB_IN_DEC_DISC / GLB_IN_DEC_FRACT)

/// максимальное и минимальный код типа событий в журнале событий
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 33

/// кол-во записей в журнале событый
#define GLB_JRN_EVENT_K400_MAX 512

/// кол-во записей в журнале защиты
#define GLB_JRN_DEF_K400_MAX 0

/// кол-во записей в журнале приемника
#define GLB_JRN_PRM_K400_MAX 512

/// кол-во записей в журнале передатчика
#define GLB_JRN_PRD_K400_MAX 512

/// максимально возможное кол-во записей в одном архиве
#define GLB_JRN_MAX 1024


/// Тип аппарата
enum eGB_TYPE_DEVICE
{
	AVANT_NO = 0,		// ошибочное значение
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_RZSK_OPTIC,
	AVANT_K400,
	AVANT_K400_OPTIC
};

/// Устройство
enum eGB_DEVICE
{
	GB_DEVICE_MIN = 0,
	GB_DEVICE_DEF = 0,
	GB_DEVICE_PRM,
	GB_DEVICE_PRD,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// Тип канала связи
enum eGB_TYPE_LINE
{
	GB_TYPE_LINE_MIN = 1,
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTIC = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// Кол-во аппаратов в линии
enum eGB_NUM_DEVICES
{
	GB_NUM_DEVICES_MIN = 1,
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// Совместимость
enum eGB_COMPATIBILITY
{
	GB_COMPATIBILITY_MIN = 0,
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK = 2,
	GB_COMPATIBILITY_PVZUE = 3,
	GB_COMPATIBILITY_MAX
};

/// Режимы работы
enum eGB_REGIME
{
	GB_REGIME_MIN = 0,
	GB_REGIME_DISABLED = 0,	// выведен
	GB_REGIME_READY,		// готов
	GB_REGIME_ENABLED,		// введен
	GB_REGIME_TALK,			// речь
	GB_REGIME_TEST_1,		// тест 1
	GB_REGIME_TEST_2,		// тест 2
	GB_REGIME_MAX,
};

// Режимы работы для ввода с клавиатуры
// порядок не совпадает с eGB_REGIME
enum eGB_REGIME_ENTER
{
	GB_REGIME_ENTER_MIN = 0,
	GB_REGIME_ENTER_DISABLED = 0,
	GB_REGIME_ENTER_ENABLED = 1,
	GB_REGIME_ENTER_TEST_1,
	GB_REGIME_ENTER_TEST_2,
	GB_REGIME_ENTER_MAX
};

/// Команды
enum eGB_COM
{
	GB_COM_NO = 0,
	GB_COM_DEF_GET_DEF_TYPE	= 0x01,
	GB_COM_DEF_GET_LINE_TYPE= 0x02,
	GB_COM_DEF_GET_T_NO_MAN	= 0x03,
	GB_COM_DEF_GET_DELAY	= 0x04,
	GB_COM_DEF_GET_OVERLAP	= 0x05,
	GB_COM_DEF_GET_RZ_DEC	= 0x06,
	GB_COM_DEF_GET_PRM_TYPE = 0x07,
	GB_COM_DEF_GET_RZ_THRESH= 0x09,	// ! в Р400 это частота ПРМ
	GB_COM_DEF_GET_TYPE_AC 	= 0x0A,	// !!! не сделана
	GB_COM_PRM_GET_TIME_ON	= 0x11,
	GB_COM_PRM_GET_TIME_OFF = 0x13,
	GB_COM_PRM_GET_BLOCK_COM= 0x14,
	GB_COM_PRD_GET_TIME_ON	= 0x21,
	GB_COM_PRD_GET_DURATION = 0x22,
	GB_COM_PRD_GET_BLOCK_COM= 0x24,
	GB_COM_PRD_GET_LONG_COM = 0x25,
	GB_COM_PRD_GET_TEST_COM = 0x26,
	GB_COM_GET_SOST			= 0x30,
	GB_COM_GET_FAULT 		= 0x31,
	GB_COM_GET_TIME 		= 0x32,
	GB_COM_GET_MEAS			= 0x34,
	GB_COM_GET_TIME_SINCHR	= 0x35,
	GB_COM_GET_COM_PRM_KEEP = 0x36,
	GB_COM_GET_COM_PRD_KEEP	= 0x37,	// ! в Р400 это тип удаленного аппарата
	GB_COM_GET_TIME_RERUN	= 0x39,	// ! в Р400 это параметры ПВЗУ-Е
	GB_COM_GET_DEVICE_NUM	= 0x3B,
	GB_COM_GET_CF_THRESHOLD	= 0x3C,	// ! порог предупр. по КЧ и загрубления
	GB_COM_GET_OUT_CHECK	= 0x3D,
	GB_COM_GET_TEST			= 0x3E,
	GB_COM_GET_VERS			= 0x3F,
	GB_COM_PRM_ENTER		= 0x51,
	GB_COM_SET_REG_DISABLED	= 0x70,
	GB_COM_SET_REG_ENABLED	= 0x71,
	GB_COM_SET_CONTROL		= 0x72,
	GB_COM_SET_PASSWORD 	= 0x73,	// ! только с ПК
	GB_COM_GET_PASSWORD 	= 0x74,	// ! только с ПК
	GB_COM_SET_REG_TEST_2	= 0x7D,
	GB_COM_SET_REG_TEST_1	= 0x7E,
	GB_COM_PRM_SET_TIME_ON	= 0x91,
	GB_COM_PRM_SET_TIME_OFF	= 0x93,
	GB_COM_PRM_SET_BLOCK_COM= 0x94,
	GB_COM_PRM_RES_IND		= 0x9A,
	GB_COM_PRD_SET_TIME_ON	= 0xA1,
	GB_COM_PRD_SET_DURATION = 0xA2,
	GB_COM_PRD_SET_BLOCK_COM= 0xA4,
	GB_COM_PRD_SET_LONG_COM	= 0xA5,
	GB_COM_PRD_SET_TEST_COM = 0xA6,
	GB_COM_SET_TIME 		= 0xB2,
	GB_COM_SET_TIME_SINCHR	= 0xB5,
	GB_COM_SET_COM_PRM_KEEP	= 0xB6,
	GB_COM_SET_COM_PRD_KEEP	= 0xB7,
	GB_COM_SET_TIME_RERUN	= 0xB9,
	GB_COM_SET_DEVICE_NUM	= 0xBB,
	GB_COM_SET_CF_THRESHOLD	= 0xBC,
	GB_COM_SET_OUT_CHECK	= 0xBD,
	GB_COM_DEF_GET_JRN_CNT	= 0xC1,
	GB_COM_DEF_GET_JRN_ENTRY= 0xC2,
	GB_COM_DEF_JRN_CLR		= 0xCA,	// ! стирание журнала ЗАЩ, только с ПК
	GB_COM_PRM_GET_JRN_CNT	= 0xD1,
	GB_COM_PRM_GET_JRN_ENTRY= 0xD2,
	GB_COM_PRM_JRN_CLR		= 0xDA,	// ! стирание журнала ПРМ, только с ПК
	GB_COM_PRD_GET_JRN_CNT	= 0xE1,
	GB_COM_PRD_GET_JRN_ENTRY= 0xE2,
	GB_COM_PRD_JRN_CLR		= 0xEA,	// ! стирание журнала ПРД, только с ПК
	GB_COM_GET_JRN_CNT		= 0xF1,
	GB_COM_GET_JRN_ENTRY	= 0xF2,
	GB_COM_JRN_CLR			= 0xFA	// ! стирание журнала событий, только с ПК
};

/// Маски команд
enum eGB_COM_MASK
{
	// Тип устройства
	GB_COM_MASK_DEVICE 		= 0x30,
	GB_COM_MASK_DEVICE_DEF 	= 0x00,
	GB_COM_MASK_DEVICE_PRM 	= 0x10,
	GB_COM_MASK_DEVICE_PRD 	= 0x20,
	GB_COM_MASK_DEVICE_GLB 	= 0x30,

	// группа команды
	GB_COM_MASK_GROUP 				= 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM 	= 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME 	= 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM 	= 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL	= 0xC0
};

/// Значения команд управления
enum eGB_CONTROL
{
	GB_CONTROL_RESET_SELF 	= 1,
	GB_CONTROL_RESET_UD_1	= 2,
	GB_CONTROL_RESET_UD_2	= 3,
	GB_CONTROL_PUSK_UD_1	= 4,
	GB_CONTROL_PUSK_UD_2 	= 5,
	GB_CONTROL_PUSK_UD_ALL	= 6,
	GB_CONTROL_CALL			= 7,
	GB_CONTROL_PUSK_ON		= 8,
	GB_CONTROL_PUSK_OFF		= 9,
	GB_CONTROL_MAX
};

/// События журнала передатчика/приемника - конец и начало команды
enum eGB_STATE_COM
{
	GB_STATE_COM_MIN = 0,
	GB_STATE_COM_END = 0,
	GB_STATE_COM_START,
	GB_STATE_COM_MAX
};

/// сигналы в тест1 и тест2
enum eGB_TEST_SIGNAL
{
	GB_SIGNAL_NO = 0,
	GB_SIGNAL_CF,		// РЗСК
	GB_SIGNAL_CF1,
	GB_SIGNAL_CF2,
	GB_SIGNAL_CF3,
	GB_SIGNAL_CF4,
	GB_SIGNAL_CF_NO_RZ,	// РЗСК
	GB_SIGNAL_CF_RZ,	// РЗСК
	GB_SIGNAL_RZ,
	GB_SIGNAL_COM1_RZ,	// РЗСК
	GB_SIGNAL_COM2_RZ,	// РЗСК
	GB_SIGNAL_COM3_RZ,	// РЗСК
	GB_SIGNAL_COM4_RZ,	// РЗСК
	GB_SIGNAL_COM1,		// сигналы команд должны идти подряд для заполнения К400
	GB_SIGNAL_COM2,
	GB_SIGNAL_COM3,
	GB_SIGNAL_COM4,
	GB_SIGNAL_COM5,
	GB_SIGNAL_COM6,
	GB_SIGNAL_COM7,
	GB_SIGNAL_COM8,
	GB_SIGNAL_COM9,
	GB_SIGNAL_COM10,
	GB_SIGNAL_COM11,
	GB_SIGNAL_COM12,
	GB_SIGNAL_COM13,
	GB_SIGNAL_COM14,
	GB_SIGNAL_COM15,
	GB_SIGNAL_COM16,
	GB_SIGNAL_COM17,
	GB_SIGNAL_COM18,
	GB_SIGNAL_COM19,
	GB_SIGNAL_COM20,
	GB_SIGNAL_COM21,
	GB_SIGNAL_COM22,
	GB_SIGNAL_COM23,
	GB_SIGNAL_COM24,
	GB_SIGNAL_COM25,
	GB_SIGNAL_COM26,
	GB_SIGNAL_COM27,
	GB_SIGNAL_COM28,
	GB_SIGNAL_COM29,
	GB_SIGNAL_COM30,
	GB_SIGNAL_COM31,
	GB_SIGNAL_COM32,
	GB_SIGNAL_MAX
};

#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))
#define BIN_TO_BCD(val) (((val / 10) << 4) + (val % 10))

/// Класс для даты и времени
class TDataTime
{
public:
	// Считывание и установка секунд
	uint8_t getSecond() const { return second_; }
	bool setSecond(uint8_t val)
	{
		bool stat = false;
		if (val < 60)
		{
			second_ = val;
			stat = true;
		}
		else
		{
			second_ = 61;
		}
		return stat;
	}

	// считывание и установка минут
	uint8_t getMinute() const { return minute_; }
	bool setMinute(uint8_t val)
	{
		bool stat = false;
		if (val < 60)
		{
			minute_ = val;
			stat = true;
		}
		else
		{
			minute_ = 61;
		}
		return stat;
	}

	// считывание и установка часов
	uint8_t getHour() const { return hour_; }
	bool setHour(uint8_t val)
	{
		bool stat = false;
		if (val < 24)
		{
			hour_ = val;
			stat = true;
		}
		else
		{
			hour_ = 25;
		}
		return stat;
	}

	// считывание и установка дня
	uint8_t getDay() const { return day_; }
	bool setDay(uint8_t val)
	{
		bool stat = false;

		if ( (val > 0) && (val <= getNumDaysInMonth()) )
		{
			day_ = val;
			stat = true;
		}
		else
		{
			day_ = 32;
		}

		return stat;
	}

	// возвращает кол-во дней в месяце
	// если месяц не задан, возвращается для текущего
	uint8_t getNumDaysInMonth (uint8_t month = 0) const
	{
		uint8_t num = 0;

		if (month == 0)
			month = month_;

		if ( (month == 4) || (month == 6) || (month == 9) ||
				(month == 11) )
		{
			num = 30;
		}
		else if (month == 2)
		{
			num = ((year_ % 4) == 0) ? 29 : 28;
		}
		else if ( (month != 0) && (month < 13) )
		{
			num = 31;
		}

		return num;
	}

	// считывание и установка месяца
	uint8_t getMonth() const { return month_; }
	bool setMonth(uint8_t val)
	{
		bool stat = false;
		if ( (val > 0) && ( val <= 12) )
		{
			month_ = val;
			stat = true;
		}
		else
		{
			month_ = 13;
		}
		return stat;
	}

	// считывание и установка года
	uint8_t getYear() 	const { return year_; }
	bool setYear(uint8_t val)
	{
		bool stat = false;
		if (val < 100)
		{
			year_ = val;
			stat = true;
		}
		else
		{
			year_ = 0;
		}
		return stat;
	}

	// считывание и установка миллисекунд
	uint16_t getMsSecond() const { return msSecond_; }
	bool setMsSecond(uint16_t val)
	{
		bool stat = false;
		if (val < 1000)
		{
			msSecond_ = val;
			stat = true;
		}
		else
		{
			msSecond_ = 1000;
		}
		return stat;
	}

private:
	uint16_t msSecond_;
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;
};


/// Класс для пароля
class TPassword
{
public:
	TPassword()
	{
		password_ = 10000;
		admin_ = PASSWORD_ADMIN;
	}

	// возвращает текущий пароль пользователя
	uint16_t get() const { return password_; }

	// устанавливает пароль пользователя
	bool set(uint16_t pas)
	{
		bool stat = false;
		if (pas < 10000)
		{
			password_ = pas;
			stat = true;
		}
		return stat;
	}

	// начальная инициализация пароля пользователя
	// если введено неверное значение, пароль будет PASSWORD_USER
	void init(uint16_t pas)
	{
		if (!set(pas))
			set(PASSWORD_USER);
	}

	// проверка значения на совпадение с паролем пользователя и администратора
	bool check(uint16_t pas) const
	{
		return ( (pas == password_) || (pas == admin_) );
	}

private:
	// пароль пользователя
	uint16_t password_;

	// пароль администратора
	uint16_t admin_;
};


/// Класс для текущего состояния аппарата
class TDeviceStatus
{
public:
	TDeviceStatus()
	{
		enable_ = false;

		fault_ = 0;
		faults_ = 0;
		numFaults_ = 0;

		warning_ = 0;
		warnings_ = 0;
		numWarnings_ = 0;

		regime_ = GB_REGIME_MAX;
		state_ = MAX_NUM_DEVICE_STATE;
		dopByte_ = 0;
	}

	// аварии
	bool setFault(uint16_t faults)
	{
		fault_ 		= getFirstSetBit(faults);
		faults_ 	= faults;
		numFaults_ 	= getNumSetBits(faults);
		return true;
	}
	uint8_t getFault() 		const { return fault_; }
	uint16_t getFaults() 	const { return faults_; }
	uint8_t getNumFaults()	const { return numFaults_; }


	// неисправности
	bool setWarning(uint16_t warnings)
	{
		warning_ 	= getFirstSetBit(warnings);
		warnings_ 	= warnings;
		numWarnings_= getNumSetBits(warnings);
		return true;
	}
	uint8_t getWarning() 	const { return warning_; }
	uint16_t getWarnings() 	const { return warnings_; }
	uint8_t getNumWarnings()const { return numWarnings_; }

	// режим работы
	// для GLB возвращается GB_REGIME_DISABLED, в случае если все имеющиеся
	// устройства выведены, иначе GB_REGIME_MAX
	bool setRegime(eGB_REGIME regime)
	{
		bool stat = false;
		if ( (regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX) )
		{
			stat = true;
		}
		else
			regime = GB_REGIME_MAX;
		regime_ = regime;
		return stat;
	}
	eGB_REGIME getRegime() 	const { return regime_; }

	//состояние
	bool setState(uint8_t state)
	{
		bool stat = false;
		if (state < MAX_NUM_DEVICE_STATE)
		{
			state_ = state;
			stat = true;
		}
		else
			state = MAX_NUM_DEVICE_STATE;

		return stat;
	}
	uint8_t getState()		const { return state_; }

	uint8_t getDopByte() 	const { return dopByte_; }
	bool setDopByte(uint8_t byte) { dopByte_ = byte; return true; }

	// работа с флагом наличия устройства
	// возвращает true если новое значение отличается от текущего
	bool setEnable(bool enable)
	{
		bool stat = false;
		if (enable_ != enable)
		{
			enable_ = enable;
			stat = true;
		}
		return stat;
	}
	bool isEnable()	const { return enable_; }

	// массивы расшифровок аварий и предупреждений
	PGM_P faultText[16];
	PGM_P warningText[8];
	PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
	PGM_P name;

private:
	// текущая приоритетная неисправность, неисправности и кол-во неисправностей
	uint8_t fault_;
	uint16_t faults_;
	uint8_t numFaults_;

	// текущее приоритетное предупреждение, предупреждения и кол-во предупрежд.
	uint8_t warning_;
	uint16_t warnings_;
	uint8_t numWarnings_;


	eGB_REGIME regime_;
	uint8_t state_;
	uint8_t dopByte_;

	// True - означает наличие данного устройства в текущем исполнении АВАНТа
	bool enable_;

	// возвращает кол-во установленных в 1 бит
	uint8_t getNumSetBits(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
				cnt++;
		}
		return cnt;
	}

	// возвращает номер младшего установленного бита
	uint8_t getFirstSetBit(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
				break;
			cnt++;
		}
		return cnt;
	}
};

/// класс для общих параметров и настроек
class TDeviceGlb
{
public:
	TDeviceGlb()
	{
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeLine_ = GB_TYPE_LINE_MAX;
		compatibility_ = GB_COMPATIBILITY_MAX;
		versBsp_ = 0;
		versDsp_ = 0;

		timeSinchr_ = false;
		deviceNum_ = GLB_DEV_NUM_MIN_F;
		outCheck_ = false;
		cfThreshold_ = GLB_CF_THRESH_MIN_F;
		timeRerun_ = GLB_T_RERUN_MIN_F;
		comPrdKeep_ = false;
		comPrmKeep_ = false;
		inDecrease_ = GLB_IN_DEC_MIN_F;
	}

	TDeviceStatus status;

	eGB_NUM_DEVICES getNumDevices() const { return numDevices_; }
	uint8_t getMaxNumDevices() const
	{
		return (numDevices_ ==  GB_NUM_DEVICES_2) ? 2 : 3;
	}
	bool setNumDevices(eGB_NUM_DEVICES numDevices)
	{
		bool stat = false;
		if ( (numDevices >= GB_NUM_DEVICES_MIN) &&
				(numDevices < GB_NUM_DEVICES_MAX) )
		{
			numDevices_ = numDevices;
			stat = true;
		}
		else
			numDevices_ = GB_NUM_DEVICES_MAX;
		return stat;
	}

	eGB_TYPE_LINE getTypeLine() const { return typeLine_; }
	bool setTypeLine(eGB_TYPE_LINE typeLine)
	{
		bool stat = false;

		if ( (typeLine >= GB_TYPE_LINE_MIN) && (typeLine <= GB_TYPE_LINE_MAX) )
		{
			typeLine_ = typeLine;
			stat = true;
		}
		else
			typeLine_ = GB_TYPE_LINE_MAX;
		return stat;
	}

	// версия прошивки AtMega BSP
	uint16_t getVersBsp() const { return versBsp_; }
	void setVersBsp(uint16_t versBsp) { versBsp_ = versBsp; }

	//  версия прошивки DSP
	uint16_t getVersDsp() const { return versDsp_; }
	void setVersDsp(uint16_t versDsp) {  versDsp_ = versDsp; }

	eGB_COMPATIBILITY getCompatibility() const { return compatibility_; }
	bool setCompatibility(eGB_COMPATIBILITY compatibility)
	{
		bool stat = false;
		if ( (compatibility >= GB_COMPATIBILITY_MIN) &&
				(compatibility <= GB_COMPATIBILITY_MAX) )
		{
			compatibility_ = compatibility;
			stat = true;
		}
		else
			compatibility_ = GB_COMPATIBILITY_MAX;
		return stat;
	}

	// синхронизация часов
	// True - включена, False - выключена
	bool setTimeSinchr(uint8_t val)
	{
		bool stat = false;
		if (val <= 1)
		{
			timeSinchr_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTimeSinchr() const { return timeSinchr_; }

	// номер аппарата
	bool setDeviceNum(uint8_t val)
	{
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;
		if ( (val >= GLB_DEV_NUM_MIN_F) && (val <= getMaxNumDevices()) )
		{
			deviceNum_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDeviceNum() const { return (deviceNum_ * GLB_DEV_NUM_FRACT); }

	// контроль выходного сигнала
	bool setOutCheck(uint8_t val)
	{
		bool stat = false;
		if (val <= 1)
		{
			outCheck_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getOutCheck() const { return outCheck_; }

	// порог предупреждения по КЧ
	bool setCfThreshold(uint8_t val)
	{
		bool stat = false;
		val = (val / GLB_CF_THRESH_DISC_F) * GLB_CF_THRESH_DISC_F;
		if ( (val >= GLB_CF_THRESH_MIN_F) && (val <= GLB_CF_THRESH_MAX_F) )
		{
			cfThreshold_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getCfThreshold() const 
	{
		return (cfThreshold_ * GLB_CF_THRESH_FRACT); 
	}

	// время перезапуска
	bool setTimeRerun(uint8_t val)
	{
		bool stat = false;
		val = (val / GLB_T_RERUN_DISC_F) * GLB_T_RERUN_DISC_F;
		if ( (val >= GLB_T_RERUN_MIN_F) && (val <= GLB_T_RERUN_MAX_F) )
		{
			timeRerun_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getTimeRerun() const { return (timeRerun_ * GLB_T_RERUN_FRACT); }

	// Удержание реле команд ПРД
	bool setComPrdKeep(uint8_t val)
	{
		bool stat = false;
		if (val <= 1)
		{
			comPrdKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrdKeep() const { return comPrdKeep_; }

	// Удержание реле команд ПРМ
	bool setComPrmKeep(uint8_t val)
	{
		bool stat = false;
		if (val <= 1)
		{
			comPrmKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrmKeep() const { return comPrmKeep_; }

	// уменьшение усиления входного сигнала
	bool setInDecrease(uint8_t val)
	{
		bool stat = false;
		val = (val / GLB_IN_DEC_DISC_F) * GLB_IN_DEC_DISC_F;
		if ( (val >= GLB_IN_DEC_MIN_F) && (val <= GLB_IN_DEC_MAX_F) )
		{
			inDecrease_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getInDecrease() const { return (inDecrease_ * GLB_IN_DEC_FRACT); }

private:
	// кол-во аппаратов в линии 2 или 3
	eGB_NUM_DEVICES numDevices_;

	// тип линии
	eGB_TYPE_LINE typeLine_;

	// версия прошивки БСП
	uint16_t versBsp_;

	// версия прошивки DSP
	uint16_t versDsp_;

	// совместимость
	eGB_COMPATIBILITY compatibility_;

	// синхронизация часов
	bool timeSinchr_;

	// номер аппарата
	uint8_t deviceNum_;

	// контроль выходного сигнала
	bool outCheck_;

	// порог предупреждения по КЧ
	uint8_t cfThreshold_;

	// время перезапуска
	uint8_t timeRerun_;

	// удержание реле команд Передатчика
	bool comPrdKeep_;

	// удержание реле команд Передатчика
	bool comPrmKeep_;

	// уменьшение усиления входного сигнала
	uint8_t inDecrease_;
};


/// класс для параметров и настроек защиты
class TDeviceDef
{
public:
	TDeviceDef()
	{
		defType_ = DEF_TYPE_MIN;
		numDevices_ = GB_NUM_DEVICES_MAX;
		timeNoMan_ = DEF_T_NO_MAN_MIN_F;
		overlap_ = DEF_OVERLAP_MIN_F;
		delay_ = DEF_DELAY_MIN_F;
		rzThreshold_ = DEF_RZ_THRESH_MIN_F;
		rzDec_ = DEF_RZ_DEC_MIN_F;
		prmType_ = DEF_PRM_TYPE_MIN;
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	// тип защиты
	bool setDefType(uint8_t val)
	{
		bool stat = false;
		if ( (val >= DEF_TYPE_MIN) && (val < DEF_TYPE_MAX) )
		{
			defType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDefType() const { return defType_ ; }

	// тип линии
	bool setNumDevices(eGB_NUM_DEVICES val)
	{
		bool stat = false;
		if ( (val >= GB_NUM_DEVICES_MIN) && (val <= GB_NUM_DEVICES_MAX) )
		{
			stat = true;
		}
		else
			val = GB_NUM_DEVICES_MAX;
		numDevices_ = val;
		return stat;
	}
	uint8_t getNumDevices() const { return numDevices_; }

	// допустимое время без манипуляции
	bool setTimeNoMan(uint8_t val)
	{
		bool stat = false;
		val = (val / DEF_T_NO_MAN_DISC_F) * DEF_T_NO_MAN_DISC_F;
		if ( (val >= DEF_T_NO_MAN_MIN_F) && (val <= DEF_T_NO_MAN_MAX_F) )
		{
			timeNoMan_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getTimeNoMan() const { return (timeNoMan_ * DEF_T_NO_MAN_FRACT); }

	// перекрытие импульсов
	bool setOverlap(uint8_t val)
	{
		bool stat = false;
		val = (val / DEF_OVERLAP_DISC_F) * DEF_OVERLAP_DISC_F;
		if ( (val >= DEF_OVERLAP_MIN_F) && (val <= DEF_OVERLAP_MAX_F) )
		{
			overlap_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getOverlap() const { return (overlap_ * DEF_OVERLAP_FRACT); }

	// компенсация задержки на линии
	bool setDelay(uint8_t val)
	{
		bool stat = false;
		val = (val / DEF_DELAY_DISC_F) * DEF_DELAY_DISC_F;
		if ( (val >= DEF_DELAY_MIN_F) && (val <= DEF_DELAY_MAX_F) )
		{
			delay_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDelay() const { return (delay_ * DEF_DELAY_FRACT); }

	// порог предуреждения по РЗ
	bool setRzThreshold(uint8_t val)
	{
		bool stat = false;
		val = (val / DEF_RZ_THRESH_DISC_F) * DEF_RZ_THRESH_DISC_F;
		if ( (val >= DEF_RZ_THRESH_MIN_F) && (val <= DEF_RZ_THRESH_MAX_F) )
		{
			rzThreshold_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getRzThreshold() const 
	{ 
		return (rzThreshold_ * DEF_RZ_THRESH_FRACT); 
	}

	// загрубление чувствительности по РЗ
	bool setRzDec(uint8_t val)
	{
		bool stat = false;
		val = (val / DEF_RZ_DEC_DISC_F) * DEF_RZ_DEC_DISC_F;
		if ( (val >= DEF_RZ_DEC_MIN_F) && (val <= DEF_RZ_DEC_MAX_F) )
		{
			rzDec_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getRzDec() const { return (rzDec_ * DEF_RZ_DEC_FRACT); }

	// тип приемника
	bool setPrmType(uint8_t val)
	{
		bool stat = false;
		if ( (val >= DEF_PRM_TYPE_MIN) && (val < DEF_PRM_TYPE_MAX) )
		{
			prmType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getPrmType() const { return prmType_; }

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_ )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const { return numJrnEntry_; }

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max)
	{
		bool stat = false;
		if (max <= 1024)
		{
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const { return maxNumJrnEntry_; }

private:
	// тип защиты
	uint8_t defType_;

	// тип линии (кол-во аппаратов)
	eGB_NUM_DEVICES numDevices_;

	// допустимое время без манипуляции
	uint8_t timeNoMan_;

	// перекрытие импульсов
	uint8_t overlap_;

	// компенсация задержки в линии
	uint8_t delay_;

	// порог предупреждения по РЗ
	uint8_t rzThreshold_;

	// загрубление чувствительности по РЗ
	uint8_t rzDec_;

	// тип приемника
	uint8_t prmType_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};


/// класс для параметров и настроек приемника
class TDevicePrm
{
public:
	TDevicePrm()
	{
		numCom_ = 0;
		timeOn_ = PRM_TIME_ON_MIN_F;

		for(uint_fast8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++)
		{
			blockCom_[i] = false;
			timeOff_[i] = PRM_TIME_OFF_MIN_F;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	// установка кол-ва команд в ПРМ, если оно равно 0 или больше 32
	// возвращает true если новое значение отличается от предыдущего, а также
	// устанавливает флаг enable
	bool setNumCom(uint8_t numCom)
	{
		bool stat = false;
		if ( (numCom <= MAX_NUM_COM_PRM) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			stat = true;
		}
		return stat;
	}
	uint8_t getNumCom() const { return numCom_; }

	// время включения команды
	bool setTimeOn(uint8_t val)
	{
		bool stat = false;
		val = (val / PRM_TIME_ON_DISC_F) * PRM_TIME_ON_DISC_F;
		if ( (val >= PRM_TIME_ON_MIN_F) && (val <= PRM_TIME_ON_MAX_F) )
		{
			timeOn_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getTimeOn() const { return (timeOn_ * PRM_TIME_ON_FRACT); }

	// блокированные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom8(uint8_t num, uint8_t val)
	{
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8))
		{
			blockCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) const
	{
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return  (blockCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getBlockCom8(uint8_t num) const
	{
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRM / 8))
			val = blockCom_[num];
		return val;
	}

	// задержка на выключение
	// buf - адрес первого элемента массива
	// значение передается в 10 раз меньше реального !!
	bool setTimeOff(uint8_t *buf)
	{
		bool stat = true;
		uint16_t val;
		for(uint_fast8_t i = 0; i < numCom_; i++)
		{
			val = ((*buf)  / PRM_TIME_OFF_DISC_F) * PRM_TIME_OFF_DISC_F;
			if ( (val >= PRM_TIME_OFF_MIN_F) && (val <= PRM_TIME_OFF_MAX_F) )
				timeOff_[i] = val;
			else
				stat = false;
			buf++;
		}
		return stat;
	}
	uint16_t getTimeOff(uint8_t num) const
	{
		return timeOff_[num]*PRM_TIME_OFF_FRACT;
	}

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_ )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const { return numJrnEntry_; }

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max)
	{
		bool stat = false;
		if (max <= 1024)
		{
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const { return maxNumJrnEntry_; }

private:
	// кол-во команд приемника
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	uint8_t blockCom_[MAX_NUM_COM_PRM / 8];

	// задержка на выключение
	uint8_t timeOff_[MAX_NUM_COM_PRM];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};


/// класс для параметров и настроек передатчика
class TDevicePrd
{
public:
	TDevicePrd()
	{
		numCom_ = 0;
		timeOn_ = PRD_TIME_ON_MIN_F;
		for(uint_fast8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++)
		{
			blockCom_[i] = false;
			longCom_[i] = false;
		}
		duration_ = PRD_DURATION_MIN_F;
		testCom_ = false;

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	// установка кол-ва команд в ПРМ, если оно равно 0 или больше 32
	// возвращает true если новое значение отличается от предыдущего, а также
	// устанавливает флаг enable
	bool setNumCom(uint8_t numCom)
	{
		bool stat = false;
		if ( (numCom <= MAX_NUM_COM_PRD) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			stat = true;
		}
		return stat;
	}
	uint8_t getNumCom() const { return numCom_; }

	// время включения команды
	bool setTimeOn(uint8_t val)
	{
		bool stat = false;
		val = (val / PRD_TIME_ON_DISC_F) * PRD_TIME_ON_DISC_F;
		if ( (val >= PRD_TIME_ON_MIN_F) && (val <= PRD_TIME_ON_MAX_F) )
		{
			timeOn_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getTimeOn() const { return timeOn_ * PRD_TIME_ON_FRACT; }

	// блокированные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom8(uint8_t num, uint8_t val)
	{
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8))
		{
			blockCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) const
	{
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return  (blockCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getBlockCom8(uint8_t num) const
	{
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = blockCom_[num];
		return val;
	}

	// длительные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setLongCom8(uint8_t num, uint8_t val)
	{
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8))
		{
			longCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getLongCom(uint8_t num) const
	{
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return  (longCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getLongCom8(uint8_t num) const
	{
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = longCom_[num];
		return val;
	}

	// тестовая команда
	// True - включена, False - выключена
	bool setTestCom(uint8_t val)
	{
		bool stat = false;
		if (val <= 1)
		{
			testCom_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTestCom() const { return testCom_; }

	// время включения команды
	bool setDuration(uint8_t val)
	{
		bool stat = false;
		val = (val / PRD_DURATION_DISC_F) * PRD_DURATION_DISC_F;
		if ( (val >= PRD_DURATION_MIN_F) && (val <= PRD_DURATION_MAX_F) )
		{
			duration_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDuration() const { return duration_ * PRD_TIME_ON_FRACT; }

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_ )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const { return numJrnEntry_; }

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max)
	{
		bool stat = false;
		if (max <= 1024)
		{
			maxNumJrnEntry_ = max;
			stat = true;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const { return maxNumJrnEntry_; }

private:
	// текущее кол-во команд
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	uint8_t blockCom_[MAX_NUM_COM_PRD / 8];

	// длительные команды, true - длительная
	uint8_t longCom_[MAX_NUM_COM_PRD / 8];

	// тестовая команда. true - вкл.
	bool testCom_;

	// длительность команды
	uint8_t duration_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// макстмалбное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;

};

/// класс для измеряемых параметров
class TMeasuredParameters
{
public:
	TMeasuredParameters()
	{
		voltDef_ = 0;
		voltCf_ = 0;
		voltNoise_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
	}

	// запас по защите
	int8_t getVoltageDef() const { return voltDef_; }
	bool setVoltageDef(int8_t voltDef)
	{
		bool stat = false;
		if ( (voltDef > - 100) && (voltDef < 100) )
		{
			voltDef_ = voltDef;
			stat = true;
		}
		return stat;

	}

	// запас по КЧ
	int8_t getVoltageCf() const { return voltCf_; }
	bool setVoltageCf(int8_t voltCf)
	{
		bool stat = false;
		if ( (voltCf > -100) && (voltCf < 100) )
		{
			voltCf_ = voltCf;
			stat = true;
		}
		return stat;

	}

	// напряжение выхода
	uint8_t getVoltageOutInt() const { return (voltOut_ / 10); }
	uint8_t getVoltageOutFract() const { return (voltOut_ % 10); }
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract)
	{
		bool stat = false;
		if ( (voltOutInt < 100) && (voltOutFract < 10) )
		{
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			stat = true;
		}
		return stat;
	}

	// ток выхода
	uint16_t getCurrentOut() const { return curOut_; }
	bool setCurrentOut(uint16_t curOut)
	{
		bool stat = false;
		if (curOut < 999)
		{
			curOut_ = curOut;
			stat = true;
		}
		return stat;
	}

	// сопротивление выхода
	uint16_t getResistOut() const { return resistOut_; }
	bool setResistOut(uint16_t resistOut)
	{
		bool stat = false;
		if (resistOut < 999)
		{
			resistOut_ = resistOut;
			stat = true;
		}
		return stat;
	}

	// уровень шума
	int8_t getVoltageNoise() const { return voltNoise_; }
	bool setVoltageNoise(int8_t voltNoise)
	{
		bool stat = false;
		if ( (voltNoise > -100) && (voltNoise < 100) )
		{
			voltNoise_ = voltNoise;
			stat = true;
		}
		return stat;
	}
private:
	// запас по защите (-99 .. 99)дБ
	int8_t voltDef_;
	// запас по КЧ (-99 .. 99)дБ
	int8_t voltCf_;
	// уровень шумов (-99 .. 99)дБ
	int8_t voltNoise_;
	// выходное напряжение умноженное на 10 (0 .. 999)В
	uint16_t voltOut_;
	// выходной ток (0 .. 999)мА
	uint16_t curOut_;
	// выходное сопротивление (0 .. 999)Ом
	uint16_t resistOut_;
};

// класс для передачи команд
class TTxCom
{
public:
	TTxCom()
	{
		clear();
	}

	// очистка буфера
	void clear()
	{
		numCom_ = 0;
		cnt_ = 0;
		comFast_ = GB_COM_NO;
		com_[0] = GB_COM_NO;
	}

	// добавление команды в буфер
	bool addCom(eGB_COM com)
	{
		bool stat = false;
		if (numCom_ < MAX_NUM_COM_BUF)
		{
			com_[numCom_++] = com;
			stat = true;
		}
		return stat;
	}

	// последовательная выдача имеющихся в буфере команд
	eGB_COM getCom()
	{
		if (cnt_ >= numCom_)
			cnt_ = 0;
		return com_[cnt_++];
	}

	// срочная команда (например изменение параметра)
	void addFastCom(eGB_COM com) { comFast_ = com; }
	eGB_COM getFastCom()
	{
		eGB_COM com = GB_COM_NO;
		if (comFast_ != GB_COM_NO)
		{
			com = comFast_;
			comFast_ = GB_COM_NO;
		}
		return com;
	}

	// возвращает кол-во команд в буфере
	uint8_t getNumCom() const { return numCom_; }

	// запись\считывание одного байта на передачу
	// byte байт данных
	// num номер байта в буфере
	void 	setInt8(uint8_t byte, uint8_t num = 0)
	{
		if (num < 6)
			byte_[num] = byte;
	}
	uint8_t getInt8(uint8_t num = 0) const
	{
		uint8_t byte = 0;
		if (num < 6)
			byte = byte_[num];
		return byte;
	}

	// запись\считывание переменной типа INT
	// хранится в byte_[1]
	void setInt16(uint16_t val) { *((uint16_t *) (byte_ + 1)) = val; }
	uint16_t getInt16() const { return *((uint16_t *) (byte_ + 1)); }

	// запись нескольких байт данных
	// source - указатель на массив данных
	// num - кол-во копируемых байт данных
	void copyToBufer(uint8_t *source, uint8_t num)
	{
		for(uint_fast8_t i = 0; i < num; i++)
		{
			// учет размера буфера
			if (i >= 6)
				break;
			byte_[i] = *source++;

		}
	}
	// считывание нескольких байт данных
	// destination- указатель на массив данных
	// num - кол-во копируемых байт данных
	void copyFromBufer(uint8_t *destination, uint8_t num) const
	{
		for(uint_fast8_t i = 0; i < num; i++)
		{
			// учет размера буфера
			if (i >= 6)
				break;
			*destination++ = byte_[i];
		}
	}
	// возвразает указатель на буфер данных
	uint8_t* getBuferAddress() { return byte_; }

private:
	// буфер команд
	eGB_COM com_[MAX_NUM_COM_BUF];
	// срочная команда (на изменение)
	eGB_COM comFast_;
	// кол-во команд в буфере
	uint8_t numCom_;
	// текущее положение указателя
	uint8_t cnt_;
	// данные на передачу
	uint8_t byte_[6];
};

class TJournalEntry
{
public:
	TJournalEntry()
	{
		clear();
	}

	void clear()
	{
		currentDevice_ = GB_DEVICE_MAX;
		deviceJrn_ = GB_DEVICE_MAX;
		eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
		regime_ = GB_REGIME_MAX;
		numCom_ = 0;

		numJrnEntries_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		addressFirstEntry_ = 0;

		currentEntry_ = 1;
		ready_ = false;
	}

	TDataTime dataTime;

	bool setCurrentDevice(eGB_DEVICE device)
	{
		bool stat = false;
		if ( (device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX) )
		{
			currentDevice_ = device;
			stat = true;
		}
		else
			currentDevice_ = GB_DEVICE_MAX;
		return stat;
	}
	eGB_DEVICE getCurrentDevice() const { return currentDevice_; }

	// запись\считывание устройства для которого сделана запись
	bool setDeviceJrn(eGB_DEVICE device)
	{
		bool stat = false;
		if ( (device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX) )
		{
			deviceJrn_ = device;
			stat = true;
		}
		else
			deviceJrn_ = GB_DEVICE_MAX;
		return stat;
	}
	eGB_DEVICE getDeviceJrn() const { return deviceJrn_; }

	// тип события
	bool setEventType(uint8_t val)
	{
		bool state = false;

		uint8_t min = 255;
		uint8_t max = 0;
		// установка мин/макс значения события, в зависимости от
		// текущего журнала
		if (currentDevice_ == GB_DEVICE_GLB)
		{
			min = MIN_JRN_EVENT_VALUE;
			max = MAX_JRN_EVENT_VALUE;
		}
		else if (currentDevice_ == GB_DEVICE_PRD)
		{
			min = GB_STATE_COM_MIN;
			max = GB_STATE_COM_MAX;
		}

		if ( (val >= min) && (val <= max) )
		{
			eventType_ = val;
			state = true;
		}
		else
			eventType_ = max;

		return state;
	}
	uint8_t getEventType() const { return eventType_; }

	// режим
	bool setRegime(eGB_REGIME regime)
	{
		bool state = false;
		if ( (regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX) )
		{
			regime_ = regime;
			state = true;
		}
		else
			regime_ = GB_REGIME_MAX;
		return state;
	}
	eGB_REGIME getRegime() const { return regime_; }

	// номер команды
	bool setNumCom(uint8_t num)
	{
		bool stat = false;
		if ( (num > 0) && (num <= MAX_NUM_COM_PRD) )
		{
			numCom_ = num;
			stat = true;
		}
		return stat;
	}
	uint8_t getNumCom() const { return numCom_; }

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;

		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_)
		{
			if (overflow_)
			{
				numJrnEntries_ = maxNumJrnEntry_;
				addressFirstEntry_ = val;
			}
			else
			{
				numJrnEntries_ = val;
				addressFirstEntry_ = 0;
			}
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntries() const { return numJrnEntries_; }

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntries(uint16_t max)
	{
		bool stat = false;
		if (max <= GLB_JRN_MAX)
		{
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const { return maxNumJrnEntry_; }

	// переполнение журнала
	bool isOverflow() const { return overflow_; }

	// номер адреса текущей записи в журнале
	uint16_t getEntryAdress() const
	{
		return (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
	}

	// текущая запись
	uint16_t getCurrentEntry() const { return currentEntry_; }
	// следующая/предыдущая запись возвращает true если новое значение
	// отличается от предыдущего
	bool setNextEntry()
	{
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
		if (tmp != currentEntry_)
		{
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}
	bool setPreviousEntry()
	{
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
		if (tmp != currentEntry_)
		{
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}

	// утстановка и считывание флага получения информации о текущей записи
	bool setReady() { return (ready_ = true); }
	bool isReady() const { return ready_; }

private:
	// текущий журнал
	eGB_DEVICE currentDevice_;

	// устройство
	eGB_DEVICE deviceJrn_;

	// тип события
	uint8_t eventType_;

	// режим
	eGB_REGIME regime_;

	// кол-во записей в журнале
	uint16_t numJrnEntries_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// переполнение журнала
	bool overflow_;

	// адрес первой записи
	uint16_t addressFirstEntry_;

	// адрес текущей записи (отображаемой на экране)
	uint16_t currentEntry_;

	// номер команды в текущей записи
	uint16_t numCom_;

	// флаг получения информации о текущей записи
	bool ready_;
};

class TTest
{
public:
	TTest()
	{
		clear();
	}
	// очистка списка сигналов
	void clear()
	{
		signalList[0] =  GB_SIGNAL_NO;
		num_ = 1;
		currentSignal_ = GB_SIGNAL_NO;
	}

	bool addSignalToList(eGB_TEST_SIGNAL signal)
	{
		bool stat = false;
		if ( (num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX) )
		{
			signalList[num_++] = signal;
			stat = true;
		}
		return stat;
	}

	// записывает в cf, rz значения КЧ и РЗ, соответственно, для передачи в БСП
	void getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig)
	{
		if ( (sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32) )
		{
			rz = 0;
			cf =  3 + sig - GB_SIGNAL_COM1; // 3 - кол-во кч ?!
		}
		else if ( (sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4) )
		{
			rz = 0;
			cf = 1 + sig - GB_SIGNAL_CF1;
		}
		else if (sig == GB_SIGNAL_CF_NO_RZ)
		{
			rz = 1;
			cf = 1;
		}
		else if (sig == GB_SIGNAL_CF_RZ)
		{
			rz = 2;
			cf = 1;
		}
		else if (sig == GB_SIGNAL_RZ)
		{
			rz = 1;
			cf = 0;
		}
		else if ( (sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM4_RZ) )
		{
			rz = 2;
			cf = 4 + sig - GB_SIGNAL_COM1_RZ;
		}
		else if ( sig == GB_SIGNAL_CF)
		{
			rz = 0;
			cf = 1;
		}
		else
		{
			rz = 0;
			cf = 0;
		}
	}

	// возвращает кол-во сигналов в списке
	uint8_t getNumSignals() const { return num_; }

	// установка текущего сигнала
	void setCurrentSignal(uint8_t *s, eGB_TYPE_DEVICE type)
	{
		eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;

		if (type == AVANT_K400)
		{
			signal = getCurrentSignalK400(s);
		}
		else if (type == AVANT_RZSK)
		{
			signal = getCurrentSignalRZSK(s);
		}
		currentSignal_ = signal;
	}
	eGB_TEST_SIGNAL getCurrentSignal() const { return currentSignal_; }

	// список сигналов
	uint8_t signalList[MAX_NUM_TEST_SIGNAL];

private:
	// кол-во возможных сигналов в тесте
	uint8_t num_;

	// текущий сигнал
	eGB_TEST_SIGNAL currentSignal_;

	// возвращает номер первого установленного бита 1..8, либо 0
	// проверка начинается с 0-ого бита
	uint8_t getSetBit(uint8_t byte)
	{
		uint8_t bit = 0;

		if (byte)
		{
			uint8_t b = 0;
			for(uint_fast8_t i = 1; i > 0; i <<= 1)
			{
				b++;
				if (byte & i)
				{
					bit = b;
					break;
				}
			}
		}
		return bit;
	}

	// добавление сигнала в список для К400
	eGB_TEST_SIGNAL getCurrentSignalK400(uint8_t *s)
	{
		eGB_TEST_SIGNAL signal = GB_SIGNAL_NO;

		uint8_t t = *s;
		// сначала проверяется наличие КЧ1-КЧ2
		if (t & 0x01 )
			signal = GB_SIGNAL_CF1;
		else if (t & 0x02)
			signal = GB_SIGNAL_CF2;
		else
		{
			// проверяется начичие команд с 1 по 8
			t = getSetBit(*(++s));
			if (t)
			{
				t =  GB_SIGNAL_COM1 + t - 1;
				signal = (eGB_TEST_SIGNAL) t;
			}
			else
			{
				// проверяется начичие команд с 9 по 16
				t = getSetBit(*(++s));
				if (t)
				{
					t =  GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				}
				else
				{
					// проверяется начичие команд с 17 по 24
					t = getSetBit(*(++s));
					if (t)
					{
						t =  GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					}
					else
					{
						// проверяется начичие команд с 25 по 32
						t = getSetBit(*(++s));
						if (t)
						{
							t =  GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	};

	// добавление сигнала в список для РЗСК
	eGB_TEST_SIGNAL getCurrentSignalRZSK(uint8_t *s)
	{
		eGB_TEST_SIGNAL signal = GB_SIGNAL_NO;

		uint8_t t = *s;
		if ( (t & 0x11) == 0x11)
			signal = GB_SIGNAL_CF_NO_RZ;
		else if ( (t & 0x12) == 0x12)
			signal = GB_SIGNAL_CF_RZ;
		else
		{
			if (t & 0x10)
			{
				t = getSetBit((*(++s)) & 0x0F);
				if (t == 0)
					signal = GB_SIGNAL_CF;
				else
				{
					t = t + GB_SIGNAL_COM1 - 1;
					signal = (eGB_TEST_SIGNAL) t;
				}
			}
			else if (t & 0x20)
			{
				t = getSetBit((*(++s)) & 0x0F);
				if (t)
				{
					t = t + GB_SIGNAL_COM1_RZ - 1;
					signal = (eGB_TEST_SIGNAL) t;
				}
			}
		}
		return signal;
	};
};

/// Структура параметров БСП
struct stGBparam
{
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;
	eGB_TYPE_DEVICE typeDevice;

	// пароль
	TPassword password;

	// дата/время
	TDataTime dataTime;

	// измеряемые параметры
	TMeasuredParameters measParam;

	// текущее состояние устройств
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;

	// тесты
	TTest test;
};

#endif /* GLBDEF_H_ */
