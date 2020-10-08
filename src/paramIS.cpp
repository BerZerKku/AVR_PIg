/*
 * paramIs.cpp
 *
 *  Created on: 07.10.2020
 *      Author: Shcheblykin
 */

#include "paramIS.h"

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
			user_ = val;
            resetTimer();
		}
	}

	return (user_ == val);
}

//
void TUser::tick() {
    if ((time_ > 0) && (user_ != OPERATOR)) {
		time_--;
	}

	if (time_ == 0) {
		reset();
	}
}

//
void TUser::resetTimer() {
    time_ = kTimeToReset;
}

//
void TUser::reset() {
    user_ = OPERATOR;
    resetTimer();
}

//----------------
//---- TUser -----
//----------------

//
TPwd::TPwd() {
    COMPILE_TIME_ASSERT(SIZE_OF(pwd_t::pwd) == PWD_LEN);
    COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);

    for(uint8_t user = TUser::MIN; user < TUser::MAX; user++) {
        reset(static_cast<TUser::user_t> (user));
    }
}

//
bool TPwd::checkPassword(TUser::user_t user, const uint8_t *pwd) {
    bool check = false;
    int8_t index = getUserIndex(user);

    if (user == TUser::OPERATOR) {
        check = true;
    } else if (index >= 0) {
        if (!isLocked(user)) {
            check = true;
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                check &= (password[index].pwd[i] == pwd[i]);
            }
        }
    } else {
        check = false;
    }

    return check;
}

//
uint8_t* TPwd::getPwd(TUser::user_t user) {
    uint8_t *array = NULL;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        array = password[index].pwd;
    }

    return array;
}

//
bool TPwd::setPwd(TUser::user_t user, uint8_t *pwd) {
    bool setnew = false;
    int8_t index = getUserIndex(user);

    if(index >= 0) {
        if (checkValue(pwd)) {
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                password[index].pwd[i] = pwd[i];
            }
            setnew = true;
        }
    }

    return setnew;
}

//
void TPwd::setCounter(TUser::user_t user, uint8_t value) {
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        if (!isInit(index)) {
            uint8_t counter = (value <= 2*PWD_CNT_BLOCK) ? value : 2*PWD_CNT_BLOCK;

            password[index].counter = counter;
            password[index].init = true;

            setTicks(user, counter > 0);
        }

        password[index].tcounter = value;
    }
}

//
void TPwd::clrCounter(TUser::user_t user) {
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        if (isInit(index)) {
            password[index].counter = 0;
        }
    }
}

//
void TPwd::incCounter(TUser::user_t user) {
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        uint8_t counter = password[index].counter;

        if (counter == 0) {
            setTicks(index, true);
        }

        if (isInit(index)) {
            if (counter < PWD_CNT_BLOCK) {
                counter++;
            }

            if (counter >= PWD_CNT_BLOCK) {
                counter = 2*PWD_CNT_BLOCK;
            }
        }

        password[index].counter = counter;
    }
}

uint8_t TPwd::getCounter(TUser::user_t user) const {
    uint8_t counter = 0;
    int8_t index = getUserIndex(user);

    if (user >= TUser::MAX) {
        counter = 2*PWD_CNT_BLOCK;
    } else if (index >= 0 ) {
        counter = password[index].counter;
    }

    return counter;
}

//
uint16_t TPwd::getLockTime(TUser::user_t user) const {
    uint16_t time = 0;
    int8_t index = getUserIndex(user);

    if (isLocked(user)) {
        if (index >= 0) {
           time = password[index].ticks;
           uint8_t counter = password[index].counter;
           if(counter > (PWD_CNT_BLOCK + 1)) {
               time += (counter - PWD_CNT_BLOCK -1 ) * kTickToDecCounter;
           }
           time /= (1000 / MENU_TIME_CYLCE);
        } else {
            time = 9999;
        }
    }

    return time;
}

void TPwd::reset(TUser::user_t user) {
    int8_t index = getUserIndex(user);

    if (index >=0) {
        password[index].init = false;
        password[index].counter = 2*PWD_CNT_BLOCK;
        password[index].tcounter = password[index].counter;
        setTicks(index, true);

        for(uint8_t i = 0; i < PWD_LEN; i++) {
            password[index].pwd[i] = 0;
        }
    }
}

//
bool TPwd::tick(TUser::user_t user) {
    bool change = false;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        if (isInit(index)) {
            uint8_t counter = password[index].counter;

            if (counter == PWD_CNT_BLOCK) {
                counter = 2*PWD_CNT_BLOCK;
            }

            if (counter > 0) {
                if (password[index].ticks > 0) {
                    password[index].ticks--;
                }

                if (password[index].ticks == 0) {
                    counter--;

                    if (counter == PWD_CNT_BLOCK) {
                        counter = 0;
                    }

                    setTicks(index, counter > 0);
                }
            }

            password[index].counter = counter;
            change = (counter != password[index].tcounter);
        }
    }

    return change;
}

//
bool TPwd::isInit(TUser::user_t user) const {
    bool init = false;
    int8_t index = getUserIndex(user);

    if (user == TUser::OPERATOR) {
        init = true;
    } else if (index >= 0) {
        init = password[index].init;
    } else {
        init = false;
    }

    return init;
}

//
bool TPwd::isLocked(TUser::user_t user) const {
    bool lock = true;
    int8_t index = getUserIndex(user);

    if (user == TUser::OPERATOR) {
        lock = false;
    } else if (index >= 0) {
        lock = password[index].counter >= PWD_CNT_BLOCK;
    } else {
        lock = true;
    }

    return lock;
}

//
bool TPwd::isInit(int8_t index) const {
    return password[index].init;
}

//
bool TPwd::checkValue(uint8_t *pwd) const {
    bool check = true;

    for(uint8_t i = 0; i < PWD_LEN; i++) {
        check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
    }

    return check;
}

//
int8_t TPwd::getUserIndex(TUser::user_t user) const {
    int8_t index = -1;

    if ((user > TUser::OPERATOR) && (user < TUser::MAX)) {
        index = user - TUser::OPERATOR;
    }

    return index;
}

//
void TPwd::setTicks(int8_t index, bool enable) {
    password[index].ticks = enable ? kTickToDecCounter : 0;
}
