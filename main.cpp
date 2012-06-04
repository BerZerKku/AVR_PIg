/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <math.h>
//#include <util/delay.h>

#include "debug.h"
#include "setup.h"
#include "ks0108.h"
#include "menu.h"
#include "keyboard.h"
#include "uart.h"


/// период обновления экрана * 100 мс
#define LCD_REFRESH_PERIOD 2

/// флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;


clUart uartPC(UART1);

/**	main.c
 * 	@param Нет
 * 	@return Нет
 */
int __attribute__ ((OS_main)) main (void)
{
	// счетчик 1с
	uint_fast8_t cnt_lcd = 0;

	vSETUP();

	sei();

	vLCDinit();
	vLCDclear();
	uartPC.init(19200);
	uartPC.trByte(0x1);

	while(1)
	{

		if (b100ms)
		{
			// задачи выполняемые раз в 100мс

			// задачи выполняемые раз в 1с

			// обновление экрана
			// выполняется с периодом LCD_REFRESH_PERIOD * 100мс
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				vMNUmain();
				cnt_lcd = 0;
			}

			b100ms = false;
		}
	}
}

/**	Прерывание по совпадению Таймер0
 * 	Срабатывает раз в 100 мкс
 * 	@param Нет
 * 	@return Нет
 */
ISR(TIMER0_COMP_vect)
{
	static uint_fast8_t cnt = 0;

	// Обработчик клавиатуры вызываем раз в 10мс
	if (cnt >  0)
		cnt--;
	else
	{
		cnt = 100 - 1;
		vKEYmain();
	}

	// Обработчик ЖКИ вызываем раз в 100мкс
	vLCDmain();
}

/**	Прерывание по совпадению А Таймер1
 * 	Срабатывает раз в 100 мс
 * 	@param Нет
 * 	@return Нет
 */
ISR(TIMER1_COMPA_vect)
{
	b100ms = true;
}

/**	Прерывание по опустошению передающего буфера UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_UDRE_vect)
{
	// *uartPC.udr = 0x55;
	// UDR1 = 0x55;
	*uartPC.udr = 33;

}

/** Прерывание по окончанию передачи данных UART1
 * 	Запретим прерывание по опустошению буфера
 * 	Разрешим прерывание по приему.
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_TX_vect)
{
	//UCSR1B &= ~(1 << UDRIE1);
	//UCSR1B |= (1 << RXCIE1);
}

