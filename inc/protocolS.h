/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <menu.h>

class clProtocolS
{
public:
	clProtocolS(uint8_t *buf, uint8_t size, stMNUparam *param);

	bool checkCRC	();
	uint8_t trData	(uint8_t com, uint8_t size, const uint8_t * buf);
	uint8_t trByte	(uint8_t com, uint8_t byte);
	bool getData	();

	// �������� ��������� ����� �� ������������ ���������
	// ���������� false � ������ ������
	// !!! ���������� � ���������� �� ������
	bool checkByte(uint8_t byte)
	{
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
					rdy = true;
				break;
		}

		return cnt;
	}

	/// �������� ����� �������� �������
	bool isRdy() const { return rdy; };

	/// ����� ����� �������� �������
	void clrRdy() { rdy = false; cnt = 0;}

	/// ������ ������ ������� ���������
	void setEnable() { enable = true; clrRdy(); }

	/// ��������� ������ ������� ���������
	void setDisable() { enable = false; }

	/// �������� �������� ��������� ���������
	bool isEnable() const { return enable; }

	/// ���-�� ������ � �������
	uint8_t getLenPackage() const { return cnt; }

private:
	// ������� ��������� ����� �� ���������
	uint8_t cnt;
	// ���������� ���-�� ���� ������ � �������
	uint8_t maxLen;
	// ��������� �� �����
	uint8_t * const buf;
	// ������ ������
	const uint8_t size;
	// ���� �������� �� ������������ ���������
	bool rdy;
	// ������� ��������� ���������. true - �������
	bool enable;
	//
	stMNUparam *param;

	// ���������� � ������� ����������� �����
	bool setCRC();
};


#endif /* PROTOCOLS_H_ */
