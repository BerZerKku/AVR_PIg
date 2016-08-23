/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <util/delay.h>
#include "keyboard.h"
#include "debug.h"

/// ������ ������
static const eKEY fcKey[] = {
		KEY_1, 		KEY_2,		KEY_3,		KEY_ENTER,
		KEY_4,		KEY_5,		KEY_6,		KEY_UP,
		KEY_7,		KEY_8,		KEY_9,		KEY_DOWN,
		KEY_MENU,	KEY_0,		KEY_HELP,	KEY_CANCEL
};

// ������ ������������� ������
static const uint8_t fcDecodeRow[16] = {0xF0,12,8,0xF0,4,0xF0,0xF0,0xF0,0,
		0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0};

// ������ ������������� �������
static const uint8_t fcDecodeCol[16] = {0xF0,0,2,0xF0,1,0xF0,0xF0,0xF0,3,
		0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0
};

/// ��� ������� ������
static eKEY keyPressed;

/**	���������� �������� ������� ������.
 *
 * 	@return eKEY ��� ������� ������.
 */
eKEY eKEYget(eGB_TYPE_DEVICE type) {
	eKEY key = keyPressed;

	keyPressed = KEY_NO;
	return key;
}

/**	������� ����������� ������� ������.
 *
 *	�������� ���� ��� � 10��.
 *	������� ������ ����������� ������ ���� ���������� ���� ����������.
 *	������� ���� � ����� ������ �� �����������.
 *	��������� ������������ ������ TIME_DELAY_REPEAT * (������ ������ �-��).
 *	����� ������������ TIME_DELAY * (������ ������ �-��).
 *
 * 	@param ���
 * 	@return ���
 */
void vKEYmain(void) {
	static uint_fast8_t delay = TIME_DELAY;	// ������� ������������
	static uint_fast8_t keyPrev = KEY_NO;	// ���������� �������� ������
	uint_fast8_t tmp;						// ��������� ����������


	for(uint8_t i = 0x01; i < 0x10; i <<= 1) {
		DDRC = i;
		_delay_us(1);
		tmp = ~PINC;
		if (tmp != i) {
			break;
		} else {
			tmp = 0;
		}
	}
	DDRC = 0;

	// ����������� ���� ������� ������
	tmp = fcDecodeRow[tmp & 0x0F] + fcDecodeCol[tmp >> 4];
	tmp = (tmp > sizeof(fcKey)) ? KEY_NO : fcKey[tmp];

	// �������� �� ���������� � ���������� ���������
	// ���� �� �������, �������� ������� ������������
	// ���� ������ �� ������, ����������� �� ���������
	// ���� ������ ������������ ���������� �����,
	// ������������ ������������ � �������� TIME_DELAY_REPEAT
	if (tmp != keyPrev) {
		keyPrev = tmp;
		delay = TIME_DELAY;
	} else {
		if (delay > 0) {
			delay--;
		} else {
			// ���� ���������� ������� ��� ����������, ������ ����� �������
			// ������� ����� ������ ������� - ������������
			if (keyPressed == KEY_NO)
				keyPressed = (eKEY) tmp;
			delay = TIME_DELAY_REPEAT;
		}
	}
}

