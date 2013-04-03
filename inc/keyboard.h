/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

/// ����� ������������ * 10��
#define TIME_DELAY 3
/// ����� �� ���������� �����������
#define TIME_DELAY_REPEAT 100


//		����������:
//	------- ------- -------
//	|KEY_2| |KEY_4| |KEY_5|
//	| SB7 | | SB8 | | SB9 |
//	|�����| |�����| |     |		�������� �-��
//	|     |	|�����| |���.�|		�������������� �-��
//	------- ------- -------
//	------- ------- -------
//	|KEY_3|	|KEY_6|	|KEY_7|
//	| SB4 | | SB5 | | SB6 |
//	|�����|	|���� | |�����|		�������� �-��
//	|���� |	|     | |����.|		�������������� �-��
//	------- ------- -------
//	-------	-------	-------
//	|KEY_1|	|KEY_8| |KEY_9|
//  | SB1 | | SB2 | | SB3 |
//	|�����|	|���� | |     |		�������� �-��
//	|     | |��   | |�����|		�������������� �-��
//	------- -------	-------

enum eKEY
{
	KEY_NO,
	KEY_CANCEL 		= 0x01,
	KEY_DOWN,
	KEY_RESET,
	KEY_LEFT,
	KEY_ENTER,
	KEY_RIGHT,
	KEY_EMPTY,
	KEY_UP,
	KEY_RESET_IND,

	KEY_FUNC 		= 0x80,
	KEY_FUNC_AK 	= KEY_FUNC | KEY_DOWN,
	KEY_FUNC_ENTER 	= KEY_FUNC | KEY_LEFT,
	KEY_FUNC_CHECK 	= KEY_FUNC | KEY_RIGHT,
	KEY_FUNC_CALL	= KEY_FUNC | KEY_UP,
	KEY_FUNC_RES_IND= KEY_FUNC | KEY_RESET_IND,
	KEY_FUNC_RESET 	= KEY_FUNC | KEY_RESET
};

void vKEYmain	(void);
eKEY eKEYget	(void);

#endif /* KEYBOARD_H_ */
