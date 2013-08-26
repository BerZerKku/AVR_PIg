/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam)
												: clProtocolS(buf, size, sParam)
{

}


/**	��������� ��������� ���������.
 * 	@param ���
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getData()
{
	bool stat = false;
	uint8_t mask = 0;
	eGB_COM com = (eGB_COM) buf[2];

	// ��������� ����������, �������� ���� �� ������
	this->stat_ = PRTS_STATUS_NO;

	// ���� ��� ����� �� ������� ��������� ��������� ��� ������
	// �� ����� ������
	mask = com & GB_COM_MASK_GROUP;
	if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM) ||
		(mask == GB_COM_MASK_GROUP_WRITE_REGIME))
	{
		stat = true;
	}
	else
	{
		mask = com & GB_COM_MASK_DEVICE;
		if (mask == GB_COM_MASK_DEVICE_DEF)
		{
			// ������

			if (com == GB_COM_DEF_GET_DEF_TYPE)
			{
				stat = sParam_->def.setDefType(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_LINE_TYPE)
			{
				stat = sParam_->def.setNumDevices((eGB_NUM_DEVICES) buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_T_NO_MAN)
			{
				stat = sParam_->def.setTimeNoMan(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_DELAY)
			{
				stat = sParam_->def.setDelay(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_OVERLAP)
			{
				stat = sParam_->def.setOverlap(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_RZ_DEC)
			{
				stat = sParam_->def.setRzDec(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_PRM_TYPE)
			{
				stat = sParam_->def.setPrmType(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_RZ_THRESH)
			{
				stat = sParam_->def.setRzThreshold(buf[B1]);
			}
			else if (com == GB_COM_DEF_GET_TYPE_AC)
			{
				// !!! �400
				// �������� ��������� ��������� ���� �� � ������� �� ��
				// 4 ���� - ��� ��
				// 5-8 - ����� �� ��
			}
			else if (com == GB_COM_DEF_GET_JRN_CNT)
			{
				stat = sParam_->def.setNumJrnEntry(TO_INT16(buf[B2], buf[B1]));
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			// ��������

			if (com == GB_COM_PRM_GET_TIME_ON)
			{
				stat = sParam_->prm.setTimeOn(buf[B1]);
			}
			else if (com == GB_COM_PRM_GET_BLOCK_COM)
			{
				sParam_->prm.setBlockCom(0, buf[B1]);
				sParam_->prm.setBlockCom(1, buf[B2]);
				sParam_->prm.setBlockCom(2, buf[B3]);
				sParam_->prm.setBlockCom(3, buf[B4]);
				stat = true;
			}
			else if (com == GB_COM_PRM_GET_TIME_OFF)
			{
				stat = sParam_->prm.setTimeOff(&buf[B1]);
			}
			else if (com == GB_COM_PRM_GET_JRN_CNT)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM)
				{
					uint16_t t = TO_INT16(buf[B2], buf[B1]);
					stat = sParam_->jrnEntry.setNumJrnEntry(t);
				}
			}
			else if (com ==  GB_COM_PRM_GET_JRN_ENTRY)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM)
				{
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
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{
			// ����������

			if (com == GB_COM_PRD_GET_TIME_ON)
			{
				stat = sParam_->prd.setTimeOn(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_DURATION)
			{
				stat = sParam_->prd.setDuration(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_TEST_COM)
			{
				// !!! ���� ������ � ������� ���� �������
				stat = sParam_->prd.setTestCom(buf[B1]);
			}
			else if (com == GB_COM_PRD_GET_BLOCK_COM)
			{
				sParam_->prd.setBlockCom(0, buf[B1]);
				sParam_->prd.setBlockCom(1, buf[B2]);
				sParam_->prd.setBlockCom(2, buf[B3]);
				sParam_->prd.setBlockCom(3, buf[B4]);
				stat = true;
			}
			else if (com == GB_COM_PRD_GET_LONG_COM)
			{
				sParam_->prd.setLongCom(0, buf[B1]);
				sParam_->prd.setLongCom(1, buf[B2]);
				sParam_->prd.setLongCom(2, buf[B3]);
				sParam_->prd.setLongCom(3, buf[B4]);
				stat = true;
			}
			else if (com == GB_COM_PRD_GET_JRN_CNT)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD)
				{
					uint16_t t = TO_INT16(buf[B2], buf[B1]);
					stat = sParam_->jrnEntry.setNumJrnEntry(t);
				}
			}
			else if (com ==  GB_COM_PRD_GET_JRN_ENTRY)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD)
				{
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
		}
		else
		{
			// �����

			if (com == GB_COM_GET_TIME)
			{
				sParam_->dataTime.setYear(BCD_TO_BIN(buf[B1]));
				sParam_->dataTime.setMonth(BCD_TO_BIN(buf[B2]));
				sParam_->dataTime.setDay(BCD_TO_BIN(buf[B3]));
				sParam_->dataTime.setHour(BCD_TO_BIN(buf[B4]));
				sParam_->dataTime.setMinute(BCD_TO_BIN(buf[B5]));
				sParam_->dataTime.setSecond(BCD_TO_BIN(buf[B6]));
				stat = true;
			}
			else if (com == GB_COM_GET_SOST)
			{

				sParam_->def.status.setRegime((eGB_REGIME) buf[B1]);
				sParam_->def.status.setState(buf[B2]);
				sParam_->def.status.setDopByte(buf[B3]);

				sParam_->prm.status.setRegime((eGB_REGIME) buf[B4]);
				sParam_->prm.status.setState(buf[B5]);
				sParam_->prm.status.setDopByte(buf[B6]);

				sParam_->prd.status.setRegime((eGB_REGIME) buf[B7]);
				sParam_->prd.status.setState(buf[B8]);
				sParam_->prd.status.setDopByte(buf[B9]);
				stat = true;
			}
			else if (com == GB_COM_GET_FAULT)
			{
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
			else if (com == GB_COM_GET_MEAS)
			{
				// ���������� �������, ���� ����� ���� ������ ���� ����������
				if (buf[B1] == 0)
				{
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
			}
			else if (com == GB_COM_GET_VERS)
			{
				bool re = false;
				// ������ � ���� ��������
				re |= sParam_->def.status.setEnable(buf[B1] == 1);
				re |= sParam_->prm.setNumCom(buf[B2] * 4);
				// buf[B3] - ���2
				re |= sParam_->prd.setNumCom(buf[B4] * 4);
				re |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES)buf[B5]);
				re |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
				sParam_->glb.setVersBsp(TO_INT16(buf[B7], buf[B8]));
				sParam_->glb.setVersDsp(TO_INT16(buf[B9], buf[B10]));
				re |=sParam_->glb.setCompatibility((eGB_COMPATIBILITY)buf[B11]);

				// ��������� ���� ������������� ��������� ���� ��������
				if (re)
					sParam_->device = false;

				stat = true;
			}
			else if (com == GB_COM_GET_TIME_SINCHR)
			{
				stat = sParam_->glb.setTimeSinchr(buf[B1]);

			}
			else if (com == GB_COM_GET_DEVICE_NUM)
			{
				stat = sParam_->glb.setDeviceNum(buf[B1]);
			}
			else if (com == GB_COM_GET_OUT_CHECK)
			{
				stat = sParam_->glb.setOutCheck(buf[B1]);
			}
			else if (com == GB_COM_GET_CF_THRESHOLD)
			{
				sParam_->glb.setCfThreshold(buf[B1]);
				sParam_->glb.setInDecrease(buf[B2]);
				// B3 - �������� �������� ������� ��� ������� ������
				stat = true;
			}
			else if (com == GB_COM_GET_TIME_RERUN)
			{
				stat = sParam_->glb.setTimeRerun(buf[B1]);
			}
			else if (com == GB_COM_GET_COM_PRM_KEEP)
			{
				stat = sParam_->glb.setComPrmKeep(buf[B1]);
			}
			else if (com == GB_COM_GET_COM_PRD_KEEP)
			{
				// !!! � �400 ��� ��� ���������� �������� (�������������)
				// !!! �������� ��������� ���� ���������� ��������
				if (sParam_->typeDevice == AVANT_K400)
				{
					stat = sParam_->glb.setComPrdKeep(buf[B1]);
				}
			}
			else if (com == GB_COM_GET_JRN_CNT)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB)
				{
					uint16_t t = TO_INT16(buf[B2], buf[B1]);
					stat = sParam_->jrnEntry.setNumJrnEntry(t);
				}
			}
			else if (com == GB_COM_GET_JRN_ENTRY)
			{
				if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB)
				{
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
		}
	}

	return stat;
}

/**	������������ ������� � �������� �������� �������.
 * 	@param com ������� �� ��������
 * 	@return ���-�� ������������ ���
 */
uint8_t
clProtocolBspS::sendData(eGB_COM com)
{
	uint8_t num = 0;
	uint8_t mask = 0;

	mask = com & GB_COM_MASK_GROUP;
	if (mask == GB_COM_MASK_GROUP_WRITE_PARAM)
	{
		// ������� ��������� ����������

		mask = com & GB_COM_MASK_DEVICE;

		if (mask == GB_COM_MASK_DEVICE_DEF)
		{

		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			if (com == GB_COM_PRM_RES_IND)
			{
				num = addCom(com);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{

		}
		else
		{
			if (com == GB_COM_SET_TIME)
			{
				num = addCom(com, 6, sParam_->txComBuf.getBuferAddress());
			}
		}
	}
	else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME)
	{
		// ������� ��������� ������

		mask = com & GB_COM_MASK_DEVICE;

		if (com == GB_COM_SET_CONTROL)
		{
			num = addCom(com, sParam_->txComBuf.getInt8());
		}
		else if (com == GB_COM_PRM_ENTER)
		{
			num = addCom(com);
		}
	}
	else if (mask == GB_COM_MASK_GROUP_READ_PARAM)
	{
		// ������� ������

		mask = com & GB_COM_MASK_DEVICE;
		if (mask == GB_COM_MASK_DEVICE_DEF)
		{
			// ������

			if (com == GB_COM_DEF_GET_DEF_TYPE)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_LINE_TYPE)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_T_NO_MAN)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_DELAY)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_OVERLAP)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_RZ_DEC)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_PRM_TYPE)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_RZ_THRESH)
			{
				num = addCom(com);	/// !!! ���������
			}
			else if (com == GB_COM_DEF_GET_TYPE_AC)
			{
				num = addCom(com);	/// !!! ���������
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			// ��������

			if (com == GB_COM_PRM_GET_TIME_ON)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRM_GET_BLOCK_COM)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRM_GET_TIME_OFF)
			{
				num = addCom(com);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{
			// ����������

			if (com == GB_COM_PRD_GET_TIME_ON)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_DURATION)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_TEST_COM)
			{
				num = addCom(com);	// !!! ���� ������ � ������� ���� �������
			}
			else if (com == GB_COM_PRD_GET_BLOCK_COM)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_LONG_COM)
			{
				num = addCom(com);
			}
		}
		else
		{
			// �����

			if (com == GB_COM_GET_TIME)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_SOST)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_FAULT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_MEAS)
			{
				num = addCom(com, 0);
			}
			else if (com == GB_COM_GET_TIME_SINCHR)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_DEVICE_NUM)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_OUT_CHECK)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_CF_THRESHOLD)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_TIME_RERUN)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_COM_PRM_KEEP)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_COM_PRD_KEEP)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_COM_PRD_KEEP)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_VERS)
			{
				num = addCom(com);
			}
		}
	}
	else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL)
	{
		// ������� ������ � ��������
		mask = com & GB_COM_MASK_DEVICE;
		if (mask == GB_COM_MASK_DEVICE_DEF)
		{
			if (com == GB_COM_DEF_GET_JRN_CNT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_DEF_GET_JRN_ENTRY)
			{
				uint16_t t = sParam_->txComBuf.getInt16();
				num = addCom(com, t >> 8, t);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRM)
		{
			if (com == GB_COM_PRM_GET_JRN_CNT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRM_GET_JRN_ENTRY)
			{
				uint16_t t = sParam_->txComBuf.getInt16();
				num = addCom(com, t >> 8, t);
			}
		}
		else if (mask == GB_COM_MASK_DEVICE_PRD)
		{
			if (com == GB_COM_PRD_GET_JRN_CNT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_PRD_GET_JRN_ENTRY)
			{
				uint16_t t = sParam_->txComBuf.getInt16();
				num = addCom(com, t >> 8, t);
			}
		}
		else
		{
			if (com == GB_COM_GET_JRN_CNT)
			{
				num = addCom(com);
			}
			else if (com == GB_COM_GET_JRN_ENTRY)
			{
				uint16_t t = sParam_->txComBuf.getInt16();
				num = addCom(com, t >> 8, t);
			}
		}
	}

	// ��������� �������, � ����������� �� ������������� �������� ���������
	stat_ = (num != 0) ? PRTS_STATUS_WRITE : PRTS_STATUS_NO;

	return num;
}
