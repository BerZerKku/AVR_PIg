/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>
#include "debug.hpp"

#include "menu.h"
#include "ks0108.h"
#include "flash.h"

/// ����� ��������� �� ���������
#define LED_REGIME LED_SWITCH

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// ���-�� ����� ������ ������������ �� ������
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)


clMenu::clMenu() {
    // ��������� ������� ������� ������ �������������� � ������������� ���-�� ���������� ������
    COMPILE_TIME_ASSERT(SIZE_OF(fcRingRenumber) == (MAX_NUM_COM_RING + 1));

	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;
	lvlCreate_ = true;
	blink_ = false;
	blinkMeasParam_ = false;
	curCom_ = 0;
    clrMessages();

	// ������ ���������
	cursorEnable_ = false;
	cursorLine_ = 0;

	// ������� ������ ��� ���
	key_ = KEY_NO;

	// ��� ���������� ��� �� ��������
	setDevice(AVANT_NO);

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
	sParam.def.status.stateText[12] = fcDefSost12;
	sParam.def.status.stateText[13] = fcDefSost13;
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
	sParam.prm.status.stateText[13] = fcPrmSost13;
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
	sParam.prd.status.stateText[13] = fcPrdSost13;
	sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	// �������� ����� �����������
	sParam.def.status.name = PSTR("���"); 	// fcDeviceName00;
    sParam.prm.status.name = PSTR("���");	// fcDeviceName01;
    sParam.prd.status.name = PSTR("���");	// fcDeviceName02;

#ifdef DEBUG
	// � ������ ������� �������� ���������� ���������
	vLCDsetLed(LED_ON);
#else
	// ��������� ������ ������ ���������
	vLCDsetLed(LED_REGIME);
#endif
}

//
void clMenu::proc(void) {

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
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO) {
		if (tmp == KEY_EMPTY)
			tmp = KEY_NO;
		key_ = tmp;

        sParam.security.rstUserTimer(USER_SOURCE_pi);
		vLCDsetLed(LED_SWITCH);
	}

	if (checkLedOn()) {
		vLCDsetLed(LED_SWITCH);
	}

    // ������� ������ ���������
    if (delay_ > 0) {
        delay_--;
        if (delay_ == 0) {
            clrMessages();
        }
    }

    if (lvlCreate_) {
        clrMessages();
    }

    if (isMessage()) {
        key_ = KEY_NO;
    }

    security();

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
        sParam.txComBuf.addFastCom(GB_COM_GET_VERS, GB_SEND_NO_DATA);
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
	sParam.glb.setTypeDevice(AVANT_K400);
	vKEYset(AVANT_K400);

	// ���������/���������� ���������� � ����������� �� ������� �������������
	eGB_COMP_K400 comp = sParam.glb.getCompK400();
	bool prm = false;
	bool prd = false;
	switch(comp) {
		case GB_COMP_K400_AVANT:
			prm = true;
			prd = true;
			break;
		case GB_COMP_K400_AVANT_PRD:
			prd = true;
			break;
		case GB_COMP_K400_AVANT_PRM:
			prm = true;
			break;
		case GB_COMP_K400_AKPA_PRD:
			prd = true;
			break;
		case GB_COMP_K400_AKPA_PRM:
			prm = true;
			break;
		case GB_COMP_K400_KEDR_PRD:
			prd = true;
			break;
		case GB_COMP_K400_KEDR_PRM:
			prm = true;
			break;
		case GB_COMP_K400_UPKC_PRD:
			prd = true;
			break;
		case GB_COMP_K400_UPKC_PRM:
			prm = true;
			break;
		case GB_COMP_K400_VCTO_PRD:
			prd = true;
			break;
		case GB_COMP_K400_VCTO_PRM:
			prm = true;
			break;
		case GB_COMP_K400_ANKA_PRD:
			prd = true;
			break;
		case GB_COMP_K400_ANKA_PRM:
			prm = true;
			break;
		case GB_COMP_K400_MAX:	// �������� !!!
			break;
	}
	sParam.prd.status.setEnable(prd);
	sParam.prm.status.setEnable(prm);

	// ���������
	sParam.def.status.stateText[9] = fcDefSost09;

	sParam.prd.status.stateText[1] = fcPrdSost01;
	sParam.prd.status.stateText[3] = fcPrdSost03;

	sParam.prm.status.stateText[1] = fcPrmSost01;
	sParam.prm.status.stateText[3] = fcPrmSost03;

	// ���� � ����� ��������� �� ���� ���������
	measParam[0] = MENU_MEAS_PARAM_TIME;
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
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_D;
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
			measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_D;
		}
	}


	// ���������� ������� ���������� ��� ���� "���������"
	uint8_t cnt = 0;
	if (sParam.prd.status.isEnable()) {
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;	// 1
		measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;	// 2
	}
	if (sParam.prm.status.isEnable()) {
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;	// 3
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;	// 4
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UN1; 	// 5
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UN2;	// 6
		} else {
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;	// 3
			measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;	// 4
			measParamLvl[cnt++] = MENU_MEAS_PARAM_D;	// 5
			measParamLvl[cnt++] = MENU_MEAS_PARAM_DF;	// 6 TODO (��� 3-� ��������
		}
	}
	measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;	// 7

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
	// 1 - ���
	sParam.glb.status.warningText[2] = fcGlbWarning04k400;
	sParam.glb.status.warningText[3] = fcGlbWarning08k400;
	sParam.glb.status.warningText[4] = fcGlbWarning10k400;
	// 5-15 ���

	// ���������� ������
	sParam.def.status.setEnable(false);

	// ��������
	// ���������� ������� �������������� ���������
	sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
	sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
	sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
	sParam.prm.status.faultText[3] = fcPrmFault0008k400;
	// 4-7 ���
	sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
	sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
	sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
	sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
	// 12-15 ���
	// ���������� ������� �������������� ���������
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
	sParam.prm.status.warningText[1] = fcPrmWarning02k400;
	sParam.prm.status.warningText[2] = fcPrmWarning04k400;
	sParam.prm.status.warningText[3] = fcPrmWarning08k400;
	// 4-15 ���

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
	sParam.glb.setTypeDevice(AVANT_RZSK);
	vKEYset(AVANT_RZSK);

	sParam.prm.status.setEnable(sParam.prm.getNumCom() != 0);
	sParam.prd.status.setEnable(sParam.prd.getNumCom() != 0);

	// ���������
	sParam.def.status.stateText[9] = fcDefSost09;

	sParam.prd.status.stateText[1] = fcPrdSost01;
	sParam.prd.status.stateText[3] = fcPrdSost03;

	sParam.prm.status.stateText[1] = fcPrmSost01;
	sParam.prm.status.stateText[3] = fcPrmSost03;

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

	// ���������� ������� ���������� ��� ���� "���������"
	uint8_t cnt = 0;
	measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;	// 1
	measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;	// 2
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ1;	// 3
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ2;	// 4
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;	// 5
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;	// 6
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UN1; 	// 7
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UN2;	// 8
	} else {
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ;	// 3
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;	// 4
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;	// 5
	}
	measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;	// 9 ��� 6

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
	// 0-9 ���
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
	sParam.def.status.faultText[15] = fcDefFault8000rzsk;
	// ���������� ������� �������������� ������
	sParam.def.status.warningText[0] = fcDefWarning01rzsk;
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
	// ���������� ������� �������������� ���������
	sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
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
	sParam.glb.setTypeDevice(AVANT_R400M);
	vKEYset(AVANT_R400M);

	// ���������
	sParam.def.status.stateText[9] = fcDefSost09;

	sParam.prd.status.stateText[1] = fcPrdSost01;
	sParam.prd.status.stateText[3] = fcPrdSost03;

	sParam.prm.status.stateText[1] = fcPrmSost01;
	sParam.prm.status.stateText[3] = fcPrmSost03;

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
	measParam[5] = MENU_MEAS_PARAM_SD;
	measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;

	// ���������� ������� ���������� ��� ���� "���������"
	uint8_t cnt = 0;
	measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;	// 1
	measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;	// 2
	measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ;	// 3
	if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;	// 4
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;	// 5
	} else {
		measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;	// 4
	}
	measParamLvl[cnt++] = MENU_MEAS_PARAM_SD;	// 6 ��� 5
	measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;	// 7 ��� 6
	measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;	// 8 ��� 7


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
	sParam.glb.setTypeDevice(AVANT_OPTO);

	sParam.prm.status.setEnable(sParam.prm.getNumCom() != 0);
	sParam.prd.status.setEnable(sParam.prd.getNumCom() != 0);

	if (sParam.def.status.isEnable()) {
		// � ���� ����� ���������� �� ����, ����� �� �400�
		if (sParam.prm.status.isEnable() || sParam.prd.status.isEnable()) {
			vKEYset(AVANT_RZSK);
		} else {
			vKEYset(AVANT_R400M);
		}
	} else {
		vKEYset(AVANT_K400);
	}

	// ���������
	sParam.def.status.stateText[9] = fcDefSost09opto;

	sParam.prd.status.stateText[1] = fcPrdSost01opto;
	sParam.prd.status.stateText[3] = fcPrdSost03opto;

	sParam.prm.status.stateText[1] = fcPrmSost01opto;
	sParam.prm.status.stateText[3] = fcPrmSost03opto;

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
	if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI) {	// �400 ������
		sParam.glb.status.warningText[2] = fcGlbWarning04ring1;	// ������ ��������
		sParam.glb.status.warningText[3] = fcGlbWarning08ring1;	// ������ ��������
	} else {
		sParam.glb.status.warningText[2] = fcGlbWarning04;
		sParam.glb.status.warningText[3] = fcGlbWarning08;
	}
	sParam.glb.status.warningText[4] = fcGlbWarning10;
	sParam.glb.status.warningText[5] = fcGlbWarning20;
	sParam.glb.status.warningText[6] = fcGlbWarning40;
	// 7 ���
	if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI) {
		sParam.glb.status.warningText[8] = fcGlbWarning100ring1;
		sParam.glb.status.warningText[9] = fcGlbWarning200ring1;
		// 10-15 ���
	} else {
		sParam.glb.status.warningText[8] = fcGlbWarning100;
		// 9-15 ���
	}


	// ������
	// ���������� ������� �������������� ������
	sParam.def.status.faultText[0] = fcDefFault0001;
	sParam.def.status.faultText[1] = fcDefFault0002;
	sParam.def.status.faultText[2] = fcDefFault0004;
	// 3
	sParam.def.status.faultText[4] = fcDefFault0010opto;
	// 5-7 ���
	sParam.def.status.faultText[8] = fcDefFault0100;
	sParam.def.status.faultText[9] = fcDefFault0200;
	// 10 ���
	sParam.def.status.faultText[11] = fcDefFault0800;
	// 12 ���
	sParam.def.status.faultText[13] = fcDefFault2000;
	// 14-15 ���
	// ���������� ������� �������������� ������
	sParam.def.status.warningText[0] = fcDefWarning01opto;
	sParam.def.status.warningText[1] = fcDefWarning02;
	// 2-15 ���

	// ��������
	// ���������� ������� �������������� ���������
	sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
	sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
	sParam.prm.status.faultText[2] = fcPrmFault0004opto;
	// 3-7 ���
	sParam.prm.status.faultText[8] = fcPrmFault0100rzsk;
	sParam.prm.status.faultText[9] = fcPrmFault0200rzsk;
	sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
	sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
	// 12-15 ���
	// ���������� ������� �������������� ������
	sParam.prm.status.warningText[0] = fcPrmWarning01opto;
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
	static const char fcUnknownFault[]	 PROGMEM = "�������������";
	static const char fcUnknownWarning[] PROGMEM = "��������������";

	bool status = false;

	sParam.glb.status.setEnable(true);

	// ���� ����������� ��� �������� ����� �������, ������� ����� ������
	// �� ������� ��������.
	//	if (device == AVANT_NO) {
	device = sParam.glb.getTypeDevice();
	if (device == AVANT_NO) {
		bool isPrm = sParam.prm.status.isEnable();
		bool isPrd = sParam.prd.status.isEnable();

		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM) {
			// �� �������
			// ���� ���� ������ + ������� (��� �/��� ���) - ����
			// ���� ���� ������ ������� (��� �/��� ���) - �400
			// ���� ���� ������ � (������ �������� & 0xF000) = 0xF000 - �400�

			if (sParam.def.status.isEnable()) {
				if (isPrm || isPrd) {
					device = AVANT_RZSK;
				} else {
					uint16_t vers = sParam.glb.getVersProgIC(GB_IC_BSP_MCU);
					if ((vers & 0xF000) == 0xF000) {
						device = AVANT_R400M;
					}
				}
			} else {
				if (isPrm || isPrd) {
					device = AVANT_K400;
				}
			}
		} else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO) {
			device = AVANT_OPTO;
		}
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

	//	if (!status) {
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

	// ��������������� ������� ������� ������������ ���������� � "���������"
	for (uint_fast8_t i = 0; i < MAX_NUM_MEAS_PARAM_LVL; i++)
		measParamLvl[i] = MENU_MEAS_PARAM_NO;

	if (device == AVANT_K400) {
		status = setDeviceK400();
	} else if (device == AVANT_RZSK) {
		status = setDeviceRZSK();
	} else if (device == AVANT_R400M) {
		status = setDeviceR400M();
	} else if (device == AVANT_OPTO) {
		status = setDeviceOPTO();
	} else {
		//	if ((!status) || (device == AVANT_NO)) {
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

    sParam.txComBuf.clearCom1();
    sParam.txComBuf.addCom1(GB_COM_GET_NET_ADR);
    sParam.txComBuf.addCom1(GB_COM_DEF_GET_LINE_TYPE);
    if (sParam.typeDevice == AVANT_R400M) {
        sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
    } else if (sParam.typeDevice == AVANT_K400) {
        sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
    }

    sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
    if (sParam.def.status.isEnable()) {
        sParam.txComBuf.addCom1(GB_COM_DEF_GET_JRN_CNT);
    }
    if (sParam.prd.status.isEnable()) {
        sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
    }
    if (sParam.prm.status.isEnable()) {
        sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
    }
    sParam.txComBuf.addCom1(GB_COM_GET_JRN_IS_CNT);

	// "�����" ����� ������������� �������� ���� ��������
	sParam.device = true;
	// ���������� �������� ������ ����
	lvlCreate_ = true;

	return status;
}

// ���������� ��������� ������� �� ����������.
eGB_COM clMenu::getTxCommand() {
    static uint8_t cnt = MAX_NUM_COM_SEND_IN_CYLCE;

//    static QVector<eGB_COM> dcom;

	// ������� ������� ���� � ����� ������� �����������
	eGB_COM com = sParam.txComBuf.getFastCom();

    // ������� ������, ��� ���������� ������� �� ��������
    for(uint8_t i = 0; (com == GB_COM_NO) && (i < 2); i++) {
        cnt = cnt < MAX_NUM_COM_SEND_IN_CYLCE ? cnt + 1 : 0;

        if (cnt == 0) {
//            qDebug() << "Commands per cycle: " << showbase << hex << dcom;
//            dcom.clear();
            com = GB_COM_GET_SOST;
        } else if (cnt == 1) {
            com = GB_COM_GET_TIME;
        } else if (cnt == 2) {
            com = sParam.txComBuf.getLocalCom();
        }

        if (com == GB_COM_NO) {
            com = sParam.txComBuf.getCom2();
        }

        if (com == GB_COM_NO) {
            com = sParam.txComBuf.getCom1();
            cnt = MAX_NUM_COM_SEND_IN_CYLCE;
        }

        if (com == GB_COM_NO) {
            cnt = MAX_NUM_COM_SEND_IN_CYLCE;
        }
    }

//    dcom.append(com);
//    if (dcom.size() >= MAX_NUM_COM_SEND_IN_CYLCE) {
//        qDebug() << "Commands per cycle: " << showbase << hex << dcom;
//        dcom.clear();
//    }

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

void clMenu::clrMessages()
{
    msg_ = MSG_NO;
    delay_ = 0;
}

void clMenu::setMessage(clMenu::msg_t msg)
{
    if (msg < MSG_MAX) {
        msg_ = msg;
        delay_ = TIME_MESSAGE;
    }
}

bool clMenu::printMessage() {
    PGM_P pmsg = nullptr;
    uint8_t nrows = 0;

    switch(msg_) {
        case MSG_NO:
        case MSG_MAX: {
            clrMessages();
        } break;

        case MSG_WRONG_USER: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //2345678901234567890
                " ������������ ����  ",
                "   ��� ���������    "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_WRONG_REGIME: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //2345678901234567890
                " �������� ��������  ",
                "  ����� ������ �    ",
                "  ������ �������    "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_WRONG_PWD: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //02345678901234567890
                "       ������       ",
                "    ������������    ",
                "       ������       "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_WRONG_DEVICE: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //02345678901234567890
                "    ��� ��������    ",
                "   �� ���������!!!  "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_BLOCK_USER: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //02345678901234567890
                "     ����� ����     ",
                "    ������������    "
            };
            // TODO �������� ����������� ������� �� ��������� ����������!
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_DISABLE: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //2345678901234567890
                "    ��������� �     ",
                "   ����� �������    "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_INIT: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //2345678901234567890
                "    �������������   "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;

        case MSG_RESET_PWD: {
            static const char message[][ROW_LEN+1] PROGMEM = {
                //2345678901234567890
                "   ����� �������   ",
                "    � ���������    ",
                "    ����������     "
            };
            pmsg = (PGM_P) message;
            nrows = SIZE_OF(message);
        } break;
    }

    if (nrows > 0) {
        uint8_t start = ((lineParam_ != 2) || (nrows != 2)) ? 3 : 4;
        for(uint8_t line = lineParam_ + 1; line <= NUM_TEXT_LINES; line++) {
            if ((line >= start) && ((line - start) < nrows)) {
                snprintf_P(&vLCDbuf[(line - 1)*ROW_LEN], ROW_LEN + 1, pmsg);
                pmsg += ROW_LEN + 1;
            } else {
                clearLine(line);
            }
        }
    }

    return nrows > 0;
}

bool clMenu::isMessage() const {
    return (delay_ > 0) && (msg_ != MSG_NO) && (msg_ < MSG_MAX);
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
        setMessage(MSG_WRONG_DEVICE);
	} else {
        setMessage(MSG_INIT);
	}
	time++;

    if (isMessage()) {
        printMessage();
    }

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
	static const char fcTimeToAc[] PROGMEM = "%02d:%02d:%02d";
#ifdef AVR
	static const char fcCompType[] PROGMEM = "���������. %S";
#else
    static const char fcCompType[] PROGMEM = "���������. %s";
#endif

	if (lvlCreate_) {
		lvlCreate_ = false;

		cursorEnable_ = false;
		lineParam_ = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.txComBuf.clear();

		sParam.txComBuf.addCom2(GB_COM_GET_FAULT);
		if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
		if (sParam.typeDevice == AVANT_R400M) {
			sParam.txComBuf.addCom2(GB_COM_DEF_GET_TYPE_AC);
		}        
	}

	// ����� �� ����� ���������� ����������
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (i >= MAX_NUM_MEAS_PARAM)
			continue;

		if (blinkMeasParam_)
			printMeasParam(i, measParam[i]);
		else
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
	}

//	uint16_t val = sParam.glb.status.getWarnings();

	uint8_t poz = lineParam_ * 20;
	if (sParam.def.status.isEnable()) {
		printDevicesStatus(poz, &sParam.def.status);
		poz += 20;

		// � �400� ��������� �� � ����� �� ��, � ����� ��� �������������
		if (sParam.typeDevice == AVANT_R400M) {
			uint16_t time = sParam.def.getTimeToAC();
			eGB_TYPE_AC ac = sParam.def.getTypeAC();
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();

			// ���� �������� � �������������, ������� ��� �� �����
			if (comp != GB_COMPATIBILITY_AVANT) {
				snprintf_P(&vLCDbuf[poz], 21, fcCompType,
						fcCompatibility[static_cast<uint8_t>(comp)]);
			}

			if (comp == GB_COMPATIBILITY_LINER) {
				// � �400� ������������� ������ ��������� �������� ""��-����"
				// �� "��-����"
				if  (ac == GB_TYPE_AC_AUTO_NORM) {
					ac = GB_TYPE_AC_AUTO;
				}
			} else if (comp == GB_COMPATIBILITY_AVZK80) {
				// � �400� ������������� ����-80 ��������� �������� ""��-����"
				// �� "��-����"
				if (ac == GB_TYPE_AC_CHECK) {
					ac = GB_TYPE_AC_CHECK_1;
				}
			} else if (comp == GB_COMPATIBILITY_PVZ90) {
				if (ac == GB_TYPE_AC_CHECK) {
					ac = GB_TYPE_AC_CHECK_1;
				}
			}
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
	}

	switch(key_) {
        case KEY_MENU: {
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
        } break;

        case KEY_CALL: {
            sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
            sParam.txComBuf.setInt8(GB_CONTROL_CALL);
        } break;

        case KEY_PUSK_UD: {
			if (sParam.def.status.isEnable()) {
				if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3) {
					sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_ALL);
				} else {
					sParam.txComBuf.setInt8(GB_CONTROL_PUSK_UD_1);
				}
                sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
			}
        } break;

        case KEY_AC_PUSK_UD: {
			if (sParam.def.status.isEnable()) {
				sParam.txComBuf.setInt8(GB_CONTROL_PUSK_AC_UD);
                sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
			}
        } break;

        case KEY_PUSK_NALAD: {
			if (sParam.def.status.isEnable()) {
				if (sParam.def.status.getState() == 7) {
					sParam.txComBuf.setInt8(GB_CONTROL_PUSK_OFF);
                    sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
				} else {
					sParam.txComBuf.setInt8(GB_CONTROL_PUSK_ON);
                    sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
				}
			}
        } break;

        case KEY_AC_RESET: {
			if (sParam.def.status.isEnable()) {
				sParam.txComBuf.setInt8(GB_CONTROL_RESET_AC);
                sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
			}
        } break;

        case KEY_AC_PUSK: {
			if (sParam.def.status.isEnable()) {
				if (sParam.typeDevice == AVANT_R400M) {
					eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();
					if (comp != GB_COMPATIBILITY_LINER) {
						if ((comp == GB_COMPATIBILITY_AVZK80) ||
								(comp == GB_COMPATIBILITY_PVZ90)) {
							sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK);
						} else {
							sParam.txComBuf.setInt8(GB_TYPE_AC_PUSK_SELF);
						}
                        sParam.txComBuf.addFastCom(GB_COM_DEF_SET_TYPE_AC, GB_SEND_INT8);
					}
				}
			}
        } break;

        case KEY_AC_REGIME: {
			if (sParam.def.status.isEnable()) {
				sParam.txComBuf.setInt8(GB_CONTROL_REG_AC);
                sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
			}
        } break;

        case KEY_RESET_IND: {
			if (sParam.prd.status.isEnable() || sParam.prm.status.isEnable()) {
                sParam.txComBuf.addFastCom(GB_COM_PRM_RES_IND, GB_SEND_NO_DATA);
			}
        } break;

        case KEY_PUSK: {
			if (sParam.prm.status.isEnable()) {
                sParam.txComBuf.addFastCom(GB_COM_PRM_ENTER, GB_SEND_NO_DATA);
			}
        } break;

        case KEY_RESET: {
			sParam.txComBuf.setInt8(GB_CONTROL_RESET_SELF);
            sParam.txComBuf.addFastCom(GB_COM_SET_CONTROL, GB_SEND_INT8);
        } break;

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
	static char punkt6[] PROGMEM = "%d. ���������";

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
		if (sParam.glb.getTypeDevice() != AVANT_OPTO) {
			Punkts_.add(punkt6);
		}

		// ������������� �������
		sParam.txComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	printPunkts();

	switch(key_) {
		case KEY_UP:
			cursorLineUp();
			break;
		case KEY_DOWN:
			cursorLineDown();
			break;

		case KEY_ENTER:
			switch(cursorLine_) {
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
				case 6:
					lvlMenu = &clMenu::lvlMeasure;
					lvlCreate_ = true;
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
	static char title[] PROGMEM = "����\\����������";
#ifdef AVR
	static char versProg[] PROGMEM = "%S: %02X.%02X";
#else
    static char versProg[] PROGMEM = "%s: %02X.%02X";
#endif

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

			if (sParam.typeDevice == AVANT_K400) {	// � �400 �������� �������� DSP
				Punkts_.add(GB_IC_BSP_DSP_PLIS);
			}
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
		uint8_t ic = Punkts_.getNumber(cntPunkts);
		uint16_t vers = sParam.glb.getVersProgIC((eGB_IC) ic);
		snprintf_P(&vLCDbuf[20 * line], 21, versProg, fcIC[ic],
				(uint8_t) (vers >> 8), (uint8_t) vers);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts()) {
			break;
		}
	}

	switch(key_) {
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
    static char punkt5[] PROGMEM = "%d. ������������";

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

        sParam.txComBuf.addCom2(GB_COM_GET_JRN_CNT);

		if (sParam.def.status.isEnable()) {
			Punkts_.add(punkt2);
            sParam.txComBuf.addCom2(GB_COM_DEF_GET_JRN_CNT);
		}

		if (sParam.prm.status.isEnable()) {
			Punkts_.add(punkt3);
            sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_CNT);
		}

		if (sParam.prd.status.isEnable()) {
			Punkts_.add(punkt4);
            sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_CNT);
		}

        Punkts_.add(punkt5);
        sParam.txComBuf.setLocalCom(GB_COM_GET_JRN_IS_CNT);
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);
	printPunkts();

	switch(key_) {
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
            } else if (name == punkt5) {
                lvlMenu = &clMenu::lvlJournalSecurity;
                lvlCreate_ = true;
            }
        } break;

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

	static char opto_ring1_13[] PROGMEM = "������ �������������";
	static char opto_ring1_14[] PROGMEM = "������������� �����";

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
        sParam.txComBuf.setLocalCom(GB_COM_GET_JRN_CNT);
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
    if (num_entries != 0) {
        if (device == AVANT_OPTO) {
            // � ������ ������������� ��������� ���-�� ������� � ����� ������
            snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
                       num_entries, sParam.jrnEntry.getNumOpticsEntries());
        } else {
            snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
        }
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
        poz += printDateAdnTimeInJournal(poz);

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
                uint8_t dev = (uint8_t) sParam.jrnEntry.getDeviceJrn();
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event],
                           fcDevicesK400[dev]);
            } else {
                snprintf_P(&vLCDbuf[poz], 21,
                           fcJrnEventK400[MAX_JRN_EVENT_VALUE], event);
            }
        } else if (device == AVANT_RZSK) {
            if (event < MAX_JRN_EVENT_VALUE) {
                uint8_t dev = (uint8_t) sParam.jrnEntry.getDeviceJrn();
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventRZSK[event],
                           fcDevicesK400[dev]);
            } else {
                snprintf_P(&vLCDbuf[poz], 21,
                           fcJrnEventRZSK[MAX_JRN_EVENT_VALUE], event);
            }
        } else if (device == AVANT_OPTO) {
            if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI) {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTOring[event], event);
            } else {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTO[event], event);
            }
        }
    }

	switch(key_) {
		case KEY_UP:
			if (sParam.jrnEntry.setPreviousEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY, GB_SEND_MAX);
				curCom_ = 1;
			}
			break;
		case KEY_DOWN:
			if (sParam.jrnEntry.setNextEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY, GB_SEND_MAX);
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
        sParam.txComBuf.setLocalCom(GB_COM_DEF_GET_JRN_CNT);
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
	if (num_entries != 0) {
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
		// ����� ���������
		snprintf_P(&vLCDbuf[poz], 21, fcStateJrn);
		if (sParam.glb.getTypeLine() != GB_TYPE_LINE_OPTO) {
			snprintf_P(&vLCDbuf[poz + 11], 10,
					sParam.def.status.stateText[sParam.jrnEntry.getEventType()]);
		}
		poz += 20;
		// ����� ����
        poz += printDateAdnTimeInJournal(poz);

		//		uint8_t signals = sParam.jrnEntry.getSignalDef();
		//		snprintf(&vLCDbuf[poz], 21, "%x",signals);
		snprintf_P(&vLCDbuf[poz], 21, fcSignalDefJrn,
				sParam.jrnEntry.getSignalPusk(),
				sParam.jrnEntry.getSignalStop(),
				sParam.jrnEntry.getSignalMan(),
				sParam.jrnEntry.getSignalPrd(),
				sParam.jrnEntry.getSignalPrm(),
				sParam.jrnEntry.getSignalOut());
	}

	switch(key_) {
		case KEY_UP:
			if (sParam.jrnEntry.setPreviousEntry())
                sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY, GB_SEND_MAX);
			break;
		case KEY_DOWN:
			if (sParam.jrnEntry.setNextEntry())
                sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY, GB_SEND_MAX);
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
        sParam.txComBuf.setLocalCom(GB_COM_PRM_GET_JRN_CNT);
		sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
		sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
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
	if (num_entries != 0) {
		if (device == AVANT_OPTO) {
			// � ������ ������������� ��������� ���-�� ������� � ����� ������
			snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
					num_entries, sParam.jrnEntry.getNumOpticsEntries());
		} else {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
		}
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

		if (sParam.glb.getMaxNumDevices() == 3) {
			uint8_t src = sParam.jrnEntry.getSrcCom();
			uint8_t devnum = sParam.glb.getDeviceNum();
			if (devnum == 1) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, fcSrcPrm1[src]);
			} else if (devnum == 2) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, fcSrcPrm2[src]);
			} else if (devnum == 3) {
				snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, fcSrcPrm3[src]);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);
			}
		} else {
			snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);
		}

		poz += 20;
        // ����� ����
        poz += printDateAdnTimeInJournal(poz);

		// ����� �������
		if (device == AVANT_OPTO) {
			// � ������ ���� ���� ������ - �� ��� ������� �������
			// ����� - ������ �� �������� ���
			if (com != 0) {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrmOptoComYes);
			} else {
				snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
			}
		} else {
			uint8_t event = sParam.jrnEntry.getEventType();
			snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
		}
	}

	switch(key_) {
		case KEY_UP:
			if (sParam.jrnEntry.setPreviousEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY, GB_SEND_MAX);
				curCom_ = 1;
			}
			break;
		case KEY_DOWN:
			if (sParam.jrnEntry.setNextEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY, GB_SEND_MAX);
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
        sParam.txComBuf.setLocalCom(GB_COM_PRD_GET_JRN_CNT);
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

	uint8_t numSubEntries = sParam.jrnEntry.getNumOpticsEntries();

	// ����� ����� ������� ������ � �� ���-��
	if (num_entries != 0) {
		if (device == AVANT_OPTO) {
			// � ������ ������������� ��������� ���-�� ������� � ����� ������
			snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntriesOpto, cur_entry,
					num_entries, numSubEntries);
		} else {
			snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
		}
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
			if (curCom_ > numSubEntries) {
				curCom_ = 1;
			}

			com = sParam.jrnEntry.getOpticEntry(curCom_);
		} else {
			com = sParam.jrnEntry.getNumCom();
		}
		uint8_t t = snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);

		// ��� ���������� ��-����������, ������� �������� ������������ �������
		if ((device == AVANT_K400) || (device == AVANT_RZSK)) {
			uint8_t s = sParam.jrnEntry.getSourceCom();
			snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
		} else if (device == AVANT_OPTO) {
			uint8_t s = sParam.jrnEntry.getOpticEntryDR(com);
			snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
		}

		poz += 20;
        // ����� ����
        poz += printDateAdnTimeInJournal(poz);

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

	switch(key_) {
		case KEY_UP:
			if (sParam.jrnEntry.setPreviousEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY, GB_SEND_MAX);
				curCom_ = 1;
			}
			break;
		case KEY_DOWN:
			if (sParam.jrnEntry.setNextEntry()) {
                sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY, GB_SEND_MAX);
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

    // FIXME ��������� ������������ ������� ��� ���������� ��������!!!
	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}


/** ������� ����. ������ ������������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlJournalSecurity() {
    static const char title[] PROGMEM = "������\\������������";
#ifdef AVR
    static const char fcUserAndSource[] PROGMEM = "%S (%S)";
#else
    static const char fcUserAndSource[] PROGMEM = "%s (%s)";
#endif
    const eGB_COM comEntry = GB_COM_GET_JRN_IS_ENTRY;

    if (lvlCreate_) {
        lvlCreate_ = false;
        cursorEnable_ = false;
        lineParam_ = 1;
        curCom_ = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� �������� ������� � ������������� ���-�� ������� � ���
        sParam.jrnEntry.clear();
        sParam.jrnEntry.setCurrentDevice(GB_DEVICE_SEC);
        sParam.jrnEntry.setMaxNumJrnEntries(SIZE_OF_SECURITY_JRN);

        // ������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.setLocalCom(GB_COM_GET_JRN_IS_CNT);
        sParam.txComBuf.addCom2(comEntry);
    }

    // ����� ������� ������ � ������ � ������������ ���-�� �������
    uint16_t cur_entry = sParam.jrnEntry.getCurrentEntry();
    uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

    uint8_t poz = 0;
    // ����� �������� �������� ������ ����
    snprintf_P(&vLCDbuf[poz], 21, title);
    poz += 20;

    // ����� ����� ������� ������ � �� ���-��
    if (num_entries != 0) {
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
        // ������������ (�������� ������� ������������)
        user_t user = sParam.jrnEntry.getUser();
        userSrc_t usersrc = sParam.jrnEntry.getUserSrc();
        snprintf_P(&vLCDbuf[poz], ROW_LEN+1, fcUserAndSource, fcUser[user],
                   sParam.security.sevent.getUserSourceString(usersrc));
        poz += 20;

        poz += printDateAdnTimeInJournal(poz);

        // ����� �������
        TSecurityEvent::event_t uevent;
        uevent = static_cast<TSecurityEvent::event_t> (sParam.jrnEntry.getEvent());
        PGM_P event = sParam.security.sevent.getEventString(uevent);
        snprintf_P(&vLCDbuf[poz], ROW_LEN+1, event, uevent);
    }

    switch(key_) {
        case KEY_UP: {
            if (sParam.jrnEntry.setPreviousEntry()) {
                sParam.txComBuf.addFastCom(comEntry , GB_SEND_GET_ENTRY);
                curCom_ = 1;
            }
        } break;
        case KEY_DOWN: {
            if (sParam.jrnEntry.setNextEntry()) {
                sParam.txComBuf.addFastCom(comEntry , GB_SEND_GET_ENTRY);
                curCom_ = 1;
            }
        } break;

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
	static char punkt32[] PROGMEM = "%d. ����� ������. 1";
	static char punkt33[] PROGMEM = "%d. ����� ������. 2";
	static char punkt34[] PROGMEM = "%d. ����� ������. 3";
	static char punkt35[] PROGMEM = "%d. ����� ���������";
	static char punkt36[] PROGMEM = "%d. ����� ����";

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
			Punkts_.add(punkt07);// ����� ���������� � ����������� �� ��������
			// ����� ������ ���� �� ���� ��������� � ��������������
			Punkts_.add(punkt03);
			// TODO ��� ����
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
				Punkts_.add(punkt05);
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
					Punkts_.add(punkt30);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt23);
					Punkts_.add(punkt24);
					Punkts_.add(punkt27);
					Punkts_.add(punkt28);
					Punkts_.add(punkt30);
					Punkts_.add(punkt19);
					Punkts_.add(punkt16);
					Punkts_.add(punkt20);
					Punkts_.add(punkt10);
					Punkts_.add(punkt17);
				}
				Punkts_.add(punkt05);
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
				Punkts_.add(punkt05);
			} else if (compatibility == GB_COMPATIBILITY_LINER) {
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt04);
					Punkts_.add(punkt02);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt33);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt34);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt23);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt24);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt26);
				}
				Punkts_.add(punkt15);
				Punkts_.add(punkt16);
				Punkts_.add(punkt17);
				Punkts_.add(punkt05);
			}
		} else if (device == AVANT_RZSK) {
			eGB_NUM_DEVICES numDevices = sParam.def.getNumDevices();
			if (sParam.def.status.isEnable()) {
				Punkts_.add(punkt07);// ����� ���������� � ����������� �� ��������
				Punkts_.add(punkt03);
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt04);
					Punkts_.add(punkt02);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt22);
					Punkts_.add(punkt23);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt24);// ����� ���������� � ����������� �� ������
					Punkts_.add(punkt26);
				}
				Punkts_.add(punkt35);
				Punkts_.add(punkt05);
			} else {
				// ���� ��� �����
				Punkts_.add(punkt03);
				if (numDevices == GB_NUM_DEVICES_2) {
					Punkts_.add(punkt04);
				} else if (numDevices == GB_NUM_DEVICES_3) {
					Punkts_.add(punkt22);
				}
				Punkts_.add(punkt35);
				Punkts_.add(punkt05);
			}
		} else if (device == AVANT_K400) {
			Punkts_.add(punkt03);
			Punkts_.add(punkt35);
		} else if (device == AVANT_OPTO) {
			if (sParam.def.status.isEnable()) {
				Punkts_.add(punkt07);// ����� ���������� � ����������� �� ��������
			}
			Punkts_.add(punkt03);
			if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
				Punkts_.add(punkt04);
			} else {
				Punkts_.add(punkt36);
			}
			if (sParam.prd.status.isEnable() || sParam.prm.status.isEnable()) {
				Punkts_.add(punkt35);
			}
			if (sParam.def.status.isEnable()) {
				Punkts_.add(punkt02);
				if (!sParam.prd.status.isEnable() && !sParam.prm.status.isEnable()) {
					// "����� ��" ���� � �400 � ��� � ����
					Punkts_.add(punkt11);
				}
			}
		}

		// ������������� �������
		sParam.txComBuf.clear();
		if (sParam.typeDevice == AVANT_R400M) {
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
			if (sParam.def.status.isEnable()) {
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
		} else if (device == AVANT_R400M) {
			if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
				eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();
				uint8_t num = sParam.glb.getDeviceNum();
				if (comp == GB_COMPATIBILITY_AVANT) {
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
				} else if (comp == GB_COMPATIBILITY_PVZUE) {
					if (num == 1) {
						Punkts_.change(punkt24, GB_COM_NO, 2);
						Punkts_.change(punkt25, GB_COM_NO, 3);
						Punkts_.change(punkt28, GB_COM_NO, 5);
						Punkts_.change(punkt29, GB_COM_NO, 6);
					} else if (num == 2) {
						Punkts_.change(punkt23, GB_COM_NO, 2);
						Punkts_.change(punkt25, GB_COM_NO, 3);
						Punkts_.change(punkt27, GB_COM_NO, 5);
						Punkts_.change(punkt29, GB_COM_NO, 6);
					} else if (num == 3) {
						Punkts_.change(punkt23, GB_COM_NO, 2);
						Punkts_.change(punkt24, GB_COM_NO, 3);
						Punkts_.change(punkt27, GB_COM_NO, 5);
						Punkts_.change(punkt28, GB_COM_NO, 6);
					}
				} else if (comp == GB_COMPATIBILITY_LINER) {
					if (num == 1) {
						Punkts_.change(punkt33, GB_COM_NO, 2);
						Punkts_.change(punkt34, GB_COM_NO, 3);
						Punkts_.change(punkt24, GB_COM_NO, 4);
						Punkts_.change(punkt25, GB_COM_NO, 5);
					} else if (num == 2) {
						Punkts_.change(punkt32, GB_COM_NO, 2);
						Punkts_.change(punkt34, GB_COM_NO, 3);
						Punkts_.change(punkt23, GB_COM_NO, 4);
						Punkts_.change(punkt25, GB_COM_NO, 5);
					} else if (num == 3) {
						Punkts_.change(punkt32, GB_COM_NO, 2);
						Punkts_.change(punkt33, GB_COM_NO, 3);
						Punkts_.change(punkt23, GB_COM_NO, 4);
						Punkts_.change(punkt24, GB_COM_NO, 5);
					}
				}
			}
		}
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();

	switch(key_) {
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
            eGB_COM com = GB_COM_NO;
            eGB_SEND_TYPE type = GB_SEND_INT8;
            uint8_t signal = 0;

            if (name == punkt02) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_1;
            } else if (name == punkt03) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_SELF;
            } else if (name == punkt04) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_UD;
            } else if (name == punkt05) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_CALL;
            } else if (name == punkt06) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_ON;
            } else if (name == punkt07) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_OFF;
            } else if (name == punkt08) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_PUSK;
            } else if (name == punkt09) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_MAN_1;
            } else if (name == punkt10) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_PUSK_SELF;
            } else if (name == punkt11) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_AC;
            } else if (name == punkt12) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_PUSK_SELF;
            } else if (name == punkt13) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_AC_UD;
            } else if (name == punkt14) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_1;
            } else if (name == punkt15) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_AUTO_NORM;
            } else if (name == punkt16) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_FAST;
            } else if (name == punkt17) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_OFF;
            } else if (name == punkt18) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_PUSK_SELF;
            } else if (name == punkt19) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_AUTO_NORM;
            } else if (name == punkt20) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_CHECK;
            } else if (name == punkt22) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_UD;
            } else if (name == punkt23) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_1;
            } else if (name == punkt24) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_2;
            } else if (name == punkt25) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_3;
            } else if (name == punkt26) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_PUSK_UD_ALL;
            } else if (name == punkt27) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_MAN_1;
            } else if (name == punkt28) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_MAN_2;
            } else if (name == punkt29) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_MAN_3;
            } else if (name == punkt30) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_MAN_ALL;
            } else if (name == punkt31) {
                com = GB_COM_DEF_SET_TYPE_AC;
                signal = GB_TYPE_AC_AUTO_NORM;
            } else if (name == punkt32) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_UD_1;
            } else if (name == punkt33) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_UD_2;
            } else if (name == punkt34) {
                signal = GB_CONTROL_RESET_UD_3;
                com = GB_COM_SET_CONTROL;
            } else if (name == punkt35) {
                com = GB_COM_PRM_RES_IND;
                type = GB_SEND_NO_DATA;
            } else if (name == punkt36) {
                com = GB_COM_SET_CONTROL;
                signal = GB_CONTROL_RESET_UD;
            }

            if (sParam.txComBuf.addFastCom(com, type)) {
                sParam.txComBuf.setInt8(signal);
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
    static char punkt4[] PROGMEM = "%d. ������������";
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

	switch(key_) {
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
                lvlMenu = &clMenu::lvlUser;
                lvlCreate_ = true;
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
		sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
	}

    snprintf_P(&vLCDbuf[0], ROW_LEN + 1, title);

    if (isMessage()) {
        printMessage();
    } else {
        uint8_t poz = lineParam_ * ROW_LEN;
        if (sParam.def.status.isEnable()) {
            printDevicesRegime(poz, &sParam.def.status);
            poz += ROW_LEN;
        }
        if (sParam.prm.status.isEnable()) {
            printDevicesRegime(poz, &sParam.prm.status);
            poz += ROW_LEN;
        }
        if (sParam.prd.status.isEnable()) {
            printDevicesRegime(poz, &sParam.prd.status);
        }

        // ���� ������ �������� ���������.
        // ������� ���������� ��������� ����� ������.
        // ��� ������� ������� � ����� "�������" �� "������" ��� "�����",
        // ���������� ������ ������. ��� ��������� ������ ��������� ���������.
        if (EnterParam.isEnable()) {
            if ((this->*enterFunc)() == MENU_ENTER_PARAM_READY) {
                eGB_COM com = GB_COM_NO;

                eGB_REGIME_ENTER val = (eGB_REGIME_ENTER) EnterParam.getValue();
                if (val == GB_REGIME_ENTER_DISABLED) {
                    com = GB_COM_SET_REG_DISABLED;
                } else if (val == GB_REGIME_ENTER_ENABLED) {
                    com = GB_COM_SET_REG_ENABLED;
                }
                sParam.txComBuf.addFastCom(com, GB_SEND_NO_DATA);
                EnterParam.setDisable();
            }
        }
    }
	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER: {
            if (!sParam.security.checkUserAccess(USER_engineer, USER_SOURCE_pi)) {
                    setMessage(MSG_WRONG_USER);
            } else {
                uint8_t min = GB_REGIME_ENTER_DISABLED;
                uint8_t max = GB_REGIME_ENTER_DISABLED;
                uint8_t val = GB_REGIME_ENTER_DISABLED;

                // "������" 	-> "�������"
                // "�������" 	-> "������"
                // "�����" 		-> "������"	 / "�������"
                // ��������� 	-> "�������" / "������"
                enterFunc = &clMenu::inputValue;
                EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
                switch(reg) {
                    case GB_REGIME_ENABLED:
                        min = GB_REGIME_ENTER_DISABLED;
                        max = GB_REGIME_ENTER_DISABLED;
                        val = GB_REGIME_ENTER_DISABLED;
                        break;
                    case GB_REGIME_DISABLED:
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
                EnterParam.list = fcRegimeEnter[min];
                EnterParam.com = GB_COM_NO;
            }
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
	static char punkt5[] PROGMEM = "%d. ������";

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

		// � ������ ��� ������ ����������� ����� ���������
		if (sParam.glb.getTypeDevice() == AVANT_OPTO) {
			if (sParam.glb.getTypeOpto() != TYPE_OPTO_STANDART) {
				Punkts_.add(punkt5);
			}
		}

		// ������������� �������
		sParam.txComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 20, title);
	printPunkts();

	switch(key_) {
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
			} else if (name == punkt5) {
				lvlMenu = &clMenu::lvlSetupParamRing;
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

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();

		// ��� ���������������� ���� ����������� ������� ������:
		// ���-�� ��������� � �����
		sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);

		sParam.local.clearParams();
		if (device == AVANT_RZSK) {
			sParam.local.addParam(GB_PARAM_DEF_TYPE);
			sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
			sParam.local.addParam(GB_PARAM_OVERLAP);
			sParam.local.addParam(GB_PARAM_DELAY);
			sParam.local.addParam(GB_PARAM_WARN_THD_RZ);
			sParam.local.addParam(GB_PARAM_SENS_DEC_RZ);
			sParam.local.addParam(GB_PARAM_PRM_TYPE);
		} else if (device == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();
			// ��� ���������������� ���� ����������� ������� ������:
			// �������������
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
			sParam.local.addParam(GB_PARAM_DEF_TYPE);
			sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
			sParam.local.addParam(GB_PARAM_SHIFT_FRONT);
			sParam.local.addParam(GB_PARAM_SHIFT_BACK);
			sParam.local.addParam(GB_PARAM_SHIFT_PRM);
			sParam.local.addParam(GB_PARAM_SHIFT_PRD);
			sParam.local.addParam(GB_PARAM_SENS_DEC);
			if (comp == GB_COMPATIBILITY_AVANT) {
				// �������� ������ �� ���� ������ � ������������� �����
				sParam.local.addParam(GB_PARAM_AC_IN_DEC);
			}
			sParam.local.addParam(GB_PARAM_FREQ_PRD);
			sParam.local.addParam(GB_PARAM_FREQ_PRM);
		} else if (device == AVANT_OPTO) {
			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
			sParam.local.addParam(GB_PARAM_DEF_TYPE);
			sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
			sParam.local.addParam(GB_PARAM_OVERLAP_OPTO);
			sParam.local.addParam(GB_PARAM_DELAY_OPTO);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
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

/** ������� ����. ��������� ���������� ���������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamPrm() {
	static char title[] PROGMEM = "���������\\��������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();

		sParam.local.clearParams();

		uint8_t numcom = sParam.prm.getNumCom();
		if (device == AVANT_K400) {
			// ��� ���������������� ���� ������� ����� ���-�� ������
			sParam.txComBuf.addCom2(GB_COM_PRM_GET_COM);

			sParam.local.addParam(GB_PARAM_PRM_COM_NUMS);
            sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
			if (numcom != 0) {
				sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
				sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
			}
			sParam.local.addParam(GB_PARAM_PRM_DR_ENABLE);
			if (numcom != 0) {
				sParam.local.addParam(GB_PARAM_PRM_DR_COM_BLOCK);
				sParam.local.addParam(GB_PARAM_PRM_DR_COM_TO_HF);
			}
			sParam.local.addParam(GB_PARAM_PRM_TEST_COM);
			sParam.local.addParam(GB_PARAM_PRM_FREQ_CORR);
			sParam.local.addParam(GB_PARAM_PRM_COM_SIGNAL);
		} else if (device == AVANT_RZSK) {
			sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
			if (numcom != 0) {
				sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
				sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
				sParam.local.addParam(GB_PARAM_PRM_INC_SAFETY);
			}
		} else if (device == AVANT_OPTO) {
                sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
				if (numcom != 0) {
					sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
					sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
				}

			if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
				sParam.local.addParam(GB_PARAM_PRM_DR_ENABLE);
				if (numcom != 0) {
					sParam.local.addParam(GB_PARAM_PRM_DR_COM_BLOCK);
					sParam.local.addParam(GB_PARAM_PRM_DR_COM_TO_HF);
				}
				sParam.local.addParam(GB_PARAM_PRM_COM_SIGNAL);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
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

/** ������� ����. ��������� ���������� �����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamPrd() {
	static char title[] PROGMEM = "���������\\����������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();

		sParam.local.clearParams();
		uint8_t numcom = sParam.prd.getNumCom();
		if (device == AVANT_K400) {
			// ��� ���������������� ���� ������� ����� ���-�� ������
			sParam.txComBuf.addCom2(GB_COM_PRD_GET_COM);
			// �������������
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			sParam.local.addParam(GB_PARAM_PRD_COM_NUMS);
			sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
			sParam.local.addParam(GB_PARAM_PRD_DURATION_L);
			sParam.local.addParam(GB_PARAM_PRD_TEST_COM);
			if (numcom != 0) {
				sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
				sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);
				sParam.local.addParam(GB_PARAM_PRD_COM_NUMS_A);
			}
			sParam.local.addParam(GB_PARAM_PRD_DR_ENABLE);
			if (numcom != 0) {
				sParam.local.addParam(GB_PARAM_PRD_DR_COM_BLOCK);
			}
			sParam.local.addParam(GB_PARAM_PRD_FREQ_CORR);

			sParam.local.addParam(GB_PARAM_PRD_DEC_CF);
			sParam.local.addParam(GB_PARAM_PRD_DEC_TM);
			sParam.local.addParam(GB_PARAM_PRD_COM_SIGNAL);
		} else if (device == AVANT_RZSK) {
			sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
			sParam.local.addParam(GB_PARAM_PRD_DURATION_L);
			sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
			sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);
		} else if (device == AVANT_OPTO) {
				sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
				sParam.local.addParam(GB_PARAM_PRD_DURATION_O);
				sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
				sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);

			if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
				sParam.local.addParam(GB_PARAM_PRD_DR_ENABLE);
				if (numcom != 0) {
					sParam.local.addParam(GB_PARAM_PRD_DR_COM_BLOCK);
				}
				sParam.local.addParam(GB_PARAM_PRD_COM_SIGNAL);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
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

/** ������� ����. ��������� ���������� �����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamGlb() {
	static char title[] PROGMEM = "���������\\�����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		sParam.txComBuf.clear();

		sParam.local.clearParams();
		if (device == AVANT_K400) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
			sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

			sParam.local.addParam(GB_PARAM_COMP_K400);
			sParam.local.addParam(GB_PARAM_TIME_SYNCH);
			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
//			TODO �� ������ ������ � �� ��� �������� ��������� ������� �� ����������� � ���. ������� �������� ���� ������!
//			if (sParam.prd.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_OUT_CHECK);
//			}
			if (sParam.prm.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_WARN_THD);
				sParam.local.addParam(GB_PARAM_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
				sParam.local.addParam(GB_PARAM_IN_DEC);
			}
			sParam.local.addParam(GB_PARAM_FREQ);
			if (sParam.prd.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_COR_U);
				sParam.local.addParam(GB_PARAM_COR_I);
			}
			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
			sParam.local.addParam(GB_PARAM_TM_K400);
			sParam.local.addParam(GB_PARAM_WARN_D);
			sParam.local.addParam(GB_PARAM_ALARM_D);
			sParam.local.addParam(GB_PARAM_TEMP_MONITOR);
			sParam.local.addParam(GB_PARAM_TEMP_THR_HI);
			sParam.local.addParam(GB_PARAM_TEMP_THR_LOW);

			eGB_COMP_K400 comp = sParam.glb.getCompK400();
			if ((comp == GB_COMP_K400_UPKC_PRD)
					|| (comp == GB_COMP_K400_UPKC_PRM)) {
				sParam.local.addParam(GB_PARAM_TM_SPEED);
			}
		} else if (device == AVANT_RZSK) {
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);

			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
			sParam.local.addParam(GB_PARAM_TIME_SYNCH);
			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
			sParam.local.addParam(GB_PARAM_OUT_CHECK);
			sParam.local.addParam(GB_PARAM_WARN_THD_CF);
			sParam.local.addParam(GB_PARAM_TIME_RERUN);
			sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
			sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
			sParam.local.addParam(GB_PARAM_IN_DEC);
			sParam.local.addParam(GB_PARAM_FREQ);
			sParam.local.addParam(GB_PARAM_DETECTOR);
			sParam.local.addParam(GB_PARAM_COR_U);
			sParam.local.addParam(GB_PARAM_COR_I);
		} else if (device == AVANT_R400M) {
			eGB_COMPATIBILITY comp = sParam.glb.getCompatibility();

			// ��� ���������������� ���� ����������� ������� ������:
			// ������������� � ���-�� ��������� � �����
			// ���������� ��������� ��� ���������
			sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
			sParam.local.addParam(GB_PARAM_COMP_P400);
			if (comp == GB_COMPATIBILITY_AVANT) {
				sParam.local.addParam(GB_PARAM_TIME_SYNCH);
			}
			sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
			sParam.local.addParam(GB_PARAM_OUT_CHECK);
			sParam.local.addParam(GB_PARAM_WARN_THD);
			sParam.local.addParam(GB_PARAM_U_OUT_NOM);
			sParam.local.addParam(GB_PARAM_FREQ);
			if (comp == GB_COMPATIBILITY_PVZL) {
				sParam.local.addParam(GB_PARAM_IN_DEC_AC_ANSWER);
			}
			sParam.local.addParam(GB_PARAM_COR_U);
			sParam.local.addParam(GB_PARAM_COR_I);
			if (comp == GB_COMPATIBILITY_PVZUE) {
				sParam.local.addParam(GB_PARAM_PVZUE_PROTOCOL);
				sParam.local.addParam(GB_PARAM_PVZUE_PARITY);
				sParam.local.addParam(GB_PARAM_PVZUE_FAIL);
				sParam.local.addParam(GB_PARAM_PVZUE_NOISE_THD);
				sParam.local.addParam(GB_PARAM_PVZUE_NOISE_LVL);
				sParam.local.addParam(GB_PARAM_PVZUE_AC_TYPE);
				sParam.local.addParam(GB_PARAM_PVZUE_AC_PERIOD);
				sParam.local.addParam(GB_PARAM_PVZUE_AC_PER_RE);
			}
		} else if (device == AVANT_OPTO) {
			sParam.local.addParam(GB_PARAM_TIME_SYNCH);
			if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
				sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
			} else {
				sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE_RING);
			}
			if (sParam.prm.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_TIME_RERUN);
			}
			if (sParam.prd.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
			}
			if (sParam.prm.status.isEnable()) {
				sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
			}
			if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
				sParam.local.addParam(GB_PARAM_BACKUP);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
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

/** ������� ����. ��������� ���������� ������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupParamRing() {
	static char title[] PROGMEM = "���������\\������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ���������� �������� ���������� � ������
		eGB_TYPE_DEVICE device = sParam.typeDevice;
		eGB_TYPE_OPTO topto = sParam.glb.getTypeOpto();
		sParam.txComBuf.clear();

		sParam.local.clearParams();
		if (device == AVANT_OPTO) {
			if (topto == TYPE_OPTO_RING_UNI) {
				sParam.local.addParam(GB_PARAM_RING_TIME_WAIT);
				sParam.local.addParam(GB_PARAM_RING_COM_TRANSIT);
				sParam.local.addParam(GB_PARAM_RING_COM_REC);
				sParam.local.addParam(GB_PARAM_RING_COM_TR);
			}
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

	// ����� �� ������� ������ ����, ���� ��� �� ������ ������
	if (sParam.glb.getTypeDevice() != AVANT_OPTO) {
			key_ = KEY_CANCEL;
	} else if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART) {
		key_ = KEY_CANCEL;
	}

	switch(key_) {
		case KEY_CANCEL:
			lvlMenu = &clMenu::lvlSetupParam;
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
    static TInterface::INTERFACE interface = TInterface::MAX;

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

        sParam.txComBuf.clear();

		// ���� ����������� ����� �� ��������� ����
		// ���������� ��������� ������
		// � USB ������: 19200 ���/�, 8 ���, 2 ����-����, ��������-���
        sParam.local.clearParams();
		sParam.local.addParam(GB_PARAM_INTF_INTERFACE);

        interface = sParam.Uart.Interface.get();
        if (interface == TInterface::RS485) {
			sParam.local.addParam(GB_PARAM_INTF_PROTOCOL);
			// � ������ ������� ����� �������� � ����� ������,
			// ����� ������ � ��������
			sParam.local.addParam(GB_PARAM_NET_ADDRESS);
			sParam.local.addParam(GB_PARAM_INTF_BAUDRATE);
			sParam.local.addParam(GB_PARAM_INTF_DATA_BITS);
			sParam.local.addParam(GB_PARAM_INTF_PARITY);
			sParam.local.addParam(GB_PARAM_INTF_STOP_BITS);
        }
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	setupParam();

    // FIXME ��� ����� ���������� �� ������ ���������� "������..."

    if (interface != sParam.Uart.Interface.get()) {
        lvlCreate_ = true;
    }

	switch(key_) {
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
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[20], 21, title);

    printMeasParam(0, MENU_MEAS_PARAM_DATE);
    printMeasParam(1, MENU_MEAS_PARAM_TIME);

    if (isMessage()) {
        printMessage();
    } else if (EnterParam.isEnable()) {
		// ����� �������� ������
		snprintf_P(&vLCDbuf[20 * lineParam_], 21, name, cursorLine_);
        eMENU_ENTER_PARAM stat = inputValue();

		if (stat == MENU_ENTER_PARAM_READY) {           
            sParam.txComBuf.addFastCom(EnterParam.com, GB_SEND_TIME);

            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getYear()), 0);
            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMonth()), 1);
            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getDay()), 2);
            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getHour()), 3);
            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMinute()), 4);
            sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getSecond()), 5);
            sParam.txComBuf.setInt8(0, 6);	// �� ������ 0
            sParam.txComBuf.setInt8(0, 7);	//
            sParam.txComBuf.setInt8(0, 8);    // 0 - ��������� � ����, 1 - � �����

            uint8_t val = static_cast<uint8_t> (EnterParam.getValue());
            uint8_t pos = static_cast<uint8_t> (EnterParam.getDopValue());
            sParam.txComBuf.setInt8(BIN_TO_BCD(val), pos);

            if (pos == 0) {
                // ���� ����, �������� ����, �.�. ����� ���� ����������
                uint8_t month = BCD_TO_BIN(sParam.txComBuf.getUInt8(1));
                uint8_t day = BCD_TO_BIN(sParam.txComBuf.getUInt8(2));
                if (day > sParam.DateTime.getNumDaysInMonth(month, val)) {
                    sParam.txComBuf.setInt8(BIN_TO_BCD(1), 2);
                }
            } else if (pos == 1) {
                // ���� ������, �������� ���-�� ������������� ����
                uint8_t day = BCD_TO_BIN(sParam.txComBuf.getUInt8(2));
                if (day > sParam.DateTime.getNumDaysInMonth(val)) {
                    sParam.txComBuf.setInt8(BIN_TO_BCD(1), 2);
                }
            }

			EnterParam.setDisable();
		}
    } else {
		printPunkts();
    }


	switch(key_) {
        case KEY_UP: {
			cursorLineUp();
        } break;
        case KEY_DOWN: {
			cursorLineDown();
        } break;

        case KEY_ENTER: {
            if (!sParam.security.checkUserAccess(USER_engineer, USER_SOURCE_pi)) {
                setMessage(MSG_WRONG_USER);
            } else {
                enterFunc = &clMenu::inputValue;
                if (name == punkt1) {
                    EnterParam.setEnable();
                    EnterParam.setValueRange(0, 99);
                    EnterParam.setValue(sParam.DateTime.getYear());
                    EnterParam.setDopValue(0);
                } else if (name == punkt2) {
                    EnterParam.setEnable();
                    EnterParam.setValueRange(1, 12);
                    EnterParam.setValue(sParam.DateTime.getMonth());
                    EnterParam.setDopValue(1);
                } else if (name == punkt3) {
                    EnterParam.setEnable();
                    uint8_t max = sParam.DateTime.getNumDaysInMonth();
                    EnterParam.setValueRange(1, max);
                    EnterParam.setValue(sParam.DateTime.getDay());
                    EnterParam.setDopValue(2);
                } else if (name == punkt4) {
                    EnterParam.setEnable();
                    EnterParam.setValueRange(0, 23);
                    EnterParam.setValue(sParam.DateTime.getHour());
                    EnterParam.setDopValue(3);
                } else if (name == punkt5) {
                    EnterParam.setEnable();
                    EnterParam.setValueRange(0, 59);
                    EnterParam.setValue(sParam.DateTime.getMinute());
                    EnterParam.setDopValue(4);
                } else if (name == punkt6) {
                    EnterParam.setEnable();
                    EnterParam.setValueRange(0, 59);
                    EnterParam.setValue(sParam.DateTime.getSecond());
                    EnterParam.setDopValue(5);
                }
                EnterParam.com = GB_COM_SET_TIME;
            }
        } break;

        case KEY_CANCEL: {
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
        } break;
        case KEY_MENU: {
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
        } break;

		default:
			break;
	}
}

/**	������� ����. ���������.
 *
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlMeasure() {
	static char title[] PROGMEM = "����\\���������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;
		cursorLine_ = 0;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ������������� �������
		// ����������� ������ �������� (�� ������ ������������)
		sParam.txComBuf.clear();
		sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
		// 2 ������� ��������� ��� ���������� ������� ������ ���������� ����������
		sParam.txComBuf.addCom2(GB_COM_GET_FAULT);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < MAX_NUM_MEAS_PARAM_LVL; i++) {
		printMeasParam(lineParam_*2 + i, measParamLvl[i]);
	}

	switch(key_) {
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

/** ������� ����. ���� 1.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlTest() {
	static char title[] PROGMEM = "���������\\�����";
	static char punkt1[] PROGMEM = "%d. ���� �����������";
	static char punkt2[] PROGMEM = "%d. ���� ���������";
//	static char message[][21] PROGMEM = {
//	//12345678901234567890
//			"    ��������� �     ",//
//			"   ����� �������    " 		//
//			};

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
        sParam.txComBuf.setLocalCom(GB_COM_GET_COM_PRD_KEEP);
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 20, title);

    if (isMessage()) {
        printMessage();
    } else {
        printPunkts();
    }

	eGB_REGIME reg = sParam.glb.status.getRegime();
	switch(key_) {
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
                    sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_1, GB_SEND_NO_DATA);
					lvlMenu = &clMenu::lvlTest1;
					lvlCreate_ = true;
				} else if (reg == GB_REGIME_TEST_1) {
					lvlMenu = &clMenu::lvlTest1;
					lvlCreate_ = true;
                } else {
                    setMessage(MSG_DISABLE);
                }
			} else if (name == punkt2) {
				if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_1)) {
                    sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_2, GB_SEND_NO_DATA);
					lvlMenu = &clMenu::lvlTest2;
					lvlCreate_ = true;
				} else if (reg == GB_REGIME_TEST_2) {
					lvlMenu = &clMenu::lvlTest2;
					lvlCreate_ = true;
                } else {
                    setMessage(MSG_DISABLE);
                }
			}
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
        if (device != AVANT_OPTO) {
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);	// ���������
        }
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// �������

		// ������� ��� ������
		sParam.test.clear();
		if (device == AVANT_R400M) {
			sParam.test.addSignalToList(GB_SIGNAL_RZ);
			if (sParam.glb.getCompatibility() == GB_COMPATIBILITY_AVANT) {
				sParam.test.addSignalToList(GB_SIGNAL_CF);
			} else if (sParam.glb.getCompatibility()
					== GB_COMPATIBILITY_LINER) {
				sParam.test.addSignalToList(GB_SIGNAL_CF1);
				sParam.test.addSignalToList(GB_SIGNAL_CF2);
				sParam.test.addSignalToList(GB_SIGNAL_CF3);
				sParam.test.addSignalToList(GB_SIGNAL_CF4);
			}
		} else if (device == AVANT_RZSK) {
			if (sParam.def.status.isEnable()) {
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
			} else {
				sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
				sParam.test.addSignalToList(GB_SIGNAL_CF2_NO_RZ);
				sParam.test.addSignalToList(GB_SIGNAL_COM1_NO_RZ);
				sParam.test.addSignalToList(GB_SIGNAL_COM2_NO_RZ);
				sParam.test.addSignalToList(GB_SIGNAL_COM3_NO_RZ);
				sParam.test.addSignalToList(GB_SIGNAL_COM4_NO_RZ);
			}
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
			sParam.test.addSignalToList(GB_SIGNAL_CS);
			if (sParam.def.status.isEnable()) {
				sParam.test.addSignalToList(GB_SIGNAL_RZ);
			}

			if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI) {
				for (uint_fast8_t i = 0; i < MAX_NUM_COM_RING; i++) {
					eGB_TEST_SIGNAL signal =
							(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1A + i);
					sParam.test.addSignalToList(signal);
				}
			} else {
				uint8_t num_com = sParam.prd.getNumCom();
				for (uint_fast8_t i = 0; i < num_com; i++) {
					eGB_TEST_SIGNAL signal =
							(eGB_TEST_SIGNAL) ((uint8_t) GB_SIGNAL_COM1 + i);
					sParam.test.addSignalToList(signal);
				}
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

	// ����� �������� �����, ��� ������ �������� ������� �������
	// ����� �� ����� ���� ������� ��� ������� ���������
	sParam.txComBuf.setInt8(0, 0);
	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
        eMENU_ENTER_PARAM stat = inputValue();

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
			sParam.txComBuf.setInt8(2, 0);				// ������ ��
			sParam.txComBuf.setInt8(rz, 1);				// ������� ������ ��
            sParam.txComBuf.addFastCom(EnterParam.com, GB_SEND_MAX);
			// ��
			sParam.txComBuf.setInt8(1, 0);				// ������ ��
			sParam.txComBuf.setInt8(cf, 1);				// ������� ������ ��
            sParam.txComBuf.addFastCom(EnterParam.com, GB_SEND_MAX);

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

	switch(key_) {
		case KEY_CANCEL:
            sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED, GB_SEND_NO_DATA);
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
	static char title[] PROGMEM = "�����\\��������";
	static char punkt1[] PROGMEM = "������� ���������";
	static char prm1[] PROGMEM = "���1: ";
	static char prm2[] PROGMEM = "���2: ";

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
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);	// ���������
		}
		sParam.txComBuf.addCom2(GB_COM_GET_TEST);	// �������
	}

	// ����� �� ����� ���������� ����������, ���� ��� �� ������
	if (device != AVANT_OPTO) {
		if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3) {
			if (sParam.typeDevice == AVANT_R400M) {
				printMeasParam(2, MENU_MEAS_PARAM_UC1);
				printMeasParam(3, MENU_MEAS_PARAM_UZ);
				printMeasParam(4, MENU_MEAS_PARAM_UC2);
			} else if (sParam.def.status.isEnable()) {
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
	// � �400� ������ ���� ��������, �� �������� �� ���-�� ��������� � �����
	if ((sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
			&& (sParam.typeDevice != AVANT_R400M)) {
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

	switch(key_) {
		case KEY_CANCEL:
            sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED, GB_SEND_NO_DATA);
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

/** ������� ����. ������������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlUser() {
    static char title[] PROGMEM = "����\\������������";

    if (lvlCreate_) {
        lvlCreate_ = false;
        cursorLine_ = 1;
        cursorEnable_ = true;
        lineParam_ = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // �������� �������!
        sParam.txComBuf.clear();

        sParam.local.clearParams();
        sParam.local.addParam(GB_PARAM_IS_USER);
        sParam.local.addParam(GB_PARAM_IS_PWD_ENGINEER);
        sParam.local.addParam(GB_PARAM_IS_PWD_ADMIN);
        sParam.local.addParam(GB_PARAM_IS_RESET_PWD);

        sParam.txComBuf.setLocalCom(GB_COM_GET_NET_ADR);
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    LocalParams *lp = &sParam.local;
    if (getCom(lp->getParam()) == GB_COM_NO) {
        eGB_PARAM param = lp->getParam();
        if (param == GB_PARAM_IS_USER) {
            lp->setValue(sParam.security.getUser(USER_SOURCE_pi));
        } else if (param == GB_PARAM_IS_RESET_PWD) {
            lp->setValue(0);
        }
    }

    setupParam();

    switch(key_) {
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

        default:
            break;
    }
}

//
eMENU_ENTER_PARAM clMenu::inputValue() {
    static char strInput[] PROGMEM = "����: ";
    static char strInputPwd[] PROGMEM = "������: ";
#ifdef AVR
    static char strListValue_P[] PROGMEM = "%S";
#else
    static char strListValue_P[] PROGMEM = "%s";
#endif
    static char strIntValue1[] PROGMEM = "%01d";
    static char strIntValue2[] PROGMEM = "%02d";
    static char strIntValue3[] PROGMEM = "%03d";
    static char strIntValue4[] PROGMEM = "%04d";
    static char strStrValue[] PROGMEM = "%s";
    static char strFloatValue[] PROGMEM = "%01u.%01u";

    eMENU_ENTER_PARAM status = EnterParam.getStatus();
    uint8_t posstart = 5*ROW_LEN;
    uint8_t posstop = posstart;

    clearLine(NUM_TEXT_LINES);

    if (status == MENU_ENTER_PASSWORD) {
        posstart += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strInputPwd);
    } else {
        posstart += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strInput);
    }
    posstop = posstart;

    // FIXME ��������� �������� ������ �� ������� ������!
	if (status == MENU_ENTER_PARAM_INT) {
        PGM_P vstr = NULL;
        switch(EnterParam.getDigitMax()) {
            case 1: {
                vstr = strIntValue1;
            } break;
            case 2: {
                vstr = strIntValue2;
            } break;
            case 3: {
                vstr = strIntValue3;
            } break;
            case 4: {
                vstr = strIntValue4;
            } break;
        }
        if (vstr != NULL) {
            int16_t val = EnterParam.getValue();
            posstop += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, vstr, val);
        } else {
            key_ = KEY_CANCEL;
//            qDebug() << "Do not find input string for parameter: " <<
//                        getNameOfParam(EnterParam.getParam());
        }
	} else if (status == MENU_ENTER_PARAM_U_COR) {
		uint16_t val = EnterParam.getValue();
        posstop += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strFloatValue,
                              val/10, val%10);
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = EnterParam.getValue();
		val = (val - EnterParam.getValueMin()) * STRING_LENGHT;
        posstop += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strListValue_P,
                              EnterParam.list + val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = EnterParam.listValue[EnterParam.getValue()];
        posstop += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strListValue_P,
				EnterParam.list + STRING_LENGHT * val);
    } else if (status == MENU_ENTER_PASSWORD) {
        uint8_t *val = EnterParam.getValuePwd();
        posstop += snprintf_P(&vLCDbuf[posstart], ROW_LEN+1, strStrValue, val);
        uint8_t pos = posstop - EnterParam.getDigit();
        for(uint8_t i = posstart; i < posstop; i++) {
            if (i != pos) {
                vLCDbuf[i] = '*';
            }
        }
    } else {
		key_ = KEY_CANCEL;
    }

	switch(key_) {
        case KEY_UP: {
			EnterParam.incValue(timePressKey());
        } break;
        case KEY_DOWN: {
			EnterParam.decValue(timePressKey());
        } break;
        case KEY_LEFT: {
            EnterParam.incDigit();
        } break;
        case KEY_RIGHT: {
            EnterParam.decDigit();
        } break;
        case KEY_CANCEL: {
            EnterParam.setDisable();
        } break;
		case KEY_ENTER:
			EnterParam.setEnterValueReady();
			break;
		default:
			break;
	}

    printCursor(posstart, posstop - 1);
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
        snprintf_P(&vLCDbuf[ROW_LEN * line], ROW_LEN + 1,
                Punkts_.getName(cntPunkts),	cntPunkts + 1);

		if (++cntPunkts >= Punkts_.getMaxNumPunkts())
			break;
	}

	// ��� ��������������, ����� ������� �� �����
	if (cursorEnable_) {
		if (cursorLine_ > numLines)
            vLCDbuf[ROW_LEN * (NUM_TEXT_LINES - 1) + 2] = '*';
		else {
            vLCDbuf[ROW_LEN * (cursorLine_ + lineParam_ - 1) + 2] = '*';
		}
	}
}

/**	����� � ��������� ����� ������������� ���������.
 * 	� ����� ������ ��������� ��� ���������.
 * 	@param poz ������� �������
 * 	@arg [0, 12), 0 ������ ������ �����, 11 - ������� ������
 * 	@param par ������������ ��������
 * 	@return ���
 */
void clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par) {
	static const char fcUout[] 	PROGMEM = "U=%02u.%01u�";	// ���������� ������.
	static const char fcIout[] 	PROGMEM = "I=%03u��";		// ��� ������.
	static const char fcRout[] 	PROGMEM = "R=%03u��";		// ������������� �����.
	static const char fcUz[] 	PROGMEM = "U�=%02d��";		// ����� �� ������.
	static const char fcUz1[] 	PROGMEM = "U�1=%02d��";		// ����� �� ������ 1.
	static const char fcUz2[] 	PROGMEM = "U�2=%02d��";		// ����� �� ������ 2.
	static const char fcUcf[] 	PROGMEM = "U�=%02d��";		// ����� �� �C.
	static const char fcUcf1[] 	PROGMEM = "U�1=%02d��";		// ����� �� �C 1.
	static const char fcUcf2[] 	PROGMEM = "U�2=%02d��";		// ����� �� �C 2.
	static const char fcUn[] 	PROGMEM = "U�=%02d��";		// ������� �����.
	static const char fcUn1[] 	PROGMEM = "U�1=%02d��";		// ������� ����� 1.
	static const char fcUn2[] 	PROGMEM = "U�2=%02d��";		// ������� ����� 2.
	static const char fcSd[] 	PROGMEM = "S�=%02u�";		// �������� � �������.
	static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";	// ����.
	static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";	// �����.
	static const char fcD[]		PROGMEM = "D=%02d��";		// ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
	static const char fcTemper[] PROGMEM= "T=%02d�C";		// �����������
	static const char fcFreqDev[] PROGMEM = "dF=%02d��";		// ���������� ������ �� �� ���

	// �������� �� ������������ �������
		// 10 - ���-�� �������� ���������� �� ������ ��� 1 ��������
	if (poz < 12) {
		poz = (poz * 10) + 1;

		switch(par) {
			case MENU_MEAS_PARAM_DATE:
				snprintf_P(&vLCDbuf[poz], 11, fcDate, sParam.DateTime.getDay(),
						sParam.DateTime.getMonth(), sParam.DateTime.getYear());
				break;
			case MENU_MEAS_PARAM_TIME:
				snprintf_P(&vLCDbuf[poz], 11, fcTime, sParam.DateTime.getHour(),
						sParam.DateTime.getMinute(),
						sParam.DateTime.getSecond());
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
				break;

			case MENU_MEAS_PARAM_D:
				snprintf_P(&vLCDbuf[poz], 11, fcD, sParam.measParam.getD());
				break;

			case MENU_MEAS_PARAM_TEMPERATURE:
				snprintf_P(&vLCDbuf[poz], 11, fcTemper,
						sParam.measParam.getTemperature());
				break;

			case MENU_MEAS_PARAM_DF:
				snprintf_P(&vLCDbuf[poz], 11, fcFreqDev,
						sParam.measParam.getFreqDev());
				break;

			case MENU_MEAS_PARAM_NO:
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
	static const char fcFaults[] 	PROGMEM = "������. %c-%04X";
	static const char fcWarnings[] 	PROGMEM = "�������. %c-%04X";

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
			y = device->getRemoteNumber();
			snprintf_P(&vLCDbuf[poz], 17, text[x], fcRemoteNum[y]);
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
			y = device->getRemoteNumber();
			snprintf_P(&vLCDbuf[poz], 17, text[x], fcRemoteNum[y]);
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

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printParam() {
    snprintf_P(&vLCDbuf[20], ROW_LEN + 1, PSTR("��������:%u �����:%u"),
			sParam.local.getNumOfCurrParam(), sParam.local.getNumOfParams());

    snprintf_P(&vLCDbuf[40], ROW_LEN + 1,
            getNameOfParam(sParam.local.getParam()));

	printSameNumber(60);
	printRange(80);
	printValue(100);
}

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printSameNumber(uint8_t pos) {
	if (sParam.local.getNumOfSameParams() > 1) {
		eGB_PARAM p = sParam.local.getParam();
		uint8_t val = sParam.local.getNumOfCurrSameParam();
		uint8_t max = sParam.local.getNumOfSameParams();

		if (p == GB_PARAM_RING_COM_TRANSIT) {
			// ��� ���������� ������ ������ ������ ��������� �������� ���� 16A/32C
			// ������ � ������� �������� �������������� ����� �������� GB_PARAM_RING_COM_REC
			Param* param = (Param*) pgm_read_word(&fParams[GB_PARAM_RING_COM_REC]);
			PGM_P pval =  (PGM_P) pgm_read_word(&param->listValues) + (val * STRING_LENGHT);
			PGM_P pmax =  (PGM_P) pgm_read_word(&param->listValues) + (max * STRING_LENGHT);
#ifdef AVR
            snprintf_P(&vLCDbuf[pos], ROW_LEN + 1, PSTR("�����: %S/%S"), pval, pmax);
#else
            snprintf_P(&vLCDbuf[pos], ROW_LEN + 1, PSTR("�����: %s/%s"), pval, pmax);
#endif
        } else {
            snprintf_P(&vLCDbuf[pos], ROW_LEN + 1, PSTR("�����: %u/%u"), val, max);
		}
	}
}

//	����� �� ����� ��������� �������� ���������.
void clMenu::printRange(uint8_t pos) {
    static prog_uint8_t MAX_CHARS = 11 ;

	LocalParams *lp = &sParam.local;
    int16_t min = getMin(lp->getParam());
	int16_t max = lp->getMax();
	PGM_P str = fcNullBuf;

	pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));

    switch(getRangeType(lp->getParam())) {
        case Param::RANGE_LIST: {
			str = PSTR("������");
        } break;

        case Param::RANGE_ON_OFF: {
			str = PSTR("���./����.");
        } break;

        case Param::RANGE_INT: {
#ifdef AVR
			str = PSTR("%d..%d%S");
#else
            str = PSTR("%d..%d%s");
#endif
        } break;

        case Param::RANGE_INT_NO_DIM: {
			str = PSTR("%d..%d");
        } break;

        case Param::RANGE_U_COR: {
			min = 0;
			max /= 10;
#ifdef AVR
			str = PSTR("%d..�%d%S");
#else
            str = PSTR("%d..�%d%s");
#endif
        } break;

        case Param::RANGE_I_COR: {
			min = 0;
#ifdef AVR
			str= PSTR("%d..�%d%S");
#else
            str= PSTR("%d..�%d%s");
#endif
        } break;

        case Param::RANGE_PWD: {
            COMPILE_TIME_ASSERT(PWD_LEN == 8);
            str = PSTR("99999999");
        } break;
	}

    PGM_P dim = fcDimension[getDim(lp->getParam())];
    snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, min, max, dim);
}

void clMenu::printCursor(uint8_t start, uint8_t stop) {
    static bool blink = false;
    uint8_t pos = SIZE_BUF_STRING;
    uint8_t shift = EnterParam.getDigit() - 1;

    blink = !blink;
    if (blink) {
        switch(EnterParam.getStatus()) {
            case MENU_ENTER_PARAM_INT: // DOWN
            case MENU_ENTER_PASSWORD: {
                pos = stop - shift;
            } break;
            case MENU_ENTER_PARAM_LIST: {
                pos = start;
            } break;

            case MENU_ENTER_PARAM_NO:
                break;
        }
    }

    if (pos != SIZE_BUF_STRING) {
        vLCDbuf[pos] = '_';
    }


}

// ����� �� ����� �������� �������� ���������.
void clMenu::printValue(uint8_t pos) {
	static prog_uint8_t MAX_CHARS = 11;

	int16_t val = sParam.local.getValue();
    PGM_P dim = fcDimension[getDim(sParam.local.getParam())];
	PGM_P str = fcNullBuf;

	pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));

	LocalParams::STATE state = sParam.local.getState();

	if (state == LocalParams::STATE_ERROR) {
		// ����� ���������� ��������
		if (blink_) {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������!!!"));
		} else {
			if (getParamType(sParam.local.getParam()) == Param::PARAM_PWD) {
				for(uint8_t i = 0; i < sParam.local.getMax(); i++) {
					vLCDbuf[pos++] = '*';
				}
			} else {
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("%d"), val);
			}
		}
	} else if (state == LocalParams::STATE_READ_PARAM) {
		if (blink_) {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������."));
		} else {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������.."));
		}
	} else {
		// ����� ����������� ��������
        switch(getParamType(sParam.local.getParam())) {
			case Param::PARAM_BITES: // DOWN
            case Param::PARAM_LIST: {
                val -= getMin(sParam.local.getParam());
                str = getListOfValues(sParam.local.getParam()) + (val * STRING_LENGHT);
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str);
            } break;
			case Param::PARAM_I_COR: // DOWN
            case Param::PARAM_INT: {
#ifdef AVR
				str = PSTR("%d%S");
#else
                str = PSTR("%d%s");
#endif
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, val, dim);
            } break;
            case Param::PARAM_U_COR: {
				if (val >= 0) {
#ifdef AVR
					str = PSTR("%d.%d%S");
#else
                    str = PSTR("%d.%d%s");
#endif
				} else {
					val = -val;
#ifdef AVR
					str = PSTR("-%d.%d%S");
#else
                    str = PSTR("-%d.%d%s");
#endif
				}
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, val / 10, val % 10,
						dim);
            } break;
            case Param::PARAM_PWD: {
                for(uint8_t i = 0; i < sParam.local.getMax(); i++) {
                    vLCDbuf[pos++] = '*';
                }
            } break;
            case Param::PARAM_NO: {
            } break;
		}
    }
}

//
uint8_t
clMenu::printDateAdnTimeInJournal(uint8_t pos) {
    uint8_t startpos = pos;

    // ����� ����
    snprintf_P(&vLCDbuf[pos], ROW_LEN + 1, fcDateJrnBCD,
               sParam.jrnEntry.getDay(),
               sParam.jrnEntry.getMonth(),
               sParam.jrnEntry.getYear());
    pos += 20;

    // ����� �������
    snprintf_P(&vLCDbuf[pos], ROW_LEN + 1, fcTimeJrnBCD,
               sParam.jrnEntry.getHour(),
               sParam.jrnEntry.getMinute(),
               sParam.jrnEntry.getSecond(),
               sParam.jrnEntry.getMSecond());
    pos += 20;

    return pos - startpos;
}



//
bool clMenu::checkChangeReg() const {
    bool check = false;
    eGB_REGIME regime = sParam.glb.status.getRegime();
    Param::CHANGE_REG changereg = getChangeReg(sParam.local.getParam());

    switch(changereg) {
        case Param::CHANGE_REG_NO: {
            check = true;
        } break;
        case Param::CHANGE_REG_DISABLE: {
            check = (regime == GB_REGIME_DISABLED);
        } break;
    }

    return check;
}

//
bool clMenu::checkPwdInput(user_t user, const uint8_t *pwd) {
    bool check = false;

    check = sParam.security.pwd.checkPassword(user, pwd);
    if (!check) {
        if (sParam.security.pwd.isLocked(user)) {
            setMessage(MSG_BLOCK_USER);
        } else {
            setMessage(MSG_WRONG_PWD);
        }
    }

    return check;
}

// ��������� ������������� ����� ������ ����� ��������� ���������.
user_t clMenu::checkPwdReq(eGB_PARAM param, int16_t value) const {
    user_t user = USER_operator;

    if (param == GB_PARAM_IS_USER) {
        user_t userpi = sParam.security.getUser(USER_SOURCE_pi);
        if ((value != USER_operator) && (value != userpi)) {
            user = static_cast<user_t> (value);
        }
    } else if (param == GB_PARAM_IS_RESET_PWD) {
        user = USER_factory;
    }

    return user;
}

// ��������� ���������� ��� ����� �������� � ����������.
void clMenu::enterParameter() {
	LocalParams *lp = &sParam.local;
    eGB_PARAM param = lp->getParam();

    if (!checkChangeReg()) {
        setMessage(MSG_WRONG_REGIME);
    } else if (!sParam.security.checkUserAccess(getChangeUser(param), USER_SOURCE_pi)) {
        setMessage(MSG_WRONG_USER);
    } else {
        switch(getParamType(param)) {
			case Param::PARAM_BITES: // DOWN
            case Param::PARAM_LIST: {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
            } break;
			case Param::PARAM_I_COR: // DOWN
            case Param::PARAM_INT: {
				EnterParam.setEnable(MENU_ENTER_PARAM_INT);
            } break;
            case Param::PARAM_U_COR: {
				EnterParam.setEnable(MENU_ENTER_PARAM_U_COR);
            } break;
            case Param::PARAM_PWD: {
                EnterParam.setEnable(MENU_ENTER_PASSWORD);
            } break;
            case Param::PARAM_NO: {
            } break;
		}

		if (EnterParam.isEnable()) {
            int16_t min = getMin(param);
			int16_t val = lp->getValue();
			int16_t max = lp->getMax();

			// ��� ����� �������� ��������� ���� � ����������
			// ����������� �������� ������ 0 , � ��������� ��������
			// ������������ ������ �������� ����������/����, � �� ���������.
			// ���� ��� ���� ��������� ��������, �� �������� ���� ������ 0.
			// ����� ��� ������ ���� ���� - ������� ������� ���������.
			//
			// ��� ��������� ���������� � ������ ������ �������� ��������,
			// ��������������� �������.
            if (getParamType(param) == Param::PARAM_I_COR) {
				min = 0;
				val = sParam.measParam.getCurrentOut();
				if ((val < min) || (val > max)) {
					val = 0;
				}
				if (lp->getState() != LocalParams::STATE_NO_ERROR) {
					val = 0;
					max = 0;
				}
            } else if (getParamType(param) == Param::PARAM_U_COR) {
				min = 0;
				val = sParam.measParam.getVoltageOut();
				if ((val < min) || (val > max)) {
					val = 0;
				}
				if (lp->getState() != LocalParams::STATE_NO_ERROR) {
					val = 0;
					max = 0;
				}
			} else 	if (lp->getState() != LocalParams::STATE_NO_ERROR) {
				val = min;
			}

            enterFunc = &clMenu::inputValue;
            EnterParam.setParam(param);
			EnterParam.setValueRange(min, max);
			EnterParam.setValue(val);
            EnterParam.list = getListOfValues(param);
		}
    }
}

//
void clMenu::saveParam() {
    if (sParam.save.param < GB_PARAM_MAX) {
        sParam.save.com = getCom(sParam.save.param);
        sParam.save.sendType = getSendType(sParam.save.param);
        sParam.save.dopByte = getSendDop(sParam.save.param);
    }

//    qDebug() << "Save -->" << hex <<
//                "param = " << dec << save.param <<
//                ", com = " << hex << save.com <<
//                ", sendType = " << dec << save.sendType <<
//                ", number = " << hex << save.number <<
//                ", dopByte = " << hex << save.dopByte <<
//                ", value[0] = " << hex << save.value[0] <<
//                ", value[1] = " << hex << save.value[1];

    if (sParam.save.com != GB_COM_NO) {
        saveParamToBsp();
    } else {
        saveParamToRam();
    }
}

//
void clMenu::saveParamToBsp() {
    if (sParam.save.com == GB_COM_NO) {
        return;
    }

    if ((sParam.save.sendType > GB_SEND_NO) && (sParam.save.sendType < GB_SEND_MAX)){
        uint8_t pos = sParam.save.number - 1;
        uint8_t wrcom = sParam.save.com | GB_COM_MASK_GROUP_WRITE_PARAM;
        sParam.save.com = static_cast<eGB_COM> (wrcom);

        sParam.txComBuf.addFastCom(sParam.save.com, sParam.save.sendType);

        switch(sParam.save.sendType) {
            case GB_SEND_INT8: {
                sParam.txComBuf.setInt8(sParam.save.getValue());
            } break;

            case GB_SEND_INT8_DOP: {
                sParam.txComBuf.setInt8(sParam.save.getValue(), 0);
                sParam.txComBuf.setFastComDopByte(pos + sParam.save.dopByte);
            } break;

            case GB_SEND_DOP_INT8: {
                sParam.txComBuf.setFastComDopByte(pos + sParam.save.dopByte);
                sParam.txComBuf.setInt8(sParam.save.getValue(), 0);
            } break;

            case GB_SEND_INT16_BE: {
                sParam.txComBuf.setInt16BE(sParam.save.getValue());
            } break;

            case GB_SEND_BITES_DOP:	{
                // FIXME ������ �������� � ���������� ���������!
                uint8_t val = sParam.local.getValueB();
                if (sParam.save.getValue() > 0) {
                    val |= (1 << (pos % 8));
                } else {
                    val &= ~(1 << (pos % 8));
                }
                sParam.txComBuf.setInt8(val, 0);
                sParam.txComBuf.setFastComDopByte(pos/8 + sParam.save.dopByte);
            } break;

            case GB_SEND_DOP_BITES: {
                // FIXME ������ �������� � ���������� ���������!
                uint8_t val = sParam.local.getValueB();
                if (sParam.save.getValue() > 0) {
                    val |= (1 << (pos % 8));
                } else {
                    val &= ~(1 << (pos % 8));
                }
                sParam.txComBuf.setFastComDopByte(pos/8 + sParam.save.dopByte);
                sParam.txComBuf.setInt8(val, 0);
            } break;

            case GB_SEND_COR_U: {
                // FIXME ������ �������� � ���������� ���������!
                // ���� ������� �������� ��������� ���� ����� 0
                // �� ���������� ��������� � ���.������ ������ 4
                // ���������� ����� ���������
                int16_t t = sParam.save.getValue();
                if (t == 0)
                    sParam.save.dopByte = 4;
                else {
                    // ����� ��������� =
                    // ���������� ������� - (���������� � ��� - ���������)
                    t -= (int16_t) (sParam.measParam.getVoltageOut());
                    t += sParam.local.getValue();
                }
                sParam.txComBuf.setInt8(sParam.save.dopByte, 0);
                sParam.txComBuf.setInt8(t / 10, 1);
                sParam.txComBuf.setInt8((t % 10) * 10, 2);
            } break;

            case GB_SEND_COR_I: {
                // FIXME ������ �������� � ���������� ���������!
                // ���� ������� �������� ��������� ���� ����� 0
                // �� ���������� ��������� � ���.������ ������ 5
                // ���������� ����� ���������
                int16_t t = sParam.save.getValue();
                if (t == 0)
                    sParam.save.dopByte = 5;
                else {
                    // ����� ��������� = ��� ������� - (��� � ��� - ���������)
                    t -= static_cast<int16_t>(sParam.measParam.getCurrentOut());
                    t += sParam.local.getValue();
                }
                sParam.txComBuf.setInt8(sParam.save.dopByte, 0);
                sParam.txComBuf.setInt8((t >> 8), 1);
                sParam.txComBuf.setInt8((t), 2);
            } break;

            case GB_SEND_DOP_PWD: {
                sParam.txComBuf.setInt8(sParam.save.dopByte, 0);
                sParam.txComBuf.setArray(sParam.save.getValueArray(), PWD_LEN, 1);
            } break;

            case GB_SEND_IS_ENTRY:
            case GB_SEND_TIME:
            case GB_SEND_NO:
            case GB_SEND_NO_DATA:
            case GB_SEND_MAX: {

            } break;
        }
    }
}

//
void clMenu::saveParamToRam() {
    if (sParam.save.param != GB_PARAM_NULL_PARAM) {
        int16_t value = sParam.save.getValue();

        if (sParam.save.param == GB_PARAM_IS_USER) {
            sParam.security.setUser((user_t) (value), USER_SOURCE_pi);
        }
    }
}

//
void clMenu::security() {
    static uint8_t cntSecurity = TIME_SECURITY;

    if (cntSecurity < TIME_SECURITY) {
        cntSecurity++;
    }

    if (!isConnectionBsp()) {
        sParam.security.pwd.reset();
        sParam.security.rstUser(USER_SOURCE_pc);
        sParam.security.rstUser(USER_SOURCE_pi);
    } else if (!isConnectionPc()) {
#ifdef NDEBUG
        sParam.security.rstUser(USER_SOURCE_pc);
#endif
    }

    if (sParam.security.pwd.isResetToDefault()) {
        setMessage(MSG_RESET_PWD);
    }

    if (cntSecurity == TIME_SECURITY) {
        eGB_COM com = GB_COM_NO;
        sParam.security.tick(sParam.glb.status.getRegime(), com);

        if (com != GB_COM_NO) {
            sParam.txComBuf.addFastCom(com, GB_SEND_NO_DATA);
        }

        for(uint8_t i = USER_operator + 1; i < USER_MAX; i++) {
            user_t user = static_cast<user_t> (i);
            if (sParam.security.pwd.isCounterChanged(user)) {
                sParam.save.param = sParam.security.pwd.getCounterParam(user);
                sParam.save.number = 1;
                sParam.save.set(sParam.security.pwd.getCounter(user));
                saveParam();
            }

            if (sParam.security.pwd.isChangedPwd(user)) {
                sParam.save.param = sParam.security.getPwdParam(user);
                sParam.save.number = 1;
                sParam.save.set(sParam.security.pwd.getPwd(user));
                saveParam();
            }
        }

        cntSecurity = 0;
    }

    if (!sParam.security.sevent.isEmpty()) {
        user_t user = USER_MAX;
        userSrc_t source = USER_SOURCE_MAX;
        TSecurityEvent::event_t event = TSecurityEvent::EVENT_MAX;
        sParam.security.sevent.pop(user, source, event);
        if (sParam.txComBuf.addFastCom(GB_COM_JRN_IS_SET_ENTRY, GB_SEND_IS_ENTRY)) {
            sParam.txComBuf.setInt8(user, 0);
            sParam.txComBuf.setInt8(source, 1);
            sParam.txComBuf.setInt8(event, 2);
        }
    }
}

// ������ � ���� ��������� ����������.
void clMenu::setupParam() {
    if (isMessage()) {
        printMessage();
    } else {
        printParam();

        if (EnterParam.isEnable()) {
            if ((this->*enterFunc)() == MENU_ENTER_PARAM_READY) {
                eGB_PARAM param = EnterParam.getParam();
                int16_t value = EnterParam.getValue();
                user_t user = checkPwdReq(param, value);
                if (user != USER_operator) {
                    if (!sParam.security.pwd.isLocked(user)) {
                        EnterParam.setEnable(MENU_ENTER_PASSWORD);
                        enterFunc = &clMenu::inputValue;
                        EnterParam.setParam(GB_PARAM_IS_PWD);
                        EnterParam.setValueRange(1, 8);
                    } else {
                        // FIXME ��� ����� �����-�� ���������� �������� �� "����".
                        setMessage(MSG_BLOCK_USER);
                        EnterParam.setDisable();
                    }
                } else if (param == GB_PARAM_IS_PWD) {
                    if (EnterParam.last.param == GB_PARAM_IS_USER) {
                        user_t user = (user_t) EnterParam.last.val;
                        if (checkPwdInput(user, EnterParam.getValuePwd())) {
                            sParam.save.param = EnterParam.last.param;
                            sParam.save.number = 1;
                            sParam.save.set(static_cast<uint8_t> (user));
                            saveParam();
                        }
                    } else if (EnterParam.last.param == GB_PARAM_IS_RESET_PWD) {
                        if (checkPwdInput(USER_factory, EnterParam.getValuePwd())) {
                            userSrc_t src = USER_SOURCE_pi;
                            user_t user = sParam.security.getUser(src);
                            sParam.security.sevent.pushPwdReset(user, src);
                            sParam.security.pwd.resetPwdToDefault();
                        }
                    }
                    EnterParam.setDisable();
                } else {
                    if (getParamType(param) == Param::PARAM_PWD) {
                        uint8_t *pwd = EnterParam.getValuePwd();
                        sParam.security.changeUserPiPwd(param, pwd);
                    } else {
                        sParam.save.param = param;
                        sParam.save.number = sParam.local.getNumOfCurrSameParam();
                        sParam.save.set(value);
                        saveParam();
                        EnterParam.setDisable();
                    }
                }
            }
        }
    }

	switch(key_) {
		case KEY_UP:
			sParam.local.prevParam();
			break;
		case KEY_DOWN:
			sParam.local.nextParam();
			break;

		case KEY_LEFT:
			sParam.local.prevSameParam();
			break;
		case KEY_RIGHT:
			sParam.local.nextSameParam();
			break;

		case KEY_ENTER:
			enterParameter();
			break;

		default:
			break;
	}

    // ������ ������� ��� ������ �������� ���������
    eGB_COM com = getCom(sParam.local.getParam());
    if (sParam.txComBuf.getLocalCom() != com) {
        sParam.txComBuf.addFastCom(com, getSendType(sParam.local.getParam()));
        sParam.txComBuf.setLocalCom(com);
    }

	// ����� �� �������� ������ ����, ���� ���-�� ���������� ����� 0
	if (sParam.local.getNumOfParams() == 0) {
		key_ = KEY_CANCEL;
	}
}

//
bool clMenu::checkLedOn() {
	bool ledOn = false;

	if (sParam.glb.status.getRegime() != GB_REGIME_ENABLED) {
		ledOn = true;
	}

	if (sParam.glb.status.isFault() || sParam.glb.status.isWarning()) {
		ledOn = true;
	}

	if (sParam.def.status.isEnable()) {
		if (sParam.def.status.getState() != 1) {
			ledOn = true;
		} else if (sParam.def.status.isFault()) {
			ledOn = true;
		} else if (sParam.def.status.isWarning()) {
			ledOn = true;
		}
	}

	if (sParam.prm.status.isEnable()) {
		if (sParam.prm.status.getState() != 1) {
			ledOn = true;
		} else if (sParam.prm.status.isFault()) {
			ledOn = true;
		} else if (sParam.prm.status.isWarning()) {
			ledOn = true;
		} else if (sParam.prm.isIndCom()) {
			ledOn = true;
		}
	}

	if (sParam.prd.status.isEnable()) {
		if (sParam.prd.status.getState() != 1) {
			ledOn = true;
		} else if (sParam.prd.status.isFault()) {
			ledOn = true;
		} else if (sParam.prd.status.isWarning()) {
			ledOn = true;
		} else if (sParam.prd.isIndCom()) {
			ledOn = true;
		}
	}

    if (!isConnectionBsp()) {
        ledOn = true;
    }

    if (sParam.security.getUser(USER_SOURCE_pc) != USER_operator) {
        ledOn = true;
    }

    if (sParam.security.getUser(USER_SOURCE_pi) != USER_operator) {
        ledOn = true;
    }

	if (sParam.glb.isLedOn()) {
		ledOn = true;
		sParam.glb.setLedOn(false);
	}

	return ledOn;
}
