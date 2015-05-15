/*
 * protocolPcI.cpp
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#include "protocolPcI.h"

// Конструктор
TProtocolPcI::TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), CIec101(buf, size) {
	// NONE
}

//	Функция отправки сообщения.
uint8_t TProtocolPcI::send() {

	return sendData();
}

//bool TProtocolPcI::procInterrog(void) {
//
//	return true;
//
//}
