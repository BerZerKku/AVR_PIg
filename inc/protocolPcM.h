/*
 * protocolBspM.h
 *
 *  Created on: 05.09.2014
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCM_H_
#define PROTOCOLPCM_H_

#include "protocolModbus.h"
#include "glbDefine.h"
#include "paramBsp.h"
#include "debug.h"

class TProtocolPcM : public TProtocolModbus
{
	/// ������ ��������� � ������ ����������
	enum ADR{
		// ���� � �����
		ADR_YEAR 			= 1,	///< ���.
		ADR_MONTH,					///< �����.
		ADR_DAY,					///< ����.
		ADR_HOUR,					///< ����.
		ADR_MINUTE,					///< ������.
		ADR_SECOND,					///< �������.
		// ������
		ADR_PASSWORD 		= 7,	///< ������� ������.
		ADR_NEW_PASSWORD,			///< ����� ������.
		// ������� ���������
		ADR_GLB_FAULT		= 10,	///< ��� ������������� �����.
		ADR_GLB_WARNING,			///< ��� �������������� �����.
		ADR_PRM_FAULT,				///< ��� ������������� ���������.
		ADR_PRM_WARNING,			///< ��� �������������� ���������.
		ADR_PRM_REGIME,				///< ����� ���������.
		ADR_PRM_STATE,				///< ��������� ���������.
		ADR_PRM_DOP,				///< �������������� ���� ���������.
		ADR_PRD_FAULT,				///< ��� ������������� �����������.
		ADR_PRD_WARNING,			///< ��� �������������� �����������.
		ADR_PRD_REGIME,				///< ����� �����������.
		ADR_PRD_STATE,				///< ��������� �����������.
		ADR_PRD_DOP,				///< �������������� ���� �����������.
		ADR_DEF_FAULT,				///< ��� ������������� ������.
		ADR_DEF_WARNING,			///< ��� �������������� ������.
		ADR_DEF_REGIME,				///< ����� ������.
		ADR_DEF_STATE,				///< ��������� ������.
		ADR_DEF_DOP,				///< �������������� ���� ������.
		// ������ �������
		ADR_JRN_EVT_CNT_PWR = 27,	///< ���-�� ��������� ������� � ������� ���������.
		ADR_JRN_EVT_CNT,			///< ���-�� ������� � �������.
		ADR_JRN_EVT_NUM,			///< ����� ������� ������ � �������.
		ADR_JRN_EVT_REZ1,			///< ������.
		ADR_JRN_EVT_REZ2,			///< ������.
		ADR_JRN_EVT_DEV,			///< ��� ����������.
		ADR_JRN_EVT_TYPE,			///< ��� �������.
		ADR_JRN_EVT_REG,			///< ����� ������.
		ADR_JRN_EVT_REZ3,			///< ������.
		ADR_JRN_EVT_REZ4,			///< ������.
		ADR_JRN_EVT_REZ5,			///< ������.
		ADR_JRN_EVT_REZ6,			///< ������.
		ADR_JRN_EVT_MSECOND,		///< ������������.
		ADR_JRN_EVT_SECOND,			///< �������.
		ADR_JRN_EVT_MINUTE,			///< ������.
		ADR_JRN_EVT_HOUR,			///< ���.
		ADR_JRN_EVT_WDAY,			///< ���� ������.
		ADR_JRN_EVT_DAY,			///< ����.
		ADR_JRN_EVT_MONTH,			///< �����.
		ADR_JRN_EVT_YEAR,			///< ���.
		// ������ ���������
		ADR_JRN_PRM_CNT_PWR = 50,	///< ���-�� ��������� ������� � ������� ���������.
		ADR_JRN_PRM_CNT,			///< ���-�� ������� � �������.
		ADR_JRN_PRM_NUM,			///< ����� ������� ������ � �������.
		ADR_JRN_PRM_REZ1,			///< ������.
		ADR_JRN_PRM_REZ2,			///< ������.
		ADR_JRN_PRM_DEV,			///< ��� ����������.
		ADR_JRN_PRM_TYPE,			///< ����� �������.
		ADR_JRN_PRM_REG,			///< �������.
		ADR_JRN_PRM_REZ3,			///< ������.
		ADR_JRN_PRM_REZ4,			///< ������.
		ADR_JRN_PRM_REZ5,			///< ������.
		ADR_JRN_PRM_REZ6,			///< ������.
		ADR_JRN_PRM_MSECOND,		///< ������������.
		ADR_JRN_PRM_SECOND,			///< �������.
		ADR_JRN_PRM_MINUTE,			///< ������.
		ADR_JRN_PRM_HOUR,			///< ���.
		ADR_JRN_PRM_WDAY,			///< ���� ������.
		ADR_JRN_PRM_DAY,			///< ����.
		ADR_JRN_PRM_MONTH,			///< �����.
		ADR_JRN_PRM_YEAR,			///< ���.
		// ������ �����������
		ADR_JRN_PRD_CNT_PWR = 74,	///< ���-�� ��������� ������� � ������� ���������.
		ADR_JRN_PRD_CNT,			///< ���-�� ������� � �������.
		ADR_JRN_PRD_NUM,			///< ����� ������� ������ � �������.
		ADR_JRN_PRD_REZ1,			///< ������.
		ADR_JRN_PRD_REZ2,			///< ������.
		ADR_JRN_PRD_DEV,			///< ��� ����������.
		ADR_JRN_PRD_TYPE,			///< ����� �������.
		ADR_JRN_PRD_REG,			///< �������.
		ADR_JRN_PRD_REZ3,			///< ������.
		ADR_JRN_PRD_REZ4,			///< ������.
		ADR_JRN_PRD_REZ5,			///< ������.
		ADR_JRN_PRD_REZ6,			///< ������.
		ADR_JRN_PRD_MSECOND,		///< ������������.
		ADR_JRN_PRD_SECOND,			///< �������.
		ADR_JRN_PRD_MINUTE,			///< ������.
		ADR_JRN_PRD_HOUR,			///< ���.
		ADR_JRN_PRD_WDAY,			///< ���� ������.
		ADR_JRN_PRD_DAY,			///< ����.
		ADR_JRN_PRD_MONTH,			///< �����.
		ADR_JRN_PRD_YEAR,			///< ���.
		// ������ ������
		ADR_JRN_DEF_CNT_PWR = 98,	///< ���-�� ��������� ������� � ������� ���������.
		ADR_JRN_DEF_CNT,			///< ���-�� ������� � �������.
		ADR_JRN_DEF_NUM,			///< ����� ������� ������ � �������.
		ADR_JRN_DEF_REZ1,			///< ������.
		ADR_JRN_DEF_REZ2,			///< ������.
		ADR_JRN_DEF_DEV,			///< ��� ����������.
		ADR_JRN_DEF_TYPE,			///< ��������� ��������.
		ADR_JRN_DEF_REG,			///< �������.
		ADR_JRN_DEF_REZ3,			///< ������.
		ADR_JRN_DEF_REZ4,			///< ������.
		ADR_JRN_DEF_REZ5,			///< ������.
		ADR_JRN_DEF_REZ6,			///< ������.
		ADR_JRN_DEF_MSECOND,		///< ������������.
		ADR_JRN_DEF_SECOND,			///< �������.
		ADR_JRN_DEF_MINUTE,			///< ������.
		ADR_JRN_DEF_HOUR,			///< ���.
		ADR_JRN_DEF_WDAY,			///< ���� ������.
		ADR_JRN_DEF_DAY,			///< ����.
		ADR_JRN_DEF_MONTH,			///< �����.
		ADR_JRN_DEF_YEAR,			///< ���.
		// ���������
		ADR_MEAS_U_OUT 		= 123,	///< �������� ����������.
		ADR_MEAS_I_OUT,				///< �������� ���.
		ADR_MEAS_UC,				///< ����� �� �� (��1).
		ADR_MEAS_UD,				///< ����� �� U�.
		// ��������� ������
		ADR_IND_COM_PRM_16	= 140,	///< ��������� ������ ��������� 1-16.
		ADR_IND_COM_PRM_32,			///< ��������� ������ ��������� 17-32.
		ADR_IND_COM_PRD_16,			///< ��������� ������ ����������� 1-16.
		ADR_IND_COM_PRD_32,			///< ��������� ������ ����������� 16-32.
		// ������ ��������
		ADR_IC_BSP_MCU 		= 156,
		ADR_IC_BSP_DSP,
		ADR_IC_PI_MCU,
		ADR_IC_BSK_PRD1,
		ADR_IC_BSK_PRM1,
		ADR_IC_BSK_PRD2,
		ADR_IC_BSK_PRM2
	};

public:
	/**	�����������.
	 *
	 * 	@param sParam ��������� �� ��������� ����������.
	 * 	@param buf ��������� �� ����� ������.
	 * 	@param size ������ ������ ������.
	 */
	TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	����������.
	 *
	 */
//	~TProtocolPcM() {};

private:
	stGBparam * const sParam_;	///< ��������� ����������

	/// ��������� �������� ������� ������ ������
	bool readCoilCom();

	/// ��������� �������� ������� ������ ���������
	bool readRegisterCom();

	/// ��������� �������� ������� ������ ID
	bool readIdCom();
};


#endif /* PROTOCOLPM_H_ */
