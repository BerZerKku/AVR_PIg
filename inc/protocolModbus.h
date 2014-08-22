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

	///< ����� ����� ��������� �������, ��� ������� ���� ����� �� ������ ������.
	static const uint16_t DELAY_RESET = 1500;
	///< ����� ����� ��������� �������, ��� ������� ������������ ��������� ������.
	static const uint16_t DELAY_READ  = 3500;

public:
	/// ��������� ������ ���������
	enum STATE {
		STATE_OFF,			///< �������� ��������.
		STATE_READ,			///< ������ �������.
		STATE_READ_OK,		///< ������� �������.
		STATE_WRITE_WAIT,	///< �������� ������ ������.
		STATE_WRITE,		///< �������� �������.
		STATE_ERROR			///< ������ � ������ ���������.
	};

	/**	������������.
	 *
	 * 	@param buf ��������� �� ����� ������.
	 * 	@param size ������ ������ ������.
	 */
	TProtocolModbus(uint8_t *buf, uint8_t size);

	/**	������ ������ ������� ���������.
	 *
	 *	�� ��������� �������� ����� ������� � ��������� "������".
	 *	� ������ ���������� ��������, �������� ��������� � ������� ���������.
	 *
	 *	@see STATE
	 * 	@param state ��������� ��������� ���������.
	 */
	void setEnable(TProtocolModbus::STATE state=STATE_READ) {
		setState(state);
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
	uint8_t trCom(){
		uint8_t cnt = 0;
		if (checkState(STATE_WRITE)) {
			cnt = cnt_;
		}
		return cnt;
	}

	/**	���������� ����� �������� �������� ��� ������/������.
	 *
	 * 	���� 0, �� ������ �� ����.
	 *
	 * 	@return ����� �������� ��������.
	 */
	uint16_t minNumRegisters() const {
		return minReg_;
	}

	/**	���������� ����� �������� �������� ��� ������/������.
	 *
	 * 	���� 0, �� ������ �� ����.
	 *
	 * 	@return ����� �������� ��������.
	 */
	uint16_t maxNumRegisters() const {
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

private:
	const uint8_t size_;	///> ������ ������ ������
	uint8_t * const buf_;	///> ����� ��������/������������ ������

	STATE state_;			///> ������� ��������� ������ ���������.

	uint16_t minReg_;		///> ����� �������� �������� ��� ������/������.
	uint16_t maxReg_;		///> ����� �������� �������� ��� ������/������.

	uint8_t cnt_;			///> ���-�� �������� ����.

	uint16_t tick_;			///> ����� ��������� � ������� ������ ���������� �����.
	uint16_t tickStep_;		///> ��� �������� �����.

	// ��������� ��� ����� � ������� �������������� �� ��������.
	void addData(uint16_t val);

	// ������� CRC ��� ��������� ���-�� ���� ������ � ������.
	uint16_t calcCRC(uint8_t num);

	// ���������� �������� � ������� CRC.
	uint16_t getCRC();
};


#endif /* __PROTOCOL_MODBUS_H_ */
