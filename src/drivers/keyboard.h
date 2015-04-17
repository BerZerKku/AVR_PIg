/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

//#include "glbDefine.h"

/// ����� ������������ * 10��
#define TIME_DELAY 3
/// ����� �� ���������� �����������
#define TIME_DELAY_REPEAT 100

/// ������� ������
enum eKEY {
	KEY_NO = 0x00,	// ��� ������� ������
	KEY_EMPTY,		// ���� ������ ������, �� ��� �����-���� �������
	KEY_FUNC,		// �������
	KEY_ENTER,		// ����
	KEY_CANCEL,		// ������
	KEY_DOWN,		// ����
	KEY_LEFT,		// �����
	KEY_RIGHT,		// ������
	KEY_UP,			// �����

	KEY_MENU,		// ���� � ����
	KEY_RESET,		// �����
	KEY_PUSK,		// ����	(���� ��������� � ������)
	KEY_RESET_IND,	// ����� ���.

	KEY_POWER_ON,	// ��������� ���������� ���
	KEY_POWER_OFF	// ���������� ���������� ���
};

void vKEYmain	();
eKEY eKEYget	();
uint8_t timePressKey();

#endif /* KEYBOARD_H_ */
