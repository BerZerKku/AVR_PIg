/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolBspS.h"

#include "glbDefine.h"
#include "paramBsp.h"

/**	�����������.
 * 	@param *buf ��������� �� ����� ������.
 * 	@param size ������ ������ ������.
 * 	@param *sParam ��������� �� ��������� ����������.
 */
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
		stat = true;
	} else {
		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_GLB)
			stat = getGlbCommand(com, pc);				// ������� �����

		LocalParams *lp = &sParam_->local;

		if (com == lp->getCom()) {
			// �� ��������� ����������� �������� ������� �����,
			// �� �������� �� ����� ��������� ����� ������� ��������
			int16_t val = -1000;
			switch(lp->getParam()) {
				case GB_PARAM_FREQ:
					val = TO_INT16(buf[B1], buf[B2]);
					break;
//				case GB_PARAM_PRM_DR_COM_BLOCK:
//					val = buf[B1 + (lp->getNumOfCurrSameParam() - 1) / 8];
//					break;
				default:
					if (lp->getMin() < 0) {
						val = (int8_t) buf[B1 + lp->getNumOfCurrSameParam() - 1];
					} else {
						val = buf[B1 + lp->getNumOfCurrSameParam() - 1];
					}

					break;
			}
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

		if (com == GB_COM_SET_TIME) {
			num = addCom(com, 6, sParam_->TxComBuf.getBuferAddress());
		} else 	{
			uint8_t val = sParam_->TxComBuf.getInt8(0);
			uint8_t dop = sParam_->TxComBuf.getInt8(1);
			switch(sParam_->TxComBuf.getSendWrType()) {
				case GB_SEND_WR_INT8:
					num = addCom(com, val);
					break;
				case GB_SEND_WR_INT8_DOP:
					num = addCom(com, val, dop);
					break;
				case GB_SEND_WR_DOP_INT8:	// DOWN
				case GB_SEND_WR_DOP_BITES:
				case GB_SEND_WR_DOP_BITE:
					num = addCom(com, dop, val);
					break;
				case GB_SEND_WR_INT16_BE:
					num = addCom(com, val, dop);
					break;
				case GB_SEND_WR_NO:
					break;
			}
		}
	} else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME) {
		// ������� ��������� ������� ������ � ������ � �������
		// �� ��������� ������������ ������ ��� �������

		if (com == GB_COM_SET_CONTROL) {
			num = addCom(com, sParam_->TxComBuf.getInt8());
		} else {
			num = addCom(com);
		}
	} else if (mask == GB_COM_MASK_GROUP_READ_PARAM) {
		// ������� ������ ����������
		// �� ��������� ������������ ������ ��� �������
		if (com == GB_COM_GET_MEAS)
			num = addCom(com, 0);
		else if (com == sParam_->local.getCom()) {
			switch(sParam_->local.getSendRdType()) {
				case GB_SEND_RD_NO_DATA:
					num = addCom(com);
					break;
				case GB_SEND_RD_DOP:
					num = addCom(com, sParam_->local.getSendDop());
					break;
				case GB_SEND_RD_NO:
					break;
			}
		} else {
			num = addCom(com);
		}

	} else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL) {
		// ������� ���������� ��������, � ��� ����� � ���-�� �������
		num = sendReadJrnCommand(com);
	}

	// ��������� �������, � ����������� �� ������������� �������� ���������
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}

/**	��������� �������� ����� �������.
 * 	@param com	��� �������
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com, bool pc) {
	bool stat = false;

	switch (com) {
		case GB_COM_GET_TIME: {
			// ������� ���� � �����
			stat =  sParam_->DataTime.setYear(BCD_TO_BIN(buf[B1]));
			stat &= sParam_->DataTime.setMonth(BCD_TO_BIN(buf[B2]));
			stat &= sParam_->DataTime.setDay(BCD_TO_BIN(buf[B3]));
			stat &= sParam_->DataTime.setHour(BCD_TO_BIN(buf[B4]));
			stat &= sParam_->DataTime.setMinute(BCD_TO_BIN(buf[B5]));
			stat &= sParam_->DataTime.setSecond(BCD_TO_BIN(buf[B6]));
		}
		break;

		case GB_COM_GET_SOST: {
			// ����� / ��������� ���������
			stat =  sParam_->Rps.Status.setRegime((eGB_REGIME) buf[B7]);
			stat &= sParam_->Rps.Status.setState(buf[B8]);
			stat &= sParam_->Rps.Status.setDopByte(buf[B9]);

			// ����� �����
			stat &= sParam_->Glb.Status.setRegime((eGB_REGIME) buf[B7]);
		}
		break;

		case GB_COM_GET_FAULT: {
			// ������������� � �������������� ���������
			stat =  sParam_->Rps.Status.setFault(TO_INT16(buf[B1], buf[B2]));
			stat &= sParam_->Rps.Status.setWarning(TO_INT16(buf[B3], buf[B4]));

			// ����� ������������� � ��������������
 			stat &= sParam_->Glb.Status.setFault(TO_INT16(buf[B13], buf[B14]));
			stat &= sParam_->Glb.Status.setWarning(TO_INT16(buf[B15],buf[B16]));
		}
		break;

		case GB_COM_GET_MEAS: {
			// ���������� �������, ���� ����� ���� ������ ���� ����������
			if (buf[B1] == 0) {
				stat  = sParam_->MeasParam.setPOut(buf[B2]);
				stat &= sParam_->MeasParam.setUCtrl(buf[B3]);
				stat &= sParam_->MeasParam.setSN(buf[B4]);
				stat &= sParam_->MeasParam.setTBack(buf[B5]);
				stat &= sParam_->MeasParam.setDFi(buf[B6]);
				stat &= sParam_->MeasParam.setFazA(buf[B7]);
				stat &= sParam_->MeasParam.setFazB(buf[B8]);
				stat &= sParam_->MeasParam.setFazC(buf[B9]);
				stat &= sParam_->MeasParam.setIA(buf[B10]);
				stat &= sParam_->MeasParam.setIB(buf[B11]);
				stat &= sParam_->MeasParam.setIC(buf[B12]);
				stat &= sParam_->MeasParam.setUA(buf[B13]);
				stat &= sParam_->MeasParam.setUB(buf[B14]);
				stat &= sParam_->MeasParam.setUC(buf[B15]);
			}
		}
		break;

		case GB_COM_GET_VERS: {
			// ������ ��������
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

		case GB_COM_GET_JRN_CNT: {
			// ���-�� ������� � ������� �������
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {
				uint16_t t = TO_INT16(buf[B2], buf[B1]);
				stat = sParam_->JrnEntry.setNumJrnEntry(t);
			}
		}
		break;

		case GB_COM_GET_JRN_ENTRY: {
			// ������ ������� �������
			if (sParam_->JrnEntry.getCurrentDevice() == GB_DEVICE_GLB) {

				// ����
				sParam_->JrnEntry.dataTime.setYear(BCD_TO_BIN(buf[B16]));
				sParam_->JrnEntry.dataTime.setMonth(BCD_TO_BIN(buf[B15]));
				sParam_->JrnEntry.dataTime.setDay(BCD_TO_BIN(buf[B14]));
				// B13 - ���� ������
				// �����
				sParam_->JrnEntry.dataTime.setHour(BCD_TO_BIN(buf[B12]));
				sParam_->JrnEntry.dataTime.setMinute(BCD_TO_BIN(buf[B11]));
				sParam_->JrnEntry.dataTime.setSecond(BCD_TO_BIN(buf[B10]));
				uint16_t t = TO_INT16(buf[B9], buf[B8]);
				sParam_->JrnEntry.dataTime.setMsSecond(t);
				// ! B1 - ��� ����������, �� ������ ������ ������������
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
		uint16_t t = sParam_->TxComBuf.getInt16();

		num = addCom(com, t >> 8, t);
	}

	return num;
}
