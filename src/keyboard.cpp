/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include <avr/io.h>
#include "../inc/keyboard.h"
#include "../inc/debug.h"

/// код нажатой кнопки
static eKEY keyPressed;

/**	Возвращает значение нажатой кнопки
 * 	@param Нет
 * 	@return eKEY Код нажатой кнопки
 */
eKEY
eKEYget(void)
{
	eKEY key_tmp = keyPressed;

	keyPressed = KEY_NO;
	return key_tmp;
}


/**	Функция определения нажатой кнопки
 *	Вызывать надо раз в 10мс
 *	Нажатие одной кнопки "Функция" игнорируется
 *	Повторное срабатывание кнопки TIME_DELAY_REPEAT * (период вызова ф-ии)
 *	Время антидребезга TIME_DELAY * (период вызова ф-ии)
 * 	@param Нет
 * 	@return Нет
 */
void
vKEYmain(void)
{
	static uint_fast8_t delay = TIME_DELAY;	// счетчик антидребезга
	static uint_fast8_t keyPrev = KEY_NO;	// предыдущее значение кнопки
	uint_fast8_t tmp;						// временная переменная
	uint_fast8_t tmpKey = KEY_NO;			// временная переменная

	tmp = (~PIND) & 0xC0;
	// сначала проверим нажатие кнопки "Функция"
	if (tmp & 0x80)
		tmpKey = KEY_FUNC;

	// сканируем клавиатуру, до определения первой нажатой кнопки
	if (tmp & 0x40)
		tmpKey |= KEY_CANCEL;
	else
	{
		tmp = (~PING) & 0x03;
		if (tmp)
		{
			if (tmp & 0x01)
				tmpKey |= KEY_LEFT;
			else if (tmp & 0x02)
				tmpKey |= KEY_UP;
		}
		else
		{
			tmp = ~(PINC) & 0x1F;
			if (tmp)
			{
				if (tmp & 0x01)
					tmpKey |= KEY_RESET_IND;
				else if (tmp & 0x02)
					tmpKey |= KEY_ENTER;
				else if (tmp & 0x04)
					tmpKey |= KEY_RIGHT;
				else if (tmp & 0x08)
					tmpKey |= KEY_DOWN;
				else if (tmp & 0x10)
					tmpKey |= KEY_RESET;
			}
		}
	}

	// Проверим на совпадение с предыдущим значением
	// Если не совпало, запустим счетчик антидребезга
	// Если кнопки не нажаты, антидребезг не считается
	// Если кнопка удерживается длительное время,
	// срабатывание определяется с периодом TIME_DELAY_REPEAT
	if (tmpKey != keyPrev)
	{
		keyPrev = tmpKey;
		tmpKey = KEY_NO;
		delay = TIME_DELAY;
	}
	else
	{
		if (delay > 0)
		{
			if (tmpKey != KEY_NO)
			{
				tmpKey = KEY_NO;
				delay--;
			}
		}
		else
		{
			delay = TIME_DELAY_REPEAT;
		}
	}

	// Если предыдущее нажатие уже обработано, вернем новое нажатие
	// Нажатие одной кнопки функция - игнорируется
	if (keyPressed == KEY_NO)
		keyPressed = (eKEY) tmpKey;
}







