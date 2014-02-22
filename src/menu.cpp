/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "../inc/menu.h"
#include "../inc/debug.h"
#include "../inc/ks0108.h"
#include "../inc/flash.h"

/// ����� ��������� �� ���������
#define LED_REGIME LED_SWITCH

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// ���-�� ����� ������ ������������ �� ������
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)

clMenu::clMenu() {
	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;
	lvlCreate_ = true;
	blink_ = false;
	blinkMeasParam_ = false;
	curCom_ = 0;
	delay_ = TIME_MESSAGE;

	// ������ ���������
	cursorEnable_ = false;
	cursorLine_ = 0;
	numPunkts_ = 0;

	// ������� ������ ��� ���
	key_ = KEY_NO;

	// ��� ���������� ��� �� ��������
	setTypeDevice(AVANT_NO);

	// ����� � ��� ��� ���
	connectionBsp_ = false;

	// �������� ������� ��������� ������ ��� ���� ���������
	// ������ ������ ���� �������� ��������� � ��������� ������ ������
	// ���� fcUnknownSost
	sParam.def.status.stateText[0] = fcDefSost00;
	sParam.def.status.stateText[1] = fcDefSost01;
	sParam.def.status.stateText[2] = fcDefSost02;
	sParam.def.status.stateText[3] = fcDefSost03;
	sParam.def.status.stateText[4] = fcDefSost04;
	sParam.def.status.stateText[5] = fcDefSost05;
	sParam.def.status.stateText[6] = fcDefSost06;
	sParam.def.status.stateText[7] = fcDefSost07;
	sParam.def.status.stateText[8] = fcDefSost08;
	sParam.def.status.stateText[9] = fcDefSost09;
	sParam.def.status.stateText[10] = fcDefSost10;
	sParam.def.status.stateText[11] = fcDefSost11;
	sParam.def.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	sParam.prm.status.stateText[0] = fcPrmSost00;
	sParam.prm.status.stateText[1] = fcPrmSost01;
	sParam.prm.status.stateText[2] = fcPrmSost02;
	sParam.prm.status.stateText[3] = fcPrmSost03;
	sParam.prm.status.stateText[4] = fcPrmSost04;
	sParam.prm.status.stateText[5] = fcPrmSost05;
	sParam.prm.status.stateText[6] = fcPrmSost06;
	sParam.prm.status.stateText[7] = fcPrmSost07;
	sParam.prm.status.stateText[8] = fcPrmSost08;
	sParam.prm.status.stateText[9] = fcPrmSost09;
	sParam.prm.status.stateText[10] = fcPrmSost10;
	sParam.prm.status.stateText[11] = fcPrmSost11;
	sParam.prm.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	sParam.prd.status.stateText[0] = fcPrdSost00;
	sParam.prd.status.stateText[1] = fcPrdSost01;
	sParam.prd.status.stateText[2] = fcPrdSost02;
	sParam.prd.status.stateText[3] = fcPrdSost03;
	sParam.prd.status.stateText[4] = fcPrdSost04;
	sParam.prd.status.stateText[5] = fcPrdSost05;
	sParam.prd.status.stateText[6] = fcPrdSost06;
	sParam.prd.status.stateText[7] = fcPrdSost07;
	sParam.prd.status.stateText[8] = fcPrdSost08;
	sParam.prd.status.stateText[9] = fcPrdSost09;
	sParam.prd.status.stateText[10] = fcPrdSost10;
	sParam.prd.status.stateText[11] = fcPrdSost11;
	sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	// �������� ����� �����������
	sParam.def.status.name = fcDeviceName00;
	sParam.prm.status.name = fcDeviceName01;
	sParam.prd.status.name = fcDeviceName02;

#ifdef DEBUG
	// � ������ ������� �������� ���������� ���������
	vLCDsetLed(LED_ON);
#else
	// ��������� ������ ������ ���������
	vLCDsetLed(LED_REGIME);
#endif
}

/**	������ � ������� ������� ����
 * 	@param ���
 * 	@return ���
 */
void clMenu::main(void) {
	// ������� ������� �� ����������������� ���
	static uint8_t cntInitLcd = 0;
	static uint8_t cntBlinkMeas = 0;
	static uint8_t cntBlinkText = 0;

	if (++cntBlinkMeas >= TIME_MEAS_PARAM) {
		blinkMeasParam_ = !blinkMeasParam_;
		cntBlinkMeas = 0;
	}

	if (++cntBlinkText >= TIME_TEXT) {
		blink_ = !blink_;
		cntBlinkText = 0;
	}

	if (++cntInitLcd >= TIME_TO_INIT_LCD) {
		vLCDinit();
		cntInitLcd = 0;
	}

	if (!sParam.device)
		setTypeDevice();

	// ������� ��� � ����������
	// ���� ������ ����� ������ - ��������� ��������������� ���������
	eKEY tmp = eKEYget(sParam.typeDevice);
	if (tmp != KEY_NO) {
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLed(LED_SWITCH);
	}

	// ������� ������ ���������
	if (delay_ < TIME_MESSAGE)
		delay_++;

	// ����� � ����� ����������� �������� ����
	// ���� ��������� ��� ��� �������� �� ���������
	clearTextBuf();
	(this->*lvlMenu)();
	key_ = KEY_NO;

#ifdef VIEW_DEBUG_PARAM
	// ����� ���������� ����������
	if (this->lvlMenu == &clMenu::lvlStart)
	{
		snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
		snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
		snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
		snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
		snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
		snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
		snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
	}
	else
	{
		snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
	}
#endif

	// ����� ��������� � ������ ���������� ����� � ���
	if (!isConnectionBsp() && (blink_)) {
		static const char fcNoConnectBsp[] PROGMEM = " ��� ����� � ���!!! ";
		snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
	}

	// �������������� ������ �������� � ������ ��� ������ �� �����
	vLCDputchar(vLCDbuf, lineParam_);
	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
}

/** ��������� ���� �������� � ��������� ���� � ��� ������.
 * 	@param device ��� ����������
 * 	@return False � ������ ������, ����� True
 */
bool clMenu::setTypeDevice(eGB_TYPE_DEVICE device) {
	bool status = false;

	sParam.glb.status.setEnable(true);

	// ���� ����������� ��� �������� ����� �������, ������� ����� ������
	// �� ������� ��������.
	if (device == AVANT_NO) {

		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM) {
			// �� �������
			// ���� ���� ������ + ������� (��� �/��� ���) - ����
			// ���� ���� ������ ������� (��� �/��� ���) - �400
			// ���� ���� ������ � (������ �������� & 0xF000) = 0xF000 - �400�

			if (sParam.def.status.isEnable()) {
				if ((sParam.prm.status.isEnable())
						|| (sParam.prd.status.isEnable())) {
					device = AVANT_RZSK;
				} else {
					uint16_t vers = sParam.glb.getVersBsp() & 0xF000;
					if ((vers & 0xF000) == 0xF000) {
						device = AVANT_R400M;
					}
				}
			} else {
				if ((sParam.prd.status.isEnable())
						|| (sParam.prm.status.isEnable())) {
					device = AVANT_K400;
				}
			}
		}

		// ���� ������� ���������� ��������� � �����, �� ������ �� ������
		// ����� ������� �� ��������� �������
		if (device == sParam.typeDevice)
			status = true;
		else {
			sParam.typeDevice = device;
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
		}
	}

	if (!status) {
		// ��������������� "�������" �������� ��������������
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.glb.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.def.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.prm.status.faultText[i] = fcUnknownFault;
		for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
			sParam.prd.status.faultText[i] = fcUnknownFault;

		// ��������������� "�������" �������� ��������������
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.glb.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.def.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.prm.status.warningText[i] = fcUnknownWarning;
		for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
			sParam.prd.status.warningText[i] = fcUnknownWarning;

		// ��������������� ������� ������� ������������ ����������
		for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
			measParam[i] = MENU_MEAS_PARAM_NO;

		if (device == AVANT_K400) {
			sParam.typeDevice = device;

			// TODO ��� � 3-� �������� U�1/2, U�1/2
			// ������ ������� ����������
			measParam[0] = MENU_MEAS_PARAM_TIME; // ���� <-> �����
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;
			// ������ ������� ����������
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

			// ���������� ������� ����� ��������������
			sParam.glb.status.faultText[0] = fcGlbFault0001;
			sParam.glb.status.faultText[1] = fcGlbFault0002;
			sParam.glb.status.faultText[2] = fcGlbFault0004;
			sParam.glb.status.faultText[3] = fcGlbFault0008;
			sParam.glb.status.faultText[4] = fcGlbFault0010;
			// 5-7 ���
			sParam.glb.status.faultText[8] = fcGlbFault0100;
			sParam.glb.status.faultText[9] = fcGlbFault0200;
			// 10 ���
			sParam.glb.status.faultText[11] = fcGlbFault0800;
			sParam.glb.status.faultText[12] = fcGlbFault1000;
			// 13-15 ���
			// ���������� ������� ����� ��������������
			sParam.glb.status.warningText[0] = fcGlbWarning01;
			// 1-15 ���

			// ���������� ������
			sParam.def.status.setEnable(false);

			// ��������� ���������
			sParam.prm.status.setEnable(true);
			// ���������� ������� �������������� ���������
			sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
			sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
			sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
			// 3-7 ���
			sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
			sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
			sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
			sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� ���������
			sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
			// 1-15 ���

			// ��������� �����������
			// ���������� ������� �������������� �����������
			sParam.prd.status.setEnable(true);
			sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
			sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
			// 2-7 ���
			sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
			sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
			sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
			sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� �����������
			// 0-15 ���

			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF1);
			sParam.test.addSignalToList(GB_SIGNAL_CF2);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}

			status = true;
		} else if (device == AVANT_RZSK) {
			sParam.typeDevice = device;

			// TODO ��� � 3-� �������� U�1/2, U�1/2, U�1/2
			// ������ ������� ����������
			measParam[0] = MENU_MEAS_PARAM_TIME;	// ���� <-> �����
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;

			// ������ ������� ����������
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UZ;
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

			// ���������� ������� ����� ��������������
			sParam.glb.status.faultText[0] = fcGlbFault0001;
			sParam.glb.status.faultText[1] = fcGlbFault0002;
			sParam.glb.status.faultText[2] = fcGlbFault0004;
			sParam.glb.status.faultText[3] = fcGlbFault0008;
			sParam.glb.status.faultText[4] = fcGlbFault0010;
			// 5-7 ���
			sParam.glb.status.faultText[8] = fcGlbFault0100;
			sParam.glb.status.faultText[9] = fcGlbFault0200;
			// 10 ���
			sParam.glb.status.faultText[11] = fcGlbFault0800;
			sParam.glb.status.faultText[12] = fcGlbFault1000;
			// 13-15 ���
			// ���������� ������� ����� ��������������
			// 1-9 ���
			sParam.glb.status.warningText[10] = fcGlbWarning01;
			// 11-15 ���

			// ��������� ������
			sParam.def.status.setEnable(true);
			// ���������� ������� �������������� ������
			sParam.def.status.faultText[0] = fcDefFault0001;
			sParam.def.status.faultText[1] = fcDefFault0002;
			sParam.def.status.faultText[2] = fcDefFault0004;
			// 3-7 ���
			sParam.def.status.faultText[8] = fcDefFault0100;
			sParam.def.status.faultText[9] = fcDefFault0200;
			// 10 ���
			sParam.def.status.faultText[11] = fcDefFault0800;
			// 12 ���
			sParam.def.status.faultText[13] = fcDefFault2000;
			sParam.def.status.faultText[14] = fcDefFault4000rzsk;
			// 15 ���
			// ���������� ������� �������������� ������
			sParam.def.status.warningText[0] = fcDefWarning01rzsk;
			sParam.def.status.warningText[1] = fcDefWarning02;
			// 2-15 ���

			// ��������� ���������
			sParam.prm.status.setEnable(true);
			// ���������� ������� �������������� ������
			sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
			sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
			sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
			// 3-7 ���
			sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
			sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
			sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
			sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� ������
			sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
			// 1-15 ���

			// ��������� �����������
			sParam.prd.status.setEnable(true);
			// ���������� ������� �������������� �����������
			sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
			sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
			// 2-7 ���
			sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
			sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
			sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
			sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� �����������
			// 0-15 ���

			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1);
			sParam.test.addSignalToList(GB_SIGNAL_COM2);
			sParam.test.addSignalToList(GB_SIGNAL_COM3);
			sParam.test.addSignalToList(GB_SIGNAL_COM4);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);

			status = true;
		} else if (device == AVANT_R400M) {
			sParam.typeDevice = device;

			// TODO ��� � 3-� �������� Uk1/2
			// ������ ������� ����������
			measParam[0] = MENU_MEAS_PARAM_TIME;	// ���� <-> �����
			measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
			measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UOUT;
			measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_IOUT;

			// ������ ������� ����������
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UZ;
			measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UC;
			measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_UN;

			// ���������� ������� ����� ��������������
			sParam.glb.status.faultText[0] = fcGlbFault0001;
			sParam.glb.status.faultText[1] = fcGlbFault0002;
			sParam.glb.status.faultText[2] = fcGlbFault0004;
			sParam.glb.status.faultText[3] = fcGlbFault0008;
			sParam.glb.status.faultText[4] = fcGlbFault0010;
			sParam.glb.status.faultText[5] = fcGlbFault0020;
			sParam.glb.status.faultText[6] = fcGlbFault0040;
			sParam.glb.status.faultText[7] = fcGlbFault0080;
			sParam.glb.status.faultText[8] = fcGlbFault0100;
			sParam.glb.status.faultText[9] = fcGlbFault0200;
			sParam.glb.status.faultText[10] = fcGlbFault0400;
			sParam.glb.status.faultText[11] = fcGlbFault0800;
			sParam.glb.status.faultText[12] = fcGlbFault1000;
			sParam.glb.status.faultText[13] = fcGlbFault2000;
			sParam.glb.status.faultText[14] = fcGlbFault4000;
			// 15 ���
			// ���������� ������� ����� ��������������
			sParam.glb.status.warningText[0] = fcGlbWarning01;
			// 1-15 - ���

			// ��������� ������
			sParam.def.status.setEnable(true);
			// ���������� ������� �������������� ������
			sParam.def.status.faultText[0] = fcDefFault0001;
			sParam.def.status.faultText[1] = fcDefFault0002;
			sParam.def.status.faultText[2] = fcDefFault0004;
			// 3 ���
			sParam.def.status.faultText[4] = fcDefFault0010;
			// 5 ���
			sParam.def.status.faultText[6] = fcDefFault0040;
			sParam.def.status.faultText[7] = fcDefFault0080;
			sParam.def.status.faultText[8] = fcDefFault0100;
			sParam.def.status.faultText[9] = fcDefFault0200;
			sParam.def.status.faultText[10] = fcDefFault0400;
			sParam.def.status.faultText[11] = fcDefFault0800;
			sParam.def.status.faultText[12] = fcDefFault1000;
			sParam.def.status.faultText[13] = fcDefFault2000;
			sParam.def.status.faultText[14] = fcDefFault4000;
			sParam.def.status.faultText[15] = fcDefFault8000;
			// ���������� ������� �������������� ������
			sParam.def.status.warningText[0] = fcDefWarning01;
			sParam.def.status.warningText[1] = fcDefWarning02;
			sParam.def.status.warningText[2] = fcDefWarning04;
			sParam.def.status.warningText[3] = fcDefWarning08;
			// 4-15 ���

			// ���������� ���������
			sParam.prm.status.setEnable(false);

			// ���������� �����������
			sParam.prd.status.setEnable(false);

			status = true;
		} else if (device == AVANT_OPTIC) {
			sParam.typeDevice = device;

			// TODO ������
			// ��������� ������ ���������� ����������, ���� ���� ?!
			measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_TIME;
			measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] =
					MENU_MEAS_PARAM_DATE;

			// ���������� ������� ����� ��������������
			sParam.glb.status.faultText[0] = fcGlbFault0001;
			sParam.glb.status.faultText[1] = fcGlbFault0002;
			sParam.glb.status.faultText[2] = fcGlbFault0004;
			sParam.glb.status.faultText[3] = fcGlbFault0008;
			sParam.glb.status.faultText[4] = fcGlbFault0010;
			// 5-8 ���
			sParam.glb.status.faultText[9] = fcGlbFault0200;
			// 10-15 ���
			// ���������� ������� ����� ��������������
			sParam.glb.status.warningText[0] = fcGlbWarning01;
			// 1-15 ���

			// ��������� ������
			sParam.def.status.setEnable(true);
			// ���������� ������� �������������� ������
			sParam.def.status.faultText[0] = fcDefFault0001;
			sParam.def.status.faultText[1] = fcDefFault0002;
			sParam.def.status.faultText[2] = fcDefFault0004;
			// 3
			sParam.def.status.faultText[4] = fcDefFault0010rzsko;
			// 5-7 ���
			sParam.def.status.faultText[8] = fcDefFault0100;
			sParam.def.status.faultText[9] = fcDefFault0200;
			// 10 ���
			sParam.def.status.faultText[11] = fcDefFault0800;
			// 12 ���
			sParam.def.status.faultText[13] = fcDefFault2000;
			// 14-15 ���
			// ���������� ������� �������������� ������
			sParam.def.status.warningText[0] = fcDefWarning01rzsko;
			sParam.def.status.warningText[1] = fcDefWarning02;
			// 2-15 ���

			// ��������� ���������
			sParam.prm.status.setEnable(true);
			// ���������� ������� �������������� ������
			sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
			sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
			sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
			// 3-7 ���
			sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
			sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
			sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
			sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� ������
			sParam.prm.status.warningText[0] = fcPrmWarning01rzsko;
			// 1-15 ���

			// ��������� �����������
			sParam.prd.status.setEnable(true);
			// ���������� ������� �������������� �����������
			sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
			sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
			// 2-7 ���
			sParam.prd.status.faultText[8] = fcPrdFault0100rzsk;
			sParam.prd.status.faultText[9] = fcPrdFault0200rzsk;
			sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
			sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
			// 12-15 ���
			// ���������� ������� �������������� �����������
			// 0-15 ���

			// TODO ������ ���� ��������� ������� ������
			// � ������ �������� ������ ���������
			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1);
			sParam.test.addSignalToList(GB_SIGNAL_COM2);
			sParam.test.addSignalToList(GB_SIGNAL_COM3);
			sParam.test.addSignalToList(GB_SIGNAL_COM4);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);
		}
	}

	if ((!status) || (device == AVANT_NO)) {
		// ���� ���������� ������ �� �������� �� ��� ���� ��������� ���
		// �� ������ ������������ ������
		sParam.typeDevice = AVANT_NO;
		// � ������ ������������ ���� ����������, �������� ���
		for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
			measParam[i] = MENU_MEAS_PARAM_NO;
		measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
		measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

		sParam.def.status.setEnable(false);
		sParam.prm.status.setEnable(false);
		sParam.prd.status.setEnable(false);

		lvlMenu = &clMenu::lvlError;
	}

	// "�����" ����� ������������� �������� ���� ��������
	sParam.device = true;

	return status;
}

/**	���������� ��������� ������� �� ����������.
 * 	������� ����������� ������� �������, ���� �� ��� ���� ������� �������.
 * 	������ ���� (��������� �� 4-� �������) ������������:
 * 	������� ���������
 * 	������� �� ������ 1
 * 		������ ������ ���� ������� ��������, ���� ����
 * 		����� ������� ����������� ��� ��������� ���� (������������� � �.�.)
 * 		���� ������ � ������ ���, �� ���������� ������� �� ������ 2
 * 	������� �� ������ 2
 * 		������� ������ ��������� ���������� � ������ ������ ����
 * 	@param ���
 * 	@return �������
 */
eGB_COM clMenu::getTxCommand() {
	static uint8_t cnt = 0;
	eGB_COM com = sParam.txComBuf.getFastCom();

	if (com == GB_COM_NO) {
		cnt++;
		if (cnt == 1)
			com = GB_COM_GET_SOST;
		else if (cnt == 2)
			com = sParam.txComBuf.getCom1();

		// ���� ��� �������, �������� ������� �� ������ 2
		if (com == GB_COM_NO)
			com = sParam.txComBuf.getCom2();

		// �������� ���� �������, ���� ���������� 4 �������
		if (cnt >= 4)
			cnt = 0;
	}

	return com;
}

/** ������� ���������� ������
 * 	@param ���
 * 	@return ���
 */
void clMenu::clearTextBuf() {
	for (uint_fast8_t i = 0; i < sizeof(vLCDbuf); i++)
		vLCDbuf[i] = ' ';
}

/** ������� ������
 * 	@param line ����� ������ 1..NUM_TEXT_LINES
 * 	@retrun ���
 */
void clMenu::clearLine(uint8_t line) {
	if ((line > 0) && (line <= NUM_TEXT_LINES)) {
		line = (line - 1) * 20;
		for (uint_fast8_t i = 0; i < 20; i++)
			vLCDbuf[line++] = ' ';
	}
}

/**	����� � ��������� ����� ������������� ���������.
 * 	� ����� ������ ��������� ��� ���������.
 * 	@param poz ������� �������
 * 	@arg 0..eMENU_MEAS_PARAM, 0 ������ ������ �����, 5 - ������ ������
 * 	@param par ������������ ��������
 * 	@return ���
 */
void clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par) {
	// �������� � ������
	if (poz < MAX_NUM_MEAS_PARAM) {
		// 10 - ���-�� �������� ���������� �� ������ ��� 1 ��������
		poz = (poz * 10) + 1;

		switch (par) {
		case MENU_MEAS_PARAM_DATE:
			snprintf_P(&vLCDbuf[poz], 11, fcDate, sParam.dataTime.getDay(),
					sParam.dataTime.getMonth(), sParam.dataTime.getYear());
			break;
		case MENU_MEAS_PARAM_TIME:
			snprintf_P(&vLCDbuf[poz], 11, fcTime, sParam.dataTime.getHour(),
					sParam.dataTime.getMinute(), sParam.dataTime.getSecond());
			break;
		case MENU_MEAS_PARAM_UZ:
			snprintf_P(&vLCDbuf[poz], 11, fcUz,
					sParam.measParam.getVoltageDef());
			break;

			// TODO ��� ��� U�1 � U�2 ����� ���� ���������
		case MENU_MEAS_PARAM_UZ1:
			snprintf_P(&vLCDbuf[poz], 11, fcUz1,
					sParam.measParam.getVoltageDef());
			break;
		case MENU_MEAS_PARAM_UZ2:
			snprintf_P(&vLCDbuf[poz], 11, fcUz2,
					sParam.measParam.getVoltageDef());
			break;

		case MENU_MEAS_PARAM_UC:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf,
					sParam.measParam.getVoltageCf());
			break;

			// TODO ��� ��� U�1 � U�2 ����� ���� ���������
		case MENU_MEAS_PARAM_UC1:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf1,
					sParam.measParam.getVoltageCf());
			break;
		case MENU_MEAS_PARAM_UC2:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf2,
					sParam.measParam.getVoltageCf());
			break;
		case MENU_MEAS_PARAM_UOUT:
			snprintf_P(&vLCDbuf[poz], 11, fcUout,
					sParam.measParam.getVoltageOutInt(),
					sParam.measParam.getVoltageOutFract());
			break;

		case MENU_MEAS_PARAM_IOUT:
			snprintf_P(&vLCDbuf[poz], 11, fcIout,
					sParam.measParam.getCurrentOut());
			break;
		case MENU_MEAS_PARAM_ROUT:
			snprintf_P(&vLCDbuf[poz], 11, fcRout,
					sParam.measParam.getResistOut());
			break;

		case MENU_MEAS_PARAM_UN:
			snprintf_P(&vLCDbuf[poz], 11, fcUn,
					sParam.measParam.getVoltageNoise());
			break;

			// TODO ��� ��� U�1 � U�2 ����� ���� ���������
		case MENU_MEAS_PARAM_UN1:
			snprintf_P(&vLCDbuf[poz], 11, fcUn1,
					sParam.measParam.getVoltageNoise());
			break;
		case MENU_MEAS_PARAM_UN2:
			snprintf_P(&vLCDbuf[poz], 11, fcUn2,
					sParam.measParam.getVoltageNoise());
			break;
		default:
			// ������ �� ������
			break;
		}
	}
}

/**	����� �� ����� �������� ��������� ����������.
 * 	���� ���� ����� ������������� - ��������� ���������� ����������� �����
 * 	������������ � ��� (���� ���� ����).
 * 	����� ����������� ������� ������������� ���������� � ���� ����, ���������
 * 	�� ���� �� ���������.
 * 	���� ���� ����� �������������� - ��������� ��������� ��������� � �����������
 * 	������� ��������������. ���� �������������� ���������, ��������� ������ ���.
 * 	����� ����������� ������� �������������� ��� ���������� � ���� ����,
 * 	�������� �� ���� �� ���������.
 * 	���� ������ ������� ��������������/��������������, �� ��������� �������
 * 	��������� � ����� ������ ����������.
 * 	@param poz ��������� ������� � ������ ������ ���
 * 	@param device ������ ��� �������� ����������
 *	@return ���
 */
void clMenu::printDevicesStatus(uint8_t poz, TDeviceStatus *device) {
	PGM_P *text;
	uint_fast8_t x = 0;
	uint_fast16_t y = 0;

	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;

	if (sParam.glb.status.getNumFaults() != 0)
	{
		if (blink_)
		{
			text = sParam.glb.status.faultText;
			x = sParam.glb.status.getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = sParam.glb.status.getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'g', y);
		}
	}
	else if (device->getNumFaults() != 0)
	{
		if (blink_)
		{
			text = device->faultText;
			x = device->getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = device->getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'l', y);
		}
	}
	else if ( (sParam.glb.status.getNumWarnings() != 0) && (blink_) )
	{
		if (sParam.glb.status.getNumWarnings() == 1)
		{
			text = sParam.glb.status.warningText;
			x = sParam.glb.status.getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = sParam.glb.status.getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else if ( (device->getNumWarnings()!= 0) && (blink_) )
	{
		if (device->getNumWarnings() == 1)
		{
			text = device->warningText;
			x = device->getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = device->getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else
	{
		text = device->stateText;
		poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
		snprintf_P(&vLCDbuf[poz], 9,
				text[device->getState()], device->getDopByte());
	}
}

/**	����� � ������ ���� "�����" �������� ������ ���������
 * 	@param poz ��������� ������� � ������ ������ ���
 * 	@param device ������ ��� �������� ����������
 *	@return ���
 */
void clMenu::printDevicesRegime(uint8_t poz, TDeviceStatus *device) {
	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;
	poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}

/** ������� "��������� ��� ��������"
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlError() {
	static const char fcNoTypeDevice0[] PROGMEM = "    ��� ��������    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   �� ���������!!!  ";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		vLCDclear();
		// ������ ���� ������ ��������� ��� ����� ����������
		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_VERS);
	}

	// ����� �� ����� ���������� ����������
	printMeasParam(0, measParam[0]);
	printMeasParam(1, measParam[1]);

	snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
	snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);

	if (sParam.typeDevice != AVANT_NO) {
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
	}
}

/** ������� ���������
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlStart() {

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorEnable_ = false;
		lineParam_ = 3;
		numPunkts_ = 0;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();
		// �������������� �������
		// ����� 1
		// �������������, ���������� ��������� (� ��)
		sParam.txComBuf.addCom1(GB_COM_GET_FAULT);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);

		// ����� 2
		// �����, �� � �400
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
		if (sParam.typeDevice == AVANT_R400M)
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_TYPE_AC);
	}

	// ����� �� ����� ���������� ����������
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (blinkMeasParam_)
			printMeasParam(i, measParam[i]);
		else
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
	}

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;

		// � �400� ��������� �� � ����� �� ��
		if (sParam.typeDevice == AVANT_R400M) {
			uint16_t time = sParam.def.getTimeToAC();
			eGB_TYPE_AC ac = sParam.def.getTypeAC();
			uint8_t t = poz + 20;
			t += snprintf_P(&vLCDbuf[t],11,fcAcType[static_cast<uint8_t>(ac)]);

			// ����� �� ��
			// ��������� ���� ����������� �������:
			// 1. �� �� ��������
			// 2. ����� = ������
			// 3. ��������� = ��������
			if (ac != GB_TYPE_AC_OFF) {
				if (sParam.def.status.getRegime() == GB_REGIME_ENABLED) {
					if (sParam.def.status.getState() == 1) {
						uint8_t hour = time / 3600;
						uint8_t min = (time % 3600) / 60;
						uint8_t sec = time % 60;
						snprintf_P(&vLCDbuf[t + 1], 11, fcTimeToAc, hour, min,
								sec);
						poz += 20;
					}
				}
			}

		}
	}
	if (sParam.prm.status.isEnable()) {
		printDevicesStatus(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable()) {
		printDevicesStatus(poz, &sParam.prd.status);
		poz += 20;
	}

	eGB_TYPE_DEVICE type = sParam.typeDevice;
	if (sParam.typeDevice == AVANT_R400M) {
		switch (key_) {
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_CALL:
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_PRD:
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_AC_UD:
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_PUSK_NALAD:
			if (sParam.def.status.getState() == 7) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
			break;

		case KEY_FUNC_RESET_AC:
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_FUNC_AC_PUSK:
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
			break;

		case KEY_FUNC_REG_AC:
			sParam.txComBuf.setInt8(GB_CONTROL_REG_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		default:
			break;
		}
	} else {
		switch (key_) {
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_RES_IND:
			if ((type == AVANT_K400) || (type == AVANT_RZSK))
				sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND);
			break;

		case KEY_FUNC_ENTER:
			if ((type == AVANT_K400) || (type == AVANT_RZSK))
				sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER);
			break;

		case KEY_FUNC_RESET:
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		default:
			break;
		}
	}
}

/** ������� ���� ������
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlFirst() {
	static char title[] PROGMEM = "����";
	static char punkt1[] PROGMEM = "%d. ������";
	static char punkt2[] PROGMEM = "%d. ����������";
	static char punkt3[] PROGMEM = "%d. ���������";
	static char punkt4[] PROGMEM = "%d. ����";
	static char punkt5[] PROGMEM = "%d. ����������";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		lineParam_ = 1;
		cursorEnable_ = true;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		punkt_[num++] = punkt5;
		numPunkts_ = num;

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	if (isMessage()) {
		static char message[][21] PROGMEM = { "    ��������� �     ",
				"   �������� �����   " };

		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		uint8_t poz = 40;
		for (uint_fast8_t i = 0; i < 2; i++, poz += 20)
			snprintf_P(&vLCDbuf[poz], 21, message[i]);
	} else
		printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_ENTER:
		switch (cursorLine_) {
		case 1:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case 2:
			lvlMenu = &clMenu::lvlControl;
			lvlCreate_ = true;
			break;
		case 3:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case 4:
			if (sParam.glb.status.getRegime() == GB_REGIME_TEST_1) {
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			} else if (sParam.glb.status.getRegime() == GB_REGIME_TEST_2) {
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			} else
				printMessage();
			break;
		case 5:
			lvlMenu = &clMenu::lvlInfo;
			lvlCreate_ = true;
			break;
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** ������� ����. ���������� �� ��������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlInfo() {
	static char title[] PROGMEM = "����\\����������";
	static char bspMcu[] PROGMEM = "��� MCU : %02X.%02x";
	static char bspDsp[] PROGMEM = "��� DSP : %02X.%02x";
	static char piMcu[] PROGMEM = "��  MCU : %02X.%02x";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint16_t vers = sParam.glb.getVersBsp();
	snprintf_P(&vLCDbuf[20], 21, bspMcu, (uint8_t) (vers >> 8), (uint8_t) vers);
	vers = sParam.glb.getVersDsp();
	snprintf_P(&vLCDbuf[40], 21, bspDsp, (uint8_t) (vers >> 8), (uint8_t) vers);
	snprintf_P(&vLCDbuf[60], 21, piMcu, (uint8_t) (VERS >> 8), (uint8_t) VERS);

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** ������� ����. �������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournal() {
	static char title[] PROGMEM = "����\\������";
	static char punkt1[] PROGMEM = "%d. �������";
	static char punkt2[] PROGMEM = "%d. ������";
	static char punkt3[] PROGMEM = "%d. ��������";
	static char punkt4[] PROGMEM = "%d. ����������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();

		// ��������� ����������� ������� ���� � ��������������� �� ������
		// �� ��������� ������ ������ �������
		punkt_[num++] = punkt1;
		if (type == AVANT_R400M) {
			punkt_[num++] = punkt2;
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		} else if (type == AVANT_K400) {
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;
	case KEY_ENTER:
		if (punkt_[cursorLine_ - 1] == punkt1) {
			lvlMenu = &clMenu::lvlJournalEvent;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			lvlMenu = &clMenu::lvlJournalDef;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			lvlMenu = &clMenu::lvlJournalPrm;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			lvlMenu = &clMenu::lvlJournalPrd;
			lvlCreate_ = true;
		}
		break;

	default:
		break;
	}
}

/** ������� ����. ������ �������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournalEvent() {
	static char title[] PROGMEM = "������\\�������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
		uint16_t t = 0;
		if (sParam.typeDevice == AVANT_K400)
			t = GLB_JRN_EVENT_K400_MAX;
		else if (sParam.typeDevice == AVANT_R400M)
			t = GLB_JRN_EVENT_R400_MSK_MAX;
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// ����� ����� ������� ������ � �� ���-��
	snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// ����� ������
		snprintf_P(&vLCDbuf[poz], 21, fcRegimeJrn);
		snprintf_P(&vLCDbuf[poz + 7], 13,
				fcRegime[sParam.jrnEntry.getRegime()]);
		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// ����� �������
		uint8_t event = sParam.jrnEntry.getEventType();

		if (sParam.typeDevice == AVANT_R400M) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
		} else
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ ������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournalDef() {
	static char title[] PROGMEM = "������\\������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_DEF);
		uint16_t t = 0;
		if (sParam.typeDevice == AVANT_R400M)
			t = GLB_JRN_DEF_R400_MSK_MAX;
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_DEF_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// ����� ����� ������� ������ � �� ���-��
	snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// ����� ���������
		snprintf_P(&vLCDbuf[poz], 21, fcStateJrn);
		snprintf_P(&vLCDbuf[poz + 11], 10,
				sParam.def.status.stateText[sParam.jrnEntry.getEventType()]);
		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());

		poz += 20;
//		uint8_t signals = sParam.jrnEntry.getSignalDef();
//		snprintf(&vLCDbuf[poz], 21, "%x",signals);
		snprintf_P(&vLCDbuf[poz], 21, fcSignalDefJrn,
				sParam.jrnEntry.getSignalPusk(),
				sParam.jrnEntry.getSignalStop(), sParam.jrnEntry.getSignalMan(),
				sParam.jrnEntry.getSignalPrd(), sParam.jrnEntry.getSignalPrm(),
				sParam.jrnEntry.getSignalOut());
//		if (sParam.typeDevice == AVANT_R400_MSK)
//		{
//			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
//		}
//		else
//			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ ���������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournalPrm() {
	static char title[] PROGMEM = "������\\��������";
	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
		if (sParam.typeDevice == AVANT_K400)
			sParam.jrnEntry.setMaxNumJrnEntries(GLB_JRN_PRM_K400_MAX);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// ����� ����� ������� ������ � �� ���-��
	snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// ����� ������ �������
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, sParam.jrnEntry.getNumCom());
		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// ����� �������
		uint8_t event = sParam.jrnEntry.getEventType();
		snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ �����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournalPrd() {
	static char title[] PROGMEM = "������\\����������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorEnable_ = false;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
		if (sParam.typeDevice == AVANT_K400)
			sParam.jrnEntry.setMaxNumJrnEntries(GLB_JRN_PRD_K400_MAX);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// ����� ����� ������� ������ � �� ���-��
	snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// ����� ������ �������
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, sParam.jrnEntry.getNumCom());
		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dataTime.getDay(),
				sParam.jrnEntry.dataTime.getMonth(),
				sParam.jrnEntry.dataTime.getYear());
		poz += 20;
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dataTime.getHour(),
				sParam.jrnEntry.dataTime.getMinute(),
				sParam.jrnEntry.dataTime.getSecond(),
				sParam.jrnEntry.dataTime.getMsSecond());
		poz += 20;
		// ����� �������
		uint8_t event = sParam.jrnEntry.getEventType();
		snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry())
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;

	default:
		break;
	}

	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/**	������� ����. ����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlControl() {
	static char title[] PROGMEM = "����\\����������";
	// %d - ����� ���� �����������, ������ ��� ����. ���-�� �������� !
	//							   	"01234567890123456789"
	static char punkt02[] PROGMEM = "%d. ���� ����������";
	static char punkt03[] PROGMEM = "%d. ����� ������";
	static char punkt04[] PROGMEM = "%d. ����� ����������";
	static char punkt05[] PROGMEM = "%d. �����";
	static char punkt06[] PROGMEM = "%d. ���� �����. ���.";
	static char punkt07[] PROGMEM = "%d. ���� �����. ����";
	static char punkt08[] PROGMEM = "%d. �� ����";
	static char punkt09[] PROGMEM = "%d. ���� ������. ���";
	static char punkt10[] PROGMEM = "%d. �� �����.������.";
	static char punkt11[] PROGMEM = "%d. ����� ��";
	static char punkt12[] PROGMEM = "%d. ���� �� ����";
	static char punkt13[] PROGMEM = "%d. ���� �� �������.";
	static char punkt14[] PROGMEM = "%d. ���� ���";
	static char punkt15[] PROGMEM = "%d. �� ������������.";
	static char punkt16[] PROGMEM = "%d. �� ����������";
	static char punkt17[] PROGMEM = "%d. �� ��������";
	static char punkt18[] PROGMEM = "%d. �� ���������";
	static char punkt19[] PROGMEM = "%d. �� ����������";
	static char punkt20[] PROGMEM = "%d. �� ������";
	static char punkt21[] PROGMEM = "%d. �� �������������";
	static char punkt22[] PROGMEM = "%d. ����� ���������";
	static char punkt23[] PROGMEM = "%d. ���� �������. 1";
	static char punkt24[] PROGMEM = "%d. ���� �������. 2";
	static char punkt25[] PROGMEM = "%d. ���� �������. 3";
	static char punkt26[] PROGMEM = "%d. ���� ���������";
	static char punkt27[] PROGMEM = "%d. ���� ����. ��� 1";
	static char punkt28[] PROGMEM = "%d. ���� ����. ��� 2";
	static char punkt29[] PROGMEM = "%d. ���� ����. ��� 3";
	static char punkt30[] PROGMEM = "%d. ���� ����-�� ���";
	static char punkt31[] PROGMEM = "%d. �� �������";


	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ����� ����� ����� �������������
		sParam.glb.isCompatibilityRefresh();
		// ����� ����� ����� ���-�� ���������
		sParam.glb.isNumDevicesRefresh();

		uint8_t num = 0;
		if (device == AVANT_R400M) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			eGB_COMPATIBILITY compatibility = sParam.glb.getCompatibility();

			// ������ ������ ���� ���� ����������
			punkt_[num++] = punkt07;
			// ����� ������ ���� �� ���� ��������� � ��������������
			punkt_[num++] = punkt03;
			if (compatibility == GB_COMPATIBILITY_AVANT) {
				if (numDevices == GB_NUM_DEVICES_2) {
					punkt_[num++] = punkt04;
					punkt_[num++] = punkt02;
					punkt_[num++] = punkt11;
					punkt_[num++] = punkt12;
					punkt_[num++] = punkt13;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt17;
				} else if (numDevices == GB_NUM_DEVICES_3) {
					punkt_[num++] = punkt22;
					punkt_[num++] = punkt23;
					punkt_[num++] = punkt24;
					punkt_[num++] = punkt26;
					punkt_[num++] = punkt11;
					punkt_[num++] = punkt12;
					punkt_[num++] = punkt13;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt17;
				}
			} else if (compatibility == GB_COMPATIBILITY_PVZ90) {
				punkt_[num++] = punkt04;
				punkt_[num++] = punkt19;
				punkt_[num++] = punkt16;
				punkt_[num++] = punkt17;
				punkt_[num++] = punkt18;
				punkt_[num++] = punkt08;
			} else if (compatibility == GB_COMPATIBILITY_PVZUE) {
				if (numDevices == GB_NUM_DEVICES_2) {
					punkt_[num++] = punkt02;
					punkt_[num++] = punkt09;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt20;
					punkt_[num++] = punkt10;
					punkt_[num++] = punkt17;
				} else if (numDevices == GB_NUM_DEVICES_3) {
					punkt_[num++] = punkt23;
					punkt_[num++] = punkt24;
					punkt_[num++] = punkt26;
					punkt_[num++] = punkt27;
					punkt_[num++] = punkt28;
					punkt_[num++] = punkt30;
					punkt_[num++] = punkt19;
					punkt_[num++] = punkt16;
					punkt_[num++] = punkt20;
					punkt_[num++] = punkt10;
					punkt_[num++] = punkt17;
				}
			} else if (compatibility == GB_COMPATIBILITY_AVZK80) {
				punkt_[num++] = punkt19;
				punkt_[num++] = punkt16;
				punkt_[num++] = punkt17;
				punkt_[num++] = punkt18;
				punkt_[num++] = punkt08;
			} else if (compatibility == GB_COMPATIBILITY_PVZL) {
				punkt_[num++] = punkt11;
				punkt_[num++] = punkt12;
				punkt_[num++] = punkt13;
				punkt_[num++] = punkt14;
				punkt_[num++] = punkt31;
				punkt_[num++] = punkt17;
			}
			// ����� ���� �� ���� ��������������
			punkt_[num++] = punkt05;
		} else if (sParam.typeDevice == AVANT_RZSK) {
			punkt_[num++] = punkt07;
			punkt_[num++] = punkt03;
			punkt_[num++] = punkt05;
		} else if (sParam.typeDevice == AVANT_K400) {
			punkt_[num++] = punkt03;
		}
		numPunkts_ = num;

		// ������������� �������
		sParam.txComBuf.clear();
		// TODO �400� ���� ������������� ������� ���������� � ��, �� �� ����
		if (sParam.typeDevice == AVANT_R400M) {
			// �������������
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// ���-�� ��������� � �����
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// ����� �������� ��������
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		}
	}


	// � �400� ���� ���������� ������� �� ���-�� ��������� � �����
	// � ���� �������������
	// ������� ��� �� ��������� ������� ������� ����
	if (device == AVANT_R400M) {
		if (sParam.glb.isCompatibilityRefresh())
			lvlCreate_ = true;

		if (sParam.glb.isNumDevicesRefresh())
			lvlCreate_ = true;
	}

	// � ����/�400�
	if ((device == AVANT_R400M) || (device == AVANT_RZSK))
		punkt_[0] = (sParam.def.status.getState() != 7) ? punkt06 : punkt07;



	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_ENTER: {
		PGM_P p = punkt_[cursorLine_ - 1];

		if (p == punkt02)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt03)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt04)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt05)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt06)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt07)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt11)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt12)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt13)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt14)
		{
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
//		else if (p == punkt15)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_FAST);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (p == punkt16)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_FAST);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt17)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		else if (p == punkt19)
		{
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
//		else if (p == punkt21)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (p == punkt22) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt23) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt24) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt25) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt26) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt27) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt28) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt29) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt30) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		else if (p == punkt31) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
	}
	break;

	default:
	break;
}

}

/** ������� ����. ���������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetup() {
	static char title[] PROGMEM = "����\\���������";
	static char punkt1[] PROGMEM = "%d. �����";
	static char punkt2[] PROGMEM = "%d. ����� � ����";
	static char punkt3[] PROGMEM = "%d. ���������";
	static char punkt4[] PROGMEM = "%d. ������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		numPunkts_ = num;

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();
	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterParam.getStatus();

		// ����� ������� ����� : ������ ��� ��������
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PASSWORD_READY) {
			uint16_t val = enterParam.getValue();

			if (sParam.password.check(val)) {
				enterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
			} else
				enterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_N_READY) {
			uint16_t val = enterParam.getValue();

			sParam.password.set(val);
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;
	case KEY_ENTER: {
		PGM_P p = punkt_[cursorLine_ - 1];
		if (p == punkt1)
		{
			lvlMenu = &clMenu::lvlRegime;
			lvlCreate_ = true;
		}
		else if (p == punkt2)
		{
			lvlMenu = &clMenu::lvlSetupDT;
			lvlCreate_ = true;
		}
		else if (p == punkt3)
		{
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
		}
		else if (p == punkt4)
		{
			enterFunc = &clMenu::enterPassword;
			enterParam.setEnable(MENU_ENTER_PASSWORD);
			enterParam.com = GB_COM_NO;
		}
	}
	break;

	default:
	break;
}
}

void clMenu::lvlRegime() {
	static char title[] PROGMEM = "���������\\�����";
	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ������������� �������
		sParam.txComBuf.clear();
		// ���-�� ��������� � �����
		sParam.txComBuf.addCom1(GB_COM_GET_DEVICE_NUM);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesRegime(poz, &sParam.def.status);
		poz += 20;
	}
	if (sParam.prm.status.isEnable()) {
		printDevicesRegime(poz, &sParam.prm.status);
		poz += 20;
	}
	if (sParam.prd.status.isEnable()) {
		printDevicesRegime(poz, &sParam.prd.status);
		poz += 20;
	}

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterParam.getStatus();

		// ����� ������� ����� : ������ ��� ��������
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			eGB_REGIME_ENTER val =
					(eGB_REGIME_ENTER) enterParam.getValueEnter();
			eGB_COM com = GB_COM_NO;

			if (val == GB_REGIME_ENTER_DISABLED)
				com = GB_COM_SET_REG_DISABLED;
			else if (val == GB_REGIME_ENTER_ENABLED)
				com = GB_COM_SET_REG_ENABLED;
			else if (val == GB_REGIME_ENTER_TEST_1) {
				com = GB_COM_SET_REG_TEST_1;
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 1);
			} else if (val == GB_REGIME_ENTER_TEST_2)
				com = GB_COM_SET_REG_TEST_2;

			sParam.txComBuf.addFastCom(com);
			enterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_READY) {
			// ������ ������ �����
			// ������� � ����� ������ ������ (����� ������ �������)
			if (sParam.password.check(enterParam.getValue())) {
				enterFunc = &clMenu::enterValue;
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_REGIME_ENTER_DISABLED,
						GB_REGIME_ENTER_DISABLED);
				enterParam.setValue(GB_REGIME_ENTER_DISABLED);
				enterParam.list = fcRegimeEnter[0];
				enterParam.com = GB_COM_NO;
			} else {
				enterParam.printMessage();
			}
		}
	}

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// �� ��������� ������ ��������� ������
			enterFunc = &clMenu::enterPassword;
			enterParam.setEnable(MENU_ENTER_PASSWORD);
		} else {
			// ���� ������� �������, �������� �����
			enterFunc = &clMenu::enterValue;
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_REGIME_ENTER_DISABLED,
					GB_REGIME_ENTER_MAX - 1);
			enterParam.setValue(GB_REGIME_ENTER_ENABLED);
			enterParam.list = fcRegimeEnter[0];
			enterParam.com = GB_COM_NO;

		}
		break;

	default:
		break;
	}
}

/** ������� ����. ��������� ����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParam() {
	static char title[] PROGMEM = "���������\\���������";
	static char punkt1[] PROGMEM = "%d. ������";
	static char punkt2[] PROGMEM = "%d. ��������";
	static char punkt3[] PROGMEM = "%d. ����������";
	static char punkt4[] PROGMEM = "%d. �����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		if (sParam.typeDevice == AVANT_R400M) {
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt4;
		} else if (sParam.typeDevice == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		} else if (sParam.typeDevice == AVANT_K400) {
			punkt_[num++] = punkt2;
			punkt_[num++] = punkt3;
			punkt_[num++] = punkt4;
		}
		numPunkts_ = num;

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 20, title);
	printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;
	case KEY_ENTER:
		if (punkt_[cursorLine_ - 1] == punkt1) {
			lvlMenu = &clMenu::lvlSetupParamDef;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			lvlMenu = &clMenu::lvlSetupParamPrm;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			lvlMenu = &clMenu::lvlSetupParamPrd;
			lvlCreate_ = true;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			lvlMenu = &clMenu::lvlSetupParamGlb;
			lvlCreate_ = true;
		}
		break;

	default:
		break;
	}

}

/** ������� ����. ��������� ���������� ������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamDef() {
	static char title[] PROGMEM = "���������\\������";
	static char punkt1[] PROGMEM = "��� ������";
	static char punkt2[] PROGMEM = "��� �����";
	static char punkt3[] PROGMEM = "���. ����� ��� ���.";
	static char punkt4[] PROGMEM = "���������� ���������";
	static char punkt5[] PROGMEM = "����������� ��������";
	static char punkt6[] PROGMEM = "����� ������� �� ��";
	static char punkt7[] PROGMEM = "������.������. �� ��";
	static char punkt8[] PROGMEM = "��� ���������";
	static char punkt9[] PROGMEM = "�������� ������ ��";
	static char punkt10[] PROGMEM = "������� ���";
	static char punkt11[] PROGMEM = "������� ���";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		enterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ����� ����� ��������� �������������
		sParam.glb.isCompatibilityRefresh();

		// ���������� �������� ���������� � ������
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DEF_TYPE);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_T_NO_MAN);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_OVERLAP);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DELAY);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_THRESH);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_DEC);
			punkt_[num++] = punkt8;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_PRM_TYPE);
		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY t = sParam.glb.getCompatibility();

			// ����������� ����� �������������, ��� ���������������� ����
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);

			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DEF_TYPE);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_T_NO_MAN);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_OVERLAP);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_DELAY);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_DEC);
			if (t == GB_COMPATIBILITY_AVANT) {
				punkt_[num++] = punkt9;
				sParam.txComBuf.addCom2(GB_COM_DEF_GET_PRM_TYPE);
			}
			punkt_[num++] = punkt10;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_FREQ_PRD);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_RZ_THRESH);
		}
		numPunkts_ = num;
	}

	// ���������� ������, ��� ������� ����� �������������
	if (sParam.glb.isCompatibilityRefresh())
		lvlCreate_ = true;

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_T_NO_MAN_MIN,
				DEF_T_NO_MAN_MAX, "���");
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_OVERLAP_MIN,
				DEF_OVERLAP_MAX, "����");
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_DELAY_MIN, DEF_DELAY_MAX,
				"����");
	} else if (p == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_THRESH_MIN,
				DEF_RZ_THRESH_MAX, "��");
	} else if (p == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_DEC_MIN,
				DEF_RZ_DEC_MAX, "��");
	} else if (p == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt4) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt5) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt6) {
				// !!! TODO ����
			} else if (p == punkt7) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt8) {
				// !!! TODO ����
			} else if (p == punkt9) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt10) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt11) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			}

			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {

		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			uint8_t val = sParam.def.getDefType();
			snprintf_P(&vLCDbuf[poz], 11, fcDefType[val]);
		} else if (p == punkt2) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		} else if (p == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%d���", sParam.def.getTimeNoMan());
		} else if (p == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%d����", sParam.def.getOverlap());
		} else if (p == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%d����", sParam.def.getDelay());
		} else if (p == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.def.getRzThreshold());
		} else if (p == punkt7) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.def.getRzDec());
		} else if (p == punkt8) {
			snprintf_P(&vLCDbuf[poz], 11, fcPrmType[sParam.def.getPrmType()]);
		} else if (p == punkt9) {
			uint8_t val = sParam.def.getAcDec() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt10) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrd()]);
		} else if (p == punkt11) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrm()]);
		}
	}
	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.def.status.getRegime() == GB_REGIME_ENABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			enterParam.printMessage();
		} else {

			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(DEF_TYPE_MIN, DEF_TYPE_MAX - 1);
				enterParam.setValue(sParam.def.getDefType());
				enterParam.list = fcDefType[0];
				enterParam.com = GB_COM_DEF_SET_DEF_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_NUM_DEVICES_MIN,
						GB_NUM_DEVICES_MAX - 1);
				enterParam.setValue(sParam.def.getNumDevices());
				enterParam.list = fcNumDevices[0];
				enterParam.com = GB_COM_DEF_SET_LINE_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_T_NO_MAN_MIN, DEF_T_NO_MAN_MAX);
				enterParam.setValue(sParam.def.getTimeNoMan());
				enterParam.setDisc(DEF_T_NO_MAN_DISC);
				enterParam.setFract(DEF_T_NO_MAN_FRACT);
				enterParam.com = GB_COM_DEF_SET_T_NO_MAN;
			} else if (punkt_[cursorLine_ - 1] == punkt4) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_OVERLAP_MIN, DEF_OVERLAP_MAX);
				enterParam.setValue(sParam.def.getOverlap());
				enterParam.setDisc(DEF_OVERLAP_DISC);
				enterParam.setFract(DEF_OVERLAP_FRACT);
				enterParam.com = GB_COM_DEF_SET_OVERLAP;
			} else if (punkt_[cursorLine_ - 1] == punkt5) {
				enterParam.setEnable();
				enterParam.setValueRange(DEF_DELAY_MIN, DEF_DELAY_MAX);
				enterParam.setValue(sParam.def.getDelay());
				enterParam.setDisc(DEF_DELAY_DISC);
				enterParam.setFract(DEF_DELAY_FRACT);
				enterParam.com = GB_COM_DEF_SET_DELAY;
			} else if (punkt_[cursorLine_ - 1] == punkt6) {
				// !!! ���� ���� �������
			} else if (punkt_[cursorLine_ - 1] == punkt7) {
				// !!! �400 � 3-� �������� 2 ������
				enterParam.setEnable();
				enterParam.setValueRange(DEF_RZ_DEC_MIN, DEF_RZ_DEC_MAX);
				enterParam.setValue(sParam.def.getRzDec());
				enterParam.setDisc(DEF_RZ_DEC_DISC);
				enterParam.setFract(DEF_RZ_DEC_FRACT);
				enterParam.com = GB_COM_DEF_SET_RZ_DEC;
			} else if (punkt_[cursorLine_ - 1] == punkt8) {
				// !!! ���� ���� �������
			} else if (punkt_[cursorLine_ - 1] == punkt9) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.def.getAcDec() ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_SET_PRM_TYPE;
			} else if (punkt_[cursorLine_ - 1] == punkt10) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_PVZL_FREQ_MIN, GB_PVZL_FREQ_MAX-1);
				enterParam.setValue(sParam.def.getFreqPrd());
				enterParam.list = fcPvzlFreq[0];
				enterParam.com = GB_COM_DEF_SET_FREQ_PRD;
			} else if (punkt_[cursorLine_ - 1] == punkt11) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(GB_PVZL_FREQ_MIN, GB_PVZL_FREQ_MAX-1);
				enterParam.setValue(sParam.def.getFreqPrm());
				enterParam.list = fcPvzlFreq[0];
				enterParam.com = GB_COM_DEF_SET_RZ_THRESH;
			}
		}
		break;

	default:
		break;
	}
}

/** ������� ����. ��������� ���������� ���������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamPrm() {
	static char title[] PROGMEM = "���������\\��������";

	static char punkt1[] PROGMEM = "�������� �� ����.���";
	static char punkt2[] PROGMEM = "��������. �������";
	static char punkt3[] PROGMEM = "�������� �� ����.���";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_BLOCK_COM);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_ON);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_OFF);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_BLOCK_COM);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_ON);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_TIME_OFF);
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	// ��� �������������, ����� ��������� ������� ���������
	poz = 60;
	if ((p == punkt2) || (p == punkt3)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prm.getNumCom());
	}

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_ON_MIN,
				PRM_TIME_ON_MAX, "��");
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_OFF_MIN,
				PRM_TIME_OFF_MAX, "��");
	}

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prm.getTimeOn());
		} else if (p == punkt2) {
			uint8_t val = (sParam.prm.getBlockCom(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%d��",
					sParam.prm.getTimeOff(curCom_ - 1));
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		curCom_ = 1;
		break;
	case KEY_DOWN:
		cursorLineDown();
		curCom_ = 1;
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.prm.getNumCom() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.prm.getNumCom() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			enterParam.printMessage();
		} else {
			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable();
				enterParam.setValueRange(PRM_TIME_ON_MIN, PRM_TIME_ON_MAX);
				enterParam.setValue(sParam.prm.getTimeOn());
				enterParam.setDisc(PRM_TIME_ON_DISC);
				enterParam.setFract(PRM_TIME_ON_FRACT);
				enterParam.com = GB_COM_PRM_SET_TIME_ON;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.setDopValue(curCom_);
				enterParam.com = GB_COM_PRM_SET_BLOCK_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable();
				enterParam.setValueRange(PRM_TIME_OFF_MIN, PRM_TIME_OFF_MAX);
				enterParam.setValue(sParam.prm.getTimeOff(curCom_ - 1));
				enterParam.setDopValue(curCom_);
				enterParam.setDisc(PRM_TIME_OFF_DISC);
				enterParam.setFract(PRM_TIME_OFF_FRACT);
				enterParam.com = GB_COM_PRM_SET_TIME_OFF;
			}
		}
		break;

	default:
		break;
	}
}

/** ������� ����. ��������� ���������� �����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamPrd() {
	static char title[] PROGMEM = "���������\\����������";
	static char punkt1[] PROGMEM = "�������� ������. ���";
	static char punkt2[] PROGMEM = "������������ �������";
	static char punkt3[] PROGMEM = "�������� �������";
	static char punkt4[] PROGMEM = "�������� �������";
	static char punkt5[] PROGMEM = "��������. �������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TIME_ON);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_DURATION);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TEST_COM);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_LONG_COM);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_BLOCK_COM);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_TIME_ON);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_DURATION);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_LONG_COM);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_BLOCK_COM);
		}
		numPunkts_ = num;
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	// ��� ������������� ������� �������� ������� ���������
	poz = 60;
	if ((p == punkt4) || (p == punkt5)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prd.getNumCom());
	}

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_TIME_ON_MIN,
				PRD_TIME_ON_MAX, "��");
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURATION_MIN,
				PRD_DURATION_MAX, "��");
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	}

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());

			} else if (p == punkt4) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getLongCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (p == punkt5) {
				uint8_t pl = enterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockCom8(pl / 8);
				if (enterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {

		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prd.getTimeOn());
		} else if (p == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prd.getDuration());
		} else if (p == punkt3) {
			uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt4) {
			uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt5) {
			uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		curCom_ = 1;
		break;
	case KEY_DOWN:
		cursorLineDown();
		curCom_ = 1;
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.prd.getNumCom() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.prd.getNumCom() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			enterParam.printMessage();
		} else {

			if (punkt_[cursorLine_ - 1] == punkt1) {
				enterParam.setEnable();
				enterParam.setValueRange(PRD_TIME_ON_MIN, PRD_TIME_ON_MAX);
				enterParam.setValue(sParam.prd.getTimeOn());
				enterParam.setDisc(PRD_TIME_ON_DISC);
				enterParam.setFract(PRD_TIME_ON_FRACT);
				enterParam.com = GB_COM_PRD_SET_TIME_ON;
			} else if (punkt_[cursorLine_ - 1] == punkt2) {
				enterParam.setEnable();
				enterParam.setValueRange(PRD_DURATION_MIN, PRD_DURATION_MAX);
				enterParam.setValue(sParam.prd.getDuration());
				enterParam.setDisc(PRD_DURATION_DISC);
				enterParam.setFract(PRD_DURATION_FRACT);
				enterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (punkt_[cursorLine_ - 1] == punkt3) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
				enterParam.setValue(val);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_TEST_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt4) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.setDopValue(curCom_);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_LONG_COM;
			} else if (punkt_[cursorLine_ - 1] == punkt5) {
				enterParam.setEnable(MENU_ENTER_PARAM_LIST);
				enterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
				enterParam.setValue(val);
				enterParam.setDopValue(curCom_);
				enterParam.list = fcOnOff[0];
				enterParam.com = GB_COM_PRD_SET_BLOCK_COM;
			}
		}
		break;

	default:
		break;
	}
}

/** ������� ����. ��������� ���������� �����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamGlb() {
	static char title[] PROGMEM = "���������\\�����";
	static char punkt1[] PROGMEM = "������������� �����";
	static char punkt2[] PROGMEM = "����� ��������";
	static char punkt3[] PROGMEM = "�������� ���.�������";
	static char punkt4[] PROGMEM = "����� �������. �� ��";
	static char punkt5[] PROGMEM = "����� �����������";
	static char punkt6[] PROGMEM = "�����. ���� ���. ���";
	static char punkt7[] PROGMEM = "�����. ���� ���. ���";
	static char punkt8[] PROGMEM = "�������. ������. ���";
	static char punkt9[] PROGMEM = "������� �����";
	static char punkt10[] PROGMEM = "U��� �����������";
	static char punkt11[] PROGMEM = "�������";
	static char punkt12[] PROGMEM = "�������������";		// ������
	static char punkt13[] PROGMEM = "�������� ������ ��";
	static char punkt14[] PROGMEM = "��� ���������";
	static char punkt15[] PROGMEM = "��������� ����������";
	static char punkt16[] PROGMEM = "��������� ����";
	static char punkt17[] PROGMEM = "�������� ������";
	static char punkt18[] PROGMEM = "������� ��������";
	static char punkt19[] PROGMEM = "���������� �������";
	static char punkt20[] PROGMEM = "����� �� ������";
	static char punkt21[] PROGMEM = "���������� ������";
	static char punkt22[] PROGMEM = "��� ������������";
	static char punkt23[] PROGMEM = "��������������";
	static char punkt24[] PROGMEM = "�������������";		// �����

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		enterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		uint8_t num = 0;
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		sParam.glb.isCompatibilityRefresh();
		if (type == AVANT_K400) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt8;
			// sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);					// �.�. ��� ���� ����� �������
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt24;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);// TODO �400 ������� "�������������"
			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I	);
		} else if (type == AVANT_RZSK) {
			punkt_[num++] = punkt1;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt5;
			sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			punkt_[num++] = punkt6;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			punkt_[num++] = punkt7;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt8;
			// sParam.txComBuf.addCom(GB_COM_GET_CF_THRESHOLD);					// �.�. ��� ���� ����� �������
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt14;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);		// TODO ���� �������
			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I);

		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY t = sParam.glb.getCompatibility();

			// TODO �400� ���� �������������� ������� � ��, �� �� ����
			// ���������� �������������
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// ���������� ��������� ��� ���������
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);

			// � �������������� ������� �����������

			if (t == GB_COMPATIBILITY_AVANT) {
				punkt_[num++] = punkt1;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_SINCHR);
			}

			punkt_[num++] = punkt2;
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
			punkt_[num++] = punkt3;
			sParam.txComBuf.addCom2(GB_COM_GET_OUT_CHECK);
			punkt_[num++] = punkt4;
			sParam.txComBuf.addCom2(GB_COM_GET_CF_THRESHOLD);
			punkt_[num++] = punkt9;
			sParam.txComBuf.addCom2(GB_COM_GET_NET_ADR);
			punkt_[num++] = punkt10;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRM_KEEP);
			punkt_[num++] = punkt11;
			sParam.txComBuf.addCom2(GB_COM_GET_FREQ);
			punkt_[num++] = punkt12;
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			if (t == GB_COMPATIBILITY_PVZL) {
				punkt_[num++] = punkt13;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			}

			punkt_[num++] = punkt15;
			sParam.txComBuf.addCom2(GB_COM_GET_COR_U_I);
			punkt_[num++] = punkt16;
//			sParam.txComBuf.addCom(GB_COM_GET_COR_U_I);

			if (t == GB_COMPATIBILITY_PVZUE) {
				punkt_[num++] = punkt17;
				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt18;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt19;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt20;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt21;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
				punkt_[num++] = punkt22;
//				sParam.txComBuf.addCom2(GB_COM_GET_TIME_RERUN);
			}
		}
		numPunkts_ = num;
	}

	// ���������� ������, ��� ������� ����� �������������
	if (sParam.glb.isCompatibilityRefresh())
		lvlCreate_ = true;

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, numPunkts_);

	poz = 40;
	PGM_P p = punkt_[cursorLine_ - 1];
	snprintf_P(&vLCDbuf[poz], 21, p);

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (p == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DEV_NUM_MIN,
				sParam.glb.getMaxNumDevices(), "");
	} else if (p == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CF_THRESH_MIN,
				GLB_CF_THRESH_MAX, "��");
	} else if (p == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_T_RERUN_MIN,
				GLB_T_RERUN_MAX, "c");
	} else if (p == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_IN_DEC_MIN,
				GLB_IN_DEC_MAX, "��");
	} else if (p == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_NET_ADR_MIN,
				GLB_NET_ADR_MAX, "");
	} else if (p == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_U_OUT_NOM_MIN,
				GLB_U_OUT_NOM_MAX, "�");
	} else if (p == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_FREQ_MIN, GLB_FREQ_MAX,
				"���");
	} else if (p == punkt12) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt13) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_AC_IN_DEC_MIN,
				GLB_AC_IN_DEC_MAX, "��");
	} else if (p == punkt15) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_COR_U_DEC_MIN / 10,
				GLB_COR_U_DEC_MAX / 10, "�");
	} else if (p == punkt16) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, 0, GLB_COR_I_DEC_MAX, "��");
	} else if (p == punkt17) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (p == punkt18) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (p == punkt19) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_FAIL_MIN,
				GLB_PVZUE_FAIL_MAX, "����");
	} else if (p == punkt20) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_TH_MIN,
				GLB_PVZUE_N_TH_MAX, "���");
	} else if (p == punkt21) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_LVL_MIN,
				GLB_PVZUE_N_LVL_MAX, "����");
	} else if (p == punkt22) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (p == punkt1) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt2) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt3) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt4) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt5) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt6) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt7) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt8) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt9) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt10) {
				sParam.txComBuf.setInt8(enterParam.getValueEnter());
			} else if (p == punkt11) {
				uint16_t t = enterParam.getValue();
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t >> 8), 0);
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t), 1);
			} else if (p == punkt12) {
				uint8_t t = enterParam.getValueEnter();
				sParam.txComBuf.setInt8(t);
			} else if (p == punkt13) {
				sParam.txComBuf.setInt8(enterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(enterParam.getValueEnter(), 1);
			} else if (p == punkt15) {
				// ���� ������� �������� ��������� ���� ����� 0
				// �� ���������� ��������� � ���.������ ������ 4
				// ���������� ����� ���������
				int16_t t = static_cast<int16_t>(enterParam.getValue());
				uint8_t dop = enterParam.getDopValue();
				if (t == 0)
					dop = 4;
				else {
					// ����� ��������� =
					// ���������� ������� - (���������� � ��� - ���������)
					t -= static_cast<int16_t>(sParam.measParam.getVoltageOut());
					t += sParam.glb.getCorU();
				}
				sParam.txComBuf.setInt8(dop, 0);
				sParam.txComBuf.setInt8(t / 10, 1);
				sParam.txComBuf.setInt8((t % 10) * 10, 2);
			} else if (p == punkt16) {
				// ���� ������� �������� ��������� ���� ����� 0
				// �� ���������� ��������� � ���.������ ������ 5
				// ���������� ����� ���������
				int16_t t = static_cast<int16_t>(enterParam.getValue());
				uint8_t dop = enterParam.getDopValue();
				if (t == 0)
					dop = 5;
				else {
					// ����� ��������� = ��� ������� - (��� � ��� - ���������)
					t -= static_cast<int16_t>(sParam.measParam.getCurrentOut());
					t += sParam.glb.getCorI();
				}
				sParam.txComBuf.setInt8(dop, 0);
				sParam.txComBuf.setInt8((t >> 8), 1);
				sParam.txComBuf.setInt8((t), 2);
			} else if (p == punkt17) {
				// TODO �400� ����-�
			} else if (p == punkt18) {
				// TODO �400� ����-�
			} else if (p == punkt19) {
				// TODO �400� ����-�
			} else if (p == punkt20) {
				// TODO �400� ����-�
			} else if (p == punkt21) {
				// TODO �400� ����-�
			} else if (p == punkt22) {
				// TODO �400� ����-�
			}
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (p == punkt1) {
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getDeviceNum());
		} else if (p == punkt3) {
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getCfThreshold());
		} else if (p == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%d�", sParam.glb.getTimeRerun());
		} else if (p == punkt6) {
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt7) {
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (p == punkt8) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getInDecrease());
		} else if (p == punkt9) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getNetAddress());
		} else if (p == punkt10) {
			snprintf(&vLCDbuf[poz], 11, "%d�", sParam.glb.getUoutNom());
		} else if (p == punkt11) {
			snprintf(&vLCDbuf[poz], 11, "%d���", sParam.glb.getFreq());
		} else if (p == punkt12) {
			uint8_t val = static_cast<uint8_t>(sParam.glb.getCompatibility());
			snprintf_P(&vLCDbuf[poz], 11, fcCompatibility[val]);
		} else if (p == punkt13) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getAcInDec());
		} else if (p == punkt15) {
			int16_t val = sParam.glb.getCorU();
			int8_t f = val % 10;
			if (f < 0)
				f = -f;
			// �������� �����/������� ����� ���������� ���������
			// � ������� ����� ������������� ���������� ���� �����
			snprintf(&vLCDbuf[poz], 11, "%d.%d�", val / 10, f);
		} else if (p == punkt16) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getCorI());
		} else if (p == punkt17) {
			uint8_t val = sParam.glb.getPvzueProtocol();
			snprintf(&vLCDbuf[poz], 11, fcPvzueProtocol[val]);
		} else if (p == punkt18) {
			uint8_t val = sParam.glb.getPvzueParity();
			snprintf(&vLCDbuf[poz], 11, fcPvzueParity[val]);
		} else if (p == punkt19) {
			snprintf(&vLCDbuf[poz], 11, "%u����", sParam.glb.getPvzueFail());
		} else if (p == punkt20) {
			snprintf(&vLCDbuf[poz], 11, "%u���", sParam.glb.getPvzueNoiseTH());
		} else if (p == punkt21) {
			snprintf(&vLCDbuf[poz], 11, "%u����",
					sParam.glb.getPvzueNoiseLvl());
		} else if (p == punkt22) {
			uint8_t val = sParam.glb.getPvzueTypeAC();
			snprintf(&vLCDbuf[poz], 11, fcPvzueTypeAC[val]);
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// ��������� ���� � ���������� �������� � ����� ������
		// ��������� ��������� ������ � "�������"
		if (punkt_[cursorLine_ - 1] == punkt15) {
			// �������� �������� �������� ����������
			// �� ��������� ����� 0, ��� ������ ����� ��������
			enterParam.setEnable(MENU_ENTER_PARAM_U_COR);
			enterParam.setValueRange(0, GLB_COR_U_DEC_MAX);
			enterParam.setValue(0);
			enterParam.setDisc(GLB_COR_U_DEC_DISC);
			enterParam.setFract(GLB_COR_U_DEC_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_COR_U_I;
		} else if (punkt_[cursorLine_ - 1] == punkt16) {
			// �������� �������� �������� ����
			// �� ��������� ����� 0, ��� ������ ����� ��������
			enterParam.setEnable();
			enterParam.setValueRange(0, GLB_COR_I_DEC_MAX);
			enterParam.setValue(0);
			enterParam.setDisc(GLB_COR_I_DEC_DISC);
			enterParam.setFract(GLB_AC_IN_DEC_FRACT);
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_SET_COR_U_I;
		} else if (sParam.glb.status.getRegime() == GB_REGIME_ENABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			enterParam.printMessage();
		} else if (punkt_[cursorLine_ - 1] == punkt1) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_DEV_NUM_MIN,
					sParam.glb.getMaxNumDevices());
			enterParam.setValue(sParam.glb.getDeviceNum());
			enterParam.setDisc(GLB_DEV_NUM_DISC);
			enterParam.setFract(GLB_DEV_NUM_FRACT);
			enterParam.com = GB_COM_SET_DEVICE_NUM;
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_OUT_CHECK;
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_CF_THRESH_MIN, GLB_CF_THRESH_MAX);
			enterParam.setValue(sParam.glb.getCfThreshold());
			enterParam.setDisc(GLB_CF_THRESH_DISC);
			enterParam.setFract(GLB_CF_THRESH_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (punkt_[cursorLine_ - 1] == punkt5) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_T_RERUN_MIN, GLB_T_RERUN_MAX);
			enterParam.setValue(sParam.glb.getTimeRerun());
			enterParam.setDisc(GLB_T_RERUN_DISC);
			enterParam.setFract(GLB_T_RERUN_FRACT);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt6) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt7) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			enterParam.setValue(val);
			enterParam.list = fcOnOff[0];
			enterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt8) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_IN_DEC_MIN, GLB_IN_DEC_MAX);
			enterParam.setValue(sParam.glb.getInDecrease());
			enterParam.setDisc(GLB_IN_DEC_DISC);
			enterParam.setFract(GLB_IN_DEC_FRACT);
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (punkt_[cursorLine_ - 1] == punkt9) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_NET_ADR_MIN, GLB_NET_ADR_MAX);
			enterParam.setValue(sParam.glb.getNetAddress());
			enterParam.setDisc(GLB_NET_ADR_DISC);
			enterParam.setFract(GLB_NET_ADR_FRACT);
			enterParam.com = GB_COM_SET_NET_ADR;
		} else if (punkt_[cursorLine_ - 1] == punkt10) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_U_OUT_NOM_MIN, GLB_U_OUT_NOM_MAX);
			enterParam.setValue(sParam.glb.getUoutNom());
			enterParam.setDisc(GLB_U_OUT_NOM_DISC);
			enterParam.setFract(GLB_U_OUT_NOM_FRACT);
			enterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt11) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_FREQ_MIN, GLB_FREQ_MAX);
			enterParam.setValue(sParam.glb.getFreq());
			enterParam.setDisc(GLB_FREQ_DISC);
			enterParam.setFract(GLB_FREQ_FRACT);
			enterParam.com = GB_COM_SET_FREQ;
		} else if (punkt_[cursorLine_ - 1] == punkt12) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_COMPATIBILITY_MIN,
					GB_COMPATIBILITY_MAX - 1);
			uint8_t val = (uint8_t) sParam.glb.getCompatibility();
			enterParam.setValue(val);
			enterParam.list = fcCompatibility[0];
			enterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (punkt_[cursorLine_ - 1] == punkt13) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_AC_IN_DEC_MIN, GLB_AC_IN_DEC_MAX);
			enterParam.setValue(sParam.glb.getAcInDec());
			enterParam.setDisc(GLB_AC_IN_DEC_DISC);
			enterParam.setFract(GLB_AC_IN_DEC_FRACT);
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt17) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_PROTOCOL_MIN,
					GB_PVZUE_PROTOCOL_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueProtocol());
			enterParam.list = fcPvzueProtocol[0];
			enterParam.setDopValue(1);
			enterParam.com = GB_COM_GET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt18) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_PARITY_MIN,
					GB_PVZUE_PARITY_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueParity());
			enterParam.list = fcPvzueParity[0];
			enterParam.setDopValue(2);
			enterParam.com = GB_COM_GET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt19) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_FAIL_MIN, GLB_PVZUE_FAIL_MAX);
			enterParam.setValue(sParam.glb.getPvzueFail());
			enterParam.setDisc(GLB_PVZUE_FAIL_DISC);
			enterParam.setFract(GLB_PVZUE_FAIL_FRACT);
			enterParam.setDopValue(3);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt20) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_N_TH_MIN, GLB_PVZUE_N_TH_MAX);
			enterParam.setValue(sParam.glb.getPvzueNoiseTH());
			enterParam.setDisc(GLB_PVZUE_N_TH_DISC);
			enterParam.setFract(GLB_PVZUE_N_TH_FRACT);
			enterParam.setDopValue(4);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt21) {
			enterParam.setEnable();
			enterParam.setValueRange(GLB_PVZUE_N_LVL_MIN, GLB_PVZUE_N_LVL_MAX);
			enterParam.setValue(sParam.glb.getPvzueNoiseLvl());
			enterParam.setDisc(GLB_PVZUE_N_LVL_DISC);
			enterParam.setFract(GLB_PVZUE_N_LVL_FRACT);
			enterParam.setDopValue(5);
			enterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (punkt_[cursorLine_ - 1] == punkt22) {
			enterParam.setEnable(MENU_ENTER_PARAM_LIST);
			enterParam.setValueRange(GB_PVZUE_TYPE_AC_MIN,
					GB_PVZUE_TYPE_AC_MAX - 1);
			enterParam.setValue(sParam.glb.getPvzueTypeAC());
			enterParam.list = fcPvzueTypeAC[0];
			enterParam.setDopValue(6);
			enterParam.com = GB_COM_GET_TIME_RERUN;
		}
		break;
	default:
		break;
	}
}

/** ������� ����. ��������� ����/�����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupDT() {
	static char title[] PROGMEM = "���������\\�����&����";
	static char punkt1[] PROGMEM = "%d. ���";
	static char punkt2[] PROGMEM = "%d. �����";
	static char punkt3[] PROGMEM = "%d. ����";
	static char punkt4[] PROGMEM = "%d. ����";
	static char punkt5[] PROGMEM = "%d. ������";
	static char punkt6[] PROGMEM = "%d. �������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 2;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		uint8_t num = 0;
		punkt_[num++] = punkt1;
		punkt_[num++] = punkt2;
		punkt_[num++] = punkt3;
		punkt_[num++] = punkt4;
		punkt_[num++] = punkt5;
		punkt_[num++] = punkt6;
		numPunkts_ = num;

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
	}

	snprintf_P(&vLCDbuf[20], 21, title);

	printMeasParam(0, MENU_MEAS_PARAM_DATE);
	printMeasParam(1, MENU_MEAS_PARAM_TIME);

	if (enterParam.isEnable()) {
		// ����� �������� ������
		snprintf_P(&vLCDbuf[20 * lineParam_], 21, punkt_[cursorLine_ - 1],
				cursorLine_);

		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����������� ��������� �������� �� ���� �����
			sParam.txComBuf.setInt8(BIN_TO_BCD(enterParam.getValueEnter()),
					enterParam.getDopValue());
			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else
		printPunkts();

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (punkt_[cursorLine_ - 1] == punkt1) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 99);
			enterParam.setValue(sParam.dataTime.getYear());
			enterParam.setDopValue(0);
		} else if (punkt_[cursorLine_ - 1] == punkt2) {
			enterParam.setEnable();
			enterParam.setValueRange(1, 12);
			enterParam.setValue(sParam.dataTime.getMonth());
			enterParam.setDopValue(1);
		} else if (punkt_[cursorLine_ - 1] == punkt3) {
			enterParam.setEnable();
			uint8_t max = sParam.dataTime.getNumDaysInMonth();
			enterParam.setValueRange(1, max);
			enterParam.setValue(sParam.dataTime.getDay());
			enterParam.setDopValue(2);
		} else if (punkt_[cursorLine_ - 1] == punkt4) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 23);
			enterParam.setValue(sParam.dataTime.getHour());
			enterParam.setDopValue(3);
		} else if (punkt_[cursorLine_ - 1] == punkt5) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 59);
			enterParam.setValue(sParam.dataTime.getMinute());
			enterParam.setDopValue(4);
		} else if (punkt_[cursorLine_ - 1] == punkt6) {
			enterParam.setEnable();
			enterParam.setValueRange(0, 59);
			enterParam.setValue(sParam.dataTime.getSecond());
			enterParam.setDopValue(5);
		}
		enterParam.com = GB_COM_SET_TIME;
		enterParam.setDisc(1);
		enterParam.setFract(1);
		// �������� ������� �������� ���� � �������
		// ����� ����������� � ������� �������� � ���
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getYear()), 0);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getMonth()), 1);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getDay()), 2);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getHour()), 3);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getMinute()), 4);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dataTime.getSecond()), 5);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** ������� ����. ���� 1.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest1() {
	static char title[] PROGMEM = "����\\���� 1";
	static char punkt1[] PROGMEM = "������ �� ������";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		numPunkts_ = 1;

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);

		// ������� ��� ������
		if (sParam.typeDevice == AVANT_R400M)
		{
			sParam.test.clear();
			sParam.test.addSignalToList(GB_SIGNAL_RZ);
			if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT)
				sParam.test.addSignalToList(GB_SIGNAL_CF);
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	if (enterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ������� ������(�)
			uint8_t sig = enterParam.listValue[enterParam.getValue()];
			uint8_t rz = 0;
			uint8_t cf = 0;

			sParam.test.getBytes(cf, rz, (eGB_TEST_SIGNAL) sig);
			// TODO ���� ����� ���� ��� ������� �� � �� ������������
			// ������ ���� - ����� ������ (1 - ��, 2 - ��)
			// ������ ���� - ����� ������� (0 - ����.)
			if (rz > 0)
			{
				sParam.txComBuf.setInt8(2, 0);
				sParam.txComBuf.setInt8(rz, 1);
			} else if (cf > 0) {
				sParam.txComBuf.setInt8(1, 0);
				sParam.txComBuf.setInt8(cf, 1);
			} else {
				// ����� ������� 0, ����� ������ �� ������������
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 1);
			}

			sParam.txComBuf.addFastCom(enterParam.com);
			enterParam.setDisable();
		}
	} else {
		uint8_t poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// ����� �� ����� ��� ��������������� ������
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_1)
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterParam.setValue(0);
		enterParam.setEnable(MENU_ENTER_PARAM_LIST_2);
		enterParam.setValueRange(0, sParam.test.getNumSignals() - 1);
		enterParam.listValue = sParam.test.signalList;
		enterParam.list = fcTest1K400[0];
		enterParam.com = GB_COM_SET_REG_TEST_1;
		break;

	default:
		break;
	}
}

/** ������� ����. ���� 2.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest2() {
	static char title[] PROGMEM = "����\\���� 2";
	static char punkt1[] PROGMEM = "������ �� �����";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		numPunkts_ = 1;

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	uint8_t poz = 100;
	poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
	snprintf_P(&vLCDbuf[poz], 11, fcTest1K400[sParam.test.getCurrentSignal()]);

	// ����� �� ����� ��� ��������������� ������
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_2)
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** ���� ���������.
 * 	@param ���
 * 	@return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterValue() {
	static char enterList[] PROGMEM = "����: %S";
	static char enterInt[] PROGMEM = "����: %01u";
	static char enterUcor[] PROGMEM = "����: %01u.%01u";

	eMENU_ENTER_PARAM status = enterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (enterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = { " �������� ��������  ",
					"  ����� ������ �    ", "  ������ �������    " };

			enterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else if (status == MENU_ENTER_PARAM_INT) {
		uint16_t val = enterParam.getValue();
		uint8_t num = enterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterInt, val);
		}
	} else if (status == MENU_ENTER_PARAM_U_COR) {
		uint16_t val = enterParam.getValue();
		uint8_t num = enterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterUcor, val / 10, val % 10);
		}
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = enterParam.getValue();
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				enterParam.list + STRING_LENGHT * val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = enterParam.listValue[enterParam.getValue()];
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				enterParam.list + STRING_LENGHT * val);
	} else
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		enterParam.setDisable();
		break;

	case KEY_UP:
		enterParam.incValue();
		break;
	case KEY_DOWN:
		enterParam.decValue();
		break;

	case KEY_ENTER:
		enterParam.setEnterValueReady();
		break;
	default:
		break;
	}

	key_ = KEY_NO;
	return enterParam.getStatus();
}

/** ���� ������
 * 	@param ���
 * 	@return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterPassword() {
	eMENU_ENTER_PARAM status = enterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (enterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					"       ������       ",//
					"    ������������    ",		//
					"       ������       " };

			enterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else {

		uint8_t poz = 100;
		clearLine(NUM_TEXT_LINES);

		uint16_t val = enterParam.getValue();

		if (status == MENU_ENTER_PASSWORD) {
			static char enter[] PROGMEM = "������: %04u";
			snprintf_P(&vLCDbuf[poz], 21, enter, val);
		} else if (status == MENU_ENTER_PASSWORD_NEW) {
			static char enterNew[] PROGMEM = "����� ������: %04u";
			snprintf_P(&vLCDbuf[poz], 21, enterNew, val);
		} else {
			key_ = KEY_CANCEL;
		}
	}

	switch (key_) {
	case KEY_CANCEL:
		enterParam.setDisable();
		break;

	case KEY_ENTER:
		if (status == MENU_ENTER_PASSWORD)
			enterParam.setEnterValueReady(MENU_ENTER_PASSWORD_READY);
		else if (status == MENU_ENTER_PASSWORD_NEW)
			enterParam.setEnterValueReady(MENU_ENTER_PASSWORD_N_READY);
		else
			enterParam.setDisable();
		break;

	case KEY_UP:
		enterParam.setDisc(1000);
		enterParam.incValue();
		break;
	case KEY_RIGHT:
		enterParam.setDisc(100);
		enterParam.incValue();
		break;
	case KEY_DOWN:
		enterParam.setDisc(10);
		enterParam.incValue();
		break;
	case KEY_LEFT:
		enterParam.setDisc(1);
		enterParam.incValue();
		break;

	default:
		break;
	}
	key_ = KEY_NO;

	return enterParam.getStatus();
}

/**	����� �� ����� ������� ������� ���� � �������
 *	@param ���
 *	@return ���
 */
void clMenu::printPunkts() {
	// ����� �� ����� ������� �������� ����
	// ����� ������������� �� ��������� ������� ������,
	// ���� ��������� ������

	// ���-�� ������������ ����� �� ������
	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	// ���� ����� ������� ������ ������, ��� ��������� ����� �� ������
	// �� �������� �� ����� �������� � (������� ����� - ���.�� �����)
	// ����� � ������
	uint8_t cntPunkts = (cursorLine_ > numLines) ? cursorLine_ - numLines : 0;

	for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++) {
		snprintf_P(&vLCDbuf[20 * line], 21, punkt_[cntPunkts], cntPunkts + 1);

		if (++cntPunkts >= numPunkts_)
			break;
	}

	// ��� ��������������, ����� ������� �� �����
	if (cursorEnable_) {
		if (cursorLine_ > numLines)
			vLCDbuf[20 * (NUM_TEXT_LINES - 1) + 2] = '*';
		else {
			vLCDbuf[20 * (cursorLine_ + lineParam_ - 1) + 2] = '*';
		}
	}
}

/**	���������� ���-�� ������ � ����, �.�. ���-�� ������������� �����.
 * 	@param ��� ������
 * 	@return ���-�� ������
 */
uint8_t clMenu::getNumError(uint16_t val) {
	uint_fast8_t tmp = 0;

	while ((val & 0x0001) == 0) {
		tmp++;
		val >>= 1;
	}

	return tmp;
}
