/*
 * debug.hpp
 *
 *  Created on: 27.04.2012
 *      Author: ������
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <stdint.h>

#if defined(AVR)

#include <avr/io.h>

#warning "������� ����� �������!!!"

// ����� ���������� ���������� �� �����
//#define VIEW_DEBUG_PARAM
//#define IEC101

// �������� ����
#define DDR_DBG		DDRE
#define PORT_DBG	PORTE
#define PIN_TP1		(1 << 6)
#define PIN_TP2		(1 << 7)

#define SET_TP1 PORT_DBG |= PIN_TP1
#define CLR_TP1 PORT_DBG &= ~PIN_TP1
#define TOG_TP1 PORT_DBG ^= PIN_TP1

#define SET_TP2 PORT_DBG |= PIN_TP2
#define CLR_TP2 PORT_DBG &= ~PIN_TP2
#define TOG_TP2 PORT_DBG ^= PIN_TP2

#endif

//
#if defined(NDEBUG)
#define COMPILE_TIME_ASSERT(expression) (void(0))
#else

// ���������� ����������
struct stDebug {
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

#define COMPILE_TIME_ASSERT(expression) switch(0) { case 0 : case (expression) : ; }
#endif

#endif /* DEBUG_HPP_ */
