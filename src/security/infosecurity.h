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
    TUser usr;
    TSecurityEvent sevent;

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

    /** �������� ������ ������������ � ��.
     *
     *  @param[in] user ������������ ��� �������� �������� ������.
     *  @param[in] cp ������ �������� ������������.
     *  @param[in] np ����� ������ ������������.
     *  @return ��������� ��������� ������.
     */
    state_t changeUserPcPwd(user_t user, const uint8_t *cp, const uint8_t *np);

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
};

#endif // TINFOSECURITY_H
