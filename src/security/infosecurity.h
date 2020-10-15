#ifndef TINFOSECURITY_H
#define TINFOSECURITY_H

#include "glbDefine.h"
#include "pwd.h"
#include "securityevent.h"

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

    /// �����������.
    TInfoSecurity();

    TPwd pwd;
    TSecurityEvent sevent;

    /** ���.
     *
     *  @param[in] regime ����� ������ ����������.
     *  @param[out] com ������� ��� �������� � ���.
     */
    void tick(eGB_REGIME regime, eGB_COM &com);

    /** ������������� ������������ ��� ������ � ��.
     *
     *  @param[in] user ������������.
     *  @param[in] p ������
     *  @return ��������� ���������.
     *  @retval STATE_OK
     *  @retval STATE_NO_ACCESS
     *  @retval STATE_WRONG_PWD
     */
    state_t setUserPc(user_t user, const uint8_t *p=NULL);

    /** ���������� ������������ ��� ���������� ��������� �������.
     *
     *  @param[in] src �������� �������.
     */
    user_t getUser(userSrc_t src) const {
        return usr.get(src);
    }

    /** ���������� ����� �� ��������������� ���������� ������ ������������
     *
     *  ! ������������ ������ ��� �������.
     *
     *  @param[in] src �������� �������.
     *  @return �����.
     */
    uint16_t getUserTime(userSrc_t src) const {
        return usr.getTimer(src);
    }

    /** ��������� ����������� ������� ������� ���� � ���������.
     *
     *  @param[in] user ����������� ������������.
     *  @param[in] src �������� �������.
     *  @return true ���� ������ ����, ����� false.
     */
    bool checkUserAccess(user_t user, userSrc_t src);

    /// ����� ������������.
    void rstUser(userSrc_t src) {
        usr.reset(src);
    }

    /** ����� ������� ���������� ������������.
     *
     *  @param[in] src �������� �������.
     */
    void rstUserTimer(userSrc_t src) {
        usr.resetTimer(src);
    }

    /** ������������� ������������.
     *
     *  @param[in] user ������������.
     *  @param[in] src �������� �������.
     *  @return true ���� ������������ ����������, ����� false.
     */
    bool setUser(user_t user, userSrc_t src);

    /** �������� ������ ������������ � ��.
     *
     *  @param[in] user ������������ ��� �������� �������� ������.
     *  @param[in] cp ������ �������� ������������.
     *  @param[in] np ����� ������ ������������.
     *  @return ��������� ��������� ������.
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

    /** �������� ������ ������������ � ��.
     *
     *  @param[in] param �������� ������������.
     *  @param[in] np ����� ������ ������������.
     *  @return true ���� ������ �������, ����� false.
     */
    bool changeUserPiPwd(eGB_PARAM param, const uint8_t *np);

    /** ��������� ������ ������������.
     *
     *  � ������ ������ ����� ��������� ��������� � ����� ������������ ������
     *  ��� ���������� ������������.
     *
     *  @param[in] user ������������ ��� �������� ������.
     *  @param[in] src �������� �������.
     *  @param[in] cp ������  ������������.
     *  @return true ���� ������ ������, ����� false.
     */
    bool checkUserPwd(user_t user, userSrc_t src, const uint8_t *cp);

    /** ��������� ���������� �������� ������� � ���.
     *
     *  ������������� ����������� ���� ���������� ������� � ��� �������������
     *  ���������� ����� ������� �� ���������� �������� ������������.
     *
     *  @param[in] com �������.
     *  @return ��������� ��������.
     *  @retval STATE_OK
     *  @retval STATE_NO_ACCESS
     */
    state_t isComAccess(eGB_COM com);

    /** ���������� �������� ��� ������.
     *
     *  @param[in] user ������������.
     *  @return ��������.
     *  @retval GB_PARAM_NO ���� � ������������ ��� ���������.
     */
    eGB_PARAM getPwdParam(user_t user) const;

private:
    /// ������������.
    TUser usr;

    /** �������� ������ ������������.
     *
     *  @param[in] user ������������ ��� �������� �������� ������.
     *  @param[in] src �������� �������.
     *  @param[in] np ����� ������ ������������.
     *  @return true ���� ������ ����������, ����� false.
     */
    bool changeUserPwd(user_t user, userSrc_t src, const uint8_t *np);
};

#endif // TINFOSECURITY_H
