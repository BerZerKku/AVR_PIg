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
 *	��� ������ ���������� ����������:
 *	- �������� ������� push() ��� �������� ���������� ����� ������, ��������
 *	� ���������� ������;
 *	- �� ���������� �������� ������ �������� ������ �� \a STATE_READ;
 *	- �������� ������� tick() � �������� � ������� setTick() ��������, ��������
 *	�� ���������� �������;
 *
 *	��� ������ ������ ���������� ���������� ������� ����� ����������,
 *	��� ����� ����� � ����� ��������� ��������:
 *	- ���������� ����� ��������� � ���� MODBUS, �������� setAddress(17);
 *	- ���������� ��� ����� ����� ����� ��������� �������, �������� ��� ��������
 *	19200 ���/� � �������� ������ ������� tick() 50��� setTick(19200, 50);
 *
 *	��� ����������� �������� ������� isReadData(), ���������� �� ��������
 *	checkReadPackage(). ���� ��������� �������� CHECK_ERR_NO, �� ���� ������� �
 *	��������� ���������� ������� � ������������ ������.
 *
 *	������� ������� �������� ������ trResponse() � ���� ��� ������ �� �������
 *	���������� �������������� ��� �������� ���� ��������� ��.
 *
 *	����� �����/�������� � ������� MODBUS ���������� �� 1 ������.
 *
 * 	@see STATE
 */
class TProtocolModbus {
private:
	/// ����� ����� ��������� �������, ��� ������� ���� ����� �� ������ ������.
	static const uint16_t DELAY_RESET = 15000;	// ����� ������ 1500
	/// ����� ����� ��������� �������, ��� ������� ������������ ��������� ������.
	static const uint16_t DELAY_READ = 35000;	// ����� ������ 3500
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

public:
	/// ��������� ������ ���������
	enum STATE {
		STATE_OFF = 0,		///< �������� ��������.
		STATE_READ,			///< ���� ������ �������.
		STATE_READ_ERROR,	///< ������ � �������� ������ (1.5�c < ����� < 3.5��).
		STATE_READ_OK,		///< ������� �������.
		STATE_WRITE_WAIT,	///< �������� ������ ������.
		STATE_WRITE_READY,	///< ������� ������ � ��������
		STATE_WRITE,		///< ���� �������� �������.
		STATE_ERROR			///< ������ � ������ ���������.
	};

	/// ������� ��������� � ������ ������
	enum COM {
		COM_01H_READ_COIL = 0x01,				///< ������ ������.
		COM_03H_READ_HOLDING_REGISTER = 0x03,	///< ������ ���������� ���������.
		COM_05H_WRITE_SINGLE_COIL = 0x05,		///< ������ ������ �����.
		COM_06H_WRITE_SINGLE_REGISTER = 0x06,	///< ������ ������ ����������� ��������.
		COM_0FH_WRITE_MULTIPLIE_COILS = 0x0F,	///< ������ ������ ������.
		COM_10H_WRITE_MULITPLIE_REGISTERS = 0x10,///< ������ ������ ���������� ���������.
		COM_11H_SLAVE_ID = 0x11, 				///< ������ ID ������������
	};

	/// ���� ����������
	enum EXCEPTION {
		EXCEPTION_01H_ILLEGAL_FUNCTION = 0x01,	///< ��� ������� �� ��������������
		EXCEPTION_02H_ILLEGAL_DATA_ADR = 0x02,	///< �������� ����� ��������
		EXCEPTION_03H_ILLEGAL_DATA_VAL = 0x03,	///< �������� �������� ���� ������
		EXCEPTION_04H_DEVICE_FAILURE = 0x04,	///< � ���������� ��������� ������
		EXCEPTION_10H_TEMP_INAC_PARAM = 0x10,	///< ������ �� ������ �������� ����������
		EXCEPTION_11H_UNCHANG_PARAM = 0x11		///< ������ �� ������ ����������
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

	/**	����������� ����������.
	 *
	 */
//	virtual ~TProtocolModbus() {};

	/**	���������� �������� ������������� ���������� ��������� ��������� ���
	 * 	������ ��� ������ � ����� ������� ���������.
	 *
	 * 	@return �������� ������������� ���������� ���������.
	 */
	uint16_t getMaxNumRegisters() const {
		return  MAX_NUM_REGISTERS;
	}

	/**	���������� �������� ������������� ���������� ������ ��������� ���
	 * 	������ ��� ������ � ����� ������� ���������.
	 *
	 * 	@return �������� ������������� ���������� ������.
	 */
	uint16_t getMaxNumCoils() const {
		return  MAX_NUM_COILS;
	}

	/**	���������� ����� ���������� � ��������� ��-���������, �.�. ������.
	 *
	 * 	@return ����� ���������� - ������.
	 */
	uint8_t getAddressError() const {
		return ADDRESS_ERR;
	}

	/**	���������� ����������� ����� ����������, ��������� � ���������.
	 *
	 * 	@return ����� ���������� - �����������.
	 */
	uint8_t getAddressMin() const {
		return ADDRESS_MIN;
	}

	/**	���������� ������������ ����� ����������, �������� � ���������.
	 *
	 * 	@return ����� ���������� - ������������.
	 */
	uint8_t getAddressMax() const {
		return ADDRESS_MAX;
	}

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
		setState(STATE_OFF);
	}

	/** �������� �������� ��������� ������ ������� ���������
	 *
	 *	@retval True - ���� ������� ��������� ��������� �� STATE_OFF.
	 *	@retval False - ���� �������� ��������, �.�. ��������� STATE_OFF.
	 */
	bool isEnable() const {
		return (state_ != STATE_OFF);
	}

	/**	����� ��������� �� �������� ����� �������.
	 *
	 */
	void setReadState() {
		setState(TProtocolModbus::STATE_READ);
	}

	/**	�������� ������� �������� �������.
	 *
	 * 	@retval True - ��� ������� �������� �������.
	 * 	@retval False - ��� ���������� �������� �������
	 */
	bool isReadData() const{
		return (state_ == STATE_READ_OK);
	}

	/**	����� ��������� ������ ���������.
	 *
	 *	�������� � public ������ ��� ����������� ������������ ������ ������!!!
	 *
	 *	� ������ ���������� ��������, �������� ��������� � ������� ���������.
	 *
	 * 	@param state ����� ��������� ������ ���������.
	 */
	void setState(TProtocolModbus::STATE state);

	/**	�������� ���������� ������� ��������� � �������.
	 *
	 * 	@retval True - ���� �������� � ������ ���������.
	 * 	@retval False - ���� ��������� ��������� ���������� �� �������.
	 */
	bool checkState(TProtocolModbus::STATE state) const {
		return (state_ == state);
	}

	/**	���������� ������� ��������� ���������.
	 *
	 *	�������� � public ������ ��� ����������� ������������ ������ ������!!!
	 *
	 *	@return ������� ��������� ���������.
	 */
	TProtocolModbus::STATE getState() const {
		return state_;
	}

	/**	�������� ���� �����.
	 *
	 *	��� ������������ ������ ������� ����� ������ ���������������.
	 *
	 *	������������ ����� �������� �������, ���������� � ������� �������
	 *	���������� �����.
	 *
	 *	� ������ �������� ��������� \a STATE_READ, ������������ ��������
	 *	�������� ������� � ���-�� �������� ���� ������:
	 *	- ���� ��� ������ ���� ������, �� ����� ������� � ����� ���������� ��
	 *	�������� �������.
	 *	- ���� ������� ������� �������� \a DELAY_RESET ������������ ������
	 *	������ � ��������� �������� �� \a STATE_READ_ERROR.
	 *
	 *	@see STATE
	 *	@see DELAY_RESET
	 *	@param byte ���� ������.
	 *	@return ���������� ���-�� �������� ������ � ������.
	 */
	uint8_t push(uint8_t byte);

	/**	���������� ��������� ����� ��������/����� � �������.
	 *
	 * 	@return ��������� ����� �������� ��� ����� � �������� �������.
	 */
	uint16_t getStartAddress() const {
		return startAdr_;
	}

	/**	���������� ���������� ���������/������ � �������.
	 *
	 * 	@return ���������� ���������/������ � �������.
	 */
	uint16_t getNumOfAddress() const {
		return numOfAdr_;
	}

	/** ���������� ���-�� ���� � �������� ������� ��� ������� � ��������.
	 *
	 *	������������ ��� ������.
	 *
	 * 	@return ���-�� ���� ������.
	 */
	uint8_t getNumOfBytes() const {
		return cnt_;
	}

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
	 *	- � ��������� \a STATE_READ ���������� �� ����� 4 �������� ����  ���
	 *	�������� � ��������� �������� ������� \a STATE_READ_OK, �����
	 *	����� ����� ����������� \a STATE_READ.
	 *	- � ��������� \a STATE_READ_ERROR, ����������� ��������� ������ �������
	 *	\a STATE_READ (�.�. �������� ����� �������).
	 *
	 * 	���������� � �������� ����������.  ��������, �� ����������.
	 *
	 *	@see DELAY_READ
	 *	@see STATE_READ
	 *	@see STATE_READ_ERROR
	 */
	void tick();

	/**	��������� ������ ���������� � ����.
	 *
	 * 	@param adr ����� ����������.
	 * 	@retval True - � ������ ����� ������ ����������.
	 * 	@retval False - � ������ ���������� ������.
	 */
	bool setAddressLan(uint8_t adr);

	/**	���������� ����� ���������� � ����.
	 *
	 * 	@return ����� ����������.
	 */
	uint8_t getAddressLan() const;

	/**	�������� ���������.
	 *
	 *	� ������, ���� ��������� ���������� �� ��������� ���������� � ��������
	 *	\a STATE_WRITE_READY, ���������� ���� ������ �� �������� ����� ����� 0.
	 *	��� ������� ������ ��� �������� ��������� ��������� �� \a STATE_WRITE.
	 *
	 *	@see STATE_WRITE
	 *	@see STATE_WRITE_READY
	 * 	@return ���-�� ���� ������ ��� ��������.
	 */
	uint8_t trResponse();

	/**	��������� �������� ������.
	 *
	 *	� ������ ����������� ������ � �������� ������, ����� ������������
	 *	����������. ��� ������� �������� ������ ��������� ���������� ��
	 *	\a STATE_WRITE_READY.
	 *
	 *	����������� �����, ��� ����������� �����.
	 *
	 *	@see STATE_WRITE_READY
	 * 	@retval True - � ������ �������� ��������� ������.
	 * 	@retval False - � ������ ������.
	 */
	bool readData();

protected:
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

	/**	������ ����� ��� ��������.
	 *
	 * 	��� �������� �������� �� ���������, ���������� ������� ��� �����.
	 *
	 * 	���� ����� ����� �� ������������� �����������, �� ����������� ������.
	 *
	 * 	@param adr ����� �����.
	 * 	@param val �������� �����.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - � ������ ������ ������.
	 */
	bool sendCoil(uint16_t adr, bool val);

	/**	������ ������ ������ ��� ��������.
	 *
	 * 	��� �������� �������� �� ����������, ���������� ������� ��������� �����
	 * 	����� � ����� ������ (�.�. ����� �������� ����� � ����� ������).
	 *
	 * 	���� �� ���� �� �������� ������ (�.�. �� �����) �� �������������
	 * 	�����������, �� ����������� ������.
	 *
	 * 	@param adr ����� �����.
	 * 	@param val �������� �����.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - � ������ ������ ������.
	 */
	bool sendCoils(uint16_t adr, uint8_t val);

	/**	������ �������� ��� ��������.
	 *
	 * 	��� �������� �������� �� ���������, ���������� ������� ��� �����.
	 *
	 *	���� ����� �������� �� ������������� �����������, �� �����������
	 *	������.
	 *
	 * 	@param adr ����� ��������.
	 * 	@param val �������� ��������.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - � ������ ������ ������.
	 */
	bool sendRegister(uint16_t adr, uint16_t val);

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
	uint16_t startAdr_;		///> ��������� ����� � �������
	uint16_t numOfAdr_;		///> ���-�� ������� � �������

	// �������� ������������ �������� ������� �������������� � ���� ������.
	TProtocolModbus::CHECK_ERR checkCommand(uint8_t com);

	// �������� ������ ���������� �� ���������� � �������������.
	bool checkAddressLan(uint8_t adr);

	// ������� CRC ��� ��������� ���-�� ���� ������ � ������.
	uint16_t calcCRC(uint8_t num);

	// ���������� �������� � ������� CRC.
	uint16_t getCRC() const;

	// ��������� � ��������� � ������ ������� ����������� �����.
	void addCRC();

	// �������� ���������� ������� �� ������������ ���������.
	TProtocolModbus::CHECK_ERR checkReadPackage();

	// ����� �� ������ � ����� ����������.
	void setException(TProtocolModbus::EXCEPTION code);

	//	���������� ������� ��� ������ �� ������.
	uint8_t prepareResponse();

	/** ��������� �������� ������� ������ ������.
	 *
	 *	� ������� ��� ������ ���������� ������������� �����.
	 *	���������� �������� ������������ ��������� ������� � �������. ����� ��
	 *	������������ � ����������� ������ ��������� �������. � �� ����������
	 *	�������������� ���� ��������� ����������.
	 *
	 *	�� ���������, �������� �� �������:
	 *	- � 1 �� 100 �����������;
	 *	- c 101 �� 200 ��������;
	 *	- c 201 �� 300 �����������;
	 *	- � 301 � ����� �� ����������, �.�. ������.
	 *
	 *	@retval True - ���� ��� ������ ���������.
	 * 	@retval False - ������� ���� �� ���� ��������� �����.
	 */
	virtual bool readCoilCom();

	/**	��������� �������� ������� ������ ���������.
	 *
	 *	� ������� ��� ������ ���������� ������������� ��������.
	 *	���������� �������� ������������ ��������� ������� � �������. ����� ��
	 *	������������ � ����������� ������ ��������� �������. � �� ����������
	 *	��� ��������������� �������� ������������ �������� 0xFFFF.
	 *
	 *	�� ���������, �������� �� �������:
	 *	- � 1 �� 100 ������������ ����� ������;
	 *	- c 101 �� 200 ������������ ����� �������� �� ���������, �.�. 0xFFFF;
	 *	- � 201 �� 300 ������������ ����� ������;
	 *	- c 300 � ����� �� ����������, �.�. ������.
	 *
	 *	@retval True - ���� ��� ������ ���������.
	 * 	@retval False - ���� ���� �� ���� ��������� �����.
	 */
	virtual bool readRegisterCom();

	/**	��������� �������� ������� ������ ID.
	 *
	 * 	� ������� ��� ������ ���������� ����������� ������.
	 * 	�� ��������� ����� ���������� ��������� "Virtual" � ��������� ���������
	 * 	OFF.
	 *
	 *	�������� ���������� ���������:
	 *	- 0x00 - OFF;
	 *	- 0xFF - ON.
	 *
	 * 	@retval False - � ������ ���������� ����������� ������.
	 * 	@retval True - ���� ������� ���������� ��� ������.
	 */
	virtual bool readIdCom();
};

#endif /* __PROTOCOL_MODBUS_H_ */
