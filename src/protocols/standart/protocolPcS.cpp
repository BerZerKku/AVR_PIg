/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
clProtocolS(buf, size, sParam) {

}


bool clProtocolPcS::getData()
{

	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, выставим флаг на чтение
	this->stat_ = PRTS_STATUS_READ;

	if (com == GB_COM_GET_PASSWORD)
	{
		uint16_t tmp = sParam_->password.get();
		buf[3] = 2;
		buf[4] = tmp >> 8;
		buf[5] = tmp;
		addCom();
		stat = true;
	}
	else if (com == GB_COM_SET_PASSWORD)
	{
		if (buf[3] == 2)
		{
			uint16_t tmp = ((uint16_t) buf[4] << 8) + buf[5];
			sParam_->password.set(tmp);
		}
		addCom(); // эхо
		stat = true;
	}

	return stat;
}

bool clProtocolPcS::modifyVersionCom() {
	bool state = false;

	if (getCurrentCom() == GB_COM_GET_VERS) {
		uint8_t crc = buf[maxLen_ - 1];
		uint8_t len = buf[3];
		if ( len < 19) {
			for(uint8_t i = len + 4; len < 19; i++, len++) {
				buf[i] = 0x00;
			}
			crc += 19 - buf[3];
			buf[3] = len;
			maxLen_ = len + 5;
		} else {
			crc -= buf[B18];
			crc -= buf[B19];
		}
		uint16_t vers = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
		crc += (buf[B18] = (vers >> 8));
		crc += (buf[B19] = (vers & 0xFF));
		buf[maxLen_ - 1] = crc;
	}

	return state;
}
