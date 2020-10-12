#include "infosecurity.h"

TInfoSecurity::state_t TInfoSecurity::setUserPc(user_t user, const uint8_t *p) {
    state_t state;

    if (user == USER_operator) {
        state = STATE_OK;
        UserPc.set(user);
    } else  if (user >= USER_MAX) {
        state = STATE_NO_ACCESS;
    } else if (pwd.isLocked(user)) {
        state = STATE_NO_ACCESS;
    } else if ((p != NULL) && pwd.checkPassword(user, p)) {
        state = STATE_OK;
        UserPc.set(user);
    } else {
        state = STATE_WRONG_PWD;
    }

    return state;
}

TInfoSecurity::state_t TInfoSecurity::changeUserPcPwd(
    user_t user, const uint8_t *cp, const uint8_t *np) {

    state_t state;
    user_t cuser = UserPc.get();

    if (pwd.isResetToDefault()) {
        qDebug() << "pwd.isResetToDefault()";
        state = STATE_NO_ACCESS;
    } else if (user >= USER_MAX) {
        qDebug() << "user >= USER_MAX";
        state = STATE_NO_ACCESS;
    } else  if (pwd.isLocked(cuser)) {
        qDebug() << "pwd.isLocked(cuser)";
        state = STATE_NO_ACCESS;
    } else if (!UserPc.checkChangeUser(user)) {
        qDebug() << "UserPc.checkChangeUser(user)";
        state = STATE_NO_ACCESS;
    } else if (pwd.checkPassword(cuser, cp)) {
        qDebug() << "pwd.checkPassword(cuser, cp)";
        if (pwd.changePwd(user, np)) {
            state = STATE_WRONG_NEW_PWD;
        }
    } else {
        state =  STATE_WRONG_PWD;
    }
}
