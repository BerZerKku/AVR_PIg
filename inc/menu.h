/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "protocolS.h"

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

	// ��������� �����
	uint16_t glb_avar;
	uint16_t glb_warn;

	// ��������� ������
	uint16_t def_avar;
	uint16_t def_warn;
	uint8_t def_regime;
	uint8_t def_sost;
	uint8_t def_dop;

	// ��������� ���������
	uint16_t prm_avar;
	uint16_t prm_warn;
	uint8_t prm_regime;
	uint8_t prm_sost;
	uint8_t prm_dop;

	// ��������� �����������
	uint16_t prd_avar;
	uint16_t prd_warn;
	uint8_t prd_regime;
	uint8_t prd_sost;
	uint8_t prd_dop;


};

enum eMNU_TYPE_DEVICE
{
	AVANT_R400 = 1,
	AVANT_R400_MSK,
	AVANT_RZSK,
	AVANT_RZSK_OPTIC,
	AVANT_K400,
	AVANT_K400_OPTIC
};

enum eMNU_TYPE_LINE
{
	UM,
	OPTIC
};


class clMenu
{
public:
	clMenu(clProtocolS *protocol);

	// ����
	void main();

	// ���������� ���-�� ����� ���������� ��� ����������
	uint8_t getLineParam() const
	{
		return lineParam;
	}

	// ��������� ���� ��������
	void setTypeDevice(eMNU_TYPE_DEVICE device)
	{
		typeDevice = device;
	}

	// ���������� ������� ��� ��������
	eMNU_TYPE_DEVICE getTypeDevice() const
	{
		return typeDevice;
	}

	// ���������� ��������� ������� �� ����������
	uint8_t txCommand();

	// ��������� ���������� ��������
	stMNUparam sParam;

	// �������� ������ � ���
	const clProtocolS *pProtocol;

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

	// ������� �� ����������
	uint8_t com;

	// ���������� ������� ����� �������������/��������������
	uint8_t getNumError(uint16_t val);

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
