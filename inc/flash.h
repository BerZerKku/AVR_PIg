/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: ������
 */

#include <avr/pgmspace.h>
#include "glbDefine.h"

#define STRING_LENGHT 11

static const char fcNull[] PROGMEM = "";

// ���������� ���������
static const char fcUout[]  PROGMEM = "U=%02u.%01u�";
static const char fcIout[] 	PROGMEM	= "I=%03u��";
static const char fcRout[] 	PROGMEM = "R=%03u��";
static const char fcUz[]	PROGMEM	= "U�=%02d��";
static const char fcUz1[] 	PROGMEM = "U�1=%02d��";
static const char fcUz2[] 	PROGMEM = "U�2=%02d��";
static const char fcUcf[] 	PROGMEM	= "U�=%02d��";
static const char fcUcf1[] 	PROGMEM	= "U�1=%02d��";
static const char fcUcf2[] 	PROGMEM	= "U�2=%02d��";
static const char fcUn[] 	PROGMEM	= "U�=%02d��";
static const char fcUn1[] 	PROGMEM	= "U�1=%02d��";
static const char fcUn2[] 	PROGMEM	= "U�2=%02d��";
static const char fcSd[]	PROGMEM = "S�=%02u�";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// ����� ��������� ��������
static const char fcDeviceName00[] PROGMEM = "���";
static const char fcDeviceName01[] PROGMEM = "���";
static const char fcDeviceName02[] PROGMEM = "���";

// ���� ������������� ��� ����������� � ��������� ���� (eGB_TYPE_AC)
static const char fcAcType[GB_TYPE_AC_MAX + 2][9] PROGMEM = {
// 		12345678
		"��-����",		//
		"��-����",		//
		"��-����",		//
		"��-�����",		//
		"��-����",		//
		"��-����",		//
		"��-�����",		//
		"��-����",		//
		"��-�����",		//
		"��-����",		//
};
static const char fcTimeToAc[] PROGMEM = "%02d:%02d:%02d";

// ������ ���������
static const char fcRegime[GB_REGIME_MAX + 1][8] PROGMEM = {
//		 1234567
		"�������",	//
		"�����",	//
		"������",	//
		"����",		//
		"����",		//
		"����",		//
		"������"	//
};

// ������ ��������
static const char fcIC[GB_IC_MAX + 1][9] PROGMEM = {
//		 12345678
		"��� MCU ",	//
		"��� DSP ",	//
		"��  MCU ",	//
		"��� ���1",	//
		"��� ���2",	//
		"��� ���1",	//
		"��� ���2",	//
		"��� ����", //
		"������"
};

// ������ ���������, ��� ����� � ����������
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"�������",	//
		"������",	//
		"���� 1",	//
		"���� 2"	//
};

// ��� ������
static const char fcDefType[DEF_TYPE_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"���-����",	//
		"���-��1",	//
		"���-��2",	//
		"���-L60",	//
		"��-����",	//
		"��-��",	//
		"���-����",	//
		"���-��",	//
		"������"	//
};

static
const char fcNumDevices[GB_NUM_DEVICES_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"������",		//
		"2 ��������",	//
		"3 ��������",	//
		"������"		//
};

static const char fcPrmType[DEF_PRM_TYPE_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"���+����",		//
		"��������",		//
		"���������",	//
		"������"		//
};

static const char fcOnOff[3][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"����.",	//
		"���.",		//
		"������"	//
};

static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
		"���",	//
		"���",	//
		"���",	//
		"���",	//
		"���"	//
};

static const char fcDevicesK400[GB_DEVICE_K400_MAX + 1][5] PROGMEM = {
// 		 123
		"���",	//
		"���1",	//
		"���2",	//
		"���",	//
		"���",	//
		"�1,2",
		"���"	//
};

static
const char fcCompatibility[GB_COMPATIBILITY_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"����� �400",	//
		"���-90",		//
		"����-80",		//
		"����-�",		//
		"����",			//
		"������"		//
};

static const char fcPvzlFreq[GB_PVZL_FREQ_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"-1000��",	//
		"-500��",	//
		"0��",		//
		"+500��",	//
		"+1000��",	//
		"������"	//
};

static
const char fcPvzueProtocol[GB_PVZUE_PROTOCOL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"������",//
		"�������",		//
		"���������",	//
		"������"		//
};

static char fcPvzueTypeAC[GB_PVZUE_TYPE_AC_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"������",//
		"���������",	//
		"����������",	//
		"���������",	//
		"������"		//
};

static char fcPvzueParity[GB_PVZUE_PARITY_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"������",	//
		"���.",		//
		"����.",	//
		"������"	//

};

static char fcInterface[GB_INTERFACE_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"RS232",	//
		"RS485",	//
		"������"	//
};

static char fcProtocol[GB_PROTOCOL_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"��������",
		"MODBUS",
		"���-101",
		"������"
};

static char fcBaudRate[UART_BAUD_RATE_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"300",		//
		"600",		//
		"1200",		//
		"2400",		//
		"4800",		//
		"9600",		//
		"19200",	//
		"������"	//
};

static char fcDataBits[UART_DATA_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"8",		//
		"������"	//
};

static char fcParity[UART_PARITY_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"���",		//
		"���",		//
		"�����",	//
		"������"
};

static char fcStopBits[UART_STOP_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",		//
		"2",		//
};

// ��������� ���������
// ���-�� ��������� ��� ���� ��������� ������ ��������� � MAX_NUM_DEVICE_STATE
// ����������� ���������
static const char fcUnknownSost[] PROGMEM = "������";
// ��������� ������
static const char fcDefSost00[] PROGMEM =	"������.";
static const char fcDefSost01[] PROGMEM =	"��������";
static const char fcDefSost02[] PROGMEM =	"����";
static const char fcDefSost03[] PROGMEM =	"�������";
static const char fcDefSost04[] PROGMEM =	"������.";
static const char fcDefSost05[] PROGMEM =	"�.������";
static const char fcDefSost06[] PROGMEM =	"��������";
static const char fcDefSost07[] PROGMEM =	"���.����";
static const char fcDefSost08[] PROGMEM = 	"��.����";
static const char fcDefSost09[] PROGMEM =	"��� ��";
static const char fcDefSost10[] PROGMEM =	"����";
static const char fcDefSost11[] PROGMEM =	"���";
static const char fcDefSost12[] PROGMEM =	"���";
// ��������� ���������
static const char fcPrmSost00[] PROGMEM = 	"������.";
static const char fcPrmSost01[] PROGMEM = 	"��� ��%01u";
static const char fcPrmSost02[] PROGMEM = 	"��� ��%02u";
static const char fcPrmSost03[] PROGMEM = 	"��� ��";
static const char fcPrmSost04[] PROGMEM = 	"������.";
static const char fcPrmSost05[] PROGMEM = 	"�.������";
static const char fcPrmSost06[] PROGMEM = 	"��������";
static const char fcPrmSost07[] PROGMEM = 	"��� ��%02u";
static const char fcPrmSost08[] PROGMEM = 	"?0x08?";
static const char fcPrmSost09[] PROGMEM = 	"?0x09?";
static const char fcPrmSost10[] PROGMEM = 	"����";
static const char fcPrmSost11[] PROGMEM =	"���";
static const char fcPrmSost12[] PROGMEM =	"���";
// ��������� �����������
static const char fcPrdSost00[] PROGMEM = 	"������.";
static const char fcPrdSost01[] PROGMEM =	"��� ��%01u";
static const char fcPrdSost02[] PROGMEM =	"��� ��%02u";
static const char fcPrdSost03[] PROGMEM =	"��� ��";
static const char fcPrdSost04[] PROGMEM =	"������.";
static const char fcPrdSost05[] PROGMEM =	"�.������";
static const char fcPrdSost06[] PROGMEM =	"��������";
static const char fcPrdSost07[] PROGMEM = 	"?0x07?";
static const char fcPrdSost08[] PROGMEM = 	"��� ��%02u";
static const char fcPrdSost09[] PROGMEM =	"������.";
static const char fcPrdSost10[] PROGMEM =	"����";
static const char fcPrdSost11[] PROGMEM =	"���";
static const char fcPrdSost12[] PROGMEM =	"���";

// ������� ��� �������������� � ������� �������� �����
static const char fcUnknownFault[]		PROGMEM = "�������������";
static const char fcUnknownWarning[] 	PROGMEM = "��������������";

// ������ ������ ���� �������������/��������������
// %c - g ��� ����������, l - ��� ���������
static const char fcFaults[] 			PROGMEM = "������. %c-%04X";
static const char fcWarnings[] 			PROGMEM = "�������. %c-%04X";

// ����� �������������
static const char fcGlbFault0001[] 		PROGMEM = "������.��. FLASH";
static const char fcGlbFault0002[] 		PROGMEM = "������.���.FLASH";
static const char fcGlbFault0004[] 		PROGMEM = "������.��. PLIS";
static const char fcGlbFault0008[] 		PROGMEM = "������.���.PLIS";
static const char fcGlbFault0010[] 		PROGMEM = "������.���.2RAM";
static const char fcGlbFault0020[] 		PROGMEM = "��-��� ������";
static const char fcGlbFault0020rzsk[] 	PROGMEM = "��� ������� ���";
static const char fcGlbFault0040[] 		PROGMEM = "��-������.������";
static const char fcGlbFault0080[] 		PROGMEM = "������ � �����";
static const char fcGlbFault0100[] 		PROGMEM = "������.DSP 2RAM ";
static const char fcGlbFault0200[] 		PROGMEM = "������.��. 2RAM ";
static const char fcGlbFault0400[] 		PROGMEM = "��� �����";
static const char fcGlbFault0400rzsk[] 	PROGMEM = "���������� ����";
static const char fcGlbFault0800[]		PROGMEM = "������ ����.���";
static const char fcGlbFault1000[] 		PROGMEM = "������� ����.���";
static const char fcGlbFault2000[] 		PROGMEM = "������. �� ��";
static const char fcGlbFault4000[] 		PROGMEM = "�� ����� �����.";

// ����� ��������������
static const char fcGlbWarning01[] 		PROGMEM = "���������� ����";
static const char fcGlbWarning02[] 		PROGMEM = "����� ������.���";
static const char fcGlbWarning10[] 		PROGMEM = "����.���� ������";
static const char fcGlbWarning20[] 		PROGMEM = "����.��� ������";
static const char fcGlbWarning40[] 		PROGMEM = "����.��� ������";

// ������������� ������
static const char fcDefFault0001[] 		PROGMEM = "��� ����� ���";
static const char fcDefFault0002[] 		PROGMEM = "������.����.���";
static const char fcDefFault0004[] 		PROGMEM = "������.������.";
static const char fcDefFault0008[] 		PROGMEM = "������.���. ���";
static const char fcDefFault0010[] 		PROGMEM = "��-��� ������";
static const char fcDefFault0010rzsko[]	PROGMEM = "��� ������� ���";
static const char fcDefFault0020[] 		PROGMEM = "������ ��. ��";
static const char fcDefFault0040[] 		PROGMEM = "������.����. ���";
static const char fcDefFault0080[] 		PROGMEM = "������.����. ���";
static const char fcDefFault0100[] 		PROGMEM = "������.����.����";
static const char fcDefFault0200[] 		PROGMEM = "������.����.����";
static const char fcDefFault0400[] 		PROGMEM = "��.���� ��� ���.";
static const char fcDefFault0800[] 		PROGMEM = "������.����. ���";
static const char fcDefFault1000[] 		PROGMEM = "��.���� ���.���.";
static const char fcDefFault2000[] 		PROGMEM = "������.���. ���";
static const char fcDefFault4000[] 		PROGMEM = "��������. ������";
static const char fcDefFault4000rzsk[]	PROGMEM = "��� ������� ��";
static const char fcDefFault8000[] 		PROGMEM = "������. ���";

// �������������� ������
static const char fcDefWarning01[] 		PROGMEM = "��-������.������";
static const char fcDefWarning01rzsk[] 	PROGMEM = "������ ����. ��";
static const char fcDefWarning01rzsko[] PROGMEM = "��� ��";
static const char fcDefWarning02[] 		PROGMEM = "��� ������� ���";
static const char fcDefWarning04[] 		PROGMEM = "����� �� ������";
static const char fcDefWarning08[] 		PROGMEM = "������������";

// ������������� ���������
static const char fcPrmFault0001rzsk[] 	PROGMEM = "��� ����� ���";
static const char fcPrmFault0002rzsk[] 	PROGMEM = "������.����. ���";
static const char fcPrmFault0004rzsk[] 	PROGMEM = "��� ��";
static const char fcPrmFault0008rzsk[] 	PROGMEM = "����.��-���.���.";
static const char fcPrmFault0100rzsk[] 	PROGMEM = "������.���. ���";
static const char fcPrmFault0200rzsk[] 	PROGMEM = "������.����. ���";
static const char fcPrmFault0400rzsk[] 	PROGMEM = "������.���. ���";
static const char fcPrmFault0800rzsk[] 	PROGMEM = "������.�����.���";
static const char fcPrmFault1000rzsk[] 	PROGMEM = "�������.��� ��";
static const char fcPrmFault8000rzsk[] 	PROGMEM = "����� ����. ���";

// �������������� ���������
static const char fcPrmWarning01rzsk[] 	PROGMEM = "����. ������ ���";
static const char fcPrmWarning01rzsko[] PROGMEM = "��� ��";
static const char fcPrmWarning02k400[] 	PROGMEM = "������ ������ ��";
static const char fcPrmWarning04k400[]	PROGMEM = "���� RX �� ����";

// ������������� �����������
static const char fcPrdFault0001rzsk[] 	PROGMEM = "��� ����� ���";
static const char fcPrdFault0002rzsk[] 	PROGMEM = "������.����. ���";
static const char fcPrdFault0100rzsk[] 	PROGMEM = "������.��.������";
static const char fcPrdFault0200rzsk[] 	PROGMEM = "������.����.����";
static const char fcPrdFault0400rzsk[] 	PROGMEM = "������.���. ����";
static const char fcPrdFault0800rzsk[] 	PROGMEM = "������.����. ���";

// �������������� �����������
static const char fcPrdWarning02k400[] 	PROGMEM = "������ ������ ��";
static const char fcPrdWarning04k400[] 	PROGMEM = "���� RX �� ����";

// ���������
static const char fcEnterInt[]			PROGMEM = "����: %f";
static const char fcValue[] 			PROGMEM = "��������: ";
static const char fcRange[] 			PROGMEM = "��������: ";
static const char fcNumCom[] 			PROGMEM = "����� �������: %u/%u";
static const char fcNumPrm[]			PROGMEM = "����� ���: %u/%u";
static const char fcRangeDec[] 			PROGMEM = "%d..%d%s";
static const char fcRangeList[] 		PROGMEM = "������";
static const char fcRangeOnOff[]		PROGMEM = "���./����.";
static const char fcNumPunkt[] 			PROGMEM = "��������:%u �����:%u";

//static const char fcOn[]				PROGMEM = "���.";
//static const char fcOff[]				PROGMEM = "����.";

//	������
static const char fcJrnEmpty[] 			PROGMEM = "������ ����";
static const char fcJrnNumEntries[] 	PROGMEM = "(%u/%u)";
static const char fcNumComJrn[]			PROGMEM = "���.%u";
static const char fcDateJrn[]			PROGMEM = "%02u.%02u.%02u";
static const char fcTimeJrn[]			PROGMEM = "%02u:%02u:%02u.%03u";
static const char fcJrnNotReady[]		PROGMEM = "���������� ������";
static const char fcSignalDefJrn[]		PROGMEM = "�%u�%u�%u ��%u��%u���%u";

/// �������� ������������ �������
static const char fcJrnSourcePrd[GB_SOURCE_COM_MAX + 1][5] PROGMEM = {
		"��%u",					// ������� � ���������� ������
		"��%u",					// ������� � �������� ������������
		"��%u"					// ��������� ��������
};

/// ������ ������� ��� � ���
static const char fcJrnPrd[GB_STATE_COM_MAX + 1][21] PROGMEM = {
		"��������� �������",	//
		"������ �������",		//
		"��������� ��������"	//
};

/// ������ ������� ������� ��� ����
static char fcJrnEventRZSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"������� - 0",				// 0 - ��������� ��������
		"���. �������/�������",		// 1
		"���������� �������",       // 2
		"��������� ������ ���",     // 3
		"������� - 4",           	// 4
		"������ ����� ���",         // 5
		"������ ����� ���",         // 6
		"������ ����� ���",         // 7
		"������ ����� ���",         // 8
		"������ ������-�� ���",   	// 9
		"��� ������� �������.",     // 10
		"������ �������� ����",     // 11
		"��� �� 5 ��� �� %S",       // 12
		"�����-� ������� ����",    	// 13
		"�����-� ������� ����",    	// 14
		"������ ������ ������",     // 15
		"����. ��. �� �� %S",	    // 16
		"������. ������ DSP",		// 17
		"�������-� ������ DSP",		// 18
		"������ ����. ������",    	// 19
		"������� ����. ������",    	// 20
		"��� �� 5 ��� �� %S",	    // 21
		"��� �� �� %S",				// 22
		"�������-� �� �� %S",   	// 23
		"�����. ��.�� �� %S",		// 24
		"������ ��/��� 2RAM",    	// 25
		"������ ��/��� ����",    	// 26
		"������ ��/��� FLASH",   	// 27
		"������������� �����",      // 28
		"����. ��. �� �� %S",		// 29
		"�������-� �� �� %S",  		// 30
		"��� �� �� %S",	        	// 31
		"�����. ��.�� �� %S",		// 32
		"������� - %d"          	// MAX_JRN_EVENT_VALUE - ��������� ��������
};

/// ������ ������� ������� ��� �400�
static char fcJrnEventR400_MSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
		"������� - 0",				// 0 - ��������� ��������
		"������ ��/��� FLASH",   	// 1
		"�� ����� ����������.",   	// 2
		"������ ��/��� ����",		// 3
		"������������",				// 4
		"��� �����",				// 5
		"������ ��/��� 2RAM",		// 6
		"������ ������ DSP",		// 7
		"�������-� ������ DSP",		// 8
		"������ ����. ������",    	// 9
		"������� ����. ������",    	// 10
		"������� ����� � ��",		// 11
		"������������� �����",      // 12
		"��� ����� ���",         	// 13
		"������ ������ ���",        // 14
		"������ ������-�� ���",   	// 15
		"��� ������� ���",			// 16
		"���.���/����������",		// 17
		"��������� ������",		    // 18
		"������ ���. ���.����",		// 19
		"���� �����. ���.����",		// 20
		"�� - �������� ������",		// 21
		"�� - ��� ������",			// 22
		"�����-� ������� ����",    	// 23
		"�����-� ���� �������",    	// 24
		"���������� ��������",      // 25
		"������ � ������",         	// 26
		"������������� ���",       	// 27
		"��: �� - ��� ������",     	// 28
		"��: ������ � ������",		// 29
		"��: ������. ���",         	// 30
		"��: ������. ���� ���",    	// 31
		"����� �� ������",         	// 32
		"������� - %d"          	// MAX_JRN_EVENT_VALUE - ��������� ��������
};

/// ������� � ������
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"����.",		//
		"��", 			//
		"��1",			//
		"��2",			//
		"��3",			//
		"��4",			//
		"��1",			//	GB_SIGNAL_CF_NO_RZ
		"��1 � ����",	//	GB_SIGNAL_CF_RZ
		"��2",			//	GB_SIGNAL_CF2_NO_RZ
		"��2 � ����",	//	GB_SIGNAL_CF2_RZ
		"��",			//
		"�������1",		//	GB_SIGNAL_COM1_NO_RZ
		"�������2",		//	GB_SIGNAL_COM2_NO_RZ
		"�������3",		//	GB_SIGNAL_COM3_NO_RZ
		"�������4",		//	GB_SIGNAL_COM4_NO_RZ
		"���1+����",	//	GB_SIGNAL_COM1_RZ
		"���2+����",	//	GB_SIGNAL_COM2_RZ
		"���3+����",	//	GB_SIGNAL_COM3_RZ
		"���4+����",	//	GB_SIGNAL_COM4_RZ
		"�� � ��",		//	GB_SIGNAL_CF_RZ_R400M
		"������"		//
};
