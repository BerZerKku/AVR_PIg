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

	sParam.Rps.Status.name = PSTR("���");

#ifdef DEBUG
	// � ������ ������� �������� ���������� ���������
	vLCDsetLed(LED_ON);
#else
	// ��������� ������ ������ ���������
	vLCDsetLed(LED_REGIME);
#endif

	sParam.local.setFlashParams(fParams);
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
	measParam[2 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_T_BACK;
	measParam[3 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_FAZ_A;
	measParam[4 + MAX_NUM_MEAS_PARAM] 	= MENU_MEAS_D_FI;
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

	lvlCreate_ = true;

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
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_RESET_IND:
			sParam.TxComBuf.addFastCom(GB_COM_SET_PRD_RES_IND);
			break;

		case KEY_PUSK:
			sParam.TxComBuf.addFastCom(GB_COM_SET_REG_ENABLED);
			break;

		case KEY_POWER_ON:
			sParam.TxComBuf.setInt8(GB_CONTROL_POWER_ON);
			sParam.TxComBuf.addFastCom(GB_COM_SET_CONTROL);
			break;

		case KEY_POWER_OFF:
			sParam.TxComBuf.setInt8(GB_CONTROL_POWER_OFF);
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
				lvlMenu = &clMenu::lvlJournal;
				lvlCreate_ = true;
			} else if (name == punkt2) {
				lvlMenu = &clMenu::lvlControl;
				lvlCreate_ = true;
			} else if (name == punkt3) {
				lvlMenu = &clMenu::lvlSetup;
				lvlCreate_ = true;
			} else if (name == punkt4) {
				lvlMenu = &clMenu::lvlInfo;
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
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_MENU: {
			lvlMenu = &clMenu::lvlStart;
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
				pEnterFunc = &clMenu::enterPassword;
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
			val = (eGB_REGIME_ENTER) EnterParam.getValue();

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
			EnterParam.list = fcRegimeEnter[min];
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
	//							    12345678901234567890
	static char punkt1[] PROGMEM = "%d. �����";
	static char punkt2[] PROGMEM = "%d. ��������";
	static char punkt3[] PROGMEM = "%d. �������� �����";
	static char punkt4[] PROGMEM = "%d. ����� ����.����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		cursorLine_ = 1;
		cursorEnable_ = true;
		lineParam_ = 1;

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		// ��������� ����, � ����������� �� �������� ����������
		Punkts_.clear();
		Punkts_.add(punkt2);
		Punkts_.add(punkt3);
		Punkts_.add(punkt4);
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
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_MENU:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
			break;

		case KEY_ENTER:
			if (name == punkt1) {
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate_ = true;
			} else if (name == punkt2) {
				lvlMenu = &clMenu::lvlSetupTerminal;
				lvlCreate_ = true;
			} else if (name == punkt3) {
				lvlMenu = &clMenu::lvlSetupSourcePusk;
				lvlCreate_ = true;
			} else if (name == punkt4) {
				lvlMenu = &clMenu::lvlSetupFindPhase;
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

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		sParam.local.clearParams();
		sParam.local.addParam(GB_PARAM_PUSK_THD);
		sParam.local.addParam(GB_PARAM_SHORT_THD_DFZ);
		sParam.local.addParam(GB_PARAM_SHORT_THD_NZ);
		sParam.local.addParam(GB_PARAM_PHASE_EDGE);
		sParam.local.addParam(GB_PARAM_I_MAX);
		sParam.local.addParam(GB_PARAM_I_OTH);
		sParam.local.addParam(GB_PARAM_Z_EDGE);
		sParam.local.addParam(GB_PARAM_DELAY);
		sParam.local.addParam(GB_PARAM_I_FAZ_MIN);
		sParam.local.addParam(GB_PARAM_U_FAZ_MIN);
		sParam.local.addParam(GB_PARAM_U_FAZ_DOP);
		sParam.local.addParam(GB_PARAM_TIME_ON_OFF);
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

/** ������� ����. ������������ ���������.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupTerminal() {
	static char title[] PROGMEM = "���������\\��������";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		sParam.local.clearParams();
		sParam.local.addParam(GB_PARAM_FREQ);
		sParam.local.addParam(GB_PARAM_SUBST_NUM);
		sParam.local.addParam(GB_PARAM_RPS_TYPE);
		sParam.local.addParam(GB_PARAM_BLOCK_ON);
		sParam.local.addParam(GB_PARAM_PHASE_OFF);
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

/** ������� ����. ����� ��������� �����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupSourcePusk() {
	static char title[] PROGMEM = "���������\\���. �����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		sParam.local.clearParams();
		sParam.local.addParam(GB_PARAM_PUSK_OP);
		sParam.local.addParam(GB_PARAM_PUSK_OP_TRIG);
		sParam.local.addParam(GB_PARAM_PUSK_NP);
		sParam.local.addParam(GB_PARAM_PUSK_NP_TRIG);
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

/** ������� ����. ����� ������������ ����.
 * 	@param ���
 * 	@return ���
 */
void clMenu::lvlSetupFindPhase() {
	static char title[] PROGMEM = "���������\\����� ����";

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		sParam.local.clearParams();
		sParam.local.addParam(GB_PARAM_FIND_PH_ANGLE);
		sParam.local.addParam(GB_PARAM_FIND_PH_I_MAX);
		sParam.local.addParam(GB_PARAM_FIND_PH_I_OTH);
		sParam.local.addParam(GB_PARAM_FIND_PH_Z);
		sParam.local.addParam(GB_PARAM_FIND_PH_R_X);
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
//			sParam.TxComBuf.setInt8(BIN_TO_BCD(EnterParam.getValueEnter()),
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

	if (lvlCreate_) {
		lvlCreate_ = false;
		EnterParam.setDisable();

		vLCDclear();
		vLCDdrawBoard(lineParam_);

		sParam.TxComBuf.clear();

		sParam.local.clearParams();
		// ���� ����������� ����� �� ��������� ����
		// ���������� ��������� ������
		// � USB ������: 19200 ���/�, 8 ���, 2 ����-����, ��������-���
		sParam.local.addParam(GB_PARAM_INTF_INTERFACE);
		if (sParam.Uart.Interface.get() == TInterface::RS485) {
			sParam.local.addParam(GB_PARAM_INTF_PROTOCOL);
//			sParam.local.addParam(GB_PARAM_NET_ADDRESS);
			sParam.local.addParam(GB_PARAM_INTF_BAUDRATE);
			sParam.local.addParam(GB_PARAM_INTF_DATA_BITS);
			sParam.local.addParam(GB_PARAM_INTF_PARITY);
			sParam.local.addParam(GB_PARAM_INTF_STOP_BITS);
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	LocalParams *lp = &sParam.local;
	if (lp->getCom() == GB_COM_NO) {
		// ��� ���������� �������� �������� ��������� ����������� � ������
		eGB_PARAM param = lp->getParam();
		if (param == GB_PARAM_INTF_INTERFACE) {
			lp->setValue(sParam.Uart.Interface.get());
		} else if (param == GB_PARAM_INTF_PROTOCOL) {
			lp->setValue(sParam.Uart.Protocol.get());
		} else if (param == GB_PARAM_INTF_BAUDRATE) {
			lp->setValue(sParam.Uart.BaudRate.get());
		} else if (param == GB_PARAM_INTF_DATA_BITS) {
			lp->setValue(sParam.Uart.DataBits.get());
		} else if (param == GB_PARAM_INTF_PARITY) {
			lp->setValue(sParam.Uart.Parity.get());
		} else if (param == GB_PARAM_INTF_STOP_BITS) {
			lp->setValue(sParam.Uart.StopBits.get());
		}
	}

	setupParam();

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

/** ���� ���������.
 * 	@param ���
 * 	@return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterValue() {
	static char enterList[] 	PROGMEM = "����: %S";
	static char enterInt[] 		PROGMEM = "����: %01d";
	static char enterFloat10P[] PROGMEM = "����: %01u.%01u";
	static char enterFloat10M[] PROGMEM = "����: -%01u.%01u";
	static char enterFloat100P[] PROGMEM = "����: %01u.%02u";
	static char enterFloat100M[] PROGMEM = "����: -%01u.%02u";

	eMENU_ENTER_PARAM status = EnterParam.getStatus();
	if (status == MENU_ENTER_PARAM_INT) {
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
	} else if (status == MENU_ENTER_PARAM_FLOAT10) {
		int16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			if (val >= 0) {
				snprintf_P(&vLCDbuf[poz], 21, enterFloat10P, val/10, val%10);
			} else {
				val = -val;
				snprintf_P(&vLCDbuf[poz], 21, enterFloat10M, val/10, val%10);
			}

		}
	} else if (status == MENU_ENTER_PARAM_FLOAT100) {
		int16_t val = EnterParam.getValue();
		uint8_t num = EnterParam.getValueNumSymbols();

		// ���� ���-�� �������� ������� �� ���������� ��������, �������� ����
		if ((num >= 5) || (num == 0)) {
			key_ = KEY_CANCEL;
		} else {
			clearLine(NUM_TEXT_LINES);
			uint8_t poz = 100;
			if (val >= 0) {
				snprintf_P(&vLCDbuf[poz], 21, enterFloat100P, val/100, val%100);
			} else {
				val = -val;
				snprintf_P(&vLCDbuf[poz], 21, enterFloat100M, val/100, val%100);
			}
		}
	} else if (status == MENU_ENTER_PARAM_LIST) {
		uint16_t val = EnterParam.getValue();
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		val = (val - EnterParam.getValueMin()) * STRING_LENGHT;
		snprintf_P(&vLCDbuf[poz], 21, enterList, EnterParam.list + val);
	} else if (status == MENU_ENTER_PARAM_LIST_2) {
		uint16_t val = EnterParam.listValue[EnterParam.getValue()];
		clearLine(NUM_TEXT_LINES);
		uint8_t poz = 100;
		snprintf_P(&vLCDbuf[poz], 21, enterList,
				EnterParam.list + STRING_LENGHT * val);
	} else
		key_ = KEY_CANCEL;

	switch(key_) {
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

			case MENU_MEAS_T_BACK:
				snprintf_P(&vLCDbuf[poz], 10, fcTBack,
						sParam.MeasParam.getTBack());
				break;

			case MENU_MEAS_D_FI:
				snprintf_P(&vLCDbuf[poz], 10, fcDFi, sParam.MeasParam.getDFi());
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

	snprintf_P(&vLCDbuf[poz], 4, device->name);
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
	snprintf_P(&vLCDbuf[poz], 4, device->name);
	poz += 3;
	snprintf(&vLCDbuf[poz], 2, ":");
	poz += 1;
	poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printParam() {
	static prog_uint8_t MAX_CHARS = 21;

	snprintf_P(&vLCDbuf[20], MAX_CHARS, PSTR("��������:%u �����:%u"),
			sParam.local.getNumOfCurrParam(), sParam.local.getNumOfParams());

	snprintf_P(&vLCDbuf[40], MAX_CHARS, sParam.local.getNameOfParam());

	printSameNumber(60);
	printRange(80);
	printValue(100);
}

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printSameNumber(uint8_t pos) {
	static prog_uint8_t MAX_CHARS = 21;

	if (sParam.local.getNumOfSameParams() > 1) {
		snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("�����: %u/%u"),
				sParam.local.getNumOfCurrSameParam(),
				sParam.local.getNumOfSameParams());
	}
}

//	����� �� ����� ��������� �������� ���������.
void clMenu::printRange(uint8_t pos) {
	static prog_uint8_t MAX_CHARS = 11;

	static const char fcList[]  PROGMEM = "������";
	static const char fcOnOff[] PROGMEM = "���./����.";
	static const char fcInt[]	PROGMEM = "%d..%d%S";
	static const char fcFloat10[]	PROGMEM = "%d.%01d..%d.%01d%S";
	static const char fcFloat10_0[] PROGMEM = "%d..%d.%01d%S";
	static const char fcFloat100[]	PROGMEM = "%d.%02d..%d.%02d%S";
	static const char fcFloat100_0[] PROGMEM = "%d..%d.%02d%S";



	LocalParams *lp = &sParam.local;
	int16_t min = lp->getMin();
	int16_t max = lp->getMax();
	uint16_t dmin = 0;
	uint16_t dmax = 0;

	PGM_P str = fcNullBuf;
	PGM_P dim = fcDimension[lp->getDim()];
	pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));


	switch(lp->getRangeType()) {
		case Param::RANGE_LIST:
			str = fcList;
			break;

		case Param::RANGE_ON_OFF:
			str = fcOnOff;
			break;

		case Param::RANGE_INT:
			str = fcInt;
			break;

		case Param::RANGE_INT_NO_DIM:
			str = fcInt;
			dim = fcNullBuf;
			break;

		case Param::RANGE_FLOAT10:
			dmin = min%10;
			min /= 10;
			dmax = max%10;
			max /= 10;
			str = fcFloat10;
			break;

		case Param::RANGE_FLOAT100:
			dmin = min%100;
			min /= 100;
			dmax = max%100;
			max /= 100;
			str = fcFloat100;
			break;
	}

	if (str == fcFloat10) {
		if ((dmin == 0) && (min == 0)) {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, fcFloat10_0, min, max, dmax, dim);
		} else {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, min, dmin, max, dmax, dim);
		}
	} else if (str == fcFloat100) {
		if ((dmin == 0) && (min == 0)) {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, fcFloat100_0, min, max, dmax, dim);
		} else {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, min, dmin, max, dmax, dim);
		}
	} else {
		snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, min, max, dim);
	}
}

// ����� �� ����� �������� �������� ���������.
void clMenu::printValue(uint8_t pos) {
	static prog_uint8_t MAX_CHARS = 11;

	int16_t val = sParam.local.getValue();
	PGM_P dim = fcDimension[sParam.local.getDim()];
	PGM_P str = fcNullBuf;

	pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));

	LocalParams::STATE state = sParam.local.getState();

	if (state == LocalParams::STATE_READ_PARAM) {
		if (blink_) {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������."));
		} else {
			snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������.."));
		}
	} else if ((state == LocalParams::STATE_ERROR) && (blink_)) {
		snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������!!!"));
	} else {
		// ����� ��������
		switch(sParam.local.getParamType()) {
			case Param::PARAM_BITE:
			case Param::PARAM_BITES: // DOWN
			case Param::PARAM_LIST:
				val -= sParam.local.getMin();
				str = sParam.local.getListOfValues() + (val * STRING_LENGHT);
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str);
				break;
			case Param::PARAM_INT:
				str = PSTR("%d%S");
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, val, dim);
				break;
			case Param::PARAM_FLOAT10:
				if (val < 0) {
					str = PSTR("-%d.%01d%S");
					val = -val;
				} else {
					str = PSTR("%d.%01d%S");
				}
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, val/10, val%10, dim);
				break;
			case Param::PARAM_FLOAT100:
				if (val < 0) {
					str = PSTR("-%d.%02d%S");
					val = -val;
				} else {
					str = PSTR("%d.%02d%S");
				}
				snprintf_P(&vLCDbuf[pos], MAX_CHARS, str, val/100, val%100, dim);
				break;
			case Param::PARAM_NO:
				break;
		}
	}
}


// ��������� ���������� ��� ����� �������� � ����������.
void clMenu::enterParameter() {
	LocalParams *lp = &sParam.local;

	if ((lp->getChangeCond() == Param::CHANGE_COND_REG_DISABLE) &&
			(sParam.Glb.Status.getRegime() != GB_REGIME_DISABLED)) {
		printMessage();
	} else {

		switch(lp->getParamType()) {
			case Param::PARAM_BITE:		// DOWN
			case Param::PARAM_BITES: 	// DOWN
			case Param::PARAM_LIST:
				EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
				break;
			case Param::PARAM_INT:
				EnterParam.setEnable(MENU_ENTER_PARAM_INT);
				break;
			case Param::PARAM_FLOAT10:
				EnterParam.setEnable(MENU_ENTER_PARAM_FLOAT10);
				break;
			case Param::PARAM_FLOAT100:
				EnterParam.setEnable(MENU_ENTER_PARAM_FLOAT100);
				break;
			case Param::PARAM_NO:
				break;

		}

		if (EnterParam.isEnable()) {
			int16_t min = lp->getMin();
			int16_t val = lp->getValue();
			int16_t max = lp->getMax();

			// ��� ��������� ���������� � ������ ������ �������� ��������,
			// ��������������� �������.
			if (lp->getState() != LocalParams::STATE_NO_ERROR) {
				val = min;
			}

			pEnterFunc = &clMenu::enterValue;
			EnterParam.setValueRange(min, max);
			EnterParam.setValue(val);
			EnterParam.list = lp->getListOfValues();
			EnterParam.setFract(lp->getFract());
			EnterParam.setDisc(lp->getDisc());
		}
	}
}

// ������ � ���� ��������� ����������.
void clMenu::setupParam() {

	if (isMessage()) {
		// ����� �� ����� ��������� � ������������� ��������� ���������,
		// ��� ���� ������� ������ ����� ���������������.
		static char message[3][21] PROGMEM = {
				//2345678901234567890
				" �������� ��������  ",
				"  ����� ������ �    ",
				"  ������ �������    " };

		for(uint8_t i = 0, pos = 40; i < SIZE_OF(message); i++, pos += 20) {
			snprintf_P(&vLCDbuf[pos], 21, message[i]);
		}

		key_ = KEY_NO;
	} else {
		// ����� �� ����� ���������� � ������� ���������
		printParam();
	}

	if (EnterParam.isEnable()) {
		eMENU_ENTER_PARAM stat = enterValue();

		if (stat == MENU_ENTER_PARAM_READY) {
			eGB_COM com = sParam.local.getCom();

			// ���� � ��������� ���� ������� ������ � ������ ���, ����
			// ������ �� ������ � ���.
			// ����� ���� ������ � ������.
			if (com != GB_COM_NO) {
				// ���������� ������ ��� ������ � ���.
				uint8_t dop = sParam.local.getSendDop();

				switch(sParam.local.getSendWrType()) {
					case GB_SEND_WR_INT8:
						sParam.TxComBuf.setInt8(EnterParam.getValueEnter());
						break;

					case GB_SEND_WR_INT8_DOP:
					case GB_SEND_WR_DOP_INT8:
						sParam.TxComBuf.setInt8(EnterParam.getValueEnter(), 0);
						sParam.TxComBuf.setInt8(dop, 1);
						break;

					case GB_SEND_WR_INT16_BE:
						sParam.TxComBuf.setInt8(EnterParam.getValue() >> 8, 0);
						sParam.TxComBuf.setInt8(EnterParam.getValue(), 1);
						break;

					case GB_SEND_WR_DOP_BITES: {
						uint8_t val = sParam.local.getValueB();
						uint8_t pos = sParam.local.getNumOfCurrSameParam() - 1;
						if (EnterParam.getValue()) {
							val |= (1 << (pos % 8));
						} else {
							val &= ~(1 << (pos % 8));
						}
						sParam.TxComBuf.setInt8(val, 0);
						sParam.TxComBuf.setInt8(1 + pos/8 , 1);
					}
					break;

					case GB_SEND_WR_DOP_BITE: {
						uint8_t val = EnterParam.getValue();
						val = sParam.local.getNewBiteValue(val);
						sParam.TxComBuf.setInt8(val, 0);
						sParam.TxComBuf.setInt8(dop, 1);
					}
					break;

					case GB_SEND_WR_NO:
						com = GB_COM_NO;
						break;
				}

				if (com != GB_COM_NO) {
					com = (eGB_COM) (com + GB_COM_MASK_GROUP_WRITE_PARAM);
					sParam.TxComBuf.addFastCom(com);
					sParam.TxComBuf.setSendType(sParam.local.getSendWrType());
				}
			} else {
				eGB_PARAM param= sParam.local.getParam();
				if (param != GB_PARAM_NULL_PARAM) {
					// ������ ���������� � ������.
					uint8_t tmp = EnterParam.getValueEnter();
					if (param == GB_PARAM_INTF_INTERFACE) {
						TInterface::INTERFACE val;
						val = (TInterface::INTERFACE) (tmp);
						// ���� ��������� ��������, ������� ����
						if (val != sParam.Uart.Interface.get()) {
							sParam.Uart.Interface.set(val);
							lvlCreate_ = true;
						}
					} else if (param == GB_PARAM_INTF_PROTOCOL) {
						sParam.Uart.Protocol.set((TProtocol::PROTOCOL) (tmp));
					} else if (param == GB_PARAM_INTF_BAUDRATE) {
						sParam.Uart.BaudRate.set((TBaudRate::BAUD_RATE) (tmp));
					} else if (param == GB_PARAM_INTF_DATA_BITS) {
						sParam.Uart.DataBits.set((TDataBits::DATA_BITS) (tmp));
					} else if (param == GB_PARAM_INTF_PARITY) {
						sParam.Uart.Parity.set((TParity::PARITY) (tmp));
					} else if (param == GB_PARAM_INTF_STOP_BITS) {
						sParam.Uart.StopBits.set((TStopBits::STOP_BITS) (tmp));
					}
				}
			}
			EnterParam.setDisable();
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

	// ������� �������, �� ������� �������� ������ ���� + ������� �������
	if (sParam.local.getState() == LocalParams::STATE_READ_PARAM) {

		eGB_COM com = sParam.local.getCom();
		sParam.TxComBuf.addFastCom(com);
		sParam.TxComBuf.addCom1(com, 0);
	}

	// ����� �� �������� ������ ����, ���� ���-�� ���������� ����� 0
	if (sParam.local.getNumOfParams() == 0) {
		key_ = KEY_CANCEL;
	}
}
