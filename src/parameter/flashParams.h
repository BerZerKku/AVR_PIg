/*
 * flashParams.h
 *
 *  Created on: 27.03.2015
 *      Author: Shcheblykin
 */

#ifndef FLASHPARAMS_H_
#define FLASHPARAMS_H_

#include <avr/pgmspace.h>
#include "glbDefine.h"
#include "param.h"

static const char fcNullBuf[] PROGMEM= "";

/// ������ ����� ������������, ������ � Param::DIMENSION
static const char fcDimension[] [5] PROGMEM = {
		"",		// Param::DIM_NO
		"��",	// Param::DIM_MA
		"�",	// Param::DIM_A
		"��",	// Param::DIM_KA
		"�",	// Param::DIM_V
		"��",	// Param::DIM_KV
		"��",	// Param::MSEC
		"c",	// Param::DIM_SEC
		"���",	// Param::DIM_HOUR
		"��",	// Param::DIM_DB
		"��",	// Param::DIM_HZ
		"���",	// Param::DIM_KHZ
		"����"	// Param::DIM_DEG
};

/// ��������� �������� ���������.
static const char fcOnOff[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"����.",		///< 0
		"���."			///< 1
};

/// ��������� ����� (�������� ���������).
static char fcInterface[TInterface::MAX][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"USB",			///< TInterface::USB
		"RS485"			///< TInterface::RS485
};

// �������� (�������� ���������).
static char fcProtocol[TProtocol::MAX ][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"��������",		///< TProtocol::STANDART
		"MODBUS",		///< TProtocol::MODBUS
		"���-101"		///< TProtocol::IEC_101
};

// ��������, ���/� (�������� ���������).
static char fcBaudRate[TBaudRate::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"600",			///< TBaudRate::_600
		"1200",			///< TBaudRate::_1200
		"2400",			///< TBaudRate::_2400
		"4800",			///< TBaudRate::_4800
		"9600",			///< TBaudRate::_9600
		"19200"			///< TBaudRate::_19200
};

/// ���������� ��� ������ (�������� ���������).
static char fcDataBits[TDataBits::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"8"				///< TDataBits::_8
};

/// �������� (�������� ���������).
static char fcParity[TParity::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"���",			///< TParity::NONE
		"���",			///< TParity::EVEN
		"�����"			///< TParity::ODD
};

/// �������� ���� (�������� ���������).
static char fcStopBits[TStopBits::MAX][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",			///< TStopBits::ONE
		"2"				///< TStopBits::TWO
};

// �������� ��������
static const Param fNullParam PROGMEM = {
		"",						// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_NO,		// ��� ���������
		Param::RANGE_ON_OFF,	// �������� ��������
		Param::DIM_NO,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		0,						// ����������� ��������
		1,						// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// �������
static const Param fFreq PROGMEM = {
		"�������",				// �������� ���������
		GB_COM_GET_FREQ,		// ������� ������������ ���������
		Param::PARAM_INT,		// ��� ���������
		Param::RANGE_INT_NO_DIM,// �������� ��������
		Param::DIM_KHZ,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		16,						// ����������� ��������
		1000,					// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_INT16_BE,		// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ����������� ��������
static const Param fCompDelay PROGMEM = {
		"����������� ��������",	// �������� ���������
		GB_COM_GET_COMP_DELAY,	// ������� ������������ ���������
		Param::PARAM_COMP_D,	// ��� ���������
		Param::RANGE_COMP_D,	// �������� ��������
		Param::DIM_MSEC,		// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		0,						// ����������� ��������
		10000,					// ������������ ��������
		125,					// ������������
		125,					// ��������� ��� ������������ ���������
		GB_SEND_INT8,			// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ������������ ���
static const Param fCurrMax PROGMEM = {
		"������������ ���",		// �������� ���������
		GB_COM_GET_CURR_MAX,	// ������� ������������ ���������
		Param::PARAM_INT,		// ��� ���������
		Param::RANGE_INT,		// �������� ��������
		Param::DIM_KA,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		0,						// ����������� ��������
		250,					// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_INT8,			// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ����������� ���
static const Param fCurrMin PROGMEM = {
		"����������� ���",		// �������� ���������
		GB_COM_GET_CURR_MAX,	// ������� ������������ ���������
		Param::PARAM_INT,		// ��� ���������
		Param::RANGE_INT,		// �������� ��������
		Param::DIM_KA,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		0,						// ����������� ��������
		250,					// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_INT8,			// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ����������� ����������
static const Param fVoltMin PROGMEM = {
		"�����. ����������",	// �������� ���������
		GB_COM_GET_VOLT_MIN,	// ������� ������������ ���������
		Param::PARAM_INT,		// ��� ���������
		Param::RANGE_INT,		// �������� ��������
		Param::DIM_KV,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		0,						// ����������� ��������
		1020,					// ������������ ��������
		4,						// ������������
		4,						// ��������� ��� ������������ ���������
		GB_SEND_INT8,			// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ��������� �����
static const Param fIntfInterface PROGMEM = {
		"��������� �����",		// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcInterface[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TInterface::MIN,		// ����������� ��������
		SIZE_OF(fcInterface),	// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// �������� - ��������� �����
static const Param fIntfProtocol PROGMEM = {
		"��������",				// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcProtocol[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TProtocol::MIN,			// ����������� ��������
		SIZE_OF(fcProtocol),	// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// �������� ����� - ��������� �����
static const Param fIntfBaudrate PROGMEM = {
		"�������� ��������",	// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcBaudRate[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TBaudRate::MIN,			// ����������� ��������
		SIZE_OF(fcBaudRate),	// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// ���������� ����� ������ - ��������� �����
static const Param fIntfDataBits PROGMEM = {
		"���� ������",			// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcDataBits[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TDataBits::MIN,			// ����������� ��������
		SIZE_OF(fcDataBits),	// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// �������� - ��������� �����
static const Param fIntfParity PROGMEM = {
		"��������",				// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcParity[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TParity::MIN,			// ����������� ��������
		SIZE_OF(fcParity),		// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// ����� �������� ����� - ��������� �����
static const Param fIntfStopBits PROGMEM = {
		"�������� ����",		// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������
		Param::PARAM_LIST,		// ��� ���������
		Param::RANGE_LIST,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcStopBits[0],			// ������ ��������
		1,						// ���-�� ���������� ���������
		TStopBits::MIN,			// ����������� ��������
		SIZE_OF(fcStopBits),	// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_NO,				// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_NO 	// ������� ��� ��������� ���������
};

// ����� � ��������� ����
static const Param fNetAddress PROGMEM = {
		"������� �����",		// �������� ���������
		GB_COM_NO,				// ������� ������������ ���������	// TODO
		Param::PARAM_INT,		// ��� ���������
		Param::RANGE_INT,		// �������� ��������
		Param::DIM_NO,			// �����������
		fcNullBuf,				// ������ ��������
		1,						// ���-�� ���������� ���������
		1,						// ����������� ��������
		247,					// ������������ ��������
		1,						// ������������
		1,						// ��������� ��� ������������ ���������
		GB_SEND_INT8,			// ��� ��������� ��� ���������� ����� ��������
		0,						// ���� �������������� ���������� ��� ����������
		Param::DEPEND_MAX_NO,	// ���������� ���������
		Param::DEPEND_SAME_NO,	// ����������� ����������
		Param::CHANGE_COND_REG_DISABLE // ������� ��� ��������� ���������
};

// ������ ���������� (������ � eGB_PARAM)
static const Param* fParams[] PROGMEM  = {
		&fNullParam,
		&fFreq,
		&fCompDelay,
		&fCurrMax,
		&fCurrMin,
		&fVoltMin,
		// ��������� ����������
		&fIntfInterface,
		&fIntfProtocol,
		&fIntfBaudrate,
		&fIntfDataBits,
		&fIntfParity,
		&fIntfStopBits,
		&fNetAddress
};

#endif /* FLASHPARAMS_H_ */
