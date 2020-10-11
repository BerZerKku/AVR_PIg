/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMIS_H_
#define PARAMIS_H_

#include "glbDefine.h"
#include "param.h"
#include "paramIS.h"

// TODO ������� �������� ������ ������� � �� ����������� ��������� ��������/���������.

class TUser {

#ifdef NDEBUG
#define kTimeToReset (900000UL / MENU_TIME_CYLCE)
#else
#define kTimeToReset (300000UL / MENU_TIME_CYLCE)
#endif
public:

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

    /** ��������� ������������ �� ����������� ��������� ���������.
     *
     *  @param[in] ������� ������������.
     *  @param[in] chuser ����������� ������������.
     *  @return True ���� ����� ������.
     */
    bool checkChangeUser(user_t chuser) const;

private:
    user_t user_;
    uint16_t time_;
};

/// ������.
class TPwd {
    /// ����� ������ ������ ����� ������
#ifdef NDEBUG
#define kkTickWaiter (600000UL / MENU_TIME_CYLCE)
#else
#define kTickToDecCounter (30000UL / MENU_TIME_CYLCE)
#endif
#define kTickWait (1000UL / MENU_TIME_CYLCE)
#define kTickToRestPwd (5000UL / MENU_TIME_CYLCE)
    /// ��������� ������ ������� � �������� �� ���������.
    enum resetState_t {
        RESET_STATE_no = 0,         ///< ���.
        RESET_STATE_waitDisable,    ///< �������� �������� � �������.
        RESET_STATE_disable,        ///< ����� ������ �� �������.
        RESET_STATE_waitDisableTime,///< �������� ������� � ������ �������.
        RESET_STATE_resetPassword,  ///< ����� ������� � ���������.
        RESET_STATE_waitReset,      ///< �������� ������ ������� � ���������.
        RESET_STATE_enable,         ///< ����� ������ �� ������.
        RESET_STATE_waitEnable,     ///< �������� ������ ������.
        //
        RESET_STATE_MAX
    };

    struct pwd_t {
        bool init;             ///< ������������� (������ ����������).
        uint8_t pwd[PWD_LEN];  ///< ������.
        uint8_t counter;       ///< ������� ����� ��������� ������.
        uint8_t tcounter;      ///< ������� ����� ��������� ������ �� ���.
        uint16_t ticks;        ///< ������� ����� �� ���������� �������� ������.
        bool changed;          ///< ���� ����� ������ ��������.
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
     *  ���� ������ �����, ������� ������ �������� ������ ������������.
     *  ���� ������ �� �����, ������� ������ �������� ������ �������������.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ �����, ����� false.
     */
    bool checkPassword(user_t user, const uint8_t *pwd);

    /** ���������� �������� ������.
     *
     *  @param[in] user ������������.
     *  @return ������.
     *  @return NULL � ������ ���������� ������.
     */
    uint8_t* getPwd(user_t user);

    /** ������������� �������� ������ ��������� �� ���.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ ����������, ����� false.
     */
    bool setPwd(user_t user, const uint8_t *pwd);

    /** �������� ������ ������������.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ �������, ����� false.
     */
    bool changePwd(user_t user, const uint8_t *pwd);

    /** �������� ������ ������������.
     *
     *  @param[in] param �������� ������ ������������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ �������, ����� false.
     */
    bool changePwd(eGB_PARAM param, const uint8_t *pwd);

    /** ������������� ������� ����� ��������� ������.
     *
     *  ���� ������������� �������� ������ �������� �� ����� "��������"
     *  ����� �� ���������� �������� ��������.
     *
     *  @param[in] user ������������.
     *  @param[in] value ��������.
     */
    void setCounter(user_t user, uint8_t value);

    /** ���������� �������� �������� ����� ��������� ������.
     *
     *  ��� TUser::OPERATOR ������ ������������ 0.
     *  ��� TUser::MAX ��� ��������� �������� ������������ 0xFF.
     *
     *  @param[in] user ������������
     *  @return ��������  ��������.
     */
    uint8_t getCounter(user_t user) const;

    /** ���������� ����� �� ��������� ���������� � ��������.
     *
     *  ��� TUser::OPERATOR ������ ������������ 0.
     *  ��� TUser::MAX ��� ��������� �������� ������������ 9999.
     *
     *  @param[in] user ������������.
     *  @return ����� �� ����������.
     */
    uint16_t getLockTime(user_t user) const;

    /// ����� ��������.
    void reset();

    /** ����� ��������.
     *
     *  ��������� ������������ ��� ������ ����� � ��� ��� ��.
     *
     *  @param[in] user ������������
     */
    void reset(user_t user);

    /** ��� �������.
     *
     *  ������� ����� �� ������ ������ ��������.
     *  ��������� ��������� ��������.
     *
     *  @param[in] user ������������
     *  @return true ���� �������� �������� ���� ��������.
     */
    bool tick(user_t user);

    /** ��������� ��������� ����� �������������.
     *
     *  ��� TUser::OPERATOR ������ ������������ true.
     *  ��� TUser::MAX ��� ��������� �������� ������������ false.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @return true ���� �������� ���� �����������, ����� false.
     */
    bool isInit(user_t user) const;

    /** ��������� ������� ��������� ���������� ������ ����.
     *
     *  ��� TUser::OPERATOR ������ ������������ false.
     *  ��� TUser::MAX ��� ��������� �������� ������������ true.
     *
     *  @param[in] user ������������
     */
    bool isLocked(user_t user) const;

    /** ��������� ���� ��������� ������.
     *
     *  @param[in] user ������������.
     *  @return
     */
    bool isChangedPwd(user_t user) const;

    /** ���������� �������� ��� �������� ������
     *
     *  @param[in] user ������������.
     *  @return ��������.
     *  @retval GB_PARAM_NO ���� � ������������ ��� ���������.
     */
    eGB_PARAM getCounterParam(user_t user) const;

    /** ���������� �������� ��� ������.
     *
     *  @param[in] user ������������.
     *  @return ��������.
     *  @retval GB_PARAM_NO ���� � ������������ ��� ���������.
     */
    eGB_PARAM getPwdParam(user_t user) const;

    /// �������� ������ � �������� �� ���������.
    void resetPwdToDefault();

    /** ���������� ������ � �������� �� ���������.
     *
     *  @param regime ������� ����� ������ ����������.
     */
    void resetPwdToDefaultCycle(eGB_REGIME regime);

    /// ��������� ������������� �������� ������ � �������� �� ���������.
    bool isResetToDefault();

    /// �������� ������������� ������� ������� �� ������.
    bool isWaitDisableDevice();

    /// �������� ������������� �������� ������ � �������� �� ���������.
    bool isWaitResetPassword();

    /// �������� ������������� ������ ������� � ������.
    bool isWaitEnableDevice();

private:
    /// ��������� ������ ������� � �������� �� ���������.
    resetState_t resetState;

    pwd_t password[USER_MAX-USER_operator-1];

    /** ��������� ���� ��������� ������.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @return
     */
    bool isChangedPwd(int8_t index) const;

    /** ���������� ��������� ����� �������������.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @return true ���� �������� ���� �����������, ����� false.
     */
    bool isInit(int8_t index) const;

    /** �������� ������ ������������.
     *
     *  @param index ������ ������������ � �������.
     *  @param[in] pwd ������.
     *  @return true ���� ������ �������, ����� false.
     */
    bool changePwd(int8_t index, const uint8_t *pwd);

    /** ��������� ������������ �������� ������.
     *
     *  @param[in] user ������������.
     *  @param[in] pwd ������.
     *  @return ��������� ��������, true ���� ��� � �������.
     */
    bool checkValue(const uint8_t *password) const;

    /** ����������� ������� ����� ��������� ������.
     *
     *  ���� ��� ������ ��������� ���� ������ ����� �������.
     *
     *  @param[in] ������ ������������ � �������.
     */
    void incCounter(int8_t index);

    /** ���������� ������� ����� ��������� ������.
     *
     *  @param[in] ������ ������������ � �������.
     */
    void clrCounter(int8_t index);

    /** ���������� ������ ������������ � �������.
     *
     *  @param[in] user ������������
     *  @return ������ ������������ � �������.
     *  @retval -1, ���� ������������ ���.
     */
    int8_t getUserIndex(user_t user) const;

    /** ������������� ���������� ����� ������� �� ������ ���������� ����.
     *
     *  @param[in] index ������ ������������ � �������.
     *  @param[in] enable true ���������� ������� �����, ����� ��������.
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
    enum state_t {
        STATE_OK = 0x00,            // ��
        STATE_NO_ACCESS = 0x01,     // ��� �������
        STATE_WRONG_PWD = 0x02,     // �������� ������
        STATE_WRONG_NEW_PWD = 0x03, // �������� ����� ������
        //
        STATE_MAX
    };

    TInfoSecurity() {
        UserPi.set(USER_operator);
        UserPc.set(USER_operator);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TUser UserPi;
    TUser UserPc;

    TPwd pwd;

    /** ������������� ������������ ��� ������ � ��.
     *
     *  @param[in] user ������������.
     *  @param[in] p ������
     *  @return ��������� ���������.
     *  @retval STATE_NO_ACCESS
     *  @retval STATE_WRONG_PWD
     */
    state_t setUserPc(user_t user, const uint8_t *p=NULL);

    /** �������� ������ ������������ � ��.
     *
     *  @param[in] user ������������ ��� �������� �������� ������.
     *  @param[in] cp ������ �������� ������������.
     *  @param[in] np ����� ������ ������������.
     *  @return
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

};

#endif /* PARAMIS_H_ */
