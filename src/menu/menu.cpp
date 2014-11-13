/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "menu.h"
#include "debug.h"
#include "ks0108.h"
#include "flash.h"

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

	// ������� ������ ��� ���
	key_ = KEY_NO;

	// ��� ���������� ��� �� ��������
	setDevice(AVANT_NO);

	// ����� � ��� ��� ���
	connectionBsp_ = false;

	// �������� ������� ����1����� ������ ��� ���� ���������
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
	sParam.def.status.stateText[12] = fcDefSost12;
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
	sParam.prm.status.stateText[12] = fcPrmSost12;
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
	sParam.prd.status.stateText[12] = fcPrdSost12;
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

void clMenu::main(void) {

	static const char fcNoConnectBsp[] PROGMEM = " ��� ����� � ���!!! ";

	// ������� ������� �� ����������������� ���
	static uint8_t cntInitLcd = 0;
	static uint8_t cntBlinkMeas = 0;
	static uint8_t cntBlinkText = 0;
	// ���������� ��������� ����� ������� ����� � ���
	static bool lastConnection = false;

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

	// ��������� ��������, ���� ��������� �����:
	// ���-�� ��������� � �����
	// ���� ������������� (�400�)
	if (!sParam.device) {
		setDevice(sParam.typeDevice);
	}

	// ������� ��� � ����������
	// ���� ������ ����� ������ - ��������� ��������������� ���������
	eKEY tmp = eKEYget(sParam.typeDevice);
	if (tmp != KEY_NO) {
		if (tmp == KEY_EMPTY)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLed(LED_SWITCH);
	}

	// ��������� �������� ������, ���� ����� �� "������"
	if (sParam.glb.status.getRegime() != GB_REGIME_ENABLED) {
		vLCDsetLed(LED_SWITCH);
	}

	// ������� ������ ���������
	if (delay_ < TIME_MESSAGE) {
		delay_++;
	}

	// ����� � ����� ����������� �������� ����
	// ���� ��������� ��� ��� �������� �� ���������
	clearTextBuf();
	(this->*lvlMenu)();
	key_ = KEY_NO;

#ifdef VIEW_DEBUG_PARAM
	// ����� ���������� ����������
	if (this->lvlMenu == &clMenu::lvlStart) {
		snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
		snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
		snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
		snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
		snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
		snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
		snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
	} else {
		snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
	}
#endif

	// ����� ��������� � ������ ���������� ����� � ���
	bool connection = isConnectionBsp();
	if (!connection) {
		if (blink_) {
			// ���� ����� ���, ������������ ������ ���������� ����������
			// ��������� ��������������� �������
			snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
		}
	} else if (!lastConnection) {
		// ���� ����� � ��� ������ ��������������
		// ������ ������ ������� ������ ������
		sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
		sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
	}
	lastConnection = connection;

	// �������������� ������ �������� � ������ ��� ������ �� �����
	vLCDputchar(vLCDbuf, lineParam_);
	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
}

/**	���������� �������� ��� �400.
 * 	@retval True - ������.
 */
bool clMenu::setDeviceK400() {

	sParam.typeDevice = AVANT_K400;

	// ���� � ����� ��������� �� ���� ���������
	measParam[0] = MENU_MEAS_PARAM_TIME; // ���� <-> �����
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
	measParam[1] = MENU_MEAS_PARAM_DATE;
	measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

	if (!sParam.prd.status.isEnable()) {
		// ���� ������ ��������, �� ����� U � I
		// � � ���� �������� ��� ���� ���� �������� U�1, Uk2, U�1, U�2
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			measParam[2] = MENU_MEAS_PARAM_UC1;
			measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC1;
			measParam[3] = MENU_MEAS_PARAM_UC2;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
			measParam[4] = MENU_MEAS_PARAM_UN1;
			measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN1;
			measParam[5] = MENU_MEAS_PARAM_UN2;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
		} else {
			measParam[3] = MENU_MEAS_PARAM_UC;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
			measParam[5] = MENU_MEAS_PARAM_UN;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;
		}
	} else if (!sParam.prm.status.isEnable()) {
		// ���� ������ ����������, �� ����� Uk/U�
		measParam[2] = MENU_MEAS_PARAM_UOUT;
		measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
		measParam[4] = MENU_MEAS_PARAM_IOUT;
		measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
	} else {
		// ���� ���� �������� � ���������� ������� U,I,Uk,U�
		// � 3-� �������� ��� ���� ��������� U�1/2, U�1/2
		measParam[2] = MENU_MEAS_PARAM_UOUT;
		measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
		measParam[4] = MENU_MEAS_PARAM_IOUT;
		measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			measParam[3] = MENU_MEAS_PARAM_UC1;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
			measParam[5] = MENU_MEAS_PARAM_UN1;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
		} else {
			measParam[3] = MENU_MEAS_PARAM_UC;
			measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
			measParam[5] = MENU_MEAS_PARAM_UN;
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;

		}
	}

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

	// ��������
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
	sParam.prm.status.warningText[1] = fcPrmWarning02k400;
	sParam.prm.status.warningText[2] = fcPrmWarning04k400;
	// 3-15 ���

	// ����������
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
	// 0 ���
	sParam.prd.status.warningText[1] = fcPrdWarning02k400;
	sParam.prd.status.warningText[2] = fcPrdWarning04k400;
	// 3-15 ���

	return true;
}

/**	���������� �������� ��� ����.
 * 	@retval True - ������.
 */
bool clMenu::setDeviceRZSK() {

	sParam.typeDevice = AVANT_RZSK;

	// ������ ������� ����������
	measParam[0] = MENU_MEAS_PARAM_TIME;	// ���� <-> �����
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
	measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
	measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;

	// ������ ������� ����������
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		measParam[1] = MENU_MEAS_PARAM_UZ1;
		measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ2;
		measParam[3] = MENU_MEAS_PARAM_UC1;
		measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
		measParam[5] = MENU_MEAS_PARAM_UN1;
		measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
	} else {
		measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
		measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
		measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;
	}

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

	// ������
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

	// ��������
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
	sParam.prm.status.warningText[1] = fcPrmWarning02k400;
	sParam.prm.status.warningText[2] = fcPrmWarning04k400;
	// 3-15 ���

	// ����������
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
	// 0 ���
	sParam.prd.status.warningText[1] = fcPrdWarning02k400;
	sParam.prd.status.warningText[2] = fcPrdWarning04k400;
	// 3-15 ���

	return true;
}

/**	���������� �������� ��� �400�.
 * 	@retval True - ������.
 */
bool clMenu::setDeviceR400M() {

	sParam.typeDevice = AVANT_R400M;

	// ������ ������� ����������
	measParam[0] = MENU_MEAS_PARAM_TIME;	// ���� <-> �����
	measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
	measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
	measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;

	// ������ ������� ����������
	measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
	if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
		measParam[3] = MENU_MEAS_PARAM_UC1;
		measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
	} else {
		measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
	}
	measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_SD;

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

	// ������
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

	return true;
}

/**	���������� �������� ��� ������.
 * 	@retval True - ������.
 */
bool clMenu::setDeviceOPTO() {

	sParam.typeDevice = AVANT_OPTO;

	measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
	measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

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
	sParam.glb.status.warningText[1] = fcGlbWarning02;
	// 2-3 ���
	sParam.glb.status.warningText[4] = fcGlbWarning10;
	sParam.glb.status.warningText[5] = fcGlbWarning20;
	sParam.glb.status.warningText[6] = fcGlbWarning40;
	// 7-15 ���

	// ������
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

	// ��������
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
	// ���������� ������� �������������� ������
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsko;
	// 1-15 ���

	// ����������
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

	return true;
}

bool clMenu::setDevice(eGB_TYPE_DEVICE device) {
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
					uint16_t vers = sParam.glb.getVersProgIC(GB_IC_BSP_MCU);
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
		} else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO) {
			device = AVANT_OPTO;
		}

		// ���� ������� ���������� ��������� � �����, �� ������ �� ������
		// ����� ������� �� ��������� �������
		if (device == sParam.typeDevice) {
			status = true;
		} else {
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
			status = setDeviceK400();
		} else if (device == AVANT_RZSK) {
			status = setDeviceRZSK();
		} else if (device == AVANT_R400M) {
			status = setDeviceR400M();
		} else if (device == AVANT_OPTO) {
			status = setDeviceOPTO();
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
	// ���������� �������� ������ ����
	lvlCreate_ = true;

	return status;
}

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

/** ������� "��������� ��� ��������"
 * 	���������� �� ����� ��������� ������� "�������������". �� ���� ��� ��������
 * 	�� ����� ��������� n-�� ���-�� �������, �� ������� �������� ��
 * 	"��� �������� �� ���������!!!"
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlError() {
	static uint8_t time = 0;

	static const char fcNoTypeDevice0[] PROGMEM = "    ��� ��������    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   �� ���������!!!  ";
	static const char fcNoTypeDevice3[] PROGMEM = "    �������������   ";

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

	// �������� ������� ���������� � ����������� ��������� ���� ��������
	if (time >= 25) {
		snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
		snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);
	} else {
		snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice3);
	}
	time++;

	if (sParam.typeDevice != AVANT_NO) {
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		time = 0;
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
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();
		// ����� 1
		// �������������� �������
		// ����� ���������� ��������� (� ��)
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

		// ����� 2
		// �������������
		// � �400� + �� + ���-�� ��������� � �����
		sParam.txComBuf.addCom1(GB_COM_GET_FAULT);
		if (sParam.typeDevice == AVANT_R400M)
			sParam.txComBuf.addCom1(GB_COM_DEF_GET_TYPE_AC);
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
	}

	// ����� �� ����� ���������� ����������
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (blinkMeasParam_)
			printMeasParam(i, measParam[i]);
		else
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
	}

	uint16_t val = sParam.glb.status.getWarnings();

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;

		// � �400� ��������� �� � ����� �� ��
		if (sParam.typeDevice == AVANT_R400M) {
			uint16_t time = sParam.def.getTimeToAC();
			eGB_TYPE_AC ac = sParam.def.getTypeAC();

			uint8_t t = poz + 20;
			t += snprintf_P(&vLCDbuf[t], 11,
					fcAcType[static_cast<uint8_t>(ac)]);

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
//		poz += 20;
	}

	switch (key_) {
	case KEY_MENU:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_CALL:
		sParam.txComBuf.setInt8(GB_CONTROL_CALL);
		sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		break;

	case KEY_PUSK_UD:
		if (sParam.def.status.isEnable()) {
			if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			}
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_AC_PUSK_UD:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_PUSK_NALAD:
		if (sParam.def.status.isEnable()) {
			if (sParam.def.status.getState() == 7) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			} else {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
				sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
			}
		}
		break;

	case KEY_AC_RESET:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_AC_PUSK:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		}
		break;

	case KEY_AC_REGIME:
		if (sParam.def.status.isEnable()) {
			sParam.txComBuf.setInt8(GB_CONTROL_REG_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
		break;

	case KEY_RESET_IND:
		if (sParam.prd.status.isEnable() || sParam.prm.status.isEnable()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND);
		}
		break;

	case KEY_PUSK:
		if (sParam.prm.status.isEnable()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER);
		}
		break;

	case KEY_RESET:
		sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
		sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		break;

	default:
		break;
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
	static char punkt4[] PROGMEM = "%d. �����";
	static char punkt5[] PROGMEM = "%d. ����������";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		lineParam_ = 1;
		cursorEnable_ = true;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);

		// ������������� �������
		sParam.txComBuf.clear();
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
			lvlMenu = &clMenu::lvlTest;
			lvlCreate_ = true;
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

	case KEY_MENU:
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
	static char title[]  PROGMEM = "����\\����������";
	static char versProg[] 	PROGMEM = "%S: %02X.%02X";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;
		cursorLine_ = 0;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(GB_IC_BSP_MCU);
		if (sParam.typeDevice != AVANT_OPTO) {
			Punkts_.add(GB_IC_BSP_DSP);				// ������ � ������ ��� DSP
		}
		Punkts_.add(GB_IC_PI_MCU);
		if (sParam.prd.status.isEnable()) {
			Punkts_.add(GB_IC_BSK_PLIS_PRD1);
			if (sParam.prd.getNumCom() > 16) {
				Punkts_.add(GB_IC_BSK_PLIS_PRD2);
			}
		}
		if (sParam.prm.status.isEnable()) {
			Punkts_.add(GB_IC_BSK_PLIS_PRM1);
			if (sParam.prm.getNumCom() > 16) {
				Punkts_.add(GB_IC_BSK_PLIS_PRM2);
			}
		}
		if (sParam.def.status.isEnable()) {
			Punkts_.add(GB_IC_BSZ_PLIS);
		}

		// ������������� �������
		// ����������� ������ �������� (�� ������ ������������)
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_VERS);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	uint8_t cntPunkts = cursorLine_;
	for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++) {
		uint8_t  ic = Punkts_.getNumber(cntPunkts);
		uint16_t vers = sParam.glb.getVersProgIC((eGB_IC) ic);
		snprintf_P(	&vLCDbuf[20*line], 21, versProg,
				fcIC[ic],
				(uint8_t) (vers >> 8),
				(uint8_t) vers);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts()) {
				break;
		}
	}

	switch (key_) {
	case KEY_UP:
		if (cursorLine_ > 0) {
			cursorLine_--;
		}
		break;
	case KEY_DOWN:
		if ((cursorLine_ + numLines) < Punkts_.getMaxNumPunkts()) {
			cursorLine_++;
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlFirst;
		lvlCreate_ = true;
		break;

	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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

		sParam.txComBuf.clear();

		// ��������� ����������� ������� ���� � ��������������� �� ������
		Punkts_.clear();
		Punkts_.add(punkt1);

		if (sParam.def.status.isEnable()) {
			Punkts_.add(punkt2);
		}

		if (sParam.prm.status.isEnable()) {
			Punkts_.add(punkt3);
		}

		if (sParam.prd.status.isEnable()) {
			Punkts_.add(punkt4);
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlJournalEvent;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlJournalDef;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlJournalPrm;
			lvlCreate_ = true;
		} else if (name == punkt4) {
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
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_EVENT_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = GLB_JRN_EVENT_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_EVENT_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_EVENT_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;

	// ����� ����� ������� ������ � �� ���-��
	if (device == AVANT_OPTO) {
		// � ������ ������������� ��������� ���-�� ������� � ����� ������
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}
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
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;

		// ����� �������
		// � ������ � ����� ������ ����� ���� ����� �������, �������
		// ����������� ��� ������� � ������
		uint8_t event = 0;
		if (device == AVANT_OPTO) {
			// �������� �������� ������ ������� � ���-��� ������� � ������
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			event = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			event = sParam.jrnEntry.getEventType();
		}

		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_R400M) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventR400_MSK[event], event);
		} else if (device == AVANT_K400) {
			if (event <= MAX_JRN_EVENT_VALUE) {
				uint8_t device = (uint8_t) sParam.jrnEntry.getDeviceJrn();
				snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event],
						fcDevicesK400[device]);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], event);
			}
		} else if (device == AVANT_RZSK) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventRZSK[event], event);
		} else if (device == AVANT_OPTO) {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTO[event], event);
		}

	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_R400M) {
			t = GLB_JRN_DEF_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_DEF_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_DEF_OPTO_MAX;
		}
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
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		//	    snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());

		poz += 20;
//		uint8_t signals = sParam.jrnEntry.getSignalDef();
//		snprintf(&vLCDbuf[poz], 21, "%x",signals);
		snprintf_P(&vLCDbuf[poz], 21, fcSignalDefJrn,
				sParam.jrnEntry.getSignalPusk(),
				sParam.jrnEntry.getSignalStop(), sParam.jrnEntry.getSignalMan(),
				sParam.jrnEntry.getSignalPrd(), sParam.jrnEntry.getSignalPrm(),
				sParam.jrnEntry.getSignalOut());
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRM_K400_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRM_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRM_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;
	// ����� ����� ������� ������ � �� ���-��
	if (device == AVANT_OPTO) {
		// � ������ ������������� ��������� ���-�� ������� � ����� ������
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {
		// ����� ������ �������
		uint8_t com = 0;
		if (device == AVANT_OPTO) {
			// � ������ � ������ ������ ������ ��� �������� �� ���� �������
			// ���� 1 - ����������, 0 - ���.
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			com = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			com = sParam.jrnEntry.getNumCom();
		}
		snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);
		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;
		// ����� �������
		if (device == AVANT_OPTO) {
			// � ������ ���� ���� ������ - �� ��� ������� �������
			// ����� - ������ �� �������� ���
			if (com != 0) {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComYes);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
			}
		} else {
			uint8_t event = sParam.jrnEntry.getEventType();
			snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
		}
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� �������� ������� � ������������� ���-�� ������� � ���
		sParam.jrnEntry.clear();
		sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRD_K400_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRD_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRD_OPTO_MAX;
		}
		sParam.jrnEntry.setMaxNumJrnEntries(t);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_ENTRY);
		sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
	}

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;

	// ����� ����� ������� ������ � �� ���-��
	if (device == AVANT_OPTO) {
		// � ������ ������������� ��������� ���-�� ������� � ����� ������
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
				num_entries, sParam.jrnEntry.getNumOpticsEntries());
	} else {
		snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
	}

	poz += 20;
	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.jrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {

		// ����� ������ �������
		uint8_t com = 0;
		if (device == AVANT_OPTO) {
			// � ������ � ����� ������ ����� ���� ����� ������
			if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
				curCom_ = 1;
			com = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			com = sParam.jrnEntry.getNumCom();
		}
		uint8_t t = snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);

		// ��� ���������� ��-����������, ������� �������� ������������ �������
		if ((device == AVANT_K400) || (device == AVANT_RZSK)) {
			uint8_t s = sParam.jrnEntry.getSourceCom();
			snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
		}

		poz += 20;
		// ����� ����
		snprintf_P(&vLCDbuf[poz], 21, fcDateJrn,
				sParam.jrnEntry.dateTime.getDay(),
				sParam.jrnEntry.dateTime.getMonth(),
				sParam.jrnEntry.dateTime.getYear());
		poz += 20;
		// ����� �������
		snprintf_P(&vLCDbuf[poz], 21, fcTimeJrn,
				sParam.jrnEntry.dateTime.getHour(),
				sParam.jrnEntry.dateTime.getMinute(),
				sParam.jrnEntry.dateTime.getSecond(),
				sParam.jrnEntry.dateTime.getMsSecond());
		poz += 20;
		// ����� �������
		if (device == AVANT_OPTO) {
			// � ������ ���� ���� ������ - �� ��� ������� �������
			// ����� - ������ �� �������� ���
			if (com != 0) {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComYes);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
			}
		} else {
			uint8_t event = sParam.jrnEntry.getEventType();
			snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
		}
	}

	switch (key_) {
	case KEY_UP:
		if (sParam.jrnEntry.setPreviousEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;
	case KEY_DOWN:
		if (sParam.jrnEntry.setNextEntry()) {
			sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
			curCom_ = 1;
		}
		break;

	case KEY_LEFT:
		if (device == AVANT_OPTO) {
			if (curCom_ > 1) {
				curCom_--;
			} else {
				curCom_ = sParam.jrnEntry.getNumOpticsEntries();
			}
		}
		break;
	case KEY_RIGHT:
		if (device == AVANT_OPTO) {
			if (curCom_ < sParam.jrnEntry.getNumOpticsEntries()) {
				curCom_++;
			} else {
				curCom_ = 1;
			}
		}
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlJournal;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
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
//	static char punkt15[] PROGMEM = "%d. �� ������������.";
	static char punkt16[] PROGMEM = "%d. �� ����������";
	static char punkt17[] PROGMEM = "%d. �� ��������";
	static char punkt18[] PROGMEM = "%d. �� ���������";
	static char punkt19[] PROGMEM = "%d. �� ����������";
	static char punkt20[] PROGMEM = "%d. �� ������";
//	static char punkt21[] PROGMEM = "%d. �� �������������";
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

		Punkts_.clear();
		if (device == AVANT_R400M) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			eGB_COMPATIBILITY compatibility = sParam.glb.getCompatibility();
			// ������ ������ ���� ���� ����������
			Punkts_.add(punkt07);	// ����� ���������� � ����������� �� ��������
			// ����� ������ ���� �� ���� ��������� � ��������������
			Punkts_.add(punkt03);
			if (compatibility == GB_COMPATIBILITY_AVANT) {
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt04);
					Punkts_.add(punkt02);
					Punkts_.add(punkt11);
					Punkts_.add(punkt12);
					Punkts_.add(punkt13);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt17);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt22);
					Punkts_.add(punkt23);
					Punkts_.add(punkt24);
					Punkts_.add(punkt26);
					Punkts_.add(punkt11);
					Punkts_.add(punkt12);
					Punkts_.add(punkt13);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt17);
				}
			} else if (compatibility == GB_COMPATIBILITY_PVZ90) {
				Punkts_.add(punkt04);
				Punkts_.add(punkt19);
				Punkts_.add(punkt16);
				Punkts_.add(punkt17);
				Punkts_.add(punkt18);
				Punkts_.add(punkt08);
			} else if (compatibility == GB_COMPATIBILITY_PVZUE) {
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt02);
					Punkts_.add(punkt09);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt23);
					Punkts_.add(punkt24);
					Punkts_.add(punkt26);
					Punkts_.add(punkt27);
					Punkts_.add(punkt28);
					Punkts_.add(punkt30);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				}
			} else if (compatibility == GB_COMPATIBILITY_AVZK80) {
				Punkts_.add(punkt19);
				Punkts_.add(punkt16);
				Punkts_.add(punkt17);
				Punkts_.add(punkt18);
				Punkts_.add(punkt08);
			} else if (compatibility == GB_COMPATIBILITY_PVZL) {
				Punkts_.add(punkt11);
				Punkts_.add(punkt12);
				Punkts_.add(punkt13);
				Punkts_.add(punkt14);
				Punkts_.add(punkt31);
				Punkts_.add(punkt17);
			}
			// ����� ���� �� ���� ��������������
			Punkts_.add(punkt05);
		} else if (device == AVANT_RZSK) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			if (numDevices == GB_NUM_DEVICES_2) {
				Punkts_.add(punkt07);	// ����� ���������� � ����������� �� ��������
				Punkts_.add(punkt03);
				Punkts_.add(punkt04);
				Punkts_.add(punkt02);
				Punkts_.add(punkt05);
			} else if (numDevices == GB_NUM_DEVICES_3) {
				Punkts_.add(punkt07);	// ����� ���������� � ����������� �� ��������
				Punkts_.add(punkt03);
				Punkts_.add(punkt22);
				Punkts_.add(punkt23);	// ����� ���������� � ����������� �� ������
				Punkts_.add(punkt24);	// ����� ���������� � ����������� �� ������
				Punkts_.add(punkt26);
				Punkts_.add(punkt05);
			}
		} else if (device == AVANT_K400) {
			Punkts_.add(punkt03);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt03);
			Punkts_.add(punkt04);
		}

		// ������������� �������
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_R400M) {
			// �������������
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
			// ���-�� ��������� � �����
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// ����� �������� ��������
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		} else if (sParam.typeDevice == AVANT_RZSK) {
			// ���-�� ��������� � �����
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			// ����� �������� ��������
			sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	if (sParam.def.status.isEnable()) {
		// ����� ���./����. ����������� �����
		if (sParam.def.status.getState() != 7)
			Punkts_.change(punkt06, GB_COM_NO, 0);
		else
			Punkts_.change(punkt07, GB_COM_NO, 0);

		// ����� ���� ����������
		if (device == AVANT_RZSK) {
			if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
				uint8_t num = sParam.glb.getDeviceNum();
				if (num == 1) {
					Punkts_.change(punkt24, GB_COM_NO, 3);
					Punkts_.change(punkt25, GB_COM_NO, 4);
				} else if (num == 2) {
					Punkts_.change(punkt23, GB_COM_NO, 3);
					Punkts_.change(punkt25, GB_COM_NO, 4);
				} else if (num == 3) {
					Punkts_.change(punkt23, GB_COM_NO, 3);
					Punkts_.change(punkt24, GB_COM_NO, 4);
				}
			}
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER: {
		if (name == punkt02) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt03) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt04) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt05) {
			sParam.txComBuf.setInt8(GB_CONTROL_CALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt06) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt07) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt09) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt10) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt11) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt12) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt13) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt14) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		}
//		else if (p == punkt15)
//		{
//			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_FAST);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		}
		else if (name == punkt16) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_FAST);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt17) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_OFF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt18) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt19) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt20) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
//		} else if (p == punkt21) {
//			sParam.txComBuf.setInt8(GB_TYPE_AC_CHECK);
//			sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC);
		} else if (name == punkt22) {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_UD);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt23) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt24) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt25) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt26) {
			sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt27) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_1);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt28) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_2);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt29) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_3);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt30) {
			sParam.txComBuf.setInt8(GB_CONTROL_MAN_ALL);
			sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL);
		} else if (name == punkt31) {
			sParam.txComBuf.setInt8(GB_TYPE_AC_AUTO_NORM);
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
	static char punkt5[] PROGMEM = "%d. ���������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();
	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();

		// ����� ������� ����� : ������ ��� ��������
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PASSWORD_READY) {
			uint16_t val = EnterParam.getValue();

			if (sParam.password.check(val)) {
				EnterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
			} else
				EnterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_N_READY) {
			uint16_t val = EnterParam.getValue();

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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER: {
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlRegime;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlSetupDT;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
		} else if (name == punkt4) {
			enterFunc = &clMenu::enterPassword;
			EnterParam.setEnable(MENU_ENTER_PASSWORD);
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt5) {
			lvlMenu = &clMenu::lvlSetupInterface;
			lvlCreate_ = true;
		}
	}
		break;

	default:
		break;
	}
}

void clMenu::lvlRegime() {
	static char title[] PROGMEM = "���������\\�����";
	eGB_REGIME reg = sParam.glb.status.getRegime();

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
//		poz += 20;
	}

	// ���� ������ �������� ���������.
	// ������� ���������� ��������� ����� ������.
	// ��� ������� ������� � ����� "�������" �� "������" ��� "�����",
	// ���������� ������ ������. ��� ��������� ������ ��������� ���������.
	if (EnterParam.isEnable()) {
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();
		eGB_REGIME_ENTER val = GB_REGIME_ENTER_MAX;

		// ����� ������� ����� : ������ ��� ��������
		(this->*enterFunc)();

		if (stat == MENU_ENTER_PARAM_READY) {
			// �������� ������, ���� �������� ������� � ����� "�������"
			// �� ������� "������" � "�����"
			val = (eGB_REGIME_ENTER) EnterParam.getValueEnter();

			if ((reg == GB_REGIME_ENABLED) || (reg == GB_REGIME_READY)) {
				if (val == GB_REGIME_ENTER_DISABLED) {
					enterFunc = &clMenu::enterPassword;
					EnterParam.setEnable(MENU_ENTER_PASSWORD);
					EnterParam.setDopValue(static_cast<uint16_t>(val));
					val = GB_REGIME_ENTER_MAX;
				}
			}
		}

		if (stat == MENU_ENTER_PASSWORD_READY) {
			// �������� ��������� ������
			if (sParam.password.check(EnterParam.getValue())) {
				val = static_cast<eGB_REGIME_ENTER>(EnterParam.getDopValue());
			} else {
				EnterParam.printMessage();
			}
		}

		if (val != GB_REGIME_ENTER_MAX) {
			eGB_COM com = GB_COM_NO;

			if (val == GB_REGIME_ENTER_DISABLED)
				com = GB_COM_SET_REG_DISABLED;
			else if (val == GB_REGIME_ENTER_ENABLED)
				com = GB_COM_SET_REG_ENABLED;
			sParam.txComBuf.addFastCom(com);
			EnterParam.setDisable();
		}
	}

	switch (key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER: {
			uint8_t min = GB_REGIME_ENTER_DISABLED;
			uint8_t max = GB_REGIME_ENTER_DISABLED;
			uint8_t val = GB_REGIME_ENTER_DISABLED;

			// "������" 	-> "�������"
			// "�������" 	-> "������"
			// "�����" 		-> "������"	 / "�������"
			// ��������� 	-> "�������" / "������"
			enterFunc = &clMenu::enterValue;
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			switch(reg) {
				case GB_REGIME_ENABLED:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_DISABLED;
					val = GB_REGIME_ENTER_DISABLED;
					break;
				case GB_REGIME_ENTER_DISABLED:
					min = GB_REGIME_ENTER_ENABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_ENABLED;
					break;
				case GB_REGIME_READY:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_ENABLED;
					break;
				default:
					min = GB_REGIME_ENTER_DISABLED;
					max = GB_REGIME_ENTER_ENABLED;
					val = GB_REGIME_ENTER_DISABLED;
					break;
			}
			EnterParam.setValueRange(min, max);
			EnterParam.setValue(val);
			EnterParam.list = fcRegimeEnter[0];
			EnterParam.com = GB_COM_NO;
		}
		break;

		default: {
		}
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

		// ��������� ����, � ����������� �� �������� ����������
		Punkts_.clear();
		if (sParam.def.status.isEnable()) {
			Punkts_.add(punkt1);
		}
		if (sParam.prm.status.isEnable()) {
			Punkts_.add(punkt2);
		}
		if (sParam.prd.status.isEnable()) {
			Punkts_.add(punkt3);
		}
		Punkts_.add(punkt4);

		// ������������� �������
		sParam.txComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			lvlMenu = &clMenu::lvlSetupParamDef;
			lvlCreate_ = true;
		} else if (name == punkt2) {
			lvlMenu = &clMenu::lvlSetupParamPrm;
			lvlCreate_ = true;
		} else if (name == punkt3) {
			lvlMenu = &clMenu::lvlSetupParamPrd;
			lvlCreate_ = true;
		} else if (name == punkt4) {
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
	//								 12345678901234567890
	static char title[]   PROGMEM = "���������\\������";
	static char punkt1[]  PROGMEM = "��� ������";
	static char punkt2[]  PROGMEM = "��� �����";
	static char punkt3[]  PROGMEM = "���. ����� ��� ���.";
	static char punkt4[]  PROGMEM = "���������� ���������";
	static char punkt5[]  PROGMEM = "����������� ��������";
	static char punkt6[]  PROGMEM = "����� �������. �� ��";
	static char punkt7[]  PROGMEM = "����������� ��������";
	static char punkt8[]  PROGMEM = "��� ���������";
	static char punkt9[]  PROGMEM = "�������� ������ ��";
	static char punkt10[] PROGMEM = "������� ���";
	static char punkt11[] PROGMEM = "������� ���";
	static char punkt12[] PROGMEM = "�������. ������. ��";	// ==punkt7 ��� ����

	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		sParam.txComBuf.clear();
		Punkts_.clear();

		// ��� ���������������� ���� ����������� ������� ������:
		// ���-�� ��������� � �����
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);

		if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_DEF_GET_DEF_TYPE);
			Punkts_.add(punkt2, GB_COM_DEF_GET_LINE_TYPE);
			Punkts_.add(punkt3, GB_COM_DEF_GET_T_NO_MAN);
			Punkts_.add(punkt4, GB_COM_DEF_GET_OVERLAP);
			Punkts_.add(punkt5, GB_COM_DEF_GET_DELAY);
			Punkts_.add(punkt6, GB_COM_DEF_GET_RZ_THRESH);
			Punkts_.add(punkt12, GB_COM_DEF_GET_RZ_DEC);
			Punkts_.add(punkt8, GB_COM_DEF_GET_PRM_TYPE);
		} else if (device == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();
			// ��� ���������������� ���� ����������� ������� ������:
			// �������������
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			Punkts_.add(punkt1, GB_COM_DEF_GET_DEF_TYPE);
			Punkts_.add(punkt2, GB_COM_DEF_GET_LINE_TYPE);
			Punkts_.add(punkt3, GB_COM_DEF_GET_T_NO_MAN);
			Punkts_.add(punkt4, GB_COM_DEF_GET_OVERLAP);
			Punkts_.add(punkt5, GB_COM_DEF_GET_DELAY);
			Punkts_.add(punkt7, GB_COM_DEF_GET_RZ_DEC);
			if (comp == GB_COMPATIBILITY_AVANT) {
				// �������� ������ �� ���� ������ � ������������� �����
				Punkts_.add(punkt9, GB_COM_DEF_GET_PRM_TYPE);
			}
			Punkts_.add(punkt10, GB_COM_DEF_GET_FREQ_PRD);
			Punkts_.add(punkt11, GB_COM_DEF_GET_RZ_THRESH);
		}
	}

	// ������� �������, �� ������� �������� ������ ����.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	// ����� �������� ��������� � �� ����������
	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	// �������� ���������
	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ����������� ���.������, ��� ���������� ����������
	poz = 60;
	if (device == AVANT_R400M) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt5) {
				if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT) {
					snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_, 2);
				}
			}
		}
	} else if (device == AVANT_RZSK) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if ((name == punkt5) || (name == punkt12)) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_, 2);
			}
		}
	}

	//  �������� �������� ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_T_NO_MAN_MIN,
				DEF_T_NO_MAN_MAX, "���");
	} else if (name == punkt4) {
		uint8_t min = (sParam.typeDevice == AVANT_R400M) ?
				DEF_OVERLAP_MIN2 : DEF_OVERLAP_MIN;
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, min, DEF_OVERLAP_MAX, "����");
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_DELAY_MIN, DEF_DELAY_MAX,
				"����");
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_THRESH_MIN,
				DEF_RZ_THRESH_MAX, "��");
	} else if ((name == punkt7) || (name == punkt12)) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, DEF_RZ_DEC_MIN,
				DEF_RZ_DEC_MAX, "��");
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���
			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt2) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt4) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt5) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt7) || (name == punkt12)) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt9) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt10) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt11) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}

			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {

		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = sParam.def.getDefType();
			snprintf_P(&vLCDbuf[poz], 11, fcDefType[val]);
		} else if (name == punkt2) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		} else if (name == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%d���", sParam.def.getTimeNoMan());
		} else if (name == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%d����", sParam.def.getOverlap());
		} else if (name == punkt5) {
			uint8_t val = sParam.def.getDelay(curCom_);
			snprintf(&vLCDbuf[poz], 11, "%d����", val);
		} else if (name == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.def.getRzThreshold());
		} else if ((name == punkt7) || (name == punkt12)) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.def.getRzDec(curCom_));
		} else if (name == punkt8) {
			snprintf_P(&vLCDbuf[poz], 11, fcPrmType[sParam.def.getPrmType()]);
		} else if (name == punkt9) {
			uint8_t val = sParam.def.getAcDec() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt10) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrd()]);
		} else if (name == punkt11) {
			snprintf_P(&vLCDbuf[poz], 11, fcPvzlFreq[sParam.def.getFreqPrm()]);
		}
	}

	switch (key_) {
	case KEY_UP:
		curCom_ = 1;
		cursorLineUp();
		break;
	case KEY_DOWN:
		curCom_ = 1;
		cursorLineDown();
		break;
	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.glb.getNumDevices() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.glb.getNumDevices() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.def.status.getRegime() != GB_REGIME_DISABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			EnterParam.printMessage();
		} else {

			if (name == punkt1) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(DEF_TYPE_MIN, DEF_TYPE_MAX - 1);
				EnterParam.setValue(sParam.def.getDefType());
				EnterParam.list = fcDefType[0];
				EnterParam.com = GB_COM_DEF_SET_DEF_TYPE;
			} else if (name == punkt2) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_NUM_DEVICES_MIN,
						GB_NUM_DEVICES_MAX - 1);
				EnterParam.setValue(sParam.def.getNumDevices());
				EnterParam.list = fcNumDevices[0];
				EnterParam.com = GB_COM_DEF_SET_LINE_TYPE;
			} else if (name == punkt3) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_T_NO_MAN_MIN, DEF_T_NO_MAN_MAX);
				EnterParam.setValue(sParam.def.getTimeNoMan());
				EnterParam.setDisc(DEF_T_NO_MAN_DISC);
				EnterParam.setFract(DEF_T_NO_MAN_FRACT);
				EnterParam.com = GB_COM_DEF_SET_T_NO_MAN;
			} else if (name == punkt4) {
				EnterParam.setEnable();
				if (sParam.typeDevice == AVANT_R400M) {
					EnterParam.setValueRange(DEF_OVERLAP_MIN2, DEF_OVERLAP_MAX);
				} else {
					EnterParam.setValueRange(DEF_OVERLAP_MIN, DEF_OVERLAP_MAX);
				}
				EnterParam.setValue(sParam.def.getOverlap());
				EnterParam.setDisc(DEF_OVERLAP_DISC);
				EnterParam.setFract(DEF_OVERLAP_FRACT);
				EnterParam.com = GB_COM_DEF_SET_OVERLAP;
			} else if (name == punkt5) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_DELAY_MIN, DEF_DELAY_MAX);
				EnterParam.setValue(sParam.def.getDelay(curCom_));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(DEF_DELAY_DISC);
				EnterParam.setFract(DEF_DELAY_FRACT);
				EnterParam.com = GB_COM_DEF_SET_DELAY;
			} else if (name == punkt6) {
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_RZ_THRESH_MIN, DEF_RZ_THRESH_MAX);
				EnterParam.setValue(sParam.def.getRzDec(curCom_));
				EnterParam.setDisc( DEF_RZ_THRESH_DISC);
				EnterParam.setFract(DEF_RZ_THRESH_FRACT);
				EnterParam.com = GB_COM_DEF_SET_RZ_THRESH;
			} else if ((name == punkt7) || (name == punkt12)) {
				// TODO �400� � 3-� �������� 2 ������
				EnterParam.setEnable();
				EnterParam.setValueRange(DEF_RZ_DEC_MIN, DEF_RZ_DEC_MAX);
				EnterParam.setValue(sParam.def.getRzDec(curCom_));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(DEF_RZ_DEC_DISC);
				EnterParam.setFract(DEF_RZ_DEC_FRACT);
				EnterParam.com = GB_COM_DEF_SET_RZ_DEC;
			} else if (name == punkt8) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(DEF_PRM_TYPE_MIN, DEF_PRM_TYPE_MAX -1);
				EnterParam.setValue(sParam.def.getPrmType());
				EnterParam.list = fcPrmType[0];
				EnterParam.com = GB_COM_DEF_SET_PRM_TYPE;
			} else if (name == punkt9) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.def.getAcDec() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_DEF_SET_PRM_TYPE;
			} else if (name == punkt10) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_PVZL_FREQ_MIN,
						GB_PVZL_FREQ_MAX - 1);
				EnterParam.setValue(sParam.def.getFreqPrd());
				EnterParam.list = fcPvzlFreq[0];
				EnterParam.com = GB_COM_DEF_SET_FREQ_PRD;
			} else if (name == punkt11) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_PVZL_FREQ_MIN,
						GB_PVZL_FREQ_MAX - 1);
				EnterParam.setValue(sParam.def.getFreqPrm());
				EnterParam.list = fcPvzlFreq[0];
				EnterParam.com = GB_COM_DEF_SET_RZ_THRESH;
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
	static char punkt4[] PROGMEM = "���������� ��";
	static char punkt5[] PROGMEM = "��������. ������� ��";
	static char punkt6[] PROGMEM = "������� �� � ��";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (device == AVANT_K400) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
			Punkts_.add(punkt4, GB_COM_PRM_GET_DR_STATE);
			Punkts_.add(punkt5, GB_COM_PRM_GET_DR_BLOCK);
			Punkts_.add(punkt6, GB_COM_PRM_GET_DR_COM);
		} else if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_PRM_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRM_GET_BLOCK_COM);
			Punkts_.add(punkt3, GB_COM_PRM_GET_TIME_OFF);
		}
	}

	// ������� �������, �� ������� �������� ������ ����.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ��� �������������, ����� ��������� ������� ���������
	poz = 60;
	if ((name == punkt2) || (name == punkt3)
			|| (name == punkt5) || (name == punkt6)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prm.getNumCom());
	}

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_ON_MIN,
				PRM_TIME_ON_MAX, "��");
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_TIME_OFF_MIN,
				PRM_TIME_OFF_MAX, "��");
	} else if (name == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRM_COM_DR_MIN,
				PRM_COM_DR_MAX, "���");
	}

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt2) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt4) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt5) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prm.getBlockComDR8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prm.getTimeOn());
		} else if (name == punkt2) {
			uint8_t val = (sParam.prm.getBlockCom(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%d��",
					sParam.prm.getTimeOff(curCom_ - 1));
		} else if (name == punkt4) {
			uint8_t val = sParam.prm.getStateDR() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt5) {
			uint8_t val = (sParam.prm.getBlockComDR(curCom_ - 1) > 0) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt6) {
			snprintf(&vLCDbuf[poz], 11, "%d���",
					sParam.prm.getComDR(curCom_ - 1));
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			EnterParam.printMessage();
		} else {
			if (name == punkt1) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_TIME_ON_MIN, PRM_TIME_ON_MAX);
				EnterParam.setValue(sParam.prm.getTimeOn());
				EnterParam.setDisc(PRM_TIME_ON_DISC);
				EnterParam.setFract(PRM_TIME_ON_FRACT);
				EnterParam.com = GB_COM_PRM_SET_TIME_ON;
			} else if (name == punkt2) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.setDopValue(curCom_);
				EnterParam.com = GB_COM_PRM_SET_BLOCK_COM;
			} else if (name == punkt3) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_TIME_OFF_MIN, PRM_TIME_OFF_MAX);
				EnterParam.setValue(sParam.prm.getTimeOff(curCom_ - 1));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(PRM_TIME_OFF_DISC);
				EnterParam.setFract(PRM_TIME_OFF_FRACT);
				EnterParam.com = GB_COM_PRM_SET_TIME_OFF;
			} else if (name == punkt4) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getStateDR() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRM_SET_DR_STATE;
			} else if (name == punkt5) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prm.getBlockComDR(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRM_SET_DR_BLOCK;
			} else if (name == punkt6) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRM_COM_DR_MIN, PRM_COM_DR_MAX);
				EnterParam.setValue(sParam.prm.getComDR(curCom_ - 1));
				EnterParam.setDopValue(curCom_);
				EnterParam.setDisc(PRM_COM_DR_DISC);
				EnterParam.setFract(PRM_COM_DR_FRACT);
				EnterParam.com = GB_COM_PRM_SET_DR_COM;
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
	static char title[]  PROGMEM = "���������\\����������";
	static char punkt1[] PROGMEM = "�������� ������. ���";
	static char punkt2[] PROGMEM = "������������ �������";	// ��
	static char punkt2o[] PROGMEM= "������������ �������";	// ������
	static char punkt3[] PROGMEM = "�������� �������";
	static char punkt4[] PROGMEM = "�������� �������";
	static char punkt5[] PROGMEM = "��������. �������";
	static char punkt6[] PROGMEM = "���������� ��";
	static char punkt7[] PROGMEM = "��������. ������� ��";
	static char punkt8[] PROGMEM = "���������� ������ �";


	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (device == AVANT_K400) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt3, GB_COM_PRD_GET_TEST_COM);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
			Punkts_.add(punkt8, GB_COM_PRD_GET_COM_A);
			Punkts_.add(punkt6, GB_COM_PRD_GET_DR_STATE);
			Punkts_.add(punkt7, GB_COM_PRD_GET_DR_BLOCK);
		} else if (device == AVANT_RZSK) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
		} else if (device == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_PRD_GET_TIME_ON);
			Punkts_.add(punkt2o, GB_COM_PRD_GET_DURATION);
			Punkts_.add(punkt4, GB_COM_PRD_GET_LONG_COM);
			Punkts_.add(punkt5, GB_COM_PRD_GET_BLOCK_COM);
		}
	}

	// ������� �������, �� ������� �������� ���������
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ��� ������������� ������� �������� ������� ���������
	poz = 60;
	if ((name == punkt4) || (name == punkt5) || (name == punkt7)) {
		snprintf_P(&vLCDbuf[poz], 21, fcNumCom, curCom_,
				sParam.prd.getNumCom());
	}

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_TIME_ON_MIN,
				PRD_TIME_ON_MAX, "��");
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURAT_L_MIN,
				PRD_DURAT_L_MAX, "��");
	} else if (name == punkt2o) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_DURAT_O_MIN,
				PRD_DURAT_O_MAX, "��");
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, PRD_COM_A_MIN,
				PRD_COM_A_MAX, "");
	}

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���

			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt2) || (name == punkt2o)) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt4) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getLongCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt5) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockCom8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt7) {
				uint8_t pl = EnterParam.getDopValue() - 1;
				uint8_t val = sParam.prd.getBlockComDR8(pl / 8);
				if (EnterParam.getValue() > 0)
					val |= (1 << (pl % 8));
				else
					val &= ~(1 << (pl % 8));
				sParam.txComBuf.setInt8(pl / 8 + 1, 0);
				sParam.txComBuf.setInt8(val, 1);
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prd.getTimeOn());
		} else if (name == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prd.getDurationL());
		} else if (name == punkt2o) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.prd.getDurationO());
		} else if (name == punkt3) {
			uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt4) {
			uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt5) {
			uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt6) {
			uint8_t val = sParam.prd.getStateDR() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt7) {
			uint8_t val = sParam.prd.getBlockComDR(curCom_ - 1) ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt8) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.prd.getNumComA());
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			EnterParam.printMessage();
		} else {

			if (name == punkt1) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_TIME_ON_MIN, PRD_TIME_ON_MAX);
				EnterParam.setValue(sParam.prd.getTimeOn());
				EnterParam.setDisc(PRD_TIME_ON_DISC);
				EnterParam.setFract(PRD_TIME_ON_FRACT);
				EnterParam.com = GB_COM_PRD_SET_TIME_ON;
			} else if (name == punkt2) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_DURAT_L_MIN, PRD_DURAT_L_MAX);
				EnterParam.setValue(sParam.prd.getDurationL());
				EnterParam.setDisc(PRD_DURAT_L_DISC);
				EnterParam.setFract(PRD_DURAT_L_FRACT);
				EnterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (name == punkt2o) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_DURAT_O_MIN, PRD_DURAT_O_MAX);
				EnterParam.setValue(sParam.prd.getDurationO());
				EnterParam.setDisc(PRD_DURAT_O_DISC);
				EnterParam.setFract(PRD_DURAT_O_FRACT);
				EnterParam.com = GB_COM_PRD_SET_DURATION;
			} else if (name == punkt3) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getTestCom() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_TEST_COM;
			} else if (name == punkt4) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getLongCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_LONG_COM;
			} else if (name == punkt5) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockCom(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_BLOCK_COM;
			} else if (name == punkt6) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getStateDR() ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_DR_STATE;
			} else if (name == punkt7) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(0, 1);
				uint8_t val = sParam.prd.getBlockComDR(curCom_ - 1) ? 1 : 0;
				EnterParam.setValue(val);
				EnterParam.setDopValue(curCom_);
				EnterParam.list = fcOnOff[0];
				EnterParam.com = GB_COM_PRD_SET_DR_BLOCK;
			} else if (name == punkt8) {
				EnterParam.setEnable();
				EnterParam.setValueRange(PRD_COM_A_MIN, PRD_COM_A_MAX);
				EnterParam.setValue(sParam.prd.getNumComA());
				EnterParam.setDisc(PRD_COM_A_DISC);
				EnterParam.setFract(PRD_COM_A_FRACT);
				EnterParam.com = GB_COM_PRD_SET_COM_A;
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
	//								   12345678901234567890
	static char punkt1[] PROGMEM 	= "������������� �����";
	static char punkt2[] PROGMEM 	= "����� ��������";
	static char punkt3[] PROGMEM 	= "�������� ���.�������";
	static char punkt4[] PROGMEM 	= "����� ��������������";
	static char punkt4rzsk[] PROGMEM= "����� �������. �� ��";
	static char punkt5[] PROGMEM 	= "����� �����������";
	static char punkt6[] PROGMEM 	= "�����. ���� ���. ���";
	static char punkt7[] PROGMEM 	= "�����. ���� ���. ���";
	static char punkt8[] PROGMEM 	= "�������. ������. ���";
	static char punkt9[] PROGMEM 	= "������� �����";
	static char punkt10[] PROGMEM 	= "U��� �����������";
	static char punkt11[] PROGMEM 	= "�������";
	static char punkt12[] PROGMEM 	= "�������������";			// ������
	static char punkt13[] PROGMEM 	= "�������� ������ ��";
	static char punkt14[] PROGMEM 	= "��� ���������";
	static char punkt15[] PROGMEM 	= "��������� ����������";
	static char punkt16[] PROGMEM 	= "��������� ����";
	static char punkt17[] PROGMEM 	= "�������� ������";
	static char punkt18[] PROGMEM 	= "������� ��������";
	static char punkt19[] PROGMEM 	= "���������� �������";
	static char punkt20[] PROGMEM 	= "����� �� ������";
	static char punkt21[] PROGMEM 	= "���������� ������";
	static char punkt22[] PROGMEM 	= "��� ������������";
	static char punkt23[] PROGMEM 	= "��������������";
	static char punkt24[] PROGMEM 	= "�������������";
	static char punkt25[] PROGMEM 	= "��� �����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE type = sParam.typeDevice;
		sParam.txComBuf.clear();
		Punkts_.clear();
		if (type == AVANT_K400) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt4, GB_COM_GET_CF_THRESHOLD);
				Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
				Punkts_.add(punkt8, GB_COM_GET_CF_THRESHOLD);
			}
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
//			Punkts_.add(punkt24, GB_COM_GET_COM_PRD_KEEP);	// TODO �400 ������� "�������������"
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
				Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
			}
			Punkts_.add(punkt25, GB_COM_DEF_GET_LINE_TYPE);
		} else if (type == AVANT_RZSK) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			Punkts_.add(punkt4rzsk, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
			Punkts_.add(punkt8, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
			Punkts_.add(punkt14, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
			Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
		} else if (type == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();

			// ��� ���������������� ���� ����������� ������� ������:
			// ������������� � ���-�� ��������� � �����
			// ���������� ��������� ��� ���������
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			if (comp == GB_COMPATIBILITY_AVANT) {
				Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			}
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			Punkts_.add(punkt3, GB_COM_GET_OUT_CHECK);
			Punkts_.add(punkt4, GB_COM_GET_CF_THRESHOLD);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt10, GB_COM_GET_COM_PRM_KEEP);
			Punkts_.add(punkt11, GB_COM_GET_FREQ);
			Punkts_.add(punkt12, GB_COM_GET_COM_PRD_KEEP);
			if (comp == GB_COMPATIBILITY_PVZL) {
				Punkts_.add(punkt13, GB_COM_GET_TIME_RERUN);
			}
			Punkts_.add(punkt15, GB_COM_GET_COR_U_I);
			Punkts_.add(punkt16, GB_COM_GET_COR_U_I);
			if (comp == GB_COMPATIBILITY_PVZUE) {
				Punkts_.add(punkt17, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt18, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt19, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt20, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt21, GB_COM_GET_TIME_RERUN);
				Punkts_.add(punkt22, GB_COM_GET_TIME_RERUN);
			}
		} else if (type == AVANT_OPTO) {
			Punkts_.add(punkt1, GB_COM_GET_TIME_SINCHR);
			Punkts_.add(punkt9, GB_COM_GET_NET_ADR);
			Punkts_.add(punkt2, GB_COM_GET_DEVICE_NUM);
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt5, GB_COM_GET_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				Punkts_.add(punkt6, GB_COM_GET_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				Punkts_.add(punkt7, GB_COM_GET_COM_PRM_KEEP);
			}
			Punkts_.add(punkt23, GB_COM_GET_COR_U_I);
		}
	}

	// ������� �������, �� ������� �������� ������ ����.
	sParam.txComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt,
			cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ����������� ���.������, ��� ���������� ����������
	poz = 60;
	if (sParam.typeDevice == AVANT_K400) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt8) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumPrm, curCom_, 2);
			}
		}
	} else if (sParam.typeDevice == AVANT_RZSK) {
		if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
			if (name == punkt8) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumPrm, curCom_, 2);
			}
		}
	}

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DEV_NUM_MIN,
				sParam.glb.getMaxNumDevices(), "");
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if ((name == punkt4) || (name == punkt4rzsk)) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CF_THRESH_MIN,
				GLB_CF_THRESH_MAX, "��");
	} else if (name == punkt5) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_T_RERUN_MIN,
				GLB_T_RERUN_MAX, "c");
	} else if (name == punkt6) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt7) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt8) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_IN_DEC_MIN,
				GLB_IN_DEC_MAX, "��");
	} else if (name == punkt9) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_NET_ADR_MIN,
				GLB_NET_ADR_MAX, "");
	} else if (name == punkt10) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_U_OUT_NOM_MIN,
				GLB_U_OUT_NOM_MAX, "�");
	} else if (name == punkt11) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_FREQ_MIN, GLB_FREQ_MAX,
				""); // "���" �� �������
	} else if (name == punkt12) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt13) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_AC_IN_DEC_MIN,
				GLB_AC_IN_DEC_MAX, "��");
	} else if (name == punkt14) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_DETECTOR_MIN,
				GLB_DETECTOR_MAX, "");
	} else if (name == punkt15) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_COR_U_DEC_MIN / 10,
				GLB_COR_U_DEC_MAX / 10, "�");
	} else if (name == punkt16) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, 0, GLB_COR_I_DEC_MAX, "��");
	} else if (name == punkt17) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt18) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt19) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_FAIL_MIN,
				GLB_PVZUE_FAIL_MAX, "����");
	} else if (name == punkt20) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_TH_MIN,
				GLB_PVZUE_N_TH_MAX, "���");
	} else if (name == punkt21) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_PVZUE_N_LVL_MIN,
				GLB_PVZUE_N_LVL_MAX, "����");
	} else if (name == punkt22) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	} else if (name == punkt23) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeOnOff);
	} else if (name == punkt24) {
		// TODO K400 �������� ��� ��������� "�������������"
	} else if (name == punkt25) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeList);
	}

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���
			if (name == punkt1) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt2) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt3) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if ((name == punkt4) || (name == punkt4rzsk)) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt5) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt6) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt7) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt8) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt9) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt10) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			} else if (name == punkt11) {
				uint16_t t = EnterParam.getValue();
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t >> 8), 0);
				sParam.txComBuf.setInt8(static_cast<uint8_t>(t), 1);
			} else if (name == punkt12) {
				uint8_t t = EnterParam.getValueEnter();
				sParam.txComBuf.setInt8(t);
			} else if (name == punkt13) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt14) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 0);
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 1);
			} else if (name == punkt15) {
				// ���� ������� �������� ��������� ���� ����� 0
				// �� ���������� ��������� � ���.������ ������ 4
				// ���������� ����� ���������
				int16_t t = static_cast<int16_t>(EnterParam.getValue());
				uint8_t dop = EnterParam.getDopValue();
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
			} else if (name == punkt16) {
				// ���� ������� �������� ��������� ���� ����� 0
				// �� ���������� ��������� � ���.������ ������ 5
				// ���������� ����� ���������
				int16_t t = static_cast<int16_t>(EnterParam.getValue());
				uint8_t dop = EnterParam.getDopValue();
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
			} else if (name == punkt17) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt18) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt19) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt20) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt21) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt22) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt23) {
				sParam.txComBuf.setInt8(EnterParam.getDopValue(), 0);
				sParam.txComBuf.setInt8(EnterParam.getValueEnter(), 1);
			} else if (name == punkt24) {
				// TODO K400 ������� �� �������� ��� ��������� "�������������"
			} else if (name == punkt25) {
				sParam.txComBuf.setInt8(EnterParam.getValueEnter());
			}
			sParam.txComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d", sParam.glb.getDeviceNum());
		} else if (name == punkt3) {
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if ((name == punkt4) || (name == punkt4rzsk)) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getCfThreshold());
		} else if (name == punkt5) {
			snprintf(&vLCDbuf[poz], 11, "%d�", sParam.glb.getTimeRerun());
		} else if (name == punkt6) {
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt7) {
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt8) {
			uint8_t val = sParam.glb.getInDecrease(curCom_);
			snprintf(&vLCDbuf[poz], 11, "%d��", val);
		} else if (name == punkt9) {
			snprintf(&vLCDbuf[poz], 11, "%u", sParam.glb.getNetAddress());
		} else if (name == punkt10) {
			snprintf(&vLCDbuf[poz], 11, "%u�", sParam.glb.getUoutNom());
		} else if (name == punkt11) {
			snprintf(&vLCDbuf[poz], 11, "%u���", sParam.glb.getFreq());
		} else if (name == punkt12) {
			uint8_t val = static_cast<uint8_t>(sParam.glb.getCompatibility());
			snprintf_P(&vLCDbuf[poz], 11, fcCompatibility[val]);
		} else if (name == punkt13) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getAcInDec());
		} else if (name == punkt14) {
			snprintf(&vLCDbuf[poz], 11, "%u", sParam.glb.getDetector());
		} else if (name == punkt15) {
			int16_t val = sParam.glb.getCorU();
			int8_t f = val % 10;
			if (f < 0)
				f = -f;
			// �������� �����/������� ����� ���������� ���������
			// � ������� ����� ������������� ���������� ���� �����
			snprintf(&vLCDbuf[poz], 11, "%d.%d�", val / 10, f);
		} else if (name == punkt16) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.glb.getCorI());
		} else if (name == punkt17) {
			uint8_t val = sParam.glb.getPvzueProtocol();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueProtocol[val]);
		} else if (name == punkt18) {
			uint8_t val = sParam.glb.getPvzueParity();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueParity[val]);
		} else if (name == punkt19) {
			snprintf(&vLCDbuf[poz], 11, "%u����", sParam.glb.getPvzueFail());
		} else if (name == punkt20) {
			snprintf(&vLCDbuf[poz], 11, "%u���", sParam.glb.getPvzueNoiseTH());
		} else if (name == punkt21) {
			snprintf(&vLCDbuf[poz], 11, "%u����",
					sParam.glb.getPvzueNoiseLvl());
		} else if (name == punkt22) {
			uint8_t val = sParam.glb.getPvzueTypeAC();
			snprintf_P(&vLCDbuf[poz], 11, fcPvzueTypeAC[val]);
		} else if (name == punkt23) {
			uint8_t val = sParam.glb.getBackup() ? 1 : 0;
			snprintf_P(&vLCDbuf[poz], 11, fcOnOff[val]);
		} else if (name == punkt24) {
			// TODO K400 ����� ��������� "�������������"
		} else if (name == punkt25) {
			snprintf_P(&vLCDbuf[poz], 11,
					fcNumDevices[sParam.def.getNumDevices()]);
		}
	}

	switch (key_) {
	case KEY_UP:
		cursorLineUp();
		break;
	case KEY_DOWN:
		cursorLineDown();
		break;

	case KEY_LEFT:
		curCom_ = curCom_ <= 1 ? sParam.glb.getNumDevices() : curCom_ - 1;
		break;
	case KEY_RIGHT:
		curCom_ = curCom_ >= sParam.glb.getNumDevices() ? 1 : curCom_ + 1;
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetupParam;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// ��������� ���� � ���������� �������� � ����� ������
		// � ����� ��������� ���������� �����
		// ��������� ��������� ������ � "�������"
		if (name == punkt15) {
			// �������� �������� �������� ����������
			// �� ��������� ����� 0, ��� ������ ����� ��������
			EnterParam.setEnable(MENU_ENTER_PARAM_U_COR);
			EnterParam.setValueRange(0, GLB_COR_U_DEC_MAX);
			EnterParam.setValue(0);
			EnterParam.setDisc(GLB_COR_U_DEC_DISC);
			EnterParam.setFract(GLB_COR_U_DEC_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (name == punkt16) {
			// �������� �������� �������� ����
			// �� ��������� ����� 0, ��� ������ ����� ��������
			EnterParam.setEnable();
			EnterParam.setValueRange(0, GLB_COR_I_DEC_MAX);
			EnterParam.setValue(0);
			EnterParam.setDisc(GLB_COR_I_DEC_DISC);
			EnterParam.setFract(GLB_AC_IN_DEC_FRACT);
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (sParam.glb.status.getRegime() != GB_REGIME_DISABLED) {
			// ���� ���� ���� �� ��������� ��������� �������
			// ��������� ���������� ���������
			EnterParam.printMessage();
		} else if (name == punkt1) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getTimeSinchr() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (name == punkt2) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_DEV_NUM_MIN,
					sParam.glb.getMaxNumDevices());
			EnterParam.setValue(sParam.glb.getDeviceNum());
			EnterParam.setDisc(GLB_DEV_NUM_DISC);
			EnterParam.setFract(GLB_DEV_NUM_FRACT);
			EnterParam.com = GB_COM_SET_DEVICE_NUM;
		} else if (name == punkt3) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getOutCheck() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_OUT_CHECK;
		} else if ((name == punkt4) || (name == punkt4rzsk)) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_CF_THRESH_MIN, GLB_CF_THRESH_MAX);
			EnterParam.setValue(sParam.glb.getCfThreshold());
			EnterParam.setDisc(GLB_CF_THRESH_DISC);
			EnterParam.setFract(GLB_CF_THRESH_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (name == punkt5) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_T_RERUN_MIN, GLB_T_RERUN_MAX);
			EnterParam.setValue(sParam.glb.getTimeRerun());
			EnterParam.setDisc(GLB_T_RERUN_DISC);
			EnterParam.setFract(GLB_T_RERUN_FRACT);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt6) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrdKeep() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (name == punkt7) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getComPrmKeep() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (name == punkt8) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_IN_DEC_MIN, GLB_IN_DEC_MAX);
			EnterParam.setValue(sParam.glb.getInDecrease(curCom_));
			EnterParam.setDisc(GLB_IN_DEC_DISC);
			EnterParam.setFract(GLB_IN_DEC_FRACT);
			EnterParam.setDopValue(curCom_ + 1);
			EnterParam.com = GB_COM_SET_CF_THRESHOLD;
		} else if (name == punkt9) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_NET_ADR_MIN, GLB_NET_ADR_MAX);
			EnterParam.setValue(sParam.glb.getNetAddress());
			EnterParam.setDisc(GLB_NET_ADR_DISC);
			EnterParam.setFract(GLB_NET_ADR_FRACT);
			EnterParam.com = GB_COM_SET_NET_ADR;
		} else if (name == punkt10) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_U_OUT_NOM_MIN, GLB_U_OUT_NOM_MAX);
			EnterParam.setValue(sParam.glb.getUoutNom());
			EnterParam.setDisc(GLB_U_OUT_NOM_DISC);
			EnterParam.setFract(GLB_U_OUT_NOM_FRACT);
			EnterParam.com = GB_COM_SET_COM_PRM_KEEP;
		} else if (name == punkt11) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_FREQ_MIN, GLB_FREQ_MAX);
			EnterParam.setValue(sParam.glb.getFreq());
			EnterParam.setDisc(GLB_FREQ_DISC);
			EnterParam.setFract(GLB_FREQ_FRACT);
			EnterParam.com = GB_COM_SET_FREQ;
		} else if (name == punkt12) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_COMPATIBILITY_MIN,
					GB_COMPATIBILITY_MAX - 1);
			uint8_t val = (uint8_t) sParam.glb.getCompatibility();
			EnterParam.setValue(val);
			EnterParam.list = fcCompatibility[0];
			EnterParam.com = GB_COM_SET_COM_PRD_KEEP;
		} else if (name == punkt13) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_AC_IN_DEC_MIN, GLB_AC_IN_DEC_MAX);
			EnterParam.setValue(sParam.glb.getAcInDec());
			EnterParam.setDisc(GLB_AC_IN_DEC_DISC);
			EnterParam.setFract(GLB_AC_IN_DEC_FRACT);
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt14) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_DETECTOR_MIN, GLB_DETECTOR_MAX);
			EnterParam.setValue(sParam.glb.getDetector());
			EnterParam.setDisc(GLB_DETECTOR_DISC);
			EnterParam.setFract(GLB_DETECTOR_FRACT);
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_TIME_SINCHR;
		} else if (name == punkt17) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_PROTOCOL_MIN,
					GB_PVZUE_PROTOCOL_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueProtocol());
			EnterParam.list = fcPvzueProtocol[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt18) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_PARITY_MIN,
					GB_PVZUE_PARITY_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueParity());
			EnterParam.list = fcPvzueParity[0];
			EnterParam.setDopValue(2);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt19) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_FAIL_MIN, GLB_PVZUE_FAIL_MAX);
			EnterParam.setValue(sParam.glb.getPvzueFail());
			EnterParam.setDisc(GLB_PVZUE_FAIL_DISC);
			EnterParam.setFract(GLB_PVZUE_FAIL_FRACT);
			EnterParam.setDopValue(3);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt20) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_N_TH_MIN, GLB_PVZUE_N_TH_MAX);
			EnterParam.setValue(sParam.glb.getPvzueNoiseTH());
			EnterParam.setDisc(GLB_PVZUE_N_TH_DISC);
			EnterParam.setFract(GLB_PVZUE_N_TH_FRACT);
			EnterParam.setDopValue(4);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt21) {
			EnterParam.setEnable();
			EnterParam.setValueRange(GLB_PVZUE_N_LVL_MIN, GLB_PVZUE_N_LVL_MAX);
			EnterParam.setValue(sParam.glb.getPvzueNoiseLvl());
			EnterParam.setDisc(GLB_PVZUE_N_LVL_DISC);
			EnterParam.setFract(GLB_PVZUE_N_LVL_FRACT);
			EnterParam.setDopValue(5);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt22) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_PVZUE_TYPE_AC_MIN,
					GB_PVZUE_TYPE_AC_MAX - 1);
			EnterParam.setValue(sParam.glb.getPvzueTypeAC());
			EnterParam.list = fcPvzueTypeAC[0];
			EnterParam.setDopValue(6);
			EnterParam.com = GB_COM_SET_TIME_RERUN;
		} else if (name == punkt23) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(0, 1);
			uint8_t val = sParam.glb.getBackup() ? 1 : 0;
			EnterParam.setValue(val);
			EnterParam.list = fcOnOff[0];
			EnterParam.setDopValue(1);
			EnterParam.com = GB_COM_SET_COR_U_I;
		} else if (name == punkt24) {
			// TODO K400 ���� ��������� � ���������� "�������������"
		} else if (name == punkt25) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(GB_NUM_DEVICES_MIN,
					GB_NUM_DEVICES_MAX - 1);
			EnterParam.setValue(sParam.def.getNumDevices());
			EnterParam.list = fcNumDevices[0];
			EnterParam.com = GB_COM_DEF_SET_LINE_TYPE;
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

		Punkts_.clear();
		Punkts_.add(punkt1);
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);
		Punkts_.add(punkt6);

		// ������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_TIME);
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[20], 21, title);

	printMeasParam(0, MENU_MEAS_PARAM_DATE);
	printMeasParam(1, MENU_MEAS_PARAM_TIME);

	if (EnterParam.isEnable()) {
		// ����� �������� ������
		snprintf_P(&vLCDbuf[20 * lineParam_], 21, name, cursorLine_);
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����������� ��������� �������� �� ���� �����
//			sParam.txComBuf.setInt8(BIN_TO_BCD(EnterParam.getValueEnter()),
//					EnterParam.getDopValue());
			uint8_t t = EnterParam.getDopValue();
			uint8_t val = EnterParam.getValueEnter();
			if (t <= 2) {
				// ���� ����
				// �������� ����������� ����� �� �������
				if (t == 0) {
					// ���� ����, �������� ����, �.�. ����� ���� ����������
					uint8_t month = BCD_TO_BIN(sParam.txComBuf.getInt8(1));
					uint8_t day = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
					if (day > sParam.dateTime.getNumDaysInMonth(month, val)) {
						sParam.txComBuf.setInt8(0x01, 2);
					}
				} else if (t == 1) {
					// ���� ������, �������� ���-�� ������������� ����
					uint8_t day = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
					if (day > sParam.dateTime.getNumDaysInMonth(val)) {
						sParam.txComBuf.setInt8(0x01, 2);
					}
				}
				sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getHour()), 3);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMinute()), 4);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getSecond()), 5);
				sParam.txComBuf.addFastCom(EnterParam.com);
			} else if (t <= 5) {
				// ���� �������
				// �������� ����������� ���� �� �������
				sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getYear()), 0);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMonth()), 1);
				sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getDay()), 2);
				sParam.txComBuf.addFastCom(EnterParam.com);
			}
			EnterParam.setDisable();
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
		if (name == punkt1) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 99);
			EnterParam.setValue(sParam.dateTime.getYear());
			EnterParam.setDopValue(0);
		} else if (name == punkt2) {
			EnterParam.setEnable();
			EnterParam.setValueRange(1, 12);
			EnterParam.setValue(sParam.dateTime.getMonth());
			EnterParam.setDopValue(1);
		} else if (name == punkt3) {
			EnterParam.setEnable();
			uint8_t max = sParam.dateTime.getNumDaysInMonth();
			EnterParam.setValueRange(1, max);
			EnterParam.setValue(sParam.dateTime.getDay());
			EnterParam.setDopValue(2);
		} else if (name == punkt4) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 23);
			EnterParam.setValue(sParam.dateTime.getHour());
			EnterParam.setDopValue(3);
		} else if (name == punkt5) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 59);
			EnterParam.setValue(sParam.dateTime.getMinute());
			EnterParam.setDopValue(4);
		} else if (name == punkt6) {
			EnterParam.setEnable();
			EnterParam.setValueRange(0, 59);
			EnterParam.setValue(sParam.dateTime.getSecond());
			EnterParam.setDopValue(5);
		}
		EnterParam.com = GB_COM_SET_TIME;
		EnterParam.setDisc(1);
		EnterParam.setFract(1);
		// �������� ������� �������� ���� � �������
		// ����� ����������� � ������� �������� � ���
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getYear()), 0);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMonth()), 1);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getDay()), 2);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getHour()), 3);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getMinute()), 4);
		sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.dateTime.getSecond()), 5);
		break;

	case KEY_CANCEL:
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	default:
		break;
	}
}

/** ������� ����. ���������
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupInterface() {
	static char title[] PROGMEM = "���������\\���������";
	static char punkt1[] PROGMEM = "��������� �����";
	// ��� RS-485 �������� ��������� ���������:
	// ���������: ��������, modbus...
	// �������� ���/�: 1200, 4800, 9600, 19200, 38400, 57600
	// ���� ������: ������ 8
	// ��������: ���,�����, ���, ������(1), ������(0)
	// �������� ����: 2, 1.5, 1
	// � RS-232 ������: ��������, 19200 ���/�, 8 ���, 2 ����-����, ��������-���
	static char punkt2[] PROGMEM = "��������";
	static char punkt3[] PROGMEM = "�������� ��������";
	static char punkt4[] PROGMEM = "���� ������";
	static char punkt5[] PROGMEM = "��������";
	static char punkt6[] PROGMEM = "�������� ����";

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(punkt1);
		// ���� ����������� ����� �� ��������� ����
		// ���������� ��������� ������
		// � RS-232 ������: 19200 ���/�, 8 ���, 2 ����-����, ��������-���
		// (�� �� ����� ������� ����� �����������)
#ifndef DEBUG
		if (sParam.Uart.Interface.get() == GB_INTERFACE_RS485) {
			Punkts_.add(punkt2);
			Punkts_.add(punkt3);
			Punkts_.add(punkt4);
			Punkts_.add(punkt5);
			Punkts_.add(punkt6);
		}
#else
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
		Punkts_.add(punkt5);
		Punkts_.add(punkt6);
#endif

		// �������������� �������
		sParam.txComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_, Punkts_.getMaxNumPunkts());

	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ����� ������� "��������:" � ������� � ������ ������ ���������
	// ��� ������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	snprintf_P(&vLCDbuf[poz], 11, fcRangeList);

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			if (name == punkt1) {
				uint8_t tmp = EnterParam.getValueEnter();
				TInterface::INTERFACE val;
				val = static_cast<TInterface::INTERFACE> (tmp);
				// ���� ��������� ��������, ������� ����
				if (val != sParam.Uart.Interface.get()) {
					sParam.Uart.Interface.set(val);
					lvlCreate_ = true;
				}
			} else if (name == punkt2) {
				uint8_t tmp = EnterParam.getValueEnter();
				TProtocol::PROTOCOL val;
				val = static_cast<TProtocol::PROTOCOL> (tmp);
				sParam.Uart.Protocol.set(val);
			} else if (name == punkt3) {
				uint8_t tmp = EnterParam.getValueEnter();
				TBaudRate::BAUD_RATE val;
				val = static_cast<TBaudRate::BAUD_RATE> (tmp);
				sParam.Uart.BaudRate.set(val);
			} else if (name == punkt4) {
				uint8_t tmp = EnterParam.getValueEnter();
				TDataBits::DATA_BITS val;
				val = static_cast<TDataBits::DATA_BITS> (tmp);
				sParam.Uart.DataBits.set(val);
			} else if (name == punkt5) {
				uint8_t tmp = EnterParam.getValueEnter();
				TParity::PARITY val;
				val = static_cast<TParity::PARITY> (tmp);
				sParam.Uart.Parity.set(val);
			} else if (name == punkt6) {
				uint8_t tmp = EnterParam.getValueEnter();
				TStopBits::STOP_BITS val;
				val = static_cast<TStopBits::STOP_BITS> (tmp);
				sParam.Uart.StopBits.set(val);
			}
			EnterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Interface.get());
			snprintf_P(&vLCDbuf[poz], 11, fcInterface[val]);
		} else if (name == punkt2) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Protocol.get());
			snprintf_P(&vLCDbuf[poz], 11, fcProtocol[val]);
		} else if (name == punkt3) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.BaudRate.get());
			snprintf_P(&vLCDbuf[poz], 11, fcBaudRate[val]);
		} else if (name == punkt4) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.DataBits.get());
			snprintf_P(&vLCDbuf[poz], 11, fcDataBits[val]);
		} else if (name == punkt5) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.Parity.get());
			snprintf_P(&vLCDbuf[poz], 11, fcParity[val]);
		} else if (name == punkt6) {
			uint8_t val = static_cast<uint8_t>(sParam.Uart.StopBits.get());
			snprintf_P(&vLCDbuf[poz], 11, fcStopBits[val]);
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
		lvlMenu = &clMenu::lvlSetup;
		lvlCreate_ = true;
		break;
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		enterFunc = &clMenu::enterValue;
		// � ����� ��������� ���������� �����
		// ��������� ��������� ������ � "�������"
		if (name == punkt1) {
			// ��������� �����
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TInterface::MIN, TInterface::MAX - 1);
			EnterParam.setValue(sParam.Uart.Interface.get());
			EnterParam.list = fcInterface[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt2) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TProtocol::MIN, TProtocol::MAX - 1);
			EnterParam.setValue(sParam.Uart.Protocol.get());
			EnterParam.list = fcProtocol[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt3) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TBaudRate::MIN, TBaudRate::MAX - 1);
			EnterParam.setValue(sParam.Uart.BaudRate.get());
			EnterParam.list = fcBaudRate[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt4) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TDataBits::MIN, TDataBits::MAX - 1);
			EnterParam.setValue(sParam.Uart.DataBits.get());
			EnterParam.list = fcDataBits[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt5) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TParity::MIN, TParity::MAX - 1);
			EnterParam.setValue(sParam.Uart.Parity.get());
			EnterParam.list = fcParity[0];
			EnterParam.com = GB_COM_NO;
		} else if (name == punkt6) {
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			EnterParam.setValueRange(TStopBits::MIN, TStopBits::MAX - 1);
			EnterParam.setValue(sParam.Uart.StopBits.get());
			EnterParam.list = fcStopBits[0];
			EnterParam.com = GB_COM_NO;
		}
		break;
	default:
		break;
	}
}

/** ������� ����. ���� 1.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest() {
	static char title[] PROGMEM 		= "���������\\�����";
	static char punkt1[] PROGMEM 		= "%d. ���� �����������";
	static char punkt2[] PROGMEM 		= "%d. ���� ���������";
	static char message[][21] PROGMEM 	= {
		   //12345678901234567890
			"    ��������� �     ",		//
			"   ����� �������    " 		//
	};

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		// ���� ����������� ��� ������� ����������� ��� ������
		if ((sParam.prd.status.isEnable()) || (sParam.def.status.isEnable())) {
			Punkts_.add(punkt1);
		}
		// ���� ��������� ��� ������� ��������� ��� ������
		if ((sParam.prm.status.isEnable()) || (sParam.def.status.isEnable())) {
			Punkts_.add(punkt2);
		}

		// ������������� �������
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_R400M) {
			// ������������� � �400�
			sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 20, title);
	if (isMessage()) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		uint8_t poz = 40;
		for (uint_fast8_t i = 0; i < 2; i++, poz += 20)
			snprintf_P(&vLCDbuf[poz], 21, message[i]);
	} else
		printPunkts();

	eGB_REGIME reg = sParam.glb.status.getRegime();
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
	case KEY_MENU:
		lvlMenu = &clMenu::lvlStart;
		lvlCreate_ = true;
		break;

	case KEY_ENTER:
		if (name == punkt1) {
			if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_2)) {
				//
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.setInt8(0, 0);
				sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_1);
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			} else if (reg == GB_REGIME_TEST_1) {
				lvlMenu = &clMenu::lvlTest1;
				lvlCreate_ = true;
			}
		} else if (name == punkt2) {
			if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_1)) {
				sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_2);
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			} else if (reg == GB_REGIME_TEST_2) {
				lvlMenu = &clMenu::lvlTest2;
				lvlCreate_ = true;
			}
		}
		// ���� ������� ���� (�.�. ����� �������� �����) �� ���������,
		// ������� ���������
		if (lvlMenu == &clMenu::lvlTest)
			printMessage();
		break;

	default:
		break;
	}

}

/** ������� ����. ���� 1.
 *  ���� � ������� 1 ������� ����� ������� ����� ����������� � ������ ������
 * 	��������� ����� � ���������� ����� ����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest1() {
	static char title[] PROGMEM = "�����\\����������";
	static char punkt1[] PROGMEM = "������� �����������";
	static uint8_t cnt = 0;		// ������� �� ������ ��� ��������� ������
	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {


		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		// � ������ �� ���� �������� ���.���������
		lineParam_ = (device == AVANT_OPTO) ? 1 : 2;

		vLCDclear();
		vLCDdrawBoard(lineParam_);
		cnt = 0;

		// �������������� �������
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom1(GB_COM_GET_MEAS);	// ���������
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// �������

		// ������� ��� ������
		sParam.test.clear();
		if (device == AVANT_R400M) {
			sParam.test.addSignalToList(GB_SIGNAL_RZ);
			if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT)
				sParam.test.addSignalToList(GB_SIGNAL_CF);
		} else if (device == AVANT_RZSK) {
			sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF2_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_CF2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_NO_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM1_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM2_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM3_RZ);
			sParam.test.addSignalToList(GB_SIGNAL_COM4_RZ);
		} else if (device == AVANT_K400) {
			sParam.test.addSignalToList(GB_SIGNAL_CF1);
			sParam.test.addSignalToList(GB_SIGNAL_CF2);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}
		} else if (device == AVANT_OPTO) {
			sParam.test.addSignalToList(GB_SIGNAL_CF);
			uint8_t num_com = sParam.prd.getNumCom();
			for (uint_fast8_t i = 0; i < num_com; i++) {
				eGB_TEST_SIGNAL signal =
						(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
				sParam.test.addSignalToList(signal);
			}
		}
	}

	// ����� �� ����� ���������� ����������, ���� ��� �� ������
	if (device != AVANT_OPTO) {
		printMeasParam(2, MENU_MEAS_PARAM_UOUT);
		printMeasParam(3, MENU_MEAS_PARAM_IOUT);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ������� ������(�)
			uint8_t sig = EnterParam.listValue[EnterParam.getValue()];
			uint8_t rz = 0;
			uint8_t cf = 0;

			sParam.test.getBytes(cf, rz, (eGB_TEST_SIGNAL) sig);
			// �.�. � ��� ��� ��������� �������� ���� ��� ������ ������
			// ��� ������ �� ������� ��������� �������� ����� �������
			// ������� � ������ ������� ��� ������ �� �����
			// !!! ��� �������� ������� ���� ��������� ������ � ������
			// ��
			sParam.txComBuf.setInt8(1, 0);				// ������ ��
			sParam.txComBuf.setInt8(cf, 1);				// ������� ������ ��
			sParam.txComBuf.addFastCom(EnterParam.com);
			// ��
			sParam.txComBuf.setInt8(2, 2);				// ������ ��
			sParam.txComBuf.setInt8(rz, 3);				// ������� ������ ��
			sParam.txComBuf.addFastCom(EnterParam.com);

			EnterParam.setDisable();
		}
	} else {
		uint8_t poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// ����� �� ����� ��� �������������� ������
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_1) {
		if (++cnt >= TIME_TEST_EXIT) {
			key_ = KEY_CANCEL;
		}
	} else {
		cnt = 0;
	}

	switch (key_) {
		case KEY_CANCEL:
			sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
			lvlMenu = &clMenu::lvlTest;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			EnterParam.setValue(0);
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST_2);
			EnterParam.setValueRange(0, sParam.test.getNumSignals() - 1);
			EnterParam.listValue = sParam.test.signalList;
			EnterParam.list = fcTest1K400[0];
			EnterParam.com = GB_COM_SET_REG_TEST_1;
			break;

		default:
			break;
	}
}

/** ������� ����. ���� 2.
 * 	���� � ������� 1 ������� ����� ������� ����� ����������� � ������ ������
 * 	��������� ����� � ���������� ����� ����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest2() {
	static char title[] 	PROGMEM = "�����\\��������";
	static char punkt1[] 	PROGMEM = "������� ���������";
	static char prm1[] 	PROGMEM = "���1: ";
	static char prm2[]	PROGMEM = "���2: ";

	static uint8_t cnt = 0;		// ������� �� ������ ��� ��������� ������
	eGB_TYPE_DEVICE device = sParam.typeDevice;

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;


		if (device == AVANT_OPTO) {
			// � ������ ���������� ��������� �������� �� ����
			lineParam_ = 1;
		} else if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			// � ���� �������� ����� 2 ������ ����������
			lineParam_ = 3;
		} else {
			// ����� ���� ������ ����������
			lineParam_ = 2;
		}
		vLCDclear();
		vLCDdrawBoard(lineParam_);
		cnt = 0;

		// �������������� �������
		sParam.txComBuf.clear();
		if (device != AVANT_OPTO) {
			sParam.txComBuf.addCom1(GB_COM_GET_MEAS);	// ���������
		}
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// �������
	}

	// ����� �� ����� ���������� ����������, ���� ��� �� ������
	if (device != AVANT_OPTO) {
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			if (sParam.def.status.isEnable()) {
				printMeasParam(2, MENU_MEAS_PARAM_UC1);
				printMeasParam(3, MENU_MEAS_PARAM_UZ1);
				printMeasParam(4, MENU_MEAS_PARAM_UC2);
				printMeasParam(5, MENU_MEAS_PARAM_UZ2);
			} else {
				printMeasParam(2, MENU_MEAS_PARAM_UC1);
				printMeasParam(3, MENU_MEAS_PARAM_UN1);
				printMeasParam(4, MENU_MEAS_PARAM_UC2);
				printMeasParam(5, MENU_MEAS_PARAM_UN2);
			}
		} else {
			printMeasParam(2, MENU_MEAS_PARAM_UC);
			if (sParam.def.status.isEnable()) {
				printMeasParam(3, MENU_MEAS_PARAM_UZ);
			} else {
				printMeasParam(3, MENU_MEAS_PARAM_UN);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

	uint8_t poz = 100;
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		poz = 80;
		poz += snprintf_P(&vLCDbuf[poz], 11, prm1);
		snprintf_P(&vLCDbuf[poz], 11,
					fcTest1K400[sParam.test.getCurrentSignal()]);
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, prm2);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal2()]);
	} else {
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
		snprintf_P(&vLCDbuf[poz], 11,
				fcTest1K400[sParam.test.getCurrentSignal()]);
	}

	// ����� �� ����� ��� ��������������� ������
	if (sParam.glb.status.getRegime() != GB_REGIME_TEST_2) {
		if (++cnt >= TIME_TEST_EXIT) {
			key_ = KEY_CANCEL;
		}
	} else {
		cnt = 0;
	}

	switch (key_) {
		case KEY_CANCEL:
			sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
			lvlMenu = &clMenu::lvlTest;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
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

	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					" �������� ��������  ",//
					"  ����� ������ �    ",		//
					"  ������ �������    " };

			EnterParam.cnt_++;
			key_ = KEY_NO;

			uint8_t poz = 40;
			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else if (status == MENU_ENTER_PARAM_INT) {
		uint16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterInt, val);
		}
	} else if (status == MENU_ENTER_PARAM_U_COR) {
		uint16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			snprintf_P(&vLCDbuf[poz], 21, enterUcor, val / 10, val % 10);
		}
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = EnterParam.getValue();
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				EnterParam.list + STRING_LENGHT * val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = EnterParam.listValue[EnterParam.getValue()];
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				EnterParam.list + STRING_LENGHT * val);
	} else
		key_ = KEY_CANCEL;

	switch (key_) {
	case KEY_CANCEL:
		EnterParam.setDisable();
		break;

	case KEY_UP:
		EnterParam.incValue(timePressKey());
		break;
	case KEY_DOWN:
		EnterParam.decValue(timePressKey());
		break;

	case KEY_ENTER:
		EnterParam.setEnterValueReady();
		break;
	default:
		break;
	}

	key_ = KEY_NO;
	return EnterParam.getStatus();
}

/** ���� ������
 * 	@param ���
 * 	@return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterPassword() {
	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					"       ������       ",//
					"    ������������    ",		//
					"       ������       " };

			EnterParam.cnt_++;
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

		uint16_t val = EnterParam.getValue();

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
		EnterParam.setDisable();
		break;

	case KEY_ENTER:
		if (status == MENU_ENTER_PASSWORD)
			EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_READY);
		else if (status == MENU_ENTER_PASSWORD_NEW)
			EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_N_READY);
		else
			EnterParam.setDisable();
		break;

	case KEY_UP:
		EnterParam.setDisc(1000);
		EnterParam.incValue();
		break;
	case KEY_RIGHT:
		EnterParam.setDisc(100);
		EnterParam.incValue();
		break;
	case KEY_DOWN:
		EnterParam.setDisc(10);
		EnterParam.incValue();
		break;
	case KEY_LEFT:
		EnterParam.setDisc(1);
		EnterParam.incValue();
		break;

	default:
		break;
	}
	key_ = KEY_NO;

	return EnterParam.getStatus();
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
		snprintf_P(&vLCDbuf[20 * line], 21, Punkts_.getName(cntPunkts),
				cntPunkts + 1);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts())
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
			snprintf_P(&vLCDbuf[poz], 11, fcDate, sParam.dateTime.getDay(),
					sParam.dateTime.getMonth(), sParam.dateTime.getYear());
			break;
		case MENU_MEAS_PARAM_TIME:
			snprintf_P(&vLCDbuf[poz], 11, fcTime, sParam.dateTime.getHour(),
					sParam.dateTime.getMinute(), sParam.dateTime.getSecond());
			break;
		case MENU_MEAS_PARAM_UZ:
			snprintf_P(&vLCDbuf[poz], 11, fcUz,
					sParam.measParam.getVoltageDef());
			break;

			// � 3-� �������� ����� ���� Uz1 == Uz, Uz2
		case MENU_MEAS_PARAM_UZ1:
			snprintf_P(&vLCDbuf[poz], 11, fcUz1,
					sParam.measParam.getVoltageDef());
			break;
		case MENU_MEAS_PARAM_UZ2:
			snprintf_P(&vLCDbuf[poz], 11, fcUz2,
					sParam.measParam.getVoltageDef2());
			break;

		case MENU_MEAS_PARAM_UC:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf,
					sParam.measParam.getVoltageCf());
			break;

			// � 3-� �������� ����� ���� Uk1 == Uk, Uk2
		case MENU_MEAS_PARAM_UC1:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf1,
					sParam.measParam.getVoltageCf());
			break;
		case MENU_MEAS_PARAM_UC2:
			snprintf_P(&vLCDbuf[poz], 11, fcUcf2,
					sParam.measParam.getVoltageCf2());
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
			// � 3-� �������� ����� ���� U�1 == U�, U�2
		case MENU_MEAS_PARAM_UN1:
			snprintf_P(&vLCDbuf[poz], 11, fcUn1,
					sParam.measParam.getVoltageNoise());
			break;
		case MENU_MEAS_PARAM_UN2:
			snprintf_P(&vLCDbuf[poz], 11, fcUn2,
					sParam.measParam.getVoltageNoise2());
			break;

		case MENU_MEAS_PARAM_SD:
			snprintf_P(&vLCDbuf[poz], 11, fcSd,
					sParam.measParam.getPulseWidth());
			// ������ �� ������
			break;

		default:
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
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'l', y);
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
	snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}


///**	���������� ���-�� ������ � ����, �.�. ���-�� ������������� �����.
// * 	@param ��� ������
// * 	@return ���-�� ������
// */
//uint8_t clMenu::getNumError(uint16_t val) {
//	uint_fast8_t tmp = 0;
//
//	while ((val & 0x0001) == 0) {
//		tmp++;
//		val >>= 1;
//	}
//
//	return tmp;
//}
