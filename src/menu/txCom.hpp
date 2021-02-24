/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "src/glbDefine.h"

/// ������������ ���-�� ������ � ������ ������
#define MAX_NUM_COM_BUF1 10

///	������������ ���-�� ������� ������
#define MAX_NUM_FAST_COM 8

/// ������������ ���-�� ������ �� ������ ������
#define MAX_NUM_COM_BUF2 4

/// ����������� ���-�� ������ ������������ �� ���� ����
// 3 = (������� ��������� + ������� ������� + ������� �� ������1 + ��������� �������)
#define MAX_NUM_COM_SEND_IN_CYLCE (MAX_NUM_COM_BUF2 + 4)

//
template<typename T, uint8_t size, T defvalue, bool isend>
class buf {
    uint8_t cnt;
    uint8_t len;
    T buffer[size];

  public:
    buf ();
    void clear();
    bool set(T value);
    T get();
};

// ����� ��� �������� ������
class TTxCom {
	static const uint8_t BUFFER_SIZE = 16;

    typedef struct {
        eGB_SEND_TYPE type;
        eGB_COM com;
        uint8_t dopByte;
        uint8_t buf[BUFFER_SIZE];
    } buf_t;

public:
	TTxCom() {
		clear();
        clearCom1();
        clearFastCom();
	}

	// ������� ������
	void clear();

    /** ���������� ������� � ����� 1.
     *
     *  ������� ������������ ��� ����������� ������, ���������� �� ��������
     *  ������ ����.
     *  ��������������� ��� ������������ ���� ��� ����������.
     *
     * 	@param[in] com ��� �������.
     * 	@param[in] last ���� ������ ��������� ������� � ������.
     *  @return true � ������ �������� ������, ����� false.
	 */
    bool addCom1(eGB_COM com);

	/** ���������������� ������ ��������� � ������ 1 ������ (�� �����).
     *
     *  ������� �������� �� �����. �� ������ ��������� ����� ��������
     *  "���������" ������� � ������, ����� ���������� ������� �� ������
     *  ������.
     *
	 * 	@return ��� ������� �������.
	 */
    eGB_COM getCom1();

    /// ������� ������ ������ 1.
    void clearCom1();

	/** ������ ������� � ����� 2.
	 * 	@param com ��� �������.
	 * 	@retval True - � ������ �������� ������.
	 * 	@retval False - ���� �� ������� ��������� ������� � �����.
	 */
    bool addCom2(eGB_COM com);

	/** ���������������� ������ ��������� � ������ 2 ������ (�� �����).
     *
     *  ����� �������� "���������" ������� � ������, ����� ����������
     *  ������� GB_COM_NO. ��� ����������� ������ ����� �������� �������
     *  �� ������ ������.
     *
	 * 	@return ��� ������� �������.
	 */
    eGB_COM getCom2();

    /** ��������� ������� � ������ ������� ������.
	 *
     *	����� ���������� ������� ����� �������� ��� ����������!
	 *
     * 	@param[in] com �������.
     *  @param[in] type ��� �������.
     *  @return true ���� ������� ���������, ����� false.
	 */
    bool addFastCom(eGB_COM com, eGB_SEND_TYPE type);

    /** ������������� �������������� ���� ������ ��� ��������� ������� �������.
     *
     *  @param[in] dopbyte �������������� ����.
     *  @return true ���� ���� ��������, ����� false.
     */
    bool setFastComDopByte(uint8_t dopbyte);

    /**	���������� ��� ������� �������.
     *
	 * 	@return ��� ������� �������.
	 */
    eGB_COM getFastCom() const;

    /** ���������� ��� ������� �������.
     *
     *  @return ��� ������� �������.
     */
    eGB_SEND_TYPE getFastComType() const;

    /// ������� ��������� ������� ������� �� �������.
    void removeFastCom();

    /// ������� ������� ������� ������.
    void clearFastCom();

    /** ���������� �������������� ���� ��� ������� �������.
    *
    *   @return �������������� ����.
    */
    uint8_t getFastComDopByte() const;

    /**	���������� ���� ������ � �����.
     *
     * 	@param[in] byte	���� ������.
     * 	@param[in] pos ������ �������� ������ ������.
	 */
    void setInt8(uint8_t byte, uint8_t pos = 0);

    /** ���������� �������� ��������� �������� �� ������ ������� �������.
     *
     * 	@param[in] pos ������ �������� ������ ������.
	 * 	@return ���� ������.
	 */
    uint8_t getUInt8(uint8_t pos) const;

	/**	������ 2-� �������� ����� (uint16_t) � �����.
	 * 	� ������ ������ ������������ � 1 � 2 ������� ������� uint8_t.
	 * 	@param val ������.
	 */
	void setInt16(uint16_t val);

    void setInt16BE(int16_t val);

    /** ���������� 2-� �������� ����� (int16_t) �� ������.
	 * 	������ �������� � 1 � 2 ��������� ������� uint8_t.
	 * 	@return ������.
	 */
    int16_t getInt16() const;

    /** ���������� ������ ������ � �����.
     *
     *  @param[in] array ������.
     *  @param[in] size ���������� ������.
     *  @param[in] pos ��������� ������� � ������ ������.
     */
    void setArray(const uint8_t *array, uint8_t size, uint8_t pos=0);

    /**	���������� ��������� �� ����� ������.
     *
     * 	@return ��������� �� ����� ������.
     */
    uint8_t* getBuferAddress();

    /** ������������� ��������� �������.
     *
     *  ���� ������� GB_COM_NO, �� ����� ��������� ������� �� ����������.
     *
     *  @param[in] com �������.
     */
    void setLocalCom(eGB_COM com) {
        if (com != GB_COM_NO) {
            localCom = com;
        }
    }

    /** ���������� ��������� �������.
     *
     *  @return �������.
     */
    eGB_COM getLocalCom() const {
        return localCom;
    }
private:
    // ���������� ������ � ������ ������� ������
    uint8_t cntComFast;
    buf_t comFast_[MAX_NUM_FAST_COM];

    eGB_COM localCom;
    buf<eGB_COM, MAX_NUM_COM_BUF1, GB_COM_NO, false> bufCom1;
    buf<eGB_COM, MAX_NUM_COM_BUF2, GB_COM_NO, true> bufCom2;

    /** ������� ���������� � ������ ������� ������� ��� ��������� �������.
     *
     *  @param[i] pos ������� [0..SIZE_OF(comFast_)).
     */
    void clearFastCom(uint8_t pos);
};

template<typename T, uint8_t size, T defvalue, bool isend>
buf<T, size, defvalue, isend>::buf() {
}

template<typename T, uint8_t size, T defvalue, bool isend>
void  buf<T, size, defvalue, isend>::clear() {
    len = 0;
    cnt = 0;
    for(uint8_t i = 0; i < size; i++) {
        buffer[i] = defvalue;
    }
}

template<typename T, uint8_t size, T defvalue, bool isend>
bool  buf<T, size, defvalue, isend>::set(T value) {
    if (len < size) {
        buffer[len++] = value;
        return true;
    } else {
        QDEBUG("Command adding error. Buffer is full!");
    }
    return false;
}

template<typename T, uint8_t size, T defvalue, bool isend>
T  buf<T, size, defvalue, isend>::get() {
    T value = defvalue;
    if (cnt < len) {
        value = buffer[cnt++];
    } else {
        cnt = 0;
        if (!isend) {
            value = buffer[cnt++];
        }
    }
    return value;
}

#endif /* TX_COM_HPP_ */
