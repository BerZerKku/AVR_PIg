#include "user.h"

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

//
bool TUser::checkChangeUser(user_t chuser) const {
    bool check = false;

    switch(chuser) {
    case USER_operator: {
        check = true;
    } break;
    case USER_engineer: {
        check = (user_ == USER_engineer) || (user_ == USER_admin);
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
