/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"
#include "glbDefine.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
    clProtocolS(buf, size, sParam)
{
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятое сообщение.
 *
 * @return Результат обработки.
 * @return true Команда обработана, надо ответить ПК.
 * @return false Команда не обработана, надо переслать в БСП.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::getData()
{

    bool    stat = false;
    eGB_COM com  = static_cast<eGB_COM>(getCurrentCom());

    switch (com)
    {
    case GB_COM_GET_PASSWORD: stat = hdlrComGetPassword(com); break;
    case GB_COM_SET_PASSWORD: stat = hdlrComSetPassword(com); break;

    case GB_COM_PRM_GET_PRM_DISABLE: stat = hdlrComGetPrmDisable(com); break;
    case GB_COM_PRM_SET_PRM_DISABLE: stat = hdlrComGetPrmDisable(com); break;
    case GB_COM_SET_COM_PRD_KEEP: stat = hdlrComSetComPrdKeep(com); break;
    case GB_COM_SET_NET_ADR: stat = hdlrComSetNetAdr(com); break;
    }

    return stat;
}


/**
 * *****************************************************************************
 *
 * @brief Изменяет команду при пересылке ее на ПК.
 *
 * Изменяются команды:
 * - 0x3F, команда чтения версии устройства
 *
 *
 * *****************************************************************************
 */
void clProtocolPcS::modifyComForPc()
{
    switch (getCurrentCom())
    {
    case GB_COM_GET_COM_PRD_KEEP: modifyComGetComPrdKeep(); break;
    case GB_COM_GET_NET_ADR: modifyComGetNetAdr(); break;
    case GB_COM_GET_VERS: modifyComGetVersion(); break;
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду чтения пароля.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComGetPassword(eGB_COM com)
{
    uint16_t password = sParam_->password.get();

    addCom(com, password >> 8, password);
    return true;
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду записи пароля.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComSetPassword(eGB_COM com)
{
    if (buf[3] == 2)
    {
        uint16_t password = TO_INT16(buf[4], buf[5]);
        sParam_->password.set(password);
    }

    addCom();  // эхо
    return true;
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду чтения параметра "Вывод ПРМ (SAC1)"
 * @note Возвращается заданное значение.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComGetPrmDisable(eGB_COM com)
{
    addCom(com, kVpSac1);
    return true;
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду записи параметра "Вывод ПРМ (SAC1)"
 * @note Возвращается заданное значение.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComSetPrmDisable(eGB_COM com)
{
    addCom(com, kVpSac1);
    return true;
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду записи параметра сброс сигнализации.
 * @note Для режима сброса сигнализации возвращается заданное значение.
 * @note Для остальных параметров эхо.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComSetComPrdKeep(eGB_COM com)
{
    bool state = false;

    if (buf[B2] == 23)
    {
        addCom(com, kAlarmResetMode, buf[B2]);
        state = true;
    }

    return state;
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает принятую команду записи параметров сети.
 * @note Для "Управление (SAC2)" и SA.m возвращаются фиксированное значние.
 * @note Для остальных параметров эхо.
 * @return true если команда обработана, иначе false.
 *
 * *****************************************************************************
 */
bool clProtocolPcS::hdlrComSetNetAdr(eGB_COM com)
{
    if (buf[B1] == 9)
    {
        addCom(com, buf[B1], kVpSac2);
    }
    else if (buf[B1] == 10)
    {
        addCom(com, buf[B1], kVpSam);
    }
    else
    {
        addCom();  // эхо
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief Изменяет команду чтения параметров передатчика.
 *
 * Добавляет параметр режим сброса сигнализации с фиксированным значением
 *
 * *****************************************************************************
 */
void clProtocolPcS::modifyComGetComPrdKeep()
{
    for (uint8_t i = buf[NUM]; i < 23; i++)
    {
        buf[B1 + i] = 0;
    }

    buf[NUM] = B23 - B1 + 1;
    buf[B23] = kAlarmResetMode;
    addCom();
}


/**
 * *****************************************************************************
 *
 * @brief Изменяет команду чтения параметров локальной сети
 *
 * Добавляет параметр режим сброса сигнализации с фиксированным значением
 *
 * *****************************************************************************
 */
void clProtocolPcS::modifyComGetNetAdr()
{
    for (uint8_t i = buf[NUM]; i < 10; i++)
    {
        buf[B1 + i] = 0;
    }

    buf[NUM] = B10 - B1 + 1;
    buf[B9]  = kVpSac2;
    buf[B10] = kVpSam;
    addCom();
}


/**
 * *****************************************************************************
 *
 * @brief Изменяет команду чтения версии устройства при пересылке ее на ПК.
 *
 * Добавляется версия прошивки МК ПИ.
 *
 * *****************************************************************************
 */
void clProtocolPcS::modifyComGetVersion()
{
    if (getCurrentCom() == GB_COM_GET_VERS)
    {
        uint8_t crc = buf[maxLen_ - 1];
        uint8_t len = buf[3];
        if (len < 19)
        {
            for (uint8_t i = len + 4; len < 19; i++, len++)
            {
                buf[i] = 0x00;
            }
            crc += 19 - buf[3];
            buf[3]  = len;
            maxLen_ = len + 5;
        }
        else
        {
            crc -= buf[B18];
            crc -= buf[B19];
        }
        uint16_t vers = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
        crc += (buf[B18] = (vers >> 8));
        crc += (buf[B19] = (vers & 0xFF));
        buf[maxLen_ - 1] = crc;
    }
}
