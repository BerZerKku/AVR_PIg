/*
 * protocolS.cpp
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */
#include "../inc/protocolS.h"

clProtocolS::clProtocolS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
										buf(buf), size(size), sParam(sParam)
{
	enable = false;
	cnt = 0;
	maxLen = 0;
	stat = PRTS_STATUS_NO;
}

/**	����� ����� ������� �������� �������.
 * 	���� ������� �������, �� ����������� ����� �������� - ���� �������������.
 * 	@param ���
 * 	@return ���
 */
bool
clProtocolS::checkReadData()
{
	bool stat = false;

	// �.�. ��������� ������� ��� ��������, ������� ������� �������� ����
	cnt = 0;

	if (checkCRC())
		stat = true;
	else
		this->stat = PRTS_STATUS_NO;

	return stat;
}

/** �������� ��������� �������� � ������
 *  @param ���
 *  @return ���-�� ���� ������ �������������� ��� ��������
 */
uint8_t
clProtocolS::trCom()
{
	this->stat = PRTS_STATUS_NO;

	return maxLen;
}

/**	����������� ��������� ������� � ���� �����.
 * 	����������� ���������� ������ ���� ����� ������� ����������� � ������
 * 	������� �� ��������� ������ ������ ������.
 * 	@param bufSource ����� ������� � ��������
 * 	@return True - � ������ �������� �����������, ����� - False
 */
bool
clProtocolS::copyCommandFrom(uint8_t * const bufSource)
{
	bool stat = true;
	uint8_t cnt = 0;

	if (bufSource[0] != 0x55)
		stat = false;
	else if (bufSource[1] != 0xAA)
		stat = false;
	else
	{
		cnt = bufSource[3] + 5;

		if (cnt > size)
			stat = false;
		else
		{
			for(uint_fast8_t i = 0; i < cnt; i++)
			{
				if (i < size)
					buf[i] = bufSource[i];
			}
		}
	}

	// � ������ �����-���� ������, ���������� ������������
	if (!stat)
	{
		this->stat = PRTS_STATUS_NO;
	}
	else
	{
		this->stat = PRTS_STATUS_WRITE;
		maxLen = cnt;
	}

	return stat;
}


///** �������� �������
// * 	@param com ��� �������
// * 	@param val ���� ������, ���� ����������
// * 	@return ���-�� ������������ ���� ������
// */
//uint8_t
//clProtocolS::addCom(uint8_t com, ePRTS_ACTION act)
//{
//	uint8_t cnt = 0;
//
//	if (act == PRTS_READ_COM)
//	{
//		trCom(0x34, 00);
//	}
//	else
//		trCom(com);
//
//	return cnt;
//}
//
///** ��������� �������� �������, ���������� ������
// * 	@param ���
// * 	@return false - ���� ����� ������� ���
// */
//bool
//clProtocolS::getData(stMNUparam *param)
//{
//	bool tmp = false;
//	switch(buf[2])
//	{
//		case 0x30:
//			param->def_regime = buf[4];
//			param->def_sost = buf[5];
//			param->def_dop = buf[6];
//
//			param->prm_regime = buf[7];
//			param->prm_sost = buf[8];
//			param->prm_dop = buf[9];
//
//			param->prd_regime = buf[10];
//			param->prd_sost = buf[11];
//			param->prd_dop = buf[12];
//			tmp = true;
//			break;
//		case 0x31:
//			param->def_avar = (((uint16_t) buf[4]) << 8) + buf[5];
//			param->def_warn = buf[6];
//
//			param->prm_avar = (((uint16_t) buf[8]) << 8) + buf[9];
//			param->prm_warn = buf[11];
//
//			param->prd_avar = (((uint16_t) buf[12]) << 8) + buf[13];
//			param->prd_warn = buf[15];
//
//			param->glb_avar = (((uint16_t) buf[16]) << 8) + buf[17];
//			param->glb_warn = buf[19];
//			tmp = true;
//			break;
//		case 0x32:
//			param->day = buf[4];
//			param->month = buf[5];
//			param->year = buf[6];
//			param->hour = buf[7];
//			param->minute = buf[8];
//			param->second = buf[9];
//			tmp = true;
//			break;
//	}
//
//	return tmp;
//}

/**	���������� � �������� ������� (���� �������, ���-�� ������ � ������
 * 	��� ������ ������ � ������)
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::addCom()
{
	uint8_t cnt = 0;

	buf[0] = 0x55;
	buf[1] = 0xAA;
	// ������� ����� ���������� ���� ����� �� ������� ������� �
	// ��� ���������� ���-�� ������ ������� ������� ������
	if (buf[2] != 0)
	{
		uint8_t len = buf[3] + 5;
		if (len <= (size - 5))
		{
			buf[len - 1] = getCRC();
			cnt = len;
			maxLen = len;
			stat = PRTS_STATUS_WRITE;
		}
	}


	return cnt;
}


/**	���������� � �������� ������� � ������� (���������� ������)
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
//uint8_t
//clProtocolS::addCom(uint8_t com, uint8_t size, uint8_t buf[])
//{
//	uint8_t cnt = 0;
//
//	if (size < (this->size - 5))
//	{
//		buf[cnt++] = 0x55;
//		buf[cnt++] = 0xAA;
//		buf[cnt++] = com;
//		buf[cnt++] = size;
//		// ��������� ������ � ����� �����������
//		for(uint8_t i = 0; i < size; i++, cnt++)
//			buf[cnt] = buf[i];
//		buf[cnt++] = getCRC();
//
//		maxLen = cnt;
//		stat = PRTS_STATUS_WRITE;
//	}
//
//	return cnt;
//}

/**	���������� � �������� ������� � 1 ������ ������ (���������� ������)
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
//uint8_t
//clProtocolS::addCom(uint8_t com, uint8_t byte)
//{
//	uint8_t cnt = 0;
//
//	buf[cnt++] = 0x55;
//	buf[cnt++] = 0xAA;
//	buf[cnt++] = com;
//	buf[cnt++] = 0x01;
//	buf[cnt++] = byte;
//	buf[cnt++] = com + 0x01 + byte;
//
//	maxLen = cnt;
//	stat = PRTS_STATUS_WRITE;
//
//	return cnt;
//}

/**	���������� � �������� ������� ��� ������ (���������� ������)
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
//uint8_t
//clProtocolS::addCom(uint8_t com)
//{
//	uint8_t cnt = 0;
//
//	buf[cnt++] = 0x55;
//	buf[cnt++] = 0xAA;
//	buf[cnt++] = com;
//	buf[cnt++] = 00;
//	buf[cnt++] = com;
//
//	maxLen = cnt;
//	stat = PRTS_STATUS_WRITE;
//
//	return cnt;
//}


/**	�������� �������� ����������� �����
 * 	@param ���
 * 	@return true - ���� ������ ����������� �����
 */
bool
clProtocolS::checkCRC()
{
	bool stat = false;
	uint8_t crc = 0;
	uint8_t len = this->maxLen - 1;

	for(uint8_t i = 2; i < len; i++)
		crc += this->buf[i];

	if (crc == this->buf[len])
		stat = true;

	return stat;
}

/**	��������� ����������� ����� ����������� ������
 * 	� ���������� �� � ����� �������.
 * 	@param ���
 * 	@return false - � ������ �������� ����� � ������ ��� ��
 *
 */
uint8_t
clProtocolS::getCRC()
{
	uint8_t crc = 0;
	uint8_t len = buf[3] + 5;
	uint8_t i = 2;

	if (len > size)
		return 0;

	for(; i < (len - 1); i++)
		crc += buf[i];

	return crc;
}
