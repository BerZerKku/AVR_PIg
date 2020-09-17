/*
 * LocalParams.cpp
 *
 *  Created on: 25.03.2015
 *      Author: Shcheblykin
 */

#include "LocalParams.h"

LocalParams::LocalParams() {
	clearParams();
}

//	������� � ���������� ���������.
void LocalParams::nextParam() {
	uint8_t t = currParam;

	t = (t < (numOfParams - 1)) ? (t + 1) : 0;

	if (t != currParam) {
		currParam = t;
		currSameParam = 0;
		refreshParam();
	}
}

// ������� � ����������� ���������.
void LocalParams::prevParam() {
	uint8_t t = currParam;

	t = (t > 0) ? (t - 1) : (numOfParams - 1);

	if (t != currParam) {
		currParam = t;
		currSameParam = 0;
		refreshParam();
	}
}

// ������� � ���������� ����������� ���������.
void LocalParams::nextSameParam() {
	uint8_t t = currSameParam;

	t = (t < (getNumOfSameParams() - 1)) ? (t + 1) : 0;

	if (t != currSameParam) {
		currSameParam = t;
		refreshParam();
	}
}

// ������� � ����������� ����������� ���������.
void LocalParams::prevSameParam() {
	uint8_t t = currSameParam;

	t = (t > 0) ? (t - 1) : (getNumOfSameParams() - 1);

	if (t != currSameParam) {
		currSameParam = t;
		refreshParam();
	}
}

// ���������� � ������ ������ ���������.
bool LocalParams::addParam(eGB_PARAM newParam) {
	if (numOfParams >= (MAX_NUM_OF_PARAMS - 1))
		return false;

	param[numOfParams++] = newParam;
	return true;
}

// ��������� ������ �������� ��������� � ��� �������� �� ������������.
void LocalParams::setValue(int16_t val) {
	uint8_t fract = pgm_read_byte(&getPtrParam()->fract);
	uint8_t disc = getDisc();

	val = val * fract;
	val = (val / disc) * disc;
	this->val = val;

    checkValue();
}

void LocalParams::setValuePwd(uint32_t val)
{
    this->valPwd = val;

    checkValue();
}

// ������

// ���������� ������� �������� ���������.
int16_t LocalParams::getValue() const {
	int16_t v = val;

	if (getParamType() == Param::PARAM_BITES) {
		uint8_t cur = getNumOfCurrSameParam() - 1;
		uint8_t bite = cur % 8;

		v = ((val & (1 << bite)) != 0) ? 1 : 0;
	}

	return v;
}

// ���������� ������� ���� ��� ������� ����������.
uint8_t LocalParams::getValueB() const {
    return val;
}

//
uint32_t LocalParams::getValuePwd() const
{
    return valPwd;
}

// ������� ������ ����������.
void LocalParams::clearParams() {
	param[0] = GB_PARAM_NULL_PARAM;
	currParam = 0;
	val = 0;
	numOfParams = 0;
	currSameParam = 0;
	state = STATE_READ_PARAM;
}

//	���������� ������������ �������� ���������.
int16_t LocalParams::getMax() const {
	uint16_t max = 0;

	switch(getDependMax()) {
		case Param::DEPEND_MAX_NO:
			max = pgm_read_word(&getPtrParam()->max);
			break;
		case Param::DEPEND_MAX_ON_NUM_DEVS:
			max = numDevices;
			break;
		case Param::DEPEND_MAX_ON_COM_PRD:
			max = numComPrd;
	}

	// ��� ��������� ����������  � ��������� �������� ����.���������� ���������
	// ������, ������� ������������� ������������ ��������

	Param::PARAM_TYPE type = getParamType();
	if ((type == Param::PARAM_LIST) || (type == Param::PARAM_BITES)) {
		max = getMin() + max - 1;
	}

	return max;
}

//	���������� ������� ����� ����������� ���������.
uint8_t LocalParams::getNumOfCurrSameParam() const {
	uint8_t num = currSameParam + 1;
	uint8_t max = getNumOfSameParams();

	num = (num > max) ? max : num;
	return num;
}

//	���������� ���������� ���������� ���������.
uint8_t LocalParams::getNumOfSameParams() const {
	uint8_t num = 1;

	switch(getDependSame()) {
		case Param::DEPEND_SAME_NO:
			num = pgm_read_byte(&getPtrParam()->num);
			break;
		case Param::DEPEND_SAME_ON_NUM_DEVS:
			num = numDevices - 1;
			break;
		case Param::DEPEND_SAME_ON_COM_PRD:
			num = numComPrd;
			break;
		case Param::DEPEND_SAME_ON_COM_PRM:
			num = numComPrm;
			break;
	}

	return num;
}

//	���������� �������� ����� ���. ���������� ��� ���������� ������ ��������.
uint8_t LocalParams::getSendDop() const {
	uint8_t dop = 0;

	dop = pgm_read_byte(&getPtrParam()->sendDop);

//	switch(getDependSame()) {
//		case Param::DEPEND_SAME_NO:
//			break;
//		case Param::DEPEND_SAME_ON_NUM_DEVS: // DOWN
//		case Param::DEPEND_SAME_ON_COM_PRD:	 // DOWN
//		case Param::DEPEND_SAME_ON_COM_PRM:
//			if (getParamType() != Param::PARAM_BITES) {
//				dop += getNumOfCurrSameParam();
//			}
//			break;
//	}

	return dop;
}

//	�������� �������������� �������� ��������� �� ������������.
void LocalParams::checkValue() {
	// ��� ������������� � �������� ����������� \a Param::PARAM_BITES
	// ��������� �������� ������������ getValue(), � �� �������� �� ���������
	// ���������� this->val �� ������.
    if (getParamType() == Param::PARAM_PWD) {
        qDebug() << "Password = " << valPwd;
        state = (valPwd <= PASSWORD_MAX) ? STATE_NO_ERROR : STATE_ERROR;
    } else {
        int16_t val = getValue();
        if ((val >= getMin()) && (val <= getMax())) {
            state = STATE_NO_ERROR;
        } else {
            state = STATE_ERROR;
        }
    }
}

//	���������� ���������.
void LocalParams::refreshParam() {
	val = 0;
    valPwd = PASSWORD_MAX + 1;
	state = STATE_READ_PARAM;
}
