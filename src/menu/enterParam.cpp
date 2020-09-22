/*
 * enterParam.cpp
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#include "enterParam.h"

uint16_t step[] = {1, 10, 100, 1000, 10000};
uint32_t stepPwd[] = {1, 10, 100,  1000, 10000, 100000, 1000000, 10000000};

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
		if ((s == MENU_ENTER_PARAM_LIST) || (s == MENU_ENTER_PARAM_LIST_2)) {
			disc_ = 1;
			fract_ = 1;
        }

        pwd_ = 0;
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
	disc_ = 1;
	param_ = GB_PARAM_NULL_PARAM;
}

// Увеличение текущего значения.
uint16_t TEnterParam::incValue(uint8_t velocity) {
    eMENU_ENTER_PARAM s = status_;

    if (s == MENU_ENTER_PASSWORD) {
        if ((PASSWORD_MAX - stepPwd[digit_ - 1]) >= pwd_) {
            pwd_ += stepPwd[digit_ - 1];
        }
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
        if (pwd_ >= stepPwd[digit_ - 1]) {
            pwd_ -= stepPwd[digit_ - 1];
        }
    } else if (getValueMin() >= 0){
        if ((getValueMin() + step[digit_ - 1]) <= val_) {
            val_ -= stepPwd[digit_ - 1];
        }
    }

    return val_;
}

//
void TEnterParam::decDigit()
{
    if (digit_ > 1) {
        digit_--;
    }
}

//
uint8_t TEnterParam::getDigit() const
{
    return digit_;
}

//
void TEnterParam::incDigit()
{
    if (digit_ < digitMax_) {
        digit_++;
    }
}




uint8_t TEnterParam::getMaxDigitNumber(eMENU_ENTER_PARAM s) const
{
    uint8_t num = 1;

    // FIXME Числа могут быть отрицателные

    if ((s == MENU_ENTER_PARAM_LIST) || (s == MENU_ENTER_PARAM_LIST_2)) {
        num = 1;
    } else if ((s == MENU_ENTER_PASSWORD) || (s == MENU_ENTER_PASSWORD_NEW)) {
        num = max_;
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
