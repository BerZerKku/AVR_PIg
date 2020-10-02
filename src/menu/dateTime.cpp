/*
 * dateTime.cpp
 *
 *  Created on: 2 окт. 2020 г.
 *      Author: Shcheblykin
 */

#include "dateTime.hpp"

//
bool TDateTime::setSecond(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if (val < 60) {
		second_ = val;
		stat = true;
	} else {
		second_ = 61;
	}

	return stat;
}

//
bool TDateTime::setMinute(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if (val < 60) {
		minute_ = val;
		stat = true;
	} else {
		minute_ = 61;
	}

	return stat;
}

//
bool TDateTime::setHour(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if (val < 24) {
		hour_ = val;
		stat = true;
	} else {
		hour_ = 25;
	}

	return stat;
}

//
bool TDateTime::setDay(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if ((val > 0) && (val <= getNumDaysInMonth())) {
		day_ = val;
		stat = true;
	} else {
		day_ = 32;
	}

	return stat;
}

//
uint8_t TDateTime::getNumDaysInMonth(uint8_t month, uint8_t year) const {
	uint8_t num = 0;

	if (month == 0) {
		month = month_;
	}

	if (year == 0) {
		year = year_;
	}

	if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
		num = 30;
	} else if (month == 2) {
		num = ((year % 4) == 0) ? 29 : 28;
	} else if ((month != 0) && (month < 13)) {
		num = 31;
	}

	return num;
}

//
bool TDateTime::setMonth(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if ((val > 0) && (val <= 12)) {
		month_ = val;
		stat = true;
	} else {
		month_ = 13;
	}

	return stat;
}

//
bool TDateTime::setYear(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if (val < 100) {
		year_ = val;
		stat = true;
	} else {
		year_ = 0;
	}

	return stat;
}

//
bool TDateTime::setMsSecond(uint16_t val) {
	bool stat = false;

	if (val < 1000) {
		msSecond_ = val;
		stat = true;
	} else {
		msSecond_ = 1000;
	}

	return stat;
}

//
bool TDateTime::setDayWeek(uint8_t val) {
	bool stat = false;

	val = BCD_TO_BIN(val);

	if ((val >= 1) || (val <= 7)) {
		dayWeek_ = val;
		stat = true;
	} else {
		dayWeek_ = 8;
	}

	return stat;
}
