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
	eGB_COM com = (eGB_COM) buf[2];

    if (com == GB_COM_GET_USER)	{
        stat = hdlrComGetUser(com);
    } else if (com == GB_COM_SET_USER) {
        stat = hdlrComSetUser(com);
	}

	return stat;
}

//
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

//
bool clProtocolPcS::hdlrComGetUser(eGB_COM com) {
    uint8_t len = 0;

    if (buf[NUM] == 0) {
        len = addCom(com, sParam_->security.UserPc.get());
    } else if (buf[NUM] == 1) {
        uint8_t array[4];
        TUser::user_t user = static_cast<TUser::user_t> (buf[B1]);
        array[0] = user;
        array[1] = sParam_->security.pwd.isLocked(user);
        *((uint16_t *) &array[2]) = sParam_->security.pwd.getLockTime(user);
        len = addCom(com, SIZE_OF(array), array);
    }

    return len > 0;
}

//
bool clProtocolPcS::hdlrComSetUser(eGB_COM com) {
    uint8_t len = 0;
    enum state_t {
        STATE_OK = 0x00,            // ОК
        STATE_NO_ACCESS = 0x01,     // Нет доступа
        STATE_WRONG_PWD = 0x02,     // Неверный пароль
        STATE_WRONG_NEW_PWD = 0x03  // Неверный новый пароль
    };

    state_t state = STATE_OK;
    TUser::user_t user = static_cast<TUser::user_t> (buf[B1]);

    switch(buf[NUM]) {
        case 1: {
            sParam_->security.UserPc.set(TUser::OPERATOR);
            len = addCom(com, sParam_->security.UserPc.get());
        } break;
        case (1 + PWD_LEN): {
            if (sParam_->security.pwd.checkPassword(user, &buf[B2])) {
                sParam_->security.UserPc.set(user);
            } else {
                state = STATE_WRONG_PWD;
            }
            len = addCom(com, user, state);
        } break;
        case (1 + 2*PWD_LEN): {
            TUser::user_t curuser = sParam_->security.UserPc.get();
            if (sParam_->security.pwd.checkPassword(curuser, &buf[B2])) {
                if (!sParam_->security.pwd.setPwd(user, &buf[B10])) {
                    state = STATE_WRONG_NEW_PWD;
                }
            } else {
                state =  STATE_WRONG_PWD;
            }
            len = addCom(com, user, state);
        } break;
    }

    return len > 0;
}
