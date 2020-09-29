/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"

/// ������.
class TPwd {
    /// ����� ������ ������ ����� ������
//    static const uint16_t kTickToDecCounter = (600000UL / MENU_TIME_CYLCE);
    static const uint16_t kTickToDecCounter = (10000UL / MENU_TIME_CYLCE);

public:

    // �����������.
    TPwd() {
        COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == PWD_LEN);

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = 0;
        }
        counter_ = PWD_CNT_BLOCK;
        time_ = kTickToDecCounter;
    }

    /** ������������� ����� �������� ������.
     *
     *  @param[in] pwd ������.
     *  @return false � ������ ���������� ��������.
     */
    bool set(uint8_t *pwd) {
        bool check = checkValue(pwd);

        if (check) {
            setValue(pwd);
        }

        return check;
    }

    /** ���������� �������� ������.
     *
     *  @return ������.
     */
    uint8_t* get() {
        return pwd_;
    }

    /** ������������� ������� ����� ��������� ������.
     *
     *  ���� ������������� �������� ������ �������� �� ����� "��������"
     *  ����� �� ���������� �������� ��������.
     *
     *  @param[in] value ��������.
     */
    bool setCounter(uint8_t value) {
        uint8_t tcounter= counter_;
        counter_ = (value <= PWD_CNT_BLOCK) ? value : PWD_CNT_BLOCK;
        if (tcounter < counter_) {
            time_ = kTickToDecCounter;
        }

        return (value == counter_);
    }

    /// ����������� ������� ������ ����� ������.
    void incCounter() {
        if (counter_ < PWD_CNT_BLOCK) {
            counter_++;
        }

        time_ = kTickToDecCounter;
    }

    /** ���������� �������� �������� ����� ��������� ������.
     *
     *  @return ��������  ��������.
     */
    uint8_t getCounter() const {
        return counter_;
    }

    /** ��� ������� ������ �������� ����� ��������� ������.
     *
     *  @return true ���� �������� ���� ��������.
     */
    bool timerTick() {
        bool change = false;
        time_ = (time_ > 0) ? time_ - 1 : kTickToDecCounter;
        if ((time_ == 0) && (counter_ > 0)) {
            counter_--;
            change = true;
        }

        return change;
    }

private:

    /** ��������� ������������ ������.
     *
     *  @param[in] pwd ������.
     *  @return ��������� ��������, true ���� ��� � �������.
     */
    bool checkValue(uint8_t *pwd) const {
        bool check = true;

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
        }

        return check;
    }

    /** ������������� �������� ������.
     *
     *  @param[in] pwd ������.
     */
    void setValue(uint8_t *pwd) {
        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = pwd[i];
        }
    }

    uint8_t pwd_[PWD_LEN];
    uint8_t counter_;
    uint16_t time_;
};

class TUser {
public:

    // ������������
    enum user_t {
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
    bool set(user_t val) {
        if ((val >= MIN) && (val < MAX)) {
            user_ = val;
        }
        return (user_ == val);
    }

    /**	������.
     *
     * 	@return ������������.
     */
    user_t get() const {
        return user_;
    }

private:
    user_t user_;
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
        COMPILE_TIME_ASSERT((sizeof(event_) * CHAR_BIT) >= IS_EVENT_BIT_MAX);
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
