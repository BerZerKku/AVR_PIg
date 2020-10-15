#ifndef TSECURITY_EVENT_H
#define TSECURITY_EVENT_H

#include "glbDefine.h"
#include "fifo.h"

class TSecurityEvent {
public:
    /// Cобытия для журнала ИБ.
    enum event_t {
        EVENT_autoEnd = 0,  // Автоматическое завершение сеанса работы
        EVENT_chgSettings,  // (БСП) Изменение настроек
        EVENT_chgTime,      // (БСП) Изменение даты или времени пользователем
        EVENT_chgRegime,    // (БСП) Изменение режима работы
        EVENT_setOperator,  // Изменение роли пользователя на Оператор
        EVENT_setEngineer,  // Изменение роли пользователя на Инженер
        EVENT_setAdmin,     // Изменение роли пользователя на Администратор
        EVENT_blkEngineer,  // Блокировка доступа к роли Инженер
        EVENT_blkAdmin,     // Блокировка доступа к роли Администратор
        EVENT_chgPwdEng,    // Изменение пароля для роли Инженер
        EVENT_chgPwdAdm,    // Изменение пароля для роли Администратор
        EVENT_wrgPwdEng,    // Ввод неверного пароля для роли Инженер
        EVENT_wrgPwdAdm,    // Ввод неверного пароля для роли Администратор
        EVENT_resetPwd,     // Сброс паролей до заводских настроек
        //
        EVENT_MAX
    };

    /// Конструктор.
    TSecurityEvent();

    /** Возвращает записанное событие из очереди.
     *
     *  @param[out] user Роль пользователя.
     *  @param[out] source Источник доступа пользователя.
     *  @param[out] event Событие.
     *  @return true если есть событие, иначе false.
     */
    bool pop(user_t &user, userSrc_t &source, event_t &event);

    /** Добавляет запись о блокировке пользователя.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] puser Заблокированный пользователь.
     *  @return true если событие добвлено, иначе false.
     */
    bool pushPwdBlocked(user_t cuser, userSrc_t source, user_t buser);

    /** Добавляет запись о смене пароля для пользователя.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] puser Пользователь для которого вводили пароль.
     *  @return true если событие добвлено, иначе false.
     */
    bool pushPwdChanged(user_t cuser, userSrc_t source, user_t puser);

    /** Добавляет запись о сбросе паролей до заводских настроек.
     *
     *  Сброс может быть только с ПК.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @return
     */
    bool pushPwdReset(user_t cuser, userSrc_t source);

    /** Добавляет запись о вводе неверного пароля для пользователя.
     *
     *  Если пользователь был заблокирован вместо записи о неверном пароле
     *  будет добавлена запись о блокировке.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] puser Пользователь для которого вводили пароль.
     *  @return true если событие добвлено, иначе false.
     */
    bool pushPwdWrong(user_t cuser, userSrc_t source, user_t puser);

    /** Добавляет запись о смене пользователя.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] nuser Новый пользователь.
     *  @return true если событие добвлено, иначе false.
     */
    bool pushUserChanged(user_t cuser, userSrc_t source, user_t nuser);

    /** Добавляет запись об автоматической смене пользователя.
     *
     *  @param[in] cuser Текущий пользователь.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] nuser Новый пользователь.
     *  @return true если событие добвлено, иначе false.
     */
    bool pushUserChangeAuto(user_t cuser, userSrc_t source);

    /// Проверка на отсутствие событий в очереди.
    bool isEmpty() const {
        return eventMsg.isEmpty();
    }

private:
    /// Структура сообщения для журнала ИБ.
    typedef struct {
        user_t user;        ///< Роль пользователя.
        userSrc_t source;    ///< Источник события.
        event_t event;      ///< Событие.
    } eventMsg_t;

    /// События.
    TFifo<IS_MSG_FIFO_SIZE, eventMsg_t> eventMsg;

    /** Добавляет запись события в очередь.
     *
     *  @param[in] user Роль пользователя.
     *  @param[in] source Источник доступа пользователя.
     *  @param[in] event Событие.
     *  @return true если событие добвлено, иначе false.
     */
    bool push(user_t user, userSrc_t source, event_t event);
};

#endif // TSECURITY_EVENT_H
