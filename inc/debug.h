/*
 * debug.h
 *
 *  Created on: 27.04.2012
 *      Author: ������
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG

#warning "������� ����� �������!!!"

#define DDR_DBG		DDRE
#define PORT_DBG	PORTE
#define PIN_TP1		(1 << 6)
#define PIN_TP2		(1 << 7)

#endif

#endif /* DEBUG_H_ */
