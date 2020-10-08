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

/// ������.
class TPwd {
    /// ����� ������ ������ ����� ������
#ifdef NDEBUG
#define kTickToDecCounter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (30000UL / MENU_TIME_CYLCE)
#endif

    struct pwd_t {
        bool init;             ///< ������������� (������ ����������).
        uint8_t pwd[PWD_LEN];  ///< ������.
        uint8_t counter;       ///< ������� ����� ��������� ������.
        uint8_t tcounter;      ///< ������� ����� ��������� ������ �� ���.
        uint16_t ticks;        ///< ������� ����� �� ���������� �������� ������.
    };

public:
    // �����������.
    TPwd();

    /** ���������� ��������� �������� ������ ��� ������������.
     *
     *  ��� TUser::OPERATOR ������ ������������ true.
     *  ��� TUser::MAX ��� ��������� �������� ������������ false.
     *  ��� ���������������� ������������ ������������ false.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ �����, ����� false.
     */
    bool checkPassword(TUser::user_t user, const uint8_t *pwd);

    /** ���������� �������� ������.
     *
     *  @param[in] user ������������.
     *  @return ������.
     *  @return NULL � ������ ���������� ������.
     */
    uint8_t* getPwd(TUser::user_t user);

    /** ������������� ����� �������� ������.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ ����������, ����� false.
     */
    bool setPwd(TUser::user_t user, uint8_t *password);

    /** ������������� ������� ����� ��������� ������.
     *
     *  ���� ������������� �������� ������ �������� �� ����� "��������"
     *  ����� �� ���������� �������� ��������.
     *
     *  @param[in] user ������������.
     *  @param[in] value ��������.
     */
    void setCounter(TUser::user_t user, uint8_t value);

    /** ����� ��������.
     *
     *  @param[in] user ������������.
     */
    void clrCounter(TUser::user_t user);

    /** ����������� ������� ������ ����� ������.
     *
     *  ���� ��� ������ ��������� ���� ������ ����� �������.
     *
     *  @param[in] user ������������
     */
    void incCounter(TUser::user_t user);

    /** ���������� �������� �������� ����� ��������� ������.
     *
     *  ��� TUser::OPERATOR ������ ������������ 0.
     *  ��� TUser::MAX ��� ��������� �������� ������������ 0xFF.
     *
     *  @param[in] user ������������
     *  @return ��������  ��������.
     */
    uint8_t getCounter(TUser::user_t user) const;

    /** ���������� ����� �� ��������� ���������� � ��������.
     *
     *  ��� TUser::OPERATOR ������ ������������ 0.
     *  ��� TUser::MAX ��� ��������� �������� ������������ 9999.
     *
     *  @param[in] user ������������.
     *  @return ����� �� ����������.
     */
    uint16_t getLockTime(TUser::user_t user) const;

    /** ����� ��������.
     *
     *  ��������� ������������ ��� ������ ����� � ���.
     *
     *  @param[in] user ������������
     */
    void reset(TUser::user_t user);

    /** ��� �������.
     *
     *  ������� ����� �� ������ ������ ��������.
     *  ��������� ��������� ��������.
     *
     *  @param[in] user ������������
     *  @return true ���� �������� �������� ���� ��������.
     */
    bool tick(TUser::user_t user);

    /** ���������� ��������� ����� �������������.
     *
     *  ��� TUser::OPERATOR ������ ������������ true.
     *  ��� TUser::MAX ��� ��������� �������� ������������ false.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @return true ���� �������� ���� �����������, ����� false.
     */
    bool isInit(TUser::user_t user) const;

    /** ���������� ������� ��������� ���������� ������ ����.
     *
     *  ��� TUser::OPERATOR ������ ������������ false.
     *  ��� TUser::MAX ��� ��������� �������� ������������ true.
     *
     *  @param[in] user ������������
     */
    bool isLocked(TUser::user_t user) const;

private:
    pwd_t password[TUser::MAX-TUser::OPERATOR];

    /** ���������� ��������� ����� �������������.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @return true ���� �������� ���� �����������, ����� false.
     */
    bool isInit(int8_t index) const;

    /** ��������� ������������ �������� ������.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return ��������� ��������, true ���� ��� � �������.
     */
    bool checkValue(uint8_t *password) const;

    /** ���������� ������ ������������ � �������.
     *
     *  @param[in] user ������������
     *  @return ������ ������������ � �������.
     *  @retval -1, ���� ������������ ���.
     */
    int8_t getUserIndex(TUser::user_t user) const;

    /** ������������� ���������� ����� ������� �� ������ ���������� ����.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @param[in] cnt ���������� ���������� ������� �� ������ ����������.
     */
    void setTicks(int8_t index, bool enable);
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

    TPwd pwd;
};

#endif /* PARAMIS_H_ */
