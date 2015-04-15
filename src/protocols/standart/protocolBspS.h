/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS
{
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam); // �����������

	bool 	getData();							///< ��������� ��������� ���������
	uint8_t sendData(eGB_COM com);				///< �������� �������

private:
	bool getRpsCommand(eGB_COM com);			///< ��������� ������ ���������
	bool getGlbCommand(eGB_COM com);			///< ��������� ������ �����

	uint8_t sendModifRpsCommand(eGB_COM com);	///< �������� ������ ���������
	uint8_t sendModifGlbCommand(eGB_COM com);	///< �������� ������ �����
	uint8_t sendReadJrnCommand(eGB_COM com);	///< �������� ������ ��������
};


#endif /* PROTOCOLBSPS_H_ */
