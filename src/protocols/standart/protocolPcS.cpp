/*
 *
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"
#include "securityevent.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
                                                                              clProtocolS(buf, size, sParam) {

}

bool clProtocolPcS::getData() {
    bool send = false;
    TInfoSecurity::state_t state;
    eGB_COM com = (eGB_COM) buf[2];

    if (sParam_->connectionBsp) {
        state = sParam_->security.isComAccess(com);
        if (state == TInfoSecurity::STATE_OK) {
            if (com == GB_COM_GET_USER)	{
                send = hdlrComGetUser(com);
            } else if (com == GB_COM_SET_USER) {
                send = hdlrComSetUser(com);
            }
        } else {
            addCom(GB_COM_ERROR, ERROR_noAccess);
            send = true;
        }
    } else {
        addCom(GB_COM_ERROR, ERROR_noConnectionBsp);
        send = true;
    }


    return send;
}

//
bool clProtocolPcS::modifyVersionCom() {
    bool state = false;
    eGB_COM com = static_cast<eGB_COM> (getCurrentCom());

    if (com == GB_COM_GET_VERS) {
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
        len = addCom(com, sParam_->security.getUser(USER_SOURCE_pc));
    } else if (buf[NUM] == 1) {
        uint8_t array[4];
        user_t user = static_cast<user_t> (buf[B1]);
        uint16_t sec = sParam_->security.pwd.getLockTime(user);
        array[0] = user;
        array[1] = sParam_->security.pwd.isLocked(user);
        array[2] = static_cast<uint8_t> (sec >> 8);
        array[3] = static_cast<uint8_t> (sec);
        len = addCom(com, SIZE_OF(array), array);
    }

    return len > 0;
}

//
bool clProtocolPcS::hdlrComSetUser(eGB_COM com) {
    uint8_t len = 0;
    user_t user = static_cast<user_t> (buf[B1]);
    TInfoSecurity::state_t state = TInfoSecurity::STATE_MAX;

    switch(buf[NUM]) {
        case 1: {
            state = sParam_->security.setUserPc(USER_operator);
        } break;
        case (1 + PWD_LEN): {
            state = sParam_->security.setUserPc(user, &buf[B2]);
        } break;
        case (1 + 2*PWD_LEN): {
            state = sParam_->security.changeUserPcPwd(user, &buf[B2], &buf[B10]);
        } break;
    }

    if (state < TInfoSecurity::STATE_MAX) {
        len = addCom(com, user, state);
    }

    return len > 0;
}
