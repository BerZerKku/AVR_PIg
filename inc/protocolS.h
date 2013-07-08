/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

enum ePRTS_ACTION
{
	PRTS_READ_COM,
	PRTS_WRITE_COM
};

enum ePRTS_STATUS
{
	PRTS_STATUS_NO = 0,		// ��������� ��-���������� (� ��� ����� ���������)
	PRTS_STATUS_READ,		// ������� �������, �� ��� �� ��������� ��
	PRTS_STATUS_READ_OK,	// ������� ������� � ��������� ��
	PRTS_STATUS_WRITE,		// � ������ ���� �������, ���� ��������
};

class clProtocolS
{

public:
	clProtocolS	(uint8_t *buf, uint8_t size);

//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// ��������� �� �����
	uint8_t * const buf;

	/// �������� ����� ������� �������� �������
	bool checkReadData();

	/// ����� ����� �������� �������
//	void clrRdy() { this->rdy = false; this->cnt = 0; }

	/// ������ ������ ������� ���������
	void setEnable() { this->enable = true; this->stat = PRTS_STATUS_NO;}

	/// ��������� ������ ������� ���������
	void setDisable() { this->enable = false; }

	/// �������� �������� ��������� ���������
	bool isEnable() const { return this->enable; }

	/// �������� ���������, ���������� ������ ������������ �������
	uint8_t trCom() { this->stat = PRTS_STATUS_NO; return this->cnt; }

	/// ������� ������� � ������
	uint8_t getCurrentCom() const { return this->buf[2]; }

	/// ������� ������ ������ ���������
	ePRTS_STATUS getCurrentStatus() const { return this->stat; }

	/// ����������� ������� �� ������� ������
	bool copyCommandFrom(uint8_t * const bufSource);

	///
	bool getData();

	/// �������� ��������� ����� �� ������������ ���������
	/// ���������� false � ������ ������
	/// !!! ���������� � ���������� �� ������
	bool checkByte(uint8_t byte)
	{
		uint8_t cnt = this->cnt;

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
			if (byte < (this->size - 5))
			{
				cnt++;
				this->maxLen = byte + 5;
			}
			else
				cnt = 0;
			break;
		default:
			cnt++;
			if (cnt >= this->maxLen)
				this->stat = PRTS_STATUS_READ;
			break;
		}

		this->cnt = cnt;
		return cnt;
	}

private:
	// ������� ������ ������ ���������
	ePRTS_STATUS stat;

	// ������� ��������� ����� �� ���������
	uint8_t cnt;

	// ���������� ���-�� ���� ������ � �������
	uint8_t maxLen;

	// ������ ������
	const uint8_t size;

	// ������� ��������� ���������. true - �������
	bool enable;

	// ���������� � �������� ������� ��� ������ (���������� ������)
	uint8_t addCom	(uint8_t com);
	// ���������� � �������� ������� � 1 ������ ������ (���������� ������)
	uint8_t addCom	(uint8_t com, uint8_t byte);
	// ���������� � �������� ������� � ������� (���������� ������)
	uint8_t addCom	(uint8_t com, uint8_t size, const uint8_t * buf);

	// �������� �������� ����������� �����
	bool checkCRC();

	// ���������� ����������� ����� ����������� ������
	uint8_t getCRC();
};


#endif /* PROTOCOLS_H_ */
