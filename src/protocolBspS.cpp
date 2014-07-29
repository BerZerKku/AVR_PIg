/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

#include "../inc/glbDefine.h"
#include "../inc/paramBsp.h"

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

		if (mask == GB_COM_MASK_DEVICE_GLB)
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

	mask = com & GB_COM_MASK_DEVICE;

	switch(com & GB_COM_MASK_GROUP) {

		case GB_COM_MASK_GROUP_WRITE_PARAM: {
			// ������� �� ��������� ����������
			switch(com & GB_COM_MASK_DEVICE) {
				case GB_COM_MASK_DEVICE_GLB:
					num = sendModifGlbCommand(com);
					break;
				case GB_COM_MASK_DEVICE_PRM:
					// GB_COM_SET_PRM_RES_IND
					num = addCom(com);
					break;
			}
		}
		break;

		case GB_COM_MASK_GROUP_WRITE_REGIME: {
			// ������� �� ��������� ������� ������
			if (com == GB_COM_SET_CONTROL) {
				num = addCom(com, sParam_->txComBuf.getInt8());
			} else {
				// GB_COM_SET_PRM_ENTER
				// GB_COM_SET_REG_DISABLED
				// GB_COM_SET_REG_ENABLED
				num = addCom(com);
			}
		}
		break;

		case GB_COM_MASK_GROUP_READ_PARAM: {
			// ������� ���������� ����������
			if (com == GB_COM_GET_MEAS)
				num = addCom(com, 0);
			else
				num = addCom(com);
		}
		break;

		case GB_COM_MASK_GROUP_READ_JOURNAL: {
			// ������� ���������� ��������, � ��� ����� � ���-�� �������
			num = sendReadJrnCommand(com);
		}
		break;
	}

	// ��������� �������, � ����������� �� ������������� �������� ���������
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}

/**	��������� �������� ����� �������.
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com) {
	bool stat = false;

	switch (com) {
		case GB_COM_GET_TIME: {
			sParam_->dataTime.setYear(BCD_TO_BIN(buf[B1]));
			sParam_->dataTime.setMonth(BCD_TO_BIN(buf[B2]));
			sParam_->dataTime.setDay(BCD_TO_BIN(buf[B3]));
			sParam_->dataTime.setHour(BCD_TO_BIN(buf[B4]));
			sParam_->dataTime.setMinute(BCD_TO_BIN(buf[B5]));
			sParam_->dataTime.setSecond(BCD_TO_BIN(buf[B6]));
			stat = true;
		}
		break;

		case GB_COM_GET_SOST: {
			sParam_->glb.status.setRegime((eGB_REGIME) buf[B1]);
			sParam_->glb.status.setState(buf[B2]);
			sParam_->glb.status.setDopByte(buf[B3]);

			stat = true;
		}
		break;

		case GB_COM_GET_FAULT: {
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
			// ������ �������� ������ ���
			TDeviceGlb *glb = &sParam_->glb;
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
			stat = sParam_->glb.setCompDelay(buf[B1]);
		}
		break;

		case GB_COM_GET_CURR_MAX: {
			stat = sParam_->glb.setCurrMax(buf[B1]);
		}
		break;

		case GB_COM_GET_CURR_MIN: {
			stat = sParam_->glb.setCurrMin(buf[B1]);
		}
		break;

		case GB_COM_GET_VOLT_MIN : {
			stat = sParam_->glb.setVoltMin(buf[B1]);
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

				// ����
				sParam_->jrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
				sParam_->jrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
				sParam_->jrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
				// B13 - ���� ������
				// �����
				sParam_->jrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
				sParam_->jrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
				sParam_->jrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
				uint16_t t = TO_INT16(buf[B9], buf[B8]);
				sParam_->jrnEntry.dataTime.setMsSecond(t);
				// ! B1 - ��� ����������, �� ������ ������ ������������
				sParam_->jrnEntry.setReady();
				stat = true;
			}
		}
		break;

		default:
			break;

	}

	return stat;
}

/**	������������ ��������� ����� �������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t clProtocolBspS::sendModifGlbCommand(eGB_COM com) {
	uint8_t num = 0;
	uint8_t b1 = sParam_->txComBuf.getInt8(0);
//	uint8_t b2 = sParam_->txComBuf.getInt8(1);

	if (com == GB_COM_SET_TIME) {
		num = addCom(com, 6, sParam_->txComBuf.getBuferAddress());
	}  else {
		// �� ��������� ���������� ���� ����
		num = addCom(com, b1);
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
	if (com == GB_COM_GET_JRN_CNT) {

		num = addCom(com);
	} else if (com == GB_COM_GET_JRN_ENTRY) {
		uint16_t t = sParam_->txComBuf.getInt16();

		num = addCom(com, t >> 8, t);
	}

	return num;
}
