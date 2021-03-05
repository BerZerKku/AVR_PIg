/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCS_H_
#define PROTOCOLPCS_H_

#include "protocolS.h"

class clProtocolPcS : public clProtocolS
{
    eGB_COM lastCom = GB_COM_NO;

public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/// ��������� ��������� ���������. � ������ ������� ���������� False.
    bool getData() override;

    /**	��������� ������� ��� �������� �� �� ��.
     *
     *  - GB_COM_GET_VERS
     *      � ������� ����������� ������ �������� ���-��.
     *      ���� ���-�� ���� ������ ������, ��� ����������, �.�. ��� �������
     *      ��� ��������, ���������� ���������� ���-�� ���� ������ ��
     *      ������������. ��� ����� ����� ������, �� ����������� �����
     *      ��������, ����� 0�00.
     *  - GB_COM_SET_NET_ADR
     *      � ������ ���� "������������" ������� ��������� � �� ����
     *      GB_COM_SET_PASSWORD � � ���� ������� ��������� ������,
     *      �� ������� ����� ������������� � ����� �� �������
     *      B_COM_SET_PASSWORD.
	 *
     *	@return true ���� ������� ���� ��������, ����� false.
	 */
    bool modifyCommand();

};


#endif /* PROTOCOLPC_H_ */
