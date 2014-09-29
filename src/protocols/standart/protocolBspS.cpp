/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolBspS.h"

#include "glbDefine.h"
#include "paramBsp.h"

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
	stat_ = PRTS_STATUS_NO;

	mask = com & GB_COM_MASK_GROUP;
	// ответ на команду изменения параметра/режима не требуется
	if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM)
			|| (mask == GB_COM_MASK_GROUP_WRITE_REGIME)) {
		stat = true;
	} else {
		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_DEF)
			stat = getDefCommand(com);				// команды защиты
		else if (mask == GB_COM_MASK_DEVICE_PRM)
			stat = getPrmCommand(com);				// команды приемника
		else if (mask == GB_COM_MASK_DEVICE_PRD)
			stat = getPrdCommand(com);				// команды передатчика
		else
			stat = getGlbCommand(com);				// команды общие
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

	mask = com & GB_COM_MASK_GROUP;
	if (mask == GB_COM_MASK_GROUP_WRITE_PARAM) {
		// команды изменения параметров

		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_DEF)
			num = sendModifDefCommand(com);
		else if (mask == GB_COM_MASK_DEVICE_PRM)
			num = sendModifPrmCommand(com);
		else if (mask == GB_COM_MASK_DEVICE_PRD)
			num = sendModifPrdCommand(com);
		else
			num = sendModifGlbCommand(com);
	} else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME) {
		// команды изменения Режимов Работы и работы с Тестами
		// по умолчанию отправляется только код команды

		if (com == GB_COM_SET_CONTROL) {
			num = addCom(com, sParam_->txComBuf.getInt8());
		} else if (com == GB_COM_SET_REG_TEST_1) {
			// есть две возможные ситуации:
			// отсылается команда включения Тестов
			// 		при этом передается 0 байт данных
			// отсылаются команды установки сигналов в тесте
			// 		тут последовательно передаются две команды
			// 		сначала команда для КЧ
			// 		а при следующем заходе  для РЗ
			if (sParam_->txComBuf.getInt8(0) == 1) {
				num = addCom(com, 1, sParam_->txComBuf.getInt8(1));		// КЧ
				sParam_->txComBuf.setInt8(0, 0);
			} else if (sParam_->txComBuf.getInt8(2) == 2) {
				num = addCom(com, 2, sParam_->txComBuf.getInt8(3));		// РЗ
				sParam_->txComBuf.setInt8(2, 0);
			} else {
				num = addCom(com);										// вкл.
			}
		} else {
			// GB_COM_PRM_ENTER
			// GB_COM_SET_REG_DISABLED
			// GB_COM_SET_REG_ENABLED
			// GB_COM_SET_REG_TEST_2
			// GB_COM_SET_REG_TEST_1
			num = addCom(com);
		}
	} else if (mask == GB_COM_MASK_GROUP_READ_PARAM) {
		// команды опроса параметров
		// по умолчанию отправляется только код команды
		if (com == GB_COM_GET_MEAS)
			num = addCom(com, 0);
		else
			num = addCom(com);
	} else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL) {
		// команды считывания журналов, в том числе и кол-ва записей
		num = sendReadJrnCommand(com);
	}

	// установка статуса, в зависимости от необходимости передачи сообщения
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}

/**	Обработка принятой команды Защиты.
 * 	@param com	Код команды
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getDefCommand(eGB_COM com) {
	bool stat = false;

	if (com == GB_COM_DEF_GET_DEF_TYPE) {
		stat = sParam_->def.setDefType(buf[B1]);
	} else if (com == GB_COM_DEF_GET_LINE_TYPE) {
		stat = sParam_->def.setNumDevices((eGB_NUM_DEVICES) buf[B1]);
		uint8_t act = sParam_->glb.setNumDevices((eGB_NUM_DEVICES) buf[B1]);
		if (act & GB_ACT_NEW)
			sParam_->device = false;
	} else if (com == GB_COM_DEF_GET_T_NO_MAN) {
		stat = sParam_->def.setTimeNoMan(buf[B1]);
	} else if (com == GB_COM_DEF_GET_DELAY) {
		stat = sParam_->def.setDelay(1, buf[B1]);
		sParam_->def.setDelay(2, buf[B2]);
	} else if (com == GB_COM_DEF_GET_OVERLAP) {
		stat = sParam_->def.setOverlap(buf[B1]);
	} else if (com == GB_COM_DEF_GET_RZ_DEC) {
		stat = sParam_->def.setRzDec(1, buf[B1]);
		sParam_->def.setRzDec(2, buf[B2]);
	} else if (com == GB_COM_DEF_GET_PRM_TYPE) {
		if (sParam_->typeDevice == AVANT_RZSK) {
			stat = sParam_->def.setPrmType(buf[B1]);
		} else if (sParam_->typeDevice == AVANT_R400M) {
			stat = sParam_->def.setAcDec(buf[B1]);
		}
	} else if (com == GB_COM_DEF_GET_FREQ_PRD) {
		sParam_->def.setFreqPrd((eGB_PVZL_FREQ) buf[B1]);
	} else if (com == GB_COM_DEF_GET_RZ_THRESH) {
		if (sParam_->typeDevice == AVANT_R400M) {
			// ! В Р400М это частота ПРМ (ПВЗЛ)
			sParam_->def.setFreqPrm((eGB_PVZL_FREQ) buf[B1]);
		} else
			stat = sParam_->def.setRzThreshold(buf[B1]);
	} else if (com == GB_COM_DEF_GET_TYPE_AC) {
		// 1 байт - тип АК
		// 2-5 - время до АК
		stat = sParam_->def.setTypeAC((eGB_TYPE_AC) buf[B1]);
		//				uint32_t t = buf[B2];
		//				t <<= 8;
		//				t += buf[B3];
		//				t <<= 8;
		//				t += buf[B4];
		//				t <<= 8;
		//				t += buf[B5];
		stat |= sParam_->def.setTimeToAC(*((uint32_t *) &buf[B2]));
	} else if (com == GB_COM_DEF_GET_JRN_CNT) {
		//				stat = sParam_->def.setNumJrnEntry(TO_INT16(buf[B2], buf[B1]));
		uint16_t t = TO_INT16(buf[B2], buf[B1]);
		stat = sParam_->jrnEntry.setNumJrnEntry(t);
	} else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF) {
			sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - день недели
			sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dataTime.setMsSecond(t);
			sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
			// sParam_->jrnEntry.setNumCom(buf[B2]);
			sParam_->jrnEntry.setSignalDef((buf[B2] << 4) + (buf[B4] & 0x0F));
			sParam_->jrnEntry.setEventType(buf[B3]);
			sParam_->jrnEntry.setReady();
			stat = true;
		}
	}

	return stat;
}

/**	Обработка принятой команды Приемника.
 * 	@param com	Код команды
 * 	@retval True - в случае успешной обработки
 * 	@retval False - в случае ошибки.
 */
bool clProtocolBspS::getPrmCommand(eGB_COM com) {
	bool stat = false;

	switch(com) {
		case GB_COM_PRM_GET_TIME_ON: {
			stat = sParam_->prm.setTimeOn(buf[B1]);
		}
		break;

		case GB_COM_PRM_GET_BLOCK_COM: {
			sParam_->prm.setBlockCom8(0, buf[B1]);
			sParam_->prm.setBlockCom8(1, buf[B2]);
			sParam_->prm.setBlockCom8(2, buf[B3]);
			sParam_->prm.setBlockCom8(3, buf[B4]);
			stat = true;
		}
		break;

		case GB_COM_PRM_GET_TIME_OFF: {
			stat = sParam_->prm.setTimeOff(&buf[B1]);
		}
		break;

		case GB_COM_PRM_GET_DR_STATE: {
			stat = sParam_->prm.setStateDR(buf[B1]);
		}
		break;

		case GB_COM_PRM_GET_DR_BLOCK: {
			stat = sParam_->prm.setBlockComDR8(0, buf[B1]);
			stat = (stat && sParam_->prm.setBlockComDR8(1, buf[B2]));
			stat = (stat && sParam_->prm.setBlockComDR8(2, buf[B3]));
			stat = (stat && sParam_->prm.setBlockComDR8(3, buf[B4]));
		}
		break;

		case GB_COM_PRM_GET_DR_COM: {
			stat = sParam_->prm.setComDR(&buf[B1]);
		}
		break;

		case GB_COM_PRM_GET_JRN_CNT: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_PRM_GET_JRN_ENTRY: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) {
				if (sParam_->typeDevice == AVANT_OPTO) {
					// дата
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B5]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B7]));
					// время
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B8]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B11], buf[B12]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B1]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
					// B13 - день недели
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
					sParam_->jrnEntry.setNumCom(buf[B2]);
					sParam_->jrnEntry.setEventType(buf[B3]);
					sParam_->jrnEntry.setReady();
				}
				stat = true;
			}
		}
		break;

		default:
			break;
	}

	return stat;
}

/**	Обработка принятой команды Передатчика.
 * 	@param com	Код команды
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getPrdCommand(eGB_COM com) {
	bool stat = false;

	switch (com) {
		case GB_COM_PRD_GET_TIME_ON: {
			stat = sParam_->prd.setTimeOn(buf[B1]);
		}
		break;

		case GB_COM_PRD_GET_DURATION: {
			if (sParam_->glb.getTypeLine() == GB_TYPE_LINE_OPTO) {
				stat = sParam_->prd.setDurationO(buf[B1]);
			} else {
				stat = sParam_->prd.setDurationL(buf[B1]);
			}
		}
		break;

		case GB_COM_PRD_GET_TEST_COM: {
			stat = sParam_->prd.setTestCom(buf[B1]);
		}
		break;

		case GB_COM_PRD_GET_BLOCK_COM: {
			sParam_->prd.setBlockCom8(0, buf[B1]);
			sParam_->prd.setBlockCom8(1, buf[B2]);
			sParam_->prd.setBlockCom8(2, buf[B3]);
			sParam_->prd.setBlockCom8(3, buf[B4]);
			stat = true;
		}
		break;

		case GB_COM_PRD_GET_LONG_COM: {
			sParam_->prd.setLongCom8(0, buf[B1]);
			sParam_->prd.setLongCom8(1, buf[B2]);
			sParam_->prd.setLongCom8(2, buf[B3]);
			sParam_->prd.setLongCom8(3, buf[B4]);
			stat = true;
		}
		break;

		case GB_COM_PRD_GET_DR_STATE: {
			stat = sParam_->prd.setStateDR(buf[B1]);
		}
		break;

		case GB_COM_PRD_GET_DR_BLOCK: {
			stat = sParam_->prd.setBlockComDR8(0, buf[B1]);
			stat = (stat && sParam_->prd.setBlockComDR8(1, buf[B2]));
			stat = (stat && sParam_->prd.setBlockComDR8(2, buf[B3]));
			stat = (stat && sParam_->prd.setBlockComDR8(3, buf[B4]));
		}
		break;

		case GB_COM_PRD_GET_COM_A: {
			stat = sParam_->prd.setNumComA(buf[B1]);
		}
		break;

		case GB_COM_PRD_GET_JRN_CNT: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_PRD_GET_JRN_ENTRY: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) {
				if (sParam_->typeDevice == AVANT_OPTO) {
					// дата
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B5]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B7]));
					// время
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B8]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B11], buf[B12]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B1]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
					// B13 - день недели
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
					sParam_->jrnEntry.setNumCom(buf[B2]);
					sParam_->jrnEntry.setEventType(buf[B3]);
					sParam_->jrnEntry.setSourceCom((eGB_SOURCE_COM) buf[B4]);
					sParam_->jrnEntry.setReady();
					stat = true;
				}
			}
		}
		break;

		default:
			break;
	}

	return stat;
}

/**	Обработка принятой Общей команды.
 * 	@param com	Код команды
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com) {
	bool stat = false;

	switch (com) {
		case GB_COM_GET_TIME: {
			sParam_->dateTime.setYear(BCD_TO_BIN(buf[B1]));
			sParam_->dateTime.setMonth(BCD_TO_BIN(buf[B2]));
			sParam_->dateTime.setDay(BCD_TO_BIN(buf[B3]));
			sParam_->dateTime.setHour(BCD_TO_BIN(buf[B4]));
			sParam_->dateTime.setMinute(BCD_TO_BIN(buf[B5]));
			sParam_->dateTime.setSecond(BCD_TO_BIN(buf[B6]));
			stat = true;
		}
		break;

		case GB_COM_GET_SOST: {
			sParam_->def.status.setRegime((eGB_REGIME) buf[B1]);
			sParam_->def.status.setState(buf[B2]);
			sParam_->def.status.setDopByte(buf[B3]);

			sParam_->prm.status.setRegime((eGB_REGIME) buf[B4]);
			sParam_->prm.status.setState(buf[B5]);
			sParam_->prm.status.setDopByte(buf[B6]);

			sParam_->prd.status.setRegime((eGB_REGIME) buf[B7]);
			sParam_->prd.status.setState(buf[B8]);
			sParam_->prd.status.setDopByte(buf[B9]);

			eGB_REGIME reg = GB_REGIME_MAX;
			eGB_REGIME regTmp = GB_REGIME_MAX;
			// определение общего режима аппарата
			// по умолчанию оно будет GB_REGIME_ENABLED
			// другое состояние возможно, если все устройства аппарата
			// имеют один и тот же режим
			if (sParam_->prd.status.isEnable()) {
				regTmp = sParam_->prd.status.getRegime();
				if (reg == GB_REGIME_MAX)
					reg = regTmp;
				else if (reg != regTmp)
					reg = GB_REGIME_ENABLED;
			}

			if (sParam_->prm.status.isEnable()) {
				regTmp = sParam_->prm.status.getRegime();
				if (reg == GB_REGIME_MAX)
					reg = regTmp;
				else if (reg != regTmp)
					reg = GB_REGIME_ENABLED;
			}

			if (sParam_->def.status.isEnable()) {
				regTmp = sParam_->def.status.getRegime();
				if (reg == GB_REGIME_MAX)
					reg = regTmp;
				else if (reg != regTmp)
					reg = GB_REGIME_ENABLED;
			}
			sParam_->glb.status.setRegime(reg);

			stat = true;
		}
		break;

		case GB_COM_GET_FAULT: {
			sParam_->def.status.setFault(TO_INT16(buf[B1], buf[B2]));
			sParam_->def.status.setWarning(TO_INT16(buf[B3], buf[B4]));

			sParam_->prm.status.setFault(TO_INT16(buf[B5], buf[B6]));
			sParam_->prm.status.setWarning(TO_INT16(buf[B7], buf[B8]));

			sParam_->prd.status.setFault(TO_INT16(buf[B9], buf[B10]));
			sParam_->prd.status.setWarning(TO_INT16(buf[B11],buf[B12]));

			sParam_->glb.status.setFault(TO_INT16(buf[B13], buf[B14]));
			sParam_->glb.status.setWarning(TO_INT16(buf[B15],buf[B16]));
			stat = true;
		}
		break;

		case GB_COM_GET_MEAS: {
			// обработаем посылку, если стоит флаг опроса всех параметров
			if (buf[B1] == 0) {
				sParam_->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
				sParam_->measParam.setCurrentOut(TO_INT16(buf[B4],buf[B5]));
				// в buf[B7] передатся дробная часть напряжения * 100
				// т.е. если там 90, то это 0.9В.
				sParam_->measParam.setVoltageOut(buf[B6], (buf[B7] / 10));
				sParam_->measParam.setVoltageDef(buf[B8]);
				sParam_->measParam.setVoltageDef2(buf[B9]);
				sParam_->measParam.setVoltageCf(buf[B10]);
				sParam_->measParam.setVoltageCf2(buf[B11]);
				sParam_->measParam.setVoltageNoise(buf[B12]);
				sParam_->measParam.setVoltageNoise2(buf[B13]);
				sParam_->measParam.setPulseWidth(TO_INT16(buf[B14], buf[B15]));
				stat = true;
			}
		}
		break;

		case GB_COM_GET_VERS: {
			uint8_t act = GB_ACT_NO;
			// данные о типе аппарата
			act |= sParam_->def.status.setEnable(buf[B1] == 1);
			act |= sParam_->prm.setNumCom(buf[B2] * 4);
			// buf[B3] - прм2
			act |= sParam_->prd.setNumCom(buf[B4] * 4);
			// кол-во аппаратов в линии
			// в def хранится значение параметра
			// все действия с меню производятся относительно значения в glb.
			// !!! и значение == кол-ву аппаратов, а раньше было на 1 меньше
			act |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES) (buf[B5] - 1));
			sParam_->def.setNumDevices((eGB_NUM_DEVICES) (buf[B5] - 1));
			// тип линии (вч, оптика, ...)
			act |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
			// версия прошивки АТмега БСП
			TDeviceGlb *glb = &sParam_->glb;
			glb->setVersProgIC16(TO_INT16(buf[B7], buf[B8]) , GB_IC_BSP_MCU);
			glb->setVersProgIC16(TO_INT16(buf[B9], buf[B10]), GB_IC_BSP_DSP);
			glb->setVersProgIC16(VERS, GB_IC_PI_MCU);
			glb->setVersProgIC8(buf[B12], GB_IC_BSK_PLIS_PRD1);
			glb->setVersProgIC8(buf[B13], GB_IC_BSK_PLIS_PRD2);
			glb->setVersProgIC8(buf[B14], GB_IC_BSK_PLIS_PRM1);
			glb->setVersProgIC8(buf[B15], GB_IC_BSK_PLIS_PRM2);
			glb->setVersProgIC8(buf[B16], GB_IC_BSZ_PLIS);

			// совместимость, только в Р400м
			act |= sParam_->glb.setCompatibility((eGB_COMPATIBILITY) buf[B11]);

			// проверим необходимость обновления типа аппарата
			if (act & GB_ACT_NEW)
				sParam_->device = false;

			stat = ((act & GB_ACT_ERROR) != GB_ACT_ERROR);
		}
		break;

		case GB_COM_GET_TIME_SINCHR: {
			stat = sParam_->glb.setTimeSinchr(buf[B1]);
			if (sParam_->typeDevice == AVANT_RZSK) {
				stat |= sParam_->glb.setDetector(buf[B2]);
			}
		}
		break;

		case GB_COM_GET_DEVICE_NUM: {
			stat = sParam_->glb.setDeviceNum(buf[B1]);
		}
		break;

		case GB_COM_GET_OUT_CHECK: {
			stat = sParam_->glb.setOutCheck(buf[B1]);
		}
		break;

		case GB_COM_GET_CF_THRESHOLD: {
			sParam_->glb.setCfThreshold(buf[B1]);
			sParam_->glb.setInDecrease(buf[B2], 1);
			sParam_->glb.setInDecrease(buf[B3], 2);
			stat = true;
		}
		break;

		case GB_COM_GET_TIME_RERUN: {
			if (sParam_->typeDevice == AVANT_R400M) {
				eGB_COMPATIBILITY comp = sParam_->glb.getCompatibility();
				if (comp == GB_COMPATIBILITY_PVZL) {
					// в Р400м ПВЗЛ это Снижение ответа АК
					stat = sParam_->glb.setAcInDec(buf[B1]);
				} else if (comp == GB_COMPATIBILITY_PVZUE) {
					// в Р400м ПВЗУ-Е это набор параметров
					TDeviceGlb *glb = &sParam_->glb;
					stat = glb->setPvzueProtocol((eGB_PVZUE_PROTOCOL) buf[B1]);
					stat |= glb->setPvzueParity((eGB_PVZUE_PARITY) buf[B2]);
					stat |= glb->setPvzueFail(buf[B3]);
					stat |= glb->setPvzueNoiseTH(buf[B4]);
					stat |= glb->setPvzueNoiseLvl(buf[B5]);
					stat |= glb->setPvzueTypeAC((eGB_PVZUE_TYPE_AC) buf[B6]);
				}
			} else {
				// в командной аппаратуре "Время перезапуска"
				stat = sParam_->glb.setTimeRerun(buf[B1]);
			}
		}
		break;

		case GB_COM_GET_FREQ: {
			uint16_t t = TO_INT16(buf[B1], buf[B2]);
			stat = sParam_->glb.setFreq(t);
		}
		break;

		case GB_COM_GET_COM_PRM_KEEP: {
			if (sParam_->typeDevice == AVANT_R400M) {
				// ! в Р400 это Uвых номинальное
				stat = sParam_->glb.setUoutNom(buf[B1]);
			} else {
				stat = sParam_->glb.setComPrmKeep(buf[B1]);
			}
		}
		break;

		case GB_COM_GET_COM_PRD_KEEP: {
			if (sParam_->typeDevice == AVANT_R400M) {
				// в Р400м это Своместимость (тип удаленного аппарата)
				uint8_t act = GB_ACT_NO;
				act = sParam_->glb.setCompatibility((eGB_COMPATIBILITY)buf[B1]);
				// в случае записи нового значения, сбросим флаг конфигурации
				if (act & GB_ACT_NEW)
					sParam_->device = false;
			} else {
				stat = sParam_->glb.setComPrdKeep(buf[B1]);
			}
		}
		break;

		case GB_COM_GET_NET_ADR: {
			stat = sParam_->glb.setNetAddress(buf[B1]);
		}
		break;

		case GB_COM_GET_COR_U_I: {
			eGB_TYPE_DEVICE type = sParam_->typeDevice;
			// в оптике это резервирование, иначе коррекция тока и напряжения
			if (type == AVANT_OPTO) {
				stat = sParam_->glb.setBackup(buf[B1]);
			} else {
				int16_t val = 0;
				int8_t i = buf[B1];
				int8_t f = buf[B2];
				val = i * 10 + static_cast<int16_t>(f / 10);
				stat = sParam_->glb.setCorU(val);
				// коррекция тока
				val = static_cast<int16_t>(TO_INT16(buf[B3], buf[B4]));
				stat |= sParam_->glb.setCorI(val);
			}
		}
		break;

		case GB_COM_GET_TEST: {
			eGB_TYPE_DEVICE type = sParam_->typeDevice;
			// определеим макимальное кол-во

			sParam_->test.setCurrentSignal(&buf[B1], type);
		}
		break;

		case GB_COM_GET_JRN_CNT: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_GET_JRN_ENTRY: {
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
				if (sParam_->typeDevice == AVANT_OPTO) {
					// дата
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B7]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B8]));
					// время
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B10]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B11]));
					uint16_t t = TO_INT16(buf[B12], buf[B13]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setRegime((eGB_REGIME) buf[B1]);
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B2]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					// дата
					sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
					// B13 - день недели
					// время
					sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dataTime.setMsSecond(t);
					// ! B1 - тип устройства, на данный момент игнорируется
					sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
					sParam_->jrnEntry.setEventType(buf[B2]);
					sParam_->jrnEntry.setRegime((eGB_REGIME) buf[B3]);
					sParam_->jrnEntry.setReady();
					stat = true;
				}
			}
		}
		break;

		default:
			break;

	}

	return stat;
}

/**	Формирование сообщения команды Защиты.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifDefCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->txComBuf.getInt8(0);
	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_DEF_SET_DELAY) {
		// Р400м трех-концевая версия может быть два параметра,
		// а в двух концевой только один
		// но будем передавать два байта всегда
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_DEF_SET_RZ_DEC) {
		num = addCom(com, b1, b2);
	} else {
		// по умолчанию передается один байт
		num = addCom(com, b1);
	}

	return num;
}

/**	Формирование сообщения команды Приемника.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifPrmCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->txComBuf.getInt8(0);
	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_PRM_SET_TIME_OFF) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_PRM_SET_BLOCK_COM) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_PRM_RES_IND) {
		num = addCom(com);
	} else if (com == GB_COM_PRM_SET_DR_BLOCK) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_PRM_SET_DR_COM) {
		num = addCom(com, b1, b2);
	} else {
		// по умолчанию передается один байт
		// GB_COM_PRM_SET_TIME_ON
		num = addCom(com, b1);
	}

	return num;
}

/**	Формирование сообщения команды Передатчика.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifPrdCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->txComBuf.getInt8(0);
	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_PRD_SET_BLOCK_COM) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_PRD_SET_LONG_COM) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_PRD_SET_DR_BLOCK) {
		num = addCom(com, b1, b2);
	} else {
		// по умолчанию передается один байт
		num = addCom(com, b1);
	}

	return num;
}

/**	Формирование сообщения Общей команды.
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendModifGlbCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->txComBuf.getInt8(0);
	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_SET_TIME) {
		num = addCom(com, 6, sParam_->txComBuf.getBuferAddress());
	} else if (com == GB_COM_SET_TIME_RERUN) {
		if (sParam_->typeDevice == AVANT_R400M) {
			num = addCom(com, b1, b2);
		} else
			num = addCom(com, b1);
	} else if (com == GB_COM_SET_CF_THRESHOLD) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_SET_FREQ) {
		num = addCom(com, b1, b2);
	} else if (com == GB_COM_SET_COR_U_I) {
		num = addCom(com, 3, sParam_->txComBuf.getBuferAddress());
	} else if (com == GB_COM_SET_TIME_SINCHR) {
		if (sParam_->typeDevice == AVANT_RZSK) {
			num = addCom(com, b1, b2);
		} else {
			num = addCom(com, b1);
		}
	} else {
		// по умолчанию передается один байт
		num = addCom(com, b1);
	}

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
	if (mask == GB_COM_MASK_DEVICE_DEF) {
		if (com == GB_COM_DEF_GET_JRN_CNT) {
			num = addCom(com);
		} else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
			uint16_t t = sParam_->txComBuf.getInt16();
			if (sParam_->typeDevice == AVANT_R400M)
				num = addCom(com, t, t >> 8);
			else
				num = addCom(com, t >> 8, t);
		}
	} else if (mask == GB_COM_MASK_DEVICE_PRM) {
		if (com == GB_COM_PRM_GET_JRN_CNT) {
			num = addCom(com);
		} else if (com == GB_COM_PRM_GET_JRN_ENTRY) {
			uint16_t t = sParam_->txComBuf.getInt16();
			num = addCom(com, t >> 8, t);
		}
	} else if (mask == GB_COM_MASK_DEVICE_PRD) {
		if (com == GB_COM_PRD_GET_JRN_CNT) {
			num = addCom(com);
		} else if (com == GB_COM_PRD_GET_JRN_ENTRY) {
			uint16_t t = sParam_->txComBuf.getInt16();
			num = addCom(com, t >> 8, t);
		}
	} else {
		if (com == GB_COM_GET_JRN_CNT) {
			num = addCom(com);
		} else if (com == GB_COM_GET_JRN_ENTRY) {
			uint16_t t = sParam_->txComBuf.getInt16();

			if (sParam_->typeDevice == AVANT_R400M)
				num = addCom(com, t, t >> 8);
			else
				num = addCom(com, t >> 8, t);
		}
	}

	return num;
}
