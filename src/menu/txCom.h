/*
 * TTxCom1.h
 *
 *  Created on: 11 ���. 2021 �.
 *      Author: Shcheblykin
 */

#ifndef SRC_MENU_TXCOM_H_
#define SRC_MENU_TXCOM_H_

#include "glbDefine.h"

// ����� ��� �������� ������
class TTxCom {
	static const uint8_t BUFFER_SIZE = 16;

public:
	TTxCom();

	// ������� ������
	void clear();
	void clearFastCom();

	/** ������ ������� � ����� 1.
	 *
	 *  ���� num > 0, �� ���������� ��������� ������� � ������;
	 *  ���� num = 0 ������� �����������.
	 *  @param com ��� �������.
	 *  @param num ������ �������� � ������.
	 *  @retval True - � ������ �������� ������.
	 *  @retval False - ���� �� ������� ��������� ������� � �����.
	 */
	bool addCom1(eGB_COM com, uint8_t num = 0);

	/** ���������������� ������ ��������� � ������ 1 ������ (�� �����).
	 *
	 *  ���� ����� �� ����� ������, �� ����� ������� �������� �� ������.
	 *
	 *  @return ��� ������� �������.
	 *  @return GB_COM_NO ���� ����� ����.
	 */
	eGB_COM getCom1();

	/** ������ ������� � ����� 2.
	 *  @param com ��� �������.
	 *  @retval True - � ������ �������� ������.
	 *  @retval False - ���� �� ������� ��������� ������� � �����.
	 */
	bool addCom2(eGB_COM com);

	/** ���������������� ������ ��������� � ������ 2 ������ (�� �����).
	 *
	 *  ���� �������� ����� ������, �� � ��������� ��� ����� ������� �������
	 *  �� ������ ������.
	 *
	 *  @return ��� ������� �������.
	 *  @return GB_COM_NO ���� ����� ���� ��� �������� ����� ������
	 */
	eGB_COM getCom2();

	/** ������ ������� ������� � ����� �������.
	 *
	 *  ��� ���������� ������� ������� ��� ���������� ������ �������� �����������.
	 *
	 *  @param com ��� ������� �������
	 */
	void addFastCom(eGB_COM com);

	/** ���������� ������� �������. ��� ���� ���� ����������� ������
	 *
	 *  ��� ���������� ������� ������� � ����� �������� ���������� �����������
	 *  ��� ������ ������� ������.
	 *
	 *  @return ��� ������� �������.
	 */
	eGB_COM getFastCom();


	/** ������ ���� ������ � �����.
	 *  @param byte ���� ������.
	 *  @param num ������ �������� �������.
	 */
	void setInt8(uint8_t byte, uint8_t num = 0) {
		if (num < BUFFER_SIZE)
			buf_[0][num] = byte;
	}

	/** ���������� ����� ������.
	 *  @param num ������ �������� �������.
	 *  @return ���� ������.
	 */
	uint8_t getInt8(uint8_t num = 0) const {
		uint8_t byte = 0;
		if (num < BUFFER_SIZE)
			byte = buf_[0][num];
		return byte;
	}

	/** ������ 2-� �������� ����� (uint16_t) � �����.
	 *  � ������ ������ ������������ � 1 � 2 ������� ������� uint8_t.
	 *  @param val ������.
	 */
	void setInt16(uint16_t val) {
		*((uint16_t *) (buf_[0] + 1)) = val;
	}

	/** ���������� 2-� �������� ����� (uint16_t) �� ������.
	 *  ������ �������� � 1 � 2 ��������� ������� uint8_t.
	 *  @return ������.
	 */
	uint16_t getInt16() const {
		return *((uint16_t *) (buf_[0] + 1));
	}

	/** ���������� ��������� �� ����� ������.
	 *  @return ��������� �� ����� ������.
	 */
	uint8_t* getBuferAddress() {
		return &buf_[0] [0];
	}

	/** ���������� ��� ������� ������� �� ��������.
	 *
	 *  @return ��� ������� �� ��������
	 */
	eGB_SEND_TYPE getSendType() const {
		return sendType_;
	}

	/** ��������� ���� ������� �� ��������.
	 *
	 *  @param sendType ��� ������� �� ��������.
	 */
	void setSendType(eGB_SEND_TYPE sendType) {
		sendType_ = sendType;
	}

private:
	// ������� ������� (�� ���������)
	eGB_COM comFast_[MAX_NUM_FAST_COM];
	// ��� ������� �������
	eGB_SEND_TYPE sendType_;
	// ����� ������� ������� �������
	uint8_t cntComFast;
	// ������ ����� ������
	eGB_COM com1_[MAX_NUM_COM_BUF1];
	// ���-�� ������ � ������ ������
	uint8_t numCom1_;
	// ����� ������� ������� � ������ ������
	uint8_t cnt1_;
	// ������ ����� ������
	eGB_COM com2_[MAX_NUM_COM_BUF2];
	// ���-�� ������ �� ������ ������
	uint8_t numCom2_;
	// ����� ������� ������� �� ������ ������
	uint8_t cnt2_;
	// ����� ������ (��� ������ ������� ������� � �������� )
	uint8_t buf_[MAX_NUM_FAST_COM + 1] [BUFFER_SIZE];
};

#endif /* SRC_MENU_TXCOM_H_ */
