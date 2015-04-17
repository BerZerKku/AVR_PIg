/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

//#include "glbDefine.h"

/// Время антидребезга * 10мс
#define TIME_DELAY 3
/// Время до повторного определения
#define TIME_DELAY_REPEAT 100

/// Функции кнопок
enum eKEY {
	KEY_NO = 0x00,	// нет нажатых кнопок
	KEY_EMPTY,		// была нажата кнопка, но без какой-либо функции
	KEY_FUNC,		// Функция
	KEY_ENTER,		// Меню
	KEY_CANCEL,		// Отмена
	KEY_DOWN,		// Вниз
	KEY_LEFT,		// Влево
	KEY_RIGHT,		// Вправо
	KEY_UP,			// Вверх

	KEY_MENU,		// Вход в Меню
	KEY_RESET,		// Сброс
	KEY_PUSK,		// Пуск	(ввод приемника в работу)
	KEY_RESET_IND,	// Сброс инд.

	KEY_POWER_ON,	// Включение напряжения ЛЭП
	KEY_POWER_OFF	// Выключение напряжения ЛЭП
};

void vKEYmain	();
eKEY eKEYget	();
uint8_t timePressKey();

#endif /* KEYBOARD_H_ */
