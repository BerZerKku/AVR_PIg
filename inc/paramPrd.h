/*
 * devicePrd.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRD_H_
#define PARAMPRD_H_

// #include "glbDefine.h"

/// ���
/// ����� ��������� (�������� ������������ ����������� �����)
/// ���� �� 5 �� 20, �������� �� ���������� ��
#define PRD_TIME_ON_MIN		5
#define PRD_TIME_ON_MAX		20
#define PRD_TIME_ON_DISC	1
#define PRD_TIME_ON_FRACT	1
#define PRD_TIME_ON_MIN_F	(PRD_TIME_ON_MIN / PRD_TIME_ON_FRACT)
#define PRD_TIME_ON_MAX_F	(PRD_TIME_ON_MAX / PRD_TIME_ON_FRACT)
#define PRD_TIME_ON_DISC_F	(PRD_TIME_ON_DISC / PRD_TIME_ON_FRACT)
/// ������������ ������� ��� ��
#define PRD_DURAT_L_MIN		20
#define PRD_DURAT_L_MAX		100
#define PRD_DURAT_L_DISC	1
#define PRD_DURAT_L_FRACT	1
#define PRD_DURAT_L_MIN_F	(PRD_DURAT_L_MIN / PRD_DURAT_L_FRACT)
#define PRD_DURAT_L_MAX_F	(PRD_DURAT_L_MAX / PRD_DURAT_L_FRACT)
#define PRD_DURAT_L_DISC_F	(PRD_DURAT_L_DISC / PRD_DURAT_L_FRACT)
/// ������������ ������� ��� ������
#define PRD_DURAT_O_MIN		20
#define PRD_DURAT_O_MAX		500
#define PRD_DURAT_O_DISC	10
#define PRD_DURAT_O_FRACT	10
#define PRD_DURAT_O_MIN_F	(PRD_DURAT_O_MIN / PRD_DURAT_O_FRACT)
#define PRD_DURAT_O_MAX_F	(PRD_DURAT_O_MAX / PRD_DURAT_O_FRACT)
#define PRD_DURAT_O_DISC_F	(PRD_DURAT_O_DISC / PRD_DURAT_O_FRACT)
/// ������������ ������� ��� ������
#define PRD_COM_A_MIN		0
#define PRD_COM_A_MAX		MAX_NUM_COM_PRD
#define PRD_COM_A_DISC		1
#define PRD_COM_A_FRACT		1
#define PRD_COM_A_MIN_F		(PRD_COM_A_MIN / PRD_COM_A_FRACT)
#define PRD_COM_A_MAX_F		(PRD_COM_A_MAX / PRD_COM_A_FRACT)
#define PRD_COM_A_DISC_F	(PRD_COM_A_DISC / PRD_COM_A_FRACT)


/// ����� ��� ���������� � �������� �����������
class TDevicePrd {
public:
	TDevicePrd() {
		numCom_ = 0;
		timeOn_ = PRD_TIME_ON_MIN_F;
		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
			blockCom_[i] = 0;
			longCom_[i] = 0;
			blockComDR_[i] = 0;
		}

		stateDR_ = false;
		durationL_ = PRD_DURAT_L_MIN_F;
		durationO_ = PRD_DURAT_O_MIN_F;
		testCom_ = false;
		numComA_ = PRD_COM_A_MIN_F;

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	/**	��������� �������� ���-�� ������ � ���.
	 * 	���� ����� �������� ������� �� �������� ���������� ��������, ���������
	 * 	������ 0. ����� ���� ������ ���, ��� �����������. ����� ����������.
	 * 	@param val ���������� ������ �� ���.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRD) {
			val = 0;
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// ���� ���-�� ������ �� ����� 0, �� ���������� ���
			this->status.setEnable(val != 0);
			act |= GB_ACT_NEW;
		}

		return act;
	}
	uint8_t getNumCom() const {
		return numCom_;
	}

	/**	��������� ���������� ������ ������ �
	 * 	���� ����� �������� ������� �� �������� ���������� ��������, ���������
	 * 	������ 0.
	 * 	@param val ���������� ������ �� ���.
	 * 	@return True - � ������ �������� ��������� ���������. False - �����.
	 */
	uint8_t setNumComA(uint8_t val) {
		bool stat = false;

		if (val >= PRD_COM_A_MIN_F) {
			if (val <= PRD_COM_A_MAX_F) {
				numComA_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	���������� ���-�� ������ ������ �.
	 * 	@return ���������� ������ ������ �.
	 */
	uint8_t getNumComA() const {
		return numComA_;
	}

	// ����� ��������� �������
	bool setTimeOn(uint8_t val) {
		bool stat = false;
		val = (val / PRD_TIME_ON_DISC_F) * PRD_TIME_ON_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= PRD_TIME_ON_MIN_F) {
			if (val <= PRD_TIME_ON_MAX_F) {
				timeOn_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getTimeOn() const {
		return timeOn_ * PRD_TIME_ON_FRACT;
	}

	// ������������� �������, ������ ��� �������� �� ��������� �������
	// num - ����� ��������� (0 - � 1 �� 8 �������, 1 - � 9 �� 16 � �.�.)
	// val - ��������
	bool setBlockCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			blockCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// ���������� True, ���� ������� �������������
	bool getBlockCom(uint8_t num) const {
		// ����� ���������
		uint8_t pl = num / 8;
		// ����� ������ ���������
		num = num % 8;
		return (blockCom_[pl] & (1 << num)) != 0;
	}
	// ���������� ��������� ������
	// num - ����� ��������� (0 - � 1 �� 8 �������, 1 - � 9 �� 16 � �.�.)
	uint8_t getBlockCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = blockCom_[num];
		return val;
	}

	// ���������� �������, ������ ��� �������� �� ��������� �������
	// num - ����� ��������� (0 - � 1 �� 8 �������, 1 - � 9 �� 16 � �.�.)
	// val - ��������
	bool setLongCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			longCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// ���������� True, ���� ������� �������������
	bool getLongCom(uint8_t num) const {
		// ����� ���������
		uint8_t pl = num / 8;
		// ����� ������ ���������
		num = num % 8;
		return (longCom_[pl] & (1 << num)) != 0;
	}
	// ���������� ��������� ������
	// num - ����� ��������� (0 - � 1 �� 8 �������, 1 - � 9 �� 16 � �.�.)
	uint8_t getLongCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = longCom_[num];
		return val;
	}

	// �������� �������
	// True - ��������, False - ���������
	bool setTestCom(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			testCom_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTestCom() const {
		return testCom_;
	}

	/**	��������� ��������� "������������ �������" ��� ��-�����.
	 * 	@param val ������������ �������.
	 * 	@retval True - � ������ �������� ���������.
	 * 	@retval False - ���� ��������� ������ �������� �� �������.
	 */
	bool setDurationL(uint8_t val) {
		bool stat = false;

		val = (val / PRD_DURAT_L_DISC_F) * PRD_DURAT_L_DISC_F;
		if (val >= PRD_DURAT_L_MIN_F) {
			if (val <= PRD_DURAT_L_MAX_F) {
				durationL_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	���������� �������� ��������� "������������ �������" ��� ��-�����.
	 * 	@return ������������ �������.
	 */
	uint16_t getDurationL() const {
		return durationL_ * PRD_DURAT_L_FRACT;
	}

	/**	��������� ��������� "������������ �������" ��� ������.
	 * 	@param val ������������ �������.
	 * 	@retval True - � ������ �������� ���������.
	 * 	@retval False - ���� ��������� ������ �������� �� �������.
	 */
	bool setDurationO(uint8_t val) {
		bool stat = false;

		val = (val / PRD_DURAT_O_DISC_F) * PRD_DURAT_O_DISC_F;
		if (val >= PRD_DURAT_O_MIN_F) {
			if (val <= PRD_DURAT_O_MAX_F) {
				durationL_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	���������� �������� ��������� "������������ �������" ��� ��-�����.
	 * 	@return ������������ �������.
	 */
	uint16_t getDurationO() const {
		return durationL_ * PRD_DURAT_O_FRACT;
	}

	/**	��������� ��������� "���������� ��"
	 *
	 * 	@param val ����(0)/���(1).
	 * 	@param True - � ������ �������� ������, False - �����.
	 */
	bool setStateDR(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			stateDR_ = (bool) val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� "���������� ��".
	 *
	 * 	@return ����(0)/���(1).
	 */
	/** ���������� �������� ��������� ��������������.
	 * 	@return ��������������.
	 */
	bool getStateDR() const {
		return stateDR_;
	}


	/** ������ ������ ���������� ������ �� ��� ��������� ������.
	 *
	 *	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 *	@param val ����� �������� ������.
	 *	@return True - � ������ �������� ������, False - �����.
	 */
	bool setBlockComDR8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			blockComDR_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� ������� ���� ���������� ��� �������� �������.
	 *
	 * 	@param num ����� �������.
	 * 	@return True- ������� �������������, False - �����.
	 */
	bool getBlockComDR(uint8_t num) const {
		// ����� ���������
		uint8_t pl = num / 8;
		// ����� ������ ���������
		num = num % 8;
		return (blockComDR_[pl] & (1 << num)) != 0;
	}

	/**	���������� ������� ����� ���������� ��� �������� ��������� ������.
	 *
	 *	������ ��� �������� ������ ��� ��������������� �������, ������� ��� -
	 *	������� �������. ������������� ��� �������� ��� ������� �������������.
	 *
	 * 	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 * 	@return ������� ��������� ������ ���������� ������.
	 */
	uint8_t getBlockComDR8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = blockComDR_[num];
		return val;
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
			maxNumJrnEntry_ = max;
			stat = true;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// ������� ���-�� ������
	uint8_t numCom_;

	// ����� ��������� �������
	uint8_t timeOn_;

	// ������������� �������, true - �������������
	uint8_t blockCom_[MAX_NUM_COM_PRD / 8];

	// ���������� �������, true - ����������
	uint8_t longCom_[MAX_NUM_COM_PRD / 8];

	// �������� �������. true - ���.
	bool testCom_;

	// ������������ ������� ��� �� �����
	uint8_t durationL_;

	// ������������ ������� ��� ������
	uint8_t durationO_;

	// ��
	bool stateDR_;

	// ������� ������ �
	uint8_t numComA_;

	// ������������� �������, true - �������������
	uint8_t blockComDR_[MAX_NUM_COM_PRD / 8];

	// ���-�� ������� � �������
	uint16_t numJrnEntry_;

	// ������������ ���-�� ������� � �������
	uint16_t maxNumJrnEntry_;

	// ���� ������������ �������
	bool overflow_;
};

#endif /* PARAMPRD_H_ */
