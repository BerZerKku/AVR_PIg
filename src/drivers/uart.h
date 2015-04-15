/**
 * uart.h
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 *
 *     ������ � UART.
 *     ��������� ������ ����� ������ ������������ �������.
 */

#ifndef UART_H_
#define UART_H_

#include "glbDefine.h"
#include "paramUart.h"

/// ����
enum eUART_PORT {
	UART_UART0, 				//
	UART_UART1					//
};

///	���������������� ����.
class TUart {
public:

	/** �����������.
	 * 	����� �����-������ ������ ���� ��������� �������.
	 * 	@param port ��� �����.
	 * 	@param *buf ��������� �� ����� ������.
	 * 	@param size ������ ������.
	 */
	TUart(eUART_PORT port, uint8_t * const buf, uint8_t size);

	/**	��������� ����.
	 * 	@param baud	�������� ������ �����.
	 * 	@param databits	���������� ����� ������.
	 * 	@param parity �������� �������� ��������.
	 * 	@param stopbits ���������� ���� �����.
	 * 	@return True - � ������ ��������� ��������
	 */
	bool open(eUART_BAUD_RATE baud, eUART_DATA_BITS databits,
			eUART_PARITY parity, eUART_STOP_BITS stopbits);

	/** ��������� ����.
	 */
	void close();

	/*	���������� ������� ��������� �����.
	 * 	@retval True ���� ������
	 * 	@retval False ���� ������.
	 */
	bool isOpen() const;

	// ������� �������� ������
	void trByte(uint8_t byte);
	uint8_t trData(uint8_t size);
	uint8_t trData(uint8_t size, const uint8_t *buf);
	uint8_t trBuf();

	/**	���������� ���-�� ���� ������ � ������.
	 */
	uint8_t getCnt() const {
		return cnt_;
	}

	/**	����� �������� ���� ������ � ������.
	 */
	void clrCnt() {
		cnt_ = 0;
	}

	/**	���������� ���������� ����������� ������ ����������.
	 * 	������ ������������� � ����������!!!
	 */
	void isrUDR() {
		if (cnt_ < numTrByte_) {
			*udr_ = buf_[cnt_++];
		} else {
			*ucsrb_ &= ~(1 << UDRIE1);
		}
	}

	/**	���������� ���������� ��������� ��������.
	 * 	����������� ���������� �� ��������, ����������� ���������� �� �����.
	 * 	������ ������������� � ����������!!!
	 */
	void isrTX() {
		cnt_ = 0;
		numTrByte_ = 0;
		*ucsrb_ |= (1 << RXCIE);
		*ucsrb_ &= ~(1 << TXCIE);

	}

	/**	���������� ���������� �� �������� ����� � ����� ���������.
	 * 	������ ������������� � ����������!!!
	 * 	@param byte �������� ���� ������
	 */
	void isrRX(uint8_t byte) {
		if (cnt_ < size_) {
			buf_[cnt_++] = byte;
		}
	}

private:
	// ���-�� ��������/���������� ������
	uint8_t cnt_;
	// ��������� �� �����
	uint8_t * const buf_;
	// ������ ������
	const uint8_t size_;
	// ���-�� ������ �� ��������
	uint8_t numTrByte_;

	// ������� ������
	volatile uint8_t *udr_;

	// �������� ���������
	volatile uint8_t *ubbrh_;
	volatile uint8_t *ubbrl_;
	volatile uint8_t *ucsrc_;
	volatile uint8_t *ucsrb_;
	volatile uint8_t *ucsra_;
};

#endif /* UART_H_ */
