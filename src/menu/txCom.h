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

    /// NOTE ������ ������ ��� ������� ������ ������ 3 ����� �� ������!
    /// NOTE ������ ������ ��� ������ 2 ������ 4 ����� �� ������!

    static const uint8_t kSizeBufComFast = 3;   ///< ������ ������ ��� ������� ������.
    static const uint8_t kSizeBufCom1 = 10;     ///< ������ ������ ��� ������ 1.
    static const uint8_t kSizeBufCom2 = 4;      ///< ������ ������ ��� ������ 2. �� ������ ������ 4!
    static const uint8_t kSizeDataBuf = 16;     ///< ������ ������ ������.

    /// ��������� ������� ������.
    struct fastCom_t {
        eGB_COM command;            ///< �������.
        eGB_SEND_TYPE sendType;     ///< ��� �������.
        uint8_t data[kSizeDataBuf]; ///< ������.
    };

public:
    TTxCom();

    /// ������� ������� ������ 1 � 2.
    void clear();

    /// ������� ������ ������� ������.
    void clearFastCom();

    /** ������ ������� � ����� 1.
     *
     *  ������� ������������ ���� ��� �� ���� ������.
     *
     *  ���� num > 0, �� ���������� ��������� ������� � ������.
     *  ���� num = 0 ������� �����������.
     *
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
     *
     *  ������� ������������ � ������ ����� ������.
     *
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


    /** ������ ���� ������.
     *  @param byte ���� ������.
     *  @param num ������ �������� �������.
     */
    void setInt8(uint8_t byte, uint8_t num = 0) {
        if (num < kSizeDataBuf)
            data_[num] = byte;
    }

    /** ���������� ����� ������.
     *  @param num ������ �������� �������.
     *  @return ���� ������.
     */
    uint8_t getInt8(uint8_t num = 0) const {
        return (num < kSizeDataBuf) ? data_[num] : 0;
    }

    /** ������ 2-� �������� ����� (uint16_t).
     *  � ������ ������ ������������ � 1 � 2 ������� ������� uint8_t.
     *  @param val ������.
     */
    void setInt16(uint16_t val) {
        *((uint16_t *) (&data_[1])) = val;
    }

    /** ���������� 2-� �������� ����� (uint16_t).
     *  ������ �������� � 1 � 2 ��������� ������� uint8_t.
     *  @return ������.
     */
    uint16_t getInt16() const {
        return *((uint16_t *) (&data_[1]));
    }

    /** ���������� ��������� �� ����� ������.
     *  @return ��������� �� ����� ������.
     */
    uint8_t* getBuferAddress() {
        return &data_[0];
    }

    /** ���������� ��� ������� ������� �� ��������.
     *
     *  ���������� ����� ���������� ������� �������, �.�. ������������� \a getFastCom.
     *
     *  @return ��� ������� �� ��������
     */
    eGB_SEND_TYPE getSendType() const;

    /** ��������� ���� ������� �� ��������.
     *
     *	��� ������� ������� ��������������� ����� �� ����������, �.�.
     *	������������� \a addFastCom.
     *
     *  @param sendType ��� ������� �� ��������.
     */
    void setSendType(eGB_SEND_TYPE sendType);

private:
    fastCom_t fastCom_[kSizeBufComFast];

    // ����� ������� ������� �������
    uint8_t cntComFast_;
    // ������ ����� ������
    eGB_COM com1_[kSizeBufCom1];
    // ���-�� ������ � ������ ������
    uint8_t numCom1_;
    // ����� ������� ������� � ������ ������
    uint8_t cnt1_;
    // ������ ����� ������
    eGB_COM com2_[kSizeBufCom2];
    // ���-�� ������ �� ������ ������
    uint8_t numCom2_;
    // ����� ������� ������� �� ������ ������
    uint8_t cnt2_;
    // ����� ������ (��� ������ ������� ������� � �������� )
    uint8_t data_ [kSizeDataBuf];
};

#endif /* SRC_MENU_TXCOM_H_ */
