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
		"������"
};

// ������ ���������, ��� ����� � ����������
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"�������",	//
		"������",	//
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
		"���"	//
};

static char fcInterface[GB_INTERFACE_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"USB",		//
		"RS485",	//
		"������"	//
};

static char fcBaudRate[UART_BAUD_RATE_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1200",		//
		"2400",		//
		"4800",		//
		"9600",		//
		"19200",	//
		"38400",	//
		"57600",	//
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
#ifdef UART_PARITY_MARK
		"������",
#endif
#ifdef UART_PARITY_SPACE
		"������",
#endif
		"������"
};

static char fcStopBits[UART_STOP_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",		//
#ifdef UART_STOP_BITS_ONEPONTFIVE
		"1.5",		//
#endif
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

//	������
static const char fcJrnEmpty[] 			PROGMEM = "������ ����";
static const char fcJrnNumEntries[] 	PROGMEM = "������ %u / %u";
static const char fcRegimeJrn[]			PROGMEM = "�����:";
static const char fcStateJrn[] 			PROGMEM = "���������:";
static const char fcDateJrn[]			PROGMEM = "����: %02u.%02u.%02u";
static const char fcTimeJrn[]			PROGMEM = "�����: %02u:%02u:%02u.%03u";
static const char fcJrnNotReady[]		PROGMEM = "���������� ������";

/// ������ ������� ������� ��� ����
static char fcJrnEvent[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"������� - %d",				// 0 - ��������� ��������
		"���. �������/�������",		// 1
		"���������� �������",       // 2
		"��������� ������ ���",     // 3
		"������� - %d",           	// 4
		"������ ����� ���",         // 5
		"������ ����� ���",         // 6
		"������ ����� ���",         // 7
		"������ ����� ���",         // 8
		"������ ������-�� ���",   	// 9
		"��� ������� �������.",     // 10
		"������ �������� ����",     // 11
		"��� ������� ��",           // 12
		"�����-� ������� ����",    	// 13
		"�����-� ������� ����",    	// 14
		"������ ������ ������",     // 15
		"������� - %d",          	// 16
		"������. ������ DSP",		// 17
		"�������-� ������ DSP",		// 18
		"������ ����. ������",    	// 19
		"������� ����. ������",    	// 20
		"��� �� 5 ��� �� ���1",	    // 21
		"��� �� 5 ��� �� ���2",		// 22
		"�������-� �� �� ���1",     // 23
		"�������-� �� �� ���2",		// 24
		"������ ��/��� 2RAM",    	// 25
		"������ ��/��� ����",    	// 26
		"������ ��/��� FLASH",   	// 27
		"������������� �����",      // 28
		"������� - %d",       		// 29
		"������� - %d",        		// 30
		"������� - %d",         	// 31
		"������� - %d",    			// 32
		"������� - %d"          	// MAX_JRN_EVENT_VALUE - ��������� ��������
};
