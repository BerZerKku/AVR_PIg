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
	// NONE
}

// Функция отправки сообщения.
uint8_t TProtocolPcI::send() {
	return sendData();
}

// Проверка наличия данных класса 1(2) на передачу.
bool TProtocolPcI::checkEvent() {
	SNumEntries *ptr = &sParam_->jrnEntry.m_stNumEntries;

	if (sParam_->jrnEntry.val) {
		uint8_t years = sParam_->jrnEntry.dateTime.getYear();
		uint8_t months = sParam_->jrnEntry.dateTime.getMonth();
		uint8_t day = sParam_->jrnEntry.dateTime.getDay();
		uint8_t hours = sParam_->jrnEntry.dateTime.getHour();
		uint8_t minutes = sParam_->jrnEntry.dateTime.getMinute();
		uint8_t seconds = 1000 * sParam_->jrnEntry.dateTime.getSecond();
		uint16_t ms =	sParam_->jrnEntry.dateTime.getMsSecond();


		SCp56Time2a time;
		writeCp56Time2a(time, years, months, day, hours, minutes, seconds, ms);

		uint16_t adr = sParam_->jrnEntry.getCurrentEntry();

		prepareFrameMSpTb1(adr, COT_SPONT, val, time);


		sParam_->jrnEntry.val = false;
	}

	// проверка на необходимость считать запись журнала
	if (ptr->numGlbPwr != ptr->numGlbTr) {

	} else if (ptr->numDefPwr != ptr->numDefTr) {

	} else if (ptr->numPrmPwr != ptr->numPrmTr) {

	} else if (ptr->numPrdPwr != ptr->numPrdTr) {

	}

	return false;
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

	if (sParam_->prd.status.isEnable() && (adr <= IE_PRD_COM_END)) {
		if (adr < IE_PRD_ON)
			adr = IE_PRD_ON;
		val = getPrd(adr);
		return true;
	}

	if (sParam_->prm.status.isEnable() && (adr <= IE_PRM_COM_END)) {
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
	sParam_->txComBuf.addFastCom(GB_COM_SET_TIME);

	// сброс флага наличия ответа на команду установки времени в БСП
	sParam_->DateTimeReq.setTimeBsp_ = false;

	uint16_t d = getDelay();

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

	if (adr >= IE_PRD_COM_START) {
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

	if (adr >= IE_PRM_COM_START) {
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
