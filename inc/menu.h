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
#define MAX_NUM_PUNKTS 15

/// ����� ������ ���.��������� �� �����
#define TIME_MESSAGE 10

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

/// ����� ������ ������� ����� ����������
enum eMENU_ENTER_PARAM
{
	MENU_ENTER_PARAM_NO,	// ������ ��������� ���������
	MENU_ENTER_PARAM_WORK,	// ��������� ���������
	MENU_ENTER_PARAM_READY	// ���������� �������� ��������
};

// ��������� ���������� ��� ����� ��������
class TEnterParam
{
public:
	bool isEnable() const { return enable_; }
	void setEnable()
	{
		enable_ = true;
		cursorPos_ = 0;

	};
	void setDisable() { enable_ = false; }

	// ������� ������ � ������� ���������� �������
	uint8_t getCursorPos() const { return cursorPos_; }
	void incCursorPos() { if (cursorPos_ < (numSymbols_ - 1)) cursorPos_++; }
	void decCursorPos() { if (cursorPos_ > 0) cursorPos_--; }

	// �������� ��������
	void setValueRange(uint16_t min, uint16_t max)
	{
		uint8_t num = 0;
		max_ = max;
		min_ = min;
		// ���������� ������������� ���-�� �������� ��� �����
		while (max > 0)
		{
			num++;
			max /= 10;
		}
		numSymbols_ = num;
	}
	uint16_t getValueMin() const { return min_; }
	uint16_t getValueMax() const { return max_; }

	// ���-�� �������� ��������
	uint16_t getValueNumSymbols() const { return numSymbols_; }

	// ��������� �������� ��������, �������� �������� ������ ���� ����� �� !
	void setValue(uint16_t val)
	{
		if ( (val < min_) || (val > max_) )
			val = min_;
		val_ = val;
	}

	// ���������� ������� ��������
	uint16_t getValue() const { return val_; }

	// ����������/���������� �������� ��������
	uint16_t incValue()
	{
		if (val_ < max_)
			val_++;
		return val_;
	}
	uint16_t decValue()
	{
		if (val_ > min_)
			val_--;
		return val_;
	}


	// ������ ��������
	uint8_t uint8[8];

	// ������� �� ��������
	eGB_COM com;

private:
	// true - ���� ���� ��������
	bool enable_;

	// ������� ��������� �������
	uint8_t cursorPos_;

	// ������� ��������
	uint16_t val_;

	// ������������ ��������
	uint16_t max_;

	// ����������� ��������
	uint16_t min_;

	// ���-�� ��������
	uint8_t numSymbols_;
};

// ����� ����
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

	// ������� �������� (�������� ����� ������� ������� ���������, ������� � 1)
	uint8_t curCom_;

	// ���� true - ���������� ������� ������� ����
	bool lvlCreate_;

	// ����� ������ ���.��������� �� ����� (�������� ��������� �� ������)
	uint8_t delay_;

	// ���������� ���������
	eMENU_MEAS_PARAM measParam[6];

	// ���-�� ������� � ������� ����
	uint8_t numPunkts_;

	// ������ � ������� ����
	PGM_P punkt_[MAX_NUM_PUNKTS];

	// ��������� ��� ����� ����� ��������
	TEnterParam enterParam;

	// ������� ���������� ������
	void clearTextBuf();

	// ������� ������
	void clearLine(uint8_t line);

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

	// ���� ������ ��������
	eMENU_ENTER_PARAM enterValue();

	// �������� ��� ����� ��������
	eMENU_ENTER_PARAM enterNo() { return MENU_ENTER_PARAM_NO; }

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

	// ������� ������� ����� ��������
	eMENU_ENTER_PARAM (clMenu:: *enterFunc_) ();

	// ������� ������� ����
	void (clMenu:: *lvlMenu) ();
};

#endif /* MENU_H_ */
