/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "../inc/glbDefine.h"

/// ����� ������������ * 10��
#define TIME_DELAY 3
/// ����� �� ���������� �����������
#define TIME_DELAY_REPEAT 100

/// ������� ������
enum eKEY {
	KEY_NO = 0x00,	// ��� ������� ������
	KEY_0,		// 0
	KEY_1,		// 1
	KEY_2,		// 2
	KEY_3,		// 3
	KEY_4,		// 4
	KEY_5,		// 5
	KEY_6,		// 6
	KEY_7,		// 7
	KEY_8,		// 8
	KEY_9,		// 9

	KEY_ENTER,	// ����
	KEY_UP,		// �����
	KEY_DOWN,	// ����
	KEY_CANCEL,	// ������
	KEY_SHARP,	// ���������
	KEY_STAR	// ����
};

void vKEYmain	(void);
eKEY eKEYget	(eGB_TYPE_DEVICE type);

#endif /* KEYBOARD_H_ */
