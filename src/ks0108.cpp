
#include <math.h>
#include <util/delay.h>
#include <stdio.h>

#include "debug.h"
#include "ks0108.h"
#include "symbols.h"

/// буфер инф-ии выводимой на ЖКИ
static uint8_t uBuf[1024];
/// текущее положение в буфере ввода
static uint16_t uCnt = 0;
/// время до выключения подсветки * 100 мкс
static uint16_t uLedTimeOn = LCD_TIME_LED_ON;
/// состояние подсветки
static eLCD_LED eLed = LED_OFF;

/// обновление информации
static volatile bool bRefresh = false;


/**	Проверка флага занятости ЖКИ
 * 	@param Нет
 * 	@return false - занято.
 */
bool vLCDcheckBusy(void)
{
	uint8_t data = 0;

	PORT_RS &= ~PIN_RS;
	DDRA = 0;
	PORT_CS |= PIN_CS;
	PORT_RW |= PIN_RW;
	asm volatile (	"nop\n\t"
					"nop\n\t"
					::);
	PORT_E |= PIN_E;
	asm volatile (	"nop\n\t"
					"nop\n\t"
					::);
	data = PINA;
	PORT_E &= ~PIN_E;
	_delay_us(4);
	PORT_CS &= ~PIN_CS;
	PORT_E |= PIN_E;
	DDRA = 0xFF;

	if (data & 0x80)
		return false;
	else
		return true;
}

/** Запись команды в ЖКИ
 * 	@param com Команда
 * 	@param cs Выбор кристалла
 * 	@return Нет
 */
void vLCDcom(uint8_t com, uint8_t cs)
{
	PORT_CS |= cs;
	PORT_RS &= ~PIN_RS;
	PORT_RW &= ~PIN_RW;
	asm volatile (	"nop\n\t"
	        		"nop\n\t"
	        		::);
	PORTA = com;
	PORT_E |= PIN_E;
	asm volatile (	"nop\n\t"
	        		"nop\n\t"
	        		::);
	PORT_E &= ~PIN_E;
	_delay_us(1);
	PORT_CS &= ~cs;
}

/** Очистка дисплея
 * 	@param Нет
 * 	@return Нет
 */
void vLCDclear(void)
{
	for(uint_fast16_t i = 0; i < 1024; i++)
	{
		uBuf[i] = 0;
	}
}

/** Запись данных в ЖКИ
 * 	@param com Данные
 * 	@param cs Выбор кристалла
 * 	@return Нет
 */
void vLCDdata(uint8_t data, uint8_t cs)
{
	PORT_CS |= cs;
	PORT_RS |= PIN_RS;
	PORT_RW &= ~PIN_RW;
	asm volatile (	"nop\n\t"
		        	"nop\n\t"
		        	::);
	PORTA = data;
	PORT_E |= PIN_E;
	asm volatile (	"nop\n\t"
		        	"nop\n\t"
		        	::);
	PORT_E &= ~PIN_E;
	_delay_us(2);
	PORT_CS &= ~cs;
}

/** Запись данных в ЖКИ
 * 	@param com Данные
 * 	@param cs Выбор кристалла
 * 	@return Нет
 */
void vLCDwriteData(uint8_t data)
{
	uBuf[uCnt++] = data;
}

/** Инициализация дисплея
 * 	@param Нет
 * 	@return Нет
 */
void vLCDinit(void)
{
	if ( !(PORT_RST & PIN_RST) )
	{
		PORT_RST |= PIN_RST;
		_delay_ms(10);
	}
    vLCDsetXY(0, 0);
    _delay_us(2);
    vLCDcom(0xc0, PIN_CS);
    _delay_us(2);
    vLCDcom(0x3f, PIN_CS);
    _delay_us(5);
}

/**	Установка текущей координаты ЖКИ
 * 	@param x Номер столбца
 * 	@param y Номер строки
 * 	@param cs Выбор кристалла
 * 	@return Нет
 */
void vLCDsetXY(uint8_t x, uint8_t y)
{
	uCnt = (y * 128) + x - 1;
}

/** Старт обновления содержимого ЖКИ
 * 	@param Нет
 * 	@return Нет
 */
void vLCDrefresh(void)
{
	bRefresh = true;
}

/** Обновление ЖКИ
 * 	@param Нет
 * 	@return Нет
 */
void vLCDmain(void)
{
	static uint8_t x = 0;
	static uint8_t y = 0;
	// счетчик 100 циклов , т.е. период 10мс
	static uint8_t cnt = 100 - 1;

	// Работа с подсветкой
	if (eLed == LED_ON)
		PORT_LED ^= PIN_LED;
	else if (uLedTimeOn > 0)
	{
		PORT_LED ^= PIN_LED;
		if (cnt > 0)
			cnt--;
		else
		{
			cnt = 100 - 1;
			uLedTimeOn--;
		}
	}
	else
		PORT_LED &= ~PIN_LED;

	if (bRefresh)
	{
		if (x == 0)
		{
			vLCDcom(0xb8 + y, PIN_CS);
			_delay_us(5);
			vLCDcom(0x40, PIN_CS);
			_delay_us(5);
		}

		if (x < 64)
		{
			vLCDdata(uBuf[x + y * 128], PIN_CS1);
			vLCDdata(uBuf[x + 64 + y * 128], PIN_CS2);
			x++;
		}
		else if (y < 7)
		{
			y++;
			x = 0;
		}
		else
		{
			y = 0;
			x = 0;
			bRefresh = false;
		}
	}
}

/** Вывод строки на экран
 *	@param symb Символ
 *	@return Нет
 */
void vLCDputchar(const char* buf)
{
	uint_fast8_t tmp;
	uint_fast16_t cnt = 0;

	for(uint_fast8_t j = 0; j < 168; j++)
	{
		tmp = *buf;

		if (j % 21 == 0)
			uBuf[cnt++] = 0;

		if (tmp < 0x20)
			tmp = 0;
		else if (tmp < 0x80)
			tmp -= 0x20;
		else if (tmp >= 0xC0)
			tmp -= 0x60;
		else
			tmp = 0;

		for(uint_fast8_t i = 0; i < 5; i++)
		{
			uBuf[cnt++] = pgm_read_byte(&sym[tmp] [i]);
		}

		uBuf[cnt++] = 0;


		// в строке 21 символ
		// первый и последний столбец обнуляем
		if (j % 21 == 0)
			uBuf[cnt++] = 0;

		if (cnt >= 1024)
			cnt = 0;

		buf++;
	}
}

/**	Управление подсветкой ЖКИ
 * 	@param val Одно из значений:
 * 	@arg LED_ON Включить постоянную подсветку
 * 	@arg LED_SWITCH Включить временную подсветку
 * 	@arg LED_OFF Выключить подсветку
 * 	@return Нет
 */
void vLCDsetLED(eLCD_LED val)
{
	if (val == LED_SWITCH)
		uLedTimeOn = LCD_TIME_LED_ON;
	else
		eLed = val;
}
