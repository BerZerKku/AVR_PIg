/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcM.h"

// �����������
TProtocolPcM::TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), TProtocolModbus(buf, size) {

}

/**	������ ���������.
 *
 *	���������� �������� ������������ ������. ���� �� ��������� � ����������
 *	���������, ������������ ������� �������� ��������.
 *
 *	������������ ������:
 *	- � 1 �� 152 ������������.
 *	� �������������� ������� ����� ������� 0xFFFF
 *
 *	@param adr ����� ��������.
 *	@param val [out] �������� ��������.
 *	@retval CHECK_ERR_NO ������ ��� ���������� �������� �� ��������.
 *	@retval CHECK_ERR_ADR ������������ ����� ��������.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readRegister(uint16_t adr, uint16_t &val) {
	//  �������� �������
	if ((adr == 0) || (adr >= ADR_REG_MAX))
		return CHECK_ERR_ADR;

	val = 0xFFFF;
	// �� ��������� ����� �������� 0xFFF
	if (adr < ADR_PASSWORD) {					// ���� � �����
		if (adr == ADR_YEAR) {
			val = sParam_->dataTime.getYear();
		} else if (adr == ADR_MONTH) {
			val = sParam_->dataTime.getMonth();
		} else if (adr == ADR_DAY) {
			val = sParam_->dataTime.getDay();
		} else if (adr == ADR_HOUR) {
			val = sParam_->dataTime.getHour();
		} else if (adr == ADR_MINUTE) {
			val = sParam_->dataTime.getMinute();
		} else if (adr == ADR_SECOND) {
			val = sParam_->dataTime.getSecond();
		}
	} else if (adr < ADR_GLB_FAULT) {			// ������
		// � ������� ������ �� ����������
	} else if (adr < ADR_JRN_EVT_CNT_PWR) {		// ������� ���������
		if (adr == ADR_GLB_FAULT) {
			val = sParam_->glb.status.getFaults();
		} else if (adr == ADR_GLB_WARNING) {
			val = sParam_->glb.status.getWarnings();
		} else if (adr == ADR_PRM_FAULT) {
			val = sParam_->prm.status.getFaults();
		} else if (adr == ADR_PRM_WARNING) {
			val = sParam_->prm.status.getWarnings();
		} else if (adr == ADR_PRM_REGIME) {
			val = sParam_->prm.status.getRegime();
		} else if (adr == ADR_PRM_STATE) {
			val = sParam_->prm.status.getState();
		} else if (adr == ADR_PRM_DOP) {
			val = sParam_->prm.status.getDopByte();
		} else if (adr == ADR_PRD_FAULT) {
			val = sParam_->prd.status.getFaults();
		} else if (adr == ADR_PRD_WARNING) {
			val = sParam_->prd.status.getWarnings();
		} else if (adr == ADR_PRD_REGIME) {
			val = sParam_->prd.status.getRegime();
		} else if (adr == ADR_PRD_STATE) {
			val = sParam_->prd.status.getState();
		} else if (adr == ADR_PRD_DOP) {
			val = sParam_->prd.status.getDopByte();
		} else if (adr == ADR_DEF_FAULT) {
			val = sParam_->def.status.getFaults();
		} else if (adr == ADR_DEF_WARNING) {
			val = sParam_->def.status.getWarnings();
		} else if (adr == ADR_DEF_REGIME) {
			val = sParam_->def.status.getRegime();
		} else if (adr == ADR_DEF_STATE) {
			val = sParam_->def.status.getState();
		} else if (adr == ADR_DEF_DOP) {
			val = sParam_->def.status.getDopByte();
		}
	} else if (adr < ADR_JRN_PRM_CNT_PWR) {		// ������ �������

	} else if (adr < ADR_JRN_PRD_CNT_PWR) {		// ������ ���������

	} else if (adr < ADR_JRN_DEF_CNT_PWR) {		// ������ �����������

	} else if (adr < ADR_MEAS_U_OUT) {			// ������ ������

	} else if (adr < ADR_IND_COM_PRM_16) {		// ��������
		if (adr == ADR_MEAS_U_OUT) {

		} else if (adr == ADR_MEAS_I_OUT) {

		} else if (adr == ADR_MEAS_UC) {

		} else if (adr == ADR_MEAS_UD) {

		}
	} else if (adr < ADR_IC_BSP_MCU) {			// ��������� ������

	} else {									// ������ ��������
		if (adr == ADR_IC_BSP_MCU) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSP_MCU);
		} else if (adr == ADR_IC_BSP_DSP) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSP_DSP);
		} else if (adr == ADR_IC_PI_MCU) {
			val = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
		} else if (adr == ADR_IC_BSK_PRD1) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRD1);
		} else if (adr == ADR_IC_BSK_PRM1) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRM1);
		} else if (adr == ADR_IC_BSK_PRD2) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRD2);
		} else if (adr == ADR_IC_BSK_PRM2) {
			val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRM2);
		}
	}

	return CHECK_ERR_NO;
}

