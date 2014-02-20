/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
		clProtocolS(buf, size, sParam) {

}

/**	��������� ��������� ���������.
 * 	@param ���
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getData() {
	bool stat = false;
	uint8_t mask = 0;
	eGB_COM com = (eGB_COM) buf[2];

	// ��������� ����������, �������� ���� �� ������
	this->stat_ = PRTS_STATUS_NO;

	mask = com & GB_COM_MASK_GROUP;
	// ����� �� ������� ��������� ���������/������ �� ���������
	if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM)
			|| (mask == GB_COM_MASK_GROUP_WRITE_REGIME)) {
		stat = true;
	} else {
		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_DEF)
			stat = getDefCommand(com);				// ������� ������
		else if (mask == GB_COM_MASK_DEVICE_PRM)
			stat = getPrmCommand(com);				// ������� ���������
		else if (mask == GB_COM_MASK_DEVICE_PRD)
			stat = getPrdCommand(com);				// ������� �����������
		else
			stat = getGlbCommand(com);				// ������� �����
	}

	return stat;
}

/**	������������ ������� � �������� �������� �������.
 * 	��-��������� � ��������� ������ ��������� ���������� ������ ��� �������.
 * 	��-��������� � ���������� ��������� ��������� ���������� 1 ���� ������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendData(eGB_COM com) {
	uint8_t num = 0;
	uint8_t mask = 0;

	mask = com & GB_COM_MASK_GROUP;
	if (mask == GB_COM_MASK_GROUP_WRITE_PARAM) {
		// ������� ��������� ����������

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
		// ������� ��������� ������� ������ � ������ � �������
		// �� ��������� ������������ ������ ��� �������

		if (com == GB_COM_SET_CONTROL)
			num = addCom(com, sParam_->txComBuf.getInt8());
		else if (com == GB_COM_SET_REG_TEST_1) {
			uint8_t b1 = sParam_->txComBuf.getInt8(0);
			uint8_t b2 = sParam_->txComBuf.getInt8(1);
			num = addCom(com, b1, b2);
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
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getDefCommand(eGB_COM com) {
	bool stat = false;

	if (com == GB_COM_DEF_GET_DEF_TYPE) {
		stat = sParam_->def.setDefType(buf[B1]);
	} else if (com == GB_COM_DEF_GET_LINE_TYPE) {
		stat = sParam_->def.setNumDevices((eGB_NUM_DEVICES) buf[B1]);
	} else if (com == GB_COM_DEF_GET_T_NO_MAN) {
		stat = sParam_->def.setTimeNoMan(buf[B1]);
	} else if (com == GB_COM_DEF_GET_DELAY) {
		stat = sParam_->def.setDelay(buf[B1]);
	} else if (com == GB_COM_DEF_GET_OVERLAP) {
		stat = sParam_->def.setOverlap(buf[B1]);
	} else if (com == GB_COM_DEF_GET_RZ_DEC) {
		stat = sParam_->def.setRzDec(buf[B1]);
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
			// ! � �400� ��� ������� ��� (����)
			sParam_->def.setFreqPrm((eGB_PVZL_FREQ) buf[B1]);
		} else
			stat = sParam_->def.setRzThreshold(buf[B1]);
	} else if (com == GB_COM_DEF_GET_TYPE_AC) {
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
		//				stat = sParam_->def.setNumJrnEntry(TO_INT16(buf[B2], buf[B1]));
		uint16_t t = TO_INT16(buf[B2], buf[B1]);
		stat = sParam_->jrnEntry.setNumJrnEntry(t);
	} else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF) {
			sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - ���� ������
			sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dataTime.setMsSecond(t);
			sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE) buf[B1]);
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
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getPrmCommand(eGB_COM com) {
	bool stat = false;

	if (com == GB_COM_PRM_GET_TIME_ON) {
		stat = sParam_->prm.setTimeOn(buf[B1]);
	} else if (com == GB_COM_PRM_GET_BLOCK_COM) {
		sParam_->prm.setBlockCom8(0, buf[B1]);
		sParam_->prm.setBlockCom8(1, buf[B2]);
		sParam_->prm.setBlockCom8(2, buf[B3]);
		sParam_->prm.setBlockCom8(3, buf[B4]);
		stat = true;
	} else if (com == GB_COM_PRM_GET_TIME_OFF) {
		stat = sParam_->prm.setTimeOff(&buf[B1]);
	} else if (com == GB_COM_PRM_GET_JRN_CNT) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) {
			uint16_t t = TO_INT16(buf[B2], buf[B1]);
			stat = sParam_->jrnEntry.setNumJrnEntry(t);
		}
	} else if (com == GB_COM_PRM_GET_JRN_ENTRY) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) {
			sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - ���� ������
			sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dataTime.setMsSecond(t);
			sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE) buf[B1]);
			sParam_->jrnEntry.setNumCom(buf[B2]);
			sParam_->jrnEntry.setEventType(buf[B3]);
			sParam_->jrnEntry.setReady();
			stat = true;
		}
	}

	return stat;
}

/**	��������� �������� ������� �����������.
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getPrdCommand(eGB_COM com) {
	bool stat = false;

	if (com == GB_COM_PRD_GET_TIME_ON) {
		stat = sParam_->prd.setTimeOn(buf[B1]);
	} else if (com == GB_COM_PRD_GET_DURATION) {
		stat = sParam_->prd.setDuration(buf[B1]);
	} else if (com == GB_COM_PRD_GET_TEST_COM) {
		stat = sParam_->prd.setTestCom(buf[B1]);
	} else if (com == GB_COM_PRD_GET_BLOCK_COM) {
		sParam_->prd.setBlockCom8(0, buf[B1]);
		sParam_->prd.setBlockCom8(1, buf[B2]);
		sParam_->prd.setBlockCom8(2, buf[B3]);
		sParam_->prd.setBlockCom8(3, buf[B4]);
		stat = true;
	} else if (com == GB_COM_PRD_GET_LONG_COM) {
		sParam_->prd.setLongCom8(0, buf[B1]);
		sParam_->prd.setLongCom8(1, buf[B2]);
		sParam_->prd.setLongCom8(2, buf[B3]);
		sParam_->prd.setLongCom8(3, buf[B4]);
		stat = true;
	} else if (com == GB_COM_PRD_GET_JRN_CNT) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) {
			uint16_t t = TO_INT16(buf[B2], buf[B1]);
			stat = sParam_->jrnEntry.setNumJrnEntry(t);
		}
	} else if (com == GB_COM_PRD_GET_JRN_ENTRY) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) {
			sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - ���� ������
			sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dataTime.setMsSecond(t);
			sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE) buf[B1]);
			sParam_->jrnEntry.setNumCom(buf[B2]);
			sParam_->jrnEntry.setEventType(buf[B3]);
			sParam_->jrnEntry.setReady();
			stat = true;
		}
	}

	return stat;
}

/**	��������� �������� ����� �������.
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com) {
	bool stat = false;

	if (com == GB_COM_GET_TIME) {
		sParam_->dataTime.setYear(BCD_TO_BIN(buf[B1]));
		sParam_->dataTime.setMonth(BCD_TO_BIN(buf[B2]));
		sParam_->dataTime.setDay(BCD_TO_BIN(buf[B3]));
		sParam_->dataTime.setHour(BCD_TO_BIN(buf[B4]));
		sParam_->dataTime.setMinute(BCD_TO_BIN(buf[B5]));
		sParam_->dataTime.setSecond(BCD_TO_BIN(buf[B6]));
		stat = true;
	} else if (com == GB_COM_GET_SOST) {

		sParam_->def.status.setRegime((eGB_REGIME) buf[B1]);
		sParam_->def.status.setState(buf[B2]);
		sParam_->def.status.setDopByte(buf[B3]);

		sParam_->prm.status.setRegime((eGB_REGIME) buf[B4]);
		sParam_->prm.status.setState(buf[B5]);
		sParam_->prm.status.setDopByte(buf[B6]);

		sParam_->prd.status.setRegime((eGB_REGIME) buf[B7]);
		sParam_->prd.status.setState(buf[B8]);
		sParam_->prd.status.setDopByte(buf[B9]);

		// ���� ���� ���� �� ��������� ��������� �� � ������ �������
		// (GB_REGIME_DISABLED) ����� ����� ����� ����� GB_REGIME_MAX
		//				eGB_REGIME reg = GB_REGIME_DISABLED;
		//				if (sParam_->prd.status.isEnable())
		//				{
		//					if (sParam_->prd.status.getRegime() == GB_REGIME_ENABLED)
		//						reg = GB_REGIME_ENABLED;
		//				}
		//				if (sParam_->prm.status.isEnable())
		//				{
		//					// � ��������� ����� �����, ���� ������� �� ������
		//					if ((sParam_->prm.status.getRegime() == GB_REGIME_ENABLED)||
		//						(sParam_->prm.status.getRegime() == GB_REGIME_READY))
		//						reg = GB_REGIME_ENABLED;
		//				}
		//				if (sParam_->def.status.isEnable())
		//				{
		//					if (sParam_->def.status.getRegime() == GB_REGIME_ENABLED)
		//						reg = GB_REGIME_ENABLED;
		//				}
		//				sParam_->glb.status.setRegime(reg);

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
	} else if (com == GB_COM_GET_FAULT) {
		sParam_->def.status.setFault(TO_INT16(buf[B1], buf[B2]));
		sParam_->def.status.setWarning(TO_INT16(buf[B3], buf[B4]));

		sParam_->prm.status.setFault(TO_INT16(buf[B5], buf[B6]));
		sParam_->prm.status.setWarning(TO_INT16(buf[B7], buf[B8]));

		sParam_->prd.status.setFault(TO_INT16(buf[B9], buf[B10]));
		sParam_->prd.status.setWarning(TO_INT16(buf[B11],buf[B12]));

		sParam_->glb.status.setFault(TO_INT16(buf[B13], buf[B14]));
		sParam_->glb.status.setWarning(TO_INT16(buf[B15],buf[B16]));
		stat = true;
	} else if (com == GB_COM_GET_MEAS) {
		// ���������� �������, ���� ����� ���� ������ ���� ����������
		if (buf[B1] == 0) {
			sParam_->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
			sParam_->measParam.setCurrentOut(TO_INT16(buf[B4],buf[B5]));
			// � buf[B7] ��������� ������� ����� ���������� * 100
			// �.�. ���� ��� 90, �� ��� 0.9�.
			sParam_->measParam.setVoltageOut(buf[B6], (buf[B7] / 10));
			sParam_->measParam.setVoltageDef(buf[B8]);
			// B9 ������� ��� U� ������ �����
			sParam_->measParam.setVoltageCf(buf[B10]);
			// B11 ���� ������� ��� Uk ������ �����
			sParam_->measParam.setVoltageNoise(buf[B12]);
			// B13 ���� ������� ��� ���������� ������������ ����� ���
			// B14, B15 ����� �������� ��� ����������� �������� ������
			stat = true;
		}
	} else if (com == GB_COM_GET_VERS) {
		bool re = false;
		// ������ � ���� ��������
		re |= sParam_->def.status.setEnable(buf[B1] == 1);
		re |= sParam_->prm.setNumCom(buf[B2] * 4);
		// buf[B3] - ���2
		re |= sParam_->prd.setNumCom(buf[B4] * 4);
		re |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES) buf[B5]);
		re |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
		sParam_->glb.setVersBsp(TO_INT16(buf[B7], buf[B8]));
		sParam_->glb.setVersDsp(TO_INT16(buf[B9], buf[B10]));
		re |= sParam_->glb.setCompatibility((eGB_COMPATIBILITY) buf[B11]);

		// "���������" ���� ������������� ��������� ���� ��������
		if (re)
			sParam_->device = false;

		stat = true;
	} else if (com == GB_COM_GET_TIME_SINCHR) {
		stat = sParam_->glb.setTimeSinchr(buf[B1]);
	} else if (com == GB_COM_GET_DEVICE_NUM) {
		stat = sParam_->glb.setDeviceNum(buf[B1]);
	} else if (com == GB_COM_GET_OUT_CHECK) {
		stat = sParam_->glb.setOutCheck(buf[B1]);
	} else if (com == GB_COM_GET_CF_THRESHOLD) {
		sParam_->glb.setCfThreshold(buf[B1]);
		sParam_->glb.setInDecrease(buf[B2]);
		// B3 - �������� �������� ������� ��� ������� ������
		stat = true;
	} else if (com == GB_COM_GET_TIME_RERUN) {
		if (sParam_->typeDevice == AVANT_R400M) {
			// � ���� ��� �������� ������ ��
			stat = sParam_->glb.setAcInDec(buf[B1]);

			// � ����-� ��� ����� ����������
		} else {
			stat = sParam_->glb.setTimeRerun(buf[B1]);
		}
	} else if (com == GB_COM_GET_FREQ) {
		uint16_t t = TO_INT16(buf[B1], buf[B2]);
		stat = sParam_->glb.setFreq(t);
	} else if (com == GB_COM_GET_COM_PRM_KEEP) {
		if (sParam_->typeDevice == AVANT_R400M) {
			// ! � �400 ��� U��� �����������
			stat = sParam_->glb.setUoutNom(buf[B1]);
		} else {
			stat = sParam_->glb.setComPrmKeep(buf[B1]);
		}
	} else if (com == GB_COM_GET_COM_PRD_KEEP) {
		// !!! �������� ��������� ���� ���������� ��������
		if (sParam_->typeDevice == AVANT_R400M) {
			// ! � �400 ��� ������������� (��� ���������� ��������)
			eGB_COMPATIBILITY t = (eGB_COMPATIBILITY) buf[B1];
			stat = sParam_->glb.setCompatibility(t);
		} else {
			stat = sParam_->glb.setComPrdKeep(buf[B1]);
		}
	} else if (com == GB_COM_GET_NET_ADR) {
		stat = sParam_->glb.setNetAddress(buf[B1]);
	} else if (com == GB_COM_GET_COR_U_I) {
		eGB_TYPE_DEVICE type = sParam_->typeDevice;

		// � ������ ��� ��������������, ����� ��������� ���� � ����������
		if (type == AVANT_OPTIC) {
			// TODO - ������, �������� ��������������
		} else {
			int16_t val = 0;
			int8_t i = buf[B1];
			int8_t f = buf[B2];
			val = i * 10 + static_cast<int16_t>(f / 10);
			stat = sParam_->glb.setCorU(val);
			// ��������� ����
			val = static_cast<int16_t>(TO_INT16(buf[B3], buf[B4]));
			stat |= sParam_->glb.setCorI(val);
		}
	} else if (com == GB_COM_GET_TEST) {
		eGB_TYPE_DEVICE type = sParam_->typeDevice;
		sParam_->test.setCurrentSignal(&buf[B1], type);
	} else if (com == GB_COM_GET_JRN_CNT) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
			uint16_t t = TO_INT16(buf[B2], buf[B1]);
			stat = sParam_->jrnEntry.setNumJrnEntry(t);
		}
	} else if (com == GB_COM_GET_JRN_ENTRY) {
		if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
			sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
			sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
			sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
			// B13 - ���� ������
			sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
			sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
			sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
			uint16_t t = TO_INT16(buf[B9], buf[B8]);
			sParam_->jrnEntry.dataTime.setMsSecond(t);
			// ! B1 - ��� ����������, �� ������ ������ ������������
			// ! sParam_->journalEntry.setDevice((eGB_DEVICE) buf[B1]);
			sParam_->jrnEntry.setEventType(buf[B2]);
			sParam_->jrnEntry.setRegime((eGB_REGIME) buf[B3]);
			sParam_->jrnEntry.setReady();
			stat = true;
		}
	}

	return stat;
}

/**	������������ ��������� ������� ������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendModifDefCommand(eGB_COM com) {
	uint8_t num = 0;

	// �� ��������� ���������� ���� ����

	// GB_COM_DEF_SET_DEF_TYPE
	// GB_COM_DEF_SET_LINE_TYPE
	// GB_COM_DEF_SET_T_NO_MAN
	// GB_COM_DEF_SET_DELAY
	// GB_COM_DEF_SET_OVERLAP
	// GB_COM_DEF_SET_RZ_DEC
	// GB_COM_DEF_SET_TYPE_AC
	// GB_COM_SET_PRM_TYPE
	// GB_COM_DEF_SET_FREQ_PRD
	// GB_COM_DEF_SET_RZ_THRESH
	num = addCom(com, sParam_->txComBuf.getInt8());

	return num;
}

/**	������������ ��������� ������� ���������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendModifPrmCommand(eGB_COM com) {
	uint8_t num = 0;

	if (com == GB_COM_PRM_SET_TIME_ON) {
		num = addCom(com, sParam_->txComBuf.getInt8());
	} else if (com == GB_COM_PRM_SET_TIME_OFF) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else if (com == GB_COM_PRM_SET_BLOCK_COM) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else if (com == GB_COM_PRM_RES_IND) {
		num = addCom(com);
	}

	return num;
}

/**	������������ ��������� ������� �����������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendModifPrdCommand(eGB_COM com) {
	uint8_t num = 0;

	// �� ��������� ���������� ���� ����
	if (com == GB_COM_PRD_SET_BLOCK_COM) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else if (com == GB_COM_PRD_SET_LONG_COM) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else {
		// GB_COM_PRD_SET_TIME_ON
		// GB_COM_PRD_SET_DURATION
		// GB_COM_PRD_SET_TEST_COM
		num = addCom(com, sParam_->txComBuf.getInt8());
	}

	return num;
}

/**	������������ ��������� ����� �������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendModifGlbCommand(eGB_COM com) {
	uint8_t num = 0;

	// �� ��������� ���������� ���� ����
	if (com == GB_COM_SET_TIME) {
		num = addCom(com, 6, sParam_->txComBuf.getBuferAddress());
	} else if (com == GB_COM_SET_TIME_RERUN) {
		if (sParam_->typeDevice == AVANT_R400M) {
			num = addCom(com, sParam_->txComBuf.getInt8(0),
					sParam_->txComBuf.getInt8(1));
		} else
			num = addCom(com, sParam_->txComBuf.getInt8());
	} else if (com == GB_COM_SET_CF_THRESHOLD) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else if (com == GB_COM_SET_FREQ) {
		num = addCom(com, sParam_->txComBuf.getInt8(0),
				sParam_->txComBuf.getInt8(1));
	} else if (com == GB_COM_SET_COR_U_I) {
		num = addCom(com, 3, sParam_->txComBuf.getBuferAddress());
	} else {
		// GB_COM_SET_TIME_SINCHR
		// GB_COM_SET_COM_PRM_KEEP
		// GB_COM_SET_DEVICE_NUM
		// GB_COM_SET_OUT_CHECK
		// GB_COM_SET_NET_ADR
		num = addCom(com, sParam_->txComBuf.getInt8());
	}

	return num;
}

/**	������������ ��������� ������� ���������� ���-�� � ����� ������� �������.
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
