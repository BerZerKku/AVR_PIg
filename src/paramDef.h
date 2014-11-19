/*
 * deviceDef.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMDEF_H_
#define PARAMDEF_H_

// #include "glbDefine.h"

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
#define DEF_OVERLAP_MIN2	18	// минимум в Р400м
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
/// сдвиг переднего фронта ПРД относительно сигнала МАН (пуск)
#define DEF_SH_FRONT_MIN	0
#define DEF_SH_FRONT_MAX	72
#define DEF_SH_FRONT_DISC	1
#define DEF_SH_FRONT_FRACT	1
#define DEF_SH_FRONT_MIN_F	(DEF_SH_FRONT_MIN / DEF_SH_FRONT_FRACT)
#define DEF_SH_FRONT_MAX_F	(DEF_SH_FRONT_MAX / DEF_SH_FRONT_FRACT)
#define DEF_SH_FRONT_DISC_F	(DEF_SH_FRONT_DISC / DEF_SH_FRONT_FRACT)
/// сдвиг заднего фронта ПРД относительно сигнала МАН (пуск)
#define DEF_SH_BACK_MIN		0
#define DEF_SH_BACK_MAX		72
#define DEF_SH_BACK_DISC	1
#define DEF_SH_BACK_FRACT	1
#define DEF_SH_BACK_MIN_F	(DEF_SH_BACK_MIN / DEF_SH_BACK_FRACT)
#define DEF_SH_BACK_MAX_F	(DEF_SH_BACK_MAX / DEF_SH_BACK_FRACT)
#define DEF_SH_BACK_DISC_F	(DEF_SH_BACK_DISC / DEF_SH_BACK_FRACT)
/// сдвиг ПРМ
#define DEF_SH_PRM_MIN		0
#define DEF_SH_PRM_MAX		72
#define DEF_SH_PRM_DISC	1
#define DEF_SH_PRM_FRACT	1
#define DEF_SH_PRM_MIN_F	(DEF_SH_PRM_MIN / DEF_SH_PRM_FRACT)
#define DEF_SH_PRM_MAX_F	(DEF_SH_PRM_MAX / DEF_SH_PRM_FRACT)
#define DEF_SH_PRM_DISC_F	(DEF_SH_PRM_DISC / DEF_SH_PRM_FRACT)
/// сдвиг ВЧ ПРД от пуска
#define DEF_SH_PRD_MIN		0
#define DEF_SH_PRD_MAX		72
#define DEF_SH_PRD_DISC	1
#define DEF_SH_PRD_FRACT	1
#define DEF_SH_PRD_MIN_F	(DEF_SH_PRD_MIN / DEF_SH_PRD_FRACT)
#define DEF_SH_PRD_MAX_F	(DEF_SH_PRD_MAX / DEF_SH_PRD_FRACT)
#define DEF_SH_PRD_DISC_F	(DEF_SH_PRD_DISC / DEF_SH_PRD_FRACT)


/// Тип автоконтроля
enum eGB_TYPE_AC {
	GB_TYPE_AC_MIN = 1, 		// минимальное значение
	GB_TYPE_AC_AUTO_NORM = 1,	// АК нормальный / АК включен
	GB_TYPE_AC_AUTO_REPEAT = 2,	// АК повторный
	GB_TYPE_AC_FAST = 3,		// АК ускоренный
	GB_TYPE_AC_OFF = 4,			// АК выключен
	GB_TYPE_AC_CHECK = 5,		// АК беглый
	GB_TYPE_AC_PUSK_SELF = 6,	// Пуск АК свой / АК испытания / АК контр.пров.
	GB_TYPE_AC_PUSK = 7,		// АК пуск
	GB_TYPE_AC_ONE_SIDE = 8,	// АК односторонний
	GB_TYPE_AC_MAX,				// максимальное принимаемое значение
	GB_TYPE_AC_AUTO				// подмена  GB_TYPE_AC_AUTO_NORM в Р400м->ЛинияР
};

/// Частоты ПРМ и ПРД в ПВЗЛ
enum eGB_PVZL_FREQ {
	GB_PVZL_FREQ_MIN 	= 0,
	GB_PVZL_FREQ_M1000 	= 0,
	GB_PVZL_FREQ_M500 	= 1,
	GB_PVZL_FREQ_0 		= 2,
	GB_PVZL_FREQ_P500 	= 3,
	GB_PVZL_FREQ_P1000 	= 4,
	GB_PVZL_FREQ_MAX
};

/// класс для параметров и настроек защиты
class TDeviceDef {
public:
	TDeviceDef() {
		defType_ = DEF_TYPE_MIN;
		numDevices_ = GB_NUM_DEVICES_MAX;
		timeNoMan_ = DEF_T_NO_MAN_MIN_F;
		overlap_ = DEF_OVERLAP_MIN_F;
		delay_[0] = DEF_DELAY_MIN_F;
		delay_[1] = DEF_DELAY_MIN_F;
		rzThreshold_ = DEF_RZ_THRESH_MIN_F;
		rzDec_[0] = DEF_RZ_DEC_MIN_F;
		rzDec_[1] = DEF_RZ_DEC_MIN_F;
		prmType_ = DEF_PRM_TYPE_MIN;
		typeAc_ = GB_TYPE_AC_AUTO_NORM;
		timeToAc_ = 0;
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		acDec_ = false;
		overflow_ = false;
		freqPrm_ = GB_PVZL_FREQ_MAX;
		freqPrd_ = GB_PVZL_FREQ_MAX;
		shFront_ = DEF_SH_FRONT_MIN_F;
		shBack_ = DEF_SH_BACK_MIN_F;
		shPrm_ = DEF_SH_PRM_MIN_F;
		shPrd_ = DEF_SH_PRD_MIN_F;
	}

	TDeviceStatus status;

	// тип защиты
	bool setDefType(uint8_t val) {
		bool stat = false;
		if ((val >= DEF_TYPE_MIN) && (val < DEF_TYPE_MAX)) {
			defType_ = val;
			stat = true;
		} else {
			defType_ = DEF_TYPE_MAX;
		}

		return stat;
	}
	uint8_t getDefType() const {
		return defType_;
	}

	// тип линии (кол-во аппаратов в линии)
	bool setNumDevices(eGB_NUM_DEVICES val) {
		bool stat = false;
		if (val >= GB_NUM_DEVICES_MIN) {
			if (val < GB_NUM_DEVICES_MAX) {
				numDevices_ = val;
				stat = true;
			}
		}

		if (!stat)
			val = GB_NUM_DEVICES_MAX;
		numDevices_ = val;
		return stat;
	}
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	// допустимое время без манипуляции
	bool setTimeNoMan(uint8_t val) {
		bool stat = false;
		val = (val / DEF_T_NO_MAN_DISC_F) * DEF_T_NO_MAN_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_T_NO_MAN_MIN_F) {
			if (val <= DEF_T_NO_MAN_MAX_F) {
				timeNoMan_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getTimeNoMan() const {
		return (timeNoMan_ * DEF_T_NO_MAN_FRACT);
	}

	// перекрытие импульсов
	bool setOverlap(uint8_t val) {
		bool stat = false;
		val = (val / DEF_OVERLAP_DISC_F) * DEF_OVERLAP_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_OVERLAP_MIN_F) {
			if (val <= DEF_OVERLAP_MAX_F) {
				overlap_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getOverlap() const {
		return (overlap_ * DEF_OVERLAP_FRACT);
	}

	/** Установка значения "Компенсация задержки на линии".
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Записывается значение первого параметра.
	 * 	@param val Значение параметра.
	 * 	@retval True Параметр успешно записан.
	 * 	@retval False Ошибка записи.
	 */
	bool setDelay(uint8_t num, uint8_t val) {
		bool stat = false;
		val = (val / DEF_DELAY_DISC_F) * DEF_DELAY_DISC_F;

		num = (num == 2) ? 1 : 0;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_DELAY_MIN_F) {
			if (val <= DEF_DELAY_MAX_F) {
				delay_[num] = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	Считывание значения "Компенсация задержки на линии".
	 *
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 *	В случае ошибочного значения номера параметра будет считан первый.
	 *
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Возвращается значение первого параметра.
	 * 	@return Значени параметра.
	 */
	uint8_t getDelay(uint8_t num) const {
		num = (num == 2) ? 1 : 0;
		return (delay_[num] * DEF_DELAY_FRACT);
	}

	// порог предуреждения по РЗ
	bool setRzThreshold(uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_THRESH_DISC_F) * DEF_RZ_THRESH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_RZ_THRESH_MIN_F) {
			if (val <= DEF_RZ_THRESH_MAX_F) {
				rzThreshold_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getRzThreshold() const {
		return (rzThreshold_ * DEF_RZ_THRESH_FRACT);
	}

	/** Установка значения "Загрубление чувствительности по РЗ".
	 *
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 *	В случае ошибочного значения номера параметра будет считан первый.
	 *
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Записывается значение первого параметра.
	 * 	@param val Значение параметра.
	 * 	@retval True Параметр успешно записан.
	 * 	@retval False Ошибка записи.
	 */
	bool setRzDec(uint8_t num, uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_DEC_DISC_F) * DEF_RZ_DEC_DISC_F;

		num = (num == 2) ? 1 : 0;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_RZ_DEC_MIN_F) {
			if (val <= DEF_RZ_DEC_MAX_F) {
				rzDec_[num] = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	Считывание значения "Загрубление чувствительности по РЗ".
	 *
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 *	В случае ошибочного значения номера параметра будет считан первый.
	 *
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Возвращается значение первого параметра.
	 * 	@return Значени параметра.
	 */
	uint8_t getRzDec(uint8_t num) const {
		num = (num == 2) ? 1 : 0;
		return (rzDec_[num] * DEF_RZ_DEC_FRACT);
	}

	// тип приемника
	bool setPrmType(uint8_t val) {
		bool stat = false;
		if ((val >= DEF_PRM_TYPE_MIN) && (val < DEF_PRM_TYPE_MAX)) {
			prmType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getPrmType() const {
		return prmType_;
	}

	// тип автоконтроля
	bool setTypeAC(eGB_TYPE_AC val) {
		bool stat = false;
		if ((val >= GB_TYPE_AC_MIN) && (val <= GB_TYPE_AC_MAX)) {
			typeAc_ = val;
			stat = true;
		}

		return stat;
	}
	eGB_TYPE_AC getTypeAC() const {
		return typeAc_;
	}

	// время до автоконтроля
	bool setTimeToAC(uint32_t val) {
		bool stat = false;

		val /= 1000;

		if (val <= 60000) {
			timeToAc_ = val;
			stat = true;
		}

		return stat;
	}
	uint64_t getTimeToAC() const {
		return timeToAc_;
	}

	// снижение уровня автоконтроля
	// True - включена, False - выключена
	bool setAcDec(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			acDec_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getAcDec() const {
		return acDec_;
	}

	// частота ПРМ (ПВЗЛ)
	// True - включена, False - выключена
	bool setFreqPrm(eGB_PVZL_FREQ val) {
		bool stat = false;
		if ((val >= GB_PVZL_FREQ_MIN) && (val <= GB_PVZL_FREQ_MAX)) {
			freqPrm_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getFreqPrm() const {
		return freqPrm_;
	}

	// частота ПРД (ПВЗЛ)
	// True - включена, False - выключена
	bool setFreqPrd(eGB_PVZL_FREQ val) {
		bool stat = false;
		if ((val >= GB_PVZL_FREQ_MIN) && (val <= GB_PVZL_FREQ_MAX)) {
			freqPrd_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getFreqPrd() const {
		return freqPrd_;
	}

	// сдвиг переднего фронта ПРД относительно сигнала МАН (пуск)
	bool setShiftFront(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_FRONT_DISC_F) * DEF_SH_FRONT_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_SH_FRONT_MIN_F) {
			if (val <= DEF_SH_FRONT_MAX_F) {
				shFront_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getShiftFront() const {
		return shFront_;
	}

	// сдвиг заднего фронта ПРД относительно сигнала МАН (пуск)
	bool setShiftBack(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_BACK_DISC_F) * DEF_SH_BACK_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_SH_BACK_MIN_F) {
			if (val <= DEF_SH_BACK_MAX_F) {
				shBack_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getShiftBack() const {
		return shBack_;
	}

	// сдвиг ПРМ
	bool setShiftPrm(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_PRM_DISC_F) * DEF_SH_PRM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_SH_PRM_MIN_F) {
			if (val <= DEF_SH_PRM_MAX_F) {
				shPrm_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getShiftPrm() const {
		return shPrm_;
	}

	// сдвиг ВЧ ПРД от пуска
	bool setShiftPrd(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_PRD_DISC_F) * DEF_SH_PRD_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_SH_PRD_MIN_F) {
			if (val <= DEF_SH_PRD_MAX_F) {
				shPrd_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getShiftPrd() const {
		return shPrd_;
	}

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

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
	uint8_t delay_[2];

	// порог предупреждения по РЗ
	uint8_t rzThreshold_;

	// загрубление чувствительности по РЗ
	uint8_t rzDec_[2];

	// тип приемника
	uint8_t prmType_;

	// тип автоконтроля
	eGB_TYPE_AC typeAc_;

	// время до автоконтроля в секундах
	uint16_t timeToAc_;

	// снижение уровня АК
	bool acDec_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;

	// частота ПРМ (ПВЗЛ)
	eGB_PVZL_FREQ freqPrm_;

	// частота ПРД (ПВЗЛ)
	eGB_PVZL_FREQ freqPrd_;

	// сдвиг переднего фронта ПРД относительно сигнала МАН (пуск)
	uint8_t shFront_;

	// сдвиг заднего фронта ПРД относительно сигнала МАН (пуск)
	uint8_t shBack_;

	// сдвиг ПРМ
	uint8_t shPrm_;

	// сдвиг ВЧ ПРД от пуска
	uint8_t shPrd_;
};


#endif /* PARAMDEF_H_ */
