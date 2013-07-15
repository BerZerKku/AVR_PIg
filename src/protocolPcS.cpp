/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolPcS.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
												clProtocolS(buf, size, sParam)
{

}


bool clProtocolPcS::getData()
{

	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, сброс флага
	this->stat = PRTS_STATUS_NO;

	if (com == GB_COM_GET_PASSWORD)
	{
		buf[3] = 2;
		buf[4] = sParam->password >> 8;
		buf[5] = sParam->password;
		addCom();
		stat = true;
	}
	else if (com == GB_COM_SET_PASSWORD)
	{
		if (buf[3] == 2)
		{
			uint16_t tmp = ((uint16_t) buf[4] << 8) + buf[5];
			sParam->setPassword(tmp);
		}
		addCom(); // эхо
		stat = true;
	}

	return stat;
}
