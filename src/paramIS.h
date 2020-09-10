/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

class TUser {
public:

    // пользователь
    enum USER {
        MIN = 0,        //
        OPERATOR = 0,   // Оператор
        ENGINEER,       // Инженер
        ADMIN,          // Адинистратор
        MAX
    };

    TUser() {
        user_ = OPERATOR;
    }

    /**	Запись.
     * 	@param val Пользователь.
     * 	@return False в случае ошибочного значения.
     */
    bool set(USER val) {
        if ((val >= MIN) && (val < MAX)) {
            user_ = val;
        }
        return (user_ == val);
    }

    /**	Чтение.
     * 	@return Пользователь.
     */
    USER get() const {
        return user_;
    }

private:
    USER user_;
};

/// структура параметров работы с информационной безопасностью
class TInfoSecurity {
public:
    TInfoSecurity() {
        User.set(TUser::OPERATOR);
    }

    TUser User;
};

#endif /* PARAMIS_H_ */
