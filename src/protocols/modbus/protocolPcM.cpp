/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include <stdio.h>
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
 *	C �������������� ������� ����� ������� 0xFFFF.
 *
 *	@param adr ����� ��������.
 *	@param val [out] �������� ��������.
 *	@retval CHECK_ERR_NO ������ ��� ���������� �������� �� ��������.
 *	@retval CHECK_ERR_ADR ������������ ����� ��������.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readRegister(uint16_t adr, uint16_t &val) {
	//  �������� �������
	if ((adr <= ADR_REG_MIN) || (adr >= ADR_REG_MAX))
		return CHECK_ERR_ADR;

	// �� ��������� ����� ���������� 0xFFFF
	val = 0xFFFF;
	if (adr >= ADR_IC_BSP_MCU) {
		val = readRegVersionIC(adr);
	} else if (adr >= ADR_IND_COM_PRM_16) {

	} else if (adr >= ADR_MEAS_U_OUT) {
		val = readRegMeasure(adr);
	} else if (adr >= ADR_JRN_DEF_CNT_PWR) {

	} else if (adr >= ADR_JRN_PRD_CNT_PWR) {

	} else if (adr >= ADR_JRN_PRM_CNT_PWR) {

	} else if (adr >= ADR_JRN_EVT_CNT_PWR) {

	} else if (adr >= ADR_GLB_FAULT) {
		val = readRegState(adr);
	} else if (adr >= ADR_PASSWORD) {
		// ������ �� �����������
	} else if (adr >= ADR_YEAR ) {
		val = readRegDateTime(adr);
	}

	return CHECK_ERR_NO;
}

/**	������ ID.
 *
 * 	� ������� ��� ������ ���������� ����������� ������. ��������� ������
 * 	���������� ��������� ���������: ON - 0xFF, OFF- 0x00.
 *
 * 	����� �������� ��������� ������� � ��������� ������ ON ���� ������� ��
 * 	��������� � ������ "�������".
 *
 * 	������ �������� �������: "����� �400�-100-�".
 *
 *	@param buf ��������� �� �����, ��� ������ ������.
 *	@param size [out] ���������� ������������ ������ ���������, �� �������
 *	���� ��� ���������� ���������. ������������ ���-�� ���� ������
 *	���������� � �����.
 * 	@retval CHECK_ERR_NO ������ ��� ����������  �� ��������.
 *	@retval CHECK_ERR_DEVICE �������� ���������� ������.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readID(char *buf, uint8_t &size) {

	// � �400� ��������� ���
	static const char r400m[] PROGMEM = "����� �400�-100-�";
	// � ���� ��������� ������� �����, ���-�� ������ �� ���, ���-�� ������ ��
	// ���, ��� ����� (��/������/�1).
	static const char rzsk[]  PROGMEM = "����� ����-%d%d%d-�";
	// � �400 ��������� ���-�� ������ �� ���, ���-�� ������ �� ���
	// � �� ���� ������ ��������� ��� ������������� � ��� ����� ?!
	static const char k400[]  PROGMEM = "����� �400-%d%d�";
	// TODO � ������, ���� �������� ��������� ��� �������� !!!
	static const char opto[]  PROGMEM = "����� ����-%d%d%d-�";
	// ����������� ����������
	static const char unknown[]  PROGMEM = "����� -%d%d%d-X";

	 eGB_TYPE_DEVICE device = sParam_->typeDevice;

	if (device == AVANT_R400M) {
		size = snprintf(buf, size - 1, r400m);
	} else if (device == AVANT_RZSK) {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, rzsk, def, prd, prm);
	} else if (device == AVANT_K400) {
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, k400, prd, prm);
	} else if (device == AVANT_OPTO) {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, opto, def, prd, prm);
	} else {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, rzsk, def, prd, prm);
	}

	eGB_REGIME reg = sParam_->glb.status.getRegime();
	buf[size++] = (reg == GB_REGIME_DISABLED) ? 0x00 : 0xFF;

	return CHECK_ERR_NO;
}

/**	���������� �� ��������� ���� � �������.
 *
 *	@param adr ����� ��������.
 * 	@return �������� ��������.
 */
uint16_t TProtocolPcM::readRegDateTime(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_YEAR) {
		val = sParam_->dateTime.getYear();
	} else if (adr == ADR_MONTH) {
		val = sParam_->dateTime.getMonth();
	} else if (adr == ADR_DAY) {
		val = sParam_->dateTime.getDay();
	} else if (adr == ADR_HOUR) {
		val = sParam_->dateTime.getHour();
	} else if (adr == ADR_MINUTE) {
		val = sParam_->dateTime.getMinute();
	} else if (adr == ADR_SECOND) {
		val = sParam_->dateTime.getSecond();
	}

	return val;
}

/**	���������� �� ��������� ���������.
 *
 *	@param adr ����� ��������.
 * 	@return �������� ��������.
 */
uint16_t TProtocolPcM::readRegState(uint16_t adr) {
	uint16_t val = 0xFFFF;

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

	return val;
}


/**	C��������� �� ��������� ���������� ����������.
 *
 * 	@param adr ����� ��������.
 * 	@return �������� ��������.
 */
uint16_t TProtocolPcM::readRegMeasure(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_MEAS_U_OUT) {
		val = ((uint16_t) sParam_->measParam.getVoltageOutInt() * 10);
		val += (uint16_t) sParam_->measParam.getVoltageOutFract();
	} else if (adr == ADR_MEAS_I_OUT) {
		val = sParam_->measParam.getCurrentOut();
	} else if (adr == ADR_MEAS_UC1) {
		val = sParam_->measParam.getVoltageCf();
	} else if (adr == ADR_MEAS_UC2) {
		val = sParam_->measParam.getVoltageCf2();
	} else if (adr == ADR_MEAS_UN1) {
		val = sParam_->measParam.getVoltageNoise();
	} else if (adr == ADR_MEAS_UN2) {
		val = sParam_->measParam.getVoltageNoise2();
	} else if (adr == ADR_MEAS_UD1) {
		val = sParam_->measParam.getVoltageDef();
	} else if (adr == ADR_MEAS_UD2) {
		val = sParam_->measParam.getVoltageDef2();
	} else if (adr == ADR_MEAS_PW) {
		val = sParam_->measParam.getPulseWidth();
	}

	return val;
}

/**	C��������� �� ��������� ������ ��������.
 *
 * 	@param adr ����� ��������.
 * 	@return �������� ��������.
 */
uint16_t TProtocolPcM::readRegVersionIC(uint16_t adr) {
	uint16_t val = 0xFFFF;

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

	return val;
}


