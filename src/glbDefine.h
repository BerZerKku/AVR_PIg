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
#define VERS 0x0112

/// ����������� ���-�� ������ �� ����� (������ ���� ������ 8)
#define MAX_NUM_COM_PRM 32

/// ������������ ���-�� ������ �� �������� (������ ���� ������ 8)
#define MAX_NUM_COM_PRD 32

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

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_WARNINGS 16

///	������������ ���-�� ������� �������
// �� ������ ������ ���� ������ ��� ������ ����, �.�. ��� ���������� 2 �������
#define MAX_NUM_FAST_COM 2

/// ������������ ���-�� ������ � ������ ������
#define MAX_NUM_COM_BUF1 5

/// ������������ ���-�� ������ �� ������ ������
#define MAX_NUM_COM_BUF2 5

/// ������������ ���-�� �������� � ������
#define MAX_NUM_TEST_SIGNAL 40


/// ������������ � ����������� ��� ���� ������� � ������� �������
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 33


#define GLB_JRN_EVENT_K400_MAX 	512		/// ���-�� ������� � ������� ������� �400
#define GLB_JRN_EVENT_R400M_MAX 512		/// ���-�� ������� � ������� ������� �400�
#define GLB_JRN_EVENT_RZSK_MAX 	256		/// ���-�� ������� � ������� ������� ����
#define GLB_JRN_EVENT_OPTO_MAX 	256		/// ���-�� ������� � ������� ������� ������

#define GLB_JRN_DEF_K400_MAX 	0		///< ���-�� ������� � ������� ������ �400
#define GLB_JRN_DEF_R400M_MAX 	1024	///< ���-�� ������� � ������� ������ �400�
#define GLB_JRN_DEF_RZSK_MAX 	256		///< ���-�� ������� � ������� ������ ����
#define GLB_JRN_DEF_OPTO_MAX 	2048	///< ���-�� ������� � ������� ������ ������

#define GLB_JRN_PRM_K400_MAX 	512		///< ���-�� ������� � ������� ��������� �400
#define GLB_JRN_PRM_RZSK_MAX 	256		///< ���-�� ������� � ������� ��������� ����
#define GLB_JRN_PRM_OPTO_MAX	256		///< ���-�� ������� � ������� ��������� ������

#define GLB_JRN_PRD_K400_MAX 	512		/// ���-�� ������� � ������� ����������� �400
#define GLB_JRN_PRD_RZSK_MAX 	256		/// ���-�� ������� � ������� ����������� ����
#define GLB_JRN_PRD_OPTO_MAX	256		/// ���-�� ������� � ������� ����������� ������

/// ����������� ��������� ���-�� ������� � ����� ������
#define GLB_JRN_MAX 2048

/// ������������ ��������� ���.����� � ������ ������� �400/����
#define GLB_JRN_DOP_MAX 6

/// ��� ��������
enum eGB_TYPE_DEVICE {
	AVANT_NO = 0,	// ��������� ��������
	AVANT_R400M,	//
	AVANT_RZSK,		//
	AVANT_OPTO,		//
	AVANT_K400,		//
	AVANT_MAX
};

/// ����������
enum eGB_DEVICE {
	GB_DEVICE_MIN = 0,
	GB_DEVICE_DEF = 0,
	GB_DEVICE_PRM,
	GB_DEVICE_PRD,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// ���������� � ������� ������� �400
enum eGB_DEVICE_K400 {
	GB_DEVICE_K400_MIN = 0,
	GB_DEVICE_K400_DEF = 0,
	GB_DEVICE_K400_PRM1,
	GB_DEVICE_K400_PRM2,
	GB_DEVICE_K400_PRD,
	GB_DEVICE_K400_GLB,
	GB_DEVICE_K400_PRM1_PRM2,
	GB_DEVICE_K400_MAX
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
	GB_REGIME_ENTER_TEST_1,
	GB_REGIME_ENTER_TEST_2,
	GB_REGIME_ENTER_MAX
};

/// �������
enum eGB_COM {
										// + �������� ��� ������� �����������������
	GB_COM_NO = 0,
	GB_COM_DEF_GET_DEF_TYPE 	= 0x01,	// +
	GB_COM_DEF_GET_LINE_TYPE 	= 0x02,	// +
	GB_COM_DEF_GET_T_NO_MAN 	= 0x03,	// +
	GB_COM_DEF_GET_DELAY 		= 0x04,	// +
	GB_COM_DEF_GET_OVERLAP 		= 0x05,	// +
	GB_COM_DEF_GET_RZ_DEC 		= 0x06,	// +
	GB_COM_DEF_GET_PRM_TYPE 	= 0x07,	// + ! � �400� ��� �������� ������ ��
	GB_COM_DEF_GET_FREQ_PRD 	= 0x08,	// +
	GB_COM_DEF_GET_RZ_THRESH 	= 0x09,	// + ! � �400� ��� ������� ���
	GB_COM_DEF_GET_TYPE_AC 		= 0x0A,	// +
	GB_COM_PRM_GET_TIME_ON 		= 0x11,	// +
	GB_COM_PRM_GET_TIME_OFF 	= 0x13,	// +
	GB_COM_PRM_GET_BLOCK_COM 	= 0x14,	// +
	GB_COM_PRM_GET_DR_STATE 	= 0x17, // +
	GB_COM_PRM_GET_DR_BLOCK		= 0x18,	// +
	GB_COM_PRM_GET_DR_COM		= 0x19,	// +
	GB_COM_PRD_GET_TIME_ON 		= 0x21,	// +
	GB_COM_PRD_GET_DURATION 	= 0x22,	// +
	GB_COM_PRD_GET_BLOCK_COM 	= 0x24,	// +
	GB_COM_PRD_GET_LONG_COM 	= 0x25,	// +
	GB_COM_PRD_GET_TEST_COM 	= 0x26,	// +
	GB_COM_PRD_GET_DR_STATE 	= 0x27, // +
	GB_COM_PRD_GET_DR_BLOCK		= 0x28,	// +
	GB_COM_PRD_GET_COM_A		= 0x29,	// +
	GB_COM_GET_SOST 			= 0x30,	// +
	GB_COM_GET_FAULT 			= 0x31,	// +
	GB_COM_GET_TIME 			= 0x32,	// +
	GB_COM_GET_COR_U_I 			= 0x33,	// + ! � ������ ��� ��������������
	GB_COM_GET_MEAS 			= 0x34,	// +
	GB_COM_GET_TIME_SINCHR 		= 0x35,	// +
	GB_COM_GET_COM_PRM_KEEP 	= 0x36, // + ! � �400� ��� U��� �����������
	GB_COM_GET_COM_PRD_KEEP 	= 0x37,	// + ! � �400� ��� ��� ���������� ��������
	GB_COM_GET_NET_ADR 			= 0x38,	// +
	GB_COM_GET_TIME_RERUN 		= 0x39,	// + ! � �400� ��� ��������� ��� ��������������
	GB_COM_GET_FREQ 			= 0x3A,	// +
	GB_COM_GET_DEVICE_NUM 		= 0x3B,	// +
	GB_COM_GET_CF_THRESHOLD 	= 0x3C,	// + ! ����� �������������� � �����������
	GB_COM_GET_OUT_CHECK 		= 0x3D,	// +
	GB_COM_GET_TEST 			= 0x3E,	// +
	GB_COM_GET_VERS 			= 0x3F,	// +
	GB_COM_PRM_ENTER 			= 0x51,	// +
	GB_COM_SET_REG_DISABLED 	= 0x70,	// +
	GB_COM_SET_REG_ENABLED 		= 0x71,	// +
	GB_COM_SET_CONTROL 			= 0x72,	// +
	GB_COM_SET_PASSWORD 		= 0x73,	// + ! ������ � ��
	GB_COM_GET_PASSWORD 		= 0x74,	// + ! ������ � ��
	GB_COM_SET_REG_TEST_2 		= 0x7D,	// +
	GB_COM_SET_REG_TEST_1 		= 0x7E,	// +
	GB_COM_DEF_SET_DEF_TYPE 	= 0x81,	// +
	GB_COM_DEF_SET_LINE_TYPE 	= 0x82,	// +
	GB_COM_DEF_SET_T_NO_MAN 	= 0x83,	// +
	GB_COM_DEF_SET_DELAY 		= 0x84,	// +
	GB_COM_DEF_SET_OVERLAP 		= 0x85,	// +
	GB_COM_DEF_SET_RZ_DEC 		= 0x86,	// +
	GB_COM_DEF_SET_PRM_TYPE 	= 0x87,	// + ! � �400� ��� �������� ������ ��
	GB_COM_DEF_SET_FREQ_PRD 	= 0x88,	// +
	GB_COM_DEF_SET_RZ_THRESH 	= 0x89,	// + ! � �400� ��� ������� ���
	GB_COM_DEF_SET_TYPE_AC 		= 0x8A,	// +
	GB_COM_PRM_SET_TIME_ON 		= 0x91,	// +
	GB_COM_PRM_SET_TIME_OFF 	= 0x93,	// +
	GB_COM_PRM_SET_BLOCK_COM 	= 0x94,	// +
	GB_COM_PRM_RES_IND 			= 0x9A,	// +
	GB_COM_PRM_SET_DR_STATE 	= 0x97, // +
	GB_COM_PRM_SET_DR_BLOCK		= 0x98,	// +
	GB_COM_PRM_SET_DR_COM		= 0x99,	// +
	GB_COM_PRD_SET_TIME_ON 		= 0xA1,	// +
	GB_COM_PRD_SET_DURATION 	= 0xA2,	// +
	GB_COM_PRD_SET_BLOCK_COM 	= 0xA4,	// +
	GB_COM_PRD_SET_LONG_COM 	= 0xA5,	// +
	GB_COM_PRD_SET_TEST_COM 	= 0xA6,	// +
	GB_COM_PRD_SET_DR_STATE 	= 0xA7, // +
	GB_COM_PRD_SET_DR_BLOCK		= 0xA8,	// +
	GB_COM_PRD_SET_COM_A		= 0xA9,	// +
	GB_COM_PRD_RES_IND 			= 0xAA,	// +
	GB_COM_SET_TIME 			= 0xB2,	// +
	GB_COM_SET_COR_U_I 			= 0xB3,	// +
	GB_COM_SET_TIME_SINCHR 		= 0xB5,	// +
	GB_COM_SET_COM_PRM_KEEP 	= 0xB6, // + ! � �400� ��� U��� �����������
	GB_COM_SET_COM_PRD_KEEP 	= 0xB7, // + ! � �400� ��� ��� ���������� ��������
	GB_COM_SET_NET_ADR 			= 0xB8,	// +
	GB_COM_SET_TIME_RERUN 		= 0xB9,	// +
	GB_COM_SET_FREQ 			= 0xBA,	// +
	GB_COM_SET_DEVICE_NUM 		= 0xBB,	// +
	GB_COM_SET_CF_THRESHOLD 	= 0xBC,	// +
	GB_COM_SET_OUT_CHECK 		= 0xBD,	// +
	GB_COM_DEF_GET_JRN_CNT 		= 0xC1,	// +
	GB_COM_DEF_GET_JRN_ENTRY 	= 0xC2,	// +
	GB_COM_DEF_JRN_CLR 			= 0xCA,	// ! �������� ������� ���, ������ � ��
	GB_COM_PRM_GET_JRN_CNT 		= 0xD1,	// +
	GB_COM_PRM_GET_JRN_ENTRY 	= 0xD2,	// +
	GB_COM_PRM_JRN_CLR 			= 0xDA,	// ! �������� ������� ���, ������ � ��
	GB_COM_PRD_GET_JRN_CNT 		= 0xE1,	// +
	GB_COM_PRD_GET_JRN_ENTRY 	= 0xE2,	// +
	GB_COM_PRD_JRN_CLR 			= 0xEA,	// ! �������� ������� ���, ������ � ��
	GB_COM_GET_JRN_CNT 			= 0xF1,	// +
	GB_COM_GET_JRN_ENTRY 		= 0xF2,	// +
	GB_COM_JRN_CLR 				= 0xFA	// ! �������� ������� �������, ������ � ��
};

/// ����� ������
enum eGB_COM_MASK {
	// ��� ����������
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
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
	GB_CONTROL_RESET_UD 	= 2,	//	����� ���������� (-��)
	GB_CONTROL_RESET_AC		= 3,	//	����� ��
	GB_CONTROL_PUSK_UD_1 	= 4,	//	���� ���������� (1)
	GB_CONTROL_PUSK_UD_2 	= 5,	//	���� ���������� 2
	GB_CONTROL_PUSK_UD_ALL 	= 6,	//	���� ���������
	GB_CONTROL_CALL 		= 7,	//	�����
	GB_CONTROL_PUSK_ON 		= 8,	//	���� ���������� ���.
	GB_CONTROL_PUSK_OFF 	= 9,	//	���� ���������� ����.
	GB_CONTROL_PUSK_AC_UD 	= 10,	//	���� �� ���������
	// 						= 11
	// 						= 12
	GB_CONTROL_PUSK_UD_3 	= 13,	// ���� ���������� 3
	GB_CONTROL_REG_AC 		= 14,	//
	// 						= 15
	GB_CONTROL_MAN_1 		= 16,	// ���� ��� ���������� (1)
	GB_CONTROL_MAN_2 		= 17,	// ���� ��� ���������� 2
	GB_CONTROL_MAN_3 		= 18,	// ���� ��� ���������� 3
	GB_CONTROL_MAN_ALL 		= 19,	// ���� ��� ���������
	GB_CONTROL_MAX					//

};

/// ������� ������� �����������/��������� - ����� � ������ �������
enum eGB_STATE_COM {
	GB_STATE_COM_MIN 	= 0,		///< ����������� ��������
	GB_STATE_COM_END 	= 0,		///< ��������� �������
	GB_STATE_COM_START,				///< ������ �������
	GB_STATE_COM_MAX				///< ������������ ��������
};

/// �������� ������������ �������
enum eGB_SOURCE_COM {
	GB_SOURCE_COM_DI 	= 0, 		///< ������� ��������� � ����������� �����
	GB_SOURCE_COM_DR	= 1,		///< ������� ��������� � ��������� �����
	GB_SOURCE_COM_MAX				///< ��������� ��������
};

/// ������� � ����1 � ����2
enum eGB_TEST_SIGNAL {
	GB_SIGNAL_OFF = 0, 				//
	GB_SIGNAL_CF,					// ���� / �400�
	GB_SIGNAL_CF1,
	GB_SIGNAL_CF2,
	GB_SIGNAL_CF3,
	GB_SIGNAL_CF4,
	GB_SIGNAL_CF_NO_RZ,				// ����
	GB_SIGNAL_CF_RZ,				// ����
	GB_SIGNAL_CF2_NO_RZ,			// ����
	GB_SIGNAL_CF2_RZ,				// ����
	GB_SIGNAL_RZ,
	GB_SIGNAL_COM1_NO_RZ,			// ����
	GB_SIGNAL_COM2_NO_RZ,			// ����
	GB_SIGNAL_COM3_NO_RZ,			// ����
	GB_SIGNAL_COM4_NO_RZ,			// ����
	GB_SIGNAL_COM1_RZ,				// ����
	GB_SIGNAL_COM2_RZ,				// ����
	GB_SIGNAL_COM3_RZ,				// ����
	GB_SIGNAL_COM4_RZ,				// ����
	GB_SIGNAL_COM1,					// ������� ������ ������ ����
	GB_SIGNAL_COM2,					// ������ ��� ���������� �400
	GB_SIGNAL_COM3,					// vvvvvvvvvvvvvvvvvvvvvvvvvv
	GB_SIGNAL_COM4,
	GB_SIGNAL_COM5,
	GB_SIGNAL_COM6,
	GB_SIGNAL_COM7,
	GB_SIGNAL_COM8,
	GB_SIGNAL_COM9,
	GB_SIGNAL_COM10,
	GB_SIGNAL_COM11,
	GB_SIGNAL_COM12,
	GB_SIGNAL_COM13,
	GB_SIGNAL_COM14,
	GB_SIGNAL_COM15,
	GB_SIGNAL_COM16,
	GB_SIGNAL_COM17,
	GB_SIGNAL_COM18,
	GB_SIGNAL_COM19,
	GB_SIGNAL_COM20,
	GB_SIGNAL_COM21,
	GB_SIGNAL_COM22,
	GB_SIGNAL_COM23,
	GB_SIGNAL_COM24,
	GB_SIGNAL_COM25,
	GB_SIGNAL_COM26,
	GB_SIGNAL_COM27,
	GB_SIGNAL_COM28,
	GB_SIGNAL_COM29,
	GB_SIGNAL_COM30,				// ^^^^^^^^^^^^^^^^^^^^^^^^^^
	GB_SIGNAL_COM31,				// ������� ������ ������ ����
	GB_SIGNAL_COM32,				// ������ ��� ���������� �400
	GB_SIGNAL_CF_RZ_R400M,
	GB_SIGNAL_MAX
};



///
enum eGB_ACT {
	GB_ACT_NO 		= 0,		// �������� �� ���������
	GB_ACT_OLD		= 0x01,		// ����� �������� ��������� � ����������
	GB_ACT_NEW		= 0x02,		// ����������� ����� ��������
	GB_ACT_ERROR	= 0x04		// ��������� ��������
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

	// ���� ������
	uint8_t getDayOfWeek() const {
		return dayWeek_;
	}
	bool setDayWeek(uint8_t val) {
		bool stat = false;
		if ((val >= 1) || (val <= 7)) {
			dayWeek_ = val;
			stat = true;
		} else {
			dayWeek_ = 8;
		}
		return stat;
	}

private:
	uint16_t msSecond_;
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t dayWeek_;
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
		name = nameDev;
		enable_ = false;
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

	/**	���������� ������� ��������� ���������� ������ ��������������.
	 *
	 *	��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
	 *	��������������. ��� ��������� �� 0
	 *
	 *	@param shift ����� ��������������.
	 *	@return ��������� ��������������.
	 */
	bool isWarning(uint8_t shift=0xFF) const {
		bool s = false;
		if (shift == 0xFF) {
			s = (warnings_ != 0);
		} else if (shift < 16) {
			s = ((warnings_ & (1 << shift)) != 0);
		}
		return s;
	}

	/**	���������� ������� ��������� ���������� ������ �������������.
	 *
	 *	��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
	 *	��������������. ��� ��������� �� 0
	 *
	 *	@param shift ����� �������������.
	 *	@return ��������� �������������.
	 */
	bool isFault(uint8_t shift=0xFF) const {
		bool s = false;
		if (shift == 0xFF) {
			s = (faults_ != 0);
		} else if (shift < 16) {
			s = ((faults_ & (1 << shift)) != 0);
		}
		return s;
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
		} else {
			state_ = MAX_NUM_DEVICE_STATE;
		}

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

	// ������ � ������ ������� ����������
	// ���������� true ���� ����� �������� ���������� �� ��������
	bool setEnable(bool val) {
		uint8_t act = GB_ACT_NO;

		if (enable_ == val) {
			act |= GB_ACT_OLD;
		} else {
			enable_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}
	bool isEnable() const {
		return enable_;
	}

	// ������� ����������� ������ � ��������������
	PGM_P faultText[MAX_NUM_FAULTS];
	PGM_P warningText[MAX_NUM_WARNINGS];
	PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
	PGM_P name;

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
		voltDef_ = 0;
		voltDef2_ = 0;
		voltCf_ = 0;
		voltCf2_ = 0;
		voltNoise_ = 0;
		voltNoise2_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
		pulseWidth_ = 0;
	}

	// ����� �� ������
	int8_t getVoltageDef() const {
		return voltDef_;
	}
	bool setVoltageDef(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// ����� �� ������ ������
	int8_t getVoltageDef2() const {
		return voltDef2_;
	}
	bool setVoltageDef2(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef2_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// ����� �� ��
	int8_t getVoltageCf() const {
		return voltCf_;
	}
	bool setVoltageCf(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// ����� �� �� ������
	int8_t getVoltageCf2() const {
		return voltCf2_;
	}
	bool setVoltageCf2(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf2_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// ���������� ������
	uint8_t getVoltageOutInt() const {
		return (voltOut_ / 10);
	}
	uint8_t getVoltageOutFract() const {
		return (voltOut_ % 10);
	}
	uint16_t getVoltageOut() const {
		return voltOut_;
	}
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract) {
		bool stat = false;
		if ((voltOutInt < 100) && (voltOutFract < 10)) {
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			stat = true;
		}
		return stat;
	}

	// ��� ������
	uint16_t getCurrentOut() const {
		return curOut_;
	}
	bool setCurrentOut(uint16_t curOut) {
		bool stat = false;
		if (curOut <= 999) {
			curOut_ = curOut;
			stat = true;
		}
		return stat;
	}

	// ������������� ������
	uint16_t getResistOut() const {
		return resistOut_;
	}
	bool setResistOut(uint16_t resistOut) {
		bool stat = false;
		if (resistOut <= 999) {
			resistOut_ = resistOut;
			stat = true;
		}
		return stat;
	}

	// ������� ����
	int8_t getVoltageNoise() const {
		return voltNoise_;
	}
	bool setVoltageNoise(int8_t voltNoise) {
		bool stat = false;
		if ((voltNoise > -100) && (voltNoise < 100)) {
			voltNoise_ = voltNoise;
			stat = true;
		}
		return stat;
	}

	// ������� ���� 2
	int8_t getVoltageNoise2() const {
		return voltNoise2_;
	}
	bool setVoltageNoise2(int8_t val) {
		bool stat = false;
		if ((val > -100) && (val < 100)) {
			voltNoise2_ = val;
			stat = true;
		}
		return stat;
	}

	// ������������ ��������� �� ���������� �� ������ ���������
	uint16_t getPulseWidth() const {
		return pulseWidth_;
	}
	bool setPulseWidth(uint16_t val) {
		bool stat = false;
		if (val <= 360) {
			pulseWidth_ = val;
			stat = true;
		}
		return stat;
	}

private:
	// ����� �� ������ (-99 .. 99)��
	int8_t voltDef_;
	// ����� �� ������ ������� ������ (-99..99)��
	int8_t voltDef2_;
	// ����� �� �� (-99 .. 99)��
	int8_t voltCf_;
	// ����� �� �� ������� ������ (-99..99)��
	int8_t voltCf2_;
	// ������� ����� (-99 .. 99)��
	int8_t voltNoise_;
	// ������� ����� 2 (-99 .. 99)��
	int8_t voltNoise2_;
	// �������� ���������� ���������� �� 10 (0 .. 999)�
	uint16_t voltOut_;
	// �������� ��� (0 .. 999)��
	uint16_t curOut_;
	// �������� ������������� (0 .. 999)��
	uint16_t resistOut_;
	// ������������ ��������� �� ���������� �� ������ ��������� (0..360)�
	uint16_t pulseWidth_;
};


// ����� ��� �������� ������
class TTxCom {
	static const uint8_t BUFFER_SIZE = 6;

public:
	TTxCom() {
		clear();
	}

	// ������� ������
	void clear() {
		numCom1_ = numCom2_ = 0;
		cnt1_ = cnt2_ = 0;
		for(uint_fast8_t i = 0; i < MAX_NUM_FAST_COM; i++) {
			comFast_[i] = GB_COM_NO;
		}
		for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
			buf_[i] = 0;
		}
		com1_[0] = com2_[0] = GB_COM_NO;
	}

	/** ������ ������� � ����� 1.
	 * 	���� num > 0 �� ���������� ������ ��������� ������� � ������,
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
		if (num < BUFFER_SIZE)
			buf_[num] = byte;
	}

	/** ���������� ����� ������.
	 * 	@param num ������ �������� �������.
	 * 	@return ���� ������.
	 */
	uint8_t getInt8(uint8_t num = 0) const {
		uint8_t byte = 0;
		if (num < BUFFER_SIZE)
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

//	// ������ ���������� ���� ������
//	// source - ��������� �� ������ ������
//	// num - ���-�� ���������� ���� ������
//	void copyToBufer(uint8_t *source, uint8_t num) {
//		for (uint_fast8_t i = 0; i < num; i++) {
//			// ���� ������� ������
//			if (i >= 6)
//				break;
//			byte_[i] = *source++;
//
//		}
//	}
//	// ���������� ���������� ���� ������
//	// destination- ��������� �� ������ ������
//	// num - ���-�� ���������� ���� ������
//	void copyFromBufer(uint8_t *destination, uint8_t num) const {
//		for (uint_fast8_t i = 0; i < num; i++) {
//			// ���� ������� ������
//			if (i >= 6)
//				break;
//			*destination++ = byte_[i];
//		}
//	}

	/**	���������� ��������� �� ����� ������.
	 * 	@return ��������� �� ����� ������.
	 */
	uint8_t* getBuferAddress() {
		return buf_;
	}

private:
	// ������� ������� (�� ���������)
	eGB_COM comFast_[MAX_NUM_FAST_COM];
	// ������ ����� ������
	eGB_COM com1_[MAX_NUM_COM_BUF1];
	// ���-�� ������ � ������ ������
	uint8_t numCom1_;
	// ����� ������� ������� � ������ ������
	uint8_t cnt1_;
	// ������ ����� ������
	eGB_COM com2_[MAX_NUM_COM_BUF2];
	// ���-�� ������ �� ������ ������
	uint8_t numCom2_;
	// ����� ������� ������� �� ������ ������
	uint8_t cnt2_;
	// ����� ������
	uint8_t buf_[BUFFER_SIZE];
};

class TJournalEntry {
public:
	TJournalEntry() {
		clear();
	}

	void clear() {
		currentDevice_ = GB_DEVICE_MAX;
		deviceJrn_ = GB_DEVICE_K400_MAX;
		eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
		regime_ = GB_REGIME_MAX;
		numCom_ = 0;
		sourceCom_ = GB_SOURCE_COM_MAX;
		signalPusk_ = false;
		signalStop_ = false;
		signalMan_ = false;
		signalPrm_ = false;
		signalPrd_ = false;
		signalOut_ = false;
		signals_ = 0;

		numJrnEntries_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		addressFirstEntry_ = 0;

		currentEntry_ = 1;
		ready_ = false;
	}

	TDataTime dateTime;

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

	// ������\���������� ���������� ��� �������� ������� ������
	bool setDeviceJrn(eGB_DEVICE_K400 device) {
		bool stat = false;
		if ((device >= GB_DEVICE_K400_MIN) && (device < GB_DEVICE_K400_MAX)) {
			deviceJrn_ = device;
			stat = true;
		} else
			deviceJrn_ = GB_DEVICE_K400_MAX;
		return stat;
	}
	eGB_DEVICE_K400 getDeviceJrn() const {
		return deviceJrn_;
	}

	// ��� �������
	bool setEventType(uint8_t val) {
		bool state = false;

		uint8_t min = 255;
		uint8_t max = 0;
		// ��������� ���/���� �������� �������, � ����������� ��
		// �������� �������
		if (currentDevice_ == GB_DEVICE_GLB) {
			min = MIN_JRN_EVENT_VALUE;
			max = MAX_JRN_EVENT_VALUE;
		} else if (currentDevice_ == GB_DEVICE_PRD) {
			min = GB_STATE_COM_MIN;
			max = GB_STATE_COM_MAX;
		} else if (currentDevice_ == GB_DEVICE_PRM) {
			min = GB_STATE_COM_MIN;
			max = GB_STATE_COM_MAX;
		} else if (currentDevice_ == GB_DEVICE_DEF) {
			min = 0;
			max = MAX_NUM_DEVICE_STATE;
		}

		if ((val >= min) && (val <= max)) {
			eventType_ = val;
			state = true;
		} else
			eventType_ = max;

		return state;
	}
	uint8_t getEventType() const {
		return eventType_;
	}


	/**	��������� ������� ��� �������� ������.
	 * 	����������� 4 �����, ������� ������.
	 * 	������ ������������� ��� � ��� �������� �� ���� �������.
	 * 	@param buf ��������� �� ������ �� 4 ���� ������.
	 * 	@retval True - ������.
	 */
	bool setOpticEntry(uint8_t *buf) {
		uint8_t num = 0;
		uint8_t byte = 0;

		// � ������ ����� ������ ��������� ���-�� ������������� �����
		for(uint_fast8_t i = 0; i <= 3; i++) {
			byte = *buf++;
			opticEntry_[i] = byte;
			for(uint_fast8_t j = 1; j > 0; j <<= 1) {
				if (byte & j) {
					num++;
				}
			}
		}
		numOpticEntries_ = num;

		return true;
	}

	/** ���������� ���-�� �������� ������� � ������� ������.
	 * 	��� ������� ������� ��� ���-�� �������, ��� ������ - ������.
	 * 	@return ���-�� �������� �������� ������� ������
	 */
	uint8_t getNumOpticsEntries() const {
		return numOpticEntries_;
	}

	/** ���� � ���������� ��� �������, ������ �� ������ �������.
	 * 	�.�. ���� � ��� � ������ 5 �������� �������, ��� ������� �� ���
	 * 	����� �������� ��� ������ (�������� ����� �������).
	 *
	 * 	@param num ����� �������, ������� � 1.
	 * 	@return ��� ������� (0 - � ������ ������).
	 */
	uint8_t getOpticEntry(uint8_t num) {
		uint8_t val = 0;
		uint8_t byte = 0;

		// �������� �� ���������� ��������
		if ((num >= 1) && (num <= numOpticEntries_)) {
			for(uint_fast8_t i = 0; i <= 3; i++) {
				// ������� 4-� ����, ������� � ��������
				byte = opticEntry_[3 - i];
				for(uint_fast8_t j = 0; j < 8; j++) {
					if (byte & (1 << j)) {
						if (--num == 0) {
							// ����� �������������� ����, �� 1 �� 32
							val = 1 + ((i * 8) + j);
						}
					}
				}
			}
		}
		return val;
	}

	// �����
	bool setRegime(eGB_REGIME regime) {
		bool state = false;
		if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
			regime_ = regime;
			state = true;
		} else
			regime_ = GB_REGIME_MAX;
		return state;
	}
	eGB_REGIME getRegime() const {
		return regime_;
	}

	// ������� ��� ������� ������
	bool setSignalDef(uint8_t val) {
		bool state = true;

		signalPusk_ = (val & 0x10) ? 1 : 0;
		signalStop_ = (val & 0x20) ? 1 : 0;
		signalMan_ = (val & 0x40) ? 1 : 0;
		signalPrm_ = (val & 0x01) ? 1 : 0;
		signalPrd_ = (val & 0x02) ? 1 : 0;
		signalOut_ = (val & 0x03) ? 1 : 0;
		signals_ = val;

		return state;
	}
	uint8_t getSignalPusk() const {
		return signalPusk_;
	}
	uint8_t getSignalStop() const {
		return signalStop_;
	}
	uint8_t getSignalMan() const {
		return signalMan_;
	}
	uint8_t getSignalPrm() const {
		return signalPrm_;
	}
	uint8_t getSignalPrd() const {
		return signalPrd_;
	}
	uint8_t getSignalOut() const {
		return signalOut_;
	}
	uint8_t getSignals() const {
		return signals_;
	}

	// ����� �������
	bool setNumCom(uint8_t num) {
		bool stat = false;
		if ((num > 0) && (num <= MAX_NUM_COM_PRD)) {
			numCom_ = num;
			stat = true;
		}
		return stat;
	}
	uint8_t getNumCom() const {
		return numCom_;
	}

	/** ��������� ��������� ������������ �������.
	 *	� ������ �� ����������� ��������, � �������� ����� �������� ��������
	 *	\a GB_SOURCE_COM_MAX.
	 *
	 *	@see eGB_SOURCE_COM
	 *	@param source �������� ������������ �������.
	 *	@return True - � ������ ����������� ��������, False - �����.
	 */
	bool setSourceCom(eGB_SOURCE_COM source) {
		bool stat = false;

		switch(source) {
			case GB_SOURCE_COM_DI:
			case GB_SOURCE_COM_DR:
			case GB_SOURCE_COM_MAX: {
				stat = true;
			}
			break;
		}

		if (!stat) {
			source = GB_SOURCE_COM_MAX;
		}

		sourceCom_ = source;
		return stat;
	}

	/** ���������� ��������� ������������ �������.
	 *
	 *	@return �������� ������������ �������.
	 */
	eGB_SOURCE_COM getSourceCom() const {
		return sourceCom_;
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

	// ��������� ������ ������
	bool setNumEntry(uint16_t num) {
		bool stat = false;
		if (num <= numJrnEntries_) {
			currentEntry_ = num;
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

	// ����������
	eGB_DEVICE_K400 deviceJrn_;

	// ��� �������
	uint8_t eventType_;

	// �����
	eGB_REGIME regime_;

	// ������� ��� ������� ������
	bool signalPusk_;
	bool signalStop_;
	bool signalMan_;
	bool signalPrm_;
	bool signalPrd_;
	bool signalOut_;
	uint8_t signals_;

	// ����� ������ ��� �������� ������
	uint8_t opticEntry_[4];

	// ���-�� ������� � ����� ������ ������� ������
	uint8_t numOpticEntries_;

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

	// ����� ������� � ������� ������
	uint16_t numCom_;

	// �������� ������������ �������
	eGB_SOURCE_COM sourceCom_;

	// ���� ��������� ���������� � ������� ������
	bool ready_;
};

class TTest {
public:
	TTest() {
		clear();
	}
	// ������� ������ ��������
	void clear() {
		for(uint_fast8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++)
			signalList[i] = GB_SIGNAL_OFF;
		num_ = 1;
		currentSignal_ = GB_SIGNAL_OFF;
		currentSignal2_ = GB_SIGNAL_OFF;
	}

	bool addSignalToList(eGB_TEST_SIGNAL signal) {
		bool stat = false;
		if ((num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX)) {
			signalList[num_++] = signal;
			stat = true;
		}
		return stat;
	}

	/**	�������������� ��������� ������� � �������� ��� �������� � ���.
	 * 	@param [out] cf	��� ������� ��/�������.
	 * 	@param [out] rz	��� ������� ��.
	 *	@param sig �������� ������.
	 *
	 */
	void getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig) {
		if ((sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1; // 3 - ���-�� �� ?!
		} else if ((sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4)) {
			rz = 0;
			cf = 1 + sig - GB_SIGNAL_CF1;
		} else if (sig == GB_SIGNAL_CF_NO_RZ) {
			rz = 1;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF_RZ) {
			rz = 2;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF2_NO_RZ) {
			rz = 1;
			cf = 2;
		} else if (sig == GB_SIGNAL_CF2_RZ) {
			rz = 2;
			cf = 2;
		} else if (sig == GB_SIGNAL_RZ) {
			rz = 1;
			cf = 0;
		} else if ((sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM4_RZ)) {
			rz = 2;
			cf = 3 + sig - GB_SIGNAL_COM1_RZ; // 3 - ���-�� �� ?!
		} else if ((sig>=GB_SIGNAL_COM1_NO_RZ)&&(sig<=GB_SIGNAL_COM4_NO_RZ)) {
			rz = 1;
			cf = 3 + sig - GB_SIGNAL_COM1_NO_RZ; // 3 - ���-�� �� ?!
		} else if (sig == GB_SIGNAL_CF) {
			rz = 0;
			cf = 1;
		} else {
			rz = 0;
			cf = 0;
		}
	}

	/** ���������� ������������ ���-�� �������� � �����.
	 *	@return ������������ ���-�� �������� � �����.
	 */
	uint8_t getNumSignals() const {
		return num_;
	}

	/** ����������� �������� ������� ������������� / ������������� � ������.
	 *
	 * 	� ����� ��������� ������ ����������� 6 ���� ������.
	 * 	� ����� �����������: � ����/�400� 3 �����, � �400/������ 5 ����.
	 *
	 * 	� 3-� �������� ��������, ���-�� ���� ������ �����������.
	 *
	 * 	@param s ��������� �� ������ ������� ������.
	 * 	@param type ��� ��������.
	 * 	@param numBytes ������������ ���������� ������.
	 */
	void setCurrentSignal(uint8_t *s, eGB_TYPE_DEVICE type) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;
		eGB_TEST_SIGNAL signal2 = GB_SIGNAL_MAX;

		if (type == AVANT_K400) {
			signal = getCurrentSignalK400(s);
			signal2 = getCurrentSignalK400((s + 5));
		} else if (type == AVANT_RZSK) {
			signal = getCurrentSignalRZSK(s);
			signal2 = getCurrentSignalRZSK((s + 3));
		} else if (type == AVANT_R400M) {
			signal = getCurrentSignalR400M(s);
		} else if (type == AVANT_OPTO) {
			signal = getCurrentSignalOpto(s);
		}
		currentSignal_ = signal;
		currentSignal2_ = signal2;
	}

	/** ���������� ������� ������ � ����� ��� ������� ���������.
	 * 	@return ������� ������ � �����.
	 */
	eGB_TEST_SIGNAL getCurrentSignal() const {
		return currentSignal_;
	}

	/** ���������� ������� ������ � ����� ��� ������� ���������.
	 * 	@return ������� ������ � �����.
	 */
	eGB_TEST_SIGNAL getCurrentSignal2() const {
		return currentSignal2_;
	}

	// ������ ��������
	uint8_t signalList[MAX_NUM_TEST_SIGNAL];

private:
	// ���-�� ��������� �������� � �����
	uint8_t num_;

	// ������� ������
	eGB_TEST_SIGNAL currentSignal_;

	// ������� ������ ������� ���������
	eGB_TEST_SIGNAL currentSignal2_;

	// ���������� ����� ������� �������������� ���� 1..8, ���� 0
	// �������� ���������� � 0-��� ����
	uint8_t getSetBit(uint8_t byte) {
		uint8_t bit = 0;

		if (byte) {
			uint8_t b = 0;
			for (uint_fast8_t i = 1; i > 0; i <<= 1) {
				b++;
				if (byte & i) {
					bit = b;
					break;
				}
			}
		}
		return bit;
	}

	/** ���������� ������� � ������ ��� �400.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		x 		x		x 		[��2]	[��1]	;
	 * 	b2 : [���8] [���7]	[���6] 	[���5] 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	b3 : [���16][���15] [���14] [���13] [���12] [���11] [���10] [���9]	;
	 * 	b4 : [���24][���23] [���22] [���21] [���20] [���19] [���18] [���17]	;
	 * 	b5 : [���32][���31] [���30] [���29] [���28] [���27] [���26] [���25]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	����� ������� �� ������� �������������� ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalK400(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		// ������� ����������� ������� ��1-��2
		if (t & 0x01)
			signal = GB_SIGNAL_CF1;
		else if (t & 0x02)
			signal = GB_SIGNAL_CF2;
		else {
			// ����������� ������� ������ � 1 �� 8
			t = getSetBit(*(++s));
			if (t) {
				t = GB_SIGNAL_COM1 + t - 1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// ����������� ������� ������ � 9 �� 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// ����������� ������� ������ � 17 �� 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// ����������� ������� ������ � 25 �� 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** ���������� ������� � ������ ��� ����.
	 * 	���: 7	6	5	4	3		2		1		0		;
	 * 	b1 : 0 	0 	0	0	[��2] 	[��1] 	[��2] 	[��1]	;
	 * 	b2 : 0	0	0 	0 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	���� ���� ������ ��, �� ��� �� ������ �� ��. ������� ��� ��1 ����!
	 * 	��1 + ���N = ������� ��� ����;
	 * 	��2 + ���N = ������� � ����;
	 * 	��1 + ��2  = ��2 ��� ����;
	 * 	��2 + ��2  = ��2 � ����;
	 * 	��1 + ��1  = �� ��� ����;
	 * 	��2 + ��1  = �� � ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalRZSK(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t b1 = (*s) & 0x0F;
		uint8_t b2 = (*(++s)) & 0x0F;

		if (b2) {
			b2 = getSetBit(b2) - 1;
			if (b1 & 0x04) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_NO_RZ);
			} else if (b1 & 0x08) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_RZ);
			}
		} else if (b1) {
			// ���� ���� ��, �� ���� � ��
			if ((b1 & 0x06) == 0x06) {
				signal = GB_SIGNAL_CF2_NO_RZ;
			} else if ((b1 & 0x0A) == 0x0A) {
				signal = GB_SIGNAL_CF2_RZ;
			} else if ((b1 & 0x05) == 0x05) {
				signal = GB_SIGNAL_CF_NO_RZ;
			} else if ((b1 & 0x09) == 0x09)  {
				signal = GB_SIGNAL_CF_RZ;
			}
		}

		return signal;
	}

	/** ���������� ������� � ������ ��� �400�.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		[��]	x		x 		x 		[��]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	������������ ����� �������������� ��� �������.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalR400M(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		// TODO �400� 3-� �������� ����� �������� �����
		// �������� ��� �������� ��� ������� �� + ��1, ��2 � �.�.

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;

			// ��������� "�� � ��" ��� �� ������������ �������
			if (t & 0x01)
				signal = GB_SIGNAL_CF_RZ_R400M;
		} else if (t & 0x01)
			signal = GB_SIGNAL_CF;

		return signal;
	}

	/** ���������� ������� � ������ ��� ������.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		x 		x		x 		x 		[��]	;
	 * 	b2 : [���8] [���7]	[���6] 	[���5] 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	b3 : [���16][���15] [���14] [���13] [���12] [���11] [���10] [���9]	;
	 * 	b4 : [���24][���23] [���22] [���21] [���20] [���19] [���18] [���17]	;
	 * 	b5 : [���32][���31] [���30] [���29] [���28] [���27] [���26] [���25]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 *	����� ������� �� ������� �������������� ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalOpto(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x01) {
			signal = GB_SIGNAL_CF;
		} else {
			t = getSetBit(*(++s));
			if (t != 0) {
				t = (t - 1) + GB_SIGNAL_COM1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// ����������� ������� ������ � 9 �� 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// ����������� ������� ������ � 17 �� 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// ����������� ������� ������ � 25 �� 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}
};

#endif /* GLBDEF_H_ */
