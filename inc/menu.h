/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"

/// ����� �� ����������������� ������� (* ����� ����� ���)
#define TIME_TO_REINIT_LCD 50

/// ��������� ���������� ���
struct stMNUparam
{
	// ����/�����
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

	// ���������� ���������
	int8_t voltDef;
	int8_t voltCF;
	int8_t voltNoise;
	uint8_t voltOutInt;
	uint8_t voltOutFract;
	uint16_t curOut;
	uint16_t resistOut;
};

enum eMNU_TYPE_DEVICE
{
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_K400
};

class clMenu
{
public:
	clMenu(stMNUparam *param);

	// ����
	void main	();

	// ���������� ���-�� ����� ���������� ��� ����������
	uint8_t getLineParam() { return lineParam; }

	// ��������� ���� ��������
	void setTypeDevice(eMNU_TYPE_DEVICE device) {typeDevice = device;}

private:
	// ��� ������
	eKEY key;
	// ���-�� ������������ ����������
	uint8_t lineParam;
	// �������� ������ ����
	bool lvlCreate;
	// ��������� �������
	uint8_t cursorLine;
	// ��������� �������
	bool cursorEnable;
	// ��� ��������
	eMNU_TYPE_DEVICE typeDevice;

	stMNUparam * const sParam;

	// ������� ���������� ������
	void clearTextBuf();

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
