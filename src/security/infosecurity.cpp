#include "infosecurity.h"

// ���� �������.
typedef struct __attribute__ ((__packed__)) {
    eGB_COM com;    ///< �������.
    user_t user;    ///< ����������� ������������ ��� ����������� �������.
    bool activity;  ///< ���� ���������� �������.
} field_t;

// ������ ��������� ������ ��� ������ � ��.
static const field_t commands[] PROGMEM = {
    {GB_COM_GET_USER,           USER_operator,  false},
    {GB_COM_GET_SOST,           USER_operator,  false},
    {GB_COM_GET_FAULT,          USER_operator,  false},
    {GB_COM_GET_TIME,           USER_operator,  false},
    {GB_COM_GET_MEAS,           USER_operator,  false},
    {GB_COM_SET_USER,           USER_operator,  true},
    {GB_COM_SET_REG_DISABLED,   USER_engineer,  true},
    {GB_COM_SET_REG_ENABLED,    USER_engineer,  true},
    {GB_COM_SET_REG_TEST_1,     USER_engineer,  true},
    {GB_COM_SET_REG_TEST_2,     USER_engineer,  true},
    {GB_COM_SET_TIME,           USER_engineer,  true},
    {GB_COM_GET_VERS,           USER_operator,  true},
    {GB_COM_GET_TEST,           USER_engineer,  true},
    {GB_COM_GET_JRN_CNT,        USER_operator,  true},
    {GB_COM_GET_JRN_ENTRY,      USER_operator,  true},
    {GB_COM_GET_JRN_IS_CNT,     USER_admin,     true},
    {GB_COM_GET_JRN_IS_ENTRY,   USER_admin,     true},
    {GB_COM_PRM_GET_JRN_CNT,    USER_operator,  true},
    {GB_COM_PRM_GET_JRN_ENTRY,  USER_operator,  true},
    {GB_COM_PRD_GET_JRN_CNT,    USER_operator,  true},
    {GB_COM_PRD_GET_JRN_ENTRY,  USER_operator,  true},
    {GB_COM_DEF_GET_JRN_CNT,    USER_operator,  true},
    {GB_COM_DEF_GET_JRN_ENTRY,  USER_operator,  true},
    {GB_COM_SET_CONTROL,        USER_operator,  true},
    {GB_COM_PRM_RES_IND,        USER_operator,  true},
    {GB_COM_PRD_RES_IND,        USER_operator,  true},
    {GB_COM_GET_COM_PRD_KEEP,   USER_operator,  true},
    {GB_COM_PRD_GET_RING_COM_TR, USER_operator, true},
    {GB_COM_PRM_GET_RING_COM_REC, USER_operator, true},
    {(eGB_COM) 0x35,            USER_operator,  true}, // FIXME ������� ��������� � GB_COM_GET_TIME_SINCHR. ��������� ��� ��� �� ������ ��.
    {(eGB_COM) 0x00,            USER_operator,  true}, // FIXME ������� GB_COM_NO = 0x00 ������������ � �������������. ��������� �� �����������.
    {(eGB_COM) 0x10,            USER_operator,  true},
    {(eGB_COM) 0x20,            USER_operator,  true},
    {(eGB_COM) 0xB0,            USER_engineer,  true},
    {(eGB_COM) 0x80,            USER_engineer,  true},
    {(eGB_COM) 0xA0,            USER_engineer,  true},
    {(eGB_COM) 0x90,            USER_engineer,  true},
    {GB_COM_SET_CF_THRESHOLD,   USER_engineer,  true},
    {GB_COM_PRD_SET_RING_COM_TR, USER_engineer, true},
    {GB_COM_PRM_SET_RING_COM_REC, USER_engineer, true},
    {GB_COM_PRD_SET_DR_STATE,   USER_engineer,  true},
    {GB_COM_PRD_SET_DR_BLOCK,   USER_engineer,  true},
    {GB_COM_PRM_SET_DR_STATE,   USER_engineer,  true},
    {GB_COM_PRM_SET_DR_BLOCK,   USER_engineer,  true}
};

//
TInfoSecurity::TInfoSecurity() {
    usr.set(USER_SOURCE_pi, USER_operator);
    usr.set(USER_SOURCE_pc, USER_operator);
}

void
TInfoSecurity::tick(eGB_REGIME regime, eGB_COM &com) {

    usr.tick();

    for(uint8_t i = 0; i < USER_SOURCE_MAX; i++) {
        user_t user = USER_MAX;
        userSrc_t src = static_cast<userSrc_t> (i);
        if (usr.isAutoReset(src, user)) {
            sevent.pushUserChangeAuto(user, src);
        }
    }

    if (pwd.isResetToDefault()) {
        pwd.resetPwdToDefaultCycle(regime);

        if (pwd.isWaitDisableDevice()) {
            com = GB_COM_SET_REG_DISABLED;
        } else if (pwd.isWaitEnableDevice()) {
            com = GB_COM_SET_REG_ENABLED;
        } else {
            com = GB_COM_NO;
        }
    } else {
        pwd.tick();
    }
}

//
TInfoSecurity::state_t
TInfoSecurity::setUserPc(user_t user, const uint8_t *p) {
    state_t state;

    if (user == USER_operator) {
        state = STATE_OK;
        setUser(user, USER_SOURCE_pc);
    } else  if (user >= USER_MAX) {
        state = STATE_NO_ACCESS;
    } else if (pwd.isLocked(user)) {
        state = STATE_NO_ACCESS;
    } else {
        if (checkUserPwd(user, USER_SOURCE_pc, p)) {
            state = STATE_OK;
            setUser(user, USER_SOURCE_pc);
        } else {
            state = STATE_WRONG_PWD;
        }
    }

    return state;
}

bool
TInfoSecurity::checkUserAccess(user_t user, userSrc_t src) {
    bool isaccess = false;

    if (src < USER_SOURCE_MAX) {
        user_t cuser = usr.get(src);

        switch(user) {
            case USER_operator: {
                isaccess = true;
            } break;
            case USER_engineer: {
                isaccess = (cuser == USER_engineer) || (cuser == USER_admin);
            } break;
            case USER_admin: {
                isaccess = (cuser == USER_admin);
            } break;
            case USER_factory: break;
            case USER_MAX: break;
        }
    }

    return isaccess;
}

bool
TInfoSecurity::setUser(user_t user, userSrc_t src) {
    bool isset;
    user_t nuser;
    user_t cuser = usr.get(src);

    isset = usr.set(src, user);
    nuser = usr.get(src);

    if (nuser != USER_operator) {
        for(uint8_t i = 0 ; i < USER_SOURCE_MAX; i++) {
            if (i != src) {
                userSrc_t isrc = static_cast<userSrc_t> (i);
                user_t iuser = usr.get(isrc);
                if (iuser != USER_operator) {
                    usr.set(isrc, USER_operator);
                    sevent.pushUserChangeAuto(iuser, isrc);
                }
            }
        }
    }

    if (cuser != nuser) {
        sevent.pushUserChanged(cuser, src, nuser);
    }

    return isset;
}

//
TInfoSecurity::state_t
TInfoSecurity::changeUserPcPwd(
    user_t user, const uint8_t *cp, const uint8_t *np) {

    state_t state;
    user_t cuser = usr.get(USER_SOURCE_pc);

    if (pwd.isResetToDefault()) {
        state = STATE_NO_ACCESS;
    } else if (user >= USER_MAX) {
        state = STATE_NO_ACCESS;
    } else  if (pwd.isLocked(cuser)) {
        state = STATE_NO_ACCESS;
    } else if (!checkUserAccess(user, USER_SOURCE_pc)) {
        state = STATE_NO_ACCESS;
    } else {
        if (checkUserPwd(cuser, USER_SOURCE_pc, cp)) {
            if (changeUserPwd(user, USER_SOURCE_pc, np)) {
                state = STATE_OK;
            } else {
                state = STATE_WRONG_NEW_PWD;
            }
        } else {
            state =  STATE_WRONG_PWD;
        }
    }

    return state;
}

//
bool
TInfoSecurity::changeUserPiPwd(eGB_PARAM param, const uint8_t *np) {
    bool ischange = false;

    for(uint8_t i = USER_operator + 1; i < USER_MAX; i++) {
        user_t user = static_cast<user_t> (i);
        if (getPwdParam(user) == param) {
            ischange = changeUserPwd(user, USER_SOURCE_pi, np);
            break;
        }
    }

    return ischange;
}

//
bool
TInfoSecurity::checkUserPwd(user_t user, userSrc_t src, const uint8_t *cp) {
    bool ischeck = false;

    ischeck = pwd.checkPassword(user, cp);
    if ((!ischeck) && (user != USER_factory)) {
        user_t cuser = usr.get(src);
        if (pwd.isLocked(user)) {
            sevent.pushPwdBlocked(cuser, src, user);
        } else {
            sevent.pushPwdWrong(cuser, src, user);
        }
    }

    return ischeck;
}

//
TInfoSecurity::state_t
TInfoSecurity::isComAccess(eGB_COM com) {
    state_t state = STATE_NO_ACCESS;

    for(uint8_t i = 0; i < SIZE_OF(commands); i++) {
        if (pgm_read_byte(&commands[i].com) == com) {
            user_t user = static_cast<user_t> (pgm_read_byte(&commands[i].user));
            if (checkUserAccess(user, USER_SOURCE_pc)) {
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

//
eGB_PARAM
TInfoSecurity::getPwdParam(user_t user) const {
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
bool
TInfoSecurity::changeUserPwd(user_t user, userSrc_t src, const uint8_t *np) {
    bool ischanged = false;

    if (pwd.changePwd(user, np)) {
        user_t cuser = usr.get(src);
        sevent.pushPwdChanged(cuser, src, user);
        ischanged = true;;
    }

    return ischanged;
}
