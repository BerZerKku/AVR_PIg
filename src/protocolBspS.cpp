/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
												clProtocolS(buf, size, sParam)
{

}


/**	Обработка принятого сообщения.
 * 	@param Нет
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getData()
{
	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, сброс флага
	this->stat = PRTS_STATUS_NO;

	if (((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_PARAM) ||
		((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_REGIME) )
		{

		}

	if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_DEF)
	{

	}
	else if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_PRM)
	{

	}
	else if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_PRD)
	{

	}
	else
	{
		if (com == GB_COM_GET_TIME)
		{

		}
	}

	return stat;
}
