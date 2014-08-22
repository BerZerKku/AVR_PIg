/*
 * bsp.h
 *
 *  Created on: 23.05.2014
 *      Author: Shcheblykin
 */

#ifndef __PROTOCOL_MODBUS_H_
#define __PROTOCOL_MODBUS_H_

#include <stdint.h> 

#define INLINE __attribute__((always_inline))

#ifdef AVR
#define VOLATILE volatile
#else
#define VOLATILE
#endif

/**	\brief ����� ������ � ���������� MODBUS.
 *
 *
 *
 * 	@see MAX_ERRORS
 */
class TProtocolModbus{
	/// ����� ����� ��������� �������, ��� ������� ���� ����� �� ������ ������.
	static const uint16_t DELAY_RESET = 1500;
	/// ����� ����� ��������� �������, ��� ������� ������������ ��������� ������.
	static const uint16_t DELAY_READ  = 3500;

public:
	/// ����� ���������� � ���� �� ��������� (�.�. ������)
	static const uint8_t ADDRESS_ERR = 255;
	/// ����������� ����� ���������� � ����
	static const uint8_t ADDRESS_MIN = 1;
	/// ������������ ����� ���������� � ����
	static const uint8_t ADDRESS_MAX = 247;

	/// ��������� ������ ���������
	enum STATE {
		STATE_OFF,			///< �������� ��������.
		STATE_READ,			///< ������ �������.
		STATE_READ_OK,		///< ������� �������.
		STATE_WRITE_WAIT,	///< �������� ������ ������.
		STATE_WRITE,		///< �������� �������.
		STATE_ERROR			///< ������ � ������ ���������.
	};

	/// ������� ��������� � ������ ������
	enum COM {
		COM_01H_READ_COIL 					= 0x01,	///< ������ ������.
		COM_03H_READ_HOLDING_REGISTER 		= 0x03,	///< ������ ���������� ���������.
		COM_05H_WRITE_SINGLE_COIL			= 0x05,	///< ������ ������ �����.
		COM_06H_WRITE_SINGLE_REGISTER		= 0x06,	///< ������ ������ ����������� ��������.
		COM_0EH_READ_DEVICE_INFORMATION		= 0x0E,	///< ������ ���������� �� ����������.
		COM_0FH_WRITE_MULTIPLIE_COILS		= 0x0F,	///< ������ ������ ������.
		COM_10H_WRITE_MULITPLIE_REGISTERS	= 0x10,	///< ������ ������ ���������� ���������.
		COM_2BH_READ_DEVICE_INDENTIFICATION = 0x2B	///< ������ ���������� �� ����������.
	};

	/// ���� ����������
	enum EXCEPTION {
		EXCEPTION_01H_ILLEGAL_FUNCTION	= 0x01,	///< ��� ������� �� ��������������
		EXCEPTION_02H_ILLEGAL_DATA_ADR	= 0x02,	///< �������� ����� ��������
		EXCEPTION_03H_ILLEGAL_DATA_VAL	= 0x03,	///< �������� �������� ���� ������
		EXCEPTION_04H_DEVICE_FAILURE	= 0x04,	///< � ���������� ��������� ������
		EXCEPTION_10H_TEMP_INAC_PARAM	= 0x10,	///< ������ �� ������ �������� ����������
		EXCEPTION_11H_UNCHANG_PARAM		= 0x11	///< ������ �� ������ ����������
	};

	/**	������������.
	 *
	 * 	@param buf ��������� �� ����� ������.
	 * 	@param size ������ ������ ������.
	 */
	TProtocolModbus(uint8_t *buf, uint8_t size);

	/**	������ ������ ������� ���������.
	 *
	 *	�������� ����� ������� � ��������� "������".
	 *	� ������ ���������� ��������, �������� ��������� � ������� ���������.
	 *
	 *	@see STATE
	 * 	@param state ��������� ��������� ���������.
	 */
	void setEnable() {
		setState(STATE_READ);
	}

	/**	��������� ������ ������� ���������.
	 *
	 */
	void setDisable() {
		state_ = STATE_OFF;
	}

	/** �������� �������� ��������� ������ ������� ���������
	 *
	 *	@retval True - ���� ������� ��������� ��������� �� STATE_OFF.
	 *	@retval False - ���� �������� ��������, �.�. ��������� STATE_OFF.
	 */
	bool isEnable() {
		return (state_ != STATE_OFF);
	}

	/**	���������� ������� ��������� ������ ���������.
	 *
	 * 	@return ������� ��������� ���������.
	 */
	TProtocolModbus::STATE getState() const {
		return state_;
	}

	/**	������ ��������� ������ ���������.
	 *
	 *	� ������ ���������� ��������, �������� ��������� � ������� ���������.
	 *
	 * 	@param state ����� ��������� ������ ���������.
	 */
	void setState(TProtocolModbus::STATE state) {
		if ((state >= STATE_OFF) && (state < STATE_ERROR)) {
			if (state == STATE_READ) {
				// ��� ����� ������ ������ �� "������", ������� ������� ��������
				// ����
				cnt_ = 0;
			}
			state_ = state;
		}
	}

	/**	�������� ���������� ������� ��������� � �������.
	 *
	 * 	@retval True - ���� �������� � ������ ���������.
	 * 	@retval False - ���� ��������� ��������� ���������� �� �������.
	 */
	bool checkState(TProtocolModbus::STATE state) {
		return (state_ == state);
	}

	/**	�������� ���� �����.
	 *
	 *	��� ������������ ������, ����� ������������� ������.
	 *	������������ ����� �������� �������, ���������� � ������� �������
	 *	���������� �����.
	 *
	 *	@param byte ���� ������.
	 */
	void push(uint8_t byte) {

		if (state_ == STATE_READ) {
			// ����� �������� �������� ����, ���� ���������� ������ ����� �������
			if (tick_ >= DELAY_RESET) {
				cnt_ = 0;
			}

			if (cnt_ < size_) {
				buf_[cnt_++] = byte;
			} else {
				// TODO MODBUS ������� ��������� ������ ��� ������������
			}

			// ����� �������� �������
			tick_ = 0;
		}
	}

	/**	�������� ������� �������� �������.
	 *
	 * 	@retval True - ��� ������� �������� �������.
	 * 	@retval False - ��� ���������� �������� �������
	 */
	bool isReadData() {
		return (state_ == STATE_READ_OK);
	}

	/**	�������� ���������.
	 *
	 *	� ������, ���� ��������� ���������� �� ��������� ��������. ����������
	 *	���� ������ �� �������� ����� ����� 0.
	 *
	 * 	@return ���-�� ���� ������ ��� ��������
	 */
	uint8_t trCom();

	/**	���������� ����� �������� �������� ��� ������/������.
	 *
	 * 	���� 0, �� ������ �� ����.
	 *
	 * 	@return ����� �������� ��������.
	 */
	uint16_t getMinNumRegisters() const {
		return minReg_;
	}

	/**	���������� ����� �������� �������� ��� ������/������.
	 *
	 * 	���� 0, �� ������ �� ����.
	 *
	 * 	@return ����� �������� ��������.
	 */
	uint16_t getMaxNumRegisters() const {
		return maxReg_;
	}

	/**	�������� ���������� ������� �� ������������ ������.
	 *
	 * 	?!
	 *
	 * 	@retval True - ���� ������ � ���������� ������� ���.
	 * 	@retval False - ���� ���� ������ � ���������� �������.
	 */
	bool checkReadPackage();

	/**	���������� ���������� ������.
	 *
	 * 	��� ���� ����� ������� ���������� �� ��������� ����� �������� ��������,
	 * 	���������� ������� ��� �����.
	 *
	 * 	@param num ����� ��������.
	 */
	uint16_t readData(uint16_t num);

	/**	������ ������ ��� ��������.
	 *
	 * 	��� �������� �������� �� ���������, ���������� ������� ��� �����.
	 *
	 * 	@param num ����� ��������.
	 * 	@param val �������� ��������.
	 */
	bool writeData(uint16_t num, uint16_t val);

	/**	���� ������� ���������� � ������� ������� ���������� �����.
	 *
	 * 	��� ������ ������� ������� ��������� (��������) \a DELAY_READ,
	 * 	��� �������� ������ �������� �������.
	 *
	 * 	���������� � �������� ����������. ��������, �� ����������.
	 *
	 *	@see DELAY_READ
	 */
	void tick() {
		if (state_ == STATE_READ) {
			if (tick_ < DELAY_READ) {
				tick_ += tickStep_;

				if (tick_ >= DELAY_READ) {
					setState(STATE_READ_OK);
				}
			}
		}
	}

	/**	��������� ������ ���������� � ����.
	 *
	 * 	@param adr ����� ����������.
	 * 	@retval True - � ������ ����� ������ ����������.
	 * 	@retval False - � ������ ���������� ������.
	 */
	bool setAddress(uint8_t adr);

	/**	���������� ����� ���������� � ����.
	 *
	 * 	@return ����� ����������.
	 */
	uint8_t getAddress() const;

	/**	����� �� ������ � ����� ����������.
	 *
	 *	@param code ��� ����������.
	 */
	void setException(TProtocolModbus::EXCEPTION code);



	// ������� CRC ��� ��������� ���-�� ���� ������ � ������.
	uint16_t calcCRC(uint8_t num);

	// ���������� �������� � ������� CRC.
	uint16_t getCRC() const;

	// ��������� � ��������� � ������ ������� ����������� �����.
	void addCRC();

protected:
	const uint8_t size_;	///> ������ ������ ������
	uint8_t * const buf_;	///> ����� ��������/������������ ������

	STATE state_;			///> ������� ��������� ������ ���������.

	uint8_t address_;		///> ����� ���������� � ����.

	uint16_t minReg_;		///> ����� �������� �������� ��� ������/������.
	uint16_t maxReg_;		///> ����� �������� �������� ��� ������/������.

	uint8_t cnt_;			///> ���-�� �������� ����.

	uint16_t tick_;			///> ����� ��������� � ������� ������ ���������� �����.
	uint16_t tickStep_;		///> ��� �������� �����.

	// ��������� ��� ����� � ������� �������������� �� ��������.
	void addData(uint16_t val);



	// �������� ������������ �������� ������� �������������� � ���� ������.
	bool checkCommand(uint8_t com);

	// �������� ������ ���������� �� ���������� � �������������.
	bool checkAddress(uint8_t adr);

	// ��������
};


#endif /* __PROTOCOL_MODBUS_H_ */
