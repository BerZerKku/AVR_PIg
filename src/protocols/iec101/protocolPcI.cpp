/*
 * protocolPcI.cpp
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#include "protocolPcI.h"


// Адреса элементов информации (EInfoElement)
const uint16_t TProtocolPcI::c_adrIE2[] PROGMEM = {
		201,	// IE_ERROR
		202, 	// IE_WARNING
		203, 	// IE_PRD_COM
		204,	// IE_PRM_COM
		301,	// IE_GLB_ERROR_H0001
		302,	// IE_GLB_ERROR_H0002
		303,	// IE_GLB_ERROR_H0004
		304,	// IE_GLB_ERROR_H0008
		305,	// IE_GLB_ERROR_H0010
		306,	// IE_GLB_ERROR_H0020
		307,	// IE_GLB_ERROR_H0040
		308,	// IE_GLB_ERROR_H0080
		309,	// IE_GLB_ERROR_H0100
		310,	// IE_GLB_ERROR_H0200
		311,	// IE_GLB_ERROR_H0400
		312,	// IE_GLB_ERROR_H0800
		313,	// IE_GLB_ERROR_H1000
		314,	// IE_GLB_ERROR_H2000
		315,	// IE_GLB_ERROR_H4000
		316,	// IE_GLB_ERROR_H8000
		317,	// IE_GLB_WARNING_H0001
		318,	// IE_GLB_WARNING_H0002
		319,	// IE_GLB_WARNING_H0004
		320,	// IE_GLB_WARNING_H0008
		321,	// IE_GLB_WARNING_H0010
		322,	// IE_GLB_WARNING_H0020
		323,	// IE_GLB_WARNING_H0040
		324,	// IE_GLB_WARNING_H0080
		325,	// IE_GLB_WARNING_H0100
		326,	// IE_GLB_WARNING_H0200
		327,	// IE_GLB_WARNING_H0400
		328,	// IE_GLB_WARNING_H0800
		329,	// IE_GLB_WARNING_H1000
		330,	// IE_GLB_WARNING_H2000
		331,	// IE_GLB_WARNING_H4000
		332,	// IE_GLB_WARNING_H8000
		400,	// IE_PRD_ON
		401,	// IE_PRD_ERROR_H0001
		402,	// IE_PRD_ERROR_H0002
		403,	// IE_PRD_ERROR_H0004
		404,	// IE_PRD_ERROR_H0008
		405,	// IE_PRD_ERROR_H0010
		406,	// IE_PRD_ERROR_H0020
		407,	// IE_PRD_ERROR_H0040
		408,	// IE_PRD_ERROR_H0080
		409,	// IE_PRD_ERROR_H0100
		410,	// IE_PRD_ERROR_H0200
		411,	// IE_PRD_ERROR_H0400
		412,	// IE_PRD_ERROR_H0800
		413,	// IE_PRD_ERROR_H1000
		414,	// IE_PRD_ERROR_H2000
		415,	// IE_PRD_ERROR_H4000
		416,	// IE_PRD_ERROR_H8000
		417,	// IE_PRD_WARNING_H0001
		418,	// IE_PRD_WARNING_H0002
		419,	// IE_PRD_WARNING_H0004
		420,	// IE_PRD_WARNING_H0008
		421,	// IE_PRD_WARNING_H0010
		422,	// IE_PRD_WARNING_H0020
		423,	// IE_PRD_WARNING_H0040
		424,	// IE_PRD_WARNING_H0080
		425,	// IE_PRD_WARNING_H0100
		426,	// IE_PRD_WARNING_H0200
		427,	// IE_PRD_WARNING_H0400
		428,	// IE_PRD_WARNING_H0800
		429,	// IE_PRD_WARNING_H1000
		430,	// IE_PRD_WARNING_H2000
		431,	// IE_PRD_WARNING_H4000
		432,	// IE_PRD_WARNING_H8000
		450,	// IE_PRD_COM_01
		451,	// IE_PRD_COM_02
		452,	// IE_PRD_COM_03
		453,	// IE_PRD_COM_04
		454,	// IE_PRD_COM_05
		455,	// IE_PRD_COM_06
		456,	// IE_PRD_COM_07
		457,	// IE_PRD_COM_08
		458,	// IE_PRD_COM_09
		459,	// IE_PRD_COM_10
		460,	// IE_PRD_COM_11
		461,	// IE_PRD_COM_12
		462,	// IE_PRD_COM_13
		463,	// IE_PRD_COM_14
		464,	// IE_PRD_COM_15
		465,	// IE_PRD_COM_16
		466,	// IE_PRD_COM_17
		467,	// IE_PRD_COM_18
		468,	// IE_PRD_COM_19
		469,	// IE_PRD_COM_20
		470,	// IE_PRD_COM_21
		471,	// IE_PRD_COM_22
		472,	// IE_PRD_COM_23
		473,	// IE_PRD_COM_24
		474,	// IE_PRD_COM_25
		475,	// IE_PRD_COM_26
		476,	// IE_PRD_COM_27
		477,	// IE_PRD_COM_28
		478,	// IE_PRD_COM_29
		479,	// IE_PRD_COM_30
		480,	// IE_PRD_COM_31
		481,	// IE_PRD_COM_32
		485,	// IE_PRD_DISABLED
		486,	// IE_PRD_ENABLED
		487,	// IE_PRD_TEST
		500,	// IE_PRM_ON
		501,	// IE_PRM_ERROR_H0001
		502,	// IE_PRM_ERROR_H0002
		503,	// IE_PRM_ERROR_H0004
		504,	// IE_PRM_ERROR_H0008
		505,	// IE_PRM_ERROR_H0010
		506,	// IE_PRM_ERROR_H0020
		507,	// IE_PRM_ERROR_H0040
		508,	// IE_PRM_ERROR_H0080
		509,	// IE_PRM_ERROR_H0100
		510,	// IE_PRM_ERROR_H0200
		511,	// IE_PRM_ERROR_H0400
		512,	// IE_PRM_ERROR_H0800
		513,	// IE_PRM_ERROR_H1000
		514,	// IE_PRM_ERROR_H2000
		515,	// IE_PRM_ERROR_H4000
		516,	// IE_PRM_ERROR_H8000
		517,	// IE_PRM_WARNING_H0001
		518,	// IE_PRM_WARNING_H0002
		519,	// IE_PRM_WARNING_H0004
		520,	// IE_PRM_WARNING_H0008
		521,	// IE_PRM_WARNING_H0010
		522,	// IE_PRM_WARNING_H0020
		523,	// IE_PRM_WARNING_H0040
		524,	// IE_PRM_WARNING_H0080
		525,	// IE_PRM_WARNING_H0100
		526,	// IE_PRM_WARNING_H0200
		527,	// IE_PRM_WARNING_H0400
		528,	// IE_PRM_WARNING_H0800
		529,	// IE_PRM_WARNING_H1000
		530,	// IE_PRM_WARNING_H2000
		531,	// IE_PRM_WARNING_H4000
		532,	// IE_PRM_WARNING_H8000
		550,	// IE_PRM_COM_01
		551,	// IE_PRM_COM_02
		552,	// IE_PRM_COM_03
		553,	// IE_PRM_COM_04
		554,	// IE_PRM_COM_05
		555,	// IE_PRM_COM_06
		556,	// IE_PRM_COM_07
		557,	// IE_PRM_COM_08
		558,	// IE_PRM_COM_09
		559,	// IE_PRM_COM_10
		560,	// IE_PRM_COM_11
		561,	// IE_PRM_COM_12
		562,	// IE_PRM_COM_13
		563,	// IE_PRM_COM_14
		564,	// IE_PRM_COM_15
		565,	// IE_PRM_COM_16
		566,	// IE_PRM_COM_17
		567,	// IE_PRM_COM_18
		568,	// IE_PRM_COM_19
		569,	// IE_PRM_COM_20
		570,	// IE_PRM_COM_21
		571,	// IE_PRM_COM_22
		572,	// IE_PRM_COM_23
		573,	// IE_PRM_COM_24
		574,	// IE_PRM_COM_25
		575,	// IE_PRM_COM_26
		576,	// IE_PRM_COM_27
		577,	// IE_PRM_COM_28
		578,	// IE_PRM_COM_29
		579,	// IE_PRM_COM_30
		580,	// IE_PRM_COM_31
		581,	// IE_PRM_COM_32
		585,	// IE_PRM_DISABLED
		586,	// IE_PRM_READY
		587,	// IE_PRM_ENABLED
		588,	// IE_PRM_TEST
		600,	// IE_DEF_ON
		601,	// IE_DEF_ERROR_H0001
		602,	// IE_DEF_ERROR_H0002
		603,	// IE_DEF_ERROR_H0004
		604,	// IE_DEF_ERROR_H0008
		605,	// IE_DEF_ERROR_H0010
		606,	// IE_DEF_ERROR_H0020
		607,	// IE_DEF_ERROR_H0040
		608,	// IE_DEF_ERROR_H0080
		609,	// IE_DEF_ERROR_H0100
		610,	// IE_DEF_ERROR_H0200
		611,	// IE_DEF_ERROR_H0400
		612,	// IE_DEF_ERROR_H0800
		613,	// IE_DEF_ERROR_H1000
		614,	// IE_DEF_ERROR_H2000
		615,	// IE_DEF_ERROR_H4000
		616,	// IE_DEF_ERROR_H8000
		617,	// IE_DEF_WARNING_H0001
		618,	// IE_DEF_WARNING_H0002
		619,	// IE_DEF_WARNING_H0004
		620,	// IE_DEF_WARNING_H0008
		621,	// IE_DEF_WARNING_H0010
		622,	// IE_DEF_WARNING_H0020
		623,	// IE_DEF_WARNING_H0040
		624,	// IE_DEF_WARNING_H0080
		625,	// IE_DEF_WARNING_H0100
		626,	// IE_DEF_WARNING_H0200
		627,	// IE_DEF_WARNING_H0400
		628,	// IE_DEF_WARNING_H0800
		629,	// IE_DEF_WARNING_H1000
		630,	// IE_DEF_WARNING_H2000
		631,	// IE_DEF_WARNING_H4000
		632		// IE_DEF_WARNING_H8000
};

// Конструктор
TProtocolPcI::TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), CIec101(buf, size) {

	COMPILE_TIME_ASSERT(SIZE_OF(c_adrIE2) == IE2_MAX);

	for(uint16_t i = 0; i < IE2_MAX; i++) {
		m_flags[i] = getValue(static_cast<EInfoElement2> (i));
	}
}

// Функция отправки сообщения.
uint8_t TProtocolPcI::send() {
	return sendData();
}

// Проверка наличия данных класса 1(2) на передачу.
bool TProtocolPcI::checkEventClass1() {

	if ((!ei1.send) && (sParam_->jrnScada.isReadyToSend())) {
		TJrnSCADA *jrn = &sParam_->jrnScada;

		if (jrn->isJrnEvent()) {
			ei1.val = true;
			ei1.adr = c_adrIe1Event1 + jrn->getEvent();
			ei1.send = true;
		} else if (jrn->isJrnPrm()) {
			ei1.val = jrn->getEvent();
			ei1.adr = c_adrIe1PrmCom1 + jrn->getCom() - 1;
			ei1.send = true;
		} else if (jrn->isJrnPrd()) {
			// TODO разделение команд на ДС и ЦПП
			ei1.val = jrn->getEvent();
			ei1.adr = c_adrIe1PrdCom1 + jrn->getCom() - 1;
			ei1.send = true;
		} else {
			// в случае ошибочного журнала, перейдем к следующему событию
			sParam_->jrnScada.setReadyToEvent();
		}

		ei1.time.years 			= jrn->dtime.getYear();
		ei1.time.months 		= jrn->dtime.getMonth();
		ei1.time.dayOfMonth 	= jrn->dtime.getDay();
		ei1.time.hours 			= jrn->dtime.getHour();
		ei1.time.minutes 		= jrn->dtime.getMinute();
		ei1.time.milliseconds 	= jrn->dtime.getSecond() * 1000;
		ei1.time.milliseconds  += jrn->dtime.getMsSecond();
	}
//	else if (!ei2.send) {
//		for(uint8_t i = 0; i < IE2_MAX; i++) {
//			bool val = getValue(static_cast<EInfoElement2> (i));
//
//			if (m_flags[i] != val) {
//				m_flags[i] = val;
//
//				ei2.val = val;
//				ei2.adr = pgm_read_word(&c_adrIE2[i]);
//
//				ei2.time.years 			= sParam_->DateTime.getYear();
//				ei2.time.months 		= sParam_->DateTime.getMonth();
//				ei2.time.dayOfMonth 	= sParam_->DateTime.getDay();
//				ei2.time.hours 			= sParam_->DateTime.getHour();
//				ei2.time.minutes 		= sParam_->DateTime.getMinute();
//				ei2.time.milliseconds 	= sParam_->DateTime.getSecond()*1000;
//				ei2.time.milliseconds  += sParam_->DateTime.getMsSecond();
//
//				ei2.send = true;
//				break;
//			}
//		}
//	}

	return (ei1.send);
}

// Отправка события.
bool TProtocolPcI::procEventClass2(void) {

	for(uint8_t i = 0; i < IE2_MAX; i++) {
		bool val = getValue(static_cast<EInfoElement2> (i));

		if (m_flags[i] != val) {
			m_flags[i] = val;

			uint16_t adr = pgm_read_word(&c_adrIE2[i]);
			SCp56Time2a time;

			time.years 			= sParam_->DateTime.getYear();
			time.months 		= sParam_->DateTime.getMonth();
			time.dayOfMonth 	= sParam_->DateTime.getDay();
			time.hours 			= sParam_->DateTime.getHour();
			time.minutes 		= sParam_->DateTime.getMinute();
			time.milliseconds 	= sParam_->DateTime.getSecond()*1000;
			time.milliseconds  += sParam_->DateTime.getMsSecond();

			prepareFrameMSpTb1(adr, COT_SPONT, val, time);
			return true;
		}
	}

	return false;

//	if (ei1.send) {
//		prepareFrameMSpTb1(ei1.adr, COT_SPONT, ei1.val, ei1.time);
//		sParam_->jrnScada.setReadyToEvent();
//		ei1.send = false;
//	} else if (ei2.send) {
//		prepareFrameMSpTb1(ei2.adr, COT_SPONT, ei2.val, ei2.time);
//		ei2.send = false;
//	}
}

// Обработка ответа на команду опроса.
bool TProtocolPcI::procInterrog(uint16_t &adr, bool &val) {
	static EInfoElement2 e = IE2_ERROR;

	if (e >= IE2_MAX) {
		e = IE2_ERROR;
		return false;
	}

	adr = pgm_read_word(&c_adrIE2[e]);
	val = getValue(e);

	e = static_cast<EInfoElement2>(e + 1);

	return true;
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

// Возвращает состояние элемента информации.
bool TProtocolPcI::getValue(EInfoElement2 ei) const {
	bool val = false;

	if (ei >= IE2_DEF_ON) {
		val = getDef(ei);
	} else if (ei >= IE2_PRM_ON) {
		val = getPrm(ei);
	} else if (ei >= IE2_PRD_ON) {
		val = getPrd(ei);
	} else if (ei >= IE2_GLB_ERROR_H0001) {
		val = getGlb(ei);
	} else if (ei >= IE2_ERROR) {
		val = getDevice(ei);
	}

	return val;
}

// Возвращает состояние флага информации аппарата.
bool TProtocolPcI::getDevice(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_ERROR) {
		val = sParam_->glb.status.isFault();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isFault();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isFault();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isFault();
	} else if (ei == IE2_WARNING) {
		val = sParam_->glb.status.isWarning();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isWarning();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isWarning();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isWarning();
	} else if (ei == IE2_PRD_COM) {
		val = sParam_->prd.isIndCom();
	} else if (ei == IE2_PRM_COM) {
		val = sParam_->prm.isIndCom();
	}

	return val;
}

// Возвращает состояние флага общей информации.
bool TProtocolPcI::getGlb(EInfoElement2 ei) const {
	bool val = false;

	if (ei >= IE2_GLB_WARNING_H0001) {
		if (ei <= IE2_GLB_WARNING_H8000) {
			val = sParam_->glb.status.isWarning(ei - IE2_GLB_WARNING_H0001);
		}
	} else if (ei >= IE2_GLB_ERROR_H0001) {
		if (ei <= IE2_GLB_ERROR_H8000) {
			val = sParam_->glb.status.isFault(ei - IE2_GLB_ERROR_H0001);
		}
	}

	return val;
}

// Возвращает состояние флага передатчика.
bool TProtocolPcI::getPrd(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_PRD_TEST) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prd.status.getRegime() == GB_REGIME_TEST_2);
	} else if (ei == IE2_PRD_ENABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_ENABLED);
	} else if (ei == IE2_PRD_DISABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_DISABLED);
	} if ((ei >= IE2_PRD_COM_01) && (ei <= IE2_PRD_COM_32)) {
			val = sParam_->prd.getIndCom(ei - IE2_PRD_COM_01);
	} else if ((ei >= IE2_PRD_WARNING_H0001) && (ei <= IE2_PRD_WARNING_H8000)) {
			val = sParam_->prd.status.isWarning(ei - IE2_PRD_WARNING_H0001);
	} else if ((ei >= IE2_PRD_ERROR_H0001) && (ei <= IE2_PRD_ERROR_H8000)) {
			val = sParam_->prd.status.isFault(ei - IE2_PRD_ERROR_H0001);
	} else if (ei == IE2_PRD_ON) {
		val = sParam_->prd.status.isEnable();
	}

	return val;
}

// Возвращает состояние флага приемника.
bool TProtocolPcI::getPrm(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_PRM_TEST) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prm.status.getRegime() == GB_REGIME_TEST_2);
	} else if (ei == IE2_PRM_ENABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_ENABLED);
	} else if (ei == IE2_PRM_READY) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_READY);
	} else if (ei == IE2_PRM_DISABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_DISABLED);
	} else if ((ei >= IE2_PRM_COM_01) && (ei <= IE2_PRM_COM_32)) {
			val = sParam_->prm.getIndCom(ei - IE2_PRM_COM_01);
	} else if ((ei >= IE2_PRM_WARNING_H0001) && (ei <= IE2_PRM_WARNING_H8000)) {
			val = sParam_->prm.status.isWarning(ei - IE2_PRM_WARNING_H0001);
	} else if ((ei >= IE2_PRM_ERROR_H0001) && (ei <= IE2_PRM_ERROR_H8000)) {
			val = sParam_->prm.status.isFault(ei - IE2_PRM_ERROR_H0001);
	} else if (ei == IE2_PRM_ON) {
		val = sParam_->prm.status.isEnable();
	}

	return val;
}

// Возвращает состояние флага защиты.
bool TProtocolPcI::getDef(EInfoElement2 ei) const {
	bool val = false;

	if ((ei >= IE2_DEF_WARNING_H0001) && (ei <= IE2_DEF_WARNING_H8000)) {
			val = sParam_->def.status.isWarning(ei - IE2_DEF_WARNING_H0001);
	} else if ((ei >= IE2_DEF_ERROR_H0001) && (ei <= IE2_DEF_ERROR_H8000)) {
			val = sParam_->def.status.isFault(ei - IE2_DEF_ERROR_H0001);
	} else if (ei == IE2_DEF_ON) {
		val = sParam_->def.status.isEnable();
	}

	return val;
}
