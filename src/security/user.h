#ifndef USER_H
#define USER_H

#include "glbDefine.h"

/// Пользователь (роль).
typedef enum {
    USER_operator = 0,  ///< Оператор. Должен быть в начале!
    USER_engineer,      ///< Инженер.
    USER_admin,         ///< Администратор.
    USER_MAX,           ///< Максимальное количество пользователей.
    USER_factory        ///< Производитель. Должен быть больше USER_MAX!
} user_t;

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

#endif // USER_H
