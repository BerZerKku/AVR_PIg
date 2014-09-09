/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include <avr/io.h>
#include "../inc/keyboard.h"
#include "../inc/debug.h"

/** Коды кнопок.
 *	Например: 1R1C - Номер строки (1..4) и стоблца (1..4).
 */
enum eBUT
{
	BUT_NO = 0x00,
	BUT_1R1C,
	BUT_1R2C,
	BUT_1R3C,
	BUT_1R4C,
	BUT_2R1C,
	BUT_2R2C,
	BUT_2R3C,
	BUT_RR4C,
	BUT_3R1C,
	BUT_3R2C,
	BUT_3R3C,
	BUT_3R4C,
	BUT_4R1C,
	BUT_4R2C,
	BUT_4R3C,
	BUT_4R4C,
	BUT_ERR = 0xF0
};

/// Массив кнопок
static const eKEY fcKey[] = {
		KEY_1, 		KEY_2,		KEY_3,		KEY_ENTER,
		KEY_4,		KEY_5,		KEY_6,		KEY_UP,
		KEY_7,		KEY_8,		KEY_9,		KEY_DOWN,
		KEY_STAR,	KEY_0,		KEY_SHARP,	KEY_CANCEL
};

// массив декодирования строки
static const uint8_t fcDecodeRow[9] = {
		0, 1, 5, BUT_ERR, 9, BUT_ERR, BUT_ERR, BUT_ERR, 13
};

// массив декодирования столбцы
static const uint8_t fcDecodeCol[9] = {
		0, 1, 2, BUT_ERR, 3, BUT_ERR, BUT_ERR, BUT_ERR, 4
};

/// код нажатой кнопки
static eBUT keyPressed;

/**	Возвращает значение нажатой кнопки
 * 	@param Нет
 * 	@return eKEY Код нажатой кнопки
 */
eKEY eKEYget(eGB_TYPE_DEVICE type) {
	uint_fast8_t but = keyPressed;
	eKEY key = KEY_NO;

	if ((but >= BUT_1R1C) && (but <= BUT_3R3C)) {
		but = 1 + (but - BUT_1R1C);
	} else if ((but >= BUT_F_1R1C) && (but <= BUT_F_3R3C)) {
		but = 10 + (but - BUT_F_1R1C);
	} else
		but = 0;

	if (but != 0) {
		but -= 1;
		if (type == AVANT_K400) {
			key = fcKeyK400[but];
		} else if (type == AVANT_R400M) {
			key = fcKeyR400M[but];
		} else if (type == AVANT_RZSK) {
			key = fcKeyRZSK[but];
		} else if (type == AVANT_OPTO) {
			key = fcKeyOPTO[but];
		}
	}

	keyPressed = BUT_NO;
	return key;
}

/**	Возвращает флаг длительного нажатия.
 * 	После опроса, флаг обнуляется.
 * 	@retval True - длительное нажатие кнопки.
 * 	@retval False - НЕ длительное нажатие кнопки
 *
 */
uint8_t timePressKey() {
	return (timePress / PRESS_TIME_STEP);
}

/**	Функция определения нажатой кнопки
 *	Вызывать надо раз в 10мс
 *	Нажатие одной кнопки "Функция" игнорируется
 *	Повторное срабатывание кнопки TIME_DELAY_REPEAT * (период вызова ф-ии)
 *	Время антидребезга TIME_DELAY * (период вызова ф-ии)
 * 	@param Нет
 * 	@return Нет
 */
void vKEYmain(void) {
	static uint_fast8_t delay = TIME_DELAY;	// счетчик антидребезга
	static uint_fast8_t keyPrev = BUT_NO;	// предыдущее значение кнопки
	uint_fast8_t tmp;						// временная переменная
	uint_fast8_t tmpKey;					// временная переменная


	for(uint8_t i = 0x01; i < 0x10; i <<= 1) {
		DDRC = i;
		_delay_us(1);
		tmp = ~PINC;
		if (PINC != i) {
			break;
		}
	}
	DDRC = 0;

	tmp = fcDecodeRow[tmp & 0x0F] + fcDecodeCol[tmp >> 4];
	if (tmp > 0) && (tmp < 0x) {

	}

	switch(tmp){
		case 0x81: tmp = 'C'; break;
		case 0x21: tmp = '#'; break;
		case 0x41: tmp = '0'; break;
		case 0x11: tmp = 'M'; break;
		case 0x82: tmp = 'D'; break;
		case 0x22: tmp = '9'; break;
		case 0x42: tmp = '8'; break;
		case 0x12: tmp = '7'; break;
		case 0x84: tmp = 'U'; break;
		case 0x24: tmp = '6'; break;
		case 0x44: tmp = '5'; break;
		case 0x14: tmp = '4'; break;
		case 0x88: tmp = 'E'; break;
		case 0x28: tmp = '3'; break;
		case 0x48: tmp = '2'; break;
		case 0x18: tmp = '1'; break;
		default: TempKey=0xF0;
	}

	// Проверим на совпадение с предыдущим значением
	// Если не совпало, запустим счетчик антидребезга
	// Если кнопки не нажаты, антидребезг не считается
	// Если кнопка удерживается длительное время,
	// срабатывание определяется с периодом TIME_DELAY_REPEAT
	if (tmpKey != keyPrev) {
		keyPrev = tmpKey;
		tmpKey = BUT_NO;
		delay = TIME_DELAY;
		timePress = 0;
	} else {
		if (delay > 0) {
			if (tmpKey != BUT_NO) {
				tmpKey = BUT_NO;
				delay--;
			}
		} else {
			delay = TIME_DELAY_REPEAT;
			if (timePress < PRESS_TIME_MAX) {
				timePress++;
			}
		}
	}

	// Если предыдущее нажатие уже обработано, вернем новое нажатие
	// Нажатие одной кнопки функция - игнорируется
	if (keyPressed == BUT_NO)
		keyPressed = (eBUT) tmpKey;
}

