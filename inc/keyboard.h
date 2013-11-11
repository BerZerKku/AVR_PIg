/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "../inc/glbDefine.h"

/// Время антидребезга * 10мс
#define TIME_DELAY 3
/// Время до повторного определения
#define TIME_DELAY_REPEAT 100


//		Клавиатура для К400, РЗСК:
//	------- ------- -------
//	|KEY_2| |KEY_4| |KEY_5|
//	| SB7 | | SB8 | | SB9 |
//	|ФУНКЦ| |ВВЕРХ| |     |		Основная ф-ия
//	|     |	|ВЫЗОВ| |СБР.И|		Альтернативная ф-ия
//	------- ------- -------
//	------- ------- -------
//	|KEY_3|	|KEY_6|	|KEY_7|
//	| SB4 | | SB5 | | SB6 |
//	|ВЛЕВО|	|МЕНЮ | |ВПРАВ|		Основная ф-ия
//	|ВВОД |	|     | |ПРОВ.|		Альтернативная ф-ия
//	------- ------- -------
//	-------	-------	-------
//	|KEY_1|	|KEY_8| |KEY_9|
//  | SB1 | | SB2 | | SB3 |
//	|ОТМЕН|	|ВНИЗ | |     |		Основная ф-ия
//	|     | |АК   | |СБРОС|		Альтернативная ф-ия
//	------- -------	-------

//		Клавиатура для Р400М:
//	--------- -------- --------------
//	|KEY_2	| |KEY_4 | |KEY_5		|
//	| SB7 	| | SB8  | | SB9 		|
//	|ФУНКЦ	| |ВВЕРХ | |     		|		Основная ф-ия
//	|     	| |ВЫЗОВ | |Уд.пуск.ПРД	| 		Альтернативная ф-ия
//	--------- -------- --------------
//	--------- -------- --------------
//	|KEY_3	| |KEY_6 | |KEY_7		|
//	| SB4 	| | SB5  | | SB6 		|
//	|ВЛЕВО	| |МЕНЮ  | |ВПРАВ		|		Основная ф-ия
//	|Пуск АК| |      | |Уд.пуск АК	|		Альтернативная ф-ия
//	--------- -------- --------------
//	--------- -------- --------------
//	|KEY_1	| |KEY_8 | |KEY_9		|
//  | SB1 	| | SB2  | | SB3 		|
//	|		| |ВНИЗ  | | Отмена 	|		Основная ф-ия
//	|Сбр. АК| |Реж.АК| | н.Пуск		|		Альтернативная ф-ия
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

	// ПВЗЛ
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
