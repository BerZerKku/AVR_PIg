#include "user.h"

TUser::TUser() {
    reset();
    sevent = NULL;
}

//
bool TUser::set(userSrc_t src, user_t val) {
    bool isset = false;

    if ((src < USER_SOURCE_MAX) && (val < USER_MAX)) {
        if (stuser[src].user != val) {
            sevent->pushUserChanged(stuser[src].user, src, val);
            stuser[src].user = val;
            resetTimer(src);
        }
        isset = true;
    }

    return isset;
}

//
user_t TUser::get(userSrc_t src) const {
    return (src < USER_SOURCE_MAX) ? stuser[src].user : USER_MAX;
}

//
void TUser::tick() {
    for(uint8_t i = 0; i < USER_SOURCE_MAX; i++) {
        tick(static_cast<userSrc_t> (i));
    }
}

//
void TUser::resetTimer(userSrc_t src) {
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
uint16_t TUser::getTimer(userSrc_t src) const {
    return (src < USER_SOURCE_MAX) ? stuser[src].time : 0;
}

//
void TUser::reset() {
    for(uint8_t i = 0; i < USER_SOURCE_MAX; i++) {
        reset(static_cast<userSrc_t> (i));
    }
}

//
void TUser::reset(userSrc_t src) {
    if (src < USER_SOURCE_MAX) {
        stuser[src].user = USER_operator;
        resetTimer(src);
    }
}

//
bool TUser::checkAccess(userSrc_t src, user_t chuser) const {
    bool check = false;

    if (src < USER_SOURCE_MAX) {
        user_t user = stuser[src].user;

        switch(chuser) {
            case USER_operator: {
                check = true;
            } break;
            case USER_engineer: {
                check = (user == USER_engineer) || (user == USER_admin);
            } break;
            case USER_admin: {
                check = (user == USER_admin);
            } break;
            case USER_factory: break;
            case USER_MAX: break;
        }
    }

    return check;
}

//
void TUser::tick(userSrc_t src) {
    if (src < USER_SOURCE_MAX) {
        user_t user = stuser[src].user;

        if (stuser[src].time > 0) {
            if ((user > USER_operator) && (user < USER_MAX)) {
                stuser[src].time--;
            }
        }

        if (stuser[src].time == 0) {
            sevent->pushUserChangeAuto(stuser[src].user, src);
            reset(src);
        }
    }
}
