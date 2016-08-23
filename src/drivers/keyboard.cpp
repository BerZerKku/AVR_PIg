/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include <avr/io.h>
#include <util/delay.h>
#include "keyboard.h"
#include "debug.h"

/// Массив кнопок
static const eKEY fcKey[] = {
		KEY_1, 		KEY_2,		KEY_3,		KEY_ENTER,
		KEY_4,		KEY_5,		KEY_6,		KEY_UP,
		KEY_7,		KEY_8,		KEY_9,		KEY_DOWN,
		KEY_MENU,	KEY_0,		KEY_HELP,	KEY_CANCEL
};

// массив декодирования строки
static const uint8_t fcDecodeRow[16] = {0xF0,12,8,0xF0,4,0xF0,0xF0,0xF0,0,
		0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0};

// массив декодирования столбцы
static const uint8_t fcDecodeCol[16] = {0xF0,0,2,0xF0,1,0xF0,0xF0,0xF0,3,
		0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0
};

/// код нажатой кнопки
static eKEY keyPressed;

/**	Возвращает значение нажатой кнопки.
 *
 * 	@return eKEY Код нажатой кнопки.
 */
eKEY eKEYget(eGB_TYPE_DEVICE type) {
	eKEY key = keyPressed;

	keyPressed = KEY_NO;
	return key;
}

/**	Функция определения нажатой кнопки.
 *
 *	Вызывать надо раз в 10мс.
 *	Нажатие кнопки фиксируется только если предыдущее было обработано.
 *	Нажатие двух и более кнопок не инорируется.
 *	Повторное срабатывание кнопки TIME_DELAY_REPEAT * (период вызова ф-ии).
 *	Время антидребезга TIME_DELAY * (период вызова ф-ии).
 *
 * 	@param Нет
 * 	@return Нет
 */
void vKEYmain(void) {
	static uint_fast8_t delay = TIME_DELAY;	// счетчик антидребезга
	static uint_fast8_t keyPrev = KEY_NO;	// предыдущее значение кнопки
	uint_fast8_t tmp;						// временная переменная


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

	// определение кода нажатой кнопки
	tmp = fcDecodeRow[tmp & 0x0F] + fcDecodeCol[tmp >> 4];
	tmp = (tmp > sizeof(fcKey)) ? KEY_NO : fcKey[tmp];

	// Проверим на совпадение с предыдущим значением
	// Если не совпало, запустим счетчик антидребезга
	// Если кнопки не нажаты, антидребезг не считается
	// Если кнопка удерживается длительное время,
	// срабатывание определяется с периодом TIME_DELAY_REPEAT
	if (tmp != keyPrev) {
		keyPrev = tmp;
		delay = TIME_DELAY;
	} else {
		if (delay > 0) {
			delay--;
		} else {
			// Если предыдущее нажатие уже обработано, вернем новое нажатие
			// Нажатие одной кнопки функция - игнорируется
			if (keyPressed == KEY_NO)
				keyPressed = (eKEY) tmp;
			delay = TIME_DELAY_REPEAT;
		}
	}
}

