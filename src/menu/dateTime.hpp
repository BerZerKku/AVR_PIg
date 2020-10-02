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
    /// Флаг установки нового времени.
	bool setTimeBsp_;

    /// Конструктор.
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

    /// Возвращает секунды.
	uint8_t getSecond() const {
		return second_;
	}

    /** Устанавливает секунды.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setSecond(uint8_t val);

    /// Возвращает минуты.
	uint8_t getMinute() const {
		return minute_;
	}

    /** Устанавливает минуты.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setMinute(uint8_t val);

    /// Возвращает часы.
	uint8_t getHour() const {
		return hour_;
	}

    /** Устанавливает часы.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setHour(uint8_t val);

    /// Возвращает день.
	uint8_t getDay() const {
		return day_;
	}

    /** Устанавливает день.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setDay(uint8_t val);

    /** Возвращает количество дней для заданного месяца и года.
     *
     *  @param[in] month Месяц (если 0, то берется текущий).
     *  @param[in] year Год (если 0, то берется текущий).
     *  @return Количество дней в месяце.
     */
	uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /// Возвращает месяц.
	uint8_t getMonth() const {
		return month_;
	}

    /** Устанавливает месяц.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setMonth(uint8_t val);

    /// Возвращает год.
	uint8_t getYear() const {
		return year_;
	}

    /** Устанавливает год.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
	bool setYear(uint8_t val);

    /// Возвращает миллисекунды.
	uint16_t getMsSecond() const {
		return msSecond_;
	}

    /** Устанавливает миллисекунды
     *
     *  @param[in] val Значение.
     *  @return true если значение установлено, иначе false.
     */
	bool setMsSecond(uint16_t val);

    /// Возвращает день недели.
	uint8_t getDayOfWeek() const {
		return dayWeek_;
	}

    /** Устанавливает день недели.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
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
