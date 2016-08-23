/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "glbDefine.h"

/// Время антидребезга * 10мс
#define TIME_DELAY 3
/// Время до повторного определения
#define TIME_DELAY_REPEAT 100

/// Функции кнопок
enum eKEY {
	KEY_NO = 0,	////< Нет нажатых кнопок.

	KEY_0,		///< 0.
	KEY_1,		///< 1.
	KEY_2,		///< 2.
	KEY_3,		///< 3.
	KEY_4,		///< 4.
	KEY_5,		///< 5.
	KEY_6,		///< 6.
	KEY_7,		///< 7.
	KEY_8,		///< 8.
	KEY_9,		///< 9.

//	// временно vvv
//	KEY_RESET_IND,
//	KEY_PUSK,
//	KEY_RIGHT,
//	KEY_LEFT,
//	// временно ^^^

	KEY_ENTER,	///< Ввод ("ENT").
	KEY_UP,		///< Вверх.
	KEY_DOWN,	///< Вниз.
	KEY_CANCEL,	///< Отмена ("CE/C").
	KEY_HELP,	///< Подсказка ('#').
	KEY_MENU	///< Меню ('*').
};

void vKEYmain	(void);
eKEY eKEYget	(eGB_TYPE_DEVICE type);

#endif /* KEYBOARD_H_ */
