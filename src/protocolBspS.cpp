/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

#include "../inc/glbDefine.h"
#include "../inc/paramBsp.h"

/**	Конструктор.
 * 	@param *buf Указатель на буфер данных.
 * 	@param size Размер буфера данных.
 * 	@param *sParam Указатель на структуру параметров.
 */
clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
clProtocolS(buf, size, sParam) {

}

/**	Обработка принятого сообщения.
 * 	@param Нет
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getData() {
	bool stat = false;
	uint8_t mask = 0;
	eGB_COM com = (eGB_COM) buf[2];

	// сообщение обработано, выставим флаг на чтение
	this->stat_ = PRTS_STATUS_NO;

	switch(com & GB_COM_MASK_GROUP) {

		case GB_COM_MASK_GROUP_READ_PARAM:		// команды чтения параметров
			// vvv ВНИЗ vvv
		case GB_COM_MASK_GROUP_READ_JOURNAL:	// команды чтения журналов
			switch(com & GB_COM_MASK_DEVICE) {
				case  GB_COM_MASK_DEVICE_GLB:
					// команды чтения общие
					stat = getGlbCommand(com);
					break;
				case GB_COM_MASK_DEVICE_RPS:
					// команды чтения терминала
					stat = getRpsCommand(com);
					break;
			}
			break;

		case GB_COM_MASK_GROUP_WRITE_PARAM:		// команды записи параметров
			// обработка ответа команды записи не требуется
			stat = true;
			break;

		case GB_COM_MASK_GROUP_WRITE_REGIME:	// команды изменения режимов
			// обработка ответа команды изменения режима не требуется
			stat = true;
			break;
	}

	return stat;
}

/**	Формирование посылки и отправка заданной команды.
 * 	По-умолчанию в сообщении опроса параметра посылается только код команды.
 * 	По-умолчанию в сообщениии изменения параметра посылается 1 байт данных.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendData(eGB_COM com) {
	uint8_t num = 0;
	uint8_t mask = 0;

	mask = com & GB_COM_MASK_DEVICE;

	switch(com & GB_COM_MASK_GROUP) {

		case GB_COM_MASK_GROUP_WRITE_PARAM: {
			// команды на изменение параметров
			switch(com & GB_COM_MASK_DEVICE) {
				case GB_COM_MASK_DEVICE_GLB:
					num = sendModifGlbCommand(com);
					break;
				case GB_COM_MASK_DEVICE_PRM:
					// GB_COM_SET_PRM_RES_IND
					num = addCom(com);
					break;
				case GB_COM_MASK_DEVICE_RPS:
					num = sendModifRpsCommand(com);
					break;
			}
		}
		break;

		case GB_COM_MASK_GROUP_WRITE_REGIME: {
			// команды на изменение режимов работы
			if (com == GB_COM_SET_CONTROL) {
				num = addCom(com, sParam_->TxComBuf.getInt8());
			} else {
				// GB_COM_SET_PRM_ENTER
				// GB_COM_SET_REG_DISABLED
				// GB_COM_SET_REG_ENABLED
				num = addCom(com);
			}
		}
		break;

		case GB_COM_MASK_GROUP_READ_PARAM: {
			// команды считывания параметров
			if (com == GB_COM_GET_MEAS) {
				// для получения всех измеряемых параметров посылаем БД = 0.
				num = addCom(com, 0);
			} else if (com == GB_COM_GET_COMP_DELAY)
				// для получения компенсации задежки посылаем БД = 2.
				// для получения всех общих параметров посылаем БД = 1.
				num = addCom(com, 2);
			else {
				num = addCom(com);
			}
		}
		break;

		case GB_COM_MASK_GROUP_READ_JOURNAL: {
			// команды считывания журналов, в том числе и кол-ва записей
			num = sendReadJrnCommand(com);
		}
		break;
	}

	// установка статуса, в зависимости от необходимости передачи сообщения
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}

/**	Обработка принятой Общей команды.
 * 	@param com	Код команды
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com) {
	bool stat = false;

	switch (com) {
		case GB_COM_GET_TIME: {
			// текущие дата и время
			stat =  sParam_->DataTime.setYear(BCD_TO_BIN(buf[B1]));
			stat &= sParam_->DataTime.setMonth(BCD_TO_BIN(buf[B2]));
			stat &= sParam_->DataTime.setDay(BCD_TO_BIN(buf[B3]));
			stat &= sParam_->DataTime.setHour(BCD_TO_BIN(buf[B4]));
			stat &= sParam_->DataTime.setMinute(BCD_TO_BIN(buf[B5]));
			stat &= sParam_->DataTime.setSecond(BCD_TO_BIN(buf[B6]));
		}
		break;

		case GB_COM_GET_SOST: {
			// режим / состояние терминала
			stat =  sParam_->Rps.Status.setRegime((eGB_REGIME) buf[B1]);
			stat &= sParam_->Rps.Status.setState(buf[B2]);
			stat &= sParam_->Rps.Status.setDopByte(buf[B3]);

			// общий режим
			stat &= sParam_->Glb.Status.setRegime((eGB_REGIME) buf[B1]);
		}
		break;

		case GB_COM_GET_FAULT: {
			// неисправности и предупреждения терминала
			stat =  sParam_->Rps.Status.setFault(TO_INT16(buf[B1], buf[B2]));
			stat &= sParam_->Rps.Status.setWarning(TO_INT16(buf[B3], buf[B4]));

			// общие неисправности и предупреждения
 			stat &= sParam_->Glb.Status.setFault(TO_INT16(buf[B13], buf[B14]));
			stat &= sParam_->Glb.Status.setWarning(TO_INT16(buf[B15],buf[B16]));
		}
		break;

		case GB_COM_GET_MEAS: {
			// обработаем посылку, если стоит флаг опроса всех параметров
			if (buf[B1] == 0) {
				stat  = sParam_->MeasParam.setPOut(buf[B2]);
				stat &= sParam_->MeasParam.setUCtrl(buf[B3]);
				stat &= sParam_->MeasParam.setSN(buf[B4]);
				stat &= sParam_->MeasParam.setNOut(buf[B5]);
				stat &= sParam_->MeasParam.setNIn(buf[B6]);
				stat &= sParam_->MeasParam.setFazA(TO_INT16(buf[B7], buf[B8]));
				stat &= sParam_->MeasParam.setFazA(TO_INT16(buf[B9], buf[B10]));
				stat &= sParam_->MeasParam.setFazA(TO_INT16(buf[B11],buf[B12]));
				stat &= sParam_->MeasParam.setIA(buf[B13]);
				stat &= sParam_->MeasParam.setIB(buf[B14]);
				stat &= sParam_->MeasParam.setIC(buf[B15]);
				stat &= sParam_->MeasParam.setUA(TO_INT16(buf[B16], buf[B17]));
				stat &= sParam_->MeasParam.setUB(TO_INT16(buf[B18], buf[B19]));
				stat &= sParam_->MeasParam.setUC(TO_INT16(buf[B20], buf[B21]));
			}
		}
		break;

		case GB_COM_GET_VERS: {
			// версии прошивок
			TDeviceGlb *glb = &sParam_->Glb;
			glb->setVersProgIC16(TO_INT16(buf[B7], buf[B8]) , GB_IC_BSP_MCU);
			glb->setVersProgIC16(TO_INT16(buf[B9], buf[B10]), GB_IC_BSP_DSP);
			glb->setVersProgIC16(VERS, GB_IC_PI_MCU);
			glb->setVersProgIC8(buf[B12], GB_IC_BSK_PLIS_PRD1);
			glb->setVersProgIC8(buf[B13], GB_IC_BSK_PLIS_PRD2);
			glb->setVersProgIC8(buf[B14], GB_IC_BSK_PLIS_PRM1);
			glb->setVersProgIC8(buf[B15], GB_IC_BSK_PLIS_PRM2);
			stat = true;
		}
		break;

		case GB_COM_GET_COMP_DELAY: {
			// компенсация задержки
			stat = sParam_->Glb.setCompDelay(buf[B1]);
		}
		break;

		case GB_COM_GET_CURR_MAX: {
			// максимальный ток
			stat = sParam_->Glb.setCurrMax(buf[B1]);
		}
		break;

		case GB_COM_GET_CURR_MIN: {
			// минимальный ток
			stat = sParam_->Glb.setCurrMin(buf[B1]);
		}
		break;

		case GB_COM_GET_VOLT_MIN : {
			// минимальное напряжение
			stat = sParam_->Glb.setVoltMin(buf[B1]);
		}
		break;

		case GB_COM_GET_JRN_CNT: {
			// кол-во записей в журнале событий
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->JrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_GET_JRN_ENTRY: {
			// запись журнала событий
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {

				// дата
				sParam_->JrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
				sParam_->JrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
				sParam_->JrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
				// B13 - день недели
				// время
				sParam_->JrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
				sParam_->JrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
				sParam_->JrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
				uint16_t t = TO_INT16(buf[B9], buf[B8]);
				sParam_->JrnEntry.dataTime.setMsSecond(t);
				// ! B1 - тип устройства, на данный момент игнорируется
				sParam_->JrnEntry.setReady();
				stat = true;
			}
		}
		break;

		default:
			break;

	}

	return stat;
}

/**	Обработка принятой команды Терминала
 * 	@param com	Код команды
 * 	@retval True В случае успешной обработки
 * 	@retval False В случае ошибки.
 */
bool clProtocolBspS::getRpsCommand(eGB_COM com) {
	bool stat = false;

	switch (com) {
		case GB_COM_GET_JRN_CNT: {
			// кол-во записей в журнале событий
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_RPS) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->JrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_GET_JRN_ENTRY: {
			// запись журнала событий
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_RPS) {
				// дата
				sParam_->JrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
				sParam_->JrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
				sParam_->JrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
				// B13 - день недели
				// время
				sParam_->JrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
				sParam_->JrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
				sParam_->JrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
				uint16_t t = TO_INT16(buf[B9], buf[B8]);
				sParam_->JrnEntry.dataTime.setMsSecond(t);
				// ! B1 - тип устройства, на данный момент игнорируется
				sParam_->JrnEntry.setReady();
				stat = true;
			}
		}
		break;

		default:
			break;

	}

	return stat;
}

/**	Формирование сообщения Общей команды.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifGlbCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->TxComBuf.getInt8(0);
//	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_SET_TIME) {
		num = addCom(com, 6, sParam_->TxComBuf.getBuferAddress());
	}  else {
		// по умолчанию передается один байт
		num = addCom(com, b1);
	}

	return num;
}

/**	Формирование сообщения команды Терминала.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifRpsCommand(eGB_COM com) {
	uint8_t num = 0;
//	uint8_t b1 = sParam_->txComBuf.getInt8(0);
//	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	return num;
}

/**	Формирование сообщения команды считавания кол-ва и самих записей журнала.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendReadJrnCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t mask = 0;

	// команды работы с журналом
	mask = com & GB_COM_MASK_DEVICE;
	if (com == GB_COM_GET_JRN_CNT) {

		num = addCom(com);
	} else if (com == GB_COM_GET_JRN_ENTRY) {
		uint16_t t = sParam_->TxComBuf.getInt16();

		num = addCom(com, t >> 8, t);
	}

	return num;
}
