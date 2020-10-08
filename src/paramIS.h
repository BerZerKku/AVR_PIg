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

/// Пароль.
class TPwd {
    /// время сборса ошибки ввода пароля
#ifdef NDEBUG
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (30000UL / MENU_TIME_CYLCE)
#endif

    struct pwd_t {
        bool init;             ///< Инициализация (первое считывание).
        uint8_t pwd[PWD_LEN];  ///< Пароль.
        uint8_t counter;       ///< Счетчик ввода неверного пароля.
        uint8_t tcounter;      ///< Счетчик ввода неверного пароля из БСП.
        uint16_t ticks;        ///< Счетчик тиков до уменьшения счетчика ошибок.
    };

public:
    // Конструктор.
    TPwd();

    /** Возвращает результат проверки пароля для пользователя.
     *
     *  Для TUser::OPERATOR всегда возвращается true.
     *  Для TUser::MAX или ошибочном значении возвращается false.
     *  Для заблокированного пользователя возвращается false.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return true если пароль верен, иначе false.
     */
    bool checkPassword(TUser::user_t user, const uint8_t *pwd);

    /** Возвращает значение пароля.
     *
     *  @param[in] user Пользователь.
     *  @return Пароль.
     *  @return NULL в случае отсутствия пароля.
     */
    uint8_t* getPwd(TUser::user_t user);

    /** Устанавливает новое значение пароля.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return true если пароль установлен, иначе false.
     */
    bool setPwd(TUser::user_t user, uint8_t *password);

    /** Устанавливает счетчик ввода неверного пароля.
     *
     *  Если установленное значение больше текущего то будет "обнулено"
     *  время до декремента текущего счетчика.
     *
     *  @param[in] user Пользователь.
     *  @param[in] value Значение.
     */
    void setCounter(TUser::user_t user, uint8_t value);

    /** Сброс счетчика.
     *
     *  @param[in] user Пользователь.
     */
    void clrCounter(TUser::user_t user);

    /** Увеличивает счетчик ошибок ввода пароля.
     *
     *  Если это первый ошибочный ввод таймер будет сброшен.
     *
     *  @param[in] user Пользователь
     */
    void incCounter(TUser::user_t user);

    /** Возвращает значение счетчика ввода неверного пароля.
     *
     *  Для TUser::OPERATOR всегда возвращается 0.
     *  Для TUser::MAX или ошибочном значении возвращается 0xFF.
     *
     *  @param[in] user Пользователь
     *  @return Значение  счетчика.
     */
    uint8_t getCounter(TUser::user_t user) const;

    /** Возвращает время до окончания блокировки в секундах.
     *
     *  Для TUser::OPERATOR всегда возвращается 0.
     *  Для TUser::MAX или ошибочном значении возвращается 9999.
     *
     *  @param[in] user Пользователь.
     *  @return Время до блокировки.
     */
    uint16_t getLockTime(TUser::user_t user) const;

    /** Сброс настроек.
     *
     *  Настройки сбрасываются при обрыве связи с БСП.
     *
     *  @param[in] user Пользователь
     */
    void reset(TUser::user_t user);

    /** Тик таймера.
     *
     *  Считает время до сброса ошибки счетчика.
     *  Проверяет изменение счетчика.
     *
     *  @param[in] user Пользователь
     *  @return true если значение счетчика было изменено.
     */
    bool tick(TUser::user_t user);

    /** Возвращает состояние флага инициализации.
     *
     *  Для TUser::OPERATOR всегда возвращается true.
     *  Для TUser::MAX или ошибочном значении возвращается false.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @return true если значение было установлено, иначе false.
     */
    bool isInit(TUser::user_t user) const;

    /** Возвращает текущее состояние блокировки выбора роли.
     *
     *  Для TUser::OPERATOR всегда возвращается false.
     *  Для TUser::MAX или ошибочном значении возвращается true.
     *
     *  @param[in] user Пользователь
     */
    bool isLocked(TUser::user_t user) const;

private:
    pwd_t password[TUser::MAX-TUser::OPERATOR];

    /** Возвращает состояние флага инициализации.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @return true если значение было установлено, иначе false.
     */
    bool isInit(int8_t index) const;

    /** Проверяет корректность символов пароля.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return Результат проверки, true если все в порядке.
     */
    bool checkValue(uint8_t *password) const;

    /** Возвращает индекс пользователя в массиве.
     *
     *  @param[in] user Пользователь
     *  @return Индекс пользователя в массиве.
     *  @retval -1, если пользователя нет.
     */
    int8_t getUserIndex(TUser::user_t user) const;

    /** Устанавливает количество тиков таймера до сброса блокировки роли.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @param[in] cnt Количество интервалов времени до сброса блокировки.
     */
    void setTicks(int8_t index, bool enable);
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

    TPwd pwd;
};

#endif /* PARAMIS_H_ */
