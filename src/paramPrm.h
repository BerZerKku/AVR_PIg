/*
 * devicePrm.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRM_H_
#define PARAMPRM_H_

// #include "glbDefine.h"

/// ���
/// ����� ���������
#define PRM_TIME_ON_MIN		0
#define PRM_TIME_ON_MAX		10
#define PRM_TIME_ON_DISC	1
#define PRM_TIME_ON_FRACT	1
#define PRM_TIME_ON_MIN_F	(PRM_TIME_ON_MIN / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_MAX_F	(PRM_TIME_ON_MAX / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_DISC_F 	(PRM_TIME_ON_DISC / PRM_TIME_ON_FRACT)
/// �������� �� ����������
#define PRM_TIME_OFF_MIN	0
#define PRM_TIME_OFF_MAX	1000
#define PRM_TIME_OFF_DISC	50
#define PRM_TIME_OFF_FRACT	10
#define PRM_TIME_OFF_MIN_F	(PRM_TIME_OFF_MIN / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_MAX_F	(PRM_TIME_OFF_MAX / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_DISC_F	(PRM_TIME_OFF_DISC/ PRM_TIME_OFF_FRACT)
/// ������� �� � ��
#define PRM_COM_DR_MIN		1
#define PRM_COM_DR_MAX		32
#define PRM_COM_DR_DISC		1
#define PRM_COM_DR_FRACT	1
#define PRM_COM_DR_MIN_F	(PRM_COM_DR_MIN / PRM_COM_DR_FRACT)
#define PRM_COM_DR_MAX_F	(PRM_COM_DR_MAX / PRM_COM_DR_FRACT)
#define PRM_COM_DR_DISC_F	(PRM_COM_DR_DISC/ PRM_COM_DR_FRACT)


/// ����� ��� ���������� � �������� ���������
class TDevicePrm {
public:
	TDevicePrm() {
		numCom_ = 0;
		timeOn_ = PRM_TIME_ON_MIN_F;

		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			blockCom_[i] = false;
			timeOff_[i] = PRM_TIME_OFF_MIN_F;
			indCom_[i] = 0;
		}

		for(uint_fast8_t i = 0; i < MAX_NUM_COM_PRM; i++) {
			comDR_[i] = i + 1;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		stateDR_ = false;
	}

	TDeviceStatus status;

	/**	��������� �������� ���-�� ������ � ���.
	 *
	 * 	���� ����� �������� ������� �� �������� ���������� ��������, ���������
	 * 	������ 0. ����� ���� ������ ���, ��� �����������. ����� ����������.
	 *
	 * 	@param val ���������� ������ �� ���.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	bool setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRM) {
			val = 0;
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// ���� ���-�� ������ �� ����� 0, �� ���������� ���

			act |= GB_ACT_NEW;
		}
		this->status.setEnable(val != 0);

		return act;
	}

	uint8_t getNumCom() const {
		return numCom_;
	}

	// ����� ��������� �������
	bool setTimeOn(uint8_t val) {
		bool stat = false;
		val = (val / PRM_TIME_ON_DISC_F) * PRM_TIME_ON_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= PRM_TIME_ON_MIN_F) {
			if (val <= PRM_TIME_ON_MAX_F) {
				timeOn_ = val;
				stat = true;
			}
		}
		return stat;
	}

	uint8_t getTimeOn() const {
		return (timeOn_ * PRM_TIME_ON_FRACT);
	}

	// ������������� �������, ������ ��� �������� �� ��������� �������
	// num - ����� ��������� (0 - � 1 �� 8 �������, 1 - � 9 �� 16 � �.�.)
	// val - ��������
	bool setBlockCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8)) {
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
		if (num < (MAX_NUM_COM_PRM / 8))
			val = blockCom_[num];
		return val;
	}

	// �������� �� ����������
	// buf - ����� ������� �������� �������
	// �������� ���������� � 10 ��� ������ ��������� !!
	bool setTimeOff(uint8_t *buf) {
		bool stat = true;
		uint16_t val;
		for (uint_fast8_t i = 0; i < numCom_; i++) {
			val = ((*buf) / PRM_TIME_OFF_DISC_F) * PRM_TIME_OFF_DISC_F;

			// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
			if (val >= PRM_TIME_OFF_MIN_F) {
				if (val <= PRM_TIME_OFF_MAX_F)
					timeOff_[i] = val;
				else
					stat = false;
			} else
				stat = false;
			buf++;
		}
		return stat;
	}

	uint16_t getTimeOff(uint8_t num) const {
		return timeOff_[num] * PRM_TIME_OFF_FRACT;
	}

	/**	��������� ��������� "���������� ��"
	 *
	 * 	@param val ����(0)/���(1).
	 * 	@return True - � ������ �������� ������, False - �����.
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
		if (num < (MAX_NUM_COM_PRM / 8)) {
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
		if (num < (MAX_NUM_COM_PRM / 8))
			val = blockComDR_[num];
		return val;
	}

	/**	������ ������� �������� ��-������ ��� ��-������.
	 *
	 * 	@param buf ����� ������� �������� ������� �������� �� �������.
	 * 	@return True - � ������ �������� ������, False - �����.
	 */
	bool setComDR(uint8_t *buf) {
		bool stat = true;
		for (uint_fast8_t i = 0; i < numCom_; i++) {
			uint8_t val = ((*buf) / PRM_COM_DR_DISC_F) * PRM_COM_DR_DISC_F;

			// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
			if (val >= PRM_COM_DR_MIN_F) {
				if (val <= PRM_COM_DR_MAX_F)
					comDR_[i] = val;
				else
					stat = false;
			} else
				stat = false;
			buf++;
		}
		return stat;
	}

	/**	���������� ����� ��-������� ��� �������� ��-�������.
	 *
	 * 	@param num ����� ��-�������, ������� � 0.
	 * 	@return ����� ��-�������.
	 *
	 */
	uint8_t getComDR(uint8_t num) const {
		uint8_t com = 0;
		if (num < numCom_) {
			com = comDR_[num] * PRM_COM_DR_FRACT;
		}
		return com;
	}

	/** ��������� �������� ���������� ����������� �� ����� ���
	 *
	 *	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 *	@param val ��������� �����������.
	 *	@return True - � ������ �������� ������, False - �����.
	 */
	bool setIndCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			indCom_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� ������� ��������� ���������� ������� �� ����� ���.
	 *
	 * 	@param num ����� �������.
	 * 	@return True- ��������� �����, False - �����.
	 */
	bool getIndCom(uint8_t num) const {
		// ����� ���������
		uint8_t pl = num / 8;
		// ����� ������ ���������
		num = num % 8;
		return (indCom_[pl] & (1 << num)) != 0;
	}

	/**	������������� ������� ��������� ����������� ������ �� ����� ���.
	 *
	 * 	������ ��� �������� ������ ��� ��������������� �������, ������� ��� -
	 * 	������� �������. ������������� ��� �������� ��� ��������� �����.
	 *
	 * 	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 * 	@return ������� ��������� ������ ���������� ������.
	 */
	uint8_t getIndCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = indCom_[num];
		return val;
	}

	/**	�������� ������� ������� ����������� ��������� ������ �� ����� ���,
	 *
	 * 	@retval True - ���� ����� ���������� ���� �� ����� �������.
	 * 	@retval False - ������� ����������� ������ ���.
	 */
	bool isIndCom() const {
		uint8_t ind = 0;

		for(uint8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			ind |= indCom_[i];
		}

		return (ind != 0);
	}

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
	// ���-�� ������ ���������
	uint8_t numCom_;

	// ����� ��������� �������
	uint8_t timeOn_;

	// ������������� �������, true - �������������
	uint8_t blockCom_[MAX_NUM_COM_PRM / 8];

	// �������� �� ����������
	uint8_t timeOff_[MAX_NUM_COM_PRM];

	// ��
	bool stateDR_;

	// ������������� �������, true - �������������
	uint8_t blockComDR_[MAX_NUM_COM_PRM / 8];

	// ������� �� � ��
	uint8_t comDR_[MAX_NUM_COM_PRM];

	// ��������� ��������� ����������� �� ����� ���
	uint8_t indCom_[MAX_NUM_COM_PRM / 8];

	// ���-�� ������� � �������
	uint16_t numJrnEntry_;

	// ������������ ���-�� ������� � �������
	uint16_t maxNumJrnEntry_;

	// ���� ������������ �������
	bool overflow_;
};

#endif /* PARAMPRM_H_ */
