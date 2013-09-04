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

enum eUART_PORT
{
	UART_UART0,
	UART_UART1
};

class clUart
{
public:

//	~clUart();
	clUart(eUART_PORT port, uint8_t * const buf, uint8_t size);

	volatile uint8_t *udr;

	void 	init	(uint16_t baudrate);
	void 	trByte	(uint8_t byte);
	uint8_t trData	(uint8_t size);
	uint8_t trData	(uint8_t size, const uint8_t *buf);
	uint8_t trBuf	();

	/// ���������� �������� ���� ������ � ������
	uint8_t getCnt() const { return cnt; }

	/// �������� ������� ���� ������ � ������
	void clrCnt() {cnt = 0;}

	/**	���������� ���������� ����������� ������ ����������.
	 * 	�� ������ ����������� � ������ ���������� !!
	 */
	void isrUDR()
	{
		if (cnt < numTrByte)
		{
			*udr = buf[cnt++];
		}
		else
		{
			*ucsrb &= ~(1 << UDRIE1);
		}
	}

	/**	���������� ���������� ��������� ��������.
	 * 	��������� ��������, ��������� �����.
	 * 	�� ������ ����������� � ������ ���������� !!
	 */
	void isrTX()
	{
		cnt = 0;
		numTrByte = 0;
		*ucsrb |= (1 << RXCIE1);
		*ucsrb &= ~(1 << TXCIE1);

	}

	/**	���������� ���������� �� �������� ����� � ����� ���������.
	 * 	@param byte �������� ���� ������
	 * 	�� ������ ����������� � ������ ���������� !!
	 */
	void isrRX(uint8_t byte)
	{
		if (cnt < size)
		{
			buf[cnt++] = byte;
		}
	}

private:
	// ���-�� ��������/���������� ������
	volatile uint8_t cnt;
	// ��������� �� �����
	uint8_t	* const buf;
	// ������ ������
	const uint8_t size;
	// ���-�� ������ �� ��������
	uint8_t numTrByte;


	// �������� ���������
	volatile uint8_t *ubbrh;
	volatile uint8_t *ubbrl;
	volatile uint8_t *ucsrc;
	volatile uint8_t *ucsrb;
};

#endif /* UART_H_ */
