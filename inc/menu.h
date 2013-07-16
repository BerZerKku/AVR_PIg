/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "glbDefine.h"

/// ����� �� ����������������� ������� (* ����� ����� ���)
#define TIME_TO_REINIT_LCD 50

class clMenu
{
public:
	clMenu();

	/// ����
	void main();

	/// ��������� ���� ��������
	void setTypeDevice(eGB_TYPE_DEVICE device) { sParam.typeDevice = device; }

	/// ���������� ��������� ������� �� ����������
	uint8_t txCommand();

	/// ���������� ������� ��������� ����� � ���
	bool isConnectionBsp() const { return connectionBsp; }

	/// ��������� ����� ������� ����� � ���, True - ����
	void setConnectionBsp(bool f) { connectionBsp = f; }

	/// ���������� ����� ��������� ���������� ����
	stGBparam* getParamStruct() { return &sParam; }

private:
	// ��� ������
	eKEY key;

	// ���� �������� ���������� ����� � ���, True - ����
	bool connectionBsp;

	// ��������� �������
	bool cursorEnable;

	// ��������� �������
	uint8_t cursorLine;

	// ���-�� ������������ ����������
	uint8_t lineParam;

	// �������� ������ ����
	bool lvlCreate;

	// ��������� ����������
	stGBparam sParam;

	// ���������� ������� ����� �������������/��������������
	uint8_t getNumError(uint16_t val);

	// ������� ���������� ������
	void clearTextBuf();

	// ����� �� ����� ���������� ����������
	void printMeasParam(eGB_MEAS_PARAM par1 = GB_MEAS_PARAM_NO,
						eGB_MEAS_PARAM par2 = GB_MEAS_PARAM_NO);

	// ������ ����
	void lvlStart();
	void lvlFirst();
	void lvlJournal();
	void lvlJournalEvent();
	void lvlJournalDef();
	void lvlJournalPrm();
	void lvlJournalPrd();
	void lvlControl();
	void lvlSetup();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupDT();
	void lvlInfo();

	// ������� ������� ����
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
