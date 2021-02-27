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

/// ����� ��� ���� � �������
class TDateTime {
public:
    /// ���� ��������� ������ �������.
	bool setTimeBsp_;

    /// �����������.
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

    /// ���������� �������.
	uint8_t getSecond() const {
		return second_;
	}

    /** ������������� �������.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setSecond(uint8_t val);

    /// ���������� ������.
	uint8_t getMinute() const {
		return minute_;
	}

    /** ������������� ������.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setMinute(uint8_t val);

    /// ���������� ����.
	uint8_t getHour() const {
		return hour_;
	}

    /** ������������� ����.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setHour(uint8_t val);

    /// ���������� ����.
	uint8_t getDay() const {
		return day_;
	}

    /** ������������� ����.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setDay(uint8_t val);

    /** ���������� ���������� ���� ��� ��������� ������ � ����.
     *
     *  @param[in] month ����� (���� 0, �� ������� �������).
     *  @param[in] year ��� (���� 0, �� ������� �������).
     *  @return ���������� ���� � ������.
     */
	uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /// ���������� �����.
	uint8_t getMonth() const {
		return month_;
	}

    /** ������������� �����.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setMonth(uint8_t val);

    /// ���������� ���.
	uint8_t getYear() const {
		return year_;
	}

    /** ������������� ���.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setYear(uint8_t val);

    /// ���������� ������������.
	uint16_t getMsSecond() const {
		return msSecond_;
	}

    /** ������������� ������������
     *
     *  @param[in] val ��������.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setMsSecond(uint16_t val);

    /// ���������� ���� ������.
	uint8_t getDayOfWeek() const {
		return dayWeek_;
	}

    /** ������������� ���� ������.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
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
