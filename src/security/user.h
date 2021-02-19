#ifndef USER_H
#define USER_H

#include "src/glbDefine.h"

class TUser {
// Время до сброса пользователя в секундах
#ifdef NDEBUG
#define kTimeToReset 900
#else
#define kTimeToReset 150
#endif

    /// Структура пользователя для источника доступа.
    typedef struct {
        user_t user;    ///< Пользователь.
        uint16_t time;  ///< Таймер.
        user_t olduser; ///< Предыдущее значение пользователя.
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
     *  @param[in] src Источник доступа пользователя.
     * 	@return Пользователь.
     */
    user_t get(userSrc_t src) const;

    /// Тик таймера.
    void tick();

    /** Проверяет наличие автоматического завершения сеанса работы.
     *
     *  @param[in] src Источник доступа пользователя.
     *  @param[out] olduser Предыдущее значение пользователя.
     *  @return true если пользователь был сброшен, иначе false.
     */
    bool isAutoReset(userSrc_t src, user_t &olduser);

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
};

#endif // USER_H
