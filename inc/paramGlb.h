/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

/// �����
/// ����� ��������
#define GLB_DEV_NUM_MIN		1
#define GLB_DEV_NUM_MAX 	3
#define GLB_DEV_NUM_DISC 	1
#define GLB_DEV_NUM_FRACT	1
#define GLB_DEV_NUM_MIN_F	(GLB_DEV_NUM_MIN / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_MAX_F 	(GLB_DEV_NUM_MAX / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_DISC_F	(GLB_DEV_NUM_DISC / GLB_DEV_NUM_FRACT)
/// ����� �������������� �� ��
#define GLB_CF_THRESH_MIN	0
#define GLB_CF_THRESH_MAX	22
#define GLB_CF_THRESH_DISC	1
#define GLB_CF_THRESH_FRACT	1
#define GLB_CF_THRESH_MIN_F	(GLB_CF_THRESH_MIN / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_MAX_F	(GLB_CF_THRESH_MAX / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_DISC_F (GLB_CF_THRESH_DISC / GLB_CF_THRESH_FRACT)
/// ����� �����������
#define GLB_T_RERUN_MIN		0
#define GLB_T_RERUN_MAX		5
#define GLB_T_RERUN_DISC	1
#define GLB_T_RERUN_FRACT	1
#define GLB_T_RERUN_MIN_F	(GLB_T_RERUN_MIN / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_MAX_F	(GLB_T_RERUN_MAX / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_DISC_F	(GLB_T_RERUN_DISC / GLB_T_RERUN_FRACT)
/// ���������� �������� ������
#define GLB_IN_DEC_MIN		0
#define GLB_IN_DEC_MAX		22
#define GLB_IN_DEC_DISC 	1
#define GLB_IN_DEC_FRACT	1
#define GLB_IN_DEC_MIN_F	(GLB_IN_DEC_MIN / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_MAX_F	(GLB_IN_DEC_MAX / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_DISC_F	(GLB_IN_DEC_DISC / GLB_IN_DEC_FRACT)
/// ������� �����
#define GLB_NET_ADR_MIN		0
#define GLB_NET_ADR_MAX		255
#define GLB_NET_ADR_DISC 	1
#define GLB_NET_ADR_FRACT	1
#define GLB_NET_ADR_MIN_F	(GLB_NET_ADR_MIN / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_MAX_F	(GLB_NET_ADR_MAX / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_DISC_F	(GLB_NET_ADR_DISC / GLB_NET_ADR_FRACT)
/// ����������� �������� ����������
#define GLB_U_OUT_NOM_MIN	18
#define GLB_U_OUT_NOM_MAX	50
#define GLB_U_OUT_NOM_DISC 	1
#define GLB_U_OUT_NOM_FRACT	1
#define GLB_U_OUT_NOM_MIN_F	(GLB_U_OUT_NOM_MIN / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_MAX_F	(GLB_U_OUT_NOM_MAX / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_DISC_F (GLB_U_OUT_NOM_DISC / GLB_U_OUT_NOM_FRACT)
// �������
#define GLB_FREQ_MIN		16
#define GLB_FREQ_MAX		1000
#define GLB_FREQ_DISC 		1
#define GLB_FREQ_FRACT		1
#define GLB_FREQ_MIN_F		(GLB_FREQ_MIN / GLB_FREQ_FRACT)
#define GLB_FREQ_MAX_F		(GLB_FREQ_MAX / GLB_FREQ_FRACT)
#define GLB_FREQ_DISC_F 	(GLB_FREQ_DISC / GLB_FREQ_FRACT)
// ��� ���������
#define GLB_DETECTOR_MIN	1
#define GLB_DETECTOR_MAX	3
#define GLB_DETECTOR_DISC	1
#define GLB_DETECTOR_FRACT	1
#define GLB_DETECTOR_MIN_F	(GLB_DETECTOR_MIN / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_MAX_F	(GLB_DETECTOR_MAX / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_DISC_F (GLB_DETECTOR_DISC / GLB_DETECTOR_FRACT)
// �������� ������ �� (����)
#define GLB_AC_IN_DEC_MIN	0
#define GLB_AC_IN_DEC_MAX	20
#define GLB_AC_IN_DEC_DISC 	1
#define GLB_AC_IN_DEC_FRACT	1
#define GLB_AC_IN_DEC_MIN_F	(GLB_AC_IN_DEC_MIN / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_MAX_F	(GLB_AC_IN_DEC_MAX / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_DISC_F (GLB_AC_IN_DEC_DISC / GLB_AC_IN_DEC_FRACT)
// ��������� ����
// ��� ����� ������� 0, �.�. �������� �������� �������� ����������
#define GLB_COR_I_DEC_MIN	-999
#define GLB_COR_I_DEC_MAX	999
#define GLB_COR_I_DEC_DISC	1
#define GLB_COR_I_DEC_FRACT	1
#define GLB_COR_I_DEC_MIN_F	(GLB_COR_I_DEC_MIN / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_MAX_F	(GLB_COR_I_DEC_MAX / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_DISC_F (GLB_COR_I_DEC_DISC / GLB_COR_I_DEC_FRACT)
// ��������� ���������� (� 10 ��� ������)
// ��� ����� ������� 0, �.�. �������� �������� �������� ����������
#define GLB_COR_U_DEC_MIN	-400
#define GLB_COR_U_DEC_MAX	400
#define GLB_COR_U_DEC_DISC	1
#define GLB_COR_U_DEC_FRACT	1
#define GLB_COR_U_DEC_MIN_F	(GLB_COR_U_DEC_MIN / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_MAX_F	(GLB_COR_U_DEC_MAX / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_DISC_F (GLB_COR_U_DEC_DISC / GLB_COR_U_DEC_FRACT)
// ���������� ������� (����-�)
#define GLB_PVZUE_FAIL_MIN		0
#define GLB_PVZUE_FAIL_MAX		90
#define GLB_PVZUE_FAIL_DISC		18
#define GLB_PVZUE_FAIL_FRACT	1
#define GLB_PVZUE_FAIL_MIN_F	(GLB_PVZUE_FAIL_MIN / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_MAX_F	(GLB_PVZUE_FAIL_MAX / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_DISC_F	(GLB_PVZUE_FAIL_DISC / GLB_PVZUE_FAIL_FRACT)
// ����� �� ������ (����-�)
#define GLB_PVZUE_N_TH_MIN		0
#define GLB_PVZUE_N_TH_MAX		255
#define GLB_PVZUE_N_TH_DISC		1
#define GLB_PVZUE_N_TH_FRACT	1
#define GLB_PVZUE_N_TH_MIN_F	(GLB_PVZUE_N_TH_MIN / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_MAX_F	(GLB_PVZUE_N_TH_MAX / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_DISC_F	(GLB_PVZUE_N_TH_DISC / GLB_PVZUE_N_TH_FRACT)
// ���������� ������ (����-�)
#define GLB_PVZUE_N_LVL_MIN		18
#define GLB_PVZUE_N_LVL_MAX		90
#define GLB_PVZUE_N_LVL_DISC	18
#define GLB_PVZUE_N_LVL_FRACT	1
#define GLB_PVZUE_N_LVL_MIN_F	(GLB_PVZUE_N_LVL_MIN / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_MAX_F	(GLB_PVZUE_N_LVL_MAX / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_DISC_F	(GLB_PVZUE_N_LVL_DISC / GLB_PVZUE_N_LVL_FRACT)

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
	GB_IC_BSZ_PLIS,
	GB_IC_MAX
};

/// �������������
enum eGB_COMPATIBILITY {
	GB_COMPATIBILITY_MIN = 0,
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK80 = 2,
	GB_COMPATIBILITY_PVZUE = 3,
	GB_COMPATIBILITY_PVZL = 4,
	GB_COMPATIBILITY_MAX
};

// �������� ������ (����-�)
enum eGB_PVZUE_PROTOCOL {
	GB_PVZUE_PROTOCOL_MIN = 1,
	GB_PVZUE_PROTOCOL_FAST = 1,
	GB_PVZUE_PROTOCOL_SLOW = 2,
	GB_PVZUE_PROTOCOL_MAX
};

// ��� ������������ (����-�)
enum eGB_PVZUE_TYPE_AC {
	GB_PVZUE_TYPE_AC_MIN = 1,
	GB_PVZUE_TYPE_AC_ALARM = 1,
	GB_PVZUE_TYPE_AC_NORM = 2,
	GB_PVZUE_TYPE_AC_CALM = 3,
	GB_PVZUE_TYPE_AC_MAX
};

// ������� �������� (����-�)
enum eGB_PVZUE_PARITY {
	GB_PVZUE_PARITY_MIN = 1,
	GB_PVZUE_PARITY_ON = 1,
	GB_PVZUE_PARITY_OFF = 2,
	GB_PVZUE_PARITY_MAX
};

/// ����� ��� ����� ���������� � ��������
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

	/**	���������� ��� ��������.
	 *
	 * 	@return ��� ��������.
	 */
	eGB_TYPE_DEVICE getTypeDevice() const {
		return typeDevice_;
	}

	/**	��������� ���� ��������.
	 *
	 * 	� ������ ������, �������� �� ����������.
	 *
	 * 	@param ��� ��������.
	 * 	@return ������ ���������.
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

	/**	���������� ������������ ���-�� ��������� � �����.
	 *
	 * 	@retval 3 � ������ 3-� �������� ������
	 * 	@retval 2 �� ���� ��������� �������.
	 */
	uint8_t getMaxNumDevices() const {
		return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2;
	}

	/**	���������� ���-�� ��������� � ����� (2-�, 3-� ��������).
	 *
	 * 	@return ���-�� ��������� � �����.
	 */
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	/** ��������� ���-�� ��������� � ����� (2-�, 3-� ��������.).
	 *
	 * 	� ������ ������, �������� �� ����������.
	 * 	@param val ���-�� ��������� � �����.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
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

	/**	���������� ������� ��� ����� (��/������ � �.�.).
	 *
	 * 	@return ��� �����.
	 */
	eGB_TYPE_LINE getTypeLine() const {
		return typeLine_;
	}

	/** ��������� ���� ����� (��/������ � �.�.).
	 *
	 * 	� ������ ������, �������� �� ����������.
	 *
	 * 	@param val ��� �����.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
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


	/**	��������� ������������� (��� ���������� ��������).
	 * 	@param val �������������.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
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

	/**	���������� ������� �������������.
	 * 	@return �������������.
	 */
	eGB_COMPATIBILITY getCompatibility() const {
		return compatibility_;
	}

	// ������������� �����
	// True - ��������, False - ���������
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

	/** ��������� ��������� ��������������.
	 * 	@param val ��������������
	 * 	@retval True - � ������ �������� ���������.
	 * 	@retval False - ���� ��������� ������ �������� �� �������.
	 */
	bool setBackup(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			backup_ = (bool) val;
			stat = true;
		}
		return stat;
	}

	/** ���������� �������� ��������� ��������������.
	 * 	@return ��������������.
	 */
	bool getBackup() const {
		return backup_;
	}

	// ����� ��������
	bool setDeviceNum(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// �������� ��������� �������
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

	// ����� �������������� �� ��
	bool setCfThreshold(uint8_t val) {
		bool stat = false;
		val = (val / GLB_CF_THRESH_DISC_F) * GLB_CF_THRESH_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ����� �����������
	bool setTimeRerun(uint8_t val) {
		bool stat = false;
		val = (val / GLB_T_RERUN_DISC_F) * GLB_T_RERUN_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ��������� ���� ������ ���
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

	// ��������� ���� ������ ���
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

	/** ��������� ������ �������� "����������� ���������������� ���".
	 * 	� 3-� �������� ��� ���������, � 2-� �������� �.���
	 * 	@param val �������� ���������.
	 * 	@param num ����� ��������� [1..2].
	 * 	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 * 	@retval True �������� ������� �������.
	 * 	@retval False ������ ������.
	 */
	bool setInDecrease(uint8_t val, uint8_t num) {
		bool stat = false;
		val = (val / GLB_IN_DEC_DISC_F) * GLB_IN_DEC_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	/**	���������� �������� ����������� ���������������� ��� ��� ���������
	 * 	������ ���������.
	 * 	� 3-� �������� ��� ���������, � 2-� �������� ����.
	 *	@param num ����� ���������.
	 *	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 *	@return �������� ���������.
	 */
	uint8_t getInDecrease(uint8_t num) const {
		if ((num < 1) || (num > 2))
			num = 1;
		return (inDecrease_[num - 1] * GLB_IN_DEC_FRACT);
	}

	// ������� �����
	bool setNetAddress(uint8_t val) {
		bool stat = false;
		val = (val / GLB_NET_ADR_DISC_F) * GLB_NET_ADR_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// U��� �����������
	bool setUoutNom(uint8_t val) {
		bool stat = false;
		val = (val / GLB_U_OUT_NOM_DISC_F) * GLB_U_OUT_NOM_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// �������
	bool setFreq(uint16_t val) {
		bool stat = false;
		val = (val / GLB_FREQ_DISC_F) * GLB_FREQ_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// �������� ������ �� (����)
	bool setAcInDec(uint8_t val) {
		bool stat = false;
		val = (val / GLB_AC_IN_DEC_DISC_F) * GLB_AC_IN_DEC_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// �������� ������ (����-�)
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

	// ������� �������� (����-�)
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

	// ���������� ������� (����-�)
	bool setPvzueFail(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_FAIL_DISC_F) * GLB_PVZUE_FAIL_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ����� �� ������ (����-�)
	bool setPvzueNoiseTH(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_TH_DISC_F) * GLB_PVZUE_N_TH_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ���������� ������ (����-�)
	bool setPvzueNoiseLvl(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_LVL_DISC_F) * GLB_PVZUE_N_LVL_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ��� ������������
	bool setPvzueTypeAC(eGB_PVZUE_TYPE_AC val) {
		bool stat = false;

		if (val >= GB_PVZUE_TYPE_AC_MIN) {
			if (val <= GB_PVZUE_TYPE_AC_MAX) {
				pvzueTypeAC_ = val;
				stat = true;
			}
		}

		// ��������� ���������� ��������
		if (!stat)
			pvzueTypeAC_ = GB_PVZUE_TYPE_AC_MAX;

		return stat;
	}
	eGB_PVZUE_TYPE_AC getPvzueTypeAC() const {
		return pvzueTypeAC_;
	}

	/** ��������� ��������� ��� ���������.
	 * 	@param val ��� ���������.
	 * 	@retval True - � ������ �������� ���������.
	 * 	@retval False - ���� ��������� ������ �������� �� �������.
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

	/** ���������� �������� ���� ���������.
	 * 	@return ��� ���������.
	 */
	uint8_t getDetector() const {
		return detector_;
	}

	// ��������� ����
	bool setCorI(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_I_DEC_DISC_F) * GLB_COR_I_DEC_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ��������� ����������
	bool setCorU(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_U_DEC_DISC_F) * GLB_COR_U_DEC_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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
	// ������ �������� ���������
	uint16_t versProgIC_[GB_IC_MAX];

	// ��� ��������
	eGB_TYPE_DEVICE typeDevice_;

	// ���-�� ��������� � ����� 2 ��� 3
	eGB_NUM_DEVICES numDevices_;

	// ��� �����
	eGB_TYPE_LINE typeLine_;

	// ������ �������� ���
	uint16_t versBsp_;

	// ������ �������� DSP
	uint16_t versDsp_;

	// �������������
	eGB_COMPATIBILITY compatibility_;

	// ������������� �����
	bool timeSinchr_;

	// ��������������
	bool backup_;

	// ����� ��������
	uint8_t deviceNum_;

	// �������� ��������� �������
	bool outCheck_;

	// ����� �������������� �� ��
	uint8_t cfThreshold_;

	// ����� �����������
	uint8_t timeRerun_;

	// ��������� ���� ������ �����������
	bool comPrdKeep_;

	// ��������� ���� ������ �����������
	bool comPrmKeep_;

	// ���������� �������� �������� �������
	uint8_t inDecrease_[2];

	// ������� �����
	uint8_t netAdr_;

	// U��� �����������
	uint8_t uOutNom_;

	// �������
	uint16_t freq_;

	// �������� ������ �� (����)
	uint8_t acInDec_;

	// �������� ������ (����-�)
	eGB_PVZUE_PROTOCOL pvzueProtocol_;

	// ������� �������� (����-�)
	eGB_PVZUE_PARITY pvzueParity_;

	// ���������� ������� (����-�)
	uint8_t pvzueFail_;

	// ����� �� ������ (����-�)
	uint8_t pvzueNoiseTH_;

	// ���������� ������ (����-�)
	uint8_t pvzueNoiseLvl_;

	// ��� ������������ (����-�)
	eGB_PVZUE_TYPE_AC pvzueTypeAC_;

	// ��� ���������
	uint8_t detector_;

	// ��������� ����
	int16_t corI_;

	// ��������� ����������
	int16_t corU_;
};


#endif /* PARAMGLB_H_ */
