/*
 * paramRps.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMRPS_H_
#define PARAMRPS_H_


/// класс для параметров и настроек Терминала
class TDeviceRps {
public:
	TDeviceRps() {
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus Status;

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};


#endif /* PARAMRPS_H_ */
