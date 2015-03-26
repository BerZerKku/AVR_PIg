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

//	Переход к следующему параметру.
void LocalParams::nextParam() {
	uint8_t t = currParam;

	t = (t < (numOfParams - 1)) ? (t + 1) : 0;

	if (t != currParam) {
		currParam = t;
		refreshParam();
	}
}

// Переход к предыдущему параметру.
void LocalParams::prevParam() {
	uint8_t t = currParam;

	t = (t > 0) ? (t - 1) : (numOfParams - 1);

	if (t != currParam) {
		currParam = t;
		refreshParam();
	}
}

// Переход к следующему однотипному параметру.
void LocalParams::nextSameParam() {
	uint8_t t = currSameParam;

	t = (t < (numOfSameParam - 1)) ? (t + 1) : 0;

	if (t != currSameParam) {
		refreshParam();
		currSameParam = t;
	}
}

// Переход к предыдущему однотипному параметру.
void LocalParams::prevSameParam() {
	uint8_t t = currSameParam;

	t = (t > 0) ? (t - 1) : (numOfSameParam - 1);

	if (t != currSameParam) {
		refreshParam();
		currSameParam = t;
	}
}

// Добавление в список нового параметра.
bool LocalParams::addParam(const Param *newParam) {
	if (numOfParams >= (MAX_NUM_OF_PARAMS - 1))
		return false;

	param[numOfParams++] = newParam;
	return true;
}

//  Установка нового значения параметра и его проверка на корректность.
void LocalParams::setValue(int16_t val) {
		uint8_t fract = pgm_read_byte(&param[currParam]->fract);
		uint8_t disc = getDisc();

		val = val * fract;
		val = (val / disc) * disc;

		check(val);

		this->val = val;
	}

//	Очистка списка параметров.
void  LocalParams::clearParams() {
	param[0] = 0;
	val = 0;
	currParam = 0;
	numOfParams = 0;
	currSameParam = 0;
	numOfSameParam = 0;
	state = STATE_READ_PARAM;
}

//	Проверка установленного значения параметра на корректность.
void LocalParams::check(uint16_t val) {
	if ((val >= getMin()) && (val <= getMax())) {
		state = STATE_ERROR;
	} else {
		state = STATE_NO_ERROR;
	}
}

//	Обновление параметра.
void LocalParams::refreshParam() {
	val = 0;
	numOfSameParam = pgm_read_byte(&param[currParam]->num);
	currSameParam = 0;
	state = STATE_READ_PARAM;
}
