/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <stdint.h>
#include "glbDefine.h"
#include "debug.h"

enum ePRTS_ACTION
{
	PRTS_READ_COM,
	PRTS_WRITE_COM
};

enum ePRTS_STATUS
{
	PRTS_STATUS_NO = 0,		// ��������� ��-���������� (� ��� ����� ���������)
	PRTS_STATUS_READ,		// ������� ������� (�� ��� �� ��������� ��)
	PRTS_STATUS_WRITE		// � ������ ���� �������, ���� ��������
};

class clProtocolS
{

public:
	clProtocolS	(uint8_t *buf, uint8_t size, stGBparam *sParam);

//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// ��������� �� �����
	uint8_t * const buf;

	/// �������� ����� ������� �������� �������
	bool checkReadData();

	/// �������� ���������, ���������� ������ ������������ �������
	uint8_t trCom();

	/// ������ ������ ������� ���������
	void setEnable() { enable = true; stat = PRTS_STATUS_NO; }

	/// ��������� ������ ������� ���������
	void setDisable() { enable = false; }

	/// �������� �������� ��������� ���������
	bool isEnable() const { return enable; }

	/// ��������� ������ ������ ������
	uint8_t getBufSize() const { return size; }

	/// ������� ������� � ������
	uint8_t getCurrentCom() const { return buf[2]; }

	/// ������� ���-�� ���� � �������
	uint8_t getCurrentLen() const { return maxLen; }

	/// ������� ���-�� �������� ���� ������ (�� ���������)
	uint8_t getCurrentCnt() const { return cnt; }

	/// ������� ������ ������ ���������
	ePRTS_STATUS getCurrentStatus() const { return this->stat; }

	/// ����������� ������� �� ������� ������
	bool copyCommandFrom(uint8_t * const bufSource);

	/// ��������� ��������� ���������. � ������ ������� ���������� False.
	virtual bool getData() { stat = PRTS_STATUS_NO; return false; }

	///
//	bool sendData(uint8_t com, uint8_t *data);


	/// �������� ��������� ����� �� ������������ ���������
	/// ���������� false � ������ ������
	/// !!! ���������� � ���������� �� ������
	bool checkByte(uint8_t byte)
	{
		uint8_t cnt = this->cnt;

		buf[cnt] = byte;

		switch(cnt)
		{
		case 0:
			if (byte == 0x55)
				cnt++;
			break;
		case 1:
			cnt = (byte == 0xAA) ? 2 : 0;
			break;
		case 2:
			cnt++;
			break;
		case 3:
			if (byte < (size - 5))
			{
				cnt++;
				maxLen = byte + 5;
			}
			else
				cnt = 0;
			break;
		default:
			cnt++;
			if (cnt >= maxLen)
			{
				stat = PRTS_STATUS_READ;
			}
			break;
		}
		this->cnt = cnt;

		return cnt;
	}

protected:
	// ������� ��������� ���������. true - �������
	bool enable;

	// ��������� ����������
	stGBparam *sParam;

	// ������� ������ ������ ���������
	ePRTS_STATUS stat;

	// ������� ��������� ����� �� ���������
	uint8_t cnt;

	// ���������� ���-�� ���� ������ � �������
	uint8_t maxLen;

	// ������ ������
	const uint8_t size;

	// ���������� � �������� ������� (���� �������, ���-�� ������ � ������
	// ��� ������ ������ � ������)
	uint8_t addCom();
	// ���������� � �������� ������� ��� ������ (���������� ������)
//	uint8_t addCom	(uint8_t com);
	// ���������� � �������� ������� � 1 ������ ������ (���������� ������)
//	uint8_t addCom	(uint8_t com, uint8_t byte);
	// ���������� � �������� ������� � ������� (���������� ������)
//	uint8_t addCom	(uint8_t com, uint8_t size, uint8_t buf[]);

	// �������� �������� ����������� �����
	bool checkCRC();

	// ���������� ����������� ����� ����������� ������
	uint8_t getCRC();
};


#endif /* PROTOCOLS_H_ */
