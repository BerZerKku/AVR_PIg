/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

/// ОБЩИЕ

/// компенсация задержки
#define GLB_COMP_DELAY_MIN		0
#define GLB_COMP_DELAY_MAX 		255
#define GLB_COMP_DELAY_DISC 	1
#define GLB_COMP_DELAY_FRACT	1
#define GLB_COMP_DELAY_MIN_F  	(GLB_COMP_DELAY_MIN / GLB_COMP_DELAY_FRACT)
#define GLB_COMP_DELAY_MAX_F  	(GLB_COMP_DELAY_MAX / GLB_COMP_DELAY_FRACT)
#define GLB_COMP_DELAY_DISC_F 	(GLB_COMP_DELAY_DISC / GLB_COMP_DELAY_FRACT)

/// максимальный ток
#define GLB_CURR_MIN_MIN		0
#define GLB_CURR_MIN_MAX 		255
#define GLB_CURR_MIN_DISC 		1
#define GLB_CURR_MIN_FRACT		1
#define GLB_CURR_MIN_MIN_F  	(GLB_CURR_MIN_MIN / GLB_CURR_MIN_FRACT)
#define GLB_CURR_MIN_MAX_F  	(GLB_CURR_MIN_MAX / GLB_CURR_MIN_FRACT)
#define GLB_CURR_MIN_DISC_F 	(GLB_CURR_MIN_DISC / GLB_CURR_MIN_FRACT)

/// минимальный ток
#define GLB_CURR_MAX_MIN		0
#define GLB_CURR_MAX_MAX 		255
#define GLB_CURR_MAX_DISC 		1
#define GLB_CURR_MAX_FRACT		1
#define GLB_CURR_MAX_MIN_F  	(GLB_CURR_MAX_MIN / GLB_CURR_MAX_FRACT)
#define GLB_CURR_MAX_MAX_F  	(GLB_CURR_MAX_MAX / GLB_CURR_MAX_FRACT)
#define GLB_CURR_MAX_DISC_F 	(GLB_CURR_MAX_DISC / GLB_CURR_MAX_FRACT)

/// минимальное напряжение
#define GLB_VOLT_MIN_MIN		0
#define GLB_VOLT_MIN_MAX 		255
#define GLB_VOLT_MIN_DISC 		1
#define GLB_VOLT_MIN_FRACT		1
#define GLB_VOLT_MIN_MIN_F  	(GLB_VOLT_MIN_MIN / GLB_VOLT_MIN_FRACT)
#define GLB_VOLT_MIN_MAX_F  	(GLB_VOLT_MIN_MAX / GLB_VOLT_MIN_FRACT)
#define GLB_VOLT_MIN_DISC_F 	(GLB_VOLT_MIN_DISC / GLB_VOLT_MIN_FRACT)

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
	GB_IC_MAX
};

/// класс для общих параметров и настроек
class TDeviceGlb {
public:
	TDeviceGlb() {
		compDelay_ 	= GLB_COMP_DELAY_MAX;
		currMax_ 	= GLB_CURR_MAX_MAX;
		currMin_ 	= GLB_CURR_MIN_MAX;
		voltMin_ 	= GLB_VOLT_MIN_MAX;

		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}
	}

	TDeviceStatus status;	///< Текущее состояние устройства

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

	/** Установка параметра "Компенсация задержки".
	 * 	@param val Новое значение параметра.
	 * 	@retval True В случае корректного значения.
	 * 	@retval False Иначе.
	 */
	bool setCompDelay(uint8_t val) {
		bool stat = false;

		val = (val / GLB_COMP_DELAY_DISC_F) * GLB_COMP_DELAY_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_COMP_DELAY_MIN_F) {
			if (val <= GLB_COMP_DELAY_MAX_F) {
				compDelay_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	Возвращает значение параметра "Компенсация задержки".
	 * 	@retval Значение параметра.
	 */
	uint8_t getCompDelay() const {
		return compDelay_ * GLB_COMP_DELAY_FRACT;
	}

	/** Установка параметра "Максимальный ток".
	 * 	@param val Новое значение параметра.
	 * 	@retval True В случае корректного значения.
	 * 	@retval False Иначе.
	 */
	bool setCurrMax(uint8_t val) {
		bool stat = false;

		val = (val / GLB_CURR_MAX_DISC_F) * GLB_CURR_MAX_DISC_F;

		if (val >= GLB_CURR_MAX_MIN_F) {
			if (val <= GLB_CURR_MAX_MAX_F) {
				currMax_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	Возвращает значение параметра "Минимальный ток".
	 * 	@retval Значение параметра.
	 */
	uint8_t getCurrMax() const {
		return currMax_ * GLB_CURR_MAX_FRACT;
	}

	/** Установка параметра "Минимальный ток".
	 * 	@param val Новое значение параметра.
	 * 	@retval True В случае корректного значения.
	 * 	@retval False Иначе.
	 */
	bool setCurrMin(uint8_t val) {
		bool stat = false;

		val = (val / GLB_CURR_MIN_DISC_F) * GLB_CURR_MIN_DISC_F;

		if (val >= GLB_CURR_MIN_MIN_F) {
			if (val <= GLB_CURR_MIN_MAX_F) {
				currMin_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	Возвращает значение параметра "Минимальный ток".
	 * 	@retval Значение параметра.
	 */
	uint8_t getCurrMin() const {
		return currMin_ * GLB_CURR_MIN_FRACT;
	}

	/** Установка параметра "Минимальное напряжение".
	 * 	@param val Новое значение параметра.
	 * 	@retval True В случае корректного значения.
	 * 	@retval False Иначе.
	 */
	bool setVoltMin(uint8_t val) {
		bool stat = false;

		val = (val / GLB_VOLT_MIN_DISC_F) * GLB_VOLT_MIN_DISC_F;

		if (val >= GLB_VOLT_MIN_MIN_F) {
			if (val <= GLB_VOLT_MIN_MAX_F) {
				voltMin_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	Возвращает значение параметра "Минимальное напряжение".
	 * 	@retval Значение параметра.
	 */
	uint8_t getVoltMin() const {
		return voltMin_ * GLB_VOLT_MIN_FRACT;
	}

private:
	// версии прошивок микросхем
	uint16_t versProgIC_[GB_IC_MAX];

	uint8_t compDelay_;		///< Компенсация задержки
	uint8_t currMax_;		///< Максимальный ток
	uint8_t currMin_;		///< Минимальный ток
	uint8_t voltMin_;		///< Минимальное напряжение
};


#endif /* PARAMGLB_H_ */
