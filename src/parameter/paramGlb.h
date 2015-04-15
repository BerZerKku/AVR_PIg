/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

/// �����

/// ���������� ������� ��������
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

/// ����� ��� ����� ���������� � ��������
class TDeviceGlb {
public:
	TDeviceGlb() {
		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}
	}

	TDeviceStatus Status;	///< ������� ��������� ����������

	/**	������ ������ �������� ��� ��������� ������.
	 * 	������ �������� � ���������� int16_t.
	 * 	������� ���� - ������ ��������. ������� - �������.
	 * 	��������: 2.3, ��� 2 - ������, 3 - �������.
	 * 	@param vers ������ ��������.
	 * 	@param ic	����������.
	 */
	void setVersProgIC16(uint16_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			versProgIC_[ic] = vers;
		}
	}

	/**	������ ������ �������� ��� ���������� ������.
	  * ������ �������� � ���������� int16_t.
	 * 	������� ���� - ������ ��������. ������� - �������.
	 * 	��������: 2.3, ��� 2 - ������, 3 - �������.
	 * 	@param vers ������ ��������.
	 * 	@param ic	����������.
	 */
	void setVersProgIC8(uint8_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			uint8_t hi = vers >> 4;
			uint8_t low = vers & 0x0F;
			versProgIC_[ic] = TO_INT16(hi, low);
		}
	}

	/**	���������� ������ �������� ��� ��������� ���������� ������.
	 * 	������� ���� int16_t - ������ ��������, ������� - �������.
	 * 	��������: 2.3, ��� 2 - ������, 3 - �������.
	 * 	@return ������ �������� ����������.
	 * 	@retval 0 � ������ ���������� ������ ����������.
	 */
	uint16_t getVersProgIC(eGB_IC ic) {
		return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0);
	}

private:
	// ������ �������� ���������
	uint16_t versProgIC_[GB_IC_MAX];
};


#endif /* PARAMGLB_H_ */
