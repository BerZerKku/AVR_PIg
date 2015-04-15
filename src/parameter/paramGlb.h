/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

/// ОБЩИЕ

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
		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}
	}

	TDeviceStatus Status;	///< Текущее состояние устройства

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

private:
	// версии прошивок микросхем
	uint16_t versProgIC_[GB_IC_MAX];
};


#endif /* PARAMGLB_H_ */
