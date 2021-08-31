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
 * @brief ������������ �������� ���������.
 *
 * @return ��������� ���������.
 * @return true ������� ����������, ���� �������� ��.
 * @return false ������� �� ����������, ���� ��������� � ���.
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
 * @brief �������� ������� ��� ��������� �� �� ��.
 *
 * ���������� �������:
 * - 0x3F, ������� ������ ������ ����������
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
 * @brief ������������ �������� ������� ������ ������.
 * @return true ���� ������� ����������, ����� false.
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
 * @brief ������������ �������� ������� ������ ������.
 * @return true ���� ������� ����������, ����� false.
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

    addCom();  // ���
    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ������������ �������� ������� ������ ��������� "����� ��� (SAC1)"
 * @note ������������ �������� ��������.
 * @return true ���� ������� ����������, ����� false.
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
 * @brief ������������ �������� ������� ������ ��������� "����� ��� (SAC1)"
 * @note ������������ �������� ��������.
 * @return true ���� ������� ����������, ����� false.
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
 * @brief ������������ �������� ������� ������ ��������� ����� ������������.
 * @note ��� ������ ������ ������������ ������������ �������� ��������.
 * @note ��� ��������� ���������� ���.
 * @return true ���� ������� ����������, ����� false.
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
 * @brief ������������ �������� ������� ������ ���������� ����.
 * @note ��� "���������� (SAC2)" � SA.m ������������ ������������� �������.
 * @note ��� ��������� ���������� ���.
 * @return true ���� ������� ����������, ����� false.
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
        addCom();  // ���
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������� ������ ���������� �����������.
 *
 * ��������� �������� ����� ������ ������������ � ������������� ���������
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
 * @brief �������� ������� ������ ���������� ��������� ����
 *
 * ��������� �������� ����� ������ ������������ � ������������� ���������
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
 * @brief �������� ������� ������ ������ ���������� ��� ��������� �� �� ��.
 *
 * ����������� ������ �������� �� ��.
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
