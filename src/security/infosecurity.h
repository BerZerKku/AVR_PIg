#ifndef TINFOSECURITY_H
#define TINFOSECURITY_H

#include "glbDefine.h"
#include "pwd.h"
#include "securityevent.h"

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

    /// Конструктор.
    TInfoSecurity();

    TPwd pwd;
    TUser usr;
    TSecurityEvent sevent;

    /** Устанавливает пользователя для работы с ПК.
     *
     *  @param[in] user Пользователь.
     *  @param[in] p Пароль
     *  @return Результат установки.
     *  @retval STATE_OK
     *  @retval STATE_NO_ACCESS
     *  @retval STATE_WRONG_PWD
     */
    state_t setUserPc(user_t user, const uint8_t *p=NULL);

    /** Изменяет пароль пользователя с ПК.
     *
     *  @param[in] user Пользователь для которого меняется пароль.
     *  @param[in] cp Пароль текущего пользователя.
     *  @param[in] np Новый пароль пользователя.
     *  @return Результат изменения пароля.
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

    /** Проверяет разрешения передачи команды в БСП.
     *
     *  Дополнительно проверяется флаг активности команды и при необходимости
     *  происходит сброс таймера до блокировки текущего пользователя.
     *
     *  @param[in] com Команда.
     *  @return Результат проверки.
     *  @retval STATE_OK
     *  @retval STATE_NO_ACCESS
     */
    state_t isComAccess(eGB_COM com);
};

#endif // TINFOSECURITY_H
