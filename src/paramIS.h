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

    // ������������
    enum USER {
        MIN = 0,        //
        OPERATOR = 0,   // ��������
        ENGINEER,       // �������
        ADMIN,          // ������������
        MAX
    };

    TUser() {
        user_ = OPERATOR;
    }

    /**	������.
     * 	@param val ������������.
     * 	@return False � ������ ���������� ��������.
     */
    bool set(USER val) {
        if ((val >= MIN) && (val < MAX)) {
            user_ = val;
        }
        return (user_ == val);
    }

    /**	������.
     * 	@return ������������.
     */
    USER get() const {
        return user_;
    }

private:
    USER user_;
};

/// ��������� ���������� ������ � �������������� �������������
class TInfoSecurity {
public:
    TInfoSecurity() {
        User.set(TUser::OPERATOR);
    }

    TUser User;
};

#endif /* PARAMIS_H_ */
