/*
 * ks0108.h
 *
 *  Created on: 06.04.2012
 *      Author: Хозяин
 */

#ifndef KS0108_H_
#define KS0108_H_

#include <avr/io.h>

/// Управление подсветкой
enum eLCD_LED
{
	/// Выключение подсветки
	LED_OFF,
	/// Включение подсветки
	LED_ON,
	/// Переключить состояние подсветки
	LED_SWITCH
};

/// Время работы кратковременной подсветки * 10мс
#define LCD_TIME_LED_ON 6000

#define PORT_RST 	PORTC
#define PIN_RST 	(1 << 5)
#define PORT_CS 	PORTC
#define PIN_CS2 	(1 << 6)
#define PIN_CS1 	(1 << 7)
#define PIN_CS  	(PIN_CS1 | PIN_CS2)

#define PORT_E 		PORTF
#define PIN_E   	(1 << 3)
#define PORT_RW 	PORTF
#define PIN_RW  	(1 << 2)
#define PORT_RS 	PORTF
#define PIN_RS  	(1 << 1)

// Управление подсветкой, 0 - выкл
#define PORT_LED	PORTD
#define PIN_LED		(1 << PD5)

bool vLCDcheckBusy	(void);
void vLCDclear		(void);
void vLCDcom		(uint8_t com, uint8_t cs);
void vLCDdata		(uint8_t data, uint8_t cs);
void vLCDinit		(void);
void vLCDmain		(void);
void vLCDrefresh	(void);
void vLCDsetXY		(uint8_t x, uint8_t y);

void vLCDputchar	(const char* buf);

void vLCDsetLED		(eLCD_LED val);

#endif /* KS0108_H_ */
