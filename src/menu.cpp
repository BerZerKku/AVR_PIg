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
	pLvlMenu = &clMenu::lvlStart;
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

	// �������� ������� ��������� ������ ��� ���� ���������
	// ������ ������ ���� �������� ��������� � ��������� ������ ������
	// ���� fcUnknownSost
	for(uint_fast8_t i = 0; i < MAX_NUM_DEVICE_STATE; i++) {
		sParam.Glb.Status.pStateText[i] = fcUnknownSost;
	}

	sParam.Rps.Status.pStateText[0] =  fcRpsSost00;
	sParam.Rps.Status.pStateText[1] =  fcRpsSost01;
	sParam.Rps.Status.pStateText[2] =  fcRpsSost02;
	sParam.Rps.Status.pStateText[3] =  fcRpsSost03;
	sParam.Rps.Status.pStateText[4] =  fcRpsSost04;
	sParam.Rps.Status.pStateText[5] =  fcRpsSost05;
	sParam.Rps.Status.pStateText[6] =  fcRpsSost06;
	sParam.Rps.Status.pStateText[7] =  fcRpsSost07;
	sParam.Rps.Status.pStateText[8] =  fcRpsSost08;
	sParam.Rps.Status.pStateText[9] =  fcRpsSost09;
	sParam.Rps.Status.pStateText[10] = fcRpsSost10;
	sParam.Rps.Status.pStateText[11] = fcRpsSost11;
	sParam.Rps.Status.pStateText[12] = fcRpsSost12;
	sParam.Rps.Status.pStateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

	sParam.Rps.Status.pName = fcDeviceName00;

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

	// ������� ��� � ����������
	// ���� ������ ����� ������ - ��������� ��������������� ���������
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO) {
		if (tmp == KEY_EMPTY)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLed(LED_SWITCH);
	}

	// ��������� �������� ������, ���� ����� �� "������"
	if (sParam.Glb.Status.getRegime() != GB_REGIME_ENABLED) {
		vLCDsetLed(LED_SWITCH);
	}

	// ������� ������ ���������
	if (delay_ < TIME_MESSAGE) {
		delay_++;
	}

	// ����� � ����� ����������� �������� ����
	// ���� ��������� ��� ��� �������� �� ���������
	clearTextBuf();
	(this->*pLvlMenu)();
	key_ = KEY_NO;

#ifdef VIEW_DEBUG_PARAM
	// ����� ���������� ����������
	if (this->pLvlMenu == &clMenu::lvlStart) {
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
		sParam.TxComBuf.addFastCom(GB_COM_GET_VERS);
		sParam.TxComBuf.addFastCom(GB_COM_GET_VERS);
	}
	lastConnection = connection;

	// �������������� ������ �������� � ������ ��� ������ �� �����
	vLCDputchar(vLCDbuf, lineParam_);
	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
}

//	��������� ���� ���������� � ��������� ���� � ��� ������.
bool clMenu::setDevice(eGB_TYPE_DEVICE device) {
	sParam.typeDevice = device;

	// ������������ ���������� ���������� �� ������
	// 0 1
	// 2 3
	// 4 5
	// 6 7

	// ������ ����� ����������
	measParam[0] = MENU_MEAS_PARAM_TIME;
	measParam[1] = MENU_MEAS_PARAM_DATE;
	measParam[2] = MENU_MEAS_I_A;
	measParam[3] = MENU_MEAS_U_A;
	measParam[4] = MENU_MEAS_I_B;
	measParam[5] = MENU_MEAS_U_B;
	measParam[6] = MENU_MEAS_I_C;
	measParam[7] = MENU_MEAS_U_C;

	// ������ ����� ����������
	measParam[0 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_P_OUT;
	measParam[1 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_U_CTRL;
	measParam[2 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_N_IN;
	measParam[3 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_FAZ_A;
	measParam[4 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_N_OUT;
	measParam[5 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_FAZ_B;
	measParam[6 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_S_N;
	measParam[7 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_FAZ_C;

	for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
		sParam.Glb.Status.pFaultText[i] = fcUnknownFault;
	for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
		sParam.Rps.Status.pFaultText[i] = fcUnknownFault;

	for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
		sParam.Glb.Status.pWarningText[i] = fcUnknownWarning;
	for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
		sParam.Rps.Status.pWarningText[i] = fcUnknownWarning;

	// ���������� ������� ����� ��������������
	sParam.Glb.Status.pFaultText[0] = fcGlbFault0001;
	sParam.Glb.Status.pFaultText[1] = fcGlbFault0002;
	sParam.Glb.Status.pFaultText[2] = fcGlbFault0004;
	sParam.Glb.Status.pFaultText[3] = fcGlbFault0008;
	sParam.Glb.Status.pFaultText[4] = fcGlbFault0010;
	// 5-7 ���
	sParam.Glb.Status.pFaultText[8] = fcGlbFault0100;
	sParam.Glb.Status.pFaultText[9] = fcGlbFault0200;
	// 10 ���
	sParam.Glb.Status.pFaultText[11] = fcGlbFault0800;
	sParam.Glb.Status.pFaultText[12] = fcGlbFault1000;
	// 13-15 ���
	// ���������� ������� ����� ��������������
	// 1-9 ���
	sParam.Glb.Status.pWarningText[10] = fcGlbWarning01;
	// 11-15 ���

	return true;
}

eGB_COM clMenu::getTxCommand() {
	static uint8_t cnt = 0;
	eGB_COM com = sParam.TxComBuf.getFastCom();

	if (com == GB_COM_NO) {
		cnt++;
		if (cnt == 1)
			com = GB_COM_GET_SOST;
		else if (cnt == 2)
			com = sParam.TxComBuf.getCom1();

		// ���� ��� �������, �������� ������� �� ������ 2
		if (com == GB_COM_NO)
			com = sParam.TxComBuf.getCom2();

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
		sParam.TxComBuf.clear();
		sParam.TxComBuf.addCom2(GB_COM_GET_VERS);
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
		pLvlMenu = &clMenu::lvlStart;
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
		lineParam_ = 4;
		curCom_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();
		// ����� 1
		// �������������� �������
		// ����� ���������� ��������� (� ��)
		sParam.TxComBuf.addCom2(GB_COM_GET_TIME);
		sParam.TxComBuf.addCom2(GB_COM_GET_MEAS);

		// ����� 2
		sParam.TxComBuf.addCom1(GB_COM_GET_FAULT);
	}

	// ����� �� ����� ���������� ����������
	for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++) {
		if (curCom_ == 2) {
			printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
		} else {
			printMeasParam(i, measParam[i]);
		}
	}

	uint8_t poz = lineParam_ * 20;
	printDevicesStatus(poz, &sParam.Rps.Status);

	switch (key_) {
		case KEY_UP:
			curCom_ = (curCom_ == 1) ? 2 : 1;
			break;

		case KEY_DOWN:
			curCom_ = (curCom_ == 1) ? 2 : 1;
			break;

		case KEY_MENU:
			pLvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_RESET_IND:
			sParam.TxComBuf.addFastCom(GB_COM_SET_PRM_RES_IND);
			break;

		case KEY_PUSK:
			sParam.TxComBuf.addFastCom(GB_COM_SET_PRM_ENTER);
			break;

		case KEY_RESET:
			sParam.TxComBuf.setInt8(GB_CONTROL_RESET_SELF);
			sParam.TxComBuf.addFastCom(GB_COM_SET_CONTROL);
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
	static char punkt4[] PROGMEM = "%d. ����������";

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

		// ������������� �������
		sParam.TxComBuf.clear();
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

		case KEY_ENTER:
			if (name == punkt1) {
				pLvlMenu = &clMenu::lvlJournal;
				lvlCreate_ = true;
			} else if (name == punkt2) {
				pLvlMenu = &clMenu::lvlControl;
				lvlCreate_ = true;
			} else if (name == punkt3) {
				pLvlMenu = &clMenu::lvlSetup;
				lvlCreate_ = true;
			} else if (name == punkt4) {
				pLvlMenu = &clMenu::lvlInfo;
				lvlCreate_ = true;
			}
			break;

		case KEY_CANCEL:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
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
	static char versProg[] PROGMEM = "%S: %02X.%02X";

	if (lvlCreate_) {
		lvlCreate_ = false;
		lineParam_ = 1;
		cursorLine_ = 0;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();
		Punkts_.add(GB_IC_BSP_MCU);
		Punkts_.add(GB_IC_BSP_DSP);
		Punkts_.add(GB_IC_PI_MCU);
		Punkts_.add(GB_IC_BSK_PLIS_PRD1);
		Punkts_.add(GB_IC_BSK_PLIS_PRM1);

		// ������������� �������
		// ����������� ������ �������� (�� ������ ������������)
		sParam.TxComBuf.clear();
		sParam.TxComBuf.addCom2(GB_COM_GET_VERS);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t numLines = NUM_TEXT_LINES - lineParam_;
	uint8_t cntPunkts = cursorLine_;
	for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++) {
		uint8_t ic = Punkts_.getNumber(cntPunkts);
		uint16_t vers = sParam.Glb.getVersProgIC((eGB_IC) ic);
		snprintf_P(&vLCDbuf[20 * line], 21, versProg, fcICname[ic],
				(uint8_t) (vers >> 8), (uint8_t) vers);

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
			pLvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
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

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		// ��������� ����������� ������� ���� � ��������������� �� ������
		Punkts_.clear();
		Punkts_.add(punkt1);
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
			pLvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			if (name == punkt1) {
				pLvlMenu = &clMenu::lvlJournalEvent;
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
		sParam.JrnEntry.clear();
		sParam.JrnEntry.setMaxNumJrnEntries(GLB_JRN_EVENT);

		// ������������� �������
		sParam.TxComBuf.clear();
		sParam.TxComBuf.addCom1(GB_COM_GET_JRN_CNT);
		sParam.TxComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
		sParam.TxComBuf.setInt16(sParam.JrnEntry.getEntryAdress());
	}

	// ����� ������� ������ � ������ � ������������ ���-�� �������
	uint16_t cur_entry = sParam.JrnEntry.getCurrentEntry();
	uint16_t num_entries = sParam.JrnEntry.getNumJrnEntries();

	uint8_t poz = 0;
	// ����� �������� �������� ������ ����
	snprintf_P(&vLCDbuf[poz], 21, title);
	poz += 20;

	if (num_entries == 0) {
		// ����� ��������� �� ���������� ������� � �������
		snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
	} else if (!sParam.JrnEntry.isReady()) {
		// ��������� � ������� ������ ��� �� ��������
		snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
	} else {

	}

	switch (key_) {
		case KEY_UP:
			if (sParam.JrnEntry.setPreviousEntry()) {
				sParam.TxComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
				curCom_ = 1;
			}
			break;
		case KEY_DOWN:
			if (sParam.JrnEntry.setNextEntry()) {
				sParam.TxComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
				curCom_ = 1;
			}
			break;

		case KEY_CANCEL:
			pLvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		default:
			break;
	}

	// �������� � ��������� ��� ��� ����� ����������� ������
	// �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
	// ������� ������ ����
	sParam.TxComBuf.setInt16(sParam.JrnEntry.getEntryAdress());
}

/**	������� ����. ����������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlControl() {
	static char title[] PROGMEM = "����\\����������";
	// %d - ����� ���� �����������, ������ ��� ����. ���-�� �������� !
	//							   	"01234567890123456789"
	static char punkt01[] PROGMEM = "%d. ����� ������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		Punkts_.clear();

		Punkts_.add(punkt01);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();

	switch (key_) {
		case KEY_UP: {
			cursorLineUp();
		}
			break;

		case KEY_DOWN: {
			cursorLineDown();
		}
			break;

		case KEY_CANCEL:
			pLvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_MENU: {
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
		}
			break;

		case KEY_ENTER: {
			if (name == punkt01) {
				sParam.TxComBuf.setInt8(GB_CONTROL_RESET_SELF);
				sParam.TxComBuf.addFastCom(GB_COM_SET_CONTROL);
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
		sParam.TxComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);
	printPunkts();
	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();

		// ����� ������� ����� : ������ ��� ��������
		(this->*pEnterFunc)();

		if (stat == MENU_ENTER_PASSWORD_READY) {
			uint16_t val = EnterParam.getValue();

			if (sParam.Password.check(val)) {
				EnterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
			} else
				EnterParam.setDisable();
		} else if (stat == MENU_ENTER_PASSWORD_N_READY) {
			uint16_t val = EnterParam.getValue();

			sParam.Password.set(val);
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
			pLvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER: {
			if (name == punkt1) {
				pLvlMenu = &clMenu::lvlRegime;
				lvlCreate_ = true;
			} else if (name == punkt2) {
				pLvlMenu = &clMenu::lvlSetupDT;
				lvlCreate_ = true;
			} else if (name == punkt3) {
				pLvlMenu = &clMenu::lvlSetupParam;
				lvlCreate_ = true;
			} else if (name == punkt4) {
				pEnterFunc = &clMenu::enterPassword;
				EnterParam.setEnable(MENU_ENTER_PASSWORD);
				EnterParam.com = GB_COM_NO;
			} else if (name == punkt5) {
				pLvlMenu = &clMenu::lvlSetupInterface;
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
	eGB_REGIME reg = sParam.Glb.Status.getRegime();

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ������������� �������
		sParam.TxComBuf.clear();
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = lineParam_ * 20;
	printDevicesRegime(poz, &sParam.Rps.Status);
	poz += 20;

	// ���� ������ �������� ���������.
	// ������� ���������� ��������� ����� ������.
	// ��� ������� ������� � ����� "�������" �� "������" ��� "�����",
	// ���������� ������ ������. ��� ��������� ������ ��������� ���������.
	if (EnterParam.isEnable()) {
		eMENU_ENTER_PARAM stat = EnterParam.getStatus();
		eGB_REGIME_ENTER val = GB_REGIME_ENTER_MAX;

		// ����� ������� ����� : ������ ��� ��������
		(this->*pEnterFunc)();

		if (stat == MENU_ENTER_PARAM_READY) {
			// �������� ������, ���� �������� ������� � ����� "�������"
			// �� ������� "������" � "�����"
			val = (eGB_REGIME_ENTER) EnterParam.getValueEnter();

			if ((reg == GB_REGIME_ENABLED) || (reg == GB_REGIME_READY)) {
				if (val == GB_REGIME_ENTER_DISABLED) {
					pEnterFunc = &clMenu::enterPassword;
					EnterParam.setEnable(MENU_ENTER_PASSWORD);
					EnterParam.setDopValue(static_cast<uint16_t>(val));
					val = GB_REGIME_ENTER_MAX;
				}
			}
		}

		if (stat == MENU_ENTER_PASSWORD_READY) {
			// �������� ��������� ������
			if (sParam.Password.check(EnterParam.getValue())) {
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
			sParam.TxComBuf.addFastCom(com);
			EnterParam.setDisable();
		}
	}

	switch (key_) {
		case KEY_CANCEL:
			pLvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER: {
			uint8_t min = GB_REGIME_ENTER_DISABLED;
			uint8_t max = GB_REGIME_ENTER_DISABLED;
			uint8_t val = GB_REGIME_ENTER_DISABLED;

			// "������" 	-> "�������" + ������
			// "�������" 	-> "������"
			// "�����" 		-> "������"	+ ������ / "�������"
			// ��������� 	-> "�������" / "������"
			pEnterFunc = &clMenu::enterValue;
			EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
			switch (reg) {
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

	static char punkt1[] PROGMEM = "%d. �����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� ����, � ����������� �� �������� ����������
		Punkts_.clear();
		Punkts_.add(punkt1);

		// ������������� �������
		sParam.TxComBuf.clear();
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
			pLvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			if (name == punkt1) {
				pLvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate_ = true;
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
	static char punkt1[] PROGMEM = "����������� ��������";
	static char punkt2[] PROGMEM = "������������ ���";
	static char punkt3[] PROGMEM = "����������� ���";
	static char punkt4[] PROGMEM = "�����. ����������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;
		curCom_ = 1;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);


		Punkts_.clear();
		Punkts_.add(punkt1, GB_COM_GET_COMP_DELAY);
		Punkts_.add(punkt2, GB_COM_GET_CURR_MAX);
		Punkts_.add(punkt3, GB_COM_GET_CURR_MIN);
		Punkts_.add(punkt4, GB_COM_GET_VOLT_MIN);

		sParam.TxComBuf.clear();
		// ������� ������� ��������� �� ������
		sParam.TxComBuf.addCom1(Punkts_.getCom(0), 0);
		// ������� ���������� ����������
//		sParam.TxComBuf.addCom2(GB_COM_GET_MEAS);
	}

	// ������� �������, �� ������� �������� ������ ����.
	sParam.TxComBuf.addCom1(Punkts_.getCom(cursorLine_ - 1), 0);

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_,
			Punkts_.getMaxNumPunkts());

	poz = 40;
	snprintf_P(&vLCDbuf[poz], 21, name);

	// ����������� ���.������, ��� ���������� ����������
//	poz = 60;

	//  ����� ������� "��������:" � ������� � ������ ������ ���������
	poz = 80;
	poz += snprintf_P(&vLCDbuf[poz], 11, fcRange);
	if (name == punkt1) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_COMP_DELAY_MIN,
				GLB_COMP_DELAY_MAX / 1000, "��");
	} else if (name == punkt2) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CURR_MAX_MIN,
				GLB_CURR_MAX_MAX, "��");
	} else if (name == punkt3) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_CURR_MIN_MIN,
				GLB_CURR_MIN_MAX, "��");
	} else if (name == punkt4) {
		snprintf_P(&vLCDbuf[poz], 11, fcRangeDec, GLB_VOLT_MIN_MIN,
				GLB_VOLT_MIN_MAX, "��");
	}

	if (EnterParam.isEnable()) {
		// ���� ������ �������� ���������
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			// ����� �������� �������, ���� �������� � ���
			sParam.TxComBuf.setInt8(EnterParam.getValueEnter());

			sParam.TxComBuf.addFastCom(EnterParam.com);
			EnterParam.setDisable();
		}
	} else {
		// ����� ������� "��������:" � ������� � ������ ������ ��������
		poz = 100;
		poz += snprintf_P(&vLCDbuf[poz], 11, fcValue);
		if (name == punkt1) {
			uint8_t i = (sParam.Glb.getCompDelay() / 1000);
			uint16_t f = (sParam.Glb.getCompDelay() % 1000);
			snprintf(&vLCDbuf[poz], 11, "%01d.%01d��", i, f);
		} else if (name == punkt2) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.Glb.getCurrMax());
		} else if (name == punkt3) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.Glb.getCurrMin());
		} else if (name == punkt4) {
			snprintf(&vLCDbuf[poz], 11, "%d��", sParam.Glb.getVoltMin());
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
			pLvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			pEnterFunc = &clMenu::enterValue;
			if (sParam.Glb.Status.getRegime() != GB_REGIME_DISABLED) {
				// ���� ���� ���� �� ��������� ��������� �������
				// ��������� ���������� ���������
				EnterParam.printMessage();
			} else if (name == punkt1) {
				EnterParam.setEnable(MENU_ENTER_PARAM_COMP_D);
				EnterParam.setValueRange(GLB_COMP_DELAY_MIN,
						GLB_COMP_DELAY_MAX);
				EnterParam.setValue(sParam.Glb.getCompDelay());
				EnterParam.setDisc(GLB_COMP_DELAY_DISC);
				EnterParam.setFract(GLB_COMP_DELAY_FRACT);
				EnterParam.com = GB_COM_SET_COMP_DELAY;
			} else if (name == punkt2) {
				EnterParam.setEnable();
				EnterParam.setValueRange(GLB_CURR_MAX_MIN, GLB_CURR_MAX_MAX);
				EnterParam.setValue(sParam.Glb.getCurrMax());
				EnterParam.setDisc(GLB_CURR_MAX_DISC);
				EnterParam.setFract(GLB_CURR_MAX_FRACT);
				EnterParam.com = GB_COM_SET_CURR_MAX;
			} else if (name == punkt3) {
				EnterParam.setEnable();
				EnterParam.setValueRange(GLB_CURR_MIN_MIN, GLB_CURR_MIN_MAX);
				EnterParam.setValue(sParam.Glb.getCurrMin());
				EnterParam.setDisc(GLB_CURR_MIN_DISC);
				EnterParam.setFract(GLB_CURR_MIN_FRACT);
				EnterParam.com = GB_COM_SET_CURR_MIN;
			} else if (name == punkt4) {
				EnterParam.setEnable();
				EnterParam.setValueRange(GLB_VOLT_MIN_MIN, GLB_VOLT_MIN_MAX);
				EnterParam.setValue(sParam.Glb.getVoltMin());
				EnterParam.setDisc(GLB_VOLT_MIN_DISC);
				EnterParam.setFract(GLB_VOLT_MIN_FRACT);
				EnterParam.com = GB_COM_SET_VOLT_MIN;
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
		sParam.TxComBuf.clear();
		sParam.TxComBuf.addCom2(GB_COM_GET_TIME);
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
					uint8_t month = BCD_TO_BIN(sParam.TxComBuf.getInt8(1));
					uint8_t day = BCD_TO_BIN(sParam.TxComBuf.getInt8(2));
					if (day > sParam.DataTime.getNumDaysInMonth(month, val)) {
						sParam.TxComBuf.setInt8(0x01, 2);
					}
				} else if (t == 1) {
					// ���� ������, �������� ���-�� ������������� ����
					uint8_t day = BCD_TO_BIN(sParam.TxComBuf.getInt8(2));
					if (day > sParam.DataTime.getNumDaysInMonth(val)) {
						sParam.TxComBuf.setInt8(0x01, 2);
					}
				}
				sParam.TxComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getHour()),
						3);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getMinute()),
						4);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getSecond()),
						5);
				sParam.TxComBuf.addFastCom(EnterParam.com);
			} else if (t <= 5) {
				// ���� �������
				// �������� ����������� ���� �� �������
				sParam.TxComBuf.setInt8(BIN_TO_BCD(val), t);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getYear()),
						0);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getMonth()),
						1);
				sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getDay()),
						2);
				sParam.TxComBuf.addFastCom(EnterParam.com);
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
			pEnterFunc = &clMenu::enterValue;
			if (name == punkt1) {
				EnterParam.setEnable();
				EnterParam.setValueRange(0, 99);
				EnterParam.setValue(sParam.DataTime.getYear());
				EnterParam.setDopValue(0);
			} else if (name == punkt2) {
				EnterParam.setEnable();
				EnterParam.setValueRange(1, 12);
				EnterParam.setValue(sParam.DataTime.getMonth());
				EnterParam.setDopValue(1);
			} else if (name == punkt3) {
				EnterParam.setEnable();
				uint8_t max = sParam.DataTime.getNumDaysInMonth();
				EnterParam.setValueRange(1, max);
				EnterParam.setValue(sParam.DataTime.getDay());
				EnterParam.setDopValue(2);
			} else if (name == punkt4) {
				EnterParam.setEnable();
				EnterParam.setValueRange(0, 23);
				EnterParam.setValue(sParam.DataTime.getHour());
				EnterParam.setDopValue(3);
			} else if (name == punkt5) {
				EnterParam.setEnable();
				EnterParam.setValueRange(0, 59);
				EnterParam.setValue(sParam.DataTime.getMinute());
				EnterParam.setDopValue(4);
			} else if (name == punkt6) {
				EnterParam.setEnable();
				EnterParam.setValueRange(0, 59);
				EnterParam.setValue(sParam.DataTime.getSecond());
				EnterParam.setDopValue(5);
			}
			EnterParam.com = GB_COM_SET_TIME;
			EnterParam.setDisc(1);
			EnterParam.setFract(1);
			// �������� ������� �������� ���� � �������
			// ����� ����������� � ������� �������� � ���
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getYear()), 0);
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getMonth()), 1);
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getDay()), 2);
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getHour()), 3);
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getMinute()), 4);
			sParam.TxComBuf.setInt8(BIN_TO_BCD(sParam.DataTime.getSecond()), 5);
			break;

		case KEY_CANCEL:
			pLvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
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
		if (sParam.Uart.Interface.get() == GB_INTERFACE_RS485) {
			Punkts_.add(punkt2);
			Punkts_.add(punkt3);
			Punkts_.add(punkt4);
			Punkts_.add(punkt5);
			Punkts_.add(punkt6);
		}

		// �������������� �������
		sParam.TxComBuf.clear();
	}

	PGM_P name = Punkts_.getName(cursorLine_ - 1);

	snprintf_P(&vLCDbuf[0], 21, title);

	uint8_t poz = 20;
	snprintf_P(&vLCDbuf[poz], 21, fcNumPunkt, cursorLine_,
			Punkts_.getMaxNumPunkts());

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
				eGB_INTERFACE val =
						static_cast<eGB_INTERFACE>(EnterParam.getValueEnter());
				// ���� ��������� ��������, ������� ����
				if (val != sParam.Uart.Interface.get()) {
					sParam.Uart.Interface.set(val);
					lvlCreate_ = true;
				}
			} else if (name == punkt2) {
				// TODO ��� �������� ��� ��
			} else if (name == punkt3) {
				uint8_t val = EnterParam.getValueEnter();
				sParam.Uart.BaudRate.set(static_cast<eUART_BAUD_RATE>(val));
			} else if (name == punkt4) {
				uint8_t val = EnterParam.getValueEnter();
				sParam.Uart.DataBits.set(static_cast<eUART_DATA_BITS>(val));
			} else if (name == punkt5) {
				uint8_t val = EnterParam.getValueEnter();
				sParam.Uart.Parity.set(static_cast<eUART_PARITY>(val));
			} else if (name == punkt6) {
				uint8_t val = EnterParam.getValueEnter();
				sParam.Uart.StopBits.set(static_cast<eUART_STOP_BITS>(val));
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
			// TODO ��� �������� ��� ��
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
			pLvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			pLvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			pEnterFunc = &clMenu::enterValue;
			// � ����� ��������� ���������� �����
			// ��������� ��������� ������ � "�������"
			if (name == punkt1) {
				// ��������� �����
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(GB_INTERFACE_MIN,
						GB_INTERFACE_MAX - 1);
				EnterParam.setValue(sParam.Uart.Interface.get());
				EnterParam.list = fcInterface[0];
				EnterParam.com = GB_COM_NO;
			} else if (name == punkt2) {
				//TODO ��� �������� ��� ��
			} else if (name == punkt3) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(UART_BAUD_RATE_MIN,
						UART_BAUD_RATE_MAX - 1);
				EnterParam.setValue(sParam.Uart.BaudRate.get());
				EnterParam.list = fcBaudRate[0];
				EnterParam.com = GB_COM_NO;
			} else if (name == punkt4) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(UART_DATA_BITS_MIN,
						UART_DATA_BITS_MAX - 1);
				EnterParam.setValue(sParam.Uart.DataBits.get());
				EnterParam.list = fcDataBits[0];
				EnterParam.com = GB_COM_NO;
			} else if (name == punkt5) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(UART_PARITY_MIN, UART_PARITY_MAX - 1);
				EnterParam.setValue(sParam.Uart.Parity.get());
				EnterParam.list = fcParity[0];
				EnterParam.com = GB_COM_NO;
			} else if (name == punkt6) {
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				EnterParam.setValueRange(UART_STOP_BITS_MIN,
						UART_STOP_BITS_MAX - 1);
				EnterParam.setValue(sParam.Uart.StopBits.get());
				EnterParam.list = fcStopBits[0];
				EnterParam.com = GB_COM_NO;
			}
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
	static char enterComp[] PROGMEM = "����: %01u.%01u";

	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_MESSAGE) {
		uint8_t poz = 40;

		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
			clearLine(i);

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					" �������� ��������  ",//
					"  ����� ������ �    ",		//
					"  ������ �������    "
			};

			EnterParam.cnt_++;
			key_ = KEY_NO;

			for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
				snprintf_P(&vLCDbuf[poz], 21, message[i]);
		} else {
			key_ = KEY_CANCEL;
		}
	} else {
		uint8_t poz = 100;
		uint16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		if (status == MENU_ENTER_PARAM_INT) {
			// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
			if ((num >= 5) || (num == 0)) {
				key_ = KEY_CANCEL;
			} else {
				clearLine(NUM_TEXT_LINES);
				uint8_t poz = 100;
				snprintf_P(&vLCDbuf[poz], 21, enterInt, val);
			}
		} else if (status == MENU_ENTER_PARAM_COMP_D) {
			// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
			if ((num >= 6) || (num == 0)) {
				key_ = KEY_CANCEL;
			} else {
				clearLine(NUM_TEXT_LINES);
				uint8_t poz = 100;
				snprintf_P(&vLCDbuf[poz], 21, enterComp, val/1000, val%1000);
			}
		} else if (status == MENU_ENTER_PARAM_LIST) {
			clearLine(NUM_TEXT_LINES);
			snprintf_P(&vLCDbuf[poz], 21, enterList,
					EnterParam.list + STRING_LENGHT * val);
		} else if (status == MENU_ENTER_PARAM_LIST_2) {
			val = EnterParam.listValue[val];
			clearLine(NUM_TEXT_LINES);
			snprintf_P(&vLCDbuf[poz], 21, enterList,
					EnterParam.list + STRING_LENGHT * val);
		} else {
			key_ = KEY_CANCEL;
		}
	}

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
		for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++) {
			clearLine(i);
		}

		// ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
		// ����� ������� � �������� ����� ����
		if (EnterParam.cnt_ < TIME_MESSAGE) {
			static char message[3][21] PROGMEM = {
			//		 12345678901234567890
					"       ������       ",		//
					"    ������������    ",		//
					"       ������       "
			};

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
		if (cursorLine_ > numLines) {
			vLCDbuf[20 * (NUM_TEXT_LINES - 1) + 2] = '*';
		} else {
			vLCDbuf[20 * (cursorLine_ + lineParam_ - 1) + 2] = '*';
		}
	}
}

/**	����� � ��������� ����� ������������� ���������.
 * 	� ����� ������ ��������� ��� ���������.
 * 	@param poz ������� �������
 * 	@arg 0..eMENU_MEAS_PARAM, 0 ������ ������ �����, 7 - ��������� ������
 * 	@param par ������������ ��������
 * 	@return ���
 */
void clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par) {
	// �������� � ������
	if (poz < MAX_NUM_MEAS_PARAM) {
		// 10 - ���-�� �������� ���������� �� ������ ��� 1 ��������
		poz = (poz*10)  + 1;

		switch (par) {
			case MENU_MEAS_PARAM_DATE:
				snprintf_P(&vLCDbuf[poz], 10, fcDate, sParam.DataTime.getDay(),
						sParam.DataTime.getMonth(), sParam.DataTime.getYear());
				break;

			case MENU_MEAS_PARAM_TIME:
				snprintf_P(&vLCDbuf[poz], 10, fcTime, sParam.DataTime.getHour(),
						sParam.DataTime.getMinute(),
						sParam.DataTime.getSecond());
				break;

			case MENU_MEAS_P_OUT:
				snprintf_P(&vLCDbuf[poz], 10, fcPout,
						sParam.MeasParam.getPOut());
				break;

			case MENU_MEAS_U_CTRL:
				snprintf_P(&vLCDbuf[poz], 10, fcUctrl,
						sParam.MeasParam.getUCtrl());
				break;

			case MENU_MEAS_S_N:
				snprintf_P(&vLCDbuf[poz], 10, fcSN, sParam.MeasParam.getSN());
				break;

			case MENU_MEAS_N_OUT:
				snprintf_P(&vLCDbuf[poz], 10, fcNOut,
						sParam.MeasParam.getNOut());
				break;

			case MENU_MEAS_N_IN:
				snprintf_P(&vLCDbuf[poz], 10, fcNIn, sParam.MeasParam.getNIn());
				break;

			case MENU_MEAS_FAZ_A:
				snprintf_P(&vLCDbuf[poz], 10, fcFazA,
						sParam.MeasParam.getFazA());
				break;

			case MENU_MEAS_FAZ_B:
				snprintf_P(&vLCDbuf[poz], 10, fcFazB,
						sParam.MeasParam.getFazB());
				break;

			case MENU_MEAS_FAZ_C:
				snprintf_P(&vLCDbuf[poz], 10, fcFazC,
						sParam.MeasParam.getFazC());
				break;

			case MENU_MEAS_I_A:
				snprintf_P(&vLCDbuf[poz], 10, fcIA, sParam.MeasParam.getIA());
				break;

			case MENU_MEAS_I_B:
				snprintf_P(&vLCDbuf[poz], 10, fcIB, sParam.MeasParam.getIB());
				break;

			case MENU_MEAS_I_C:
				snprintf_P(&vLCDbuf[poz], 10, fcIC, sParam.MeasParam.getIC());
				break;

			case MENU_MEAS_U_A:
				snprintf_P(&vLCDbuf[poz], 10, fcUA, sParam.MeasParam.getUA());
				break;

			case MENU_MEAS_U_B:
				snprintf_P(&vLCDbuf[poz], 10, fcUB, sParam.MeasParam.getUB());
				break;

			case MENU_MEAS_U_C:
				snprintf_P(&vLCDbuf[poz], 10, fcUC, sParam.MeasParam.getUC());
				break;

			case MENU_MEAS_PARAM_NO:
				// ��������, ������ �������� �� ����
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

	snprintf_P(&vLCDbuf[poz], 4, device->pName);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;

	if (sParam.Glb.Status.getNumFaults() != 0)
	{
		if (blink_)	{
			text = sParam.Glb.Status.pFaultText;
			x = sParam.Glb.Status.getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		} else {
			y = sParam.Glb.Status.getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'g', y);
		}
	}
	else if (device->getNumFaults() != 0)
	{
		if (blink_)	{
			text = device->pFaultText;
			x = device->getFault();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		} else {
			y = device->getFaults();
			snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'l', y);
		}
	}
	else if ( (sParam.Glb.Status.getNumWarnings() != 0) && (blink_) )
	{
		if (sParam.Glb.Status.getNumWarnings() == 1) {
			text = sParam.Glb.Status.pWarningText;
			x = sParam.Glb.Status.getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		} else {
			y = sParam.Glb.Status.getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else if ( (device->getNumWarnings()!= 0) && (blink_) )
	{
		if (device->getNumWarnings() == 1) {
			text = device->pWarningText;
			x = device->getWarning();
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		} else {
			y = device->getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'l', y);
		}
	}
	else
	{
		text = device->pStateText;
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
	snprintf_P(&vLCDbuf[poz], 4, device->pName);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;
	poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
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
