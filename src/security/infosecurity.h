#ifndef TINFOSECURITY_H
#define TINFOSECURITY_H

#include "glbDefine.h"
#include "user.h"
#include "pwd.h"
#include "isevent.h"

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

#endif // TINFOSECURITY_H
