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
				sParam->dataTime.setYearFromBCD(buf[4]);
				sParam->dataTime.setMonthFromBCD(buf[5]);
				sParam->dataTime.setDayFromBCD(buf[6]);
				sParam->dataTime.setHourFromBCD(buf[7]);
				sParam->dataTime.setMinuteFromBCD(buf[8]);
				sParam->dataTime.setSecondFromBCD(buf[9]);
			}
			else if (com == GB_COM_GET_SOST)
			{
				sParam->def.status.setRegime(buf[4]);
				sParam->def.status.setState(buf[5]);
				sParam->def.status.setDopByte(buf[6]);

				sParam->prm.status.setRegime(buf[7]);
				sParam->prm.status.setState(buf[8]);
				sParam->prm.status.setDopByte(buf[9]);

				sParam->prd.status.setRegime(buf[10]);
				sParam->prd.status.setState(buf[11]);
				sParam->prd.status.setDopByte(buf[12]);
			}
			else if (com == GB_COM_GET_FAULT)
			{
				sParam->def.status.setFault(TO_INT16(buf[4], buf[5]));
				sParam->def.status.setWarning(TO_INT16(buf[6], buf[7]));

				sParam->prm.status.setFault(TO_INT16(buf[8], buf[9]));
				sParam->prm.status.setWarning(TO_INT16(buf[10], buf[11]));

				sParam->prd.status.setFault(TO_INT16(buf[12], buf[13]));
				sParam->prd.status.setWarning(TO_INT16(buf[14], buf[15]));

				sParam->glb.status.setFault(TO_INT16(buf[16], buf[17]));
				sParam->glb.status.setWarning(TO_INT16(buf[18], buf[19]));
			}
			else if (com == GB_COM_GET_MEAS)
			{
				// ���������� �������, ���� ����� ���� ������ ���� ����������
				if (buf[4] == 0)
				{
					sParam->measParam.setResistOut(TO_INT16(buf[5], buf[6]));
					sParam->measParam.setCurrentOut(TO_INT16(buf[7], buf[8]));
					// � buf[10] ��������� ������� ����� ���������� * 100
					// �.�. ���� ��� 90, �� ��� 0.9�.
					sParam->measParam.setVoltageOut(buf[9], (buf[10] / 10));
					sParam->measParam.setVoltageDef(buf[11]);
					// 12 ���� ������� ��� U� ������ �����
					sParam->measParam.setVoltageCf(buf[13]);
					// 14 ���� ������� ��� Uk ������ �����
					sParam->measParam.setVoltageNoise(buf[15]);
					// 15 ���� ������� ��� ���������� ������������ ����� ���
					// 16, 17 ����� �������� ��� ����������� �������� ������
				}
			}
		}
	}

	return stat;
}
