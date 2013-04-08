/*
 * protocolS.cpp
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#include <stdint.h>
#include "protocolS.h"
#include "debug.h"

clProtocolS::clProtocolS(uint8_t *buf, uint8_t size, stMNUparam *param) :
							buf(buf), size(size), param(param)
{
	enable = false;
	rdy = false;
	cnt = 0;
	maxLen = 0;
}

/**	�������� �������� ����������� �����
 * 	@param ���
 * 	@return true - ���� ������ ����������� �����
 *
 */
bool
clProtocolS::checkCRC()
{
	uint8_t crc = 0;

	for(uint8_t i = 2; i < (maxLen - 1); i++)
		crc += buf[i];

	if (crc == buf[maxLen - 1])
		return true;

	return false;
}

/**	���������� ������ ��� �������� ������ �� �������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::trCom(uint8_t com, uint8_t size, const uint8_t *buf)
{
	uint8_t cnt = 0;

	if (size <= (this->size - 4))
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = size;

		// ��������� ������ � ����� �����������
		for(uint8_t i = 0; i < size; i++, cnt++)
			this->buf[cnt] = buf[i];

		this->buf[cnt++] = getCRC();
	}

	return cnt;
}

/**	���������� ������ ��� �������� ������ ����� ������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::trCom(uint8_t com, uint8_t byte)
{
	uint8_t cnt = 0;

	if (size >= 5)
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = 0x01;
		this->buf[cnt++] = byte;
		this->buf[cnt++] = com + 0x01 + byte;
	}

	return cnt;
}

/**	���������� ������ ��� �������� ������� ��� ������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::trCom(uint8_t com)
{
	uint8_t cnt = 0;

	if (size >= 4)
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = 00;
		this->buf[cnt++] = com;
	}

	return cnt;
}

/** �������� �������
 * 	@param com ��� �������
 * 	@param val ���� ������, ���� ����������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::addCom(uint8_t com, ePRTS_ACTION act)
{
	uint8_t cnt = 0;

	if (act == PRTS_READ_COM)
	{
		trCom(0x34, 00);
	}
	else
		trCom(com);

	return cnt;
}

/** ��������� �������� �������, ���������� ������
 * 	@param ���
 * 	@return false - ���� ����� ������� ���
 */
bool
clProtocolS::getData()
{
	bool tmp = false;
	switch(buf[2])
	{
		case 0x30:
			param->def_regime = buf[4];
			param->def_sost = buf[5];
			param->def_dop = buf[6];

			param->prm_regime = buf[7];
			param->prm_sost = buf[8];
			param->prm_dop = buf[9];

			param->prd_regime = buf[10];
			param->prd_sost = buf[11];
			param->prd_dop = buf[12];
			tmp = true;
			break;
		case 0x31:
			param->def_avar = (((uint16_t) buf[4]) << 8) + buf[5];
			param->def_warn = buf[6];

			param->prm_avar = (((uint16_t) buf[8]) << 8) + buf[9];
			param->prm_warn = buf[11];

			param->prd_avar = (((uint16_t) buf[12]) << 8) + buf[13];
			param->prd_warn = buf[15];

			param->glb_avar = (((uint16_t) buf[16]) << 8) + buf[17];
			param->glb_warn = buf[19];
			tmp = true;
			break;
		case 0x32:
			param->day = buf[4];
			param->month = buf[5];
			param->year = buf[6];
			param->hour = buf[7];
			param->minute = buf[8];
			param->second = buf[9];
			tmp = true;
			break;
	}

	return tmp;
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
