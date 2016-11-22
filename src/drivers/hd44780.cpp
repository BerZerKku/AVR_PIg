#include <avr/io.h>
#include <avr/pgmspace.h>
//#include <util/delay.h>


#include "debug.h"
#include "hd44780.h"

/// ������ �������
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

// ��������� ������ ������������� ���
typedef struct {
	uint8_t com;	// ������� ��� �������� � ���
	uint16_t time;	// ����� �������� ����� �������� ������� � ���
} SComInit;

// ������� �������������� ������� �������� � ���� ���
static const uint8_t decode[] PROGMEM = {
		//A	  �    �	�    �    �    �    �    �    �    �    �    �    �    �    �
		0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
		//�   �    �    �    �    �    �    �    �    �    �    �    �    �    �    �
		0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
		//�   �    �    �    �    �    �    �    �    �    �    �    �    �    �    �
		0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
		//�   �    �    �    �    �    �    �    �    �    �    �    �    �    �    �
		0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};


/// ���� ������������� �������� ������ �� ���
volatile bool bRefresh = false;
/// ���� ������������� �������������������� �������
volatile bool bReInit = false;


/// ����� ������ ��� ������ �� ���
char *buf;

void sendData(uint8_t val);	//	�������� ����� ������ � ���.
void sendCom(uint8_t val);	//	�������� ������� � ���.

// ������������� �������.
void vLCDinit(void) {
	bReInit = true;
};

// ������� ����������� ������
//void vLCDclear(void) {
//	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
//		buf[i] = ' ';
//	}
//}

// �������� ������� ���������� ����������� ���.
void vLCDmain(void) {
	// ������ ������ ������������� ���
	static const SComInit comInit[] PROGMEM = {
			// �����������������
			{0x38,  50},	// 8-���, 2 ����� � ���� ����� 5��
			{0x38,	10},	// 8-���, 2 ����� � ���� ����� 1��
			{0x38, 	2},		// 8-���, 2 ����� � ���� ����� 0.2��
			{0x14, 	2},		// ����� ������ DDRAM � ���� ����� 0.2��
			{0x06, 	2},		// ��������� �������� DDRAM � ���� ����� 0.2��
			{0x02, 	40},	// ����� DDRAM � ����� � 0 � ���� ����� 4��
			{0x0C, 	2}		// ���. ������� � ����. ������� � ���� ����� 0.2��
	};

	// ���� �������� �������������
	static bool init = true;
	// ������� ��������� � ������ �������� ��� �������������
	// ��� ������ ������ ���� 0, ����� ������������� ����� � ������
	static uint8_t cnt = 0;
	// ������� �� ��������, ������ ���� 0
	static uint8_t com = 0;
	// ����� �������� �� �������� ������� (40 ��) ����� ���������
	static uint16_t time = 400;

	// ������������ ����� ����� ���������� �������
	if ((time > 0) && (--time > 0)) {
		return;
	}

	// �������� ������������� �����������������
	if (bReInit) {
		init = true;
		cnt = 0;
		bReInit = false;
	}

	// ������������� ���
	if (init) {
		if (cnt < SIZE_OF(comInit)) {
			com = pgm_read_byte(&comInit[cnt].com);
			time = pgm_read_byte(&comInit[cnt].time);
			cnt++;
		} else {
			init = false;
			cnt = 0;
		}
	}


	if (com != 0) {
		// �������� �������
		sendCom(com);
		com = 0;
	} else if (bRefresh) {
		// ���������� ����������� ���

		// �������� ����� ������
		sendData(buf[cnt]);

		// ���������� ������ ������ � ���
		if (++cnt >= SIZE_BUF_STRING) {
			cnt = 0;
			bRefresh = false;
		}

		// ������� �� ������ ������
		if (cnt == 0) {
			com = 0x80;
		} else if (cnt == 20) {
			com = 0xC0;
		} else if (cnt == 40) {
			com = 0x94;
		} else if (cnt == 60) {
			com = 0xD4;
		}
	}
}

// ������ ���������� ����������� ���.
void vLCDrefresh(void) {
	bRefresh = true;
}

// �������������� ������� ��������, � ������ ��� ������ �� ���.
void vLCDputchar(char *b) {
	buf = b;
	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
		if ((buf[i] >= '�') && (buf[i] <= '�')) {
			buf[i] = pgm_read_byte(&decode[buf[i] - '�']);
		} else if (buf[i] == 0x00) {
			buf[i] = ' ';
		}
		// ������ '�' ������ � ��� �������� ������, ������� �� ����������
//		else if (buf[i] == 0xB0) {
//			buf[i] = 0xEF;	// ������ ������� '�'
//		}
	}
}

/**	�������� ����� ������ � ���.
 *
 * 	@param val ��� ������� � ������� ������ ���.
 */
void sendData(uint8_t val) {
	PORT_RS	|=  (1 << PIN_RS);
	PORT_ES	|=  (1 << PIN_ES);
	PORT_DB	 = val;
	PORT_ES	&= ~(1 << PIN_ES);
}

/**	�������� ������� � ���.
 *
 * 	@param val �������.
 *
 */
void sendCom(uint8_t val) {
	PORT_RS	&= ~(1 << PIN_RS);
	PORT_ES	|=  (1 << PIN_ES);
	PORT_DB	 = val;
	PORT_ES	&= ~(1 << PIN_ES);
}
