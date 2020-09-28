/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef ENTERPARAM_H_
#define ENTERPARAM_H_

#include "glbDefine.h"
#include "paramGlb.h"

// ��������� ����������� ��� ����� ������ �������� ���������.
class TEnterParam { 
    // ��������� ���������� ��� ����������� �����.
    struct last_t {
        eGB_PARAM param;
        eGB_COM com;
        int16_t val;
        uint16_t dopValue;
    };

public:
	/**	�����������.
	 */
    TEnterParam();

	/**	�������� �������� ������� ������ � ����������.
	 *
	 * 	@param ���
	 * 	@return True - ���� ���� ���� ������ ��������, ����� False.
	 */
	bool isEnable();

	/** ����� ����� ������ �������� ���������.
	 * 	 ���� ���� �� ������, ������������� ������������ � �������� ����� 1.
	 * 	��� ������ ������������� ������������ �������� ����� � ��������� ��������.
	 *
	 * 	@param s ����� ������ ������� ����� ����������.
	 */
	void setEnable(eMENU_ENTER_PARAM s = MENU_ENTER_PARAM_INT);

	/**	��������� ����� ������ �������� ���������.
	 *
	 * 	@param ���
	 * 	@return ���
	 */
	void setDisable();

    /** ������������� �������� ��������.
     *
     *  ������������� ����������� ������������ ���������� ��������.
     *
     *  @param[in] min �������.
     *  @param[in] max ��������.
     */
    void setValueRange(int16_t min, int16_t max);

    /** ���������� ����������� �������� ���������.
     *
     *  @return ����������� ��������.
     */
    int16_t getValueMin() const;

    /** ���������� ������������ �������� ���������.
     *
     *  @return ������������ ��������.
     */
    int16_t getValueMax() const;

    /** ������������� �������� ���������.
     *
     *  �������� �������� ������ ���� ��� ����������!
     *
     *  @param[in] val �������� ���������.
     */
    void setValue(int16_t val);

    /** ���������� ������� �������� ���������.
     *
     *  @return �������� ���������.
     */
	int16_t getValue() const {
		return val_;
	}

    /** ���������� ������� �������� ������.
     *
     *  @return ������.
     */
    uint8_t *getValuePwd();

    /// ���������� �������� �������� � ������ ������������ � ��������
    int16_t getValueEnter() const;

	/** ���������� �������� ��������.
	 *
	 * 	@param velocity �������� ��������� �������� (��� ����� ����� ��������
	 * 	� ��������� ����������).
	 * 	@argval 0 ���������� �� ��� �������� �������������.
	 * 	@argval 1 ���������� �� ��� � 10 ��� ������ �������� ������������.
	 * 	@argval 2 ���������� �� ��� � 50 ��� ������ �������� ������������.
	 */
	uint16_t incValue(uint8_t velocity = 0);

	/** ���������� �������� ��������.
	 *
	 * 	@param velocity �������� ��������� �������� (��� ����� ����� ��������
	 * 	� ��������� ����������).
	 * 	@argval 0 ���������� �� ��� �������� �������������.
	 * 	@argval 1 ���������� �� ��� � 10 ��� ������ �������� ������������.
	 * 	@argval 2 ���������� �� ��� � 50 ��� ������ �������� ������������.
	 */
	uint16_t decValue(uint8_t velocity = 0);

    /// ������� �� ������ ������.
    void decDigit();

    /** ���������� ����� ���������� �������.
     *
     *  @return ����� �������, ������� � 1.
     */
    uint8_t getDigit() const;

    /// ������� �� ������ ������.
    void incDigit();

    /** ���������� ������������ ���������� �������� � �����.
     *
     *  @param[in] s ����� ������ ������� ����� ����������
     *  @return ������������ ���������� �������� � �����.
     */
    uint8_t getMaxDigitNumber(eMENU_ENTER_PARAM s) const;

    /** ���������� ����������� �������� ������.
     *
     *  @param[in] s ����� ������ ������� ����� ����������
     *  @return ����������� �������� ������.
     */
    uint8_t getMinDigitNumber(eMENU_ENTER_PARAM s) const;

	// ��������� ������ ���������
	void setParam(eGB_PARAM param) {param_ = param; }

	// ���������� ����� ���������
	eGB_PARAM getParam() const {return param_; }

	// ��������� ��������������� �����
	void setDopValue(uint16_t byte) { dopValue_ = byte; }

	// ���������� �������������� ����
	uint16_t getDopValue() const { return dopValue_; }

	// ��������� ������������
	void setDisc(uint16_t disc) { disc_ = disc; }

	// ���������� ������������
	uint16_t getDisc() const { return disc_; }

	// ��������� ��������
	void setFract(uint8_t fract) { fract_ = fract; }

	// ���������� ��������
	uint8_t getFract() const { return fract_; }

	// ���������� ������� ������
	eMENU_ENTER_PARAM getStatus() const { return status_; }

	// ��������� ����� ��������� ����� ���������
	void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY) {
		status_ = status;

        if (param_ != GB_PARAM_IS_PWD) {
            saveSettings();
        }
    }

	// ��������� �� ������ ������� ������
	PGM_P list;

	// ��������� �� ������ ��������
	uint8_t *listValue;

	// ������� �� ��������
	eGB_COM com;

    // �������� ����������� �����.
    last_t last;

private:
	// ������� ��������
	int16_t val_;
    // �������� ������
    uint8_t pwd_[PWD_LEN+1];

	// ������������ ��������
	int16_t max_;

	// ����������� ��������
	int16_t min_;

	// ���� � ������������� �����������
	uint16_t dopValue_;

	// ������������
	uint16_t disc_;

    // ������� ��������� ������, ������� � 1
    uint8_t digit_;
    // ������������ ������ ��� ���������
    uint8_t digitMax_;
    // ����������� ������ ��� ���������
    uint8_t digitMin_;

	// ��������
	uint8_t fract_;

	// ������� ������ ����������
	eMENU_ENTER_PARAM status_;

	// ������� ��������
	eGB_PARAM param_;

    /// ���������� �������� ������ � "����".
    void clearPwd();

    /// ��������� ��������� �����.
    void saveSettings();

    /// ������� ��������� �����.
    void clearSetting();
};


#endif /* ENTERPARAM_H_ */
