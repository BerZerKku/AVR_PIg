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

	// если это ответ на команду изменения параметра или режима
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
				sParam->def.status.setFault(TO_INT16(buf[4], buf[5]));
				sParam->def.status.setWarning(TO_INT16(buf[6], buf[7]));

				sParam->prm.status.setFault(TO_INT16(buf[8], buf[9]));
				sParam->prm.status.setWarning(TO_INT16(buf[10], buf[11]));

				sParam->prd.status.setFault(TO_INT16(buf[12], buf[13]));
				sParam->prd.status.setWarning(TO_INT16(buf[14], buf[15]));

				sParam->glb.status.setFault(TO_INT16(buf[16], buf[17]));
				sParam->glb.status.setWarning(TO_INT16(buf[18], buf[19]));
			}
			else if (com == GB_COM_GET_MEAS)
			{
				// обработаем посылку, если стоит флаг опроса всех параметров
				if (buf[4] == 0)
				{
					sParam->measParam.setResistOut(TO_INT16(buf[5], buf[6]));
					sParam->measParam.setCurrentOut(TO_INT16(buf[7], buf[8]));
					// в buf[10] передатся дробная часть напряжения * 100
					// т.е. если там 90, то это 0.9В.
					sParam->measParam.setVoltageOut(buf[9], (buf[10] / 10));
					sParam->measParam.setVoltageDef(buf[11]);
					// 12 байт отведен под Uз второй линии
					sParam->measParam.setVoltageCf(buf[13]);
					// 14 байт отведен под Uk второй линии
					sParam->measParam.setVoltageNoise(buf[15]);
					// 15 байт отведен под кэффициент переполнения входа АЦП
					// 16, 17 байты отведены под вероятность пропуска команд
				}
			}
		}
	}

	return stat;
}
