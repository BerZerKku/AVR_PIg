/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS {
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam);
	/// ��������� ��������� ���������. � ������ ������� ���������� False.
    bool getData(bool pc);

	/// �������� �������
	uint8_t sendData(eGB_COM com);

private:
	void getDefCommand(eGB_COM com, bool pc);
	void getPrmCommand(eGB_COM com, bool pc);
	void getPrdCommand(eGB_COM com, bool pc);
	void getGlbCommand(eGB_COM com, bool pc);

    void hdlrComGetComPrdKeep();
    void hdlrComGetDeviceNum();
    void hdlrComGetFault();
    void hdlrComGetMeas();
    void hdlrComGetNetAdr();
    void hdlrComSetNetAdr();
    void hdlrComGetSost();
    void hdlrComGetTime();
    void hdlrComGetVers();


    /** ������������� ���� � ����� ��� ������ �������.
     *
     *  !!! ���� ��� ��������� � ������ (������ � ������, ������������):
     *  - ��� ��� ������;
     *  - ������������ ���� ������� ������ ������.
     *
     *
     *  @param[in] pos ��������� ������� � ������.
     *  @param[in] yearfirst true ���� ��� ���� ������, ����� false.
     *  @return ������� � ������ ����� ���������.
     */
    uint8_t setJrnDateTime(uint8_t pos, bool yearfirst);

    /** ������������� ������ �������.
     *
     *  @param[in] device ���������� ��� �������� �������� ������.
     *  @param[in] pc ���� ������� � ��.
     */
    void setJrnEntry(eGB_DEVICE device, bool pc);

    /// ������������� ������ ��� ������� ������.
    void setJrnEntryDef();
    /// ������������� ������ ��� ������� �������.
    void setJrnEntryGlb();
    /// ������������� ������ ��� ������� �����������.
    void setJrnEntryPrd();
    /// ������������� ������ ��� ������� ���������.
    void setJrnEntryPrm();
    /// ������������� ������ ��� ������� ������������.
    void setJrnEntrySec();

    /** ������������ ������ ������� ��� ����������� ��������.
     *
     *  @param[in] device ���������� ��� �������� �������� ������.
     *  @param[in] pc ���� ������� � ��.
     */
    void setJrnEntryOpto(eGB_DEVICE device , bool pc);

    /** ������������� ������ �������.
     *
     *  @param[in] device ���������� ��� �������� ������� ������� �������.
     */
    void setJrnSize(eGB_DEVICE device);

    /** ���������� ���������� ��� ������ GB_COM_GET_NET_ADR � SET.
     *
     *  @param pos ����� ���������.
     *  @param buf ��������� ����� ������.
     *  @param len ���������� ���������� ���� � ������.
     *  @return ���������� ��������� ���� ������.
     *  @retval len ���� � ������ �� ���������� ���� ��� ���������.
     */
    uint8_t getComNetAdr(posComNetAdr_t pos, const uint8_t *buf, uint8_t len);

//	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);
};


#endif /* PROTOCOLBSPS_H_ */
