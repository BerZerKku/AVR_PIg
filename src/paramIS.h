/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "src/glbDefine.h"
#include <cstdint>
#include <climits>

class TPwd {
public:
    static const uint32_t max = 100000000;

    TPwd() {
        pwd_ = max;
    }

    /** Устанавливает новое значение пароля.
     *
     *  @return false в случае ошибочного значения.
     */
    bool set(uint32_t val) {
        bool check = false;

        if (val < max) {
            pwd_ = val;
            check = true;
        }

        return check;
    }

    /** Возвращает значение пароля.
     *
     *  @return Пароль.
     */
    uint32_t get() const {
        return pwd_;
    }

private:
    uint32_t pwd_;
};

class TUser {
public:

    // пользователь
    enum USER {
        MIN = 0,        //
        OPERATOR = 0,   // Оператор
        ENGINEER,       // Инженер
        ADMIN,          // Адинистратор
        MAX
    };

    TUser() {
        user_ = OPERATOR;
    }

    /**	Запись.
     *
     * 	@param val Пользователь.
     * 	@return False в случае ошибочного значения.
     */
    bool set(USER val) {
        if ((val >= MIN) && (val < MAX)) {
            user_ = val;
        }
        return (user_ == val);
    }

    /**	Чтение.
     *
     * 	@return Пользователь.
     */
    USER get() const {
        return user_;
    }

private:
    USER user_;
};

class TIsEvent {
public:
    // события для журнала ИБ
    enum IS_EVENT_BIT {
        PWD_CHANGE = 0, // пароль изменен
        PWD_IN_ERROR,   // пароль введен неверно
        PWD_BLOCK,      // блокировка ввода пароля
        USR_IN,         // вход пользователя
        USR_OUT,        // выход пользователя
        USR_OUT_AUTO,   // выход пользователя автоматический
        //
        IS_EVENT_BIT_MAX
    };

    TIsEvent() {
        COMPILE_TIME_ASSERT((sizeof(event_) * CHAR_BIT) <= EVENT_BIT_MAX);
        event_ = 0;
    }

    /** Установка флага события.
     *
     *  Флаг добавляется к текущему значению событий.
     *
     * @param event Событие.
     * @return Флаги событий.
     */
    uint8_t set(IS_EVENT_BIT event) {
        if (event < IS_EVENT_BIT_MAX) {
            event_ |= (1 << event);
        }
        return event_;
    }

    /** Возвращает текущее состояние флагов событий.
     *
     *  После считывания флаги очищаются.
     *
     *  @return Флаги событий.
     */
    uint8_t get() {
        uint8_t tevent = event_;
        event_ = 0;
        return tevent;
    }

private:
    uint8_t event_;
};

/// структура параметров работы с информационной безопасностью
class TInfoSecurity {
public:
    TInfoSecurity() {
        User.set(TUser::OPERATOR);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TPwd pwdAdmin;
    TPwd pwdEngineer;

    TUser User;    
};

#endif /* PARAMIS_H_ */
