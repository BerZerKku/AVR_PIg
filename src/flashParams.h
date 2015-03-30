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

#define STRING_LENGHT 11

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
		"����.",		///<
		"���."			///<
};

/// ��� ������ (�������� ������).
static const char fcDefType[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"���-����",		///<
		"���-��1",		///<
		"���-��2",		///<
		"���-L60",		///<
		"��-����",		///<
		"��-��",		///<
		"���-����",		///<
		"���-��"		///<
};

/// ��� ����� (�������� �����).
static const char fcNumDevices[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"2 ��������",	///<
		"3 ��������"	///<
};

/// ��� ��������� (�������� �����).
static const char fcPrmType[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"���+����",		///<
		"��������",		///<
		"���������"	///<
};

/// ��� ������������� (�������� ������).
static const char fcCompatibility[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"����� �400",	///<
		"���-90",		///<
		"����-80",		///<
		"����-�",		///<
		"����",			///<
		"�����-�"		///<
};

/// ��� ������������� (�������� �400).
static const char fcCompK400[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"�����",		///<
		"����� ���",	///<
		"����� ���",	///<
		"���� ���",		///<
		"���� ���",		///<
		"���� ���",		///<
		"���� ���",		///<
		"���-� ���",	///<
		"���-� ���",	///<
		"���� ���",		///<
		"���� ���"		///<
};

/// ���������� ������.
static const char fcK400NumCom[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"0",			///<
		"4",			///<
		"8",			///<
		"12",			///<
		"16",			///<
		"24",			///<
		"32"			///<
};

/// ������� ��� / ��� (�������� ������).
static const char fcPvzlFreq[][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"-1000��",		///<
		"-500��",		///<
		"0��",			///<
		"+500��",		///<
		"+1000��"		///<
};

/// �������� ������ (�������� �����).
static
const char fcPvzueProtocol[][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"�������",		///<
		"���������"	///<
};

/// ��� ����������� (�������� �����).
static char fcPvzueTypeAC[][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"���������",	///<
		"����������",	///<
		"���������"		///<
};

/// ������� �������� (�������� �����).
static char fcPvzueParity[][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"���.",			///<
		"����."			///<
};

//								   				   12345678901234567890
static const char nameBackup[]			PROGMEM = "��������������";
static const char nameCompK400[]		PROGMEM = "�������������";
static const char nameCompP400[]		PROGMEM = "�������������";
static const char nameComPrdKeep[]		PROGMEM = "�����. ���� ���. ���";
static const char nameComPrmKeep[] 		PROGMEM = "�����. ���� ���. ���";
static const char nameCorI[]			PROGMEM = "��������� ����";
static const char nameCorU[]			PROGMEM = "��������� ����������";
static const char nameDefType[] 		PROGMEM = "��� ������";
static const char nameDelay[]			PROGMEM = "����������� ��������";
static const char nameDetector[]		PROGMEM = "��� ���������";
static const char nameFreq[]			PROGMEM = "�������";
static const char nameFreqPrd[]			PROGMEM = "������� ���";
static const char nameFreqPrm[]			PROGMEM = "������� ���";
static const char nameInDec[]			PROGMEM = "�������. ������. ���";
static const char nameInDecAc[]			PROGMEM = "�������� ������ ��";
static const char nameInDecAcAnswer[]	PROGMEM = "�������� ������ ��";
static const char nameNumOfDevice[] 	PROGMEM = "����� ��������";
static const char nameNumOfDevices[]	PROGMEM = "��� �����";
static const char nameNetAddress[]		PROGMEM = "������� �����";
static const char nameOutCheck[] 		PROGMEM = "�������� ���.�������";
static const char nameOverlap[]			PROGMEM = "���������� ���������";
static const char namePrdComBlock[]		PROGMEM = "��������. �������";
static const char namePrdComLong[]		PROGMEM = "�������� �������";
static const char namePrdComNumbers[]	PROGMEM = "���������� ������";
static const char namePrdComNumbersA[]	PROGMEM = "���������� ������ �";
static const char namePrdDrComBlock[]	PROGMEM = "��������. ������� ��";
static const char namePrdDrEnable[]		PROGMEM = "���������� ��";
static const char namePrdDuration[]		PROGMEM = "������������ �������";
static const char namePrdInDelay[]		PROGMEM = "�������� ������. ���";
static const char namePrmComBlock[]		PROGMEM = "��������. �������";
static const char namePrmComNumbers[]	PROGMEM = "���������� ������";
static const char namePrmDrComToHF[]	PROGMEM = "������� �� � ��";
static const char namePrmDrComBlock[]	PROGMEM = "��������. ������� ��";
static const char namePrmDrEnable[]		PROGMEM = "���������� ��";
static const char namePrmTimeOff[]		PROGMEM = "�������� �� ����.���";
static const char namePrmTimeOn[]		PROGMEM = "�������� �� ����.���";
static const char namePrmType[]			PROGMEM = "��� ���������";
static const char namePvzueAcType[]		PROGMEM = "��� ������������";
static const char namePvzueFail[]		PROGMEM = "���������� �������";
static const char namePvzueNoiseLvl[]	PROGMEM = "���������� ������";
static const char namePvzueNoiseTh[]	PROGMEM = "����� �� ������";
static const char namePvzueParity[]		PROGMEM = "������� ��������";
static const char namePvzuePeriodAC[]	PROGMEM = "������ ������� ��";
static const char namePvzuePeriodReAC[]	PROGMEM = "������ ����.����. ��";
static const char namePvzueProtocol[]	PROGMEM = "�������� ������";
static const char nameSensDec[]			PROGMEM = "����������� ��������";
static const char nameSensDecRz[]		PROGMEM = "�������. ������. ��";
static const char nameShiftBack[]		PROGMEM = "����� ���.������ ���";
static const char nameShiftFront[]		PROGMEM = "����� ���.������ ���";
static const char nameShiftPrd[]		PROGMEM = "����� �� ��� �� ����";
static const char nameShiftPrm[]		PROGMEM = "����� ���";
static const char nameTestCom[]			PROGMEM = "�������� �������";
static const char nameTimeNoMan[]		PROGMEM = "���. ����� ��� ���.";
static const char nameTimeRerun[]		PROGMEM = "����� �����������";
static const char nameTimeSynch[] 		PROGMEM = "������������� �����";
static const char nameTmK400[]			PROGMEM = "������������";
static const char nameUOutNom[]			PROGMEM = "U��� �����������";
static const char nameWarnThreshold[]	PROGMEM = "����� ��������������";
static const char nameWarnThresholdCf[]	PROGMEM = "����� �������. �� ��";
static const char nameWarnThresholdRz[]	PROGMEM = "����� �������. �� ��";

//	GB_PARAM_PRM_DR_COM,		///< ������� �� � ��
//	GB_PARAM_PRM_COM_NUMS		///< ���������� ������

/// ��������� (������� � eGB_PARAM)
static const Param fcParams[] PROGMEM = {
		// ��������
		{fcNullBuf,				GB_COM_NO,					Param::PARAM_NO,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ����� ��������
		// ������������� �����
		{nameTimeSynch,			GB_COM_GET_TIME_SINCHR,		Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ����� �������� (2-� ��������)
		{nameNumOfDevice,		GB_COM_GET_DEVICE_NUM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		2,		1,		1},
		// ����� �������� (3-� ��������)
		{nameNumOfDevice,		GB_COM_GET_DEVICE_NUM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		3,		1,		1},
		// �������� ��������� �������
		{nameOutCheck, 			GB_COM_GET_OUT_CHECK,		Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ����� ��������������
		{nameWarnThreshold,		GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0, 		22,		1,		1},
		// ����� �������������� �� �� (��� ����)
		{nameWarnThresholdCf,	GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0, 		22,		1,		1},
		// ����� �����������
		{nameTimeRerun,			GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC,		fcNullBuf,			1,	 	0,		5,		1,		1},
		// ��������� ���� ������ ���
		{nameComPrdKeep,		GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ��������� ���� ������ ���
		{nameComPrmKeep,		GB_COM_GET_COM_PRM_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ����������� ���������������� ��� (2-� ��������)
		{nameInDec,				GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		22,		1,		1},
		// ����������� ���������������� ��� (3-� ��������)
		{nameInDec,				GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			2,		0,		22,		1,		1},
		// ����� � ��������� ����
		{nameNetAddress,		GB_COM_GET_NET_ADR,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		247,	1,		1},
		// ������������ �������� ����������
		{nameUOutNom,			GB_COM_GET_COM_PRM_KEEP,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_V,		fcNullBuf,			1,		18,		50,		1,		1},
		// �������
		{nameFreq,				GB_COM_GET_FREQ,			Param::PARAM_INT,	Param::RANGE_INT_NO_DIM,Param::DIM_KHZ,		fcNullBuf,			1,		16,		1000,	1,		1},
		// ������������� (�400, �400�)
		{nameCompP400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcCompatibility[0],	1,		0,		5,		1,		1},
		// �������� ������ �� (����)
		{nameInDecAcAnswer,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		20,		1,		1},
		// ��� ���������
		{nameDetector,			GB_COM_GET_TIME_SINCHR,		Param::PARAM_INT,  	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		3,		1,		1},
		// ��������� ����������
		{nameCorU,				GB_COM_GET_COR_U_I,			Param::PARAM_U,		Param::RANGE_U_COR,		Param::DIM_V,		fcNullBuf,			1,		0,		600,	1,		1},
		// ��������� ����
		{nameCorI,				GB_COM_GET_COR_U_I,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MA,		fcNullBuf,			1,		0,		999,	1,		1},
		// �������� ������ (����-�)
		{namePvzueProtocol,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueProtocol[0],	1,		1,		2,		1,		1},
		// ������� �������� (����-�)
		{namePvzueParity,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueParity[0],	1,		1,		2,		1,		1},
		// ���������� ������� (����-�)
		{namePvzueFail,			GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		80,		2,		1},
		// ����� �� ������ (����-�)
		{namePvzueNoiseTh, 		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC,		fcNullBuf,			1,		0,		255,	1,		1},
		// ���������� ������ (����-�)
		{namePvzueNoiseLvl,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		20,		80,		20,		1},
		// ��� ������������ (����-�)
		{namePvzueAcType,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueTypeAC[0],	1,		1,		3,		1,		1},
		// ������ ������� ������ �� (����-�)
		{namePvzuePeriodAC,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC, 	fcNullBuf,			1,		1,		255,	1,		1},
		//	������ ������� ������� ������ �� (����-�)
		{namePvzuePeriodReAC,	GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC, 	fcNullBuf,			1,		1,		255,	1,		1},
		// ��������������
		{nameBackup,			GB_COM_GET_COR_U_I,			Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcNullBuf,			1,		0,		1,		1,		1},
		// ������������� �400
		{nameCompK400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcCompK400[0],		1, 		0,		10,		1,		1},
		// ��� ����� (���-�� ��������� � �����)
		{nameNumOfDevices,		GB_COM_DEF_GET_LINE_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcNumDevices[0],	1,		1,		2,		1,		1},
		// ������������
		{nameTmK400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ��������� ������
		// ��� ������
		{nameDefType, 			GB_COM_DEF_GET_DEF_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcDefType[0],		1,		0,		7,		1,		1},
		// �������������� ����� ��� �����������
		{nameTimeNoMan,			GB_COM_DEF_GET_T_NO_MAN,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_HOUR,	fcNullBuf,			1,		0,		99,		1,		1},
		// ���������� ���������
		{nameOverlap,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		54,		2,		1},
		// ���������� ��������� �400
		{nameOverlap,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		18,		54,		2,		1},
		// ����������� �������� � ����� (2-� ��������)
		{nameDelay,				GB_COM_DEF_GET_DELAY, 		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf, 			1, 		0, 		18,		2,		1},
		// ����������� �������� � ����� (3-� ��������)
		{nameDelay,				GB_COM_DEF_GET_DELAY, 		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf, 			2, 		0, 		18,		2,		1},
		// ����� �������������� �� ��
		{nameWarnThresholdRz,	GB_COM_DEF_GET_RZ_THRESH,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		16,		1,		1},
		// ����������� ����������������
		{nameSensDec,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// ����������� ���������������� �� (2-� ��������)
		{nameSensDecRz,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// ����������� ���������������� �� (3-� ��������)
		{nameSensDecRz,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// ��� ���������
		{namePrmType,			GB_COM_DEF_GET_PRM_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPrmType[0],		1,		0,		2,		1,		1},
		// �������� ������ ��
		{nameInDecAc,			GB_COM_DEF_GET_PRM_TYPE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ������� ���
		{nameFreqPrd,			GB_COM_DEF_GET_FREQ_PRD,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzlFreq[0],		1,		0,		4,		1,		1},
		// ������� ���
		{nameFreqPrm,			GB_COM_DEF_GET_RZ_THRESH,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzlFreq[0],		1,		0,		4,		1,		1},
		// ����� ��������� ������ ��� ������������ ������� ��� (����)
		{nameShiftFront,		GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// ����� ������� ������ ��� ������������ ������� ��� (����)
		{nameShiftBack,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// ����� ���
		{nameShiftPrm,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// ����� �� ��� �� ����
		{nameShiftPrd,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ��������� �����������
		// ����� ��������� (�������� ������������ ����������� �����)
		{namePrdInDelay,		GB_COM_PRD_GET_TIME_ON,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		0,		20,		1,		1},
		// ������������ ������� ��
		{namePrdDuration,		GB_COM_PRD_GET_DURATION,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		30,		100,	1,		1},
		// ������������ ������� ������
		{namePrdDuration,		GB_COM_PRD_GET_DURATION,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		30,		500,	10,		10},
		// �������� �������
		{nameTestCom,			GB_COM_PRD_GET_TEST_COM,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// �������� �������
		// TODO ������� �� ���������� ������� ������ �� ��������!!!
		{namePrdComLong,		GB_COM_PRD_GET_LONG_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// ������������� �������
		// TODO ������� �� ���������� ������� ������ �� ��������!!!
		{namePrdComBlock,		GB_COM_PRD_GET_BLOCK_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// ���������� ��
		{namePrdDrEnable,		GB_COM_PRD_GET_DR_STATE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ������������� ������� ��
		// TODO ������� �� ���������� ������� ������ �� ��������!!!
		{namePrdDrComBlock,		GB_COM_PRD_GET_DR_BLOCK,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// ���������� ������
		{namePrdComNumbers, 	GB_COM_PRD_GET_COM,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		4,		4},
		// ���������� ������ ������ �
		// TODO �������� ������� �� ���������� ������� ������ �� ��������!!!
		{namePrdComNumbersA, 	GB_COM_PRD_GET_COM_A,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ��������� ���������
		// �������� �� �������� ������� (����� ���������)
		{namePrmTimeOn,			GB_COM_PRM_GET_TIME_ON,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		0,		5,		1,		1},
		// ������������� �������
		// TODO ������� �� ���������� ������� ������ �� ������!!!
		{namePrmComBlock,		GB_COM_PRM_GET_BLOCK_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// �������� �� ����������
		// TODO ������� �� ���������� ������� ������ �� ������!!!
		{namePrmTimeOff,		GB_COM_PRM_GET_TIME_OFF,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			32,		0,		1000,	50,		10},
		// ���������� ��
		{namePrmDrEnable,		GB_COM_PRM_GET_DR_STATE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ������������� ������� ��
		// TODO ������� �� ���������� ������� ������ �� ������!!!
		{namePrmDrComBlock,		GB_COM_PRM_GET_DR_BLOCK,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// ������� �� � ��
		{namePrmDrComToHF,		GB_COM_PRM_GET_DR_COM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			32,		1,		32,		1,		1},
		// ���������� ������
		{namePrmComNumbers,		GB_COM_PRM_GET_COM,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		4,		4}
};

#endif /* FLASHPARAMS_H_ */
