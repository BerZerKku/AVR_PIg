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

#define DDR_DBG		DDRE
#define PORT_DBG	PORTE
#define PIN_TP1		(1 << 6)
#define PIN_TP2		(1 << 7)

#define SET_TP1 PORTE |= PIN_TP1
#define CLR_TP1 PORTE &= ~PIN_TP1
#define TOG_TP1 PORTE ^= PIN_TP1

#define SET_TP2 PORTE |= PIN_TP2
#define CLR_TP2 PORTE &= ~PIN_TP2
#define TOG_TP2 PORTE ^= PIN_TP2

struct stDebug
{
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
};

extern stDebug sDebug;

#endif

#endif /* DEBUG_H_ */
