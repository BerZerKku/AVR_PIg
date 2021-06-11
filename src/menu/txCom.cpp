/*
 * TTxCom1.cpp
 *
 *  Created on: 11 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include <txCom.h>

//
TTxCom::TTxCom()
{
    clear();
    clearFastCom();
}

// очистка буфера
void TTxCom::clear()
{
    numCom1_ = numCom2_ = 0;
    cnt1_ = cnt2_ = 0;
    com1_[0] = com2_[0] = GB_COM_NO;
}

//
void TTxCom::clearFastCom()
{
    cntComFast = 0;
    for(uint_fast8_t j = 0; j < MAX_NUM_FAST_COM; j++) {
        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[j] [i] = 0;
        }
    }
    sendType_ = GB_SEND_NO;
}

//
bool TTxCom::addCom1(eGB_COM com, uint8_t num)
{
    bool stat = false;

    if (num > 0) {
        if (numCom1_ == 0)
            numCom1_ = 1;
        com1_[numCom1_ - 1] = com;
        stat = true;
    } else {
        if (numCom1_ < MAX_NUM_COM_BUF1) {
            com1_[numCom1_++] = com;
            stat = true;
        }
    }

    return stat;
}

//
eGB_COM TTxCom::getCom1()
{
    eGB_COM com = GB_COM_NO;

    if (numCom1_ > 0) {
        if (cnt1_ >= numCom1_) {
            cnt1_ = 0;
        }
        com = com1_[cnt1_++];
    }

    return com;
}

//
bool TTxCom::addCom2(eGB_COM com)
{
    bool stat = false;

    if (numCom2_ < MAX_NUM_COM_BUF2) {
        com2_[numCom2_++] = com;
        stat = true;
    }

    return stat;
}

//
eGB_COM TTxCom::getCom2()
{
    eGB_COM com = GB_COM_NO;

    if (numCom2_ > 0) {
        if (cnt2_ >= numCom2_) {
            cnt2_ = 0;
        } else {
            com = com2_[cnt2_++];
        }
    }

    return com;
}

//
void TTxCom::addFastCom(eGB_COM com)
{
    if (cntComFast < MAX_NUM_FAST_COM) {
        comFast_[cntComFast] = com;

        // сохранение данных для быстрой команды
        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[1 + cntComFast] [i] = buf_[0] [i];
            buf_[0] [i] = 0x00;
        }

        cntComFast++;
    }
}

//
eGB_COM TTxCom::getFastCom()
{
    eGB_COM com = GB_COM_NO;

    if (cntComFast > 0) {
        com = comFast_[cntComFast - 1];

        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[0] [i] = buf_[cntComFast] [i];
        }

        cntComFast--;
    }

    return com;
}
