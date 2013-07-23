/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "../inc/protocolBspS.h"

clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
												clProtocolS(buf, size, sParam)
{

}


/**	��������� ��������� ���������.
 * 	@param ���
 * 	@return True - � ������ �������� ���������, False - � ������ ������.
 */
bool clProtocolBspS::getData()
{
	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	// ��������� ����������, ����� �����
	this->stat = PRTS_STATUS_NO;

	// ���� ��� ����� �� ������� ��������� ��������� ��� ������
	// �� ����� ������
	if (((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_PARAM) ||
		((com & GB_COM_MASK_GROUP) == GB_COM_MASK_GROUP_WRITE_REGIME))
	{
		stat = true;
	}
	else
	{

		if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_DEF)
		{

		}
		else if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_PRM)
		{

		}
		else if ((com & GB_COM_MASK_DEVICE) == GB_COM_MASK_DEVICE_PRD)
		{

		}
		else
		{
			if (com == GB_COM_GET_TIME)
			{
				sParam->dataTime.setYearFromBCD(buf[B1]);
				sParam->dataTime.setMonthFromBCD(buf[B2]);
				sParam->dataTime.setDayFromBCD(buf[B3]);
				sParam->dataTime.setHourFromBCD(buf[B4]);
				sParam->dataTime.setMinuteFromBCD(buf[B5]);
				sParam->dataTime.setSecondFromBCD(buf[B6]);
			}
			else if (com == GB_COM_GET_SOST)
			{
				sParam->def.status.setRegime(buf[B1]);
				sParam->def.status.setState(buf[B2]);
				sParam->def.status.setDopByte(buf[B3]);

				sParam->prm.status.setRegime(buf[B4]);
				sParam->prm.status.setState(buf[B5]);
				sParam->prm.status.setDopByte(buf[B6]);

				sParam->prd.status.setRegime(buf[B7]);
				sParam->prd.status.setState(buf[B8]);
				sParam->prd.status.setDopByte(buf[B9]);
			}
			else if (com == GB_COM_GET_FAULT)
			{
				sParam->def.status.setFault(TO_INT16(buf[B1], buf[B2]));
				sParam->def.status.setWarning(TO_INT16(buf[B3], buf[B4]));

				sParam->prm.status.setFault(TO_INT16(buf[B5], buf[B6]));
				sParam->prm.status.setWarning(TO_INT16(buf[B7], buf[B8]));

				sParam->prd.status.setFault(TO_INT16(buf[B9], buf[B10]));
				sParam->prd.status.setWarning(TO_INT16(buf[B11], buf[B12]));

				sParam->glb.status.setFault(TO_INT16(buf[B13], buf[B14]));
				sParam->glb.status.setWarning(TO_INT16(buf[B15], buf[B16]));
			}
			else if (com == GB_COM_GET_MEAS)
			{
				// ���������� �������, ���� ����� ���� ������ ���� ����������
				if (buf[B1] == 0)
				{
					sParam->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
					sParam->measParam.setCurrentOut(TO_INT16(buf[B4], buf[B5]));
					// � buf[B7] ��������� ������� ����� ���������� * 100
					// �.�. ���� ��� 90, �� ��� 0.9�.
					sParam->measParam.setVoltageOut(buf[B6], (buf[B7] / 10));
					sParam->measParam.setVoltageDef(buf[B8]);
					// B9 ������� ��� U� ������ �����
					sParam->measParam.setVoltageCf(buf[B10]);
					// B11 ���� ������� ��� Uk ������ �����
					sParam->measParam.setVoltageNoise(buf[B12]);
					// B13 ���� ������� ��� ���������� ������������ ����� ���
					// B14, B15 ����� �������� ��� ����������� �������� ������
				}
			}
			else if (com == GB_COM_GET_VERS)
			{
				bool re = false;
				// ������ � ���� ��������
				re |= sParam->def.status.setEnable(buf[B1] == 1);
				re |= sParam->prm.setNumCom(buf[B2] * 4);
				// buf[B3] - ���2
				re |= sParam->prd.setNumCom(buf[B4] * 4);
				re |= sParam->glb.setNumDevices((eGB_NUM_DEVICES)buf[B5]);
				re |= sParam->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
				sParam->glb.setVersBsp(TO_INT16(buf[B7], buf[B8]));
				sParam->glb.setVersDsp(TO_INT16(buf[B9], buf[B10]));
				re |= sParam->glb.setCompatibility((eGB_COMPATIBILITY)buf[B11]);

				// ��������� ���� ������������� ��������� ���� ��������
				if (re)
					sParam->device = false;

				// ����������� ���� ��������
				if (buf[B1] == 1)
				{
					// ������ ����

				}
				else
				{

				}

				sDebug.byte1 = sParam->prm.getNumCom();
				sDebug.byte2 = sParam->prd.getNumCom();

				sDebug.byte5 = sParam->glb.getNumDevices();
				sDebug.byte6 = sParam->glb.getTypeLine();
				sDebug.byte7 = sParam->glb.getCompatibility();
			}
		}
	}

	return stat;
}
