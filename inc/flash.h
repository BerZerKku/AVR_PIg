/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: ������
 */

#include <avr/pgmspace.h>

static const char fcNull[] PROGMEM = "";

// ���������� ���������
static const char fcUout[] 	PROGMEM = "U=%02u.%01u�";
static const char fcIout[] 	PROGMEM	= "I=%03u��";
static const char fcRout[] 	PROGMEM = "R=%03u��";
static const char fcUz[]	PROGMEM	= "U�=%02d��";
static const char fcUcf[] 	PROGMEM	= "U�=%02d��";
static const char fcUn[] 	PROGMEM	= "U�=%02d��";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// ����� ��������� ��������
static const char fcDeviceName00[] PROGMEM = "���";
static const char fcDeviceName01[] PROGMEM = "���";
static const char fcDeviceName02[] PROGMEM = "���";

// ������ ���������
// ���-�� ������� (��� ����� "������") ������ ��������� � MAX_NUM_REGIME
static const char fcRegime[] [8] PROGMEM =
{
	"�������",
	"�����",
	"������",
	"����",
	"���� 1",
	"���� 2",
	"������"
};

// ��������� ���������
// ���-�� ��������� ��� ���� ��������� ������ ��������� � MAX_NUM_DEVICE_STATE
// ����������� ���������
static const char fcUnknownSost[] PROGMEM = "������";
// ��������� ������
static const char fcDefSost00[] PROGMEM =	"������.";
static const char fcDefSost01[] PROGMEM =	"��������";
static const char fcDefSost02[] PROGMEM =	"����";
static const char fcDefSost03[] PROGMEM =	"������";
static const char fcDefSost04[] PROGMEM =	"������.";
static const char fcDefSost05[] PROGMEM =	"�.������";
static const char fcDefSost06[] PROGMEM =	"��������";
static const char fcDefSost07[] PROGMEM =	"���.����";
static const char fcDefSost08[] PROGMEM = 	"?0x08?";
static const char fcDefSost09[] PROGMEM =	"��� ��";
static const char fcDefSost10[] PROGMEM =	"����";
// ��������� ���������
static const char fcPrmSost00[] PROGMEM = 	"������.";
static const char fcPrmSost01[] PROGMEM = 	"��� ��";
static const char fcPrmSost02[] PROGMEM = 	"��� ��";
static const char fcPrmSost03[] PROGMEM = 	"��� ��";
static const char fcPrmSost04[] PROGMEM = 	"������.";
static const char fcPrmSost05[] PROGMEM = 	"�.������";
static const char fcPrmSost06[] PROGMEM = 	"��������";
static const char fcPrmSost07[] PROGMEM = 	"��� ��";
static const char fcPrmSost08[] PROGMEM = 	"?0x08?";
static const char fcPrmSost09[] PROGMEM = 	"?0x09?";
static const char fcPrmSost10[] PROGMEM = 	"����";
// ��������� �����������
static const char fcPrdSost00[] PROGMEM = 	"������.";
static const char fcPrdSost01[] PROGMEM =	"��� ��";
static const char fcPrdSost02[] PROGMEM =	"��� ��";
static const char fcPrdSost03[] PROGMEM =	"��� ��";
static const char fcPrdSost04[] PROGMEM =	"������.";
static const char fcPrdSost05[] PROGMEM =	"�.������";
static const char fcPrdSost06[] PROGMEM =	"��������";
static const char fcPrdSost07[] PROGMEM = 	"?0x07?";
static const char fcPrdSost08[] PROGMEM = 	"?0x08?";
static const char fcPrdSost09[] PROGMEM =	"������.";
static const char fcPrdSost10[] PROGMEM =	"����";

// ������� ��� �������������� � ������� �������� �����
static const char fcUnknownFault[] PROGMEM = 		"�������������";
static const char fcUnknownWarning[] PROGMEM = 		"��������������";

// ������ ������ ���� �������������/��������������
// %c - g ��� ����������, l - ��� ���������
static const char fcFaults[] PROGMEM 	= 			"������. %c-%04X";
static const char fcWarnings[] PROGMEM  =	 		"�������. %c-%04X";

// ����� �������������
static const char fcGlbFault0001[] PROGMEM = 		"������.��. FLASH";
static const char fcGlbFault0002[] PROGMEM = 		"������.���.FLASH";
static const char fcGlbFault0004[] PROGMEM = 		"������.��. PLIS";
static const char fcGlbFault0008[] PROGMEM = 		"������.���.PLIS";
static const char fcGlbFault0010[] PROGMEM = 		"������.���.2RAM";
static const char fcGlbFault0020[] PROGMEM = 		"��-��� ������";
static const char fcGlbFault0020rzsk[] PROGMEM =	"��� ������� ���";
static const char fcGlbFault0040[] PROGMEM = 		"��-������.������";
static const char fcGlbFault0080[] PROGMEM =		"������ � �����";
static const char fcGlbFault0100[] PROGMEM = 		"������.DSP 2RAM ";
static const char fcGlbFault0200[] PROGMEM = 		"������.��. 2RAM ";
static const char fcGlbFault0400[] PROGMEM = 		"��� �����";
static const char fcGlbFault0400rzsk[] PROGMEM = 	"���������� ����";
static const char fcGlbFault0800[] PROGMEM = 		"������ ����.���";
static const char fcGlbFault1000[] PROGMEM = 		"������� ����.���";
static const char fcGlbFault2000[] PROGMEM = 		"������. �� ��";
static const char fcGlbFault4000[] PROGMEM = 		"�� ����� �����.";

// ����� ��������������
static const char fcGlbWarning01[] PROGMEM = 		"���������� ����";
static const char fcGlbWarning02[] PROGMEM = 		"����� ������.���";
static const char fcGlbWarning10[] PROGMEM = 		"����.���� ������";
static const char fcGlbWarning20[] PROGMEM = 		"����.��� ������";
static const char fcGlbWarning40[] PROGMEM = 		"����.��� ������";

// ������������� ������
static const char fcDefFault0001[] PROGMEM =		"��� ����� ���";
static const char fcDefFault0002[] PROGMEM = 		"������.����.���";
static const char fcDefFault0004[] PROGMEM = 		"������.������.";
static const char fcDefFault0008[] PROGMEM = 		"������.���. ���";
static const char fcDefFault0010[] PROGMEM = 		"��-��� ������";
static const char fcDefFault0010rzsk[] PROGMEM =	"��� ������� ���";
static const char fcDefFault0020[] PROGMEM =		"������ ��. ��";
static const char fcDefFault0040[] PROGMEM =		"������.����. ���";
static const char fcDefFault0080[] PROGMEM =		"������.����. ���";
static const char fcDefFault0100[] PROGMEM =		"������.����.����";
static const char fcDefFault0200[] PROGMEM =		"������.����.����";
static const char fcDefFault0400[] PROGMEM =		"��.���� ��� ���.";
static const char fcDefFault0800[] PROGMEM = 		"������.����. ���";
static const char fcDefFault1000[] PROGMEM = 		"��.���� ���.���.";
static const char fcDefFault2000[] PROGMEM =		"������.���. ���";
static const char fcDefFault4000[] PROGMEM =		"��������. ������";
static const char fcDefFault8000[] PROGMEM =		"������. ���";

// �������������� ������
static const char fcDefWarning01[] PROGMEM = 		"��-������.������";
static const char fcDefWarning01rzsk[] PROGMEM = 	"������ ����. ��";
static const char fcDefWarning01rzsko[] PROGMEM =	"��� ��";
static const char fcDefWarning02[] PROGMEM = 		"��� ������� ���";
static const char fcDefWarning04[] PROGMEM = 		"����� �� ������";
static const char fcDefWarning08[] PROGMEM = 		"������������";

// ������������� ���������
static const char fcPrmFault0001rzsk[] PROGMEM =	"��� ����� ���";
static const char fcPrmFault0002rzsk[] PROGMEM =	"������.����. ���";
static const char fcPrmFault0004rzsk[] PROGMEM =	"��� ��";
static const char fcPrmFault0008rzsk[] PROGMEM =	"����.��-���.���.";
static const char fcPrmFault0100rzsk[] PROGMEM =	"������.���. ���";
static const char fcPrmFault0200rzsk[] PROGMEM =	"������.����. ���";
static const char fcPrmFault0400rzsk[] PROGMEM =	"������.���. ���";
static const char fcPrmFault0800rzsk[] PROGMEM =	"������.�����.���";
static const char fcPrmFault1000rzsk[] PROGMEM =	"�������.��� ��";
static const char fcPrmFault8000rzsk[] PROGMEM =	"����� ����. ���";

// �������������� ���������
static const char fcPrmWarning01rzsk[] PROGMEM = 	"����. ������ ���";
static const char fcPrmWarning01rzsko[] PROGMEM = 	"��� ��";

// ������������� �����������
static const char fcPrdFault0001rzsk[] PROGMEM = 	"��� ����� ���";
static const char fcPrdFault0002rzsk[] PROGMEM = 	"������.����. ���";
static const char fcPrdFault0100rzsk[] PROGMEM = 	"������.��.������";
static const char fcPrdFault0200rzsk[] PROGMEM = 	"������.����.����";
static const char fcPrdFault0400rzsk[] PROGMEM =	"������.���. ����";
static const char fcPrdFault0800rzsk[] PROGMEM = 	"������.����. ���";

