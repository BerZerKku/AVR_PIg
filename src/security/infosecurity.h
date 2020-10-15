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
    TSecurityEvent sevent;

    /** Тик.
     *
     *  @param[in] regime Режим работы устройства.
     *  @param[out] com Команда для отправки в БСП.
     */
    void tick(eGB_REGIME regime, eGB_COM &com);

    /** Устанавливает пользователя при работе с ПК.
     *
     *  @param[in] user Пользователь.
     *  @param[in] p Пароль
     *  @return Результат установки.
     *  @retval STATE_OK
     *  @retval STATE_NO_ACCESS
     *  @retval STATE_WRONG_PWD
     */
    state_t setUserPc(user_t user, const uint8_t *p=NULL);

    /** Возвращает пользователя для указанного источника доступа.
     *
     *  @param[in] src Источник доступа.
     */
    user_t getUser(userSrc_t src) const {
        return usr.get(src);
    }

    /** Возвращает время до автоматического завершения сеанса пользователя
     *
     *  ! Используется только для отладки.
     *
     *  @param[in] src Источник доступа.
     *  @return Время.
     */
    uint16_t getUserTime(userSrc_t src) const {
        return usr.getTimer(src);
    }

    /** Проверяет возможность доступа текущей роли к указанной.
     *
     *  @param[in] user Необходимый пользователь.
     *  @param[in] src Источник доступа.
     *  @return true если доступ есть, иначе false.
     */
    bool checkUserAccess(user_t user, userSrc_t src);

    /// Сброс пользователя.
    void rstUser(userSrc_t src) {
        usr.reset(src);
    }

    /** Сброс таймера активности пользователя.
     *
     *  @param[in] src Источник доступа.
     */
    void rstUserTimer(userSrc_t src) {
        usr.resetTimer(src);
    }

    /** Устанавливает пользователя.
     *
     *  @param[in] user Пользователь.
     *  @param[in] src Источник доступа.
     *  @return true если пользователь установлен, иначе false.
     */
    bool setUser(user_t user, userSrc_t src);

    /** Изменяет пароль пользователя с ПК.
     *
     *  @param[in] user Пользователь для которого меняется пароль.
     *  @param[in] cp Пароль текущего пользователя.
     *  @param[in] np Новый пароль пользователя.
     *  @return Результат изменения пароля.
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

    /** Изменяет пароль пользователя с ПИ.
     *
     *  @param[in] param Параметр пользователя.
     *  @param[in] np Новый пароль пользователя.
     *  @return true если пароль изменен, иначе false.
     */
    bool changeUserPiPwd(eGB_PARAM param, const uint8_t *np);

    /** Проверяет пароль пользователя.
     *
     *  В случае ошибки будет добавлено сообщение о вводе неправльного пароля
     *  или блокировке пользователя.
     *
     *  @param[in] user Пользователь для проверки пароля.
     *  @param[in] src Источник доступа.
     *  @param[in] cp Пароль  пользователя.
     *  @return true если верный пароль, иначе false.
     */
    bool checkUserPwd(user_t user, userSrc_t src, const uint8_t *cp);

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

    /** Возвращает параметр для пароля.
     *
     *  @param[in] user Пользователь.
     *  @return Параметр.
     *  @retval GB_PARAM_NO если у пользователя нет параметра.
     */
    eGB_PARAM getPwdParam(user_t user) const;

private:
    /// Пользователи.
    TUser usr;

    /** Изменяет пароль пользователя.
     *
     *  @param[in] user Пользователь для которого меняется пароль.
     *  @param[in] src Источник доступа.
     *  @param[in] np Новый пароль пользователя.
     *  @return true если пароль установлен, иначе false.
     */
    bool changeUserPwd(user_t user, userSrc_t src, const uint8_t *np);
};

#endif // TINFOSECURITY_H
