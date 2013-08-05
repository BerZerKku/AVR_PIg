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
	uint8_t mask = 0;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, сброс флага
	this->stat_ = PRTS_STATUS_NO;

	// если это ответ на команду изменения параметра или режима
	// то сразу выйдем
	mask = com & GB_COM_MASK_GROUP;
	if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM) ||
		(mask == GB_COM_MASK_GROUP_WRITE_REGIME))
	{
		stat = true;
	}
	else
	{
		mask = com & GB_COM_MASK_DEVICE;
		if (mask == GB_COM_MASK_DEVICE_DEF)
		{
			if (com == GB_COM_DEF_GET_TYPE_AC)
			{
				// !!! Р400
				// Добавить обработку принятого типа АК и времени до АК
				// 4 байт - тип АК
				// 5-8 - время до АК
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			if (com == GB_COM_PRM_GET_TIME_ON)
			{
				stat = sParam_->prm.setTimeOn(buf[B1]);
			}
			else if (com == GB_COM_PRM_GET_BLOCK_COM)
			{
				sParam_->prm.setBlockCom(0, buf[B1]);
				sParam_->prm.setBlockCom(1, buf[B2]);
				sParam_->prm.setBlockCom(2, buf[B3]);
				sParam_->prm.setBlockCom(3, buf[B4]);
				stat = true;
			}
			else if (com == GB_COM_PRM_GET_TIME_OFF)
			{
				stat = sParam_->prm.setTimeOff(&buf[B1]);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{
			if (com == GB_COM_PRD_GET_TIME_ON)
			{
				stat = sParam_->prd.setTimeOn(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_DURATION)
			{
				stat = sParam_->prd.setDuration(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_TEST_COM)
			{
				// !!! пока только в составе МЕГА команды
				stat = sParam_->prd.setTestCom(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_BLOCK_COM)
			{
				sParam_->prd.setBlockCom(0, buf[B1]);
				sParam_->prd.setBlockCom(1, buf[B2]);
				sParam_->prd.setBlockCom(2, buf[B3]);
				sParam_->prd.setBlockCom(3, buf[B4]);
				stat = true;
			}
			else if (com == GB_COM_PRD_GET_LONG_COM)
			{
				sParam_->prd.setLongCom(0, buf[B1]);
				sParam_->prd.setLongCom(1, buf[B2]);
				sParam_->prd.setLongCom(2, buf[B3]);
				sParam_->prd.setLongCom(3, buf[B4]);
				stat = true;
			}
		}
		else
		{
			if (com == GB_COM_GET_TIME)
			{
				sParam_->dataTime.setYearFromBCD(buf[B1]);
				sParam_->dataTime.setMonthFromBCD(buf[B2]);
				sParam_->dataTime.setDayFromBCD(buf[B3]);
				sParam_->dataTime.setHourFromBCD(buf[B4]);
				sParam_->dataTime.setMinuteFromBCD(buf[B5]);
				sParam_->dataTime.setSecondFromBCD(buf[B6]);
				stat = true;
			}
			else if (com == GB_COM_GET_SOST)
			{

				sParam_->def.status.setRegime(buf[B1]);
				sParam_->def.status.setState(buf[B2]);
				sParam_->def.status.setDopByte(buf[B3]);

				sParam_->prm.status.setRegime(buf[B4]);
				sParam_->prm.status.setState(buf[B5]);
				sParam_->prm.status.setDopByte(buf[B6]);

				sParam_->prd.status.setRegime(buf[B7]);
				sParam_->prd.status.setState(buf[B8]);
				sParam_->prd.status.setDopByte(buf[B9]);
				stat = true;
			}
			else if (com == GB_COM_GET_FAULT)
			{
				stat = true;
				sParam_->def.status.setFault(TO_INT16(buf[B1], buf[B2]));
				sParam_->def.status.setWarning(TO_INT16(buf[B3], buf[B4]));

				sParam_->prm.status.setFault(TO_INT16(buf[B5], buf[B6]));
				sParam_->prm.status.setWarning(TO_INT16(buf[B7], buf[B8]));

				sParam_->prd.status.setFault(TO_INT16(buf[B9], buf[B10]));
				sParam_->prd.status.setWarning(TO_INT16(buf[B11],buf[B12]));

				sParam_->glb.status.setFault(TO_INT16(buf[B13], buf[B14]));
				sParam_->glb.status.setWarning(TO_INT16(buf[B15],buf[B16]));
			}
			else if (com == GB_COM_GET_MEAS)
			{
				// обработаем посылку, если стоит флаг опроса всех параметров
				if (buf[B1] == 0)
				{
					sParam_->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
					sParam_->measParam.setCurrentOut(TO_INT16(buf[B4],buf[B5]));
					// в buf[B7] передатся дробная часть напряжения * 100
					// т.е. если там 90, то это 0.9В.
					sParam_->measParam.setVoltageOut(buf[B6], (buf[B7] / 10));
					sParam_->measParam.setVoltageDef(buf[B8]);
					// B9 отведен под Uз второй линии
					sParam_->measParam.setVoltageCf(buf[B10]);
					// B11 байт отведен под Uk второй линии
					sParam_->measParam.setVoltageNoise(buf[B12]);
					// B13 байт отведен под кэффициент переполнения входа АЦП
					// B14, B15 байты отведены под вероятность пропуска команд
					stat = true;
				}
			}
			else if (com == GB_COM_GET_VERS)
			{
				bool re = false;
				// данные о типе аппарата
				re |= sParam_->def.status.setEnable(buf[B1] == 1);
				re |= sParam_->prm.setNumCom(buf[B2] * 4);
				// buf[B3] - прм2
				re |= sParam_->prd.setNumCom(buf[B4] * 4);
				re |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES)buf[B5]);
				re |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
				sParam_->glb.setVersBsp(TO_INT16(buf[B7], buf[B8]));
				sParam_->glb.setVersDsp(TO_INT16(buf[B9], buf[B10]));
				re |=sParam_->glb.setCompatibility((eGB_COMPATIBILITY)buf[B11]);

				// установим флаг необходимости настройки типа аппарата
				if (re)
					sParam_->device = false;

				stat = true;
			}
			else if (com == GB_COM_GET_UD_DEVICE)
			{
				// !!! Р400
				// !!! Добавить обработку Типа удаленного аппарата
			}
		}
	}

	return stat;
}

/**	Формирование посылки и отправка заданной команды.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t
clProtocolBspS::sendData(eGB_COM com)
{
	uint8_t num = 0;
	uint8_t mask = 0;

	mask = com & GB_COM_MASK_GROUP;
	if (mask == GB_COM_MASK_GROUP_WRITE_PARAM)
	{
		// команды изменения параметров
		mask = com & GB_COM_MASK_DEVICE;
	}
	else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME)
	{
		// команды изменения режима
		mask = com & GB_COM_MASK_DEVICE;

		if (com == GB_COM_SET_CONTROL)
		{
			num = addCom(com, sParam_->txComBuf.getByte());
		}
	}
	else
	{
		// команды опроса
		mask = com & GB_COM_MASK_DEVICE;
		if (mask == GB_COM_MASK_DEVICE_DEF)
		{
			if (com == GB_COM_DEF_GET_TYPE_AC)
			{
				num = addCom(com);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			if (com == GB_COM_PRM_GET_TIME_ON)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRM_GET_BLOCK_COM)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRM_GET_TIME_OFF)
			{
				num = addCom(com);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{
			if (com == GB_COM_PRD_GET_TIME_ON)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_DURATION)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_TEST_COM)
			{
				num = addCom(com);	// !!! пока только в составе МЕГА команды
			}
			else if (com == GB_COM_PRD_GET_BLOCK_COM)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_LONG_COM)
			{
				num = addCom(com);
			}
		}
		else
		{

			if (com == GB_COM_GET_TIME)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_SOST)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_FAULT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_MEAS)
			{
				num = addCom(com, 0);
			}
			else if (com == GB_COM_GET_VERS)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_UD_DEVICE)
			{
				num = addCom(com);
			}
		}
	}

	return num;
}
