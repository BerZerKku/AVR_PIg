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
    if (val < USER_MAX) {
		if (user_ != val) {
			user_ = val;
            resetTimer();
		}
	}

	return (user_ == val);
}

//
void TUser::tick() {
    if (time_ > 0) {
        if ((user_ > USER_operator) && (user_ < USER_MAX)) {
            time_--;
        }
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
    user_ = USER_operator;
    resetTimer();
}


bool TUser::checkChangeUser(user_t chuser) const {
    bool check = false;

    switch(chuser) {
        case USER_operator: {
            check = true;
        } break;
        case USER_engineer: {
            check = (user_ >= USER_engineer);
        } break;
        case USER_admin: {
            check = (user_ == USER_admin);
        } break;
        case USER_factory: {
            check = (user_ == USER_factory);
        } break;
        case USER_MAX: break;
    }

    return check;
}


//----------------
//---- TPWD ------
//----------------

//
TPwd::TPwd() {
    COMPILE_TIME_ASSERT(SIZE_OF(pwd_t::pwd) == PWD_LEN);
    COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);

    reset();
}

//
bool TPwd::checkPassword(user_t user, const uint8_t *pwd) {
    bool check = false;
    int8_t index = getUserIndex(user);

    if (user == USER_operator) {
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
uint8_t* TPwd::getPwd(user_t user) {
    uint8_t *array = NULL;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        array = password[index].pwd;
    }

    return array;
}

//
bool TPwd::setPwd(user_t user, const uint8_t *pwd, bool bsp) {
    bool setnew = false;
    int8_t index = getUserIndex(user);

    if(index >= 0) {
        if (checkValue(pwd)) {
            bool changed = false;
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                if (password[index].pwd[i] != pwd[i]) {
                    changed = true;
                    if (!bsp || !isChangedPwd(user)) {
                        password[index].pwd[i] = pwd[i];

                    }
                }                
            }
            password[index].changed = changed;
            setnew = true;
        }
    }

    return setnew;
}

//
bool TPwd::setPwd(eGB_PARAM param, const uint8_t *pwd, bool bsp) {
    bool setnew = false;

    for(uint8_t i = 0; i < USER_MAX; i++) {
        user_t user = static_cast<user_t> (i);
        if (getPwdParam(user) == param) {
            setnew = setPwd(user, pwd, bsp);
        }
    }

    return setnew;
}

//
void TPwd::setCounter(user_t user, uint8_t value) {
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
void TPwd::clrCounter(user_t user) {
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        if (isInit(index)) {
            password[index].counter = 0;
        }
    }
}

//
void TPwd::incCounter(user_t user) {
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
        qDebug() << "user = " << user << ", set counter = " << counter;
        password[index].counter = counter;
    }
}

uint8_t TPwd::getCounter(user_t user) const {
    uint8_t counter = 0;
    int8_t index = getUserIndex(user);

    if (user >= USER_MAX) {
        counter = 2*PWD_CNT_BLOCK;
    } else if (index >= 0 ) {
        counter = password[index].counter;
    }

    return counter;
}

//
uint16_t TPwd::getLockTime(user_t user) const {
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

//
void TPwd::reset() {
    for(uint8_t user = USER_operator; user < USER_MAX; user++) {
        reset(static_cast<user_t> (user));
    }
}

//
void TPwd::reset(user_t user) {
    int8_t index = getUserIndex(user);

    if (index >=0) {
        password[index].init = false;
        password[index].counter = 2*PWD_CNT_BLOCK;
        password[index].tcounter = password[index].counter;
        password[index].changed = false;
        setTicks(index, true);

        for(uint8_t i = 0; i < PWD_LEN; i++) {
            password[index].pwd[i] = 0;
        }
    }
}

//
bool TPwd::tick(user_t user) {
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
bool TPwd::isInit(user_t user) const {
    bool init = false;
    int8_t index = getUserIndex(user);

    if (user == USER_operator) {
        init = true;
    } else if (index >= 0) {
        init = password[index].init;
    } else {
        init = false;
    }

    return init;
}

//
bool TPwd::isLocked(user_t user) const {
    bool lock = true;
    int8_t index = getUserIndex(user);

    if (user == USER_operator) {
        lock = false;
    } else if (index >= 0) {
        lock = password[index].counter >= PWD_CNT_BLOCK;
    } else {
        lock = true;
    }

    return lock;
}

//
bool TPwd::isChangedPwd(user_t user) const {
    bool changed = true;
    int8_t index = getUserIndex(user);

     if (index >= 0) {
        changed = password[index].changed;
    } else {
        changed = false;
    }

     return changed;
}

eGB_PARAM TPwd::getCounterParam(user_t user) const {
    eGB_PARAM param = GB_PARAM_MAX;

    switch(user) {
        case USER_engineer: {
            param = GB_PARAM_IS_PWD_ENG_CNT;
        } break;
        case USER_admin: {
            param = GB_PARAM_IS_PWD_ADM_CNT;
        } break;

        case USER_operator: // DOWN
        case USER_factory:  // DOWN
        case USER_MAX: break;
    }

    return param;
}

eGB_PARAM TPwd::getPwdParam(user_t user) const {
    eGB_PARAM param = GB_PARAM_MAX;

    switch(user) {
        case USER_engineer: {
            param = GB_PARAM_IS_PWD_ENGINEER;
        } break;
        case USER_admin: {
            param = GB_PARAM_IS_PWD_ADMIN;
        } break;

        case USER_operator: // DOWN
        case USER_factory:  // DOWN
        case USER_MAX: break;
    }

    return param;
}

//
bool TPwd::isInit(int8_t index) const {
    return password[index].init;
}

//
bool TPwd::checkValue(const uint8_t *pwd) const {
    bool check = true;

    for(uint8_t i = 0; i < PWD_LEN; i++) {
        check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
    }

    return check;
}

//
int8_t TPwd::getUserIndex(user_t user) const {
    int8_t index = -1;

    if ((user > USER_operator) && (user < USER_MAX)) {
        index = user - USER_operator;
    }

    return index;
}

//
void TPwd::setTicks(int8_t index, bool enable) {
    password[index].ticks = enable ? kTickToDecCounter : 0;
}

//----------------
//---- TUser -----
//----------------


