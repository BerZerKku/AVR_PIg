/*
 * paramIs.cpp
 *
 *  Created on: 07.10.2020
 *      Author: Shcheblykin
 */

#include "paramIS.h"


TPwd::TPwd() {
	COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == PWD_LEN);
	COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);

	reset();
}

//
bool TPwd::set(uint8_t *pwd) {
	bool check = checkValue(pwd);

	if (check) {
		setValue(pwd);
	}

	return check;
}

//
bool TPwd::setCounter(uint8_t value) {
	counter_ = (value <= 2*PWD_CNT_BLOCK) ? value : 2*PWD_CNT_BLOCK;

	if (!isInit()) {
		if (counter_ == PWD_CNT_BLOCK) {
			counter_ = 2 * PWD_CNT_BLOCK;
		}
		tcounter_ = counter_;
		time_ = (counter_ > 0) ? kTickToDecCounter : 0;
		init_ = true;
	}

	return (value == counter_);
}

//
void TPwd::clrCounter() {
	if (isInit()) {
		tcounter_ = 0;
	}
}

//
void TPwd::incCounter() {
	if (tcounter_ == 0) {
		time_ = kTickToDecCounter;
	}

	if (isInit()) {
		if (tcounter_ <= PWD_CNT_BLOCK) {
			tcounter_++;
		}

		if (tcounter_ >= PWD_CNT_BLOCK) {
			tcounter_ = 2*PWD_CNT_BLOCK;
		}
	}
}

void TPwd::reset() {
        init_ = false;
        counter_ = 2*PWD_CNT_BLOCK;
        tcounter_ = counter_;
        time_ = kTickToDecCounter;

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = 0;
        }
    }

//
bool TPwd::tick() {
	bool change = false;

	if (isInit()) {
		if (tcounter_ > 0) {
			if (time_ > 0) {
				time_--;
			}

			if (time_ == 0) {
				tcounter_--;

				if (tcounter_ == PWD_CNT_BLOCK) {
					tcounter_ = 0;
				}

				if (tcounter_ > 0) {
					time_ = kTickToDecCounter;
				}
			}
		}

		if (tcounter_ != counter_) {
			change = true;
		}
	}

	return change;
}

//
bool TPwd::checkValue(uint8_t *pwd) const {
	bool check = true;

	for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
		check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
	}

	return check;
}

//
void TPwd::setValue(uint8_t *pwd){
	for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
		pwd_[i] = pwd[i];
	}
}

//----------------
//---- TUser -----
//----------------

TUser::TUser() {
	reset();
}

//
bool TUser::set(user_t val) {
	if ((val >= MIN) && (val < MAX)) {
		if (user_ != val) {
			time_ = kTimeToReset;
			user_ = val;
		}
	}

	return (user_ == val);
}

//
void TUser::tick() {
	if (time_ > 0) {
		time_--;
	}

	if (time_ == 0) {
		reset();
	}
}

//
void TUser::resetTimer() {
	if (user_ != OPERATOR) {
		time_ = kTimeToReset;
	}
}

void TUser::reset() {
        user_ = OPERATOR;
        time_ = 0;
    }
