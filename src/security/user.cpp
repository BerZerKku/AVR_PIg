#include "user.h"

TUser::TUser() {
    reset();
}

//
bool
TUser::set(userSrc_t src, user_t val) {
    bool isset = false;

    if ((src < USER_SOURCE_MAX) && (val < USER_MAX)) {
        if (stuser[src].user != val) {
            stuser[src].user = val;
            stuser[src].olduser = val;
            resetTimer(src);
        }
        isset = true;
    }

    return isset;
}

//
user_t
TUser::get(userSrc_t src) const {
    return (src < USER_SOURCE_MAX) ? stuser[src].user : USER_MAX;
}

//
void
TUser::tick() {
    for(uint8_t i = 0; i < USER_SOURCE_MAX; i++) {
        tick(static_cast<userSrc_t> (i));
    }
}

//
bool
TUser::isAutoReset(userSrc_t src, user_t &olduser) {
    bool isautorst = false;

    if (src < USER_SOURCE_MAX) {
        olduser = stuser[src].olduser;
        isautorst = (stuser[src].user != olduser);
        stuser[src].olduser = stuser[src].user;
    }

    return isautorst;
}

//
void
TUser::resetTimer(userSrc_t src) {
    switch(src) {
        case USER_SOURCE_pi: {
            stuser[src].time = kTimeToResetPi;
        } break;
        case USER_SOURCE_pc: {
            stuser[src].time = kTimeToResetPc;
        } break;
        case USER_SOURCE_MAX: break;
    }
}

//
uint16_t
TUser::getTimer(userSrc_t src) const {
    return (src < USER_SOURCE_MAX) ? stuser[src].time : 0;
}

//
void
TUser::reset() {
    for(uint8_t i = 0; i < USER_SOURCE_MAX; i++) {
        reset(static_cast<userSrc_t> (i));
    }
}

//
void
TUser::reset(userSrc_t src) {
    if (src < USER_SOURCE_MAX) {
        stuser[src].user = USER_operator;
        stuser[src].olduser= stuser[src].user;
        resetTimer(src);
    }
}

//
void
TUser::tick(userSrc_t src) {
    if (src < USER_SOURCE_MAX) {
        user_t user = stuser[src].user;

        if (stuser[src].time > 0) {
            if ((user > USER_operator) && (user < USER_MAX)) {
                stuser[src].time--;
            }
        }

        if (stuser[src].time == 0) {
            reset(src);
            stuser[src].olduser = user;
        }
    }
}
