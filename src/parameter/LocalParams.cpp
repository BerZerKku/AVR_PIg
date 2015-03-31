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
		refresh = true;
		refreshParam();
	}
}

// Переход к предыдущему параметру.
void LocalParams::prevParam() {
	uint8_t t = currParam;

	t = (t > 0) ? (t - 1) : (numOfParams - 1);

	if (t != currParam) {
		currParam = t;
		refresh = true;
		refreshParam();
	}
}

// Переход к следующему однотипному параметру.
void LocalParams::nextSameParam() {
	uint8_t t = currSameParam;

	t = (t < (numOfSameParam - 1)) ? (t + 1) : 0;

	if (t != currSameParam) {
		refreshParam();
		refresh = true;
		currSameParam = t;
	}
}

// Переход к предыдущему однотипному параметру.
void LocalParams::prevSameParam() {
	uint8_t t = currSameParam;

	t = (t > 0) ? (t - 1) : (numOfSameParam - 1);

	if (t != currSameParam) {
		refreshParam();
		refresh = true;
		currSameParam = t;
	}
}

// Добавление в список нового параметра.
bool LocalParams::addParam(eGB_PARAM newParam) {
	if (numOfParams >= (MAX_NUM_OF_PARAMS - 1))
		return false;

	param[numOfParams++] = newParam;
	return true;
}

// Установка нового значения параметра и его проверка на корректность.
void LocalParams::setValue(int16_t val) {
	uint8_t fract = pgm_read_byte(&getPtrParam()->fract);
	uint8_t disc = getDisc();

	val = val * fract;
	val = (val / disc) * disc;

	check(val);

	this->val = val;
}

// Установка нового значения параметра содержащего набор бит
void LocalParams::setValueBits(uint8_t *ptr) {
	for(uint8_t i = 0; i < MAX_BUF_BITS_VALUES; i++) {
		this->valB[i] = *ptr++;
	}

	state = STATE_NO_ERROR;
}

// Возвращает текущее значение параметра.
int16_t LocalParams::getValue() const {
	int16_t v = val;

	if (getParamType() == Param::PARAM_BITES) {
		uint8_t byte = currSameParam / 8;
		uint8_t bite = currSameParam % 8;

		v = ((valB[byte] & (1 << bite)) > 0) ? 1 : 0;
	}

	return v;
}

// Считывание флага обновления параметра с последующем сбросом.
bool LocalParams::isRefresh() {
	bool r = refresh;
	refresh = false;
	return r;
}

// Очистка списка параметров.
void  LocalParams::clearParams() {
	param[0] = GB_PARAM_NULL_PARAM;
	currParam = 0;
	val = 0;
	numOfParams = 0;
	currSameParam = 0;
	numOfSameParam = 0;
	state = STATE_READ_PARAM;
}

//	Проверка установленного значения параметра на корректность.
void LocalParams::check(int16_t val) {
	if ((val >= getMin()) && (val <= getMax())) {
		state = STATE_NO_ERROR;
	} else {
		state = STATE_ERROR;
	}
}

//	Обновление параметра.
void LocalParams::refreshParam() {
	val = 0;
	setNumOfSameParams(pgm_read_byte(&getPtrParam()->num));
	currSameParam = 0;
	state = STATE_READ_PARAM;
}
