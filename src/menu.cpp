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

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING];

//clMenu::clMenu(clProtocolS *protocol) : pProtocol(protocol)
clMenu::clMenu()
{
	lvlMenu = &clMenu::lvlStart;
	lineParam = 3;

	lvlCreate = true;

	cursorEnable= false;
	cursorLine = 0;

	key = KEY_NO;

	typeDevice = AVANT_R400;

	com = 0;

	connectionBsp = false;

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
	// ������� ������� �� ����������������� ���
	static uint8_t reInit = false;

	// ����������������� ��������
	if (reInit > 0)
	{
		reInit--;
	}
	else
	{
		reInit = TIME_TO_REINIT_LCD - 1;
		vLCDinit();
	}

	// ������� ��� � ����������
	// ���� ������ ����� ������ - ��������� ��������������� ���������
	eKEY tmp = eKEYget();
	if (tmp != KEY_NO)
	{
		if (tmp == KEY_FUNC)
			tmp = KEY_NO;
		key = tmp;

		vLCDsetLED(LED_SWITCH);
	}

	// ������� ���������� ������ � ������� �� ������� ������� ����
	clearTextBuf();
	(this->*lvlMenu) ();
	key = KEY_NO;


	// ����� ��������� � ������ ���������� ����� � ���
	if (!isConnectionBsp() && ((reInit % 10) < 5))
		snprintf_P(&vLCDbuf[0], 21, fcNoConnectBsp);

#ifdef DEBUG
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

	// �������������� ������ �������� � ������ ��� ������ �� �����
	vLCDputchar(vLCDbuf, lineParam);

	// ������ ���������� ���-�� �� ���
	vLCDrefresh();
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

/** ������� ���������
 * 	@param ���
 * 	@return ���
 */
void
clMenu::lvlStart()
{
	if (lvlCreate)
	{
		PORT_DBG |= PIN_TP2;

		lvlCreate = false;

		cursorEnable = false;
		lineParam = 3;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

#ifdef REGIME_OFF
	sParam.def_regime = 2;
	sParam.prd_regime = 2;
	sParam.prm_regime = 2;
#endif
#ifdef SOST_OFF
	sParam.def_sost = 1;
	sParam.prm_sost = 1;
	sParam.prd_sost = 1;
#endif

	if (typeDevice == AVANT_R400)
	{
		// ����� ����������
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam.hour, sParam.minute, sParam.second);
		snprintf_P(&vLCDbuf[12], 11, fcUz, sParam.voltDef);
		snprintf_P(&vLCDbuf[20], 11, fcUout, sParam.voltOutInt, sParam.voltOutFract);
		snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam.voltCF);
		snprintf_P(&vLCDbuf[40], 11, fcIout, sParam.curOut);
		snprintf_P(&vLCDbuf[52], 11, fcUn, sParam.voltNoise);

		// ����� ������\��������� ���������
		snprintf_P(&vLCDbuf[60], 21, fcDef, fcRegime[sParam.def_regime], //
											fcDefSost[sParam.def_sost]);
	}
	else if (typeDevice == AVANT_RZSK)
	{
		// ����� ����������
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam.hour, sParam.minute, sParam.second);
		snprintf_P(&vLCDbuf[12], 11, fcUz, sParam.voltDef);
		snprintf_P(&vLCDbuf[20], 11, fcUout, sParam.voltOutInt, sParam.voltOutFract);
		snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam.voltCF);
		snprintf_P(&vLCDbuf[40], 11, fcIout, sParam.curOut);
		snprintf_P(&vLCDbuf[52], 11, fcUn, sParam.voltNoise);

		// ����� ������\��������� ���������
		snprintf_P(&vLCDbuf[60], 21, fcDef, fcRegime[sParam.def_regime], //
											fcDefSost[sParam.def_sost]);
		snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[sParam.prm_regime], //
											fcPrmSost[sParam.prm_sost]);
		snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[sParam.prd_regime], //
											 fcPrdSost[sParam.prd_sost]);
	}
	else if (typeDevice == AVANT_K400)
	{
		// ����� ����������
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam.hour, sParam.minute, sParam.second);
		snprintf_P(&vLCDbuf[12], 11, fcDate, sParam.day, sParam.month, sParam.year);
		snprintf_P(&vLCDbuf[20], 11, fcUout, sParam.voltOutInt, sParam.voltOutFract);
		snprintf_P(&vLCDbuf[32], 11, fcUcf, sParam.voltCF);
		snprintf_P(&vLCDbuf[40], 11, fcIout, sParam.curOut);
		snprintf_P(&vLCDbuf[52], 11, fcUn, sParam.voltNoise);

		// ����� ������\��������� ���������
		snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[sParam.prd_regime], //
											 fcDefSost[sParam.prd_sost]);
	}
	else if (typeDevice == AVANT_K400_OPTIC)
	{
		snprintf_P(&vLCDbuf[00], 11, fcTime, sParam.hour, sParam.minute, sParam.second);
		snprintf_P(&vLCDbuf[12], 11, fcDate, sParam.day, sParam.month, sParam.year);

		if (sParam.glb_avar != 0)
		{
			uint_fast8_t tmp = 0;

			tmp = getNumError(sParam.glb_avar);
			snprintf_P(&vLCDbuf[80], 21, fcPrm, fcGlbError[tmp], fcNull);
			snprintf_P(&vLCDbuf[100], 21, fcPrd, fcGlbError[tmp], fcNull);

		}
		else if (sParam.glb_warn != 0)
		{
			uint_fast8_t tmp = 0;

			tmp = getNumError(sParam.glb_warn);
			snprintf_P(&vLCDbuf[80], 21, fcPrm, fcGlbWarning[tmp], fcNull);
			snprintf_P(&vLCDbuf[100], 21, fcPrd, fcGlbWarning[tmp], fcNull);
		}
		else
		{
			// ����� ������\��������� ���������
			if (sParam.prm_avar == 0)
			{
				if (sParam.prm_warn == 0)
				{
					snprintf_P(&vLCDbuf[80], 21, fcPrm, fcRegime[sParam.prm_regime], //
													fcPrmSost[sParam.prm_sost]);
				}
				else
				{
					uint_fast8_t tmp = 0;

					tmp = getNumError(sParam.prm_warn);
					snprintf_P(&vLCDbuf[80], 21, fcPrm, fcPrmWarningOptic[tmp], fcNull);
				}
			}
			else
			{
				uint_fast8_t tmp = 0;

				tmp = getNumError(sParam.prm_avar);
				snprintf_P(&vLCDbuf[80], 21, fcPrm, fcPrmError[tmp], fcNull);

			}

			if (sParam.prd_avar == 0)
			{
				if (sParam.prd_warn == 0)
				{
					snprintf_P(&vLCDbuf[100], 21, fcPrd, fcRegime[sParam.prd_regime],//
														 fcPrdSost[sParam.prd_sost]);
				}
				else
				{
					uint_fast8_t tmp = 0;

					tmp = getNumError(sParam.prd_warn);
					snprintf_P(&vLCDbuf[100], 21, fcPrd, fcPrdWarning[tmp], fcNull);
				}
			}
			else
			{
				uint_fast8_t tmp = 0;

				tmp = getNumError(sParam.prd_avar);
				snprintf_P(&vLCDbuf[100], 21, fcPrd, fcPrdError[tmp], fcNull);

			}
		}
	}


	switch(key)
	{
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;

		case KEY_ENTER:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;

		case KEY_FUNC_RES_IND:
			com = 1;
			break;

		case KEY_FUNC_ENTER:
			com = 2;
			break;

		case KEY_FUNC_RESET:
			com = 3;
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


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_RIGHT:
			switch(cursorLine)
			{
				case 1:
					lvlMenu = &clMenu::lvlJournal;
					lvlCreate = true;
					break;
				case 2:
					lvlMenu = &clMenu::lvlControl;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetup;
					lvlCreate = true;
					break;
				case 5:
					lvlMenu = &clMenu::lvlInfo;
					lvlCreate = true;
					break;
			}
			break;


		case KEY_LEFT:
		case KEY_ENTER:
			lvlMenu = &clMenu::lvlStart;
			lvlCreate = true;
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


	if (lvlCreate)
	{
		lvlCreate = false;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title );

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	switch(key)
	{
		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);

		if (typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
		}
		else if (typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
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

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlJournalEvent;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt2)
			{

				lvlMenu = &clMenu::lvlJournalDef;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlJournalPrm;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlJournalPrd;
				lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		else
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 1)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
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


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
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


	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);
	for(uint_fast8_t i = 0; i < 5; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt0[i]);
		if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlJournal;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);


		if ((typeDevice == AVANT_RZSK) || (typeDevice == AVANT_R400))
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
			punkt[numPunkt++] = punkt5;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
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

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlFirst;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			switch(cursorLine)
			{
				case 2:
					lvlMenu = &clMenu::lvlSetupDT;
					lvlCreate = true;
					break;
				case 3:
					lvlMenu = &clMenu::lvlSetupParam;
					lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);

		if (typeDevice == AVANT_R400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_RZSK)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt1;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}
		else if (typeDevice == AVANT_K400_OPTIC)
		{
			numPunkt = 0;
			punkt[numPunkt++] = punkt2;
			punkt[numPunkt++] = punkt3;
			punkt[numPunkt++] = punkt4;
		}

	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < numPunkt; i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i], i + 1);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < numPunkt)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate = true;
			break;
		case KEY_RIGHT:
			if (punkt[cursorLine - 1] == punkt1)
			{
				lvlMenu = &clMenu::lvlSetupParamDef;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt2)
			{
				lvlMenu = &clMenu::lvlSetupParamPrm;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt3)
			{
				lvlMenu = &clMenu::lvlSetupParamPrd;
				lvlCreate = true;
			}
			else if (punkt[cursorLine - 1] == punkt4)
			{
				lvlMenu = &clMenu::lvlSetupParamGlb;
				lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 7)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < 2)
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		if (typeDevice == AVANT_K400_OPTIC)
			cursorLine = 2;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);



	for(uint_fast8_t i = 0; i < 4; i++)
	{
		if (cursorLine == 1)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt1[i]);
		else if (cursorLine == 2)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt2[i]);
		else if (cursorLine == 3)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt3[i]);
		else if (cursorLine == 4)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt4[i]);
		else if (cursorLine == 5)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt5[i]);
		else if (cursorLine == 6)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt6[i]);
		else if (cursorLine == 7)
			snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt7[i]);
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;

			if (typeDevice == AVANT_K400)
			{
				if (cursorLine == 6)
					cursorLine = 5;
			}
			else if (typeDevice == AVANT_K400_OPTIC)
			{
				if (cursorLine <= 2)
					cursorLine = 2;
				else if ((cursorLine == 3) || (cursorLine == 6))
				{
					cursorLine--;
				}
			}
			else if (typeDevice == AVANT_R400)
			{
				if (cursorLine == 2)
					cursorLine = 1;
				else if ((cursorLine == 4) || (cursorLine == 5))
					cursorLine = 3;
			}

			break;
		case KEY_DOWN:
			if (cursorLine < 7)
				cursorLine++;

			if (typeDevice == AVANT_K400)
			{
				if (cursorLine == 6)
					cursorLine = 7;
			}
			else if (typeDevice == AVANT_K400_OPTIC)
			{
				if ((cursorLine == 3) || (cursorLine == 6))
				{
					cursorLine++;
				}
			}
			else if (typeDevice == AVANT_R400)
			{
				if (cursorLine == 2)
					cursorLine = 3;
				else if ((cursorLine == 4) || (cursorLine == 5))
					cursorLine = 6;
			}
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetupParam;
			lvlCreate = true;
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

	if (lvlCreate)
	{
		lvlCreate = false;

		cursorLine = 1;
		cursorEnable = true;

		lineParam = 1;
		vLCDclear();
		vLCDdrawBoard(lineParam);
	}

	snprintf_P(&vLCDbuf[0], 21, title);

	for(uint_fast8_t i = 0; i < (sizeof(punkt) / sizeof(punkt[0])); i++)
	{
		snprintf_P(&vLCDbuf[20 + 20 * i], 21, punkt[i]);
	}

	if (cursorEnable)
	{
		vLCDbuf[2 + 20 * cursorLine] = '*';
	}

	switch(key)
	{
		case KEY_UP:
			if (cursorLine > 1)
				cursorLine--;
			break;
		case KEY_DOWN:
			if (cursorLine < (sizeof(punkt) / sizeof(punkt[0])))
				cursorLine++;
			break;

		case KEY_LEFT:
			lvlMenu = &clMenu::lvlSetup;
			lvlCreate = true;
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
	uint_fast8_t t = com;
	com = 0;

	return t;
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
