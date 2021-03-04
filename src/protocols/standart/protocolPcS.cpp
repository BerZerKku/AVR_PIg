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


bool clProtocolPcS::getData() {

	bool stat = false;
    eGB_COM com = static_cast<eGB_COM> (buf[2]);

    switch(static_cast<eGB_COM> (buf[2])) {
        case GB_COM_GET_PASSWORD: {
            uint16_t password = sParam_->password.get();
            buf[NUM] = 2;
            buf[B1] = static_cast<uint8_t> (password >> 8);
            buf[B2] = static_cast<uint8_t> (password);
            stat = (addCom() > 0);
        } break;

        case GB_COM_SET_PASSWORD: {
            if (buf[NUM] == 2) {
                // Если в команде 2 байта, то
                // сформируем команду установки нового значения пароля.
                // Иначе завернем команду обратно.
                uint8_t bytehi = buf[B1];
                uint8_t bytelo = buf[B2];

                buf[COM] = GB_COM_SET_NET_ADR;
                buf[NUM] = 3;
                buf[B1] = POS_COM_NET_ADR_password;
                buf[B2] = bytelo;
                buf[B3] = bytehi;
                buf[B4] = getCRC();
            } else {
                stat = (addCom() > 0);
            }
        } break;

        default: {

        }
    }

	if (com == GB_COM_GET_PASSWORD)	{

	} else if (com == GB_COM_SET_PASSWORD) {

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
