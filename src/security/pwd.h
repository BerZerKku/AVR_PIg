#ifndef TPWD_H
#define TPWD_H

#include "src/glbDefine.h"
#include "user.h"

/// ������.
class TPwd {
    /// ����� ������ ������ ����� ������, � ��������
#ifdef NDEBUG
#define kTickToDecCounter 600
#else
#define kTickToDecCounter 60
#endif

#define kTickWait 1
#define kTickToRestPwd 5

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
     *  @param[in] user ������������.
     *  @return ��������  ��������.
     */
    uint8_t getCounter(user_t user) const;

    /** ���������� ������� ������� ����� ��������� ������ � ���������� �� ���.
     *
     *  @param[in] user ������������.
     *  @return true ���� �������� ����������, ����� false.
     */
    uint8_t isCounterChanged(user_t user) const;

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

    /// ��� �������.
    void tick();

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
     *  @return true ���� ������������ ������������, ����� false.
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

    /// ������ �������� �������.
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

    /** ��� �������.
     *
     *  ������� ����� �� ������ ������ ��������.
     *
     *  @param index ������ ������������ � �������.
     */
    void tick(int8_t index);
};

#endif // TPWD_H
