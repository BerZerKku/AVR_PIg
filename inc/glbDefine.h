/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "debug.h"

/// версия текущей прошивки
#define VERS 0x0100

/// максимально кол-во команд на прием
#define MAX_NUM_COM_PRM 32

/// максимальное кол-во команд на передачу
#define MAX_NUM_COM_PRD 32

/// преобразование двух CHAR в INT
#define TO_INT16(high, low) (((uint16_t) high << 8) + low)

/// максимально возможное кол-во состояний устройств
#define MAX_NUM_DEVICE_STATE 11

/// максимально возможное кол-во режимов устройств
#define MAX_NUM_REGIME 6

/// максимальное кол-во команд в буфере (не считая 3-х основных)
#define MAX_NUM_COM_BUF 15

/// максимальное и минимальный код типа событий в журнале событий
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 32

/// Минимальные, максимальные значения параметров приемника и их дискретность
/// ЗАЩИТА
/// тип защиты
#define DEF_TYPE_MIN		0
#define DEF_TYPE_MAX		7
/// тип линии
#define DEF_LINE_TYPE_MIN 	1
#define DEF_LINE_TYPE_MAX 	2
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
#define DEF_PRM_TYPE_MAX	2
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
/// время включения
#define PRD_TIME_ON_MIN		0
#define PRD_TIME_ON_MAX		10
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

/// кол-во записей в журнале событый
#define GLB_JRN_EVENT_K400_MAX 512

/// кол-во записей в журнале защиты
#define GLB_JRN_DEF_K400_MAX 0

/// кол-во записей в журнале приемника
#define GLB_JRN_PRM_K400_MAX 512

/// кол-во записей в журнале передатчика
#define GLB_JRN_PRD_K400_MAX 512


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
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTIC = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// Кол-во аппаратов в линии
enum eGB_NUM_DEVICES
{
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// Совместимость
enum eGB_COMPATIBILITY
{
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK = 2,
	GB_COMPATIBILITY_PVZUE = 3,
	GB_COMPATIBILITY_MAX
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
	GB_COM_PRD_GET_TEST_COM = 0,	// !!! пока только в составе МЕГА команды
	GB_COM_PRD_GET_BLOCK_COM= 0x24,
	GB_COM_PRD_GET_LONG_COM = 0x25,
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
	GB_COM_GET_VERS			= 0x3F,
	GB_COM_PRM_ENTER		= 0x51,
	GB_COM_SET_CONTROL		= 0x72,
	GB_COM_SET_PASSWORD 	= 0x73,	// ! только с ПК
	GB_COM_GET_PASSWORD 	= 0x74,	// ! только с ПК
	GB_COM_PRM_RES_IND		= 0x9A,
	GB_COM_SET_TIME 		= 0xB2,	// !!! не сделана
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
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// группа команды
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
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


/// Класс для даты и времени
class TDataTime
{
public:
	// Считывание и установка секунд
	uint8_t getSecond() const { return second_; }
	bool setSecondFromBCD(uint8_t val) { return setSecond(bcdToBin(val)); }
	bool setSecond(uint8_t val)
	{
		bool stat = false;
		if (val < 60)
		{
			second_ = val;
			stat = true;
		}
		return stat;
	}

	// считывание и установка минут
	uint8_t getMinute() const { return minute_; }
	bool setMinuteFromBCD(uint8_t val) { return setMinute(bcdToBin(val)); }
	bool setMinute(uint8_t val)
	{
		bool stat = false;
		if (val < 60)
		{
			minute_ = val;
			stat = true;
		}
		return stat;
	}

	// считывание и установка часов
	uint8_t getHour() const { return hour_; }
	bool setHourFromBCD(uint8_t val) { return setHour(bcdToBin(val)); }
	bool setHour(uint8_t val)
	{
		bool stat = false;
		if (val < 24)
		{
			hour_ = val;
			stat = true;
		}
		return stat;
	}

	// считывание и установка дня
	uint8_t getDay() const { return day_; }
	bool setDayFromBCD(uint8_t val) { return setDay(bcdToBin(val)); }
	bool setDay(uint8_t val)
	{
		bool stat = false;

		if (val > 0)
		{
			if ( (month_ == 4) || (month_ == 6) || (month_ == 9) ||
					 	 	 	 	 	 	 	 	 	 	 (month_ == 11) )
			{
				if (val < 31)
				{
					day_ = val;
					stat = true;
				}
			}
			else if (month_ == 2)
			{
				if (val <= 28)
				{

				}
				else if ( ((val <= 29)) && ((year_ % 4) == 0) )
				{
					day_ = val;
					stat = true;
				}
			}
			else
			{
				if (val <= 31)
				{
					day_ = val;
					stat = true;
				}
			}
		}
		return stat;
	}

	// считывание и установка месяца
	uint8_t getMonth() const { return month_; }
	bool setMonthFromBCD(uint8_t val) { return setMonth(bcdToBin(val)); }
	bool setMonth(uint8_t val)
	{
		bool stat = false;
		if ( (val > 0) && ( val <= 12) )
		{
			month_ = val;
			stat = true;
		}
		return stat;
	}

	// считывание и установка года
	uint8_t getYear() 	const { return year_; }
	bool setYearFromBCD(uint8_t val) { return setYear(bcdToBin(val)); }
	bool setYear(uint8_t val)
	{
		bool stat = false;
		if (val < 100)
		{
			year_ = val;
			stat = true;
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

	uint8_t bcdToBin(uint8_t val) { return (val >> 4) * 10 + (val & 0x0F); }
};


/// Класс для пароля
class TPassword
{
public:
	uint16_t get() const { return password_; }
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

private:
	uint16_t password_;
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

		regime_ = 0;
		state_ = 0;
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
	bool setRegime(uint8_t regime)
	{
		bool status = false;
		if (regime < MAX_NUM_REGIME)
		{
			regime_ = regime;
			status = true;
		}
		else
			regime = MAX_NUM_REGIME;


		return true;
	}
	uint8_t getRegime() 	const { return regime_; }

	//состояние
	bool setState(uint8_t state)
	{
		bool status = false;
		if (state < MAX_NUM_DEVICE_STATE)
		{
			state_ = state;
			status = true;
		}
		else
			state = MAX_NUM_DEVICE_STATE;

		return status;
	}
	uint8_t getState()		const { return state_; }

	uint8_t getDopByte() 	const { return dopByte_; }
	bool setDopByte(uint8_t byte) { dopByte_ = byte; return true; }

	// работа с флагом наличия устройства
	// возвращает true если новое значение отличается от текущего
	bool setEnable(bool enable)
	{
		bool status = false;
		if (enable_ != enable)
		{
			enable_ = enable;
			status = true;
		}
		return status;
	}
	bool isEnable()	const { return enable_; }

	// массивы расшифровок аварий и предупреждений
	PGM_P faultText[16];
	PGM_P warningText[8];
	PGM_P stateText[MAX_NUM_DEVICE_STATE];
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


	uint8_t regime_;
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
		numDevices_ = GB_NUM_DEVICES_2;
		typeLine_ = GB_TYPE_LINE_UM;
		compatibility_ = GB_COMPATIBILITY_AVANT;
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

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
	}

	TDeviceStatus status;

	// при установке возвращает true если новое значение отличается от текущего
	eGB_NUM_DEVICES getNumDevices() const { return numDevices_; }
	bool setNumDevices(eGB_NUM_DEVICES numDevices)
	{
		bool status = false;
		if ( (numDevices == GB_NUM_DEVICES_2) ||
				(numDevices == GB_NUM_DEVICES_3) )
		{
			numDevices_ = numDevices;
			status = true;
		}
		return status;
	}

	// при установке возвращает true если новое значени отличается от текущего
	eGB_TYPE_LINE getTypeLine() const { return typeLine_; }
	bool setTypeLine(eGB_TYPE_LINE typeLine)
	{
		bool status = false;

		if ( (typeLine == GB_TYPE_LINE_UM) || (typeLine == GB_TYPE_LINE_E1) ||
				(typeLine == GB_TYPE_LINE_OPTIC) )
		{
			typeLine_ = typeLine;
			status = true;
		}
		return status;
	}

	// версия прошивки AtMega BSP
	uint16_t getVersBsp() const { return versBsp_; }
	void setVersBsp(uint16_t versBsp) { versBsp_ = versBsp; }

	//  версия прошивки DSP
	uint16_t getVersDsp() const { return versDsp_; }
	void setVersDsp(uint16_t versDsp) {  versDsp_ = versDsp; }

	// при установке возаращает true если новое значение отличается от текущего
	eGB_COMPATIBILITY getCompatibility() const { return compatibility_; }
	bool setCompatibility(eGB_COMPATIBILITY compatibility)
	{
		bool status = false;
		if ( (compatibility == GB_COMPATIBILITY_AVANT) ||
				(compatibility == GB_COMPATIBILITY_AVZK) ||
				(compatibility == GB_COMPATIBILITY_PVZ90) ||
				(compatibility == GB_COMPATIBILITY_PVZUE) )
		{
			compatibility_ = compatibility;
			status = true;
		}
		return status;
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
	bool getTimeSinchr() { return timeSinchr_; }

	// номер аппарата
	bool setDeviceNum(uint8_t val)
	{
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;
		if ( (val >= GLB_DEV_NUM_MIN_F) && (val <= GLB_DEV_NUM_MAX_F) )
		{
			deviceNum_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDeviceNum() { return (deviceNum_ * GLB_DEV_NUM_FRACT); }

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
	bool getOutCheck() { return outCheck_; }

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
	uint8_t getCfThreshold() { return (cfThreshold_ * GLB_CF_THRESH_FRACT); }

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
	uint8_t getTimeRerun() { return (timeRerun_ * GLB_T_RERUN_FRACT); }

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
	bool getComPrdKeep() { return comPrdKeep_; }

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
	bool getComPrmKeep() { return comPrmKeep_; }

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
	uint8_t getInDecrease() { return (inDecrease_ * GLB_IN_DEC_FRACT); }

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;
		val &= 0x3FFF;
		if (val <= maxNumJrnEntry_ )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() { return numJrnEntry_; }

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
	uint16_t getMaxNumJrnEntry() { return maxNumJrnEntry_; }

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

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;
};


/// класс для параметров и настроек защиты
class TDeviceDef
{
public:
	TDeviceDef()
	{
		defType_ = DEF_TYPE_MIN;
		lineType_ = DEF_LINE_TYPE_MIN;
		timeNoMan_ = DEF_T_NO_MAN_MIN_F;
		overlap_ = DEF_OVERLAP_MIN_F;
		delay_ = DEF_DELAY_MIN_F;
		rzThreshold_ = DEF_RZ_THRESH_MIN_F;
		rzDec_ = DEF_RZ_DEC_MIN_F;
		prmType_ = DEF_PRM_TYPE_MIN;
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
	}

	TDeviceStatus status;

	// тип защиты
	bool setDefType(uint8_t val)
	{
		bool stat = false;
		if ( (val >= DEF_TYPE_MIN) && (val <= DEF_TYPE_MAX) )
		{
			defType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getDefType() { return defType_ ; }

	// тип линии
	bool setLineType(uint8_t val)
	{
		bool stat = false;
		if ( (val >= DEF_LINE_TYPE_MIN) && (val <= DEF_LINE_TYPE_MAX) )
		{
			lineType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getLineType() { return lineType_; }

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
	uint8_t getTimeNoMan() { return (timeNoMan_ * DEF_T_NO_MAN_FRACT); }

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
	uint8_t getOverlap() { return (overlap_ * DEF_OVERLAP_FRACT); }

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
	uint8_t getDelay() { return (delay_ * DEF_DELAY_FRACT); }

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
	uint8_t getRzThreshold() { return (rzThreshold_ * DEF_RZ_THRESH_FRACT); }

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
	uint8_t getRzDec() { return (rzDec_ * DEF_RZ_DEC_FRACT); }

	// тип приемника
	bool setPrmType(uint8_t val)
	{
		bool stat = false;
		if ( (val >= DEF_PRM_TYPE_MIN) && (val <= DEF_PRM_TYPE_MAX) )
		{
			prmType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getPrmType() { return prmType_; }

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;
		if (val <= maxNumJrnEntry_)
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() { return numJrnEntry_; }

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
	uint16_t getMaxNumJrnEntry() { return maxNumJrnEntry_; }

private:
	// тип защиты
	uint8_t defType_;

	// тип линии (кол-во аппаратов)
	uint8_t lineType_;

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
};


/// класс для параметров и настроек приемника
class TDevicePrm
{
public:
	TDevicePrm()
	{
		numCom_ = 0;
		timeOn_ = PRM_TIME_ON_MIN_F;

		for(uint_fast8_t i = 0; i < MAX_NUM_COM_PRM; i++)
		{
			blockCom_[i] = false;
			timeOff_[i] = PRM_TIME_OFF_MIN_F;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
	}

	TDeviceStatus status;

	// установка кол-ва команд в ПРМ, если оно равно 0 или больше 32
	// возвращает true если новое значение отличается от предыдущего, а также
	// устанавливает флаг enable
	bool setNumCom(uint8_t numCom)
	{
		bool status = false;
		if ( (numCom <= MAX_NUM_COM_PRM) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			status = true;
		}
		return status;
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
	uint8_t getTimeOn() { return (timeOn_ * PRM_TIME_ON_FRACT); }

	// блокированные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom(uint8_t num, uint8_t val)
	{
		bool stat = false;

		num *= 8;	// номер первой команды

		if (num < numCom_ )
		{

			for(uint_fast8_t i = 0x01; (i > 0) && (num < numCom_); i <<= 1)
			{
				blockCom_[num++] = (bool) (i & val);
			}
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) { return  blockCom_[num]; }

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
	uint16_t getTimeOff(uint8_t num) {return timeOff_[num]*PRM_TIME_OFF_FRACT;}

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;
		if (val <= 1024 )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() { return numJrnEntry_; }

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
	uint16_t getMaxNumJrnEntry() { return maxNumJrnEntry_; }

private:
	// кол-во команд приемника
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	bool blockCom_[MAX_NUM_COM_PRM];

	// задержка на выключение
	uint8_t timeOff_[MAX_NUM_COM_PRM];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;
};


/// класс для параметров и настроек передатчика
class TDevicePrd
{
public:
	TDevicePrd()
	{
		numCom_ = 0;
		timeOn_ = PRD_TIME_ON_MIN_F;
		for(uint_fast8_t i = 0; i < MAX_NUM_COM_PRD; i++)
		{
			blockCom_[i] = false;
			longCom_[i] = false;
		}
		duration_ = PRD_DURATION_MIN_F;
		testCom_ = false;

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
	}

	TDeviceStatus status;

	// установка кол-ва команд в ПРМ, если оно равно 0 или больше 32
	// возвращает true если новое значение отличается от предыдущего, а также
	// устанавливает флаг enable
	bool setNumCom(uint8_t numCom)
	{
		bool status = false;
		if ( (numCom <= MAX_NUM_COM_PRD) && (numCom_ != numCom) )
		{
			numCom_ = numCom;
			this->status.setEnable(numCom != 0);
			status = true;
		}
		return status;
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
	uint8_t getTimeOn() { return timeOn_ * PRD_TIME_ON_FRACT; }

	// блокированные команды каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom(uint8_t num, uint8_t val)
	{
		bool stat = false;

		num *= 8;	// номер первой команды

		if (num < numCom_ )
		{

			for(uint_fast8_t i = 0x01; (i > 0) && (num < numCom_); i <<= 1)
			{
				blockCom_[num++] = (bool) (i & val);
			}
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) { return  blockCom_[num]; }

	// длительные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setLongCom(uint8_t num, uint8_t val)
	{
		bool stat = false;

		num *= 8;	// номер первой команды

		if (num < numCom_ )
		{

			for(uint_fast8_t i = 0x01; (i > 0) && (num < numCom_); i <<= 1)
			{
				longCom_[num++] = (bool) (i & val);
			}
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getLongCom(uint8_t num) { return  longCom_[num]; }

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
	bool getTestCom() { return testCom_; }

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
	uint8_t getDuration() { return duration_ * PRD_TIME_ON_FRACT; }

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val)
	{
		bool stat = false;
		if (val <= 1024 )
		{
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() { return numJrnEntry_; }

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
	uint16_t getMaxNumJrnEntry() { return maxNumJrnEntry_; }

private:
	// текущее кол-во команд
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	bool blockCom_[MAX_NUM_COM_PRD];

	// длительные команды, true - длительная
	bool longCom_[MAX_NUM_COM_PRD];

	// тестовая команда. true - вкл.
	bool testCom_;

	// длительность команды
	uint8_t duration_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// макстмалбное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

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
		bool state = false;
		if ( (voltDef > - 100) && (voltDef < 100) )
		{
			voltDef_ = voltDef;
			state = true;
		}
		return state;

	}

	// запас по КЧ
	int8_t getVoltageCf() const { return voltCf_; }
	bool setVoltageCf(int8_t voltCf)
	{
		bool state = false;
		if ( (voltCf > -100) && (voltCf < 100) )
		{
			voltCf_ = voltCf;
			state = true;
		}
		return state;

	}

	// напряжение выхода
	uint8_t getVoltageOutInt() const { return (voltOut_ / 10); }
	uint8_t getVoltageOutFract() const { return (voltOut_ % 10); }
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract)
	{
		bool state = false;
		if ( (voltOutInt < 100) && (voltOutFract < 10) )
		{
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			state = true;
		}
		return state;
	}

	// ток выхода
	uint16_t getCurrentOut() const { return curOut_; }
	bool setCurrentOut(uint16_t curOut)
	{
		bool state = false;
		if (curOut < 999)
		{
			curOut_ = curOut;
			state = true;
		}
		return state;
	}

	// сопротивление выхода
	uint16_t getResistOut() const { return resistOut_; }
	bool setResistOut(uint16_t resistOut)
	{
		bool state = false;
		if (resistOut < 999)
		{
			resistOut_ = resistOut;
			state = true;
		}
		return state;
	}

	// уровень шума
	int8_t getVoltageNoise() const { return voltNoise_; }
	bool setVoltageNoise(int8_t voltNoise)
	{
		bool state = false;
		if ( (voltNoise > -100) && (voltNoise < 100) )
		{
			voltNoise_ = voltNoise;
			state = true;
		}
		return state;
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
	uint8_t getNumCom() { return numCom_; }

	// запись\считывание одного байта на передачу
	void 	setInt8(uint8_t byte) { byte_[0] = byte; }
	uint8_t getInt8() { return byte_[0]; }

	// запись\считывание переменной типа INT
	void setInt16(uint16_t val) { *((uint16_t *) byte_) = val; }
	uint16_t getInt16() { return *((uint16_t *) byte_); }

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
	TDataTime dataTime;

	// запись\считывание устройства для которого сделана запись
	bool setDevice(eGB_DEVICE device)
	{
		bool state = false;
		if ( (device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX) )
		{
			device_ = device;
			state = true;
		}
		else
			device_ = GB_DEVICE_MAX;
		return true;
	}
	eGB_DEVICE getDevice() const { return device_; }

	// тип события
	bool setEventType(uint8_t val)
	{
		bool state = false;
		if ( (val >= MIN_JRN_EVENT_VALUE) && (val <= MAX_JRN_EVENT_VALUE) )
		{
			eventType_ = val;
			state = true;
		}
		else
			eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
		return state;
	}
	uint8_t getEventType() const { return eventType_; }

private:
	// устройство
	eGB_DEVICE device_;

	// тип события
	uint8_t eventType_;
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
	TJournalEntry journalEntry;
};

#endif /* GLBDEF_H_ */
