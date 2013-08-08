/*
 * protocolS.cpp
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */
#include "../inc/protocolS.h"

clProtocolS::clProtocolS(uint8_t *buf, uint8_t size, stGBparam *sParam)
									: buf(buf), size_(size), sParam_(sParam)
{
	enable_ = false;
	cnt_ = 0;
	maxLen_ = 0;
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
	cnt_ = 0;

	if (checkCRC())
		stat = true;
	else
		this->stat_ = PRTS_STATUS_NO;

	return stat;
}

/** �������� ��������� �������� � ������
 *  @param ���
 *  @return ���-�� ���� ������ �������������� ��� ��������
 */
uint8_t
clProtocolS::trCom()
{
	this->stat_ = PRTS_STATUS_WRITE;

	return maxLen_;
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

		if (cnt > size_)
			stat = false;
		else
		{
			for(uint_fast8_t i = 0; i < cnt; i++)
			{
				if (i < size_)
					buf[i] = bufSource[i];
			}
		}
	}

	// � ������ �����-���� ������, ���������� ������������
	if (!stat)
	{
		this->stat_ = PRTS_STATUS_NO;
	}
	else
	{
		this->stat_ = PRTS_STATUS_WRITE_PC;
		maxLen_ = cnt;
	}

	return stat;
}

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
		if (len <= (size_ - 5))
		{
			buf[len - 1] = getCRC();
			cnt = len;
			maxLen_ = len;
			stat_ = PRTS_STATUS_WRITE;
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
uint8_t
clProtocolS::addCom(uint8_t com, uint8_t byte)
{
	uint8_t cnt = 0;

	buf[cnt++] = 0x55;
	buf[cnt++] = 0xAA;
	buf[cnt++] = com;
	buf[cnt++] = 0x01;
	buf[cnt++] = byte;
	buf[cnt++] = com + 0x01 + byte;

	maxLen_ = cnt;
	stat_ = PRTS_STATUS_WRITE;

	return cnt;
}

/**	���������� � �������� ������� ��� ������ (���������� ������)
 * 	@param *buf ��������� �� ������ ������
 * 	@param size ���-�� ���� ������
 * 	@return ���-�� ������������ ���� ������
 */
uint8_t
clProtocolS::addCom(uint8_t com)
{
	uint8_t cnt = 0;

	buf[cnt++] = 0x55;
	buf[cnt++] = 0xAA;
	buf[cnt++] = com;
	buf[cnt++] = 00;
	buf[cnt++] = com;

	maxLen_ = cnt;
	stat_ = PRTS_STATUS_WRITE;

	return cnt;
}


/**	�������� �������� ����������� �����
 * 	@param ���
 * 	@return true - ���� ������ ����������� �����
 */
bool
clProtocolS::checkCRC()
{
	bool stat = false;
	uint8_t crc = 0;
	uint8_t len = this->maxLen_ - 1;

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

	if (len > size_)
		return 0;

	for(; i < (len - 1); i++)
		crc += buf[i];

	return crc;
}
