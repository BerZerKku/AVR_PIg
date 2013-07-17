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

/// ���������� ���������
enum eMENU_MEAS_PARAM
{
	MENU_MEAS_PARAM_NO,
	MENU_MEAS_PARAM_DATE,
	MENU_MEAS_PARAM_TIME,
	MENU_MEAS_PARAM_UZ,
	MENU_MEAS_PARAM_UC,
	MENU_MEAS_PARAM_UN,
	MENU_MEAS_PARAM_UOUT,
	MENU_MEAS_PARAM_IOUT,
	MENU_MEAS_PARAM_ROUT
};

class clMenu
{
public:
	clMenu();

	/// ����
	void main();

	/// ��������� ���� ��������
	bool setTypeDevice(eGB_TYPE_DEVICE device);

	/// ���������� ��������� ������� �� ����������
	uint8_t txCommand();

	/// ���������� ������� ��������� ����� � ���
	bool isConnectionBsp() const { return connectionBsp_; }

	/// ��������� ����� ������� ����� � ���, True - ����
	void setConnectionBsp(bool f) { connectionBsp_ = f; }

	// ��������� ����������
	stGBparam sParam;

private:
	// ��� ������
	eKEY key_;

	// ���� �������� ���������� ����� � ���, True - ����
	bool connectionBsp_;

	// ��������� �������
	bool cursorEnable_;

	// ��������� �������
	uint8_t cursorLine_;

	// ���-�� ������������ ����������
	uint8_t lineParam_;

	// �������� ������ ����
	bool lvlCreate_;

	// ���������� ���������
	eMENU_MEAS_PARAM measParam[6];

	// ���������� ������� ����� �������������/��������������
	uint8_t getNumError(uint16_t val);

	// ������� ���������� ������
	void clearTextBuf();

	// ����� �� ����� ����������� ���������
	void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

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
