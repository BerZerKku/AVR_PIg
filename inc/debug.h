/*
 * debug.h
 *
 *  Created on: 27.04.2012
 *      Author: ’оз€ин
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG

#warning "¬ключен режим отладки!!!"

// вывод отладочной информации на экран
// #define VIEW_DEBUG_PARAM

// тестовые пины
#define DDR_DBG		DDRE
#define PORT_DBG	PORTE
#define PIN_TP		(1 << 7)

#define SET_TP PORT_DBG |= PIN_TP
#define CLR_TP PORT_DBG &= ~PIN_TP
#define TOG_TP PORT_DBG ^= PIN_TP

// отладочна€ информаци€
struct stDebug
{
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
};

extern stDebug sDebug;

#endif

#endif /* DEBUG_H_ */
