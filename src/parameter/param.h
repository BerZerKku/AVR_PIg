#ifndef PARAM_H_
#define PARAM_H_

#include <avr/pgmspace.h>
#include <stdint.h>

#include "glbDefine.h"

struct Param
{

    /// ���������� ��������� (������� � fcDimension)
    typedef enum
    {
        DIM_NO = 0,  ///< ���
        DIM_MA,      ///< ��
        DIM_A,       ///< �
        DIM_KA,      ///< ��
        DIM_V,       ///< �
        DIM_KV,      ///< ��
        DIM_MSEC,    ///< ��
        DIM_SEC,     ///< �
        DIM_HOUR,    ///< ���
        DIM_DB,      ///< ��
        DIM_HZ,      ///< ��
        DIM_KHZ,     ///< ���
        DIM_DEG      ///< ����
    } DIMENSION;

    /// ��� ������ ��� ����������� ��������� �������� ���������.
    typedef enum
    {
        RANGE_LIST = 0,    ///< ������.
        RANGE_ON_OFF,      ///< ������ ���./����.
        RANGE_INT,         ///< <min>..<max><dim>
        RANGE_INT_NO_DIM,  ///< <min>..<max>
        RANGE_U_COR,       ///< 0..�<max/10><dim>
        RANGE_INT_PM       ///< 0..�<max><dim>
    } RANGE_TYPE;

    /// ��� ���������, � ����� ����� / �����������.
    typedef enum
    {
        PARAM_NO = 0,  ///< ������������ ��������.
        PARAM_LIST,    ///< ������.
        PARAM_INT,     ///< ����� �������� jn -32768..32767.
        PARAM_U_COR,   ///< ��������� ���������� ���.�.
        PARAM_I_COR,   ///< ��������� ���� �XXX.
        PARAM_BITES    ///< ����� ������� ��������.
    } PARAM_TYPE;

    /// ����������� ��������� ���������.
    typedef enum
    {
        DEPEND_MAX_NO,          ///< ��� �����������.
        DEPEND_MAX_ON_COM_PRD,  ///< ������� �� ���������� ������ �� ���.
        DEPEND_MAX_ON_NUM_DEVS  ///< ������� �� ���������� ������ �� ���.
    } DEPEND_MAX;

    /// ����������� �������� ���������.
    typedef enum
    {
        DEPEND_SAME_NO,           ///< ��� �����������.
        DEPEND_SAME_ON_NUM_DEVS,  ///< ������� �� ���������� ��������� � �����.
        DEPEND_SAME_ON_COM_PRD,   ///< ������� �� ���������� ������ �� ���.
        DEPEND_SAME_ON_COM_PRM    ///< ������� �� ���������� ������ �� ���.
    } DEPEND_SAME;

    /// ������� ��� ��������� ���������.
    typedef enum
    {
        CHANGE_COND_NO,          ///< ��� �������.
        CHANGE_COND_REG_DISABLE  ///< �������� ����� ������ � ������ "�������".
    } CHANGE_COND;

    char    name[NAME_PARAM_LENGHT];  ///< ��� ���������.
    eGB_COM com;                      ///< ������� ���������� ��� ������������ ���������.

    PARAM_TYPE param;       ///< ��� ���������.
    RANGE_TYPE range;       ///< ��� ������ ��� ������ ��������� ��������.
    DIMENSION  dim;         ///< ����������� ���������.
    PGM_P      listValues;  ///< ������ ���������� ��� ��������� ����������.

    uint8_t num;    ///< ���������� �������� ����������.
    int16_t min;    ///< ����������� �������� ���������.
    int16_t max;    ///< ������������ �������� ��������� ��� ���-�� ��������� � ������
    uint8_t disc;   ///< ������������.
    uint8_t fract;  ///< ��������� ��� ������ �� ��������� � ���.

    eGB_SEND_TYPE send;        ///< ��� ��������� ��� ���������� ������ ��������.
    uint8_t       sendDop;     ///< ���� �������������� ���������� ��� ����������.
    DEPEND_MAX    dependMax;   ///< ����������� ��������� �������� ���������.
    DEPEND_SAME   dependSame;  ///< ����������� ���������� �������� ���������.
    CHANGE_COND   changeCond;  ///< ������� ��� ��������� ���������.
};

#endif /* PARAM_H_ */
