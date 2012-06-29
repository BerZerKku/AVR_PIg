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
clProtocolS::trData(uint8_t com, uint8_t size, const uint8_t *buf)
{
	uint8_t cnt = 0;

	if (size <= (this->size - 4))
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = size;

		// ��������� ������ � �����
		for(uint8_t i = 0; i < size; i++, cnt++)
			this->buf[cnt] = buf[i];

		setCRC();
	}

	// +1 - ���� ����������� �����
	return (cnt + 1);
}

/**	���������� ������ ��� �������� ������ ����� ������
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::trByte(uint8_t com, uint8_t byte)
{
	uint8_t cnt = 0;

	if (size >= 5)
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = 01;
		this->buf[cnt++] = byte;

		setCRC();
	}

	// +1 - ���� ����������� �����
	return (cnt + 1);
}

/** ��������� �������� �������, ��������� ������
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
			sDebug.byte1= 0x30;
			tmp = true;
			break;
		case 0x31:
			sDebug.byte1= 0x31;
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
bool
clProtocolS::setCRC()
{
	uint8_t crc = 0;
	uint8_t len = buf[3] + 5;
	uint8_t i = 2;

	if (len > size)
		return false;

	for(; i < (len - 1); i++)
		crc += buf[i];
	buf[i] = crc;

	return true;
}
