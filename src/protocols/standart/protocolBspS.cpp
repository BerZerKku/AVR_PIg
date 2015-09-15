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

/**	��������� ��������� ���������.
 *
 * 	@param pc True - ������� ����������� � ��, False - ����������� � ��-���.
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getData(bool pc) {
	bool stat = false;
	uint8_t mask = 0;
	eGB_COM com = (eGB_COM) buf[2];

	// ��������� ����������, �������� ���� �� ������
	stat_ = PRTS_STATUS_NO;

	mask = com & GB_COM_MASK_GROUP;
	// ����� �� ������� ��������� ���������/������ �� ���������
	if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM)
			|| (mask == GB_COM_MASK_GROUP_WRITE_REGIME)) {
		if (com == GB_COM_SET_TIME) {
			if (buf[NUM] >= 8) {
				stat =  sParam_->DateTimeReq.setYear(BCD_TO_BIN(buf[B1]));
				stat &= sParam_->DateTimeReq.setMonth(BCD_TO_BIN(buf[B2]));
				stat &= sParam_->DateTimeReq.setDay(BCD_TO_BIN(buf[B3]));
				stat &= sParam_->DateTimeReq.setHour(BCD_TO_BIN(buf[B4]));
				stat &= sParam_->DateTimeReq.setMinute(BCD_TO_BIN(buf[B5]));
				stat &= sParam_->DateTimeReq.setSecond(BCD_TO_BIN(buf[B6]));
				// ������������ ���������������, ������ ���� ��� ���� � �������
				uint16_t ms =  (buf[NUM] >= 8) ? *((uint16_t *) &buf[B7]) : 0;
				stat &= sParam_->DateTimeReq.setMsSecond(ms);
				sParam_->DateTimeReq.setTimeBsp_ = true;
			} else {
				stat = true;
			}
		} else {
			stat = true;
		}
	} else {
		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_DEF)
			stat = getDefCommand(com, pc);				// ������� ������
		else if (mask == GB_COM_MASK_DEVICE_PRM)
			stat = getPrmCommand(com, pc);				// ������� ���������
		else if (mask == GB_COM_MASK_DEVICE_PRD)
			stat = getPrdCommand(com, pc);				// ������� �����������
		else
			stat = getGlbCommand(com, pc);				// ������� �����

		LocalParams *lp = &sParam_->local;

		if (com == lp->getCom()) {
			// �� ��������� ����������� �������� ������� �����,
			// �� �������� �� ����� ��������� ����� ������� ��������
			int16_t val = -1000;
			switch(lp->getParam()) {
				case GB_PARAM_IN_DEC:
					val = buf[B2 + lp->getNumOfCurrSameParam() - 1];
					break;
				case GB_PARAM_FREQ:
					val = TO_INT16(buf[B1], buf[B2]);
					break;
				case GB_PARAM_DETECTOR:
					val = buf[B2];
					break;
				case GB_PARAM_COR_U:
					val = ((int8_t) buf[B1])*10;
					val += ((int8_t) buf[B2])/10;
					break;
				case GB_PARAM_COR_I:
					val = TO_INT16(buf[B3], buf[B4]);
					break;
				case GB_PARAM_PVZUE_PROTOCOL:
					val = buf[B1];
					break;
				case GB_PARAM_PVZUE_PARITY:
					val = buf[B2];
					break;
				case GB_PARAM_PVZUE_FAIL:
					val = buf[B3];
					break;
				case GB_PARAM_PVZUE_NOISE_THD:
					val = buf[B4];
					break;
				case GB_PARAM_PVZUE_NOISE_LVL:
					val = buf[B5];
					break;
				case GB_PARAM_PVZUE_AC_TYPE:
					val = buf[B6];
					break;
				case GB_PARAM_PVZUE_AC_PERIOD:
					val = buf[B7];
					break;
				case GB_PARAM_PVZUE_AC_PER_RE:
					val = buf[B8];
					break;
				case GB_PARAM_COMP_K400:
					val = buf[B2];
					break;
				case GB_PARAM_TM_K400:
					val = buf[B3];
					break;
				case GB_PARAM_SHIFT_FRONT:
					val = buf[B1];
					break;
				case GB_PARAM_SHIFT_BACK:
					val = buf[B2];
					break;
				case GB_PARAM_SHIFT_PRM:
					val = buf[B3];
					break;
				case GB_PARAM_SHIFT_PRD:
					val = buf[B4];
					break;
				case GB_PARAM_PRD_COM_LONG:		// DOWN
				case GB_PARAM_PRD_COM_BLOCK:	// DOWN
				case GB_PARAM_PRD_DR_COM_BLOCK:	// DOWN
				case GB_PARAM_PRM_COM_BLOCK:	// DOWN
				case GB_PARAM_PRM_DR_COM_BLOCK:
					val = buf[B1 + (lp->getNumOfCurrSameParam() - 1) / 8];
					break;
				default:
					val = buf[B1 + lp->getNumOfCurrSameParam() - 1];
					break;
			}

			// ��� ������� ���������� ���������� ��������� �� ������ ������,
			// � ��� ��������� ������� ��������.
			lp->setValue(val);
		}
	}

	return stat;
}

/**	������������ ������� � �������� �������� �������.
 *
 * 	��-��������� � ��������� ������ ��������� ���������� ������ ��� �������.
 * 	��-��������� � ���������� ��������� ��������� ���������� 1 ���� ������.
 *
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendData(eGB_COM com) {
	uint8_t num = 0;
	uint8_t mask = 0;


	mask = com & GB_COM_MASK_GROUP;

	if (mask == GB_COM_MASK_GROUP_WRITE_PARAM) {
		// ������� ��������� ����������

		eGB_SEND_TYPE sendType = sParam_->txComBuf.getSendType();

		if (com == GB_COM_SET_TIME) {
			num = addCom(com, 9, sParam_->txComBuf.getBuferAddress());
		} else if (com ==  GB_COM_PRM_RES_IND) {
			num = addCom(com);
		} else if (com == GB_COM_DEF_SET_TYPE_AC) {
			num = addCom(com, sParam_->txComBuf.getInt8());
		} else 	if (sendType != GB_SEND_NO) {
			uint8_t val = sParam_->txComBuf.getInt8(0);
			uint8_t dop = sParam_->txComBuf.getInt8(1);

			switch(sendType) {
				case GB_SEND_INT8:
					num = addCom(com, val);
					break;
				case GB_SEND_INT8_DOP:
					num = addCom(com, val, dop);
					break;
				case GB_SEND_DOP_INT8:	// DOWN
				case GB_SEND_DOP_BITES:
					num = addCom(com, dop, val);
					break;
				case GB_SEND_INT16_BE:
					num = addCom(com, val, dop);
					break;
				case GB_SEND_COR_U:
				case GB_SEND_COR_I:	// DOWN
					num = addCom(com, 3, sParam_->txComBuf.getBuferAddress());
					break;
				case GB_SEND_NO:
					break;
			}
		}
	} else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME) {
		// ������� ��������� ������� ������ � ������ � �������
		// �� ��������� ������������ ������ ��� �������

		if (com == GB_COM_SET_CONTROL) {
			num = addCom(com, sParam_->txComBuf.getInt8());
		} else if (com == GB_COM_SET_REG_TEST_1) {
			// ���� ��� ��������� ��������:
			// ���������� ������� ��������� ������
			// 		��� ���� ���������� 0 ���� ������
			// ���������� ������� ��������� �������� � �����
			// 		��� ��������������� ���������� ��� �������
			// 		������� ������� ��� ��
			// 		� ��� ��������� ������  ��� ��
			if (sParam_->txComBuf.getInt8(0) == 1) {
				num = addCom(com, 1, sParam_->txComBuf.getInt8(1));		// ��
				sParam_->txComBuf.setInt8(0, 0);
			} else if (sParam_->txComBuf.getInt8(2) == 2) {
				num = addCom(com, 2, sParam_->txComBuf.getInt8(3));		// ��
				sParam_->txComBuf.setInt8(2, 0);
			} else {
				num = addCom(com);										// ���.
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
		// ������� ������ ����������
		// �� ��������� ������������ ������ ��� �������
		if (com == GB_COM_GET_MEAS)
			num = addCom(com, 0);
		else
			num = addCom(com);
	} else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL) {
		// ������� ���������� ��������, � ��� ����� � ���-�� �������
		num = sendReadJrnCommand(com);
	}

	// ��������� �������, � ����������� �� ������������� �������� ���������
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}

/**	��������� �������� ������� ������.
 *
 * 	@param com	��� �������
 * 	@param pc True - ������� ����������� � ��, False - ����������� � ��-���.
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getDefCommand(eGB_COM com, bool pc) {
	bool stat = false;

	if (com == GB_COM_DEF_GET_LINE_TYPE) {
		stat = sParam_->def.setNumDevices((eGB_NUM_DEVICES) (buf[B1]));
		uint8_t act = sParam_->glb.setNumDevices((eGB_NUM_DEVICES) (buf[B1]));
		sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
		if (act & GB_ACT_NEW)
			sParam_->device = false;
	} else  if (com == GB_COM_DEF_GET_TYPE_AC) {
		// 1 ���� - ��� ��
		// 2-5 - ����� �� ��
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
		uint16_t t = *((uint16_t *) &buf[B1]);
		sParam_->jrnEntry.m_stNumEntries.numDef = t;
		if (buf[NUM] >= 4) {
			sParam_->jrnEntry.setNumEntries(GB_DEVICE_DEF, *((uint16_t *) &buf[B3]));
		}
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF) {
			stat = sParam_->jrnEntry.setNumJrnEntry(t);
		}
	} else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
		if ((sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF) && (!pc)){
			sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - ���� ������
			sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dateTime.setMsSecond(t);
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

/**	��������� �������� ������� ���������.
 *
 * 	@param com	��� �������
 * 	@param pc True - ������� ����������� � ��, False - ����������� � ��-���.
 * 	@retval True - � ������ �������� ���������
 * 	@retval False - � ������ ������.
 */
bool clProtocolBspS::getPrmCommand(eGB_COM com, bool pc) {
	bool stat = false;

	switch(com) {

		case GB_COM_PRM_GET_COM: {
			uint8_t act = GB_ACT_NO;
			if (sParam_->typeDevice == AVANT_K400) {
				act = sParam_->prm.setNumCom(buf[B1] * 4);
				sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
			}
			// � ������ ������ ������ ��������, ������� ���� ������������
			if (act & GB_ACT_NEW)
				sParam_->device = false;
		}
		break;

		case GB_COM_PRM_GET_JRN_CNT: {
			uint16_t t = *((uint16_t *) &buf[B1]);
			sParam_->jrnEntry.m_stNumEntries.numPrm = t;
			if (buf[NUM] >= 4) {
				sParam_->jrnEntry.setNumEntries(GB_DEVICE_PRM, *((uint16_t *) &buf[B3]));
			}
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) {
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_PRM_GET_JRN_ENTRY: {
			if ((sParam_->jrnEntry.getCurrentDevice()==GB_DEVICE_PRM)&& (!pc)){
				if (sParam_->typeDevice == AVANT_OPTO) {
					// ����
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B5]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B7]));
					// �����
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B8]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B11], buf[B12]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B1]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B14]));
					// B13 - ���� ������
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
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

/**	��������� �������� ������� �����������.
 *
 * 	@param com	��� �������
 * 	@param pc True - ������� ����������� � ��, False - ����������� � ��-���.
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getPrdCommand(eGB_COM com, bool pc) {
	bool stat = false;

	switch (com) {

		case GB_COM_PRD_GET_COM: {
			uint8_t act = GB_ACT_NO;
			if (sParam_->typeDevice == AVANT_K400) {
				act = sParam_->prd.setNumCom(buf[B1] * 4);
				sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
			}
			// � ������ ������ ������ ��������, ������� ���� ������������
			if (act & GB_ACT_NEW)
				sParam_->device = false;
		}
		break;

		case GB_COM_PRD_GET_JRN_CNT: {
			uint16_t t = *((uint16_t *) &buf[B1]);
			sParam_->jrnEntry.m_stNumEntries.numPrd = t;
			if (buf[NUM] >= 4) {
				sParam_->jrnEntry.setNumEntries(GB_DEVICE_PRD, *((uint16_t *) &buf[B3]));
			}
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) {
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_PRD_GET_JRN_ENTRY: {
			if ((sParam_->jrnEntry.getCurrentDevice()==GB_DEVICE_PRD)&& (!pc)) {
				if (sParam_->typeDevice == AVANT_OPTO) {
					// ����
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B5]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B7]));
					// �����
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B8]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B11], buf[B12]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B1]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B14]));
					// B13 - ���� ������
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
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

/**	��������� �������� ����� �������.
 *
 * 	@param com	��� �������
 * 	@param pc True - ������� ����������� � ��, False - ����������� � ��-���.
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com, bool pc) {
	bool stat = false;
	eGB_TYPE_DEVICE device = sParam_->typeDevice;

	switch (com) {
		case GB_COM_GET_TIME: {
			stat =  sParam_->DateTime.setYear(BCD_TO_BIN(buf[B1]));
			stat &= sParam_->DateTime.setMonth(BCD_TO_BIN(buf[B2]));
			stat &= sParam_->DateTime.setDay(BCD_TO_BIN(buf[B3]));
			stat &= sParam_->DateTime.setHour(BCD_TO_BIN(buf[B4]));
			stat &= sParam_->DateTime.setMinute(BCD_TO_BIN(buf[B5]));
			stat &= sParam_->DateTime.setSecond(BCD_TO_BIN(buf[B6]));
			// ������������ ���������������, ������ ���� ��� ���� � �������
			uint16_t ms =  buf[NUM] >= 8 ? *((uint16_t *) &buf[B7]) : 0;
			stat &= sParam_->DateTime.setMsSecond(ms);
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

			if (sParam_->typeDevice == AVANT_K400) {
				uint8_t num = buf[NUM];
				if (sParam_->prm.status.isEnable()) {
					if (num >= 13) 	sParam_->prm.setIndCom8(0, buf[B13]);
					if (num >= 14)  sParam_->prm.setIndCom8(1, buf[B14]);
					if (num >= 15)  sParam_->prm.setIndCom8(2, buf[B15]);
					if (num >= 16)  sParam_->prm.setIndCom8(3, buf[B16]);
				}
				if (sParam_->prd.status.isEnable()) {
					if (num >= 17) 	sParam_->prd.setIndCom8(0, buf[B17]);
					if (num >= 18)  sParam_->prd.setIndCom8(1, buf[B18]);
					if (num >= 19)  sParam_->prd.setIndCom8(2, buf[B19]);
					if (num >= 20)  sParam_->prd.setIndCom8(3, buf[B20]);
				}
			}

			eGB_REGIME reg = GB_REGIME_MAX;
			eGB_REGIME regTmp = GB_REGIME_MAX;
			// ����������� ������ ������ ��������
			// �� ��������� ��� ����� GB_REGIME_ENABLED
			// ������ ��������� ��������, ���� ��� ���������� ��������
			// ����� ���� � ��� �� �����
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
			// ��������� ������(��) ��.��������� ���� �400(�) � ������ ����-�
			uint8_t n = 0;
			if ((device == AVANT_R400) || (device == AVANT_R400M)) {
				if (sParam_->glb.getCompatibility() == GB_COMPATIBILITY_PVZUE) {
					n = buf[B5];
				}
			}
			sParam_->def.status.setRemoteNumber(n);

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
			// ���������� �������, ���� ����� ���� ������ ���� ����������
			if (buf[B1] == 0) {
				sParam_->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
				sParam_->measParam.setCurrentOut(TO_INT16(buf[B4],buf[B5]));
				// � buf[B7] ��������� ������� ����� ���������� * 100
				// �.�. ���� ��� 90, �� ��� 0.9�.
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
			// ������ � ���� ��������
			act |= sParam_->def.status.setEnable(buf[B1] == 1);
			act |= sParam_->prm.setNumCom(buf[B2] * 4);
			sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
			// buf[B3] - ���2
			act |= sParam_->prd.setNumCom(buf[B4] * 4);
			sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
			// ���-�� ��������� � �����
			// � def �������� �������� ���������
			// ��� �������� � ���� ������������ ������������ �������� � glb.
			// !!! � �������� == ���-�� ���������, � ������ ���� �� 1 ������
			act |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES) (buf[B5]-1));
			sParam_->def.setNumDevices((eGB_NUM_DEVICES) (buf[B5]-1));
			sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
			// ��� ����� (��, ������, ...)
			act |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
			// ������ �������� ������ ���
			TDeviceGlb *glb = &sParam_->glb;
			glb->setVersProgIC16(TO_INT16(buf[B7], buf[B8]) , GB_IC_BSP_MCU);
			glb->setVersProgIC16(TO_INT16(buf[B9], buf[B10]), GB_IC_BSP_DSP);

			// �������������, ������ � �400�
			act |= sParam_->glb.setCompatibility((eGB_COMPATIBILITY) buf[B11]);

			glb->setVersProgIC16(VERS, GB_IC_PI_MCU);
			glb->setVersProgIC8(buf[B12], GB_IC_BSK_PLIS_PRD1);
			glb->setVersProgIC8(buf[B13], GB_IC_BSK_PLIS_PRD2);
			glb->setVersProgIC8(buf[B14], GB_IC_BSK_PLIS_PRM1);
			glb->setVersProgIC8(buf[B15], GB_IC_BSK_PLIS_PRM2);
			glb->setVersProgIC8(buf[B16], GB_IC_BSZ_PLIS);

			// �������������, ��� �400/�400� � �400 ����������
			if (sParam_->def.status.isEnable()) {
				eGB_COMPATIBILITY comp;
				comp = static_cast<eGB_COMPATIBILITY>(buf[B11]);
				act |= sParam_->glb.setCompatibility(comp);
			} else {
				eGB_COMP_K400 comp;
				comp = static_cast<eGB_COMP_K400> (buf[B11]);
				act |= sParam_->glb.setCompK400(comp);
			}
			if (buf[3] > 16) {
				// ��� ��������, � �������� 2014 �������� � ���� � �400
				// � ������ 2014 ��������  � �400
				act |= glb->setTypeDevice((eGB_TYPE_DEVICE) buf[B17]);
			} else {
				glb->setTypeDevice(AVANT_NO);
			}

			// �������� ������������� ���������� ���� ��������
			if (act & GB_ACT_NEW)			if (act & GB_ACT_NEW) {
				sParam_->device = false;
			}

			stat = ((act & GB_ACT_ERROR) != GB_ACT_ERROR);
		}
		break;

		case GB_COM_GET_DEVICE_NUM: {
			stat = sParam_->glb.setDeviceNum(buf[B1]);
		}
		break;

		case GB_COM_GET_COM_PRD_KEEP: {
			uint8_t act = GB_ACT_NO;
			if (sParam_->typeDevice == AVANT_R400M) {
				// � �400�/�400 ��� ������������� (��� ���������� ��������)
				act = sParam_->glb.setCompatibility((eGB_COMPATIBILITY)buf[B1]);
			} else if (sParam_->typeDevice == AVANT_K400) {
				// ������������� �400
				act = sParam_->glb.setCompK400((eGB_COMP_K400) buf[B2]);
			}
			// � ������ ������ ������ ��������, ������� ���� ������������
			if (act & GB_ACT_NEW)
				sParam_->device = false;
		}
		break;

		case GB_COM_GET_NET_ADR: {
			stat = sParam_->glb.setNetAddress(buf[B1]);
		}
		break;

		case GB_COM_GET_TEST: {
			eGB_TYPE_DEVICE type = sParam_->typeDevice;
			// ���������� ����������� ���-��
			sParam_->test.setCurrentSignal(&buf[B1], type);
		}
		break;

		case GB_COM_GET_JRN_CNT: {
			uint16_t t = *((uint16_t *) &buf[B1]);
			sParam_->jrnEntry.m_stNumEntries.numGlb = t;
			if (buf[NUM] >= 4) {
				sParam_->jrnEntry.setNumEntries(GB_DEVICE_GLB, *((uint16_t *) &buf[B3]));
			}
			if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
				stat = sParam_->jrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_GET_JRN_ENTRY: {
			if ((sParam_->jrnEntry.getCurrentDevice()==GB_DEVICE_GLB)&& (!pc)) {
				sParam_->jrnEntry.val = true;
				if (sParam_->typeDevice == AVANT_OPTO) {
					// ����
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B6]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B7]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B8]));
					// �����
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B9]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B10]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B11]));
					uint16_t t = TO_INT16(buf[B12], buf[B13]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
					//
					sParam_->jrnEntry.setRegime((eGB_REGIME) buf[B1]);
					sParam_->jrnEntry.setOpticEntry((uint8_t *) &buf[B2]);
					sParam_->jrnEntry.setReady();
					stat = true;
				} else {
					// ����
					sParam_->jrnEntry.dateTime.setYear(BCD_TO_BIN(buf[B16]));
					sParam_->jrnEntry.dateTime.setMonth(BCD_TO_BIN(buf[B15]));
					sParam_->jrnEntry.dateTime.setDay(BCD_TO_BIN(buf[B14]));
					sParam_->jrnEntry.dateTime.setDayWeek(BCD_TO_BIN(buf[B13]));
					// �����
					sParam_->jrnEntry.dateTime.setHour(BCD_TO_BIN(buf[B12]));
					sParam_->jrnEntry.dateTime.setMinute(BCD_TO_BIN(buf[B11]));
					sParam_->jrnEntry.dateTime.setSecond(BCD_TO_BIN(buf[B10]));
					uint16_t t = TO_INT16(buf[B9], buf[B8]);
					sParam_->jrnEntry.dateTime.setMsSecond(t);
					// ! B1 - ��� ����������, �� ������ ������ ������������
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

/**	������������ ��������� ������� ���������� ���-�� � ����� ������� �������.
 *
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendReadJrnCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t mask = 0;

	// ������� ������ � ��������
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
