#include "securityevent.h"

//
TSecurityEvent::TSecurityEvent() {
    eventMsg.flush();
}

//
PGM_P
TSecurityEvent::getEventString(TSecurityEvent::event_t event) {
    static const char fcEventString[][21] PROGMEM = {
        //2345678901234567890
        "Авто. заверш. сеанса", // EVENT_autoEnd
        "Изменение настроек",   // EVENT_chgSettings
        "Изменение даты/врем",  // EVENT_chgTime
        "Изменен. на Оператор", // EVENT_setOperator
        "Изменен. на Инженер",  // EVENT_setEngineer
        "Изменен. на Админ.",   // EVENT_setAdmin
        "Блокировка Инженера",  // EVENT_blkEngineer
        "Блокировка Админ.",    // EVENT_blkAdmin
        "Изм. пароля Инженера", // EVENT_chgPwdEng
        "Изм. пароля Админ.",   // EVENT_chgPwdAdm
        "Ош. пароль Инженера",  // EVENT_wrgPwdEng
        "Ош. пароль Админ.",    // EVENT_wrgPwdAdm
        "Сброс паролей",        // EVENT_resetPwd
		"Режим Введен",			// EVENT_regEnable
		"Режим Выведен",		// EVENT_regDisable
		"Режим Тест ПРД",		// EVENT_regTestPrd
		"Режим Тест ПРМ",		// EVENT_regTestPrm
        "Событие - %d"          // EVENT_MAX
    };

    COMPILE_TIME_ASSERT(SIZE_OF(fcEventString) == (EVENT_MAX + 1));

    if (event > EVENT_MAX) {
        event = EVENT_MAX;
    }

    return fcEventString[event];
}

//
PGM_P
TSecurityEvent::getUserSourceString(userSrc_t src) {
    static const char fcUserSourceString[][3] PROGMEM = {
        //23
        "ПИ",
        "ПК",
        "ОШ"
    };

    COMPILE_TIME_ASSERT(SIZE_OF(fcUserSourceString) == (USER_SOURCE_MAX + 1));

    if (src > USER_SOURCE_MAX) {
        src = USER_SOURCE_MAX;
    }

    return fcUserSourceString[src];
}

//
bool TSecurityEvent::pop(user_t &user, userSrc_t &source, event_t &event) {

    eventMsg_t msg;
    bool ismsg = eventMsg.pop(msg);

    if (ismsg) {
        user = msg.user;
        source = msg.source;
        event = msg.event;
    }

    return ismsg;
}

//
bool TSecurityEvent::pushPwdBlocked(user_t cuser, userSrc_t source, user_t buser) {
    TSecurityEvent::event_t event = EVENT_MAX;

    switch(buser) {
        case USER_engineer: {
            event = EVENT_blkEngineer;
        } break;
        case USER_admin: {
            event = EVENT_blkAdmin;
        } break;
        case USER_operator: break;
        case USER_MAX: break;
        case USER_factory: break;
    }

    return push(cuser, source, event);
}

//
bool TSecurityEvent::pushPwdChanged(user_t cuser, userSrc_t source, user_t puser) {
    TSecurityEvent::event_t event = EVENT_MAX;

    switch(puser) {
        case USER_engineer: {
            event = TSecurityEvent::EVENT_chgPwdEng;
        } break;
        case USER_admin: {
            event = TSecurityEvent::EVENT_chgPwdAdm;
        } break;
        case USER_operator: break;
        case USER_MAX: break;
        case USER_factory: break;
    }

    return push(cuser, source, event);
}

//
bool TSecurityEvent::pushPwdReset(user_t cuser, userSrc_t source) {
    TSecurityEvent::event_t event = EVENT_resetPwd;

    return push(cuser, source, event);
}

//
bool TSecurityEvent::pushPwdWrong(user_t cuser, userSrc_t source, user_t puser) {
    TSecurityEvent::event_t event = EVENT_MAX;

    switch(puser) {
        case USER_engineer: {
            event = EVENT_wrgPwdEng;
        } break;
        case USER_admin: {
            event = EVENT_wrgPwdAdm;
        } break;
        case USER_operator: break;
        case USER_MAX: break;
        case USER_factory: break;
    }

    return push(cuser, source, event);
}

//
bool TSecurityEvent::pushUserChanged(user_t cuser, userSrc_t source, user_t nuser) {
    TSecurityEvent::event_t event = EVENT_MAX;

    switch(nuser) {
        case USER_operator: {
            event = TSecurityEvent::EVENT_setOperator;
        } break;
        case USER_admin: {
            event = TSecurityEvent::EVENT_setAdmin;
        } break;
        case USER_engineer: {
            event = TSecurityEvent::EVENT_setEngineer;
        } break;
        case USER_MAX: break;
        case USER_factory: break;
    }

    return push(cuser, source, event);
}


//
bool TSecurityEvent::pushUserChangeAuto(user_t cuser, userSrc_t source) {
    TSecurityEvent::event_t event = EVENT_autoEnd;

    return push(cuser, source, event);
}


//
bool TSecurityEvent::push(user_t user, userSrc_t source, event_t event) {
    bool ispush = true;

    ispush &= (user <= USER_MAX);
    ispush &= (source <= USER_SOURCE_MAX);
    ispush &= (event < EVENT_MAX);

    if (ispush) {
        eventMsg_t msg = {user, source, event};
        ispush = this->eventMsg.push(msg);
    } else {
        Q_ASSERT(ispush);
    }

    return ispush;
}
