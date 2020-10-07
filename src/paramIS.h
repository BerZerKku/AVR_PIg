/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"

// TODO ������� �������� ������ ������� � �� ����������� ��������� ��������/���������.

/// ������.
class TPwd {
    /// ����� ������ ������ ����� ������
#ifdef NDEBUG
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (10000UL / MENU_TIME_CYLCE)
#endif
public:
    // �����������.
    TPwd();

    /** ������������� ����� �������� ������.
     *
     *  @param[in] pwd ������.
     *  @return false � ������ ���������� ��������.
     */
    bool set(uint8_t *pwd);

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
    bool setCounter(uint8_t value);

    /// ����� ��������.
    void clrCounter();

    /** ����������� ������� ������ ����� ������.
     *
     *  ���� ��� ������ ��������� ���� ������ ����� �������.
     */
    void incCounter();

    /** ���������� �������� �������� ����� ��������� ������.
     *
     *  @return ��������  ��������.
     */
    uint8_t getCounter() const {
        return tcounter_;
    }

    /** ����� ��������.
     *
     *  ��������� ������������ ��� ������ ����� � ���.
     *
     *  @param[in] value ��������� �������������.
     */
    void reset();

    /** ��� �������.
     *
     *  ������� ����� �� ������ ������ ��������.
     *  ��������� ��������� ��������.
     *
     *  @return true ���� �������� �������� ���� ��������.
     */
    bool tick();

    /// ���������� ����� �� ���������� �������� ������.
    uint16_t getTicksToDecrement() const {
        return time_;
    }

    /// ���������� ������� ��������� ���������� ������ ����.
    bool isLock() const {
        return tcounter_ >= PWD_CNT_BLOCK;
    }

    /** ���������� ��������� ����� �������������.
     *
     *  @return true ���� �������� ���� �����������, ����� false.
     */
    bool isInit() const {
        return init_;
    }

private:

    /** ��������� ������������ ������.
     *
     *  @param[in] pwd ������.
     *  @return ��������� ��������, true ���� ��� � �������.
     */
    bool checkValue(uint8_t *pwd) const;

    /** ������������� �������� ������.
     *
     *  @param[in] pwd ������.
     */
    void setValue(uint8_t *pwd);

    bool init_;             ///< ������������� (������ ����������).
    uint8_t pwd_[PWD_LEN];  ///< ������.
    uint8_t counter_;       ///< ������� ����� ��������� ������ �� ���.
    uint8_t tcounter_;      ///< ������� ����� ��������� ������ � ���-��.
    uint16_t time_;         ///< ������� ����� �� ���������� �������� ������.
};

class TUser {

#ifdef NDEBUG
#define kTimeToReset (900000UL / MENU_TIME_CYLCE)
#else
#define kTimeToReset (300000UL / MENU_TIME_CYLCE)
#endif
public:

    // ������������
    enum user_t {
        MIN = 0,        //
        OPERATOR = 0,   // ��������
        ENGINEER,       // �������
        ADMIN,          // ������������
        MAX
    };

    TUser();

    /**	������.
     *
     * 	@param val ������������.
     * 	@return False � ������ ���������� ��������.
     */
    bool set(user_t val);

    /**	������.
     *
     * 	@return ������������.
     */
    user_t get() const {
        return user_;
    }

    /** ��� �������.
     *
     *  ����������� ����� �� ������ ������� ����.
     */
    void tick();

    /// ����� �������.
    void resetTimer();

    /// ���������� ������� �������� �������.
    uint16_t getTimer() {
        return time_;
    }

    /// ����� ���� �� ���������.
    void reset();

private:
    user_t user_;
    uint16_t time_;
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
        UserPi.set(TUser::OPERATOR);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TUser UserPi;
    TUser UserPc;

    TPwd pwdAdmin;
    TPwd pwdEngineer;
};

#endif /* PARAMIS_H_ */
