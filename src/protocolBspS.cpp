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


/**	ќбработка прин€того сообщени€.
 * 	@param Ќет
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getData()
{
	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, сброс флага
	this->stat = PRTS_STATUS_NO;

	// если это ответ на команду изменени€ параметра или режима
	// то сразу выйдем
	if (((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_PARAM) ||
		((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_REGIME))
	{
		stat = true;
	}
	else
	{

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
				sParam->dataTime.setYearFromBCD(buf[4]);
				sParam->dataTime.setMonthFromBCD(buf[5]);
				sParam->dataTime.setDayFromBCD(buf[6]);
				sParam->dataTime.setHourFromBCD(buf[7]);
				sParam->dataTime.setMinuteFromBCD(buf[8]);
				sParam->dataTime.setSecondFromBCD(buf[9]);
			}
			else if (com == GB_COM_GET_SOST)
			{
				sParam->def.status.setRegime(buf[4]);
				sParam->def.status.setState(buf[5]);
				sParam->def.status.setDopByte(buf[6]);

				sParam->prm.status.setRegime(buf[7]);
				sParam->prm.status.setState(buf[8]);
				sParam->prm.status.setDopByte(buf[9]);

				sParam->prd.status.setRegime(buf[10]);
				sParam->prd.status.setState(buf[11]);
				sParam->prd.status.setDopByte(buf[12]);
			}
			else if (com == GB_COM_GET_FAULT)
			{
				sParam->def.status.setFault(((uint16_t)buf[4]<<8) + buf[5]);
				sParam->def.status.setWarning(((uint16_t)buf[6]<<8) + buf[7]);

				sParam->prm.status.setFault(((uint16_t)buf[8]<<8) + buf[9]);
				sParam->prm.status.setWarning(((uint16_t)buf[10]<<8) + buf[11]);

				sParam->prd.status.setFault(((uint16_t)buf[12]<<8) + buf[13]);
				sParam->prd.status.setWarning(((uint16_t)buf[14]<<8) + buf[15]);

				sParam->glb.status.setFault(((uint16_t)buf[16]<<8) + buf[17]);
				sParam->glb.status.setWarning(((uint16_t)buf[18]<<8) + buf[19]);
			}
		}
	}

	return stat;
}
