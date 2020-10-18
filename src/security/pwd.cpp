#include "pwd.h"

TPwd::TPwd() {
//    COMPILE_TIME_ASSERT(SIZE_OF(password[0].pwd) == PWD_LEN);
    COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);
    COMPILE_TIME_ASSERT(SIZE_OF(PWD_DEFAULT) == (PWD_LEN + 1));
    COMPILE_TIME_ASSERT(SIZE_OF(password) == (USER_MAX - USER_operator - 1));

    reset();
}

//
bool TPwd::checkPassword(user_t user, const uint8_t *pwd) {
    bool check = false;

    if (user == USER_operator) {
        check = true;
    } else if (pwd != NULL) {
         if (user == USER_factory) {
            check = true;
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                check &= (pwd[i] == PWD_RESET[i]);
            }
        } else  {
            int8_t index = getUserIndex(user);
            if (index >= 0) {
                if (!isLocked(user)) {
                    check = true;
                    for(uint8_t i = 0; i < PWD_LEN; i++) {
                        check &= (password[index].pwd[i] == pwd[i]);
                    }

                    if (check) {
                        clrCounter(index);
                    } else {
                        incCounter(index);
                    }
                }
            }
        }
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
bool TPwd::setPwd(user_t user, const uint8_t *pwd) {
    bool setnew = false;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        bool changed = false;
        if (checkValue(pwd)) {
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                if (password[index].pwd[i] != pwd[i]) {
                    changed = true;
                    if (!isChangedPwd(index)) {
                        password[index].pwd[i] = pwd[i];
                    }
                }
            }

            if (!changed) {
                password[index].changed = changed;
            }

            setnew = true;
        }
    }

    return setnew;
}

//
bool TPwd::changePwd(user_t user, const uint8_t *pwd) {
    bool changed  = false;
    int8_t index = getUserIndex(user);

    if(index >= 0) {
        changed = changePwd(index, pwd);
    }

    return changed;
}

//
void TPwd::setCounter(user_t user, uint8_t value) {
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        if (!isInit(index)) {
            uint8_t counter = (value <= 2*PWD_CNT_BLOCK) ? value : 2*PWD_CNT_BLOCK;

            password[index].counter = counter;
            password[index].init = true;

            setTicks(index, counter > 0);
        }
        password[index].tcounter = value;
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
uint8_t TPwd::isCounterChanged(user_t user) const {
    bool ischange = false;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        ischange = (password[index].counter != password[index].tcounter);
    }

    return ischange;
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
        } else {
            time = 9999;
        }
    }

    return time;
}

//
void TPwd::reset() {
    resetState = RESET_STATE_no;
    for(uint8_t user = USER_operator; user < USER_MAX; user++) {
        reset(static_cast<user_t> (user));
    }
}

//
void TPwd::reset(user_t user) {
    int8_t index = getUserIndex(user);

    resetState = RESET_STATE_no;

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
void TPwd::tick() {
    for(uint8_t index = 0; index < SIZE_OF(password); index++) {
        tick(index);
    }
}

//
bool TPwd::isInit(user_t user) const {
    bool init = false;
    int8_t index = getUserIndex(user);

    if (user == USER_operator) {
        init = true;
    } else if (index >= 0) {
        init = isInit(index);
    } else {
        init = false;
    }

    return init;
}

//
bool TPwd::isLocked(user_t user) const {
    bool lock = true;

    if (user == USER_operator) {
        lock = false;
    } else if (user == USER_factory) {
        lock = false;
    } else {
        int8_t index = getUserIndex(user);
        if (index >= 0) {
            lock = password[index].counter >= PWD_CNT_BLOCK;
        } else {
            lock = true;
        }
    }

    return lock;
}

//
bool TPwd::isChangedPwd(user_t user) const {
    bool changed = true;
    int8_t index = getUserIndex(user);

    if (index >= 0) {
        changed = isChangedPwd(index);
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

//
void TPwd::resetPwdToDefault() {
    resetState = RESET_STATE_waitDisable;
    setTicks(0, false);
}

//
void TPwd::resetPwdToDefaultCycle(eGB_REGIME regime) {
    static const uint8_t pwdDefault[] = PWD_DEFAULT;
    int8_t index = 0;

    if (resetState >= RESET_STATE_MAX) {
        resetState = RESET_STATE_no;
    }

    if (password[index].ticks > 0) {
        password[index].ticks--;
    }

    switch(resetState) {
    case RESET_STATE_waitDisable: {
        if (regime == GB_REGIME_DISABLED) {
            resetState = RESET_STATE_waitDisableTime;
            setTicks(index, true);
        } else if (password[index].ticks == 0)  {
            resetState = RESET_STATE_disable;
            setTicks(index, false);
        }
    } break;
    case RESET_STATE_disable: {
        resetState = RESET_STATE_waitDisable;
        setTicks(index, true);
    } break;
    case RESET_STATE_waitDisableTime: {
        if (regime != GB_REGIME_DISABLED) {
            resetState = RESET_STATE_waitDisable;
            setTicks(index, false);
        } else if (password[index].ticks == 0) {
            resetState = RESET_STATE_resetPassword;
            for(uint8_t i = 0; i < SIZE_OF(password); i++) {
                changePwd(i, pwdDefault);
            }
        }
        if (resetState != RESET_STATE_waitDisableTime) {
            setTicks(index, false);
        }
    } break;
    case RESET_STATE_resetPassword: {
        resetState = RESET_STATE_waitReset;
        setTicks(index, true);
    } break;
    case RESET_STATE_waitReset: {
        if (password[index].ticks == 0) {
            bool changed = false;
            for(uint8_t i = 0; i < SIZE_OF(password); i++) {
                changed |= password[i].changed;
            }
            if (!changed) {
                resetState = RESET_STATE_enable;
            } else {
                resetState = RESET_STATE_resetPassword;
            }
            setTicks(index, true);
        }
    } break;
    case RESET_STATE_enable: {
        resetState = RESET_STATE_waitEnable;
        setTicks(index, true);
    } break;
    case RESET_STATE_waitEnable: {
        if (regime == GB_REGIME_ENABLED) {
            reset();
        } else if (password[index].ticks == 0) {
            resetState = RESET_STATE_enable;
            setTicks(index, false);
        }
    } break;
    case RESET_STATE_no: break;
    case RESET_STATE_MAX: break;
    }
}

//
bool TPwd::isResetToDefault() {
    return resetState != RESET_STATE_no;
}

bool TPwd::isWaitDisableDevice() {
    return resetState == RESET_STATE_disable;
}

//
bool TPwd::isWaitResetPassword() {
    return resetState == RESET_STATE_resetPassword;
}

//
bool TPwd::isWaitEnableDevice() {
    return resetState == RESET_STATE_enable;
}

//
bool TPwd::isChangedPwd(int8_t index) const {
    Q_ASSERT(index < SIZE_OF(password));

    return password[index].changed;
}

//
bool TPwd::isInit(int8_t index) const {
    Q_ASSERT(index < SIZE_OF(password));

    return password[index].init;
}

//
bool TPwd::changePwd(int8_t index, const uint8_t *pwd) {
    bool changed =false;

    Q_ASSERT(index < SIZE_OF(password));

    if (checkValue(pwd)) {
        changed = true;
        for(uint8_t i = 0; i < PWD_LEN; i++) {
            if (password[index].pwd[i] != pwd[i]) {
                changed = true;
                password[index].pwd[i] = pwd[i];
            }
        }
        clrCounter(index);
        password[index].changed = changed;
    }

    return changed;
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
void TPwd::incCounter(int8_t index) {
    uint8_t counter = password[index].counter;

    Q_ASSERT(index < SIZE_OF(password));

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

//
void TPwd::clrCounter(int8_t index) {
    Q_ASSERT(index < SIZE_OF(password));

    if (isInit(index)) {
        password[index].counter = 0;
    }
}

//
int8_t TPwd::getUserIndex(user_t user) const {
    int8_t index = -1;

    if ((user > USER_operator) && (user < USER_MAX)) {
        index = user - 1;
    }

    return index;
}

//
void TPwd::setTicks(int8_t index, bool enable) {
    uint16_t ticks = 0;

    Q_ASSERT(index < SIZE_OF(password));

    if (enable) {
        if (resetState == RESET_STATE_waitDisableTime) {
            ticks = kTickToRestPwd;
        } else if (resetState != RESET_STATE_no) {
            ticks = kTickWait;
        } else {
            ticks = kTickToDecCounter;
        }
    }

    password[index].ticks = ticks;
}

//
void TPwd::tick(int8_t index) {
    Q_ASSERT(index < SIZE_OF(password));

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
    }
}
