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
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (10000UL / MENU_TIME_CYLCE)
#endif
public:
    // Конструктор.
    TPwd();

    /** Устанавливает новое значение пароля.
     *
     *  @param[in] pwd Пароль.
     *  @return false в случае ошибочного значения.
     */
    bool set(uint8_t *pwd);

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
    bool setCounter(uint8_t value);

    /// Сброс счетчика.
    void clrCounter();

    /** Увеличивает счетчик ошибок ввода пароля.
     *
     *  Если это первый ошибочный ввод таймер будет сброшен.
     */
    void incCounter();

    /** Возвращает значение счетчика ввода неверного пароля.
     *
     *  @return Значение  счетчика.
     */
    uint8_t getCounter() const {
        return tcounter_;
    }

    /** Сброс настроек.
     *
     *  Настройки сбрасываются при обрыве связи с БСП.
     *
     *  @param[in] value Состояние инициализации.
     */
    void reset();

    /** Тик таймера.
     *
     *  Считает время до сброса ошибки счетчика.
     *  Проверяет изменение счетичка.
     *
     *  @return true если значение счетчика было изменено.
     */
    bool tick();

    /// Возвращает время до декремента счетчика ошибок.
    uint16_t getTicksToDecrement() const {
        return time_;
    }

    /// Возвращает текущее состояние блокировки выбора роли.
    bool isLock() const {
        return tcounter_ >= PWD_CNT_BLOCK;
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
    bool checkValue(uint8_t *pwd) const;

    /** Устанавливает значение пароля.
     *
     *  @param[in] pwd Пароль.
     */
    void setValue(uint8_t *pwd);

    bool init_;             ///< Инициализация (первое считывание).
    uint8_t pwd_[PWD_LEN];  ///< Пароль.
    uint8_t counter_;       ///< Счетчик ввода неверного пароля из БСП.
    uint8_t tcounter_;      ///< Счетчик ввода неверного пароля в БСП-ПИ.
    uint16_t time_;         ///< Счетчик тиков до уменьшения счетчика ошибок.
};

class TUser {

#ifdef NDEBUG
#define kTimeToReset (900000UL / MENU_TIME_CYLCE)
#else
#define kTimeToReset (300000UL / MENU_TIME_CYLCE)
#endif
public:

    // пользователь
    enum user_t {
        MIN = 0,        //
        OPERATOR = 0,   // Оператор
        ENGINEER,       // Инженер
        ADMIN,          // Адинистратор
        MAX
    };

    TUser();

    /**	Запись.
     *
     * 	@param val Пользователь.
     * 	@return False в случае ошибочного значения.
     */
    bool set(user_t val);

    /**	Чтение.
     *
     * 	@return Пользователь.
     */
    user_t get() const {
        return user_;
    }

    /** Тик таймера.
     *
     *  Проверяется время до сброса текущей роли.
     */
    void tick();

    /// Сброс таймера.
    void resetTimer();

    /// Возвращает текущее значение таймера.
    uint16_t getTimer() {
        return time_;
    }

    /// Сброс роли на оператора.
    void reset();

private:
    user_t user_;
    uint16_t time_;
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
        UserPi.set(TUser::OPERATOR);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TUser UserPi;
    TUser UserPc;

    TPwd pwdAdmin;
    TPwd pwdEngineer;
};

#endif /* PARAMIS_H_ */
