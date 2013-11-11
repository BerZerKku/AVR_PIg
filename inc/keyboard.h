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


//		���������� ��� �400, ����:
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

//		���������� ��� �400�:
//	--------- -------- --------------
//	|KEY_2	| |KEY_4 | |KEY_5		|
//	| SB7 	| | SB8  | | SB9 		|
//	|�����	| |����� | |     		|		�������� �-��
//	|     	| |����� | |��.����.���	| 		�������������� �-��
//	--------- -------- --------------
//	--------- -------- --------------
//	|KEY_3	| |KEY_6 | |KEY_7		|
//	| SB4 	| | SB5  | | SB6 		|
//	|�����	| |����  | |�����		|		�������� �-��
//	|���� ��| |      | |��.���� ��	|		�������������� �-��
//	--------- -------- --------------
//	--------- -------- --------------
//	|KEY_1	| |KEY_8 | |KEY_9		|
//  | SB1 	| | SB2  | | SB3 		|
//	|		| |����  | | ������ 	|		�������� �-��
//	|���. ��| |���.��| | �.����		|		�������������� �-��
//	--------- -------- --------------

enum eKEY
{
	KEY_NO = 0,
	KEY_CANCEL 			= 0x01,	// KEY_1
	KEY_DOWN,					// KEY_8
	KEY_RESET,					// KEY_9
	KEY_LEFT,					// KEY_3
	KEY_ENTER,					// KEY_6
	KEY_RIGHT,					// KEY_7
	KEY_EMPTY,
	KEY_UP,						// KEY_4
	KEY_RESET_IND,				// KEY_5

	KEY_FUNC 			= 0x80,		// KEY_2
	KEY_FUNC_AK 		= KEY_FUNC | KEY_DOWN,
	KEY_FUNC_ENTER 		= KEY_FUNC | KEY_LEFT,
	KEY_FUNC_CHECK 		= KEY_FUNC | KEY_RIGHT,
	KEY_FUNC_CALL		= KEY_FUNC | KEY_UP,
	KEY_FUNC_RES_IND	= KEY_FUNC | KEY_RESET_IND,
	KEY_FUNC_RESET 		= KEY_FUNC | KEY_RESET,

	// ����
//	KEY_FUNC_CALL		= KEY_FUNC | KEY_UP,
	KEY_FUNC_PUSK_PRD 	= KEY_FUNC | KEY_RESET_IND,
	KEY_FUNC_PUSK_AC_UD = KEY_FUNC | KEY_RIGHT,
	KEY_FUNC_PUSK_NALAD = KEY_FUNC | KEY_RESET,
	KEY_FUNC_RESET_AC	= KEY_FUNC | KEY_CANCEL,
	KEY_FUNC_AC_PUSK	= KEY_FUNC | KEY_LEFT

};

void vKEYmain	(void);
eKEY eKEYget	(eGB_TYPE_DEVICE type);

#endif /* KEYBOARD_H_ */
