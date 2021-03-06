/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: ������
 */

#include "src/glbDefine.h"
#include "src/flashParams.h"
#include "src/paramDef.h"
#include "src/paramGlb.h"
#include "src/security/securityevent.h"

/// ������ ������.
static const char fcNull[] PROGMEM = "";

/// ���� ������������� ��� ����������� � ��������� ����.
static const char fcAcType[GB_TYPE_AC_MAX + 3][9] PROGMEM = {
// 		12345678
	"��-����",///< ��������� ��������.
	"��-����",///< �� ���������� (����������).
	"��-����",///< �� ���������� (�������).
	"��-�����",///< �� ����������.
	"��-����",///< �� ��������.
	"��-����",///< �� ������ (��� �������������).
	"��-�����",///< �� ��������� (��� �������������).
	"��-����",///< �� ���� (��� �������������)..
	"��-�����",///< �� ������������� (��� �������������).
	"��-����",///< ��������� ��������.
	"��-����",///< ������� GB_TYPE_AC_AUTO_NORM � �400�->������.
	"��-����"///< ������� GB_TYPE_AC_CHECK � �400�->����-80 � ���-90.
};

/// ������ ������ ���������.
static const char fcRegime[GB_REGIME_MAX + 1][8] PROGMEM = {
//		 1234567
	"�������",///<
	"�����",///<
	"������",///<
	"����",///<
	"����",///< ���� + ��� � ���������
	"����",///< ���� + ��� � ���������
	"������"///< ��������� ��������
};

/// ������ ��������.
static const char fcIC[GB_IC_MAX + 1][9] PROGMEM = {
//		 12345678
	"��� ��  ",///< ������ �������� �� ����� ���
	"��� ��� ",///< ������ �������� DSP ����� ���
	"��� ����",///< ������ �������� ���� DSP ����� ���
	"��  ��  ",///< ������ �������� �� ����� ��
	"��� ���1",///< ������ �������� ���� ��� ����� ��� 1-16 ���
	"��� ���2",///< ������ �������� ���� ��� ����� ��� 17-32 ���
	"��� ���1",///< ������ �������� ���� ��� ����� ��� 1-16 ���
	"��� ���2",///< ������ �������� ���� ��� ����� ��� 17-32 ���
	"��� ����",///< ������ �������� ����� ���
	"������"///< ��������� ��������
};

/// ������ ���������, ��� ����� � ����������.
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
	"�������",///<
	"������",///<
	"���� 1",///< ���� �����������
	"���� 2"///< ���� ���������
};

/// ����� ��������� (��� ��������).
static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
	"���",///<
	"���",///<
	"���",///<
	"���",///< �����
	"���"///< ��������� ��������
};

/// ����� ��������� � �400 (��� ��������).
static const char fcDevicesK400[GB_DEVICE_K400_MAX + 1][5] PROGMEM = {
// 		 1234
	"���",///<
	"���1",///< �������� 1
	"���2",///< �������� 2 (� 3-� ��������)
	"���",///<
	"���",///<
	"�1,2",///< �������� 1 � 2
	"���"///< ��������� ��������
};

/// ������ ���������� ������� ��������� � ������� 1(��� ������� ���������).
static const char fcSrcPrm1[8][7] PROGMEM = {
		"      ",
		"(���2)",
		"(���3)",
		"      ",
		"      ",
		"(�2,3)",
		"      ",
		"      "
};

/// ������ ���������� ������� ��������� � ������� 2(��� ������� ���������).
static const char fcSrcPrm2[8][7] PROGMEM = {
		"      ",
		"(���1)",
		"(���3)",
		"      ",
		"      ",
		"(�1,3)",
		"      ",
		"      "
};

/// ������ ���������� ������� ��������� � ������� 3(��� ������� ���������).
static const char fcSrcPrm3[8][7] PROGMEM = {
		"      ",
		"(���1)",
		"(���2)",
		"      ",
		"      ",
		"(�1,2)",
		"      ",
		"      "
};

/// ������ ����������(��) ��������
static char fcRemoteNum[TDeviceStatus::GB_MAX_REM_NUM + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
	"   ",///< 000
	" 1 ",///< 001
	" 2 ",///< 010
	"1 2",///< 011
	" 3 ",///< 100
	"1 3",///< 101
	"2 3",///< 110
	" ? "///< 111
};

//��������� ���������
// 	���������� ��������� ��� ���� ��������� ������ ��������� � MAX_NUM_DEVICE_STATE
static const char fcUnknownSost[] PROGMEM = "������";///< ����������� ���������
// ��������� ������
static const char fcDefSost00[] PROGMEM = "������.";///<
static const char fcDefSost01[] PROGMEM = "��������";///<
static const char fcDefSost02[] PROGMEM = "����";///<
static const char fcDefSost03[] PROGMEM = "�������";///<
static const char fcDefSost04[] PROGMEM = "������.";///<
static const char fcDefSost05[] PROGMEM = "�.������";///<
static const char fcDefSost06[] PROGMEM = "��������";///<
static const char fcDefSost07[] PROGMEM = "���.����";///<
static const char fcDefSost08[] PROGMEM = "��.����";///<
static const char fcDefSost09[] PROGMEM = "��� ��";///<
static const char fcDefSost09opto[] PROGMEM = "��� ��";///<
static const char fcDefSost10[] PROGMEM = "����";///<
static const char fcDefSost11[] PROGMEM = "���";///<
static const char fcDefSost12[] PROGMEM = "���";///<
static const char fcDefSost13[] PROGMEM = "���. ��";///<
// ��������� ���������
static const char fcPrmSost00[] PROGMEM = "������.";///<
static const char fcPrmSost01[] PROGMEM = "��� ��%01u";///<
static const char fcPrmSost01opto[] PROGMEM = "��� ��%01u";///<
static const char fcPrmSost02[] PROGMEM = "��� ��%02u";///<
static const char fcPrmSost03[] PROGMEM = "��� ��";///<
static const char fcPrmSost03opto[] PROGMEM = "��� ��";///<
static const char fcPrmSost04[] PROGMEM = "������.";///<
static const char fcPrmSost05[] PROGMEM = "�.������";///<
static const char fcPrmSost06[] PROGMEM = "��������";///<
static const char fcPrmSost07[] PROGMEM = "��� ��%02u";///<
static const char fcPrmSost08[] PROGMEM = "?0x08?";///<
static const char fcPrmSost09[] PROGMEM = "?0x09?";///<
static const char fcPrmSost10[] PROGMEM = "����";///<
static const char fcPrmSost11[] PROGMEM = "���";///<
static const char fcPrmSost12[] PROGMEM = "���";///<
static const char fcPrmSost13[] PROGMEM = "���. ��";///<
// ��������� �����������
static const char fcPrdSost00[] PROGMEM = "������.";///<
static const char fcPrdSost01[] PROGMEM = "��� ��%01u";///<
static const char fcPrdSost01opto[] PROGMEM = "��� ��%01u";///<
static const char fcPrdSost02[] PROGMEM = "��� ��%02u";///<
static const char fcPrdSost03[] PROGMEM = "��� ��";///<
static const char fcPrdSost03opto[] PROGMEM = "��� ��";///<
static const char fcPrdSost04[] PROGMEM = "������.";///<
static const char fcPrdSost05[] PROGMEM = "�.������";///<
static const char fcPrdSost06[] PROGMEM = "��������";///<
static const char fcPrdSost07[] PROGMEM = "?0x07?";///<
static const char fcPrdSost08[] PROGMEM = "��� ��%02u";///<
static const char fcPrdSost09[] PROGMEM = "������.";///<
static const char fcPrdSost10[] PROGMEM = "����";///<
static const char fcPrdSost11[] PROGMEM = "���";///<
static const char fcPrdSost12[] PROGMEM = "���";///<
static const char fcPrdSost13[] PROGMEM = "���. ��";///<

// ����� �������������
static const char fcGlbFault0001[] PROGMEM = "������.��. FLASH";///<
static const char fcGlbFault0002[] PROGMEM = "������.���.FLASH";///<
static const char fcGlbFault0004[] PROGMEM = "������.��. PLIS";///<
static const char fcGlbFault0008[] PROGMEM = "������.���.PLIS";///<
static const char fcGlbFault0010[] PROGMEM = "������.���.2RAM";///<
static const char fcGlbFault0020[] PROGMEM = "��-��� ������";///<
static const char fcGlbFault0020rzsk[] PROGMEM = "��� ������� ���";///<
static const char fcGlbFault0040[] PROGMEM = "��-������.������";///<
static const char fcGlbFault0080[] PROGMEM = "������ � �����";///<
static const char fcGlbFault0100[] PROGMEM = "������.DSP 2RAM ";///<
static const char fcGlbFault0200[] PROGMEM = "������.��. 2RAM ";///<
static const char fcGlbFault0400[] PROGMEM = "��� �����";///<
static const char fcGlbFault0400rzsk[] PROGMEM = "���������� ����";///<
static const char fcGlbFault0800[] PROGMEM = "������ ����.���";///<
static const char fcGlbFault1000[] PROGMEM = "������� ����.���";///<
static const char fcGlbFault2000[] PROGMEM = "������. �� ��";///<
static const char fcGlbFault4000[] PROGMEM = "�� ����� �����.";///<

// ����� ��������������
static const char fcGlbWarning01[] PROGMEM = "���������� ����";///< 1
static const char fcGlbWarning02[] PROGMEM = "����� ������.���";///< 2
static const char fcGlbWarning04[] PROGMEM = "������ 1 �����.";///< 3
static const char fcGlbWarning04k400[] PROGMEM = "������� ��������";///< 3
static const char fcGlbWarning04ring1[] PROGMEM = "���.�� ���������";///< 3
static const char fcGlbWarning08[] PROGMEM = "������ 2 �����.";///< 4
static const char fcGlbWarning08k400[] PROGMEM = "������ ��������";///< 4
static const char fcGlbWarning08ring1[] PROGMEM = "������ ��������";///< 4
static const char fcGlbWarning10[] PROGMEM = "����.���� ������";///< 5
static const char fcGlbWarning10k400[] PROGMEM = "������. ���.����";///< 5
static const char fcGlbWarning20[] PROGMEM = "����.��� ������";///< 6
static const char fcGlbWarning40[] PROGMEM = "����.��� ������";///< 7
static const char fcGlbWarning100[] PROGMEM = "������ ������ ��";///< 9
static const char fcGlbWarning100ring1[]PROGMEM = "��� �� �� ���1";///< 9
static const char fcGlbWarning200ring1[]PROGMEM = "��� �� �� ���2";///< 10

// ������������� ������
static const char fcDefFault0001[] PROGMEM = "��� ����� ���";///<
static const char fcDefFault0002[] PROGMEM = "������.����.���";///<
static const char fcDefFault0004[] PROGMEM = "������.������.";///<
static const char fcDefFault0008[] PROGMEM = "������.���. ���";///<
static const char fcDefFault0010[] PROGMEM = "��-��� ������%S";///<
static const char fcDefFault0010opto[] PROGMEM = "��� ��";///<
static const char fcDefFault0020[] PROGMEM = "������ ��. ��";///<
static const char fcDefFault0040[] PROGMEM = "������.��.���%S";///<
static const char fcDefFault0080[] PROGMEM = "������.��.���%S";///<
static const char fcDefFault0100[] PROGMEM = "������.����.����";///<
static const char fcDefFault0200[] PROGMEM = "������.����.����";///<
static const char fcDefFault0400[] PROGMEM = "����.��� ���.%S";///<
static const char fcDefFault0800[] PROGMEM = "������.���� ���";///<
static const char fcDefFault1000[] PROGMEM = "����.���.���.%S";///<
static const char fcDefFault2000[] PROGMEM = "������.���. ���";///<
static const char fcDefFault4000[] PROGMEM = "������ � �����";///<
static const char fcDefFault4000rzsk[] PROGMEM = "��� ������� ��";///<
static const char fcDefFault8000[] PROGMEM = "������. ���";///<
static const char fcDefFault8000rzsk[] PROGMEM = "����. ����������";///<

// �������������� ������
static const char fcDefWarning01[] PROGMEM = "��-��.������ %S";///<
static const char fcDefWarning01rzsk[] PROGMEM = "������ ����. ��";///<
static const char fcDefWarning01opto[] PROGMEM = "��� ��";///<
static const char fcDefWarning02[] PROGMEM = "��� ������� ���";///<
static const char fcDefWarning04[] PROGMEM = "����� �� ������";///<
static const char fcDefWarning08[] PROGMEM = "������������";///<

// ������������� ���������
static const char fcPrmFault0001rzsk[] PROGMEM = "��� ����� ���";///<
static const char fcPrmFault0002rzsk[] PROGMEM = "������.����. ���";///<
static const char fcPrmFault0004rzsk[] PROGMEM = "��� ��";///<
static const char fcPrmFault0004opto[] PROGMEM = "��� ��";///<
static const char fcPrmFault0008k400[] PROGMEM = "������ ������� D";///<
static const char fcPrmFault0008rzsk[] PROGMEM = "����.��-���.���.";///<
static const char fcPrmFault0100rzsk[] PROGMEM = "������.���. ���";///<
static const char fcPrmFault0200rzsk[] PROGMEM = "������.����.���.";///<
static const char fcPrmFault0400rzsk[] PROGMEM = "������.���. ���.";///<
static const char fcPrmFault0800rzsk[] PROGMEM = "������.�����.���";///<
static const char fcPrmFault8000rzsk[] PROGMEM = "����� ����. ���.";///<

// �������������� ���������
static const char fcPrmWarning01rzsk[] PROGMEM = "����. ������ ���";///<
static const char fcPrmWarning01opto[] PROGMEM = "��� ��";///<
static const char fcPrmWarning02k400[] PROGMEM = "������ ������ ��";///<
static const char fcPrmWarning04k400[] PROGMEM = "���� RX �� ����";///<
static const char fcPrmWarning08k400[] PROGMEM = "������ ������� D";///<

// ������������� �����������
static const char fcPrdFault0001rzsk[] PROGMEM = "��� ����� ���";///<
static const char fcPrdFault0002rzsk[] PROGMEM = "������.����. ���";///<
static const char fcPrdFault0100rzsk[] PROGMEM = "������.��.������";///<
static const char fcPrdFault0200rzsk[] PROGMEM = "������.����.����";///<
static const char fcPrdFault0400rzsk[] PROGMEM = "������.���. ����";///<
static const char fcPrdFault0800rzsk[] PROGMEM = "������.����. ���";///<

// �������������� �����������
static const char fcPrdWarning02k400[] PROGMEM = "������ ������ ��";///<
static const char fcPrdWarning04k400[] PROGMEM = "���� RX �� ����";///<

/// ������� �������� ���������.
static const char fcValue[] PROGMEM = "��������: ";

//	������
/// ��������� � ���, ��� � ������� ������� ��� �������.
static const char fcJrnEmpty[] PROGMEM = "������ ����";
/// ����� ������ ������� ������ ������� � �� ����������.
static const char fcJrnNumEntries[] PROGMEM = "������ %u / %u";
/// ����� ������ ������� ������ �������, ���-�� ������� � ������� ������ � ������
/// ���������� ������� � ������� (��� ������).
static const char fcJrnNumEntriesOpto[] PROGMEM = "������ %u / %u (%u)";
/// ����� ���������� � ������� ���� ������� ������.
static const char fcRegimeJrn[] PROGMEM = "�����:";
/// ��������� ���������� � ������� ���� ������� ������.
static const char fcStateJrn[] PROGMEM = "���������:";
/// ����� �������, ��� ������� ���� ������� ������.
static const char fcNumComJrn[] PROGMEM = "�������: %u";
/// ����� ������� ���������, ��� ������� ���� ������� ������.
static const char fcNumComJrnPrm[] PROGMEM = "�������: %u %S";
/// ����, ����� ���� ������� ������.
static const char fcDateJrn[] PROGMEM = "����: %02u.%02u.%02u";
static const char fcDateJrnBCD[] PROGMEM = "����: %02X.%02X.%02X";
/// �����, ����� ���� ������� ������.
static const char fcTimeJrn[] PROGMEM = "�����: %02u:%02u:%02u.%03u";
static const char fcTimeJrnBCD[] PROGMEM = "�����: %02X:%02X:%02X.%03u";
/// ��������� � ���. ��� ���� ���������� ��������� ������ �������.
static const char fcJrnNotReady[] PROGMEM = "���������� ������";
/// ����� ��������� ������/������� ������ � ������ ������ �������.
static const char fcSignalDefJrn[] PROGMEM = "�%d�%d�%d ��%d��%d���%d";
/// �������� ������������ ������� (��� �������� ������ � �� � ��).
static const char fcJrnSourcePrd[GB_SOURCE_COM_MAX + 1][5] PROGMEM = {
	"",		///< ������� � ���������� ������
	"(��)",	///< ������� � �������� ������������
	"(��)"	///< ��������� ��������
};

/// ��������� ������� ���/��� ��� �������� ���� ������� ������.
static const char fcJrnPrd[GB_STATE_COM_MAX + 1][21] PROGMEM = {
	"��������� �������",	///<
	"������ �������",///<
	"��������� ��������"///< ��������� ��������
};
/// ������ �� ��������� ���� ������ ���/��� (������).
static const char fcJrnPrdOptoComNo[] PROGMEM = "������ ���";
/// ������ � ������� ������ �� ���/��� (������).
static const char fcJrnPrdOptoComYes[] PROGMEM = "������� ����������";
/// ������ � ������� ������ �� ��� (������).
static const char fcJrnPrmOptoComYes[] PROGMEM = "������� �����������";

/// ������ ������� ������� ��� �400.
static char fcJrnEventK400[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
	"������� - 0",///< 0 - ��������� ��������
	"���. �������/�������",///< 1
	"���������� �������",///< 2
	"��������� ������ ���",///< 3
	"��������� ����������",///< 4
	"������ ����� ���",///< 5
	"������ ����� ���",///< 6
	"������� - 7",///< 7
	"������ ����� ���",///< 8
	"��������� ����/�����",///< 9
	"������� - 10",///< 10
	"������� - 11",///< 11
	"������� - 12",///< 12
	"������ �� D",///< 13
	"�������������� �� D",///< 14
	"������ ������ ������",///< 15
	"�������������� D",///< 16
	"������. ������ DSP",///< 17
	"�������-� ������ DSP",///< 18
	"������ ����. ������",///< 19
	"������� ����. ������",///< 20
	"��� �� 5 ��� �� %S",///< 21
	"��� �� �� %S",///< 22
	"�������-� �� �� %S",///< 23
	"�����.��. �� �� %S",///< 24
	"������ ��/��� 2RAM",///< 25
	"������ ��/��� ����",///< 26
	"������ ��/��� FLASH",///< 27
	"������������� �����",///< 28
	"����.��. �� �� %S",///< 29
	"������ ������ ��",///< 30
	"���� RX �� ����",///< 31
	"������ �� ����������",///< 32
	"������� �����������",///< 33
	"������ �����������",///< 34
	"������. �����������",///< 35
	"������.������ ������",///< 36
	"������.������ ������",///< 37
	"������� - %d"///< ��������� ��������
};

/// ������ ������� ������� ��� ����.
static char fcJrnEventRZSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
	"������� - %d",///< 0 - ��������� ��������
	"���. �������/�������",///< 1
	"���������� �������",///< 2
	"��������� ������ ���",///< 3
	"��������� ����������",///< 4
	"������ ����� ���",///< 5
	"������ ����� ���",///< 6
	"������ ����� ���",///< 7
	"������ ����� ���",///< 8
	"������ ������-�� ���",///< 9
	"��� ������� �������.",///< 10
	"������ �������� ����",///< 11
	"��� �� 5 ��� �� %S",///< 12
	"�����-� ������� ����",///< 13
	"�����-� ������� ����",///< 14
	"������ ������ ������",///< 15
	"����. ��. �� �� %S",///< 16
	"������. ������ DSP",///< 17
	"�������-� ������ DSP",///< 18
	"������ ����. ������",///< 19
	"������� ����. ������",///< 20
	"��� �� 5 ��� �� %S",///< 21
	"��� �� �� %S",///< 22
	"�������-� �� �� %S",///< 23
	"�����. ��.�� �� %S",///< 24
	"������ ��/��� 2RAM",///< 25
	"������ ��/��� ����",///< 26
	"������ ��/��� FLASH",///< 27
	"������������� �����",///< 28
	"����. ��. �� �� %S",///< 29
	"�������-� �� �� %S",///< 30
	"��� �� �� %S",///< 31
	"�����. ��.�� �� %S",///< 32
	"������� - %d",///< 33
	"������� - %d",///< 34
	"������� - %d",///< 35
	"������� - %d",///< 36
	"������� - %d",///< 37
	"������� - %d"///< ��������� ��������
};

/// ������ ������� ������� ��� �400�.
static char fcJrnEventR400_MSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
	"������� - %d",///< 0 - ��������� ��������
	"������ ��/��� FLASH",///< 1
	"�� ����� ����������.",///< 2
	"������ ��/��� ����",///< 3
	"������������",///< 4
	"��� �����",///< 5
	"������ ��/��� 2RAM",///< 6
	"������ ������ DSP",///< 7
	"�������-� ������ DSP",///< 8
	"������ ����. ������",///< 9
	"������� ����. ������",///< 10
	"������� ����� � ��",///< 11
	"������������� �����",///< 12
	"��� ����� ���",///< 13
	"������ ������ ���",///< 14
	"������ ������-�� ���",///< 15
	"��� ������� ���",///< 16
	"���.���/����������",///< 17
	"��������� ������",///< 18
	"������ ���. ���.����",///< 19
	"���� �����. ���.����",///< 20
	"�� - �������� ������",///< 21
	"�� - ��� ������",///< 22
	"�����-� ������� ����",///< 23
	"�����-� ���� �������",///< 24
	"���������� ��������",///< 25
	"������ � �����",///< 26
	"������������� ���",///< 27
	"��: �� - ��� ������",///< 28
	"��: ������ � �����",///< 29
	"��: ������. ���",///< 30
	"��: ������. ���� ���",///< 31
	"����� �� ������",///< 32
	"������� - %d",///< 33
	"������� - %d",///< 34
	"������� - %d",///< 35
	"������� - %d",///< 36
	"������� - %d",///< 37
	"������� - %d"///< ��������� ��������
};

/// ������ ������� ������� ��� ������.
static char fcJrnEventOPTO[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
	"������� - %d",///< 0 - ��������� ��������
	"������ ��/��� FLASH",///< 1
	"������ ��/��� 2RAM",///< 2
	"������ ��/��� ����",///< 3
	"������� - %d",///< 4
	"������ ����� ���",///< 5
	"������ ����.���� ���",///< 6
	"������ ����� ���",///< 7
	"������ ��.������ ���",///< 8
	"��� ����� ���",///< 9
	"������ ������ ���",///< 10
	"����� ������. �����",///< 11
	"������������� �����",///< 12
	"��� ����� ���",///< 13
	"������ ������ ���",///< 14
	"������ ������-�� ���",///< 15
	"��� ������� ���",///< 16
	"���.���/����������",///< 17
	"��������� ������",///< 18
	"������ ���� ���.",///< 19
	"�������-� �� �� ���",///< 20
	"������ ������ ��",///< 21
	"�������-� ������ ��",///< 22
	"�����-� ������� ����",///< 23
	"�����-� ���� �������",///< 24
	"���������� �������",///< 25
	"��� ��",///< 26
	"��� �� 5 ������",///< 27
	"��: ���� ����������",///< 28
	"��: ��� ����������",///< 29
	"��: ��� ����������",///< 30
	"��������� ����/�����",///< 31
	"��������� ����������",///< 32
	"������� - %d",///< 33
	"������� - %d",///< 34
	"������� - %d",///< 35
	"������� - %d",///< 36
	"������� - %d",///< 37
	"������� - %d"///< ��������� ��������
};

/// ������ ������� ������� ��� ������ ������ ����������������
static char fcJrnEventOPTOring[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
	"������� - %d",///< 0 - ��������� ��������
	"������ ��/��� FLASH",///< 1
	"������ ��/��� 2RAM",///< 2
	"������ ��/��� ����",///< 3
	"������ ��������",///< 4
	"������ ����� ���",///< 5
	"������ ����.���� ���",///< 6
	"������ ����� ���",///< 7
	"������ ��.������ ���",///< 8
	"��� ����� ���",///< 9
	"������ ������ ���",///< 10
	"����� ������. �����",///< 11
	"������������� �����",///< 12
	"������ �������������",///< 13 ������
	"������������� �����",///< 14 ������
	"������� �� ���������",///< 15 ������
	"��� ������� ���",///< 16
	"���.���/����������",///< 17
	"��������� ������",///< 18
	"������ ���� ���.",///< 19
	"�������-� �� �� ���",///< 20
	"�������-� �� ���1",///< 21
	"�������-� �� ���2",///< 22
	"��� �� ���1",///< 23
	"��� �� ���2",///< 24
	"���������� �������",///< 25
	"��� ��",///< 26
	"��� �� 5 ������",///< 27
	"��: ���� ����������",///< 28
	"��: ��� ����������",///< 29
	"��: ��� ����������",///< 30
	"��������� ����/�����",///< 31
	"��������� ����������",///< 32
	"������� - %d",///< 33
	"������� - %d",///< 34
	"������� - %d",///< 35
	"������� - %d",///< 36
	"������� - %d",///< 37
	"������� - %d"///< ��������� ��������
};

/// ������� � ������ ���/���.
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
	"����.",///<
	"��",///<
	"��1",///<
	"��2",///<
	"��3",///<
	"��4",///<
	"��1",///< GB_SIGNAL_CF_NO_RZ
	"��1 � ����",///< GB_SIGNAL_CF_RZ
	"��2",///< GB_SIGNAL_CF2_NO_RZ
	"��2 � ����",///< GB_SIGNAL_CF2_RZ
	"��",///<
	"�������1",///< GB_SIGNAL_COM1_NO_RZ
	"�������2",///< GB_SIGNAL_COM2_NO_RZ
	"�������3",///< GB_SIGNAL_COM3_NO_RZ
	"�������4",///< GB_SIGNAL_COM4_NO_RZ
	"���1+����",///< GB_SIGNAL_COM1_RZ
	"���2+����",///< GB_SIGNAL_COM2_RZ
	"���3+����",///< GB_SIGNAL_COM3_RZ
	"���4+����",///< GB_SIGNAL_COM4_RZ
	"�������1",///<
	"�������2",///<
	"�������3",///<
	"�������4",///<
	"�������5",///<
	"�������6",///<
	"�������7",///<
	"�������8",///<
	"�������9",///<
	"�������10",///<
	"�������11",///<
	"�������12",///<
	"�������13",///<
	"�������14",///<
	"�������15",///<
	"�������16",///<
	"�������17",///<
	"�������18",///<
	"�������19",///<
	"�������20",///<
	"�������21",///<
	"�������22",///<
	"�������23",///<
	"�������24",///<
	"�������25",///<
	"�������26",///<
	"�������27",///<
	"�������28",///<
	"�������29",///<
	"�������30",///<
	"�������31",///<
	"�������32",///<
	"�� � ��",///< GB_SIGNAL_CF_RZ_R400M
	"��1 � ��",///< GB_SIGNAL_CF1_RZ_R400M
	"��2 � ��",///< GB_SIGNAL_CF2_RZ_R400M
	"��3 � ��",///< GB_SIGNAL_CF3_RZ_R400M
	"��4 � ��",///< GB_SIGNAL_CF4_RZ_R400M
	"��",///< � ������ ������ ��
	"�������1A",///< ������ ���������������� vvv
	"�������2A",
	"�������3A",
	"�������4A",
	"�������5A",
	"�������6A",
	"�������7A",
	"�������8A",
	"�������9A",
	"�������10A",
	"�������11A",
	"�������12A",
	"�������13A",
	"�������14A",
	"�������15A",
	"�������16A",
	"�������17A",
	"�������18A",
	"�������19A",
	"�������20A",
	"�������21A",
	"�������22A",
	"�������23A",
	"�������24A",
	"�������25A",
	"�������26A",
	"�������27A",
	"�������28A",
	"�������29A",
	"�������30A",
	"�������31A",
	"�������32A",
	"�������1B",
	"�������2B",
	"�������3B",
	"�������4B",
	"�������5B",
	"�������6B",
	"�������7B",
	"�������8B",
	"�������9B",
	"�������10B",
	"�������11B",
	"�������12B",
	"�������13B",
	"�������14B",
	"�������15B",
	"�������16B",
	"�������17B",
	"�������18B",
	"�������19B",
	"�������20B",
	"�������21B",
	"�������22B",
	"�������23B",
	"�������24B",
	"�������25B",
	"�������26B",
	"�������27B",
	"�������28B",
	"�������29B",
	"�������30B",
	"�������31B",
	"�������32B",
	"�������1C",
	"�������2C",
	"�������3C",
	"�������4C",
	"�������5C",
	"�������6C",
	"�������7C",
	"�������8C",
	"�������9C",
	"�������10C",
	"�������11C",
	"�������12C",
	"�������13C",
	"�������14C",
	"�������15C",
	"�������16C",
	"�������17C",
	"�������18C",
	"�������19C",
	"�������20C",
	"�������21C",
	"�������22C",
	"�������23C",
	"�������24C",
	"�������25C",
	"�������26C",
	"�������27C",
	"�������28C",
	"�������29C",
	"�������30C",
	"�������31C",
	"�������32C",///< ������ ���������������� ^^^
	"������"///< ��������� ��������
};

