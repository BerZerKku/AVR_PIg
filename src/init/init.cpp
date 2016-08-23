/*
 * setup.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include "debug.h"

void
low_level_init() __attribute__((__naked__)) __attribute__((section(".init3")));

/**	Инициализация периферии
 * 	Неиспользуемые порты по умолчанию настроены на вход с подтяжкой к +
 * 	@param Нет
 * 	@return Нет
 */
void
low_level_init()
{
	// Включение подтяжки
	SFIOR &= ~(1 << PUD);

	// Порт А
	// Двунаправленная шина
	DDRA = 0xFF;
	PORTA= 0x00;

	// Порт B
	// PORTB.0 = 		- вход +
	// PORTB.1 = 		- вход +
	// PORTB.2 = 		- вход +
	// PORTB.3 = 		- вход +
	// PORTB.4 = 		- вход +
	// PORTB.5 = E_STR	- выход +
	// PORTB.6 = RS		- выход +
	// PORTB.7 = 		- вход +
	DDRB = (1 << PB5) | (1 << PB6);
	PORTB = 0xFF;

	// Порт С
	// PORTC.0 = ROW1	- вход
	// PORTC.1 = ROW2	- вход
	// PORTC.2 = ROW3	- вход
	// PORTC.3 = ROW4	- вход
	// PORTC.4 = COL1	- вход
	// PORTC.5 = COL2	- вход
	// PORTC.6 = COL3	- вход
	// PORTC.7 = COl4	- вход
	DDRC = 0;
	PORTC= 0;

	// Порт D
	// PORTD.0 = 		- вход +.
	// PORTD.1 = 	 	- вход +
	// PORTD.2 = RXD1	- альтер.
	// PORTD.3 = TXD1	- альтер.
	// PORTD.4 = MUX	- выход 0 (переключение интерфейсов 0-USB, 1-485)
	// PORTD.5 = 		- вход +
	// PORTD.6 = 		- вход +
	// PORTD.7 = 		- вход +
	DDRD = (1 << DDD4);
	PORTD= ~(1 << PD4);

	// Порт Е
	// PORTE.0 = RXD0	- альтер.
	// PORTE.1 = TXD0	- альтер.
	// PORTE.2 = 		- вход +
	// PORTE.3 = 		- вход +
	// PORTE.4 = 		- вход +
	// PORTE.5 = 		- вход +
	// PORTE.6 = 		- вход +
	// PORTE.7 = 		- вход +
	DDRE = 0x00;
	PORTE = 0xFF;

	// Порт F
	// PORTF.0 = 		- вход +
	// PORTF.1 = 		- вход +
	// PORTF.2 = 		- вход (проходит сигнал E_STR)
	// PORTF.3 = 		- вход (проходит сигнал RS)
	// PORTF.4 = 		- вход +
	// PORTF.5 = 		- вход +
	// PORTF.6 = 		- вход +
	// PORTF.7 = 		- вход +
	DDRF = 0x00;
	PORTF = 0xFC;

	// Порт G
	// PORTG.0 = 		- вход +
	// PORTG.1 = 		- вход +
	// PORTG.2 = 		- вход +
	// PORTG.3 = 		- вход +
	// PORTG.4 = 		- вход +
	DDRG = 0x00;
	PORTG= ~0x00;

#ifdef DEBUG
	DDR_DBG |= (PIN_TP);
#endif

	// Обнуление регистра
	TIMSK = 0;

	// Таймер 0
	// режим сброс по совпадению
	// делитель N = 32, счет до OCR = 24
	// частота = F_CPU / (2 * N * (1 + OCR)
	// при F_CPU = 16МГц получим 10000Гц
	TCCR0 = (1 << WGM01) | (0 << WGM00) |
			(0 << CS02) | (1 << CS01) | (1 << CS00);
	OCR0  = 24;
	TIMSK |= (1 << OCIE0);

	// Таймер 1
	// режим сброс по совпадению
	// делитель N = 256, счет до OCR = 624
	// частота = F_CPU / (2 * N * (1 + OCR)
	// при F_CPU = 16МГц получим 50Гц
	TCCR1A 	= (0 << WGM11) | (0 << WGM10);
	TCCR1B 	= (0 << WGM13) | (1 << WGM12) |
				(1 << CS12) | (0 << CS11) | (0 << CS10);
	TCCR1C 	= 0;
	TCNT1 	= 0;
	OCR1A 	= 624;
	TIMSK 	|= (1 << OCIE1A);

	wdt_enable(WDTO_250MS);
}



