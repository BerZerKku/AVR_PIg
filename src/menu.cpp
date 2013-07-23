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

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING];

//clMenu::clMenu(clProtocolS *protocol) : pProtocol(protocol)
clMenu::clMenu()
{
	lvlMenu = &clMenu::lvlStart;
	lineParam_ = 3;


	lvlCreate_ = true;

	blink_ = false;

	// ������ ���������
	cursorEnable_= false;
	cursorLine_ = 0;

	// ������� ������ ��� ���
	key_ = KEY_NO;

	// ��� ���������� ��� �� ��������
	sParam.typeDevice = AVANT_NO;

	// ����� � ��� ��� ���
	connectionBsp_ = false;

	// ���������� ���������
	for(uint_fast8_t i = 0; i < 6; i++)
		measParam[i] = MENU_MEAS_PARAM_NO;

	// ��� ������ �������� ��������� ������� �������������� � ��������������
	// ��� ���� ��������� "������������" �����������
	for(uint_fast8_t i = 0; i < 16; i++)
	{
		sParam.glb.status.faultText[i] = fcUnknownFault;
		sParam.def.status.faultText[i] = fcUnknownFault;
		sParam.prm.status.faultText[i] = fcUnknownFault;
		sParam.prd.status.faultText[i] = fcUnknownFault;
	}

	for(uint_fast8_t i = 0; i < 8; i++)
	{
		sParam.def.status.warningText[i] = fcUnknownWarning;
		sParam.prm.status.warningText[i] = fcUnknownWarning;
		sParam.prd.status.warningText[i] = fcUnknownWarning;
	}

	// �������� ������� ��������� ������ ��� ���� ���������
	// ������ ������ ���� �������� ��������� � ��������� ������ ������
	// ���� fcUnknownSost
	sParam.def.status.stateText[0] =  fcDefSost00;
	sParam.def.status.stateText[1] =  fcDefSost01;
	sParam.def.status.stateText[2] =  fcDefSost02;
	sParam.def.status.stateText[3] =  fcDefSost03;
	sParam.def.status.stateText[4] =  fcDefSost04;
	sParam.def.status.stateText[5] =  fcDefSost05;
	sParam.def.status.stateText[6] =  fcDefSost06;
	sParam.def.status.stateText[7] =  fcDefSost07;
	sParam.def.status.stateText[8] =  fcDefSost08;
	sParam.def.status.stateText[9] =  fcDefSost09;
	sParam.def.status.stateText[10] = fcDefSost10;
	sParam.def.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	sParam.prm.status.stateText[0] =  fcPrmSost00;
	sParam.prm.status.stateText[1] =  fcPrmSost01;
	sParam.prm.status.stateText[2] =  fcPrmSost02;
	sParam.prm.status.stateText[3] =  fcPrmSost03;
	sParam.prm.status.stateText[4] =  fcPrmSost04;
	sParam.prm.status.stateText[5] =  fcPrmSost05;
	sParam.prm.status.stateText[6] =  fcPrmSost06;
	sParam.prm.status.stateText[7] =  fcPrmSost07;
	sParam.prm.status.stateText[8] =  fcPrmSost08;
	sParam.prm.status.stateText[9] =  fcPrmSost09;
	sParam.prm.status.stateText[10] = fcPrmSost10;
	sParam.prm.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	sParam.prd.status.stateText[0] =  fcPrdSost00;
	sParam.prd.status.stateText[1] =  fcPrdSost01;
	sParam.prd.status.stateText[2] =  fcPrdSost02;
	sParam.prd.status.stateText[3] =  fcPrdSost03;
	sParam.prd.status.stateText[4] =  fcPrdSost04;
	sParam.prd.status.stateText[5] =  fcPrdSost05;
	sParam.prd.status.stateText[6] =  fcPrdSost06;
	sParam.prd.status.stateText[7] =  fcPrdSost07;
	sParam.prd.status.stateText[8] =  fcPrdSost08;
	sParam.prd.status.stateText[9] =  fcPrdSost09;
	sParam.prd.status.stateText[10] = fcPrdSost10;
	sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE - 1] = fcUnknownSost;

	// �������� ����� �����������
	sParam.def.status.name = fcDeviceName00;
	sParam.prm.status.name = fcDeviceName01;
	sParam.prd.status.name = fcDeviceName02;

	// ��������� ���������� ���������
	vLCDsetLED(LED_ON);
}


/**	������ � ������� ������� ����
 * 	@param ���
 * 	@return ���
 */
void
clMenu::main(void)
{
	static const char fcNoConnectBsp[] PROGMEM = " ��� ����� � ���!!! ";

	// ������� ������� �� ����������������� ���
	static uint8_t reInit = false;

	blink_ =  ((reInit % 8) < 4) ? false : true;


	// ����������������� �������
	if (reInit > 0)
	{
		reInit--;
	}
	else
	{
		reInit = TIME_TO_REINIT_LCD - 1;
		vLCDinit();
	}

	if (!sParam.device)
		setTypeDevice();

	// ������� ��� � ����������
	// ���� ������ ����� ������ - ��������� ��������������� ���������
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		key_ = tmp;

		vLCDsetLED(LED_SWITCH);
	}

	// ������� ���������� ������

	clearTextBuf();

	// ������� ���������, ���� ������� ��� �������� �� ���������
	if (sParam.typeDevice != AVANT_NO)
		(this->*lvlMenu) ();
	else
		lvlError();
	key_ = KEY_NO;


#ifdef VIEW_DEBUG_PARAM
	// ����� ���������� ����������
	if (this->lvlMenu == &clMenu::lvlStart)
	{
		snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
		snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
		snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
		snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
		snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
		snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
		snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
	}
	else
	{
		snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
		snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
	}
#endif

	// ����� ��������� � ������ ���������� ����� � ���
	if (!isConnectionBsp() && (blink_))
		snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);

	// �������������� ������ �������� � ������ ��� ������ �� �����
	vLCDputchar(vLCDbuf, lineParam_);

	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
}

/** ��������� ���� �������� � ��������� ���� � ��� ������.
 * 	@param device ��� ����������
 * 	@return False � ������ ������, ����� True
 */
bool
clMenu::setTypeDevice(eGB_TYPE_DEVICE device)
{
	bool status = false;


	sParam.glb.status.setEnable(true);

	// ���� ����������� ��� �������� ����� �������, ������� ����� ������
	// �� ������� ��������.
	if (device == AVANT_NO)
	{
		if (sParam.def.status.isEnable())
		{
			if ((sParam.prm.status.isEnable()) ||
					(sParam.prd.status.isEnable()))
			{
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTIC)
					device = AVANT_RZSK_OPTIC;
				else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_RZSK;
			}
			else
			{
				// !!! ��� ��� ������ ���� ����������� �������� ��� ������
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_R400;
			}
		}
		else
		{
			if ((sParam.prd.status.isEnable()) ||
					(sParam.prm.status.isEnable()))
			{
				if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTIC)
					device = AVANT_K400_OPTIC;
				else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
					device = AVANT_K400;
			}
		}

		// ���� ������� ���������� ��������� � �����, �� ������ �� ������
		// ����� ������� �� ��������� �������
		if (device == sParam.typeDevice)
			status = true;
		else
		{
			sParam.typeDevice = device;
			lvlMenu = &clMenu::lvlStart;
			lvlCreate_ = true;
		}
	}

	if (!status)
	{
		if (device == AVANT_RZSK)
		{
			sParam.typeDevice = device;

			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			measParam[2] = MENU_MEAS_PARAM_UOUT;
			measParam[3] = MENU_MEAS_PARAM_IOUT;
			measParam[4] = MENU_MEAS_PARAM_UZ;
			measParam[5] = MENU_MEAS_PARAM_UC;

			// ���������� ������� ����� �������������� � ��������������
			sParam.glb.status.faultText[0] =  fcGlbFault0001;
			sParam.glb.status.faultText[1] =  fcGlbFault0002;
			sParam.glb.status.faultText[2] =  fcGlbFault0004;
			sParam.glb.status.faultText[3] =  fcGlbFault0008;
			sParam.glb.status.faultText[4] =  fcGlbFault0010;
			sParam.glb.status.faultText[5] =  fcGlbFault0020rzsk;
			sParam.glb.status.faultText[6] =  fcGlbFault0040;
			sParam.glb.status.faultText[7] =  fcGlbFault0080;
			sParam.glb.status.faultText[8] =  fcGlbFault0100;
			sParam.glb.status.faultText[9] =  fcGlbFault0200;
			sParam.glb.status.faultText[10] = fcGlbFault0400rzsk;
			sParam.glb.status.faultText[11] = fcGlbFault0800;
			sParam.glb.status.faultText[12] = fcGlbFault1000;
			sParam.glb.status.faultText[13] = fcGlbFault2000;
			sParam.glb.status.faultText[14] = fcGlbFault4000;
			sParam.glb.status.faultText[15] = fcUnknownFault;

			sParam.glb.status.warningText[0] = fcGlbWarning01;
			sParam.glb.status.warningText[1] = fcGlbWarning02;
			sParam.glb.status.warningText[2] = fcUnknownWarning;
			sParam.glb.status.warningText[3] = fcUnknownWarning;
			sParam.glb.status.warningText[4] = fcGlbWarning10;
			sParam.glb.status.warningText[5] = fcGlbWarning20;
			sParam.glb.status.warningText[6] = fcGlbWarning40;
			sParam.glb.status.warningText[7] = fcUnknownWarning;

			// ���������� ������
			sParam.def.status.setEnable(false);

			// ��������� ���������
			// � ���������� �������� �������������� � ��������������
			sParam.prm.status.setEnable(true);
			sParam.prm.status.faultText[0] =  fcPrmFault0001rzsk;
			sParam.prm.status.faultText[1] =  fcPrmFault0002rzsk;
			sParam.prm.status.faultText[2] =  fcPrmFault0004rzsk;
			sParam.prm.status.faultText[3] =  fcPrmFault0008rzsk;
			sParam.prm.status.faultText[4] =  fcUnknownFault;
			sParam.prm.status.faultText[5] =  fcUnknownFault;
			sParam.prm.status.faultText[6] =  fcUnknownFault;
			sParam.prm.status.faultText[7] =  fcUnknownFault;
			sParam.prm.status.faultText[8] =  fcPrmFault0100rzsk;
			sParam.prm.status.faultText[9] =  fcPrmFault0200rzsk;
			sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
			sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
			sParam.prm.status.faultText[12] = fcPrmFault1000rzsk;
			sParam.prm.status.faultText[13] = fcUnknownFault;
			sParam.prm.status.faultText[14] = fcUnknownFault;
			sParam.prm.status.faultText[15] = fcPrmFault8000rzsk;

			sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
			sParam.prm.status.warningText[1] = fcUnknownWarning;
			sParam.prm.status.warningText[2] = fcUnknownWarning;
			sParam.prm.status.warningText[3] = fcUnknownWarning;
			sParam.prm.status.warningText[4] = fcUnknownWarning;
			sParam.prm.status.warningText[5] = fcUnknownWarning;
			sParam.prm.status.warningText[6] = fcUnknownWarning;
			sParam.prm.status.warningText[7] = fcUnknownWarning;

			// ��������� �����������
			// � ���������� �������� �������������� � ��������������
			sParam.prd.status.setEnable(true);
			sParam.prd.status.faultText[0] =  fcPrdFault0001rzsk;
			sParam.prd.status.faultText[1] =  fcPrdFault0002rzsk;
			sParam.prd.status.faultText[2] =  fcUnknownFault;
			sParam.prd.status.faultText[3] =  fcUnknownFault;
			sParam.prd.status.faultText[4] =  fcUnknownFault;
			sParam.prd.status.faultText[5] =  fcUnknownFault;
			sParam.prd.status.faultText[6] =  fcUnknownFault;
			sParam.prd.status.faultText[7] =  fcUnknownFault;
			sParam.prd.status.faultText[8] =  fcPrdFault0100rzsk;
			sParam.prd.status.faultText[9] =  fcPrdFault0200rzsk;
			sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
			sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
			sParam.prd.status.faultText[12] = fcUnknownFault;
			sParam.prd.status.faultText[13] = fcUnknownFault;
			sParam.prd.status.faultText[14] = fcUnknownFault;
			sParam.prd.status.faultText[15] = fcUnknownFault;

			sParam.prd.status.warningText[0] = fcUnknownWarning;
			sParam.prd.status.warningText[1] = fcUnknownWarning;
			sParam.prd.status.warningText[2] = fcUnknownWarning;
			sParam.prd.status.warningText[3] = fcUnknownWarning;
			sParam.prd.status.warningText[4] = fcUnknownWarning;
			sParam.prd.status.warningText[5] = fcUnknownWarning;
			sParam.prd.status.warningText[6] = fcUnknownWarning;
			sParam.prd.status.warningText[7] = fcUnknownWarning;

			status = true;
		}
		else if (device == AVANT_R400)
		{
			sParam.typeDevice = device;

			sParam.prm.status.setEnable(false);
			sParam.prd.status.setEnable(false);

			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			measParam[2] = MENU_MEAS_PARAM_UOUT;
			measParam[3] = MENU_MEAS_PARAM_IOUT;
			measParam[4] = MENU_MEAS_PARAM_UZ;
			measParam[5] = MENU_MEAS_PARAM_UC;

			// ��������� ������
			// � ���������� �������� �������������� � ��������������
			sParam.def.status.setEnable(true);
			sParam.def.status.faultText[0] =  fcDefFault0001;
			sParam.def.status.faultText[1] =  fcDefFault0002;
			sParam.def.status.faultText[2] =  fcDefFault0004;
			sParam.def.status.faultText[3] =  fcDefFault0008;
			sParam.def.status.faultText[4] =  fcDefFault0010;
			sParam.def.status.faultText[5] =  fcDefFault0020;
			sParam.def.status.faultText[6] =  fcDefFault0040;
			sParam.def.status.faultText[7] =  fcDefFault0080;
			sParam.def.status.faultText[8] =  fcDefFault0100;
			sParam.def.status.faultText[9] =  fcDefFault0200;
			sParam.def.status.faultText[10] = fcDefFault0400;
			sParam.def.status.faultText[11] = fcDefFault0800;
			sParam.def.status.faultText[12] = fcDefFault1000;
			sParam.def.status.faultText[13] = fcDefFault2000;
			sParam.def.status.faultText[14] = fcDefFault4000;
			sParam.def.status.faultText[15] = fcDefFault8000;

			status = true;
		}
		else
		{
			// ���� ���������� ������ �� �������� �� ��� ���� ��������� ���
			// �� ������ ������������ ������
			sParam.typeDevice = AVANT_NO;
			// � ������ ������������ ���� ����������, �������� ���
			measParam[0] = MENU_MEAS_PARAM_TIME;
			measParam[1] = MENU_MEAS_PARAM_DATE;
			for(uint_fast8_t i = 2; i < 6; i++)
				measParam[i] = MENU_MEAS_PARAM_NO;
			sParam.def.status.setEnable(false);
			sParam.prm.status.setEnable(false);
			sParam.prd.status.setEnable(false);

			lineParam_ = 1;
		}
	}

	// ����� ����� ������������� �������� ���� ��������
	sParam.device = true;

	return status;
}

/** ������� ���������� ������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::clearTextBuf()
{
	for(uint_fast8_t i = 0; i < sizeof(vLCDbuf);i++)
		vLCDbuf[i] = ' ';
}

/**	����� � ��������� ����� ������������� ���������.
 * 	@param poz ������� ������� (0..5, 0 ������ ������ �����, 5 - ������ ������)
 * 	@param par ������ ������������ ��������
 * 	@return ���
 */
void
clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par)
{
	// �������� � ������
	if (poz < 6)
	{
		poz = (poz * 10) + 1;

		switch(par)
		{
			case MENU_MEAS_PARAM_DATE:
				snprintf_P(&vLCDbuf[poz], 9, fcDate,
						sParam.dataTime.getDay(),
						sParam.dataTime.getMonth(),
						sParam.dataTime.getYear());
				break;
			case MENU_MEAS_PARAM_TIME:
				snprintf_P(&vLCDbuf[poz], 9, fcTime,
						sParam.dataTime.getHour(),
						sParam.dataTime.getMinute(),
						sParam.dataTime.getSecond());
				break;
			case MENU_MEAS_PARAM_UZ:
				snprintf_P(&vLCDbuf[poz], 9, fcUz,
						sParam.measParam.getVoltageDef());
				break;
			case MENU_MEAS_PARAM_UC:
				snprintf_P(&vLCDbuf[poz], 11, fcUcf,
						sParam.measParam.getVoltageCf());
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
			default:
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
void
clMenu::printDevicesStatus(uint8_t poz, TDeviceStatus *device)
{
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
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
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
			snprintf_P(&vLCDbuf[poz], 17, text[x]);
		}
		else
		{
			y = device->getWarnings();
			snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
		}
	}
	else
	{
		text = device->stateText;
		poz += snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]) + 1;
		snprintf_P(&vLCDbuf[poz], 9, text[device->getState()]);
	}
}

/** ������� "��������� ��� ��������"
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlError()
{
	static const char fcNoTypeDevice0[] PROGMEM = "    ��� ��������    ";
	static const char fcNoTypeDevice1[] PROGMEM = "   �� ���������!!!  ";

	if (lvlCreate_)
	{
		lvlCreate_ = false;
		cursorEnable_ = false;
		vLCDclear();
		// ������ ���� ������ ��������� ��� ����� ����������
		vLCDdrawBoard(1);
	}

	// ����� �� ����� ���������� ����������
	printMeasParam(0, measParam[0]);
	printMeasParam(1, measParam[1]);

	snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
	snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);
}

/** ������� ���������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlStart()
{
	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorEnable_ = false;
		lineParam_ = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	// ����� �� ����� ���������� ����������
	for(uint_fast8_t i = 0; i < 6; i++)
		printMeasParam(i, measParam[i]);

	if (sParam.def.status.isEnable())
		printDevicesStatus(60, &sParam.def.status);
	if (sParam.prm.status.isEnable())
		printDevicesStatus(80, &sParam.prm.status);
	if (sParam.prd.status.isEnable())
		printDevicesStatus(100, &sParam.prd.status);

	switch(key_)
	{
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;

		case KEY_FUNC_RES_IND:
//			com = 1;
			break;

		case KEY_FUNC_ENTER:
//			com = 2;
			break;

		case KEY_FUNC_RESET:
//			com = 3;
			break;

		default:
			break;
	}
}

/** ������� ���� ������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlFirst()
{
	static char title[] PROGMEM = "����";
	static char punkt[] [21] PROGMEM =
	{
		"1. ������",
		"2. ����������",
		"3. ���������",
		"4. ����",
		"5. ����������"
	};


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_RIGHT:
			switch(cursorLine_)
			{
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
				case 5:
					lvlMenu = &clMenu::lvlInfo;
					lvlCreate_ = true;
					break;
			}
			break;


		case KEY_LEFT:
		case KEY_ENTER:
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
void
clMenu::lvlInfo()
{
	static char title[] PROGMEM = "����\\����������";
	static char punkt[] [21] PROGMEM =
	{
			"��� MCU : v1.11",
			"��� DSP : v1.78",
			"��  MCU : v1.49"
	};


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	switch(key_)
	{
		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. �������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlJournal()
{
	static char title[] PROGMEM = "����\\������";
	static PGM_P punkt[4];
	static uint8_t numPunkt = 0;

	static char punkt1[] PROGMEM = "%d. �������";
	static char punkt2[] PROGMEM = "%d. �����";
	static char punkt3[] PROGMEM = "%d. ���������";
	static char punkt4[] PROGMEM = "%d. �����������";

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		if (sParam.typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlJournalEvent;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt2)
			{

				lvlMenu = &clMenu::lvlJournalDef;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlJournalPrm;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlJournalPrd;
				lvlCreate_ = true;
			}
			break;
		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ������ �������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlJournalEvent()
{
	static char title[] PROGMEM = "������\\�������";
	static char punkt0[] [21] PROGMEM =
	{
			"�����: 1  �����: 2",
			"����:  04.07.12",
			"�����: 09:45:12.357",
			"���: ����������",
			"��������: �������"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 2  �����: 2",
			"����:  04.07.12",
			"�����: 09:47:14.512",
			"���: ����������",
			"��������: ������"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		else
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ������ ������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlJournalDef()
{
	static char title[] PROGMEM = "������\\������";
	static char punkt0[] [21] PROGMEM =
	{
			"�����: 1  �����: 1",
			"����:  04.07.12",
			"�����: 09:53:45.012",
			"���: ��� ��",
			"��������: 000 000"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 1)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ������ ���������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlJournalPrm()
{
	static char title[] PROGMEM = "������\\��������";
	static char punkt0[] [21] PROGMEM =
	{
			"�����: 1  �����: 2",
			"����:  05.07.12",
			"�����: 14:13:45.987",
			"���: ������� 2",
			"��������: ������"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 2  �����: 2",
			"����:  05.07.12",
			"�����: 14:13:46.033",
			"���: ������� 2",
			"��������: �����"
	};


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ������ �����������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlJournalPrd()
{
	static char title[] PROGMEM = "������\\����������";
	static char punkt0[] [21] PROGMEM =
	{
			"�����: 1  �����: 2",
			"����:  05.07.12",
			"�����: 14:13:45.964",
			"���: ������� 2",
			"��������: ������"
	};

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 2  �����: 2",
			"����:  05.07.12",
			"�����: 14:13:46.0014",
			"���: ������� 2",
			"��������: �����"
	};


	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/**	������� ����. ����������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlControl()
{
	static char title[] PROGMEM = "����\\����������";
	static PGM_P punkt[10];
	static uint8_t numPunkt;

	static char punkt1[] PROGMEM= "%d. ���� ����������";
	static char punkt2[] PROGMEM= "%d. ���� ����������";
	static char punkt3[] PROGMEM= "%d. ����� ������";
	static char punkt4[] PROGMEM= "%d. ����� ����������";
	static char punkt5[] PROGMEM= "%d. �����";

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);


		if ((sParam.typeDevice == AVANT_RZSK) ||
				(sParam.typeDevice == AVANT_R400))
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
			punkt[numPunkt++] = punkt5;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}

}

/** ������� ����. ���������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetup()
{
	static char title[] PROGMEM = "����\\���������";
	static char punkt[] [21] PROGMEM =
	{
			"1. �����",
			"2. ����� � ����",
			"3. ���������",
			"4. ������"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			switch(cursorLine_)
			{
				case 2:
					lvlMenu = &clMenu::lvlSetupDT;
					lvlCreate_ = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetupParam;
					lvlCreate_ = true;
					break;
			}
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ��������� ����������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupParam()
{
	static char title[] PROGMEM = "���������\\���������";
	static PGM_P punkt[4];
	static uint8_t numPunkt = 0;

	static char punkt1[] PROGMEM = "%d. ������";
	static char punkt2[] PROGMEM = "%d. ���������";
	static char punkt3[] PROGMEM = "%d. �����������";
	static char punkt4[] PROGMEM = "%d. �����";

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);

		if (sParam.typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (sParam.typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 20, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < numPunkt)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine_ - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlSetupParamDef;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupParamPrm;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParamPrd;
				lvlCreate_ = true;
			}
			else if (punkt[cursorLine_ - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate_ = true;
			}
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}

}

/** ������� ����. ��������� ���������� ������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupParamDef()
{
	static char title[] PROGMEM = "���������\\������";

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 1  �����: 7",
			"��� ������",
			"��������: ���-����",
			"��������: ������"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"�����: 2  �����: 7",
			"��� �����",
			"��������: 2 ��������",
			"��������: ������"
	};
	static char punkt3[] [21] PROGMEM =
	{
			"�����: 3  �����: 7",
			"���. ����� ��� ���.",
			"��������: 0�",
			"��������: 0..99�"
	};
	static char punkt4[] [21] PROGMEM =
	{
			"�����: 4  �����: 7",
			"����� ������� �� ��",
			"��������: 6��",
			"��������: 0..20��"
	};
	static char punkt5[] [21] PROGMEM =
	{
			"�����: 5  �����: 7",
			"���������� ���������",
			"��������: 0����",
			"��������: 0..54����"
	};
	static char punkt6[] [21] PROGMEM =
	{
			"�����: 6  �����: 7",
			"����������� ��������",
			"��������: 0����",
			"��������: 0..18����"
	};
	static char punkt7[] [21] PROGMEM =
	{
			"�����: 7  �����: 7",
			"������.������. �� ��",
			"��������: 0��",
			"��������: 0..32��"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine_ == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine_ == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine_ == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine_ == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine_ == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 7)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ��������� ���������� ���������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupParamPrm()
{
	static char title[] PROGMEM = "���������\\��������";

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 1  �����: 2",
			"����� ��������� ���.",
			"��������: 5��",
			"��������: 0..10��"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"�����: 2  �����: 2",
			"�������� �� ����.���",
			"��������: 100��",
			"��������: 0..1000��"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ��������� ���������� �����������.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupParamPrd()
{
	static char title[] PROGMEM = "���������\\����������";

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 1  �����: 2",
			"����� ��������� ���.",
			"��������: 5��",
			"��������: 0..10��"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"�����: 2  �����: 2",
			"������������ ������",
			"��������: 50��",
			"��������: 20..100��"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < 2)
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ��������� ���������� �����.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupParamGlb()
{
	static char title[] PROGMEM = "���������\\�����";

	static char punkt1[] [21] PROGMEM =
	{
			"�����: 1  �����: 7",
			"�������� ���.�������",
			"��������: ���.",
			"��������: ������"
	};
	static char punkt2[] [21] PROGMEM =
	{
			"�����: 2  �����: 7",
			"����� �����������",
			"��������: 5���",
			"��������: 0..5���"
	};
	static char punkt3[] [21] PROGMEM =
	{
			"�����: 3  �����: 7",
			"����� �������. �� ��",
			"��������: 10��",
			"��������: 0..15��"
	};
	static char punkt4[] [21] PROGMEM =
	{
			"�����: 4  �����: 7",
			"�����. ���� ���. ���",
			"��������: ����.",
			"��������: ������"
	};
	static char punkt5[] [21] PROGMEM =
	{
			"�����: 5  �����: 7",
			"�����. ���� ���. ���",
			"��������: ����.",
			"��������: ������"
	};
	static char punkt6[] [21] PROGMEM =
	{
			"�����: 6  �����: 7",
			"������.������. �� ��",
			"��������: 0��",
			"��������: 0..32��"
	};
	static char punkt7[] [21] PROGMEM =
	{
			"�����: 7  �����: 7",
			"�������� �����",
			"��������: ��������",
			"��������: ������"
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		if (sParam.typeDevice == AVANT_K400_OPTIC)
			cursorLine_ = 2;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine_ == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine_ == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine_ == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine_ == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine_ == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine_ == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine_ == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;

			if (sParam.typeDevice == AVANT_K400)
			{
				if (cursorLine_ == 6)
					cursorLine_ = 5;
			}
			else if (sParam.typeDevice == AVANT_K400_OPTIC)
			{
				if (cursorLine_ <= 2)
					cursorLine_ = 2;
				else if ((cursorLine_ == 3) || (cursorLine_ == 6))
				{
					cursorLine_--;
				}
			}
			else if (sParam.typeDevice == AVANT_R400)
			{
				if (cursorLine_ == 2)
					cursorLine_ = 1;
				else if ((cursorLine_ == 4) || (cursorLine_ == 5))
					cursorLine_ = 3;
			}

			break;
		case KEY_DOWN:
			if (cursorLine_ < 7)
				cursorLine_++;

			if (sParam.typeDevice == AVANT_K400)
			{
				if (cursorLine_ == 6)
					cursorLine_ = 7;
			}
			else if (sParam.typeDevice == AVANT_K400_OPTIC)
			{
				if ((cursorLine_ == 3) || (cursorLine_ == 6))
				{
					cursorLine_++;
				}
			}
			else if (sParam.typeDevice == AVANT_R400)
			{
				if (cursorLine_ == 2)
					cursorLine_ = 3;
				else if ((cursorLine_ == 4) || (cursorLine_ == 5))
					cursorLine_ = 6;
			}
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

/** ������� ����. ��������� ����/�����.
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlSetupDT()
{
	static char title[] PROGMEM = "���������\\�����&����";
	static char punkt[] [21] PROGMEM =
	{
			"1. ����",
			"2. �����",
	};

	if (lvlCreate_)
	{
		lvlCreate_ = false;

		cursorLine_ = 1;
		cursorEnable_ = true;

		lineParam_ = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam_);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable_)
	{
		vLCDbuf[2 + 20 * cursorLine_] = '*';
	}

	switch(key_)
	{
		case KEY_UP:
			if (cursorLine_ > 1)
				cursorLine_--;
			break;
		case KEY_DOWN:
			if (cursorLine_ < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine_++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate_ = true;
			break;
		case KEY_RIGHT:
			break;

		case KEY_ENTER:
			break;

		default:
			break;
	}
}

uint8_t
clMenu::txCommand()
{
//	uint_fast8_t t = com;
//	com = 0;

	return 0;
}

uint8_t
clMenu::getNumError(uint16_t val)
{
	uint_fast8_t tmp = 0;

	while ((val & 0x0001) == 0)
	{
		tmp++;
		val >>= 1;
	}

	return tmp;
}
