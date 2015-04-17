#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "glbDefine.h"

//typedef struct Param Param_t PROGMEM;

struct Param {
	/// ���������� ��������� (������� � fcDimension)
	enum DIMENSION {
		DIM_NO = 0,		///< ���
		DIM_MA,			///< ��
		DIM_A,			///< �
		DIM_KA,			///< ��
		DIM_V,			///< �
		DIM_KV,			///< ��
		DIM_MSEC,		///< ��
		DIM_SEC,		///< �
		DIM_HOUR,		///< ���
		DIM_DB,			///< ��
		DIM_HZ,			///< ��
		DIM_KHZ,		///< ���
		DIM_DEG			///< ����
	};

	/// ��� ������ ��� ����������� ��������� �������� ���������.
	enum RANGE_TYPE {
		RANGE_LIST = 0,	///< ������.
		RANGE_ON_OFF,	///< ������ ���./����.
		RANGE_INT,		///< <min>..<max><dim>
		RANGE_INT_NO_DIM,///< <min>..<max>
		RANGE_FLOAT10,	///< <min/10>..<max/10>
		RANGE_FLOAT100	///< <min/100>..<max/100>
	};

	/// ��� ���������, � ����� ����� / �����������.
	enum PARAM_TYPE {
		PARAM_NO = 0,	///< ������������ ��������.
		PARAM_LIST,		///< ������.
		PARAM_INT,		///< ����� �������� -32768..32767.
		PARAM_BITES,	///< ����� ������� �������� ��� ���������� ����������.
		PARAM_BITE,		///< ���� ��� � �������� ����� ������.
		PARAM_FLOAT10,	///< ������� �������� <val/10>.<val%10>.
		PARAM_FLOAT100	///< ������� �������� <val/100>.<val%100>.
	};

	/// ����������� ��������� ���������.
	enum DEPEND_MAX {
		DEPEND_MAX_NO,				///< ��� �����������.
		DEPEND_MAX_ON_COM_PRD,		///< ������� �� ���������� ������ �� ���.
		DEPEND_MAX_ON_NUM_DEVS		///< ������� �� ���������� ������ �� ���.
	};

	/// ����������� �������� ���������.
	enum DEPEND_SAME {
		DEPEND_SAME_NO,				///< ��� �����������.
		DEPEND_SAME_ON_NUM_DEVS,	///< ������� �� ���������� ��������� � �����.
		DEPEND_SAME_ON_COM_PRD,		///< ������� �� ���������� ������ �� ���.
		DEPEND_SAME_ON_COM_PRM		///< ������� �� ���������� ������ �� ���.
	};

	/// ������� ��� ��������� ���������.
	enum CHANGE_COND {
		CHANGE_COND_NO,				///< ��� �������.
		CHANGE_COND_REG_DISABLE		///< �������� ����� ������ � ������ "�������".
	};

	char name[NAME_PARAM_LENGHT];	///< ��� ���������.
	eGB_COM com;		///< ������� ���������� ��� ������������ ���������.

	PARAM_TYPE param;	///< ��� ���������.
	RANGE_TYPE range;	///< ��� ������ ��� ������ ��������� ��������.
	DIMENSION dim;		///< ����������� ���������.
	PGM_P listValues;	///< ������ ���������� ��� ��������� ����������.

	uint8_t num;		///< ���������� �������� ����������.
	int16_t min;		///< ����������� �������� ���������.
	int16_t max;		///< ������������ �������� ��������� ��� ���-�� ��������� � ������.
	uint8_t disc;		///< ������������.
	uint8_t fract;		///< ��������� ��� ������ �� ��������� � ���.

	eGB_SEND_WR_TYPE sendWr;	///< ��� ������� ��� ������ ���������.
	eGB_SEND_RD_TYPE sendRd;	///< ��� ������� ��� ������ ���������.
	uint8_t sendDop;		///< ���� �������������� ���������� ��� ����������.
	uint8_t posBite;		///< ������� ������� ���� ��� ������� �� ���������� �������.
	DEPEND_MAX dependMax;	///< ����������� ��������� �������� ���������.
	DEPEND_SAME dependSame;	///< ����������� ���������� �������� ���������.
	CHANGE_COND changeCond;	///< ������� ��� ��������� ���������.
};

#endif /* PARAM_H_ */
