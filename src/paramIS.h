/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"

/// Пароль.
class TPwd {
    /// время сборса ошибки ввода пароля
//    static const uint16_t kTickToDecCounter = (600000UL / MENU_TIME_CYLCE);
    static const uint16_t kTickToDecCounter = (10000UL / MENU_TIME_CYLCE);

public:

    // Конструктор.
    TPwd() {
        COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == PWD_LEN);

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = 0;
        }
        counter_ = PWD_CNT_BLOCK;
        time_ = kTickToDecCounter;
    }

    /** Устанавливает новое значение пароля.
     *
     *  @param[in] pwd Пароль.
     *  @return false в случае ошибочного значения.
     */
    bool set(uint8_t *pwd) {
        bool check = checkValue(pwd);

        if (check) {
            setValue(pwd);
        }

        return check;
    }

    /** Возвращает значение пароля.
     *
     *  @return Пароль.
     */
    uint8_t* get() {
        return pwd_;
    }

    /** Устанавливает счетчик ввода неверного пароля.
     *
     *  Если установленное значение больше текущего то будет "обнулено"
     *  время до декремента текущего счетчика.
     *
     *  @param[in] value Значение.
     */
    bool setCounter(uint8_t value) {
        uint8_t tcounter= counter_;
        counter_ = (value <= PWD_CNT_BLOCK) ? value : PWD_CNT_BLOCK;
        if (tcounter < counter_) {
            time_ = kTickToDecCounter;
        }

        return (value == counter_);
    }

    /// Увеличивает счетчик ошибок ввода пароля.
    void incCounter() {
        if (counter_ < PWD_CNT_BLOCK) {
            counter_++;
        }

        time_ = kTickToDecCounter;
    }

    /** Возвращает значение счетчика ввода неверного пароля.
     *
     *  @return Значение  счетчика.
     */
    uint8_t getCounter() const {
        return counter_;
    }

    /** Тик таймера сброса счетчика ввода неверного пароля.
     *
     *  @return true если значение было изменено.
     */
    bool timerTick() {
        bool change = false;
        time_ = (time_ > 0) ? time_ - 1 : kTickToDecCounter;
        if ((time_ == 0) && (counter_ > 0)) {
            counter_--;
            change = true;
        }

        return change;
    }

private:

    /** Проверяет корректность пароля.
     *
     *  @param[in] pwd Пароль.
     *  @return Результат проверки, true если все в порядке.
     */
    bool checkValue(uint8_t *pwd) const {
        bool check = true;

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
        }

        return check;
    }

    /** Устанавливает значение пароля.
     *
     *  @param[in] pwd Пароль.
     */
    void setValue(uint8_t *pwd) {
        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = pwd[i];
        }
    }

    uint8_t pwd_[PWD_LEN];
    uint8_t counter_;
    uint16_t time_;
};

class TUser {
public:

    // пользователь
    enum user_t {
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
    bool set(user_t val) {
        if ((val >= MIN) && (val < MAX)) {
            user_ = val;
        }
        return (user_ == val);
    }

    /**	Чтение.
     *
     * 	@return Пользователь.
     */
    user_t get() const {
        return user_;
    }

private:
    user_t user_;
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
        COMPILE_TIME_ASSERT((sizeof(event_) * CHAR_BIT) >= IS_EVENT_BIT_MAX);
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
