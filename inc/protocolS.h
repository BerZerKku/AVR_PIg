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

// ���-�� ������, ����� �������� �������� ������� ��������� ����� ��������
// � ��������� ��-���������
#define MAX_CYCLE_TO_REST_SOST 10

enum ePRTS_ACTION
{
	PRTS_READ_COM,
	PRTS_WRITE_COM
};

enum ePRTS_STATUS
{
	PRTS_STATUS_NO = 0,		// ��������� ��-���������� (� ��� ����� ���������)
	PRTS_STATUS_READ,		// ���� ���������� ���������
	PRTS_STATUS_READ_OK,	// ���������� ������� ���������, �� �� �� ���������
	PRTS_STATUS_WRITE_PC,	// ���� �������� �������� ��(�) ��
	PRTS_STATUS_WRITE		// ���������� ���������
};

// ������ ���� ������ � ���������
enum ePRTS_DATA_BYTE_NAME
{
	B1 = 4,
	B2,
	B3,
	B4,
	B5,
	B6,
	B7,
	B8,
	B9,
	B10,
	B11,
	B12,
	B13,
	B14,
	B15,
	B16
};

class clProtocolS
{

public:
	clProtocolS	(uint8_t *buf, uint8_t size, stGBparam *sParam);

//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// ����� ��������/������������ ������
	uint8_t * const buf;

	/// �������� ����� ������� �������� �������
	bool checkReadData();

	/// �������� ���������, ���������� ������ ������������ �������
	uint8_t trCom();

	/// ������ ������ ������� ���������
	void setEnable(ePRTS_STATUS stat) { enable_ = true; stat_= statDef_= stat; }

	/// ��������� ������ ������� ���������
	void setDisable() { enable_ = false; }

	/// �������� �������� ��������� ���������
	bool isEnable() const { return enable_; }

	/// ��������� ������ ������ ������
	uint8_t getBufSize() const { return size_; }

	/// ������� ������� � ������
	uint8_t getCurrentCom() const { return buf[2]; }

	/// ������� ���-�� ���� � �������
	uint8_t getCurrentLen() const { return maxLen_; }

	/// ������� ���-�� �������� ���� ������ (�� ���������)
	uint8_t getCurrentCnt() const { return cnt_; }

	/// ������� ������ ������ ���������
	ePRTS_STATUS getCurrentStatus() const { return stat_; }

	/// ����� �������� ������� ������ ���������
	void setCurrentStatus(ePRTS_STATUS stat) { stat_ = stat; }

	/// ����������� ������� �� ������� ������
	bool copyCommandFrom(uint8_t * const bufSource);

	/// ��������� ��������� ���������. � ������ ������� ���������� False.
	virtual bool getData() { stat_ = statDef_; return false; }

	/// �������� �������� ���������
	void checkStat();

	/// �������� ��������� ����� �� ������������ ���������
	/// ���������� false � ������ ������
	/// !!! ���������� � ���������� �� ������
	bool checkByte(uint8_t byte)
	{
		uint8_t cnt = this->cnt_;

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
			if (byte < (size_ - 5))
			{
				cnt++;
				maxLen_ = byte + 5;
			}
			else
				cnt = 0;
			break;
		default:
			cnt++;
			if (cnt >= maxLen_)
			{
				stat_ = PRTS_STATUS_READ_OK;
			}
			break;
		}
		this->cnt_ = cnt;

		return cnt;
	}

protected:
	// ������� ��������� ���������. true - �������
	bool enable_;

	// ��������� ����������
	stGBparam *sParam_;

	// ������� ������ ������ ���������
	ePRTS_STATUS stat_;

	// ������� ��������� ����� �� ���������
	uint8_t cnt_;

	// ���������� ���-�� ���� ������ � �������
	uint8_t maxLen_;

	// ������ ������
	const uint8_t size_;

	// ��������� ������ ��������� ��-���������
	ePRTS_STATUS statDef_;

	// ���������� � �������� ������� (���� �������, ���-�� ������ � ������
	// ��� ������ ������ � ������)
	uint8_t addCom();
	// ���������� � �������� ������� ��� ������ (���������� ������)
	uint8_t addCom	(uint8_t com);
	// ���������� � �������� ������� � 1 ������ ������ (���������� ������)
	uint8_t addCom	(uint8_t com, uint8_t byte);
	// ���������� � �������� ������� � ������� (���������� ������)
//	uint8_t addCom	(uint8_t com, uint8_t size, uint8_t buf[]);

	// �������� �������� ����������� �����
	bool checkCRC();

	// ���������� ����������� ����� ����������� ������
	uint8_t getCRC();
};


#endif /* PROTOCOLS_H_ */
