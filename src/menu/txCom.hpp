/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "glbDefine.h"

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
	}

	// ������� ������
	void clear();

    /** ���������� ������� � ����� 1.
     *
     * 	@param[in] com ��� �������.
     * 	@param[in] last ���� ������ ��������� ������� � ������.
     *  @return true � ������ �������� ������, ����� false.
	 */
    bool addCom1(eGB_COM com, bool last=false);

	/** ���������������� ������ ��������� � ������ 1 ������ (�� �����).
     *
     *  ������� �������� �� �����. �� ������ ��������� ����� ��������
     *  "���������" ������� � ������, ����� ���������� ������� �� ������
     *  ������.
     *
	 * 	@return ��� ������� �������.
	 */
    eGB_COM getCom1();

    /** ���������� ��������� ������� � ������ ������ 1.
     *
     *  @return ��������� �������.
     *  @retval GB_COM_NO � ������ ���� ����� ����.
     */
    eGB_COM lastCom1() const;

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

    /** ��������� ������� �������� � ������ ������� ������.
     *
     *  @param[in] com �������
     *  @return true ���� ���� ������� � ������, ����� false.
     */
    bool containsFastCom(eGB_COM com) const;

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

    /** ������� ���������� � ������ ������� ������� ��� ��������� �������.
     *
     *  @param[i] pos ������� [0..SIZE_OF(comFast_)).
     */
    void clearFastCom(uint8_t pos);

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

private:
    // ���������� ������ � ������ ������� ������
    uint8_t cntComFast;
    buf_t comFast_[MAX_NUM_FAST_COM];

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
};

#endif /* TX_COM_HPP_ */
