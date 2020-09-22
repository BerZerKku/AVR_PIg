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
public:
	/**	�����������.
	 */
	TEnterParam() {
		setDisable();
	}

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

	// ��������� ��������� ��������� ��������.
	void setValueRange(int16_t min, int16_t max) {
		max_ = max;
		min_ = min;

        // FIXME ����� ����� ���� ������������
        digitMin_ = getMinDigitNumber(status_);
        digitMax_ = getMaxDigitNumber(status_);
	}

	// ���������� ����������� �������� ���������.
	int16_t getValueMin() const {
		return min_;
	}

	// ���������� ������������ �������� ���������.
	int16_t getValueMax() const {
		return max_;
	}

	// ��������� �������� ��������, �������� �������� ������ ���� ����� �� !
	void setValue(int16_t val) {
		val_ = (val < min_) || (val > max_) ? min_ : val;
	}

	// ���������� ������� ��������
	int16_t getValue() const {
		return val_;
	}

    uint32_t getValuePwd() const {
        return pwd_;
    }

	// ���������� �������� �������� � ������ ������������ � ��������
	int16_t getValueEnter() const {
		return ((val_ / disc_) * disc_) / fract_;
	}

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
	}

	// ��������� �� ������ ������� ������
	PGM_P list;

	// ��������� �� ������ ��������
	uint8_t *listValue;

	// ������� �� ��������
	eGB_COM com;

private:
	// ������� ��������
	int16_t val_;
    // �������� ������
    uint32_t pwd_;

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
};


#endif /* ENTERPARAM_H_ */
