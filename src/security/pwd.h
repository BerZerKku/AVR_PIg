#ifndef TPWD_H
#define TPWD_H

#include "glbDefine.h"
#include "user.h"

/// Пароль.
class TPwd {
    /// время сборса ошибки ввода пароля
#ifdef NDEBUG
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE)
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

#endif // TPWD_H
