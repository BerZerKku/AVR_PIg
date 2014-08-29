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
private:
	/// ����� ����� ��������� �������, ��� ������� ���� ����� �� ������ ������.
	static const uint16_t DELAY_RESET = 15000;	// ����� ������ 1500
	/// ����� ����� ��������� �������, ��� ������� ������������ ��������� ������.
	static const uint16_t DELAY_READ  = 35000;	// ����� ������ 3500
public:
	/// ������������ ���������� ��������� ��������� ��� ������ � ����� ������
	static const uint16_t MAX_NUM_REGISTERS = 32;
	/// ������������ ���������� ������ ��������� ��� ������ � ����� �������
	static const uint16_t MAX_NUM_COILS = 256;

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
		STATE_READ_ERROR,	///< ������ � �������� ������ (1.5�c < ����� < 3.5��)
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
		COM_0FH_WRITE_MULTIPLIE_COILS		= 0x0F,	///< ������ ������ ������.
		COM_10H_WRITE_MULITPLIE_REGISTERS	= 0x10,	///< ������ ������ ���������� ���������.
		COM_11H_SLAVE_ID					= 0x11, ///< ������ ID ������������
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
    
    /// ���� ������ �������� ��������� ���������
    enum CHECK_ERR {
        CHECK_ERR_NO = 0,       ///< ������ ���                       
        CHECK_ERR_ADR_DEVICE,   ///< ������ �������� ������ ����������
        CHECK_ERR_CRC,          ///< ������ ����������� �����
        CHECK_ERR_FUNCTION,     ///< ������ �������� ���� �������
        CHECK_ERR_FUNCTION_DATA	///< ������ �������� ������ �������
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
				tick_ = 0;
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

	/**	�������� ������� �������� �������.
	 *
	 * 	@retval True - ��� ������� �������� �������.
	 * 	@retval False - ��� ���������� �������� �������
	 */
	bool isReadData() {
		return (state_ == STATE_READ_OK);
	}

	/**	�������� ���� �����.
	 *
	 *	��� ������������ ������ ������� ����� ������ ���������������.
	 *	������������ ����� �������� �������, ���������� � ������� �������
	 *	���������� �����.
	 *
	 *	@param byte ���� ������.
	 *	@return ���������� ���-�� �������� ������ � ������.
	 */
	uint8_t push(uint8_t byte) {
		uint8_t cnt = cnt_;
		if (state_ == STATE_READ) {
			// ����� �������� �������� ����, ���� ���������� ������ ����� �������
			if (tick_ >= DELAY_RESET) {
				cnt = 0;
				setState(STATE_READ_ERROR);
			} else {
				if (cnt < size_) {
					buf_[cnt++] = byte;
				}
			}

			// ����� �������� �������
			tick_ = 0;
		}
		return (cnt_ = cnt);
	}

	/**	���������� ��������� ����� ��������/����� � �������.
	 *
	 * 	@return ��������� ����� �������� ��� ����� � �������� �������.
	 */
	uint16_t getStartAddress() const {
		return ((uint16_t) buf_[2] << 8) + buf_[3];
	}

	/**	���������� ���������� ���������/������ � �������.
	 *
	 * 	@return ���������� ���������/������ � �������.
	 */
	uint16_t getNumOfAddress() const {
		return ((uint16_t) buf_[4] << 8) + buf_[5];
	}

	/**	�������� ���������� ������� �� ������������ ���������.
	 *
	 * 	�����������:
	 * 	- ����� ����������;
	 * 	- ����������� �����;
	 * 	- �������������� �������;
	 * 	- ������������ ������ � �������� �������.
	 *
	 * 	�� �����������:
	 * 	- ������������ ������� ��������� � ������.
	 *
	 * 	@return ������ ��������.
	 * 	@see CHECK_ERR
	 */
	TProtocolModbus::CHECK_ERR checkReadPackage();

	/**	���������� ���������� ������ ��� ������ � �������.
	 *
	 * 	��� ���� ����� ������� ���������� �� ��������� ����� �������� ��������,
	 * 	���������� ������� ��� �����.
	 *
	 * 	@param num ����� ��������.
	 * 	@return �������� ��������.
	 * 	@retval 0xFFFF - �������� ��-���������.
	 */
	uint16_t getRegister(uint16_t adr);

	/**	���������� ���������� ������ ��� ������ �����.
	 *
	 * 	��� ���� ����� ������� ���������� �� ��������� ���� �������� �����,
	 * 	���������� ������� ��� �����.
	 *
	 * 	������������ ���� ������, ������� ��� ������������� �������� ������.
	 * 	��� ��� ������ 1 ��������, ��� ���� ���������� ����, � 0 - ��������.
	 *
	 * 	@param num ����� �����.
	 * 	@return �������� �����.
	 * 	@retval 0 - �������� ����.
	 * 	@retval 1 - ���������� ����.
	 * 	@retval 0xFF - ��������� �����.
	 */
	uint8_t getCoil(uint16_t adr);

	/**	������ ������ ��� ��������.
	 *
	 * 	��� �������� �������� �� ���������, ���������� ������� ��� �����.
	 *
	 * 	@param num ����� ��������.
	 * 	@param val �������� ��������.
	 */
	bool writeData(uint16_t num, uint16_t val);

	/**	�������� ���������.
	 *
	 *	� ������, ���� ��������� ���������� �� ��������� ��������. ����������
	 *	���� ������ �� �������� ����� ����� 0.
	 *
	 * 	@return ���-�� ���� ������ ��� ��������
	 */
	uint8_t trCom();

	/**	��������� �������� �������.
	 *
	 * 	������ �� ���������� �������� ������ ���������, ������� ��������
	 * 	�������� � ���-�� ����-�����, � ��� �� ������� ������ ������� tick(),
	 * 	�������������� ��� �����. ��� ���� ����������� ��� ���-�� "��� ������"
	 * 	������ 8, � ���-�� ��� "�������� ��������" + "����-����" ����� 2.
	 *
	 * 	��������:
	 * 	��� ��������  19200 � ������� ������ ������� 1 ��� � 50���, �������:
	 * 	step = (period / 10^6) * (DELAY_RESET / 1.5) * (baudrate / 11) =
	 * 	= (50 / 10^6) * (15000/1.5) * (19200 / 11) = 872
	 *
	 * 	��� ������ ���� 19200 ������������� ��������: 750��� ��� �����������
	 * 	���������, � 1750 ��� 3.5 ���������. ������� ��� ����� ��� ������ ��� �
	 * 	50 ��� �����::
	 * 	step = DELAY_RESET * period / 750 = 15000/750 * period = 20 * 50 = 1000
	 *
	 *	@param baudrate �������� ������ ����������������� �����, ���/�.
	 *	@param period ������� ������ ������� tick(), ���.
	 *	@return ������������ ��� ��������.
	 */
	uint16_t setTick(uint16_t baudrate, uint8_t period);

	/**	���� ������� ���������� � ������� ������� ���������� �����.
	 *	
	 *	������� ������� ������ � ���������� \a STATE_READ � \a STATE_READ_ERROR.
	 *	
	 *	��� ������ ������� ������� ��������� (��������) \a DELAY_READ, �.�.
	 *	����� ���������� "3.5 �����":
	 *	- � ��������� \a STATE_READ, ����������� ��������� �������� �������
	 *	\a STATE_READ_OK;
	 *	- � ��������� \a STATE_READ_ERROR, ����������� ��������� ������ �������
	 *	\a STATE_READ (�.�. �������� ����� �������).
	 *
	 * 	���������� � �������� ����������.  ��������, �� ����������.
	 *
	 *	@see DELAY_READ
	 *	@see STATE_READ
	 *	@see STATE_READ_ERROR
	 */
	void tick() {
		uint16_t tick = tick_;
		TProtocolModbus::STATE state = state_;
		
		if ((state == STATE_READ) || (state == STATE_READ_ERROR)) {
			if (tick < DELAY_READ) {
				tick += tickStep_;

				if (tick >= DELAY_READ) {
					if (state == STATE_READ) {
						setState(STATE_READ_OK);
					} else {
						setState(STATE_READ);
					}
				}
			}
			tick_ = tick;
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
	
private:
	const uint8_t size_;	///> ������ ������ ������
	uint8_t * const buf_;	///> ����� ��������/������������ ������

	static const uint8_t CRC_HI[256];	///> ������ �������� ��� �������� CRC
	static const uint8_t CRC_LOW[256];	///> ������ �������� ��� �������� CRC

	STATE state_;			///> ������� ��������� ������ ���������.

	uint8_t address_;		///> ����� ���������� � ����.
	
	VOLATILE uint8_t cnt_;	///> ���-�� �������� ����.

	VOLATILE uint16_t tick_;///> ����� ��������� � ������� ������ ���������� �����.
	uint16_t tickStep_;		///> ��� �������� �����.

	// ��������� ��� ����� � ������� �������������� �� ��������.
	void addData(uint16_t val);

	// �������� ������������ �������� ������� �������������� � ���� ������.
	TProtocolModbus::CHECK_ERR checkCommand(uint8_t com);

	// �������� ������ ���������� �� ���������� � �������������.
	bool checkAddress(uint8_t adr);

	// ������� CRC ��� ��������� ���-�� ���� ������ � ������.
	uint16_t calcCRC(uint8_t num);
	
	// ���������� �������� � ������� CRC.
	uint16_t getCRC() const;

	// ��������� � ��������� � ������ ������� ����������� �����.
	void addCRC();
};


#endif /* __PROTOCOL_MODBUS_H_ */
