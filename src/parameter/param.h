#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "glbDefine.h"

struct Param {

	/// �������� ����������
	enum NAME {
		TIME_SYNCH,			///<
		NUM_OF_DEVICES_2X,
		NUM_OF_DEVICES_3X,
	};

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
		RANGE_U_COR///< <min/10>..<max/10>
	};

	/// ��� ���������, � ����� ����� / �����������.
	enum PARAM_TYPE {
		PARAM_LIST = 0,	///< ������.
		PARAM_INT,		///< ����� �������� jn -32768..32767.
		PARAM_U,		///< ��������� ���������� ���.�.
		PARAM_BITES		///< ����� ������� ��������.
	};

	PGM_P name;			///< ��� ���������.
	eGB_COM com;		///< ������� ���������� ��� ������������ ���������.

	PARAM_TYPE param;	///< ��� ���������.
	RANGE_TYPE range;	///< ��� ������ ��� ������ ��������� ��������.
	DIMENSION dim;		///< ����������� ���������.
	PGM_P listValues;	///< ������ ���������� ��� ��������� ����������.

	uint8_t num;		///< ���������� �������� ����������.
	int16_t min;		///< ����������� �������� ���������.
	int16_t max;		///< ������������ �������� ���������.
	uint8_t disc;		///< ������������.
	uint8_t fract;		///< ��������� ��� ������ �� ��������� � ���.
};

#endif /* PARAM_H_ */
