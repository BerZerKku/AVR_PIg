/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"
#include "param.h"
#include "paramIS.h"

// TODO Сделать описание работы паролей и по возможности упростить алгоритм/изменение.

class TUser {

#ifdef NDEBUG
#define kTimeToReset (900000UL / MENU_TIME_CYLCE)
#else
#define kTimeToReset (300000UL / MENU_TIME_CYLCE)
#endif
public:

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

    /** Проверяет пользователя на возможность изменения параметра.
     *
     *  @param[in] Текущий пользователь.
     *  @param[in] chuser Необходимый пользователь.
     *  @return True если можно менять.
     */
    bool checkChangeUser(user_t chuser) const;

private:
    user_t user_;
    uint16_t time_;
};

/// Пароль.
class TPwd {
    /// время сборса ошибки ввода пароля
#ifdef NDEBUG
#define kkTickWaiter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (30000UL / MENU_TIME_CYLCE)
#endif
#define kTickWait (1000UL / MENU_TIME_CYLCE)
#define kTickToRestPwd (5000UL / MENU_TIME_CYLCE)
    /// Состояния сброса паролей в значение по умолчанию.
    enum resetState_t {
        RESET_STATE_no = 0,         ///< Нет.
        RESET_STATE_waitDisable,    ///< Ожидание перехода в Выведен.
        RESET_STATE_disable,        ///< Смена режима на Выведен.
        RESET_STATE_waitDisableTime,///< Ожидание времени в режиме Выведен.
        RESET_STATE_resetPassword,  ///< Сброс паролей и счетчиков.
        RESET_STATE_waitReset,      ///< Ожидание сброса паролей и счетчиков.
        RESET_STATE_enable,         ///< Смена режима на Введен.
        RESET_STATE_waitEnable,     ///< Ожидание режима Введен.
        //
        RESET_STATE_MAX
    };

    struct pwd_t {
        bool init;             ///< Инициализация (первое считывание).
        uint8_t pwd[PWD_LEN];  ///< Пароль.
        uint8_t counter;       ///< Счетчик ввода неверного пароля.
        uint8_t tcounter;      ///< Счетчик ввода неверного пароля из БСП.
        uint16_t ticks;        ///< Счетчик тиков до уменьшения счетчика ошибок.
        bool changed;          ///< Флаг ввода нового значения.
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
     *  Если пароль верен, счетчик ошибок неверных вводов сбрасывается.
     *  Если пароль не верен, счетчик ошибок неверных вводов увеличивается.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return true если пароль верен, иначе false.
     */
    bool checkPassword(user_t user, const uint8_t *pwd);

    /** Возвращает значение пароля.
     *
     *  @param[in] user Пользователь.
     *  @return Пароль.
     *  @return NULL в случае отсутствия пароля.
     */
    uint8_t* getPwd(user_t user);

    /** Устанавливает значение пароля считанное из БСП.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return true если пароль установлен, иначе false.
     */
    bool setPwd(user_t user, const uint8_t *pwd);

    /** Изменяет пароль пользователя.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return true если пароль изменен, иначе false.
     */
    bool changePwd(user_t user, const uint8_t *pwd);

    /** Изменяет пароль пользователя.
     *
     *  @param[in] param Параметр пароля пользователя.
     *  @param[in] pwd Пароль.
     *  @return true если пароль изменен, иначе false.
     */
    bool changePwd(eGB_PARAM param, const uint8_t *pwd);

    /** Устанавливает счетчик ввода неверного пароля.
     *
     *  Если установленное значение больше текущего то будет "обнулено"
     *  время до декремента текущего счетчика.
     *
     *  @param[in] user Пользователь.
     *  @param[in] value Значение.
     */
    void setCounter(user_t user, uint8_t value);

    /** Возвращает значение счетчика ввода неверного пароля.
     *
     *  Для TUser::OPERATOR всегда возвращается 0.
     *  Для TUser::MAX или ошибочном значении возвращается 0xFF.
     *
     *  @param[in] user Пользователь
     *  @return Значение  счетчика.
     */
    uint8_t getCounter(user_t user) const;

    /** Возвращает время до окончания блокировки в секундах.
     *
     *  Для TUser::OPERATOR всегда возвращается 0.
     *  Для TUser::MAX или ошибочном значении возвращается 9999.
     *
     *  @param[in] user Пользователь.
     *  @return Время до блокировки.
     */
    uint16_t getLockTime(user_t user) const;

    /// Сброс настроек.
    void reset();

    /** Сброс настроек.
     *
     *  Настройки сбрасываются при обрыве связи с БСП или ПК.
     *
     *  @param[in] user Пользователь
     */
    void reset(user_t user);

    /** Тик таймера.
     *
     *  Считает время до сброса ошибки счетчика.
     *  Проверяет изменение счетчика.
     *
     *  @param[in] user Пользователь
     *  @return true если значение счетчика было изменено.
     */
    bool tick(user_t user);

    /** Проверяет состояние флага инициализации.
     *
     *  Для TUser::OPERATOR всегда возвращается true.
     *  Для TUser::MAX или ошибочном значении возвращается false.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @return true если значение было установлено, иначе false.
     */
    bool isInit(user_t user) const;

    /** Проверяет текущее состояние блокировки выбора роли.
     *
     *  Для TUser::OPERATOR всегда возвращается false.
     *  Для TUser::MAX или ошибочном значении возвращается true.
     *
     *  @param[in] user Пользователь
     */
    bool isLocked(user_t user) const;

    /** Проверяет флаг изменения пароля.
     *
     *  @param[in] user Пользователь.
     *  @return
     */
    bool isChangedPwd(user_t user) const;

    /** Возвращает параметр для счетчика ошибок
     *
     *  @param[in] user Пользователь.
     *  @return Параметр.
     *  @retval GB_PARAM_NO если у пользователя нет параметра.
     */
    eGB_PARAM getCounterParam(user_t user) const;

    /** Возвращает параметр для пароля.
     *
     *  @param[in] user Пользователь.
     *  @return Параметр.
     *  @retval GB_PARAM_NO если у пользователя нет параметра.
     */
    eGB_PARAM getPwdParam(user_t user) const;

    /// Сбросить пароли в значение по умолчанию.
    void resetPwdToDefault();

    /** Сбрасывает пароли в значение по умолчанию.
     *
     *  @param regime Текущий режим работы устройства.
     */
    void resetPwdToDefaultCycle(eGB_REGIME regime);

    /// Проверяет необходимость сбросить пароли в значение по умолчанию.
    bool isResetToDefault();

    /// Проверка необходимости вывести аппарат из работы.
    bool isWaitDisableDevice();

    /// Проверка необходимости сбросить пароли в значение по умолчанию.
    bool isWaitResetPassword();

    /// Проверка необходимости ввести аппарат в работу.
    bool isWaitEnableDevice();

private:
    /// Состояние сброса паролей к значению по умолчанию.
    resetState_t resetState;

    pwd_t password[USER_MAX-USER_operator-1];

    /** Проверяет флаг изменения пароля.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @return
     */
    bool isChangedPwd(int8_t index) const;

    /** Возвращает состояние флага инициализации.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @return true если значение было установлено, иначе false.
     */
    bool isInit(int8_t index) const;

    /** Изменяет пароль пользователя.
     *
     *  @param index Индекс пользователя в массиве.
     *  @param[in] pwd Пароль.
     *  @return true если пароль изменен, иначе false.
     */
    bool changePwd(int8_t index, const uint8_t *pwd);

    /** Проверяет корректность символов пароля.
     *
     *  @param[in] user Пользователь.
     *  @param[in] pwd Пароль.
     *  @return Результат проверки, true если все в порядке.
     */
    bool checkValue(const uint8_t *password) const;

    /** Увеличивает счетчик ввода неверного пароля.
     *
     *  Если это первый ошибочный ввод таймер будет сброшен.
     *
     *  @param[in] Индекс пользователя в массиве.
     */
    void incCounter(int8_t index);

    /** Сбрасывает счетчик ввода неверного пароля.
     *
     *  @param[in] Индекс пользователя в массиве.
     */
    void clrCounter(int8_t index);

    /** Возвращает индекс пользователя в массиве.
     *
     *  @param[in] user Пользователь
     *  @return Индекс пользователя в массиве.
     *  @retval -1, если пользователя нет.
     */
    int8_t getUserIndex(user_t user) const;

    /** Устанавливает количество тиков таймера до сброса блокировки роли.
     *
     *  @param[in] index Индекс пользователя в массиве.
     *  @param[in] enable true установить счетчик тиков, иначе обнулить.
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
    enum state_t {
        STATE_OK = 0x00,            // ОК
        STATE_NO_ACCESS = 0x01,     // Нет доступа
        STATE_WRONG_PWD = 0x02,     // Неверный пароль
        STATE_WRONG_NEW_PWD = 0x03, // Неверный новый пароль
        //
        STATE_MAX
    };

    TInfoSecurity() {
        UserPi.set(USER_operator);
        UserPc.set(USER_operator);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TUser UserPi;
    TUser UserPc;

    TPwd pwd;

    /** Устанавливает пользователя для работы с ПК.
     *
     *  @param[in] user Пользователь.
     *  @param[in] p Пароль
     *  @return Результат установки.
     *  @retval STATE_NO_ACCESS
     *  @retval STATE_WRONG_PWD
     */
    state_t setUserPc(user_t user, const uint8_t *p=NULL);

    /** Изменяет пароль пользвоателя с ПК.
     *
     *  @param[in] user Пользователь для которого меняется пароль.
     *  @param[in] cp Пароль текущего пользователя.
     *  @param[in] np Новый пароль пользователя.
     *  @return
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

};

#endif /* PARAMIS_H_ */
