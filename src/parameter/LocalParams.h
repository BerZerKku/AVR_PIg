/*
 * LocalParams.h
 *
 *  Created on: 25.03.2015
 *      Author: Shcheblykin
 */

#ifndef LOCALPARAMS_H_
#define LOCALPARAMS_H_

#include "param.h"

class LocalParams {
	/// ������������ ���������� ���������� � ������.
	static const uint8_t MAX_NUM_OF_PARAMS = 25;

	/// ������������ ���������� ���� ��� ������� ����������.
//	static const uint8_t MAX_BUF_BITS_VALUES = 12;

public:

	/// ������� ��������� �������� ���������
	enum STATE {
		STATE_NO_ERROR = 0,	///< �������� ���������.
		STATE_READ_PARAM,	///< ���� ���������� ���������.
		STATE_ERROR			///< �������� ���������.
	};

	/// �����������
	LocalParams();

	/** ��������� ������ �������� ��������� � ��� �������� �� ������������.
	 *
	 * 	@param val ����� �������� ���������.
	 */
	void setValue(int16_t val);

	/**	���������� ������� �������� ���������.
	 *
	 *	���� ������� �������� ������� (\a Param::PARAM_BITES), �� ������������
	 *	�������� 0 ��� 1 ��� �������� ���.������ \a currSameParam. �����
	 *	������������ ������� �������� ���������.
	 *
	 * 	@return ������� �������� ���������.
	 */
	int16_t getValue() const;

	/**	���������� ������� ���� ��� ������� ����������.
	 *
	 * 	@return ������� ���� ��� �������� ���������.
	 */
	uint8_t getValueB() const;

	/**	���������� ������� ��������� ���������.
	 *
	 * 	@retval True ��������� ��������.
	 * 	@retval False ���������� ��������.
	 */
	STATE getState() { return state; }

	/**	������� � ���������� ���������.
	 *
	 * 	���� �������� ����� ������, ���������� ������ �� ������ ��������.
	 * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
	 * 	���������� �������� ���������.
	 *
	 * 	���� �������� ��� �������, �� �������������� ���� ���������� \a refresh.
	 */
	void nextParam();

	/**	������� � ����������� ���������.
	 *
	 * 	���� �������� ������ ������, ���������� ������ �� ��������� ��������.
	 * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
	 * 	���������� �������� ���������.
	 *
	* 	���� �������� ��� �������, �� �������������� ���� ���������� \a refresh.
	 */
	void prevParam();

	/** ������� � ���������� ����������� ���������.
	 *
	 *	���� �������� ����� ������ , ���������� ������ �� ������ ��������.
	 * 	���� ��� ����������� ������� (���-�� ���������� ���������� ������ 1),
	 * 	�� ���� ���������� �������� ���������.
	 */
	void nextSameParam();

	/** ������� � ����������� ����������� ���������.
	 *
	 *	���� �������� ������ ������, ���������� ������ �� ��������� ��������.
	 *	���� ��� ����������� ������� (���-�� ���������� ���������� ������ 1),
	 * 	�� ���� ���������� �������� ���������.
	 */
	void prevSameParam();

	/** ���������� � ������ ������ ���������.
	 *
	 * 	� ������ ������������ ������ ����������, ����� ���������� ������.
	 *
	 * 	@param *newParam ��������� �� ��������� ���������.
	 * 	@retval True �������� ��������.
	 * 	@retval False ����� ����������, �������� ����� ��������.
	 *
	 */
	bool addParam(eGB_PARAM newParam);

	/**	������� ������ ����������.
	 *
	 * 	�� ��������� ��������������� ������ �������� �� ������
	 * 	\a GB_PARAM_TIME_SYNCH, ��� ���� ���������� ���������� ����������.
	 */
	void clearParams();

	/**	���������� ����������� �������� ���������.
	 *
	 * 	@return ����������� �������� ���������.
	 */
	int16_t getMin() const { return pgm_read_word(&getPtrParam()->min); }

	/**	���������� ������������ �������� ���������.
	 *
	 *	��� ��������� ����������, � ��������� ������� �������� ���-�� ���������
	 *	������, ����� ���������� �������� ������������� �������� � ������
	 *	������������ max = min + flash_max - 1.
	 *
	 * 	@return ������������ �������� ���������.
	 */
	int16_t getMax() const;

	/**	���������� ������������ ��������� ?!
	 *
	 * 	@param ������������ ���������.
	 */
	uint8_t getDisc() const { return pgm_read_byte(&getPtrParam()->disc); }

	/**	���������� ��������� ���������.
	 *
	 * 	@param ��������� ���������.
	 */
	uint8_t getFract() const { return pgm_read_byte(&getPtrParam()->fract); }

	/**	���������� ����� �������� ���������.
	 *
	 * 	@return ����� �������� ���������.
	 */
	uint8_t getNumOfCurrParam() const {	return currParam + 1; }

	/**	���������� ���������� ������� ����������.
	 *
	 * 	@return ���������� ������� ����������.
	 */
	uint8_t getNumOfParams() const {
		return numOfParams;
	}

	/**	���������� ��������� �� ������ � ��������� ���������.
	 *
	 * 	������ ��������� �� FLASH.
	 *
	 * 	@return ��������� �� ������ � ��������� ���������.
	 *
	 */
	PGM_P getNameOfParam() const {
		return (PGM_P) getPtrParam()->name;
	}

	/**	���������� ��������� �� ������ ������ ������� �������� ���������.
	 *
	 * 	��������� ��� ���������� �������� ������� ���������� �� ������. ���
	 * 	������ ��������� �� FLASH.
	 *
	 * 	@return ��������� �� ������ ������ ������� �������� ���������.
	 *
	 */
	PGM_P getListOfValues() const {
#ifdef AVR
		return (PGM_P) pgm_read_word(&getPtrParam()->listValues);
#else
        return getPtrParam()->listValues;
#endif
	}

	/**	���������� ������� ����� ����������� ��������� ������� � 1.
	 *
	 * 	���� ����� �������� ��������� ��������� ������������ ����������, �����
	 * 	��������� ��������.
	 *
	 * 	@return ������� ����� ����������� ���������.
	 */
	uint8_t getNumOfCurrSameParam() const;

	/**	���������� ���������� ���������� ���������.
	 *
	 * 	��������, ���� ��� ������ �� ������ �������� ���������� ���� ��������
	 * 	"������������ �������", �� ��� ����� ���������� ������.
	 *
	 * 	@return ���������� ���������� ����������.
	 */
	uint8_t getNumOfSameParams() const;

	/**	���������� ��� �������� ���������.
	 *
	 * 	@return ��� �������� ���������.
	 */
	Param::PARAM_TYPE getParamType() const {
		return (Param::PARAM_TYPE) pgm_read_byte(&getPtrParam()->param);
	}

	/**	���������� ��� ��������� �������� �������� ���������.
	 *
	 * 	@return ��� ��������� �������� �������� ���������.
	 */
	Param::RANGE_TYPE getRangeType() const {
		return (Param::RANGE_TYPE) pgm_read_byte(&getPtrParam()->range);
	}

	/**	���������� ����������� �������� ���������.
	 *
	 * 	@return ����������� �������� ���������.
	 */
	Param::DIMENSION getDim() const {
		return (Param::DIMENSION) pgm_read_byte(&getPtrParam()->dim);
	}

	/**	���������� ������� ������������ ��������� ��� �������� ���������.
	 *
	 * 	@return ������� ������������ ��������� ��� �������� ���������.
	 */
	eGB_COM getCom() const {
		return (eGB_COM) pgm_read_byte(&getPtrParam()->com);
	}

	/**	���������� ��� ��������� ��� ���������� ������ ��������.
	 *
	 * 	@return ��� ��������� ��� ���������� ������ ��������.
	 */
	eGB_SEND_TYPE getSendType() const {
		return (eGB_SEND_TYPE) pgm_read_byte(&getPtrParam()->send);
	}

	/**	���������� �������� ����� ���. ���������� ��� ���������� ������ ��������.
	 *
	 *	��� ����������, ���������� ������� �� ��������� (������� �������� ��
	 *	���������� ������ �� ��������) ������������ ����� ����� ���.���������� �
	 *	�������� ������ ����������.
	 *
	 * 	@return �������� ����� ���. ���������� ��� ���������� ������ ��������.
	 */
	uint8_t getSendDop() const;

	/**	��������� ������� �������� ����������.
	 *
	 * 	��� ������ ��������� �� FLASH, �� ��� ����������� �����.
	 *
	 * 	@param *ptr ��������� �� ������ �������� ����������.
	 */
	void setFlashParams(const Param** ptr) {
		fps = ptr;
	};

	/**	���������� ������� ��������.
	 *
	 * 	@return ������� ��������.
	 */
	eGB_PARAM getParam() const {
		return param[currParam];
	}

	/**	���������� ����������� ���������� �������� ���������.
	 *
	 * 	@return ����������� �������� ���������.
	 */
	Param::DEPEND_SAME getDependSame() const {
		return (Param::DEPEND_SAME) pgm_read_byte(&getPtrParam()->dependSame);
	}

	/**	���������� ����������� ��������� �������� ���������.
	 *
	 * 	@return ����������� �������� ���������.
	 */
	Param::DEPEND_MAX getDependMax() const {
		return (Param::DEPEND_MAX) pgm_read_byte(&getPtrParam()->dependMax);
	}

	/** ��������� ���-�� ������ �� ��������.
	 *
	 * 	@param numComPrd ���-�� ������ �� ��������.
	 */
	void setNumComPrd(uint8_t numComPrd) {
		this->numComPrd = numComPrd;
	}

	/** ��������� ���-�� ������ �� ������.
	 *
	 * 	@param numComPrd ���-�� ������ �� ������.
	 */
	void setNumComPrm(uint8_t numComPrm) {
		this->numComPrm = numComPrm;
	}

	/**	��������� ���������� ��������� � �����
	 *
	 * 	@param numDevices ���������� ��������� � �����
	 * 	@argval 2
	 * 	@argval 3
	 */
	void setNumDevices(uint8_t numDevices) {
		this->numDevices = numDevices;
	}

	/**	���������� ������� ������� ��� ��������� ���������.
	 *
	 *	@return ������� ��� ��������� ���������.
	 */
	Param::CHANGE_COND getChangeCond() const {
		return (Param::CHANGE_COND) pgm_read_byte(&getPtrParam()->changeCond);
	}

private:
	const Param** fps;		///< ������ �������� ����������.

	eGB_PARAM param[MAX_NUM_OF_PARAMS]; ///< ������ ����������.

	int16_t val;			///< �������� �������� ���������.

	uint8_t currParam;		///< ����� �������� ���������.

	uint8_t numOfParams;	///< ���������� ���������� � ������� ������.

	uint8_t currSameParam;	///< ����� �������� ����������� ���������.

	STATE state;			///< ���� ������ � ������� ��������.

	uint8_t numComPrm;		///< ���������� ������ �� ������.

	uint8_t numComPrd;		///< ���������� ������ �� ��������.

	uint8_t numDevices;		///< ���������� ��������� � �����.

	/**	�������� �������������� �������� ��������� �� ������������.
	 *
	 * 	� ����� ���������� �������� � ��������� \a state ����� ��������
	 * 	�������� STATE_ERROR, ���� ��� ����� - \a STATE_NO_ERROR.
	 *
	 *	@see STATE_ERROR
	 *	@see STATE_NO_ERROR
	 */
	void checkValue();

	/**	���������� ���������.
	 *
	 * 	������� ������ \a state ������������ � �������� ������ STATE_READ_PARAM,
	 * 	� �������� ��������� ������������ � 0.
	 *
	 */
	void refreshParam();

	/**	���������� ��������� �� ������ �������� �������� ���������.
	 *
	 * 	@return ��������� �� ������ �������� �������� ���������.
	 */
	Param* getPtrParam() const {
#ifdef AVR
		return (Param*) pgm_read_word(&fps[param[currParam]]);
#else
        return (Param*) fps[param[currParam]];
#endif
	}
};

#endif /* LOCALPARAMS_H_ */
