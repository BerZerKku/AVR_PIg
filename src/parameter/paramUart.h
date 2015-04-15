/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

// ��������� �����
enum eGB_INTERFACE {
	GB_INTERFACE_MIN = 0,	//
	GB_INTERFACE_USB = 0,	// ����������� ����� USB �� �������� ������
	GB_INTERFACE_RS485,		// ����������� ����� 485 ��������� �� ������ ������
	GB_INTERFACE_MAX		//
};

/// �������� ��������
enum eUART_BAUD_RATE {
	UART_BAUD_RATE_MIN = 0,		//
	UART_BAUD_RATE_1200 = 0,	//
	UART_BAUD_RATE_2400,		//
	UART_BAUD_RATE_4800,		//
	UART_BAUD_RATE_9600,		//
	UART_BAUD_RATE_19200,		//
	UART_BAUD_RATE_38400,		//
	UART_BAUD_RATE_57600,		//
	UART_BAUD_RATE_MAX			//
};

/// ���-�� ���� ������
enum eUART_DATA_BITS {
	UART_DATA_BITS_MIN = 0,		//
	UART_DATA_BITS_8 = 0,		//
	UART_DATA_BITS_MAX			//
};

/// ��������
enum eUART_PARITY {
	UART_PARITY_MIN = 0,		//
	UART_PARITY_NONE = 0,		// ���
	UART_PARITY_EVEN,			// ����� ������������� ����� ������ �����
	UART_PARITY_ODD,			// ����� ������������� ����� ������ �������
//	UART_PARITY_MARK,			// ��� �������� ������ 1
//	UART_PARITY_SPACE,			// ��� �������� ������ 0
	UART_PARITY_MAX
};

/// ���-�� ����-���
enum eUART_STOP_BITS {
	UART_STOP_BITS_MIN = 0,		//
	UART_STOP_BITS_ONE = 0,		//
//	UART_STOP_BITS_ONEPONTFIVE,	//
	UART_STOP_BITS_TWO,			//
	UART_STOP_BITS_MAX			//
};


struct sEeprom {
	uint16_t 		password;
	eGB_INTERFACE 	interface;
	eUART_BAUD_RATE baudRate;
	eUART_DATA_BITS dataBits;
	eUART_PARITY 	parity;
	eUART_STOP_BITS stopBits;
};


/// ��������� �����
class TInterface {
public:
	/**	�����������.
	 * 	�� ��������� ������������� ����� �� USB.
	 */
	TInterface(eGB_INTERFACE val = GB_INTERFACE_USB) {
		if (!set(val))
			interface_ = GB_INTERFACE_USB;
	}

	/**	������
	 * 	@param val ��������� �����.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(eGB_INTERFACE val) {
		bool stat = false;
		if ((val >= GB_INTERFACE_MIN) && (val < GB_INTERFACE_MAX)) {
			interface_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@return ��������� �����
	 */
	eGB_INTERFACE get() const {
		return interface_;
	}

private:
	// ���-�� ����� ������
	eGB_INTERFACE interface_;
};

/// �������� ��������.
class TBaudRate {
public:
	/**	�����������.
	 * 	�� ��������� ������������� �������� 19200 ���/�.
	 */
	TBaudRate(eUART_BAUD_RATE val = UART_BAUD_RATE_19200) {
		if (!set(val))
			baudRate_ = UART_BAUD_RATE_19200;
	}

	/**	������
	 * 	@param val �������� ��������
	 * 	@return
	 * 	@retval False � ������ ���������� ��������
	 */
	bool set(eUART_BAUD_RATE val) {
		bool stat = false;
		if ((val >= UART_BAUD_RATE_MIN) && (val < UART_BAUD_RATE_MAX)) {
			baudRate_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@return �������� ��������
	 */
	eUART_BAUD_RATE get() const {
		return baudRate_;
	}

private:
	// �������� ��������
	eUART_BAUD_RATE baudRate_;
};

/// ���������� ����� ������
class TDataBits {
public:
	/**	�����������.
	 * 	�� ��������� ������������� 8 ��� ������.
	 */
	TDataBits(eUART_DATA_BITS val = UART_DATA_BITS_8) {
		if (!set(val))
			dataBits_ = UART_DATA_BITS_8;
	}

	/**	������
	 * 	@param val ���-�� ����� ������.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(eUART_DATA_BITS val) {
		bool stat = false;
		if ((val >= UART_DATA_BITS_MIN) && (val < UART_DATA_BITS_MAX)) {
			dataBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return ���-�� ����� ������
	 */
	eUART_DATA_BITS get() const {
		return dataBits_;
	}

private:
	// ���-�� ����� ������
	eUART_DATA_BITS dataBits_;
};

///	�������� �������� ��������
class TParity {
public:
	/**	�����������.
	 * 	�� ��������� �������� �������� ���������.
	 */
	TParity(eUART_PARITY val = UART_PARITY_NONE) {
		if (!set(val))
			parity_ = UART_PARITY_NONE;
	}

	/**	������
	 * 	@param val �������� ��������
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(eUART_PARITY val) {
		bool stat = false;
		if ((val >= UART_PARITY_MIN) && (val < UART_PARITY_MAX)) {
			parity_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return �������� ��������
	 */
	eUART_PARITY get() const {
		return parity_;
	}

private:
	// �������� ��������
	eUART_PARITY parity_;
};

///	����� �������� �����
class TStopBits {
public:
	/**	�����������.
	 * 	�� ��������� 2 ���� ����.
	 */
	TStopBits(eUART_STOP_BITS val = UART_STOP_BITS_TWO) {
		if (!set(val))
			stopBits_ = UART_STOP_BITS_TWO;
	}

	/**	������
	 * 	@param val ����� �������� �����
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(eUART_STOP_BITS val) {
		bool stat = false;
		if ((val >= UART_STOP_BITS_MIN) && (val < UART_STOP_BITS_MAX)) {
			stopBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return  ����� �������� �����
	 */
	eUART_STOP_BITS get() const {
		return stopBits_;
	}

private:
	// ����� �������� �����
	eUART_STOP_BITS stopBits_;
};


/// ��������� ���������� ������ � ���������������� ������
class TUartData {
public:
	/// ��������� �����
	TInterface Interface;
	/// �������� ��������
	TBaudRate BaudRate;
	/// ���������� ����� ������
	TDataBits DataBits;
	/// �������� �������� ��������
	TParity Parity;
	/// ���������� ���� �����
	TStopBits StopBits;
};

#endif /* PARAMUART_H_ */
