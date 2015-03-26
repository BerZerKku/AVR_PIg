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
	static const uint8_t MAX_NUM_OF_PARAMS = 20;

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
	 * 	@return ������� �������� ���������.
	 */
	int16_t getValue() const { return val; }

	/**	���������� ��������� ����� ������ �������� ��������.
	 *
	 * 	@retval True ��������� ��������.
	 * 	@retval False ���������� ��������.
	 */
	STATE isError() { return state; }

	/**	������� � ���������� ���������.
	 *
	 * 	���� �������� ����� ������, ���������� ������ �� ������ ��������.
	 * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
	 * 	���������� �������� ���������.
	 */
	void nextParam();

	/**	������� � ����������� ���������.
	 *
	 * 	���� �������� ������ ������, ���������� ������ �� ��������� ��������.
	 * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
	 * 	���������� �������� ���������.
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
	bool addParam(const Param *newParam);

	///	������� ������ ����������.
	void clearParams();

	/**	���������� ����������� �������� ��������� ?!
	 *
	 * 	@return ����������� �������� ���������.
	 */
	int16_t getMin() const { return pgm_read_word(&param[currParam]->min);	}

	/**	���������� ������������ �������� ��������� ?!
	 *
	 * 	@return ������������ �������� ���������.
	 */
	int16_t getMax() const { return pgm_read_word(&param[currParam]->max); }

	/**	���������� ������������ ��������� ?!
	 *
	 * 	@param ������������ ���������.
	 */
	uint8_t getDisc() const { return pgm_read_byte(&param[currParam]->disc); }

	/**	���������� ��������� ���������.
	 *
	 * 	@param ��������� ���������.
	 */
	uint8_t getFract() const { return pgm_read_byte(&param[currParam]->fract); }

	/** �������� ������� �� ���������� �� ������������ ���������.
	 *
	 * 	@param *buf ��������� �� ����� �����������.
	 * 	@param *val �������������� ���� ������.
	 * 	@return ���������� ���-�� ���� ������ �� ��������.
	 */
	uint8_t  (*txComByProtocolS) (uint8_t *buf, uint8_t val);

	/** ������������ ������� ������������ ���������.
	 *
	 * 	@param *buf ��������� �� ����� �����������.
	 * 	@param *val ��������� �� �������� ���������.
	 * 	@return ���������� ���-�� ���� ������ �� ��������.
	 */
	uint8_t  (*txParamByProtocolS) (uint8_t *buf, uint8_t *val);

	/** ���������� ������ �� ������� ������������ ���������.
	 *
	 * 	@param *buf ��������� �� ����� ���������.
	 * 	@return ���������� �������� ���������.
	 */
	uint16_t (*rxParamByProtocolS) (uint8_t *buf);

	uint8_t getNumOfCurrParam() const { return currParam + 1; }

	uint8_t getNumOfParams() const { return numOfParams; }

	PGM_P getNameOfParam() const {
		return (PGM_P) pgm_read_word(&param[currParam]->name);
	}

	PGM_P getListOfValues() const {
		return (PGM_P) pgm_read_word(&param[currParam]->listValues);
	}

	uint8_t getNumOfCurrSameParam() const { return currSameParam + 1; }

	uint8_t getNumOfSameParams() const { return numOfSameParam; }

	Param::PARAM_TYPE getParamType() const {
		return (Param::PARAM_TYPE) pgm_read_byte(&param[currParam]->param);
	}

	Param::RANGE_TYPE getRangeType() const {
		return (Param::RANGE_TYPE) pgm_read_byte(&param[currParam]->range);
	}

	Param::DIMENSION getDim() const {
		return (Param::DIMENSION) pgm_read_byte(&param[currParam]->dim);
	}

	eGB_COM getCom() const {
		return (eGB_COM) pgm_read_byte(&param[currParam]->com);
	}

private:

	const Param* param[MAX_NUM_OF_PARAMS];	///< ������ ����������.

	uint16_t val;			///< �������� �������� ���������.

	uint8_t currParam;		///< ����� �������� ���������.

	uint8_t numOfParams;	///< ���������� ���������� � ������� ������.

	uint8_t currSameParam;	///< ����� �������� ����������� ���������.

	uint8_t numOfSameParam;	///< ���������� ���������� ����������.

	STATE state;			///< ���� ������ � ������� ��������.

	/**	�������� �������������� �������� ��������� �� ������������.
	 *
	 * 	� ����� ���������� �������� � ��������� \a state ����� ��������
	 * 	�������� STATE_ERROR, ���� ��� ����� - \a STATE_NO_ERROR.
	 *
	 *	@see STATE_ERROR
	 *	@see STATE_NO_ERROR
	 *
	 * 	@param val ����� �������� ���������.
	 *
	 */
	void check(uint16_t val);

	/**	���������� ���������.
	 *
	 * 	������� ������ \a state ������������ � �������� ������ STATE_READ_PARAM,
	 * 	� �������� ��������� ������������ � 0.
	 *
	 */
	void refreshParam();
};

#endif /* LOCALPARAMS_H_ */
