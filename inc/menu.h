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

/// ���-�� �����, ������������ �� ������

/// ����� �� ����������������� ������� (* ����� ����� ���)
#define TIME_TO_REINIT_LCD 50

/// ������������ ���-�� ������� � ����
#define MAX_NUM_PUNKTS 12

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
	/// �� ��������� ����� ������ ����� ������ �� ��������� ���������� �
	/// ���������� ��������� ���������
	bool setTypeDevice(eGB_TYPE_DEVICE device = AVANT_NO);

	/// ���������� ��������� ������� �� ����������
	eGB_COM getTxCommand();

	/// ���������� ������� ��������� ����� � ���
	bool isConnectionBsp() const { return connectionBsp_; }

	/// ��������� ����� ������� ����� � ���, True - ����
	void setConnectionBsp(bool f) { connectionBsp_ = f; }

	// ��������� ����������
	stGBparam sParam;

private:
	// ��� ������
	eKEY key_;

	// ���� ������� �������
	bool blink_;

	// ���� �������� ���������� ����� � ���, True - ����
	bool connectionBsp_;

	// true - ���������� ������� �� ����� ������
	bool cursorEnable_;

	// ������� ��������� ������� (����� ������)
	uint8_t cursorLine_;

	// ���-�� ������������ ����������
	uint8_t lineParam_;

	// ���� true - ���������� ������� ������� ����
	bool lvlCreate_;

	// ���������� ���������
	eMENU_MEAS_PARAM measParam[6];

	// ���-�� ������� � ������� ����
	uint8_t numPunkts_;

	// ������ � ������� ����
	PGM_P punkt_[MAX_NUM_PUNKTS];

	// ������� ���������� ������
	void clearTextBuf();

	// ����� �� ����� ����������� ���������
	void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

	// ����� �� ����� �������� ��������� ���������
	void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

	// ����� � ������ ���� "�����" �������� ������ ���������
	void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

	// ������ ����
	void lvlError();
	void lvlStart();
	void lvlFirst();
	void lvlJournal();
	void lvlJournalEvent();
	void lvlJournalDef();
	void lvlJournalPrm();
	void lvlJournalPrd();
	void lvlControl();
	void lvlSetup();
	void lvlRegime();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupDT();
	void lvlInfo();

	// ����������� ������� �����
	void cursorLineUp()
	{
		cursorLine_ = (cursorLine_ > 1) ? cursorLine_ - 1 : numPunkts_;
	};

	// ���������� ������� ����
	void cursorLineDown()
	{
		cursorLine_ = (cursorLine_ < numPunkts_) ? cursorLine_ + 1 : 1;
	}

	// ����� �� ����� ������� ������� ���� � �������
	void printPunkts();

	// ���������� ������� ����� �������������/��������������
	uint8_t getNumError(uint16_t val);

	// ������� ������� ����
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
