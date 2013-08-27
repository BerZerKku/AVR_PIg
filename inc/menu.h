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
	MENU_ENTER_PARAM_NO,		// ������ ��������� ���������
	MENU_ENTER_PARAM_WORK,		// ��������� ���������
	MENU_ENTER_PARAM_READY,		// ���������� �������� ��������
	MENU_ENTER_PARAM_MESSAGE	// ����� ��������� �� �����
};

// ��������� ���������� ��� ����� ��������
class TEnterParam
{
public:
	TEnterParam()
	{
		setDisable();
	}

	bool isEnable()
	{
		// �������� �������� ������� �� ����������� ��������
		if ( (status_ <  MENU_ENTER_PARAM_NO) ||
				(status_ > MENU_ENTER_PARAM_MESSAGE) )
			status_ = MENU_ENTER_PARAM_NO;
		return (status_ != MENU_ENTER_PARAM_NO);
	}
	void setEnable() { status_ = MENU_ENTER_PARAM_WORK; }
	void setDisable() { status_ = MENU_ENTER_PARAM_NO; }

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

	// ���������� �������� �������� � ������ ������������ � ��������
	uint8_t getValueEnter()
	{
		return ((val_ / disc_) * disc_) / fract_;
	}

	// ����������/���������� �������� ��������
	uint16_t incValue()
	{
		if (val_ <= (max_ - disc_))
			val_ += disc_;
		else
			val_ = min_;
		return val_;
	}
	uint16_t decValue()
	{
		if (val_ >= (min_ + disc_))
			val_ -= disc_;
		else
			val_ = max_;
		return val_;
	}

	// ������/���������� ��������������� �����
	void setDopByte(uint8_t byte) { dopByte_ = byte; }
	uint8_t getDopByte() const { return dopByte_; }

	// ������/���������� ������������
	void setDisc(uint8_t disc) { disc_ = disc; }
	uint8_t getDisc() const { return disc_; }

	// ������/���������� ��������
	void setFract(uint8_t fract) { fract_ = fract; }
	uint8_t getFract() const { return fract_; }

	// ����� ��������� �� �����
	void printMessage() { status_ = MENU_ENTER_PARAM_MESSAGE; cnt_ = 0; }

	// ���������� ��������� �������
	eMENU_ENTER_PARAM getStatus() const { return status_; }
	void setEnterValueReady() { status_ = MENU_ENTER_PARAM_READY; }

	// ������� �� ��������
	eGB_COM com;

	// ������� �������
	uint8_t cnt_;
private:
	// true - ���� ���� ��������
	bool enable_;

	// ������� ��������
	uint16_t val_;

	// ������������ ��������
	uint16_t max_;

	// ����������� ��������
	uint16_t min_;

	// ���-�� ��������
	uint8_t numSymbols_;

	// ���� � ������������� �����������
	uint8_t dopByte_;

	// ������������
	uint8_t disc_;

	// ��������
	uint8_t fract_;



	// ������� ������ ����������
	eMENU_ENTER_PARAM status_;
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
