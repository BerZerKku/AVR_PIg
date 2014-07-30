/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 * 	������������ �������� ��� ��������� ���������� �� 1 ������ ����������,
 * 	��� �������� � ������� ���������� �������� � �������� �����������
 * 	��������������.
 * 	��� ��� ��� ����� ���������� �������� �������� ������ ���� ������ �� 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "debug.h"

/// ������ ��������������
#define PASSWORD_ADMIN 6352

/// ������ ������������ �� ���������
#define PASSWORD_USER 0

/// ������ ������� ��������
#define VERS 0x0100

/// �������������� ���� CHAR � INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// �������������� �������-����������� ����� � �����
#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))

/// �������������� ������ ����� � �������-���������� ���
#define BIN_TO_BCD(val) (((val / 10) << 4) + (val % 10))

/// ����������� ��������� ���-�� ��������� ���������
#define MAX_NUM_DEVICE_STATE 13

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_FAULTS 16

///	������������ ���-�� ������� �������
#define MAX_NUM_FAST_COM 2

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_WARNINGS 16

/// ������������ ���-�� ������ � ������ ������
#define MAX_NUM_COM_BUF1 5

/// ������������ ���-�� ������ �� ������ ������
#define MAX_NUM_COM_BUF2 5


/// ������������ � ����������� ��� ���� ������� � ������� �������
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 33


#define GLB_JRN_EVENT 	512		///< ���-�� ������� � ������� �������
#define GLB_JRN_DEF 	0		///< ���-�� ������� � ������� ������ �400
#define GLB_JRN_MAX 	2048	///< ����������� ���-�� ������� � ����� ������

/// ������������ ��������� ���.����� � ������ ������� �400/����
#define GLB_JRN_DOP_MAX 6

/// ��� ��������
enum eGB_TYPE_DEVICE {
	AVANT_NO = 0,	// ��������� ��������
	AVANT_TERMINAL
};

/// ����������
enum eGB_DEVICE {
	GB_DEVICE_MIN = 0,
	GB_DEVICE_RPS = 0,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// ��� ������ �����
enum eGB_TYPE_LINE {
	GB_TYPE_LINE_MIN = 1,
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTO = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// ���-�� ��������� � �����
// 	������ ��� 1 � 2 ������������ ��� ���������� � 3-� ��������. �� ������ !!!
enum eGB_NUM_DEVICES {
	GB_NUM_DEVICES_MIN = 1,
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// ������ ������
enum eGB_REGIME {
	GB_REGIME_MIN = 0, 				//
	GB_REGIME_DISABLED = 0,			// �������
	GB_REGIME_READY,				// �����
	GB_REGIME_ENABLED,				// ������
	GB_REGIME_TALK,					// ����
	GB_REGIME_TEST_1,				// ���� ���
	GB_REGIME_TEST_2,				// ���� ���
	GB_REGIME_MAX,
};

// ������ ������ ��� ����� � ����������
// ������� �� ��������� � eGB_REGIME
enum eGB_REGIME_ENTER {
	GB_REGIME_ENTER_MIN = 0,
	GB_REGIME_ENTER_DISABLED = 0,
	GB_REGIME_ENTER_ENABLED = 1,
	GB_REGIME_ENTER_MAX
};

/// �������
enum eGB_COM {
										// + �������� ��� ������� �����������������
	GB_COM_NO = 0,
	GB_COM_GET_SOST 			= 0x30,	// +
	GB_COM_GET_FAULT 			= 0x31,	// +
	GB_COM_GET_TIME 			= 0x32,	// +
	GB_COM_GET_MEAS 			= 0x34,	// +
	GB_COM_GET_COMP_DELAY		= 0x35,	// +
	GB_COM_GET_CURR_MAX 		= 0x36, // +
	GB_COM_GET_CURR_MIN	 		= 0x37,	// +
	GB_COM_GET_VOLT_MIN 			= 0x38,	// +
	GB_COM_GET_VERS 			= 0x3F,	// +
	GB_COM_SET_PRM_ENTER 		= 0x51,	// +
	GB_COM_SET_REG_DISABLED 	= 0x70,	// +
	GB_COM_SET_REG_ENABLED 		= 0x71,	// +
	GB_COM_SET_CONTROL 			= 0x72,	// +
	GB_COM_SET_PASSWORD 		= 0x73,	// + ! ������ � ��
	GB_COM_GET_PASSWORD 		= 0x74,	// + ! ������ � ��
	GB_COM_SET_PRM_RES_IND		= 0x9A,	// +
	GB_COM_SET_TIME 			= 0xB2,	// +
	GB_COM_SET_COMP_DELAY 		= 0xB5,	// +
	GB_COM_SET_CURR_MAX 		= 0xB6, // +
	GB_COM_SET_CURR_MIN	 		= 0xB7, // +
	GB_COM_SET_VOLT_MIN 		= 0xB8,	// +
	GB_COM_GET_JRN_CNT 			= 0xF1,	// +
	GB_COM_GET_JRN_ENTRY 		= 0xF2,	// +
	GB_COM_JRN_CLR 				= 0xFA	// ! �������� ������� �������, ������ � ��
};

/// ����� ������
enum eGB_COM_MASK {
	// ��� ����������
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_RPS = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// ������ �������
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

/// �������� ������ ����������
enum eGB_CONTROL {
	GB_CONTROL_RESET_SELF 	= 1,	//	����� ������
};


/// ����� ��� ���� � �������
class TDataTime {
public:
	// ���������� � ��������� ������
	uint8_t getSecond() const {
		return second_;
	}
	bool setSecond(uint8_t val) {
		bool stat = false;
		if (val < 60) {
			second_ = val;
			stat = true;
		} else {
			second_ = 61;
		}
		return stat;
	}

	// ���������� � ��������� �����
	uint8_t getMinute() const {
		return minute_;
	}
	bool setMinute(uint8_t val) {
		bool stat = false;
		if (val < 60) {
			minute_ = val;
			stat = true;
		} else {
			minute_ = 61;
		}
		return stat;
	}

	// ���������� � ��������� �����
	uint8_t getHour() const {
		return hour_;
	}
	bool setHour(uint8_t val) {
		bool stat = false;
		if (val < 24) {
			hour_ = val;
			stat = true;
		} else {
			hour_ = 25;
		}
		return stat;
	}

	// ���������� � ��������� ���
	uint8_t getDay() const {
		return day_;
	}
	bool setDay(uint8_t val) {
		bool stat = false;

		if ((val > 0) && (val <= getNumDaysInMonth())) {
			day_ = val;
			stat = true;
		} else {
			day_ = 32;
		}

		return stat;
	}

	// ���������� ���-�� ���� � ������
	// ���� ����� ��� ��� �� ������, ������������ ��� ��������
	uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const {
		uint8_t num = 0;

		if (month == 0) {
			month = month_;
		}

		if (year == 0) {
			year = year_;
		}

		if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
			num = 30;
		} else if (month == 2) {
			num = ((year % 4) == 0) ? 29 : 28;
		} else if ((month != 0) && (month < 13)) {
			num = 31;
		}

		return num;
	}

	// ���������� � ��������� ������
	uint8_t getMonth() const {
		return month_;
	}
	bool setMonth(uint8_t val) {
		bool stat = false;
		if ((val > 0) && (val <= 12)) {
			month_ = val;
			stat = true;
		} else {
			month_ = 13;
		}
		return stat;
	}

	// ���������� � ��������� ����
	uint8_t getYear() const {
		return year_;
	}
	bool setYear(uint8_t val) {
		bool stat = false;
		if (val < 100) {
			year_ = val;
			stat = true;
		} else {
			year_ = 0;
		}
		return stat;
	}

	// ���������� � ��������� �����������
	uint16_t getMsSecond() const {
		return msSecond_;
	}
	bool setMsSecond(uint16_t val) {
		bool stat = false;
		if (val < 1000) {
			msSecond_ = val;
			stat = true;
		} else {
			msSecond_ = 1000;
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
};

/// ����� ��� ������
class TPassword {
public:
	TPassword() {
		password_ = 10000;
		admin_ = PASSWORD_ADMIN;
	}

	// ���������� ������� ������ ������������
	uint16_t get() const {
		return password_;
	}

	// ������������� ������ ������������
	bool set(uint16_t pas) {
		bool stat = false;
		if (pas < 10000) {
			password_ = pas;
			stat = true;
		}
		return stat;
	}

	// ��������� ������������� ������ ������������
	// ���� ������� �������� ��������, ������ ����� PASSWORD_USER
	void init(uint16_t pas) {
		if (!set(pas))
			set(PASSWORD_USER);
	}

	// �������� �������� �� ���������� � ������� ������������ � ��������������
	bool check(uint16_t pas) const {
		return ((pas == password_) || (pas == admin_));
	}

private:
	// ������ ������������
	uint16_t password_;

	// ������ ��������������
	uint16_t admin_;
};

/// ����� ��� �������� ��������� ��������
class TDeviceStatus {
public:
	TDeviceStatus() {
		// ������������ ������ �� ���������
		static const char nameDev[] PROGMEM = "���";
		pName = nameDev;
		enable_ = true;
		fault_ = 0;
		faults_ = 0;
		numFaults_ = 0;
		warning_ = 0;
		warnings_ = 0;
		numWarnings_ = 0;
		regime_ = GB_REGIME_MAX;
		state_ = MAX_NUM_DEVICE_STATE;
		dopByte_ = 0;
	}

	// ������
	bool setFault(uint16_t faults) {
		fault_ = getFirstSetBit(faults);
		faults_ = faults;
		numFaults_ = getNumSetBits(faults);
		return true;
	}
	uint8_t getFault() const {
		return fault_;
	}
	uint16_t getFaults() const {
		return faults_;
	}
	uint8_t getNumFaults() const {
		return numFaults_;
	}

	// �������������
	bool setWarning(uint16_t warnings) {
		warning_ = getFirstSetBit(warnings);
		warnings_ = warnings;
		numWarnings_ = getNumSetBits(warnings);
		return true;
	}
	uint8_t getWarning() const {
		return warning_;
	}
	uint16_t getWarnings() const {
		return warnings_;
	}
	uint8_t getNumWarnings() const {
		return numWarnings_;
	}

	// ����� ������
	// ��� GLB ������������ GB_REGIME_DISABLED, � ������ ���� ��� ���������
	// ���������� ��������, ����� GB_REGIME_MAX
	bool setRegime(eGB_REGIME regime) {
		bool stat = false;
		if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
			stat = true;
		} else
			regime = GB_REGIME_MAX;
		regime_ = regime;
		return stat;
	}
	eGB_REGIME getRegime() const {
		return regime_;
	}

	//���������
	bool setState(uint8_t state) {
		bool stat = false;
		if (state < MAX_NUM_DEVICE_STATE) {
			state_ = state;
			stat = true;
		} else
			state = MAX_NUM_DEVICE_STATE;

		return stat;
	}
	uint8_t getState() const {
		return state_;
	}

	uint8_t getDopByte() const {
		return dopByte_;
	}
	bool setDopByte(uint8_t byte) {
		dopByte_ = byte;
		return true;
	}

	// ������� ����������� ������ � ��������������
	PGM_P pFaultText[MAX_NUM_FAULTS];
	PGM_P pWarningText[MAX_NUM_WARNINGS];
	PGM_P pStateText[MAX_NUM_DEVICE_STATE + 1];
	PGM_P pName;

private:
	// ������� ������������ �������������, ������������� � ���-�� ��������������
	uint8_t fault_;
	uint16_t faults_;
	uint8_t numFaults_;

	// ������� ������������ ��������������, �������������� � ���-�� ����������.
	uint8_t warning_;
	uint16_t warnings_;
	uint8_t numWarnings_;

	eGB_REGIME regime_;
	uint8_t state_;
	uint8_t dopByte_;

	// True - �������� ������� ������� ���������� � ������� ���������� ������
	bool enable_;

	// ���������� ���-�� ������������� � 1 ���
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

	// ���������� ����� �������� �������������� ����
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


/// ����� ��� ���������� ����������
class TMeasuredParameters {
public:
	TMeasuredParameters() {
		parPOut_ = 0;
		parUCtrl_= 0;
		parSN_ 	 = 0;
		parNOut_ = 0;
		parNin_  = 0;
		parFazA_ = 0;
		parFazB_ = 0;
		parFazC_ = 0;
		parIA_	 = 0;
		parIB_	 = 0;
		parIC_	 = 0;
		parUA_	 = 0;
		parUB_	 = 0;
		parUC_	 = 0;
	}

	/**	��������� ��������� Pout.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setPOut(uint8_t val) {
		bool stat = false;
		if (val <= 50) {
			parPOut_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� Pout.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getPOut() const {
		return parPOut_;
	}

	/**	��������� ��������� Uctrl.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setUCtrl(int8_t val) {
		bool stat = false;
		if ((val >= -50) && (val <= 50)) {
			parUCtrl_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� Uctrl.
	 *
	 * 	@return �������� ���������.
	 */
	int8_t getUCtrl() const {
		return parUCtrl_;
	}

	/**	��������� ��������� S_N.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setSN(int8_t val) {
		bool stat = false;
		if ((val >= -50) && (val <= 50)) {
			parSN_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� S_N.
	 *
	 * 	@return �������� ���������.
	 */
	int8_t getSN() const {
		return parSN_;
	}

	/**	��������� ��������� N_out.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setNOut(uint8_t val) {
		bool stat = false;
		if (val <= 9) {
			parNOut_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� N_out.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getNOut() const {
		return parNOut_;
	}

	/**	��������� ��������� N_in.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setNIn(uint8_t val) {
		bool stat = false;
		if (val <= 9) {
			parNin_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� N_in.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getNIn() const {
		return parNin_;
	}

	/**	��������� ��������� FazA.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setFazA(int16_t val) {
		bool stat = false;
		if ((val >= -180) && (val <= 179)) {
			parFazA_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� FazA.
	 *
	 * 	@return �������� ���������.
	 */
	int16_t getFazA() const {
		return parFazA_;
	}

	/**	��������� ��������� FazB.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setFazB(int16_t val) {
		bool stat = false;
		if ((val >= -180) && (val <= 179)) {
			parFazB_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� FazB.
	 *
	 * 	@return �������� ���������.
	 */
	int16_t getFazB() const {
		return parFazB_;
	}

	/**	��������� ��������� FazC.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setFazC(int16_t val) {
		bool stat = false;
		if ((val >= -180) && (val <= 179)) {
			parFazC_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� FazC.
	 *
	 * 	@return �������� ���������.
	 */
	int16_t getFazC() const {
		return parFazC_;
	}

	/**	��������� ��������� I_A.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setIA(uint8_t val) {
		bool stat = false;
		if (val <= 250) {
			parIA_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� I_A.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getIA() const {
		return parIA_;
	}

	/**	��������� ��������� I_B.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setIB(uint8_t val) {
		bool stat = false;
		if (val <= 250) {
			parIB_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� I_B.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getIB() const {
		return parIB_;
	}

	/**	��������� ��������� I_C.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setIC(uint8_t val) {
		bool stat = false;
		if (val <= 250) {
			parIC_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� I_C.
	 *
	 * 	@return �������� ���������.
	 */
	uint8_t getIC() const {
		return parIC_;
	}

	/**	��������� ��������� U_A.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setUA(uint16_t val) {
		bool stat = false;
		if (val <= 1020) {
			parUA_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� U_A.
	 *
	 * 	@return �������� ���������.
	 */
	uint16_t getUA() const {
		return parUA_;
	}

	/**	��������� ��������� U_B.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setUB(uint16_t val) {
		bool stat = false;
		if (val <= 1020) {
			parUB_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� U_B.
	 *
	 * 	@return �������� ���������.
	 */
	uint16_t getUB() const {
		return parUB_;
	}

	/**	��������� ��������� U_C.
	 *	@param ����� ������� ���������.
	 *	@retval True � ������ ����������� �������.
	 *	@retval False � ������ ���������� �������� (������� �� �������� ��������).
	 */
	bool setUC(uint16_t val) {
		bool stat = false;
		if (val <= 1020) {
			parUC_ = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� �������� ��������� U_C.
	 *
	 * 	@return �������� ���������.
	 */
	uint16_t getUC() const {
		return parUC_;
	}

private:
	uint8_t	 parPOut_;		///<  Pout	(   0 .. 50  )���
	int8_t	 parUCtrl_;		///<  Uctrl ( -50 .. 50  )��
	int8_t	 parSN_;		///<  S_N   ( -50 .. 50  )��
	uint8_t	 parNOut_;		///<  N_out (   0 .. 9   )
	uint8_t	 parNin_;		///<  N_in  (   0 .. 9   )
	int16_t  parFazA_;		///<  FazA  (-180 .. 179 )����.
	int16_t  parFazB_;		///<  FazB  (-180 .. 179 )����.
	int16_t  parFazC_;		///<  FazC  (-180 .. 179 )����.
	uint8_t	 parIA_;		///<  I_A	(   0 .. 250 )��
	uint8_t	 parIB_;		///<  I_B	(   0 .. 250 )��
	uint8_t	 parIC_;		///<  I_C	(   0 .. 250 )��
	uint16_t parUA_;		///<  U_A	(   0 .. 1020)��
	uint16_t parUB_;		///<  U_B	(   0 .. 1020)��
	uint16_t parUC_;		///<  U_C	(   0 .. 1020)��
};


// ����� ��� �������� ������
class TTxCom {
public:
	TTxCom() {
		clear();
	}

	// ������� ������
	void clear() {
		numCom1_ = numCom2_ = 0;
		cnt1_ = cnt2_ = 0;
		for(uint_fast8_t i = 0; i < MAX_NUM_FAST_COM; i++)
			comFast_[i] = GB_COM_NO;
		com1_[0] = com2_[0] = GB_COM_NO;
	}

	/** ������ ������� � ����� 1.
	 * 	���� num >= 0 �� ���������� ������ ��������� ������� � ������,
	 * 	���� num < 0 ������� �����������.
	 * 	@param com ��� �������.
	 * 	@param num ������ �������� � ������.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - ���� �� ������� ��������� ������� � �����.
	 */
	bool addCom1(eGB_COM com, int8_t num = -1) {
		bool stat = false;
		if (numCom1_ < MAX_NUM_COM_BUF1) {
			if (num >= 0) {
				if (num > numCom1_)
					numCom1_ = num;
				com1_[num] = com;
				stat = true;
			} else {
				com1_[numCom1_++] = com;
				stat = true;
			}
		}
		return stat;
	}

	/** ���������������� ������ ��������� � ������ 1 ������ (�� �����).
	 * 	@return ��� ������� �������.
	 */
	eGB_COM getCom1() {
		if (cnt1_ >= numCom1_)
			cnt1_ = 0;
		return com1_[cnt1_++];
	}

	/** ������ ������� � ����� 2.
	 * 	@param com ��� �������.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - ���� �� ������� ��������� ������� � �����.
	 */
	bool addCom2(eGB_COM com) {
		bool stat = false;
		if (numCom2_ < MAX_NUM_COM_BUF2) {
			com2_[numCom2_++] = com;
			stat = true;
		}
		return stat;
	}

	/** ���������������� ������ ��������� � ������ 2 ������ (�� �����).
	 * 	@return ��� ������� �������.
	 */
	eGB_COM getCom2() {
		if (cnt2_ >= numCom2_)
			cnt2_ = 0;
		return com2_[cnt2_++];
	}

	/**	������ ������� ������� � ����� �������.
	 * 	@param com ��� ������� �������
	 */
	void addFastCom(eGB_COM com) {
		for(uint_fast8_t i = 0; i < MAX_NUM_FAST_COM; i++) {
			if (comFast_[i] == GB_COM_NO) {
				comFast_[i] = com;
				break;
			}
		}
	}

	/**	���������� ������� ������� �� ������ �������.
	 * 	@return ��� ������� �������.
	 */
	eGB_COM getFastCom() {
		eGB_COM com = comFast_[0];
		// ��������, ���� �� ������� � ������
		if (com != GB_COM_NO) {
			// ������ ������ ������ ���������� ���
			// ������ ������� ���� ����� �� ���� ������� �����,
			// � ��������� ������� "�������"
			for(uint_fast8_t i = 1; i < MAX_NUM_FAST_COM; i++) {
				comFast_[i - 1] = comFast_[i];
			}
			comFast_[MAX_NUM_FAST_COM - 1] = GB_COM_NO;
		}
		return com;
	}

	/**	������ ���� ������ � �����.
	 * 	@param byte	���� ������.
	 * 	@param num ������ �������� �������.
	 */
	void setInt8(uint8_t byte, uint8_t num = 0) {
		if (num < 6)
			buf_[num] = byte;
	}

	/** ���������� ����� ������.
	 * 	@param num ������ �������� �������.
	 * 	@return ���� ������.
	 */
	uint8_t getInt8(uint8_t num = 0) const {
		uint8_t byte = 0;
		if (num < 6)
			byte = buf_[num];
		return byte;
	}

	/**	������ 2-� �������� ����� (uint16_t) � �����.
	 * 	� ������ ������ ������������ � 1 � 2 ������� ������� uint8_t.
	 * 	@param val ������.
	 */
	void setInt16(uint16_t val) {
		*((uint16_t *) (buf_ + 1)) = val;
	}

	/** ���������� 2-� �������� ����� (uint16_t) �� ������.
	 * 	������ �������� � 1 � 2 ��������� ������� uint8_t.
	 * 	@return ������.
	 */
	uint16_t getInt16() const {
		return *((uint16_t *) (buf_ + 1));
	}

	/**	���������� ��������� �� ����� ������.
	 * 	@return ��������� �� ����� ������.
	 */
	uint8_t* getBuferAddress() {
		return buf_;
	}

private:
	// ����� ������
	uint8_t buf_[6];
	// ������� ������� (�� ���������)
	eGB_COM comFast_[MAX_NUM_FAST_COM];

	// ������ ����� ������
	eGB_COM com1_[MAX_NUM_COM_BUF1];
	// ����� ������� ������� � ������ ������
	uint8_t cnt1_;
	// ���-�� ������ � ������ ������
	uint8_t numCom1_;

	// ������ ����� ������
	eGB_COM com2_[MAX_NUM_COM_BUF2];
	// ����� ������� ������� �� ������ ������
	uint8_t cnt2_;
	// ���-�� ������ �� ������ ������
	uint8_t numCom2_;


};

class TJournalEntry {
public:
	TJournalEntry() {
		clear();
	}

	void clear() {
		currentDevice_ = GB_DEVICE_MAX;
		eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;

		numJrnEntries_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		addressFirstEntry_ = 0;

		currentEntry_ = 1;
		ready_ = false;
	}

	TDataTime dataTime;

	bool setCurrentDevice(eGB_DEVICE device) {
		bool stat = false;
		if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
			currentDevice_ = device;
			stat = true;
		} else
			currentDevice_ = GB_DEVICE_MAX;
		return stat;
	}
	eGB_DEVICE getCurrentDevice() const {
		return currentDevice_;
	}

	// ���������� ������� � �������
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;

		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			if (overflow_) {
				numJrnEntries_ = maxNumJrnEntry_;
				addressFirstEntry_ = val;
			} else {
				numJrnEntries_ = val;
				addressFirstEntry_ = 0;
			}
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntries() const {
		return numJrnEntries_;
	}

	// ������������ ���-�� ������� � �������
	bool setMaxNumJrnEntries(uint16_t max) {
		bool stat = false;
		if (max <= GLB_JRN_MAX) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

	// ������������ �������
	bool isOverflow() const {
		return overflow_;
	}

	// ����� ������ ������� ������ � �������
	uint16_t getEntryAdress() const {
		return (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
	}

	// ������� ������
	uint16_t getCurrentEntry() const {
		return currentEntry_;
	}
	// ���������/���������� ������ ���������� true ���� ����� ��������
	// ���������� �� �����������
	bool setNextEntry() {
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
		if (tmp != currentEntry_) {
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}
	bool setPreviousEntry() {
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
		if (tmp != currentEntry_) {
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}

	// ���������� � ���������� ����� ��������� ���������� � ������� ������
	bool setReady() {
		return (ready_ = true);
	}
	bool isReady() const {
		return ready_;
	}

private:
	// ������� ������
	eGB_DEVICE currentDevice_;

	// ��� �������
	uint8_t eventType_;

	// ���-�� ������� � �������
	uint16_t numJrnEntries_;

	// ������������ ���-�� ������� � �������
	uint16_t maxNumJrnEntry_;

	// ������������ �������
	bool overflow_;

	// ����� ������ ������
	uint16_t addressFirstEntry_;

	// ����� ������� ������ (������������ �� ������)
	uint16_t currentEntry_;

	// ���� ��������� ���������� � ������� ������
	bool ready_;
};

#endif /* GLBDEF_H_ */
