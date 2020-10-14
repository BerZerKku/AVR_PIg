#ifndef USER_H
#define USER_H

#include "glbDefine.h"
#include "securityevent.h"

class TUser {

// Время до сброса пользователя в секундах
#ifdef NDEBUG
#define kTimeToResetPi 1800
#define kTimeToResetPc 900
#else
#define kTimeToResetPi 300
#define kTimeToResetPc 150
#endif

    /// Структура пользователя для источника доступа.
    typedef struct {
        user_t user;    ///< Пользователь.
        uint16_t time;  ///< Таймер.
    } stUser_t;

public:

    TUser();

    /**	Устанавливает пользователя.
     *
     *  @param[in] src Источник доступа.
     * 	@param val Пользователь.
     * 	@return true если значение установлено, иначе false.
     */
    bool set(userSrc_t src, user_t val);

    /**	Чтение.
     *
     *  @param[in] src Источник доступа.
     * 	@return Пользователь.
     */
    user_t get(userSrc_t src) const;

    /// Тик таймера.
    void tick();

    /** Сброс таймера.
     *
     *  @param[in] src Источник доступа.
     */
    void resetTimer(userSrc_t src);

    /** Возвращает текущее значение таймера.
     *
     *  @param[in] src Источник доступа.
     */
    uint16_t getTimer(userSrc_t src) const;

    /// Сброс всех пользователей.
    void reset();

    /** Сброс роли на оператора.
     *
     *  @param[in] src Источник доступа.
     */
    void reset(userSrc_t src);

    /** Проверяет возможность доступа текущей роли к указанной.
     *
     *  @param[in] src Источник доступа.
     *  @param[in] chuser Необходимый пользователь.
     *  @return True если можно менять.
     */
    bool checkAccess(userSrc_t src, user_t chuser) const;

    /** Устанавливает указатель на очередь событий.
     *
     *  @param[in] sevent Очередь событий.
     */
    void setInfoSecurity(TSecurityEvent *sevent) {
        this->sevent = sevent;
    }

private:
    /** Тик таймера.
     *
     *  Проверяется время до сброса текущей роли.
     *
     *  @param[in] src Источник доступа.
     */
    void tick(userSrc_t src);

    /// Структуры пользователей.
    stUser_t stuser[USER_SOURCE_MAX];

    /// Очередь событий.
    TSecurityEvent *sevent;
};

#endif // USER_H
