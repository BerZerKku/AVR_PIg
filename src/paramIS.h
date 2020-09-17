/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "src/glbDefine.h"
#include <cstdint>
#include <climits>

class TPwd {
public:
    static const uint32_t max = 100000000;

    TPwd() {
        pwd_ = max;
    }

    /** ������������� ����� �������� ������.
     *
     *  @return false � ������ ���������� ��������.
     */
    bool set(uint32_t val) {
        bool check = false;

        if (val < max) {
            pwd_ = val;
            check = true;
        }

        return check;
    }

    /** ���������� �������� ������.
     *
     *  @return ������.
     */
    uint32_t get() const {
        return pwd_;
    }

private:
    uint32_t pwd_;
};

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
     *
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
     *
     * 	@return ������������.
     */
    USER get() const {
        return user_;
    }

private:
    USER user_;
};

class TIsEvent {
public:
    // ������� ��� ������� ��
    enum IS_EVENT_BIT {
        PWD_CHANGE = 0, // ������ �������
        PWD_IN_ERROR,   // ������ ������ �������
        PWD_BLOCK,      // ���������� ����� ������
        USR_IN,         // ���� ������������
        USR_OUT,        // ����� ������������
        USR_OUT_AUTO,   // ����� ������������ ��������������
        //
        IS_EVENT_BIT_MAX
    };

    TIsEvent() {
        COMPILE_TIME_ASSERT((sizeof(event_) * CHAR_BIT) <= EVENT_BIT_MAX);
        event_ = 0;
    }

    /** ��������� ����� �������.
     *
     *  ���� ����������� � �������� �������� �������.
     *
     * @param event �������.
     * @return ����� �������.
     */
    uint8_t set(IS_EVENT_BIT event) {
        if (event < IS_EVENT_BIT_MAX) {
            event_ |= (1 << event);
        }
        return event_;
    }

    /** ���������� ������� ��������� ������ �������.
     *
     *  ����� ���������� ����� ���������.
     *
     *  @return ����� �������.
     */
    uint8_t get() {
        uint8_t tevent = event_;
        event_ = 0;
        return tevent;
    }

private:
    uint8_t event_;
};

/// ��������� ���������� ������ � �������������� �������������
class TInfoSecurity {
public:
    TInfoSecurity() {
        User.set(TUser::OPERATOR);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TPwd pwdAdmin;
    TPwd pwdEngineer;

    TUser User;    
};

#endif /* PARAMIS_H_ */
