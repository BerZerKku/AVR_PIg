#include "infosecurity.h"

// Поля команды.
typedef struct __attribute__ ((__packed__)) {
    eGB_COM com;    ///< Команда.
    user_t user;    ///< Необходимый пользователь для прохождения команды.
    bool activity;  ///< Флаг активности команды.
} field_t;

// Список доступных команд для работы с ПК.
static const field_t commands[] PROGMEM = {
    {GB_COM_GET_USER,           USER_operator,  false},
    {GB_COM_GET_SOST,           USER_operator,  false},
    {GB_COM_GET_FAULT,          USER_operator,  false},
    {GB_COM_GET_TIME,           USER_operator,  false},
    {GB_COM_SET_USER,           USER_operator,  true},
    {GB_COM_SET_REG_DISABLED,   USER_engineer,  true},
    {GB_COM_SET_REG_ENABLED,    USER_engineer,  true},
    {GB_COM_SET_TIME,           USER_engineer,  true},
    {GB_COM_GET_VERS,           USER_operator,  true}
};

//
TInfoSecurity::TInfoSecurity() {
    usr.set(USER_SOURCE_pi, USER_operator);
    usr.set(USER_SOURCE_pc, USER_operator);

    usr.setInfoSecurity(&sevent);
}

//
TInfoSecurity::state_t TInfoSecurity::setUserPc(user_t user, const uint8_t *p) {
    state_t state;

    if (user == USER_operator) {
        state = STATE_OK;
        usr.set(USER_SOURCE_pc, user);
    } else  if (user >= USER_MAX) {
        state = STATE_NO_ACCESS;
    } else if (pwd.isLocked(user)) {
        state = STATE_NO_ACCESS;
    } else if ((p != NULL) && pwd.checkPassword(user, p)) {
        state = STATE_OK;
        usr.set(USER_SOURCE_pc, user);
    } else {
        user_t cuser = usr.get(USER_SOURCE_pc);
        if (pwd.isLocked(user)) {
            sevent.pushPwdBlocked(cuser, USER_SOURCE_pc, user);
        } else {
            sevent.pushPwdWrong(cuser, USER_SOURCE_pc, user);
        }
        state = STATE_WRONG_PWD;
    }

    return state;
}

TInfoSecurity::state_t TInfoSecurity::changeUserPcPwd(
    user_t user, const uint8_t *cp, const uint8_t *np) {

    state_t state;
    user_t cuser = usr.get(USER_SOURCE_pc);

    if (pwd.isResetToDefault()) {
        state = STATE_NO_ACCESS;
    } else if (user >= USER_MAX) {
        state = STATE_NO_ACCESS;
    } else  if (pwd.isLocked(cuser)) {
        state = STATE_NO_ACCESS;
    } else if (!usr.checkAccess(USER_SOURCE_pc, user)) {
        state = STATE_NO_ACCESS;
    } else {
        if (pwd.checkPassword(cuser, cp)) {
            if (pwd.changePwd(user, np)) {
                sevent.pushPwdChanged(cuser, USER_SOURCE_pc, user);
                state = STATE_OK;
            } else {
                state = STATE_WRONG_NEW_PWD;
            }
        } else {
            if (pwd.isLocked(cuser)) {
                sevent.pushPwdBlocked(cuser, USER_SOURCE_pc, user);
            } else {
                sevent.pushPwdWrong(cuser, USER_SOURCE_pc, user);
            }
            state =  STATE_WRONG_PWD;
        }
    }

    return state;
}

//
TInfoSecurity::state_t TInfoSecurity::isComAccess(eGB_COM com) {
    state_t state = STATE_NO_ACCESS;

    for(uint8_t i = 0; i < SIZE_OF(commands); i++) {
        if (pgm_read_byte(&commands[i].com) == com) {
            user_t user = static_cast<user_t> (pgm_read_byte(&commands[i].user));
            if (usr.checkAccess(USER_SOURCE_pc, user)) {
                state = STATE_OK;
                if (pgm_read_byte(&commands[i].activity)) {
                    usr.resetTimer(USER_SOURCE_pc);
                }
            }
            break;
        }
    }

    return state;
}
