/*
 * deviceDef.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMDEF_H_
#define PARAMDEF_H_

// #include "glbDefine.h"

/// �����������, ������������ �������� ���������� ��������� � �� ������������
/// ������
/// ��� ������
#define DEF_TYPE_MIN		0
#define DEF_TYPE_MAX		8
/// ���������� ����� ��� �����������
#define DEF_T_NO_MAN_MIN	0
#define DEF_T_NO_MAN_MAX	99
#define DEF_T_NO_MAN_DISC	1
#define DEF_T_NO_MAN_FRACT	1
#define DEF_T_NO_MAN_MIN_F	(DEF_T_NO_MAN_MIN / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_MAX_F	(DEF_T_NO_MAN_MAX / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_DISC_F	(DEF_T_NO_MAN_DISC / DEF_T_NO_MAN_FRACT)
/// ���������� ���������
#define DEF_OVERLAP_MIN		0
#define DEF_OVERLAP_MIN2	18	// ������� � �400�
#define DEF_OVERLAP_MAX		54
#define DEF_OVERLAP_DISC	2
#define DEF_OVERLAP_FRACT	1
#define DEF_OVERLAP_MIN_F	(DEF_OVERLAP_MIN / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_MAX_F	(DEF_OVERLAP_MAX / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_DISC_F	(DEF_OVERLAP_DISC / DEF_OVERLAP_FRACT)
/// ����������� �������� � �����
#define DEF_DELAY_MIN		0
#define DEF_DELAY_MAX		18
#define DEF_DELAY_DISC		2
#define DEF_DELAY_FRACT		1
#define DEF_DELAY_MIN_F		(DEF_DELAY_MIN / DEF_DELAY_FRACT)
#define DEF_DELAY_MAX_F		(DEF_DELAY_MAX / DEF_DELAY_FRACT)
#define DEF_DELAY_DISC_F	(DEF_DELAY_DISC / DEF_DELAY_FRACT)
/// ����� �������������� �� ��
#define DEF_RZ_THRESH_MIN	0
#define DEF_RZ_THRESH_MAX	16
#define DEF_RZ_THRESH_DISC	1
#define DEF_RZ_THRESH_FRACT	1
#define DEF_RZ_THRESH_MIN_F	(DEF_RZ_THRESH_MIN / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_MAX_F	(DEF_RZ_THRESH_MAX / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_DISC_F (DEF_RZ_THRESH_DISC / DEF_RZ_THRESH_FRACT)
/// ����������� ���������������� �� ��
#define DEF_RZ_DEC_MIN		0
#define DEF_RZ_DEC_MAX		32
#define DEF_RZ_DEC_DISC		1
#define DEF_RZ_DEC_FRACT	1
#define DEF_RZ_DEC_MIN_F	(DEF_RZ_DEC_MIN / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_MAX_F	(DEF_RZ_DEC_MAX / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_DISC_F	(DEF_RZ_DEC_DISC / DEF_RZ_DEC_FRACT)
/// ��� ���������
#define DEF_PRM_TYPE_MIN	0
#define DEF_PRM_TYPE_MAX	3
/// ����� ��������� ������ ��� ������������ ������� ��� (����)
#define DEF_SH_FRONT_MIN	0
#define DEF_SH_FRONT_MAX	72
#define DEF_SH_FRONT_DISC	1
#define DEF_SH_FRONT_FRACT	1
#define DEF_SH_FRONT_MIN_F	(DEF_SH_FRONT_MIN / DEF_SH_FRONT_FRACT)
#define DEF_SH_FRONT_MAX_F	(DEF_SH_FRONT_MAX / DEF_SH_FRONT_FRACT)
#define DEF_SH_FRONT_DISC_F	(DEF_SH_FRONT_DISC / DEF_SH_FRONT_FRACT)
/// ����� ������� ������ ��� ������������ ������� ��� (����)
#define DEF_SH_BACK_MIN		0
#define DEF_SH_BACK_MAX		72
#define DEF_SH_BACK_DISC	1
#define DEF_SH_BACK_FRACT	1
#define DEF_SH_BACK_MIN_F	(DEF_SH_BACK_MIN / DEF_SH_BACK_FRACT)
#define DEF_SH_BACK_MAX_F	(DEF_SH_BACK_MAX / DEF_SH_BACK_FRACT)
#define DEF_SH_BACK_DISC_F	(DEF_SH_BACK_DISC / DEF_SH_BACK_FRACT)
/// ����� ���
#define DEF_SH_PRM_MIN		0
#define DEF_SH_PRM_MAX		72
#define DEF_SH_PRM_DISC	1
#define DEF_SH_PRM_FRACT	1
#define DEF_SH_PRM_MIN_F	(DEF_SH_PRM_MIN / DEF_SH_PRM_FRACT)
#define DEF_SH_PRM_MAX_F	(DEF_SH_PRM_MAX / DEF_SH_PRM_FRACT)
#define DEF_SH_PRM_DISC_F	(DEF_SH_PRM_DISC / DEF_SH_PRM_FRACT)
/// ����� �� ��� �� �����
#define DEF_SH_PRD_MIN		0
#define DEF_SH_PRD_MAX		72
#define DEF_SH_PRD_DISC	1
#define DEF_SH_PRD_FRACT	1
#define DEF_SH_PRD_MIN_F	(DEF_SH_PRD_MIN / DEF_SH_PRD_FRACT)
#define DEF_SH_PRD_MAX_F	(DEF_SH_PRD_MAX / DEF_SH_PRD_FRACT)
#define DEF_SH_PRD_DISC_F	(DEF_SH_PRD_DISC / DEF_SH_PRD_FRACT)


/// ��� ������������
enum eGB_TYPE_AC {
	GB_TYPE_AC_MIN = 1, 		// ����������� ��������
	GB_TYPE_AC_AUTO_NORM = 1,	// �� ���������� / �� �������
	GB_TYPE_AC_AUTO_REPEAT = 2,	// �� ���������
	GB_TYPE_AC_FAST = 3,		// �� ����������
	GB_TYPE_AC_OFF = 4,			// �� ��������
	GB_TYPE_AC_CHECK = 5,		// �� ������
	GB_TYPE_AC_PUSK_SELF = 6,	// ���� �� ���� / �� ��������� / �� �����.����.
	GB_TYPE_AC_PUSK = 7,		// �� ����
	GB_TYPE_AC_ONE_SIDE = 8,	// �� �������������
	GB_TYPE_AC_MAX,				// ������������ ����������� ��������
	GB_TYPE_AC_AUTO				// �������  GB_TYPE_AC_AUTO_NORM � �400�->������
};

/// ������� ��� � ��� � ����
enum eGB_PVZL_FREQ {
	GB_PVZL_FREQ_MIN 	= 0,
	GB_PVZL_FREQ_M1000 	= 0,
	GB_PVZL_FREQ_M500 	= 1,
	GB_PVZL_FREQ_0 		= 2,
	GB_PVZL_FREQ_P500 	= 3,
	GB_PVZL_FREQ_P1000 	= 4,
	GB_PVZL_FREQ_MAX
};

/// ����� ��� ���������� � �������� ������
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

	// ��� ������
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

	// ��� ����� (���-�� ��������� � �����)
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

	// ���������� ����� ��� �����������
	bool setTimeNoMan(uint8_t val) {
		bool stat = false;
		val = (val / DEF_T_NO_MAN_DISC_F) * DEF_T_NO_MAN_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ���������� ���������
	bool setOverlap(uint8_t val) {
		bool stat = false;
		val = (val / DEF_OVERLAP_DISC_F) * DEF_OVERLAP_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	/** ��������� �������� "����������� �������� �� �����".
	 * 	� 3-� �������� ��� ���������, � 2-� �������� ����.
	 * 	@param num 	����� ���������.
	 * 	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 * 	@param val �������� ���������.
	 * 	@retval True �������� ������� �������.
	 * 	@retval False ������ ������.
	 */
	bool setDelay(uint8_t num, uint8_t val) {
		bool stat = false;
		val = (val / DEF_DELAY_DISC_F) * DEF_DELAY_DISC_F;

		num = (num == 2) ? 1 : 0;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= DEF_DELAY_MIN_F) {
			if (val <= DEF_DELAY_MAX_F) {
				delay_[num] = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	���������� �������� "����������� �������� �� �����".
	 *
	 * 	� 3-� �������� ��� ���������, � 2-� �������� ����.
	 *	� ������ ���������� �������� ������ ��������� ����� ������ ������.
	 *
	 * 	@param num 	����� ���������.
	 * 	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 * 	@return ������� ���������.
	 */
	uint8_t getDelay(uint8_t num) const {
		num = (num == 2) ? 1 : 0;
		return (delay_[num] * DEF_DELAY_FRACT);
	}

	// ����� ������������� �� ��
	bool setRzThreshold(uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_THRESH_DISC_F) * DEF_RZ_THRESH_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	/** ��������� �������� "����������� ���������������� �� ��".
	 *
	 * 	� 3-� �������� ��� ���������, � 2-� �������� ����.
	 *	� ������ ���������� �������� ������ ��������� ����� ������ ������.
	 *
	 * 	@param num 	����� ���������.
	 * 	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 * 	@param val �������� ���������.
	 * 	@retval True �������� ������� �������.
	 * 	@retval False ������ ������.
	 */
	bool setRzDec(uint8_t num, uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_DEC_DISC_F) * DEF_RZ_DEC_DISC_F;

		num = (num == 2) ? 1 : 0;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= DEF_RZ_DEC_MIN_F) {
			if (val <= DEF_RZ_DEC_MAX_F) {
				rzDec_[num] = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	���������� �������� "����������� ���������������� �� ��".
	 *
	 * 	� 3-� �������� ��� ���������, � 2-� �������� ����.
	 *	� ������ ���������� �������� ������ ��������� ����� ������ ������.
	 *
	 * 	@param num 	����� ���������.
	 * 	@arg 1	������.
	 * 	@arg 2	������ (������ � 3-� ��������).
	 * 	@arg Other ������������ �������� ������� ���������.
	 * 	@return ������� ���������.
	 */
	uint8_t getRzDec(uint8_t num) const {
		num = (num == 2) ? 1 : 0;
		return (rzDec_[num] * DEF_RZ_DEC_FRACT);
	}

	// ��� ���������
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

	// ��� ������������
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

	// ����� �� ������������
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

	// �������� ������ ������������
	// True - ��������, False - ���������
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

	// ������� ��� (����)
	// True - ��������, False - ���������
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

	// ������� ��� (����)
	// True - ��������, False - ���������
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

	// ����� ��������� ������ ��� ������������ ������� ��� (����)
	bool setShiftFront(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_FRONT_DISC_F) * DEF_SH_FRONT_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ����� ������� ������ ��� ������������ ������� ��� (����)
	bool setShiftBack(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_BACK_DISC_F) * DEF_SH_BACK_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ����� ���
	bool setShiftPrm(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_PRM_DISC_F) * DEF_SH_PRM_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ����� �� ��� �� �����
	bool setShiftPrd(uint8_t val) {
		bool stat = false;
		val = (val / DEF_SH_PRD_DISC_F) * DEF_SH_PRD_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
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

	// ���������� ������� � �������
	// ���������� ������� � �������
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

	// ������������ ���-�� ������� � �������
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
	// ��� ������
	uint8_t defType_;

	// ��� ����� (���-�� ���������)
	eGB_NUM_DEVICES numDevices_;

	// ���������� ����� ��� �����������
	uint8_t timeNoMan_;

	// ���������� ���������
	uint8_t overlap_;

	// ����������� �������� � �����
	uint8_t delay_[2];

	// ����� �������������� �� ��
	uint8_t rzThreshold_;

	// ����������� ���������������� �� ��
	uint8_t rzDec_[2];

	// ��� ���������
	uint8_t prmType_;

	// ��� ������������
	eGB_TYPE_AC typeAc_;

	// ����� �� ������������ � ��������
	uint16_t timeToAc_;

	// �������� ������ ��
	bool acDec_;

	// ���-�� ������� � �������
	uint16_t numJrnEntry_;

	// ������������ ���-�� ������� � �������
	uint16_t maxNumJrnEntry_;

	// ���� ������������ �������
	bool overflow_;

	// ������� ��� (����)
	eGB_PVZL_FREQ freqPrm_;

	// ������� ��� (����)
	eGB_PVZL_FREQ freqPrd_;

	// ����� ��������� ������ ��� ������������ ������� ��� (����)
	uint8_t shFront_;

	// ����� ������� ������ ��� ������������ ������� ��� (����)
	uint8_t shBack_;

	// ����� ���
	uint8_t shPrm_;

	// ����� �� ��� �� �����
	uint8_t shPrd_;
};


#endif /* PARAMDEF_H_ */
