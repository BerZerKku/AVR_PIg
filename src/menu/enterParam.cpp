/*
 * enterParam.cpp
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#include "enterParam.h"

static const uint16_t step[] = {1, 10, 100, 1000, 10000};

//
TEnterParam::TEnterParam() {
    COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == (PWD_LEN+1));

    setDisable();
}

// Проверка текущего статуса работы с параметром.
bool TEnterParam::isEnable() {
    // проверка текущего статуса на достоверное значение
    if ((status_ < MENU_ENTER_PARAM_NO) || (status_ > MENU_ENTER_PARAM_MAX)) {
		status_ = MENU_ENTER_PARAM_NO;
	}

	return (status_ != MENU_ENTER_PARAM_NO);
}

// Старт ввода нового значения параметра.
void TEnterParam::setEnable(eMENU_ENTER_PARAM s) {

	if ((s >= MENU_ENTER_PARAM_INT) && (s <= MENU_ENTER_PASSWORD_NEW)) {
        clearPwd();
        digit_ = 1;
        digitMin_ = 1;
        digitMax_ = 1;
		status_ = s;
	}
}

// Окончание ввода нового значения параметра.
void TEnterParam::setDisable() {
	status_ = MENU_ENTER_PARAM_NO;
	com = GB_COM_NO;
	param_ = GB_PARAM_NULL_PARAM;
    clearPwd();
    clearSetting();
}

//
void TEnterParam::setValueRange(int16_t min, int16_t max) {
    max_ = max;
    min_ = min;

    // FIXME Числа могут быть отрицателные
    digitMin_ = getMinDigitNumber(status_);
    digitMax_ = getMaxDigitNumber(status_);
}

//
int16_t TEnterParam::getValueMin() const {
    return min_;
}

//
int16_t TEnterParam::getValueMax() const {
    return max_;
}

//
void TEnterParam::setValue(int16_t val) {
    val_ = (val < min_) || (val > max_) ? min_ : val;
}

//
uint8_t *TEnterParam::getValuePwd() {
    return pwd_;
}

// Увеличение текущего значения.
uint16_t TEnterParam::incValue(uint8_t velocity) {
    eMENU_ENTER_PARAM s = status_;

    if (s == MENU_ENTER_PASSWORD) {
        uint8_t val = pwd_[PWD_LEN - digit_];
        pwd_[PWD_LEN - digit_] = (val < '9') ? val + 1 : '0';
    } else {
        if ((getValueMax() - step[digit_ - 1]) >= val_) {
            val_ += step[digit_ - 1];
        }
    }

	return val_;
}

// Уменьшение текущего значения.
uint16_t TEnterParam::decValue(uint8_t velocity) {
	eMENU_ENTER_PARAM s = status_;

    if (s == MENU_ENTER_PASSWORD) {
        uint8_t val = pwd_[PWD_LEN - digit_];
        pwd_[PWD_LEN - digit_] = (val > '0') ? val - 1 : '9';
    } else if (getValueMin() >= 0){
        if ((getValueMin() + step[digit_ - 1]) <= val_) {
            val_ -= step[digit_ - 1];
        }
    }

    return val_;
}

//
void TEnterParam::decDigit() {
    if (digit_ > 1) {
        digit_--;
    }
}

//
uint8_t TEnterParam::getDigit() const {
    return digit_;
}



//
void TEnterParam::incDigit() {
    if (digit_ < digitMax_) {
        digit_++;
    }
}

//
uint8_t TEnterParam::getDigitMax() const{
    return digitMax_;
}

//
uint8_t TEnterParam::getMaxDigitNumber(eMENU_ENTER_PARAM s) const
{
    uint8_t num = 1;

    // FIXME Числа могут быть отрицателные

    if ((s == MENU_ENTER_PARAM_LIST) || (s == MENU_ENTER_PARAM_LIST_2)) {
        num = 1;
    } else if ((s == MENU_ENTER_PASSWORD) || (s == MENU_ENTER_PASSWORD_NEW)) {
        num = static_cast<uint8_t> (max_);
    } else {
        int16_t max = max_ / 10;
        while(max > 0) {
            num++;
            max /= 10;
        }
    }

    return num;
}

uint8_t TEnterParam::getMinDigitNumber(eMENU_ENTER_PARAM s) const
{
    // FIXME Добавить зависимость от шага
    return 1;
}

void TEnterParam::clearPwd()
{
    for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
        pwd_[i] = '0';
    }
    pwd_[PWD_LEN] = '\0';
}

//
void TEnterParam::saveSettings()
{
    last.param = param_;
    last.val = val_;
}

//
void TEnterParam::clearSetting()
{
    last.param = GB_PARAM_MAX;
    last.val = 0;
}
