/*
 * protocolPcI.cpp
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#include "protocolPcI.h"

// Конструктор
TProtocolPcI::TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), CIec101(buf, size) {

	for(uint8_t i = 0; i < SIZE_OF(m_bValue); i++) {
		m_bValue[i] = getDevice(IE_ERROR + i);
	}
}

// Функция отправки сообщения.
uint8_t TProtocolPcI::send() {
	return sendData();
}

// Проверка наличия данных класса 1(2) на передачу.
bool TProtocolPcI::checkEvent() {
	bool state = false;

	for(uint8_t i = 0; i < SIZE_OF(m_bValue); i++) {
		if (m_bValue[i] != getDevice(IE_ERROR + i)) {
			state = true;
			break;
		}
	}

	return state;
}

// Отправка события.
bool TProtocolPcI::procEvent(void) {
	bool state = false;
	SCp56Time2a time;
	bool val = false;
	uint16_t adr = 0;

	for(uint8_t i = 0; i < SIZE_OF(m_bValue); i++) {
		adr = IE_ERROR + i;
		val = getDevice(adr);
		if (m_bValue[i] != val) {
			time.years 			= sParam_->DateTime.getYear();
			time.months 		= sParam_->DateTime.getMonth();
			time.dayOfMonth 	= sParam_->DateTime.getDay();
			time.hours 			= sParam_->DateTime.getHour();
			time.minutes 		= sParam_->DateTime.getMinute();
			time.milliseconds 	= sParam_->DateTime.getSecond() * 1000;
			time.milliseconds  += sParam_->DateTime.getMsSecond();

			prepareFrameMSpTb1(adr, COT_SPONT, val, time);

			m_bValue[i] = val;
			state = true;
			break;
		}
	}

	return state;
}

// Обработка ответа на команду опроса.
bool TProtocolPcI::procInterrog(uint16_t &adr, bool &val) {
	adr++;

	if (adr <= IE_PRM_COM) {
		if (adr < IE_ERROR)
			adr = IE_ERROR;
		val = getDevice(adr);
		return true;
	}

	if (adr <= IE_GLB_WARNING_END) {
		if (adr < IE_GLB_ERROR_START)
			adr = IE_GLB_ERROR_START;
		val = getGlb(adr);
		return true;
	}

	if (sParam_->prd.status.isEnable() && (adr <= IE_PRD_TEST)) {
		if (adr < IE_PRD_ON)
			adr = IE_PRD_ON;
		val = getPrd(adr);
		return true;
	}

	if (sParam_->prm.status.isEnable() && (adr <= IE_PRM_TEST)) {
		if (adr < IE_PRM_ON)
			adr = IE_PRM_ON;
		val = getPrm(adr);
		return true;
	}

	if (sParam_->def.status.isEnable() &&(adr <= IE_DEF_WARNING_END)) {
		if (adr < IE_DEF_ON)
			adr = IE_DEF_ON;
		val = getDef(adr);
		return true;
	}

	return false;
}

// Установка времени
bool TProtocolPcI::procSetTime() {
	// Заполнение команды для передачи нового времени в БСП

	uint8_t i = 0;
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.years), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.months), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.dayOfMonth), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.hours), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.minutes), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD(stTime.milliseconds / 1000), i++);
	uint16_t ms = stTime.milliseconds % 1000 + getDelay();
	sParam_->txComBuf.setInt8(BIN_TO_BCD((uint8_t) ms), i++);
	sParam_->txComBuf.setInt8(BIN_TO_BCD((uint8_t) (ms >> 8)), i++);
	sParam_->txComBuf.setInt8(1, i++);
	sParam_->txComBuf.addFastCom(GB_COM_SET_TIME);

	// сброс флага наличия ответа на команду установки времени в БСП
	sParam_->DateTimeReq.setTimeBsp_ = false;

//	uint16_t d = getDelay();

	return true;
}

// Установка времени, сообщение об окочнании.
bool TProtocolPcI::procSetTimeEnd() {
	if (!sParam_->DateTimeReq.setTimeBsp_)
		return false;

	// очистка метки времени
	uint8_t *ptr = (uint8_t *) &stTime;
	for(uint8_t i = 0; i < sizeof(SCp56Time2a); i++) {
		*ptr++ = 0;
	}

	stTime.years = sParam_->DateTimeReq.getYear();
	stTime.months = sParam_->DateTimeReq.getMonth();
	stTime.dayOfMonth = sParam_->DateTimeReq.getDay();
	stTime.hours = sParam_->DateTimeReq.getHour();
	stTime.minutes = sParam_->DateTimeReq.getMinute();
	stTime.milliseconds = 1000 * sParam_->DateTimeReq.getSecond();
	stTime.milliseconds +=	sParam_->DateTimeReq.getMsSecond();

	// сброс флага наличия принятых даты/времени в момент синхронизации БСП
	sParam_->DateTimeReq.setTimeBsp_ = false;

	return true;
}

// Возвращает флаги общие для аппарата.
bool TProtocolPcI::getDevice(uint16_t adr) const {
	bool val = false;

	if (adr == IE_ERROR) {
		val = sParam_->glb.status.isFault();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isFault();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isFault();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isFault();
	} else if (adr == IE_WARNING) {
		val = sParam_->glb.status.isWarning();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isWarning();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isWarning();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isWarning();
	} else if (adr == IE_PRD_COM) {
		val = sParam_->prd.isIndCom();
	} else if (adr == IE_PRM_COM) {
		val = sParam_->prm.isIndCom();
	}

	return val;
}

// Возвращает флаги общего состояния.
bool TProtocolPcI::getGlb(uint16_t adr) const {
	bool val = false;

	if (adr >= IE_GLB_WARNING_START) {
		if (adr <= IE_GLB_WARNING_END) {
			val = sParam_->glb.status.isWarning(adr - IE_GLB_WARNING_START);
		}
	} else if (adr >= IE_GLB_ERROR_START) {
		if (adr <= IE_GLB_ERROR_END) {
			val = sParam_->glb.status.isFault(adr - IE_GLB_ERROR_START);
		}
	}

	return val;
}

// Возвращает флаги передатчика.
bool TProtocolPcI::getPrd(uint16_t adr) const {
	bool val = false;

	if (adr == IE_PRD_TEST) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prd.status.getRegime() == GB_REGIME_TEST_2);
	} else if (adr == IE_PRD_ENABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_ENABLED);
	} else if (adr == IE_PRD_DISABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_DISABLED);
	} if (adr >= IE_PRD_COM_START) {
		if (adr <= IE_PRD_COM_END) {
			val = sParam_->prd.getIndCom(adr - IE_PRD_COM_START);
		}
	} else if (adr >= IE_PRD_WARNING_START) {
		if (adr <= IE_PRD_WARNING_END) {
			val = sParam_->prd.status.isWarning(adr - IE_PRD_WARNING_START);
		}
	} else if (adr >= IE_PRD_ERROR_START) {
		if (adr <= IE_PRD_ERROR_END) {
			val = sParam_->prd.status.isFault(adr - IE_PRD_ERROR_START);
		}
	} else if (adr == IE_PRD_ON) {
		val = sParam_->prd.status.isEnable();
	}

	return val;
}

// Возвращает флаги приемника.
bool TProtocolPcI::getPrm(uint16_t adr) const {
	bool val = false;

	if (adr == IE_PRM_TEST) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prm.status.getRegime() == GB_REGIME_TEST_2);
	} else if (adr == IE_PRM_ENABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_ENABLED);
	} else if (adr == IE_PRM_READY) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_READY);
	} else if (adr == IE_PRM_DISABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_DISABLED);
	} else if (adr >= IE_PRM_COM_START) {
		if (adr <= IE_PRM_COM_END) {
			val = sParam_->prm.getIndCom(adr - IE_PRM_COM_START);
		}
	} else if (adr >= IE_PRM_WARNING_START) {
		if (adr <= IE_PRM_WARNING_END) {
			val = sParam_->prm.status.isWarning(adr - IE_PRM_WARNING_START);
		}
	} else if (adr >= IE_PRM_ERROR_START) {
		if (adr <= IE_PRM_ERROR_END) {
			val = sParam_->prm.status.isFault(adr - IE_PRM_ERROR_START);
		}
	} else if (adr == IE_PRM_ON) {
		val = sParam_->prm.status.isEnable();
	}

	return val;
}

// Возвращает флаги защиты.
bool TProtocolPcI::getDef(uint16_t adr) const {
	bool val = false;

	if (adr >= IE_DEF_WARNING_START) {
		if (adr <= IE_DEF_WARNING_END) {
			val = sParam_->def.status.isWarning(adr - IE_DEF_WARNING_START);
		}
	} else if (adr >= IE_DEF_ERROR_START) {
		if (adr <= IE_DEF_ERROR_END) {
			val = sParam_->def.status.isFault(adr - IE_DEF_ERROR_START);
		}
	} else if (adr == IE_DEF_ON) {
		val = sParam_->def.status.isEnable();
	}

	return val;
}
