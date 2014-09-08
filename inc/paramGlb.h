/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

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
/// сетевой адрес
#define GLB_NET_ADR_MIN		0
#define GLB_NET_ADR_MAX		255
#define GLB_NET_ADR_DISC 	1
#define GLB_NET_ADR_FRACT	1
#define GLB_NET_ADR_MIN_F	(GLB_NET_ADR_MIN / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_MAX_F	(GLB_NET_ADR_MAX / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_DISC_F	(GLB_NET_ADR_DISC / GLB_NET_ADR_FRACT)
/// номинальное выходное напряжение
#define GLB_U_OUT_NOM_MIN	18
#define GLB_U_OUT_NOM_MAX	50
#define GLB_U_OUT_NOM_DISC 	1
#define GLB_U_OUT_NOM_FRACT	1
#define GLB_U_OUT_NOM_MIN_F	(GLB_U_OUT_NOM_MIN / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_MAX_F	(GLB_U_OUT_NOM_MAX / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_DISC_F (GLB_U_OUT_NOM_DISC / GLB_U_OUT_NOM_FRACT)
// частота
#define GLB_FREQ_MIN		16
#define GLB_FREQ_MAX		1000
#define GLB_FREQ_DISC 		1
#define GLB_FREQ_FRACT		1
#define GLB_FREQ_MIN_F		(GLB_FREQ_MIN / GLB_FREQ_FRACT)
#define GLB_FREQ_MAX_F		(GLB_FREQ_MAX / GLB_FREQ_FRACT)
#define GLB_FREQ_DISC_F 	(GLB_FREQ_DISC / GLB_FREQ_FRACT)
// тип детектора
#define GLB_DETECTOR_MIN	1
#define GLB_DETECTOR_MAX	3
#define GLB_DETECTOR_DISC	1
#define GLB_DETECTOR_FRACT	1
#define GLB_DETECTOR_MIN_F	(GLB_DETECTOR_MIN / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_MAX_F	(GLB_DETECTOR_MAX / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_DISC_F (GLB_DETECTOR_DISC / GLB_DETECTOR_FRACT)
// снижение ответа АК (ПВЗЛ)
#define GLB_AC_IN_DEC_MIN	0
#define GLB_AC_IN_DEC_MAX	20
#define GLB_AC_IN_DEC_DISC 	1
#define GLB_AC_IN_DEC_FRACT	1
#define GLB_AC_IN_DEC_MIN_F	(GLB_AC_IN_DEC_MIN / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_MAX_F	(GLB_AC_IN_DEC_MAX / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_DISC_F (GLB_AC_IN_DEC_DISC / GLB_AC_IN_DEC_FRACT)
// коррекция тока
// при вводе минимум 0, т.к. вводится реальное значение напряжения
#define GLB_COR_I_DEC_MIN	-999
#define GLB_COR_I_DEC_MAX	999
#define GLB_COR_I_DEC_DISC	1
#define GLB_COR_I_DEC_FRACT	1
#define GLB_COR_I_DEC_MIN_F	(GLB_COR_I_DEC_MIN / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_MAX_F	(GLB_COR_I_DEC_MAX / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_DISC_F (GLB_COR_I_DEC_DISC / GLB_COR_I_DEC_FRACT)
// коррекция напряжения (в 10 раз больше)
// при вводе минимум 0, т.к. вводится реальное значение напряжения
#define GLB_COR_U_DEC_MIN	-400
#define GLB_COR_U_DEC_MAX	400
#define GLB_COR_U_DEC_DISC	1
#define GLB_COR_U_DEC_FRACT	1
#define GLB_COR_U_DEC_MIN_F	(GLB_COR_U_DEC_MIN / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_MAX_F	(GLB_COR_U_DEC_MAX / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_DISC_F (GLB_COR_U_DEC_DISC / GLB_COR_U_DEC_FRACT)
// допустимые провалы (ПВЗУ-Е)
#define GLB_PVZUE_FAIL_MIN		0
#define GLB_PVZUE_FAIL_MAX		90
#define GLB_PVZUE_FAIL_DISC		18
#define GLB_PVZUE_FAIL_FRACT	1
#define GLB_PVZUE_FAIL_MIN_F	(GLB_PVZUE_FAIL_MIN / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_MAX_F	(GLB_PVZUE_FAIL_MAX / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_DISC_F	(GLB_PVZUE_FAIL_DISC / GLB_PVZUE_FAIL_FRACT)
// порог по помехе (ПВЗУ-Е)
#define GLB_PVZUE_N_TH_MIN		0
#define GLB_PVZUE_N_TH_MAX		255
#define GLB_PVZUE_N_TH_DISC		1
#define GLB_PVZUE_N_TH_FRACT	1
#define GLB_PVZUE_N_TH_MIN_F	(GLB_PVZUE_N_TH_MIN / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_MAX_F	(GLB_PVZUE_N_TH_MAX / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_DISC_F	(GLB_PVZUE_N_TH_DISC / GLB_PVZUE_N_TH_FRACT)
// допустимая помеха (ПВЗУ-Е)
#define GLB_PVZUE_N_LVL_MIN		18
#define GLB_PVZUE_N_LVL_MAX		90
#define GLB_PVZUE_N_LVL_DISC	18
#define GLB_PVZUE_N_LVL_FRACT	1
#define GLB_PVZUE_N_LVL_MIN_F	(GLB_PVZUE_N_LVL_MIN / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_MAX_F	(GLB_PVZUE_N_LVL_MAX / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_DISC_F	(GLB_PVZUE_N_LVL_DISC / GLB_PVZUE_N_LVL_FRACT)

/// Микросхемы имеющие прошивку
enum eGB_IC {
	GB_IC_MIN = 0,
	GB_IC_BSP_MCU = 0,
	GB_IC_BSP_DSP,
	GB_IC_PI_MCU,
	GB_IC_BSK_PLIS_PRD1,
	GB_IC_BSK_PLIS_PRD2,
	GB_IC_BSK_PLIS_PRM1,
	GB_IC_BSK_PLIS_PRM2,
	GB_IC_BSZ_PLIS,
	GB_IC_MAX
};

/// Совместимость
enum eGB_COMPATIBILITY {
	GB_COMPATIBILITY_MIN = 0,
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK80 = 2,
	GB_COMPATIBILITY_PVZUE = 3,
	GB_COMPATIBILITY_PVZL = 4,
	GB_COMPATIBILITY_MAX
};

// Протокол обмена (ПВЗУ-Е)
enum eGB_PVZUE_PROTOCOL {
	GB_PVZUE_PROTOCOL_MIN = 1,
	GB_PVZUE_PROTOCOL_FAST = 1,
	GB_PVZUE_PROTOCOL_SLOW = 2,
	GB_PVZUE_PROTOCOL_MAX
};

// Тип автоконтроля (ПВЗУ-Е)
enum eGB_PVZUE_TYPE_AC {
	GB_PVZUE_TYPE_AC_MIN = 1,
	GB_PVZUE_TYPE_AC_ALARM = 1,
	GB_PVZUE_TYPE_AC_NORM = 2,
	GB_PVZUE_TYPE_AC_CALM = 3,
	GB_PVZUE_TYPE_AC_MAX
};

// Признак четности (ПВЗУ-Е)
enum eGB_PVZUE_PARITY {
	GB_PVZUE_PARITY_MIN = 1,
	GB_PVZUE_PARITY_ON = 1,
	GB_PVZUE_PARITY_OFF = 2,
	GB_PVZUE_PARITY_MAX
};

/// класс для общих параметров и настроек
class TDeviceGlb {
public:
	TDeviceGlb() {
		typeDevice_ = AVANT_NO;
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeLine_ = GB_TYPE_LINE_MAX;
		compatibility_ = GB_COMPATIBILITY_MAX;
		versBsp_ = 0;
		versDsp_ = 0;

		timeSinchr_ = false;
		backup_ = false;
		deviceNum_ = GLB_DEV_NUM_MIN_F;
		outCheck_ = false;
		cfThreshold_ = GLB_CF_THRESH_MIN_F;
		timeRerun_ = GLB_T_RERUN_MIN_F;
		comPrdKeep_ = false;
		comPrmKeep_ = false;
		inDecrease_[0] = GLB_IN_DEC_MIN_F;
		inDecrease_[1] = GLB_IN_DEC_MIN_F;
		freq_ = GLB_FREQ_MIN_F;
		uOutNom_ = GLB_U_OUT_NOM_MIN_F;
		netAdr_ = GLB_NET_ADR_MIN_F;
		acInDec_ = GLB_AC_IN_DEC_MIN_F;
		pvzueProtocol_ = GB_PVZUE_PROTOCOL_MAX;
		pvzueParity_ = GB_PVZUE_PARITY_MAX;
		pvzueFail_ = GLB_PVZUE_FAIL_MIN_F;
		pvzueNoiseTH_ = GLB_PVZUE_N_TH_MIN_F;
		pvzueNoiseLvl_ = GLB_PVZUE_N_LVL_MIN_F;
		pvzueTypeAC_ = GB_PVZUE_TYPE_AC_MAX;
		detector_ = GLB_DETECTOR_MIN_F;
		corI_ = GLB_COR_I_DEC_MIN_F;
		corU_ = GLB_COR_U_DEC_MIN_F;

		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}
	}

	TDeviceStatus status;

	/**	Возвращает тип аппарата.
	 *
	 * 	@return Тип аппарата.
	 */
	eGB_TYPE_DEVICE getTypeDevice() const {
		return typeDevice_;
	}

	/**	Установка типа аппарата.
	 *
	 * 	В случае ошибки, значение не поменяется.
	 *
	 * 	@param Тип аппарата.
	 * 	@return Статус установки.
	 */
	uint8_t setTypeDevice(eGB_TYPE_DEVICE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < AVANT_NO) || (val > AVANT_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (typeDevice_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeDevice_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	Возвращает максимальное кол-во аппаратов в линии.
	 *
	 * 	@retval 3 В случае 3-х концевой версии
	 * 	@retval 2 Во всех остальных случаях.
	 */
	uint8_t getMaxNumDevices() const {
		return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2;
	}

	/**	Возвращает кол-во аппаратов в линии (2-х, 3-х концевая).
	 *
	 * 	@return Кол-во аппаратов в линии.
	 */
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	/** Установка кол-ва аппаратов в линии (2-х, 3-х концевая.).
	 *
	 * 	В случае ошибки, значение не поменяется.
	 * 	@param val Кол-во аппаратов в линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setNumDevices(eGB_NUM_DEVICES val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_NUM_DEVICES_MIN) || (val >= GB_NUM_DEVICES_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (numDevices_ == val) {
				act = GB_ACT_OLD;
			} else {
				numDevices_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	Возвращает текущий тип линии (вч/оптика и т.д.).
	 *
	 * 	@return Тип линии.
	 */
	eGB_TYPE_LINE getTypeLine() const {
		return typeLine_;
	}

	/** Установка типа линии (вч/оптика и т.д.).
	 *
	 * 	В случае ошибки, значение не поменяется.
	 *
	 * 	@param val Тип линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setTypeLine(eGB_TYPE_LINE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_TYPE_LINE_MIN) || (val >= GB_TYPE_LINE_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (typeLine_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeLine_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	Запись версии прошивки для микросхем АВАНТа.
	 * 	Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC16(uint16_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			versProgIC_[ic] = vers;
		}
	}

	/**	Запись версии прошивки для микросхема АВАНТа.
	  * Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC8(uint8_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			uint8_t hi = vers >> 4;
			uint8_t low = vers & 0x0F;
			versProgIC_[ic] = TO_INT16(hi, low);
		}
	}

	/**	Возвращает версию прошивки для указанной микросхемы АВАНТа.
	 * 	Старший байт int16_t - версия прошивки, младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@return Версия прошивки микросхемы.
	 * 	@retval 0 В случае ошибочного номера микросхемы.
	 */
	uint16_t getVersProgIC(eGB_IC ic) {
		return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0);
	}


	/**	Установка совместимости (тип удаленного аппарата).
	 * 	@param val Совместимость.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setCompatibility(eGB_COMPATIBILITY val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMPATIBILITY_MIN) || (val >= GB_COMPATIBILITY_MAX)) {
			val = GB_COMPATIBILITY_MAX;
			act = GB_ACT_ERROR;
		}

		if (compatibility_ == val) {
			act |= GB_ACT_OLD;
		} else {
			compatibility_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

	/**	Возвращает текущую совместимость.
	 * 	@return Совместимость.
	 */
	eGB_COMPATIBILITY getCompatibility() const {
		return compatibility_;
	}

	// синхронизация часов
	// True - включена, False - выключена
	bool setTimeSinchr(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			timeSinchr_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTimeSinchr() const {
		return timeSinchr_;
	}

	/** Установка параметра Резервирование.
	 * 	@param val Резервирование
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setBackup(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			backup_ = (bool) val;
			stat = true;
		}
		return stat;
	}

	/** Возвращает значение параметра Резервирование.
	 * 	@return Резервирование.
	 */
	bool getBackup() const {
		return backup_;
	}

	// номер аппарата
	bool setDeviceNum(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_DEV_NUM_MIN_F) {
			if (val <= getMaxNumDevices()) {
				deviceNum_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getDeviceNum() const {
		return (deviceNum_ * GLB_DEV_NUM_FRACT);
	}

	// контроль выходного сигнала
	bool setOutCheck(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			outCheck_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getOutCheck() const {
		return outCheck_;
	}

	// порог предупреждения по КЧ
	bool setCfThreshold(uint8_t val) {
		bool stat = false;
		val = (val / GLB_CF_THRESH_DISC_F) * GLB_CF_THRESH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_CF_THRESH_MIN_F) {
			if (val <= GLB_CF_THRESH_MAX_F) {
				cfThreshold_ = val;
				stat = true;
			}
		}

		return stat;
	}
	uint8_t getCfThreshold() const {
		return (cfThreshold_ * GLB_CF_THRESH_FRACT);
	}

	// время перезапуска
	bool setTimeRerun(uint8_t val) {
		bool stat = false;
		val = (val / GLB_T_RERUN_DISC_F) * GLB_T_RERUN_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_T_RERUN_MIN_F) {
			if (val <= GLB_T_RERUN_MAX_F) {
				timeRerun_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getTimeRerun() const {
		return (timeRerun_ * GLB_T_RERUN_FRACT);
	}

	// Удержание реле команд ПРД
	bool setComPrdKeep(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			comPrdKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrdKeep() const {
		return comPrdKeep_;
	}

	// Удержание реле команд ПРМ
	bool setComPrmKeep(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			comPrmKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrmKeep() const {
		return comPrmKeep_;
	}

	/** Установка нового значения "Загрубление чувствительности ПРМ".
	 * 	В 3-х концевой два параметра, в 2-х концевой о.дин
	 * 	@param val Значение параметра.
	 * 	@param num Номер параметра [1..2].
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Записывается значение первого параметра.
	 * 	@retval True Параметр успешно записан.
	 * 	@retval False Ошибка записи.
	 */
	bool setInDecrease(uint8_t val, uint8_t num) {
		bool stat = false;
		val = (val / GLB_IN_DEC_DISC_F) * GLB_IN_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_IN_DEC_MIN_F) {
			if (val <= GLB_IN_DEC_MAX_F) {
				if ((num == 1) || (num == 2)) {
					inDecrease_[num - 1] = val;
					stat = true;
				}
			}
		}
		return stat;
	}

	/**	Возвращает значение загрубления чувствительности ПРМ для заданного
	 * 	номера параметра.
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 *	@param num Номер параметра.
	 *	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Возвращается значение первого параметра.
	 *	@return Значение параметра.
	 */
	uint8_t getInDecrease(uint8_t num) const {
		if ((num < 1) || (num > 2))
			num = 1;
		return (inDecrease_[num - 1] * GLB_IN_DEC_FRACT);
	}

	// сетевой адрес
	bool setNetAddress(uint8_t val) {
		bool stat = false;
		val = (val / GLB_NET_ADR_DISC_F) * GLB_NET_ADR_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_NET_ADR_MIN_F) {
			if (val <= GLB_NET_ADR_MAX_F) {
				netAdr_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getNetAddress() const {
		return (netAdr_ * GLB_NET_ADR_FRACT);
	}

	// Uвых номинальное
	bool setUoutNom(uint8_t val) {
		bool stat = false;
		val = (val / GLB_U_OUT_NOM_DISC_F) * GLB_U_OUT_NOM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_U_OUT_NOM_MIN_F) {
			if (val <= GLB_U_OUT_NOM_MAX_F) {
				uOutNom_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getUoutNom() const {
		return (uOutNom_ * GLB_U_OUT_NOM_FRACT);
	}

	// частота
	bool setFreq(uint16_t val) {
		bool stat = false;
		val = (val / GLB_FREQ_DISC_F) * GLB_FREQ_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_FREQ_MIN_F) {
			if (val <= GLB_FREQ_MAX_F) {
				freq_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint16_t getFreq() const {
		return (freq_ * GLB_FREQ_FRACT);
	}

	// снижение ответа АК (ПВЗЛ)
	bool setAcInDec(uint8_t val) {
		bool stat = false;
		val = (val / GLB_AC_IN_DEC_DISC_F) * GLB_AC_IN_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_AC_IN_DEC_MIN_F) {
			if (val <= GLB_AC_IN_DEC_MAX_F) {
				acInDec_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getAcInDec() const {
		return (acInDec_ * GLB_AC_IN_DEC_FRACT);
	}

	// Протокол обмена (ПВЗУ-Е)
	bool setPvzueProtocol(eGB_PVZUE_PROTOCOL val) {
		bool stat = false;

		if ((val >= GB_PVZUE_PROTOCOL_MIN) && (val <= GB_PVZUE_PROTOCOL_MAX)) {
			pvzueProtocol_ = val;
			stat = true;
		} else
			pvzueProtocol_ = GB_PVZUE_PROTOCOL_MAX;

		return stat;
	}
	eGB_PVZUE_PROTOCOL getPvzueProtocol() const {
		return pvzueProtocol_;
	}

	// Признак четности (ПВЗУ-Е)
	bool setPvzueParity(eGB_PVZUE_PARITY val) {
		bool stat = false;

		if ((val >= GB_PVZUE_PARITY_MIN) && (val <= GB_PVZUE_PARITY_MAX)) {
			pvzueParity_ = val;
			stat = true;
		} else
			pvzueParity_ = GB_PVZUE_PARITY_MAX;

		return stat;
	}
	eGB_PVZUE_PARITY getPvzueParity() const {
		return pvzueParity_;
	}

	// Допустимые провалы (ПВЗУ-Е)
	bool setPvzueFail(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_FAIL_DISC_F) * GLB_PVZUE_FAIL_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_FAIL_MIN_F) {
			if (val <= GLB_PVZUE_FAIL_MAX_F) {
				pvzueFail_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getPvzueFail() const {
		return pvzueFail_;
	}

	// Порог по помехе (ПВЗУ-Е)
	bool setPvzueNoiseTH(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_TH_DISC_F) * GLB_PVZUE_N_TH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_N_TH_MIN_F) {
			if (val <= GLB_PVZUE_N_TH_MAX_F) {
				pvzueNoiseTH_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getPvzueNoiseTH() const {
		return pvzueNoiseTH_;
	}

	// Допустимая помеха (ПВЗУ-Е)
	bool setPvzueNoiseLvl(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_LVL_DISC_F) * GLB_PVZUE_N_LVL_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_N_LVL_MIN_F) {
			if (val <= GLB_PVZUE_N_LVL_MAX_F) {
				pvzueNoiseLvl_ = val;
				stat = true;
			}
		}

		return stat;
	}
	uint8_t getPvzueNoiseLvl() const {
		return pvzueNoiseLvl_;
	}

	// Тип автоконтроля
	bool setPvzueTypeAC(eGB_PVZUE_TYPE_AC val) {
		bool stat = false;

		if (val >= GB_PVZUE_TYPE_AC_MIN) {
			if (val <= GB_PVZUE_TYPE_AC_MAX) {
				pvzueTypeAC_ = val;
				stat = true;
			}
		}

		// установка ошибочного значения
		if (!stat)
			pvzueTypeAC_ = GB_PVZUE_TYPE_AC_MAX;

		return stat;
	}
	eGB_PVZUE_TYPE_AC getPvzueTypeAC() const {
		return pvzueTypeAC_;
	}

	/** Установка параметра Тип детектора.
	 * 	@param val Тип детектора.
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setDetector(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DETECTOR_DISC_F) * GLB_DETECTOR_DISC_F;

		if (val >= GLB_DETECTOR_MIN_F) {
			if (val <= GLB_DETECTOR_MAX_F) {
				detector_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/** Возвращает значение Типа детектора.
	 * 	@return Тип детектора.
	 */
	uint8_t getDetector() const {
		return detector_;
	}

	// коррекция тока
	bool setCorI(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_I_DEC_DISC_F) * GLB_COR_I_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_COR_I_DEC_MIN_F) {
			if (val <= GLB_COR_I_DEC_MAX_F) {
				corI_ = val;
				stat = true;
			}
		}
		return stat;
	}
	int16_t getCorI() const {
		return (corI_ * GLB_COR_I_DEC_FRACT);
	}

	// коррекция напряжения
	bool setCorU(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_U_DEC_DISC_F) * GLB_COR_U_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_COR_U_DEC_MIN_F) {
			if (val <= GLB_COR_U_DEC_MAX_F) {
				corU_ = val;
				stat = true;
			}
		}
		return stat;
	}
	int16_t getCorU() const {
		return (corU_ * GLB_COR_U_DEC_FRACT);
	}

private:
	// версии прошивок микросхем
	uint16_t versProgIC_[GB_IC_MAX];

	// тип аппарата
	eGB_TYPE_DEVICE typeDevice_;

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

	// резервирование
	bool backup_;

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
	uint8_t inDecrease_[2];

	// сетевой адрес
	uint8_t netAdr_;

	// Uвых номинальное
	uint8_t uOutNom_;

	// Частота
	uint16_t freq_;

	// Снижение ответа АК (ПВЗЛ)
	uint8_t acInDec_;

	// Протокол обмена (ПВЗУ-Е)
	eGB_PVZUE_PROTOCOL pvzueProtocol_;

	// Признак четности (ПВЗУ-Е)
	eGB_PVZUE_PARITY pvzueParity_;

	// Допустимые провалы (ПВЗУ-Е)
	uint8_t pvzueFail_;

	// Порог по помехе (ПВЗУ-Е)
	uint8_t pvzueNoiseTH_;

	// Допустимая помеха (ПВЗУ-Е)
	uint8_t pvzueNoiseLvl_;

	// Тип автоконтроля (ПВЗУ-Е)
	eGB_PVZUE_TYPE_AC pvzueTypeAC_;

	// Тип детектора
	uint8_t detector_;

	// Коррекция тока
	int16_t corI_;

	// Коррекция напряжения
	int16_t corU_;
};


#endif /* PARAMGLB_H_ */
