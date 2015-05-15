/*
 * iec_60870_5_101.cpp
 *
 *  Created on: 21.04.2015
 *      Author: Shcheblykin
 */

#include "CIec101.h"

// ������������.
CIec101::CIec101(uint8_t *pBuf, uint8_t u8Size) :
m_pBuf(pBuf), m_u8Size(u8Size) {
	m_eState = STATE_OFF;
	m_u8Address = s_u8AddressErr;
	m_u8Cnt = 0;
	m_u8Func = FUNCTION_RESET_WAIT;

	m_u8Tick = 0;
	m_u16TickStep = 0;

	m_u16CntInterrog = 0;

	m_u8Fcb = false;

#ifndef NDEBUG
	// �������� ������� ���� ����������.
	enum {err_1 = 1 / (!!(sizeof(UControlField) == 1))};

	// �������� ������� ����� � ������������� ������.
	enum {err_2 = 1 / (!!(sizeof(SFrameFixLength) == s_u8SizeOfFrameFixLenght))};

	// �������� ������� ����� �������.
	enum {err_3 = 1 / (!!(sizeof(SCp56Time2a) == 7))};

	// �������� ������� �������������� ����� ������.
	enum {err_4 = 1 / (!!(sizeof(SDataUnitId) == 4))};

	// �������� ������� ������� �������������
	enum {err_5 = 1 / (!!(sizeof(SCoi) == 1))};

	// �������� ������� ��������� �������������.
	enum {err_6 = 1 / (!!(sizeof(SMEiNa1) == 7))};
#endif
}

// ����� ��������� ������ ���������.
void CIec101::setState(CIec101::EState state) {
	if ((state >= STATE_OFF) && (state < STATE_ERROR)) {
		if (state == STATE_READ) {
			m_u8Cnt = 0;
			m_u8Tick = 0;
		}

		m_eState = state;
	}
}

// �������� ���� ������.
uint8_t CIec101::push(uint8_t u8Byte) {
	uint8_t cnt = m_u8Cnt;

	if (checkState(STATE_READ)) {
		if (cnt < m_u8Size) {
			m_pBuf[cnt++] = u8Byte;
		}
	}
	m_u8Tick = 0;	// ����� �������� �������

	sDebug.byte1 = m_pBuf[0];
	sDebug.byte2 = m_pBuf[1];
	sDebug.byte3 = m_pBuf[2];
	sDebug.byte4 = m_pBuf[3];
	sDebug.byte5 = m_pBuf[4];
	sDebug.byte6 = m_pBuf[5];
	sDebug.byte8++;

	return (m_u8Cnt = cnt);
}

// ��������� �������� �������.
uint16_t CIec101::setTick(uint16_t u16Baudrate, uint8_t u8Period) {
	uint16_t step = (((1UL * u16Baudrate * u8Period) / 1100)* s_u16DelayFinish)
			/ 15000;
	return (m_u16TickStep = step);
}

// ���� ������� ���������� � ������� ������� ���������� �����.
void CIec101::tick() {
	uint16_t tick = m_u8Tick;

	if (checkState(STATE_READ)) {
		tick += m_u16TickStep;
		if (tick >= s_u16DelayFinish) {
			if (m_u8Cnt >= s_u8SizeOfFrameFixLenght) {
				setState(STATE_READ_OK);
			} else {
				setState(STATE_READ);
			}
		}
	} else if (checkState(STATE_READ_ERROR)) {
		tick += m_u16TickStep;
		if (tick >= s_u16DelayFix) {
			setState(STATE_READ);
		}
	}

	sDebug.byte7 = getState();

	m_u8Tick = tick;
}

// ��������� ������ ���������� � ����.
bool CIec101::setAddressLan(uint8_t u8Adr) {
	bool state = false;

	if ((u8Adr >= s_u8AddressMin) && (u8Adr <= s_u8AddressMax)) {
		m_u8Address = u8Adr;
		state = true;
	}

	return state;
}

// �������� ���������.
uint8_t CIec101::sendData() {
	uint8_t cnt = 0;

	if (checkState(STATE_WRITE_READY)) {
		setState(STATE_WRITE);
		cnt = m_u8Cnt;
	}

	return cnt;
}

// ��������� �������� ������.
CIec101::EError CIec101::readData() {
	EError error = ERROR_COMMON;

	switch (static_cast<EFrameStartCharacter>(m_pBuf[0])) {
		case FRAME_START_CHARACTER_FIX: {
			SFrameFixLength &frame = *((SFrameFixLength *) m_pBuf);
			error = checkFrameFixLenght(frame);
			if (error == ERROR_NO) {
				readFrameFixLenght(frame);
			}
		}
		break;
		case FRAME_START_CHARACTER_VAR: {
			SFrameVarLength &frame = *((SFrameVarLength*) m_pBuf);
			error = checkFrameVarLenght(frame);
			if (error == ERROR_NO) {
				readFrameVarLenght(frame);
			}
		}
		break;
	}

	if (error != ERROR_NO) {
		// � ������ ����������� ������, ������� ��������� ����� ����� 33 ����
		setState(STATE_READ_ERROR);
	}

	return error;
}

//	������������� �������.
bool CIec101::getTime(uint8_t *pDist, uint8_t *pSource) {
	if (!isFunc(FUNCTION_TIME_SYNCH_CONF))
		return false;

	*pDist++ = stTime.years;					// ���
	*pDist++ = stTime.months;					// �����
	*pDist++ = stTime.dayOfMonth;				// ���� ������
	*pDist++ = stTime.hours;					// ����
	*pDist++ = stTime.minutes;				// �������
	*pDist++ = stTime.milliseconds / 1000;	// �������

	// ������� ����� �������
	uint8_t *ptr = (uint8_t *) &stTime;
	for(uint8_t i = 0; i < sizeof(SCp56Time2a); i++) {
		*ptr++ = 0;
	}

	pSource++;								// ��, ������� ����
	pSource++;								// ��, ������� ����
	stTime.milliseconds = (*pSource++) * 1000;// �������
	stTime.minutes = *pSource++;				// ������
	stTime.hours = *pSource++;				// ����
	pSource++;								// ���� ������
	stTime.dayOfMonth = *pSource++;			// ���� ������
	stTime.months = *pSource++;				// �����
	stTime.years = *pSource;					// ���

	clrFunc(FUNCTION_TIME_SYNCH_CONF);
	setFunc(FUNCTION_TIME_SYNCH_END);

	return true;
}

//	�������� ������� ������ ������ 1(2) �� ��������.
void CIec101::checkEvent() {
	setFunc(FUNCTION_EVENT);
}

// ���������� �������� � ������� CRC.
uint8_t CIec101::getCrcFixFrame(SFrameFixLength &rFrame) const {
	return rFrame.controlField.common + rFrame.linkAddress;
}

// ������� ����������� ����� ��� ����� � ���������� ������.
uint8_t CIec101::getCrcVarFrame(SFrameVarLength &rFrame) const {
	// ���-�� ���������������� ���� ������, ������� ���� ���������� � �����
	uint8_t len = rFrame.length1;
	uint8_t crc = 0;

	for(uint8_t *ptr = (uint8_t *) &rFrame.controlField; len > 0; len--) {
		crc += *ptr++;
	}

	return crc;
}


// �������� ����� � ������������� ������.
CIec101::EError CIec101::checkFrameFixLenght(SFrameFixLength &rFrame) const {

	if (getNumOfBytes() != s_u8SizeOfFrameFixLenght) {
		return ERROR_SIZE_FRAME;
	}

	if (rFrame.stopCharacter != s_u8FrameStopCharacter) {
		return ERROR_STOP_CHAR;
	}

	if (rFrame.checkSum != getCrcFixFrame(rFrame)) {
		return ERROR_CRC;
	}

	if ((rFrame.linkAddress != getAddressLan() && rFrame.linkAddress != s_u8AddressGlb)) {
		return ERROR_LINK_ADDRESS;
	}

	if (rFrame.controlField.primary.prm != 1) {
		return ERROR_PRM_BITE;
	}

	return ERROR_NO;
}

// �������� ����� � ���������� ������.
CIec101::EError CIec101::checkFrameVarLenght(SFrameVarLength &rFrame)  const {
	uint8_t len = getNumOfBytes();

	if (len < s_u8SizeOfFrameVarLenghtMin)
		return ERROR_SIZE_FRAME;

	if (rFrame.length1 != rFrame.length2) {
		return ERROR_LENGHT2;
	}

	if (rFrame.length1 != (len - 6)) {
		return ERROR_LENGHT1;
	}

	if (rFrame.startCharacter2 != FRAME_START_CHARACTER_VAR) {
		return ERROR_START2_CHAR;
	}

	if ((rFrame.linkAddress != getAddressLan() && rFrame.linkAddress != s_u8AddressGlb)) {
		return ERROR_LINK_ADDRESS;
	}

	if (m_pBuf[len - 1] != s_u8FrameStopCharacter) {
		return ERROR_STOP_CHAR;
	}

	if (m_pBuf[len - 2] != getCrcVarFrame(rFrame)) {
		return ERROR_CRC;
	}

	if (rFrame.controlField.primary.prm != 1) {
		return ERROR_PRM_BITE;
	}

	return ERROR_NO;
}

// ��������� ����� � ������������� ������.
void CIec101::readFrameFixLenght(SFrameFixLength &rFrame) {
	if (isFunc(FUNCTION_RESET_WAIT)) {
		// ���� ������ �� ��������� ������� ������ � ��� �� ������� ������
		// �������� ���� ��������� �� �����
		switch(rFrame.controlField.primary.function) {
			case RESET_REMOTE_LINK:
			case REQUEST_STATUS_OF_LINK:
				break;
			case REQUEST_USER_DATA_CLASS_1:	// DOWN
			case REQUEST_USER_DATA_CLASS_2: // DOWN
			default:
				setReadState();
				return;
		}
	}

	uint8_t fcb = rFrame.controlField.primary.fcb;

	switch(rFrame.controlField.primary.function) {
		case RESET_REMOTE_LINK:
			// � ��������� ����� ������ ���� FCB = 1
			rFrame.controlField.primary.fcb = 0;
			sendFrameFixLenght(CONFIRM_ACK);
			clrFunc(FUNCTION_RESET_WAIT);
			setFunc(FUNCTION_RESET_END);
			break;
		case USER_DATA_NO_REPLY:
			setReadState();
			break;
		case REQUEST_STATUS_OF_LINK:
			sendFrameFixLenght(RESPOND_STATUS_OF_LINK);
			break;
		case REQUEST_USER_DATA_CLASS_1:	// DOWN
		case REQUEST_USER_DATA_CLASS_2:
			procFrameFixLenghtUserData(rFrame);
			break;
		case USER_DATA_CONFIRM:
			// ������������ ������� ��� ����� ������������� ������ - ������� NACK
			sendFrameFixLenght(CONFIRM_NACK);
			break;
		default:
			sendFrameFixLenght(LINK_SERV_NOT_IMPL);
	}

	// � �������� ����� ��� ����� ������ ������ ���� ������
	m_u8Fcb = fcb ^ 1;
}

void CIec101::readFrameVarLenght(SFrameVarLength &rFrame) {
	if (isFunc(FUNCTION_RESET_WAIT)) {
		// ���� ������ �� ��������� ������� ������ �������� ���� ��������� �� �����
		setReadState();
		return;
	}

	uint8_t fcb = rFrame.controlField.primary.fcb;

	switch(rFrame.controlField.primary.function) {
		case USER_DATA_CONFIRM:
			if (!checkFcb(rFrame.controlField.primary)) {
				// ���� ��������� ���������� ����
				sendFrameVarLenght();
			} else {
				// ���������� �������� ���� � �������� ������������� ��������������
				// ��� ������������� �������������
				if (procFrameVarLenght(rFrame.asdu)) {
					sendFrameFixLenght(CONFIRM_ACK);
				} else {
					sendFrameFixLenght(CONFIRM_NACK);
				}
			}
			break;

		case USER_DATA_NO_REPLY:
			procFrameVarLenght(rFrame.asdu);
			setReadState();
			break;

		case RESET_REMOTE_LINK:			// DOWN
		case REQUEST_STATUS_OF_LINK:	// DOWN
		case REQUEST_USER_DATA_CLASS_1:	// DOWN
		case REQUEST_USER_DATA_CLASS_2:
			sendFrameFixLenght(CONFIRM_NACK);
			break;

		default:
			sendFrameFixLenght(LINK_SERV_NOT_IMPL);
	}

	// � �������� ����� ��� ����� ������ ������ ���� ������
	m_u8Fcb = fcb ^ 1;
}

// �������� ����� � ������������� ������.
void CIec101::sendFrameFixLenght(EFcSecondary function) {
	SFrameFixLength &frame = *((SFrameFixLength *) m_pBuf);

	frame.startCharacter = FRAME_START_CHARACTER_FIX;
	frame.controlField.common = 0;
	frame.controlField.secondary.function = function;
//	frame.controlField.secondary.dfc = 0;	// ����� ��������� ������ ��������
	frame.controlField.secondary.acd = isAcd();
//	frame.controlField.secondary.prm = 0;	// ����������� �������� �� ��������� ������� = 0
//	frame.controlField.secondary.res = 0;	// ������ ������ 0
	frame.linkAddress = getAddressLan();
	frame.checkSum = getCrcFixFrame(frame);
	frame.stopCharacter = s_u8FrameStopCharacter;

	m_u8Cnt = s_u8SizeOfFrameFixLenght;
	setState(STATE_WRITE_READY);
}

// �������� ����� � ���������� ������.
void CIec101::sendFrameVarLenght() {
	uint8_t *ptr = (uint8_t *) &m_stFrameVar;
	uint8_t i = 0;

	m_stFrameVar.controlField.secondary.acd = isAcd();

	while(i < (m_stFrameVar.length1 + 4)) {
		m_pBuf[i++] = *ptr++;
	}
	m_pBuf[i++] = getCrcVarFrame(m_stFrameVar);
	m_pBuf[i++] = m_stFrameVar.stopCharacter;

	m_u8Cnt = i;
	setState(STATE_WRITE_READY);
}

// ���������� ����� � ���������� ������ � ��������.
void CIec101::prepareFrameVarLenght(ETypeId eId, ECot eCot, uint8_t u8SizeAsdu) {

	m_stFrameVar.startCharacter = FRAME_START_CHARACTER_VAR;
	m_stFrameVar.length1 = u8SizeAsdu + 2; //������ = ������ ASDU + ������ ControlField � LinkAdres
	m_stFrameVar.length2 = u8SizeAsdu + 2; //������ = ������ ASDU + ������ ControlField � LinkAdres
	m_stFrameVar.startCharacter2 = FRAME_START_CHARACTER_VAR;

	m_stFrameVar.controlField.common = 0;
	m_stFrameVar.controlField.secondary.function = RESPOND_USER_DATA;
	// Acd ����� �������� �������, ����� ��������� �����
	//	frame.controlField.secondary.dfc = 0;		// ����� ��������� ������ ��������
//	m_stFrameVar.controlField.secondary.acd = isAcd();	// ��������� ��� ��������
	//	frame.controlField.secondary.prm = 0;		// ����������� �������� �� ��������� ������� = 0
	//	frame.controlField.secondary.res = 0;		// ������ ������ 0

	m_stFrameVar.linkAddress = getAddressLan();

	m_stFrameVar.asdu.common.dataUnitId.typeId = eId;
	m_stFrameVar.asdu.common.dataUnitId.variableStructureQualifier = s_u8VariableStructureQualifier; // ������ ���������� 1 ������ ����������
	m_stFrameVar.asdu.common.dataUnitId.causeOfTramsmission.cot = eCot;	// �������
	m_stFrameVar.asdu.common.dataUnitId.causeOfTramsmission.pN = 0; 	// �� ������������
	m_stFrameVar.asdu.common.dataUnitId.causeOfTramsmission.t = 0;		// ������ �� ����
	m_stFrameVar.asdu.common.dataUnitId.commonAddressOfASDU = s_u8CommonAddressOfAsdu;

	// ��������� ������ ASDU ��� ������ ���� �� ������.

//	m_stFrameVar.checkSum =  getCrcVarFrame(m_stFrameVar);
	m_stFrameVar.stopCharacter = s_u8FrameStopCharacter;
}

//���������� ����� ��������������� ���������� ��� ����� �������.
void CIec101::prepareFrameMSpNa1(uint16_t u16Address, ECot eCause, bool bVal) {
	*((uint8_t *) &m_stFrameVar.asdu.mSpNa1.siq) = 0;
	m_stFrameVar.asdu.mSpNa1.siq.spi = bVal ? 1 : 0;
	m_stFrameVar.asdu.mSpNa1.informationObjectAddress = u16Address;

	prepareFrameVarLenght(TYPE_ID_M_SP_NA_1, eCause, sizeof(SMSpNa1));
}

// ���������� ����� ��������������� ���������� � ������ �������.
void CIec101::prepareFrameMSpTb1(uint16_t u16Address, ECot eCause,
		bool bVal, SCp56Time2a &rTime) {
	*((uint8_t *) &m_stFrameVar.asdu.mSpTb1.siq) = 0;
	m_stFrameVar.asdu.mSpNa1.siq.spi = bVal ? 1 : 0;
	m_stFrameVar.asdu.mSpNa1.informationObjectAddress = u16Address;
	copyCp56time2a(m_stFrameVar.asdu.mSpTb1.cp56time2a, rTime);

	prepareFrameVarLenght(TYPE_ID_M_SP_TB_1, eCause, sizeof(SMSpTb1));
}

// ���������� ����� � �������� ��������� �������������.
void CIec101::prepareFrameMEiNa1(ECoi eCause) {
	m_stFrameVar.asdu.mEiNa1.coi.init = 0;
	m_stFrameVar.asdu.mEiNa1.coi.cause = eCause;
	m_stFrameVar.asdu.mEiNa1.informationObjectAddress = 0;

	prepareFrameVarLenght(TYPE_ID_M_EI_NA_1, COT_INIT, sizeof(SMEiNa1));
}

//	���������� ����� � �������� ������.
void CIec101::prepareFrameCIcNa1(ECot eCause) {
	m_stFrameVar.asdu.cIcNa1.qoi.qoi = QOI_NOT_USED;
	m_stFrameVar.asdu.cIcNa1.informationObjectAddress = 0;

	prepareFrameVarLenght(TYPE_ID_C_IC_NA_1, eCause, sizeof(SCIcNa1));
}

//	���������� ����� � �������� ������������� ������� (���������).
void CIec101::prepareFrameCCsNa1(ECot eCause) {
	m_stFrameVar.asdu.cCsNa1.informationObjectAddress = 0;
	copyCp56time2a(m_stFrameVar.asdu.cCsNa1.cp56Time2a, stTime);

	prepareFrameVarLenght(TYPE_ID_C_CS_NA_1, eCause, sizeof(SCCsNa1));
}

// ��������� ����� ������������� ����� � �������� ������� ������ ������ 1 � 2.
void CIec101::procFrameFixLenghtUserData(SFrameFixLength &rFrame) {

	// �������� �� �������� ����� ��������� ������.
	if (isFunc(FUNCTION_RESET_END)) {
		// ���� fcb ������ � ��������� 1, �� �������� ����� �� ���������
		// �������������, ����� ������ ����� ��� ������ ����������.
		if (rFrame.controlField.primary.fcb == 1) {
			// ��������� �������������
			clrFunc(FUNCTION_RESET_END);
			prepareFrameMEiNa1(COI_LOCAL_MANUAL_RESET);
			sendFrameVarLenght();
		} else {
			sendFrameFixLenght(RESPOND_NACK);
		}
		return;
	}

	// �������� �� ������ �������� �����.
	if (!checkFcb(rFrame.controlField.primary)) {
		sendFrameVarLenght();
		return;
	}

	// �������� �� �������� ����� ��������� ������������� �������.
	if (isFunc(FUNCTION_TIME_SYNCH_END)) {
		clrFunc(FUNCTION_TIME_SYNCH_END);
		prepareFrameCCsNa1(COT_ACTCON);
		sendFrameVarLenght();
		return;
	}

	// �������� �� ������������� ������ ������
	if (isFunc(FUNCTION_INTERROG_CONF)) {
		clrFunc(FUNCTION_INTERROG_CONF);
		setFunc(FUNCTION_INTERROG_MONIT);
		prepareFrameCIcNa1(COT_ACTCON);
		sendFrameVarLenght();
		return;
	}

	// �������� �� �������� ������ ������
	if (isFunc(FUNCTION_INTERROG_MONIT)) {
		if (!procInterrog()) {
			// ������ �� �������� ������ ���, �������� �����.
			clrFunc(FUNCTION_INTERROG_MONIT);
			prepareFrameCIcNa1(COT_ACTTERM);
		}
		sendFrameVarLenght();
		return;
	}

	// �������� �� �������� ������ ������ 1 � 2
	if (isFunc(FUNCTION_EVENT)) {
		if (!procEvent()) {
			clrFunc(FUNCTION_EVENT);
		}
		sendFrameVarLenght();
		return;
	}

	// ��� ������ �� ��������.
	sendFrameFixLenght(RESPOND_NACK);
}

// ��������� ��������� ����� � ���������� ������.
bool CIec101::procFrameVarLenght(UAsdu asdu) {
	switch(asdu.common.dataUnitId.typeId) {
		case TYPE_ID_C_IC_NA_1:	// ������� ������.
			// TODO ��������� ����� qoi �������� �� ������� � ������� �������� �� ���� ?!
			return procFrameCIcNa1(asdu.cIcNa1);
			break;
		case TYPE_ID_C_CS_NA_1:	// ������� ������������� �����.
			return procFrameCCsNa1(asdu.cCsNa1);
			break;
		case TYPE_ID_M_EI_NA_1:	// DOWN
		case TYPE_ID_M_SP_NA_1:	// DOWN
		case TYPE_ID_M_SP_TB_1:
			// ������� �� �������������� � ����� � ���������� ������
			break;
	}

	return false;
}

// �������� �������.
bool CIec101::procEvent(void) {
	static uint8_t cnt = 0;

	bool state = true;
	SCp56Time2a time;
	bool val = false;
	uint16_t adr = 0;

	if (cnt < 4) {
		adr = 254 + cnt;
		val = (cnt != 0);
		writeCp56Time2a(time, 12 + cnt, 5 + cnt, 14 + cnt, 8 + cnt, 23 +cnt,
				16 + cnt, 98 + cnt);

		prepareFrameMSpTb1(adr, COT_SPONT, val, time);
		cnt++;
	}

	if (cnt >= 4) {
		cnt = 0;
		state = false;
	}

	return state;
}

// ��������� ������ �� ������� ������.
bool CIec101::procInterrog(void) {
	bool state = true;
	uint16_t cnt = m_u16CntInterrog + 1;
	bool val = false;

	if (cnt == 1) {
		m_u16CntInterrog = 200;
		cnt = 201;
	}

	if ((cnt >= 201) && (cnt <= 204)) {
		if (cnt == 201) {
			val = false;
		} else if (cnt == 202) {
			val = false;
		} else if (cnt == 203) {
			val = true;
		} else if (cnt == 204) {
			val = true;
		}
	} else {
		state = false;
	}

	if (state) {
		// ��� ����������� �������� ����� + 1, �.�. � cnt ����� ������
		// ��������� �����
		prepareFrameMSpNa1(m_u16CntInterrog + 1, COT_INROGEN, val);
	}

	m_u16CntInterrog = cnt;

	return state;
}

//	��������� ��������� ����� ������������� �����.
bool CIec101::procFrameCCsNa1(SCCsNa1 stCCsNa1) {

	if (stCCsNa1.dataUnitId.causeOfTramsmission.cot != COT_ACT)
		return false;

	setFunc(FUNCTION_TIME_SYNCH_CONF);
	copyCp56time2a(stTime, stCCsNa1.cp56Time2a);

	return true;
}

//	��������� ��������� ����� ������.
bool CIec101::procFrameCIcNa1(SCIcNa1 stCIcNa1) {

	if (stCIcNa1.dataUnitId.causeOfTramsmission.cot != COT_ACT)
		return false;

	m_u16CntInterrog = 0;	// ����� �������� ������
	setFunc(FUNCTION_INTERROG_CONF);

	return true;
}

// ����������� ����� ������� Cp56Time2a.
void CIec101::copyCp56time2a(SCp56Time2a &rDist, const SCp56Time2a &rSource) {
	// ��������������� �������
	uint8_t *ptr = (uint8_t *) &rDist;
	for(uint8_t i = 0; i < sizeof(SCp56Time2a); i++) {
		*ptr++ = 0;
	}
	// ����������� ����������� ������
	rDist.milliseconds = rSource.milliseconds;
	rDist.minutes = rSource.minutes;
	rDist.hours = rSource.hours;
	rDist.dayOfMonth = rSource.dayOfMonth;
	rDist.months = rSource.months;
	rDist.years = rSource.years;
}

// ���������� ����� ������� Cp56Time2a.
void CIec101::writeCp56Time2a(SCp56Time2a &rTime, uint8_t years, uint8_t months,
			uint8_t day, uint8_t hours, uint8_t min, uint8_t sec, uint16_t msec) {
	// ������� �����
	uint8_t *ptr = (uint8_t *) &rTime;
	for(uint8_t i = 0; i < sizeof(SCp56Time2a); i++) {
		*ptr++ = 0;
	}

	rTime.milliseconds = msec + sec * 1000;
	rTime.minutes = min;
	rTime.hours = hours;
	rTime.dayOfMonth = day;
	rTime.months = months;
	rTime.years = years;

}
