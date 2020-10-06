/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"

// TODO Сделать описание работы паролей и по возможности упростить алгоритм/изменение.

/// Пароль.
class TPwd {
    /// время сборса ошибки ввода пароля
#ifdef NDEBUG
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE);
#else
#define kTickToDecCounter (10000UL / MENU_TIME_CYLCE);
#endif
#define kMaskLock 0x04
public:
    // Конструктор.
    TPwd() {
        COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == PWD_LEN);
        COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);
        COMPILE_TIME_ASSERT(kMaskLock == 0x04);

        reset();
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
        counter_ = value & kMaskLock;
        value  &= ~kMaskLock;
        counter_ += (value <= PWD_CNT_BLOCK) ? value : PWD_CNT_BLOCK;

        if (!isInit()) {
            tcounter_ = counter_;
            time_ = kTickToDecCounter;
            init_ = true;
        }

        return (value == counter_);
    }

    /// Сброс счетчика.
    void clrCounter() {
        if (isInit()) {
            tcounter_ = 0;
        }
    }

    /// Увеличивает счетчик ошибок ввода пароля.
    void incCounter() {
        if (isInit()) {
            uint8_t value = tcounter_ & ~kMaskLock;
            tcounter_ &= ~kMaskLock;

            if (value < PWD_CNT_BLOCK) {
                value++;
            }

            if (value >= PWD_CNT_BLOCK) {
                value = PWD_CNT_BLOCK | kMaskLock;
            }

            tcounter_ |= value;
        }

        time_ = kTickToDecCounter;
    }

    /** Возвращает значение счетчика ввода неверного пароля.
     *
     *  @return Значение  счетчика.
     */
    uint8_t getCounter() const {
        return tcounter_;
    }

    /** Установка наличия инициализации счетчика.
     *
     *  @param[in] value Состояние инициализации.
     */
    void reset() {
        init_ = false;
        counter_ = PWD_CNT_BLOCK | kMaskLock;
        tcounter_ = counter_;
        time_ = kTickToDecCounter;

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = 0;
        }
    }

    /** Тик таймера.
     *
     *  Считает время до сброса ошибки счетчика.
     *  Проверяет изменение счетичка.
     *
     *  @return true если значение счетчика было изменено.
     */
    bool timerTick() {
        bool change = false;

        if (isInit()) {
            time_ = (time_ > 0) ? time_ - 1 : kTickToDecCounter;

            if (time_ == 0) {
                uint8_t value = tcounter_ & ~kMaskLock;
                tcounter_ &= kMaskLock;

                if (value > 0) {
                    value--;
                }

                tcounter_ = (value == 0) ? 0 : (tcounter_ | value);
            }

            if (tcounter_ != counter_) {
                change = true;
            }
        }

        return change;
    }

    /// Возвращает время до декремента счетчика ошибок.
    uint16_t getTicksToDecrement() const {
        return time_;
    }

    /// Возвращает текущее состояние блокировки выбора роли.
    bool isLock() const {
        return tcounter_ & kMaskLock;
    }

    /** Возвращает состояние флага инициализации.
     *
     *  @return true если значение было установлено, иначе false.
     */
    bool isInit() const {
        return init_;
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

    bool init_;             ///< Инициализация (первое считывание).
    uint8_t pwd_[PWD_LEN];  ///< Пароль.
    uint8_t counter_;       ///< Счетчик ввода неверного пароля из БСП.
    uint8_t tcounter_;      ///< Счетчик ввода неверного пароля в БСП-ПИ.
    uint16_t time_;         ///< Счетчик тиков до уменьшения счетчика ошибок.
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
        reset();
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

    /// Сброс роли на оператора.
    void reset() {
        user_ = OPERATOR;
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

    TUser User;

    TPwd pwdAdmin;
    TPwd pwdEngineer;
};

#endif /* PARAMIS_H_ */
