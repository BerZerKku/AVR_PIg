/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef DATE_TIME_HPP_
#define DATE_TIME_HPP_

#include <stdint.h>

#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))

/// Класс для даты и времени
class TDateTime {
public:
	// флаг установки нового времени
	bool setTimeBsp_;

	TDateTime() {
		msSecond_ = 0;
		second_ = 0;
		minute_ = 0;
		hour_ = 0;
		day_ = 1;
		month_ = 1;
		year_ = 0;
		dayWeek_ = 0;

		setTimeBsp_ = false;
	}

	// Считывание и установка секунд
	uint8_t getSecond() const {
		return second_;
	}
	bool setSecond(uint8_t val);

	// считывание и установка минут
	uint8_t getMinute() const {
		return minute_;
	}
	bool setMinute(uint8_t val);

	// считывание и установка часов
	uint8_t getHour() const {
		return hour_;
	}
	bool setHour(uint8_t val);

	// считывание и установка дня
	uint8_t getDay() const {
		return day_;
	}
	bool setDay(uint8_t val);

	// возвращает кол-во дней в месяце
	// если месяц или год не заданы, возвращается для текущего
	uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

	// считывание и установка месяца
	uint8_t getMonth() const {
		return month_;
	}
	bool setMonth(uint8_t val);

	// считывание и установка года
	uint8_t getYear() const {
		return year_;
	}
	bool setYear(uint8_t val);

	// считывание и установка миллисекунд
	uint16_t getMsSecond() const {
		return msSecond_;
	}
	bool setMsSecond(uint16_t val);

	// день недели
	uint8_t getDayOfWeek() const {
		return dayWeek_;
	}
	bool setDayWeek(uint8_t val);

private:
	uint16_t msSecond_;
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t dayWeek_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;
};

#endif /* DATE_TIME_HPP_ */
