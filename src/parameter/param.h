#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "glbDefine.h"

//typedef struct Param Param_t PROGMEM;

struct Param {
	/// ���������� ��������� (������� � fcDimension)
	typedef enum {
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
	} DIMENSION;

	/// ��� ������ ��� ����������� ��������� �������� ���������.
	typedef enum {
		RANGE_LIST = 0,	///< ������.
		RANGE_ON_OFF,	///< ������ ���./����.
		RANGE_INT,		///< <min>..<max><dim>
		RANGE_INT_NO_DIM,///< <min>..<max>
		RANGE_U_COR,	///< 0..�<max/10><dim>
        RANGE_I_COR,	///< 0..�<max><dim>
        RANGE_PWD       ///< <max password value>
	} RANGE_TYPE;

	/// ��� ���������, � ����� ����� / �����������.
	typedef enum {
		PARAM_NO = 0,	///< ������������ ��������.
		PARAM_LIST,		///< ������.
        PARAM_INT,		///< ����� �������� �� -32768..32767.
		PARAM_U_COR,	///< ��������� ���������� ���.�.
		PARAM_I_COR,	///< ��������� ���� �XXX.
        PARAM_BITES,	///< ����� ������� ��������.
        PARAM_PWD       ///< ������.
	} PARAM_TYPE;

	/// ����������� ��������� ���������.
	typedef enum {
        DEPEND_MAX_NO = 0,			///< ��� �����������.
		DEPEND_MAX_ON_COM_PRD,		///< ������� �� ���������� ������ �� ���.
		DEPEND_MAX_ON_NUM_DEVS		///< ������� �� ���������� ������ �� ���.
	} DEPEND_MAX;

	/// ����������� �������� ���������.
	typedef enum {
        DEPEND_SAME_NO = 0,			///< ��� �����������.
		DEPEND_SAME_ON_NUM_DEVS,	///< ������� �� ���������� ��������� � �����.
		DEPEND_SAME_ON_COM_PRD,		///< ������� �� ���������� ������ �� ���.
		DEPEND_SAME_ON_COM_PRM		///< ������� �� ���������� ������ �� ���.
	} DEPEND_SAME;

    /// ����������� ����� ��� ��������� ���������.
	typedef enum {
        CHANGE_REG_NO = 0,  ///< ��� �������.
        CHANGE_REG_DISABLE	///< �������� ����� ������ � ������ "�������".
    } CHANGE_REG;

    /// ����������� ������������ ��� ��������� ���������.
    typedef enum {
        CHANGE_USER_NO = 0,     ///< ��� �������.
        CHANGE_USER_ENGINEER,   ///< ��������� ������������ �� ���� ��������.
        CHANGE_USER_ADMIN       ///< ��������� ������������ �� ���� ������.
    } CHANGE_USER;

	char name[NAME_PARAM_LENGHT];	///< ��� ���������.
	eGB_COM com;		///< ������� ���������� ��� ������������ ���������.

	PARAM_TYPE param;	///< ��� ���������.
	RANGE_TYPE range;	///< ��� ������ ��� ������ ��������� ��������.
	DIMENSION dim;		///< ����������� ���������.
	PGM_P listValues;	///< ������ ���������� ��� ��������� ����������.

	uint8_t num;		///< ���������� �������� ����������.
	int16_t min;		///< ����������� �������� ���������.
	int16_t max;		///< ������������ �������� ��������� ��� ���-�� ��������� � ������
	uint8_t disc;		///< ������������.
	uint8_t fract;		///< ��������� ��� ������ �� ��������� � ���.

	eGB_SEND_TYPE send;		///< ��� ��������� ��� ���������� ������ ��������.
	uint8_t sendDop;		///< ���� �������������� ���������� ��� ����������.
	DEPEND_MAX dependMax;	///< ����������� ��������� �������� ���������.
	DEPEND_SAME dependSame;	///< ����������� ���������� �������� ���������.
    CHANGE_REG changeReg;	///< ����������� ����� ��� ��������� ���������.
    CHANGE_USER changeUser; ///< ����������� ������������ ��� ��������� ���������.
};

extern const Param *fParams[] PROGMEM;

// ���������� ��������� �� ��������� ���������.
extern Param const *getPtrParam(eGB_PARAM pn);

// ���������� ���������� �������� �������� ���������.
extern int16_t getAbsMax(eGB_PARAM pn);
// ���������� ���������� �������� ���������� ���������� ����������.
extern int16_t getAbsMaxNumOfSameParams(eGB_PARAM pn);
// ���������� ���������� � ������ ��� ��������� ���������.
extern Param::CHANGE_REG getChangeReg(eGB_PARAM pn);
// ���������� ������������ ������������ ��� ��������� ���������.
extern Param::CHANGE_USER getChangeUser(eGB_PARAM pn);
// ���������� ������� ������������ ��������� ��� ��������.
extern eGB_COM getCom(eGB_PARAM pn);
// ���������� ����������� ��������� ��� ���������.
extern Param::DEPEND_MAX getDependMax(eGB_PARAM pn);
// ���������� ����������� ���������� ��� ���������.
extern Param::DEPEND_SAME getDependSame(eGB_PARAM pn);
// ���������� ����������� ���������.
extern Param::DIMENSION getDim(eGB_PARAM pn);
// ���������� ������������ ��� �������� ���������.
extern uint8_t getDisc(eGB_PARAM pn);
// ���������� ��������� �������� ��� ���������.
extern uint8_t getFract(eGB_PARAM pn);
// ���������� ��������� �� ������ ������ �������� ���������.
extern PGM_P getListOfValues(eGB_PARAM pn);
// ���������� ������� �������� ���������.
extern int16_t getMin(eGB_PARAM pn);
// ���������� ��������� �� ������ � ��������� ���������.
extern PGM_P getNameOfParam(eGB_PARAM pn);
// ���������� ��� ���������.
extern Param::PARAM_TYPE getParamType(eGB_PARAM pn);
// ���������� ��� ��������� �������� ��� ���������.
extern Param::RANGE_TYPE getRangeType(eGB_PARAM pn);
// ���������� �������� ����� ���. ���������� ��� ���������� ������ ��������.
extern uint8_t getSendDop(eGB_PARAM pn);
// ���������� ��� ��������� ��� ���������� ������ ��������.
extern eGB_SEND_TYPE getSendType(eGB_PARAM pn);

#endif /* PARAM_H_ */
