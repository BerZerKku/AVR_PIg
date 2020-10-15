#ifndef TSECURITY_EVENT_H
#define TSECURITY_EVENT_H

#include "glbDefine.h"
#include "fifo.h"

class TSecurityEvent {
public:
    /// C������ ��� ������� ��.
    enum event_t {
        EVENT_autoEnd = 0,  // �������������� ���������� ������ ������
        EVENT_chgSettings,  // (���) ��������� ��������
        EVENT_chgTime,      // (���) ��������� ���� ��� ������� �������������
        EVENT_chgRegime,    // (���) ��������� ������ ������
        EVENT_setOperator,  // ��������� ���� ������������ �� ��������
        EVENT_setEngineer,  // ��������� ���� ������������ �� �������
        EVENT_setAdmin,     // ��������� ���� ������������ �� �������������
        EVENT_blkEngineer,  // ���������� ������� � ���� �������
        EVENT_blkAdmin,     // ���������� ������� � ���� �������������
        EVENT_chgPwdEng,    // ��������� ������ ��� ���� �������
        EVENT_chgPwdAdm,    // ��������� ������ ��� ���� �������������
        EVENT_wrgPwdEng,    // ���� ��������� ������ ��� ���� �������
        EVENT_wrgPwdAdm,    // ���� ��������� ������ ��� ���� �������������
        EVENT_resetPwd,     // ����� ������� �� ��������� ��������
        //
        EVENT_MAX
    };

    /// �����������.
    TSecurityEvent();

    /** ���������� ���������� ������� �� �������.
     *
     *  @param[out] user ���� ������������.
     *  @param[out] source �������� ������� ������������.
     *  @param[out] event �������.
     *  @return true ���� ���� �������, ����� false.
     */
    bool pop(user_t &user, userSrc_t &source, event_t &event);

    /** ��������� ������ � ���������� ������������.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] puser ��������������� ������������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool pushPwdBlocked(user_t cuser, userSrc_t source, user_t buser);

    /** ��������� ������ � ����� ������ ��� ������������.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] puser ������������ ��� �������� ������� ������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool pushPwdChanged(user_t cuser, userSrc_t source, user_t puser);

    /** ��������� ������ � ������ ������� �� ��������� ��������.
     *
     *  ����� ����� ���� ������ � ��.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @return
     */
    bool pushPwdReset(user_t cuser, userSrc_t source);

    /** ��������� ������ � ����� ��������� ������ ��� ������������.
     *
     *  ���� ������������ ��� ������������ ������ ������ � �������� ������
     *  ����� ��������� ������ � ����������.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] puser ������������ ��� �������� ������� ������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool pushPwdWrong(user_t cuser, userSrc_t source, user_t puser);

    /** ��������� ������ � ����� ������������.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] nuser ����� ������������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool pushUserChanged(user_t cuser, userSrc_t source, user_t nuser);

    /** ��������� ������ �� �������������� ����� ������������.
     *
     *  @param[in] cuser ������� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] nuser ����� ������������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool pushUserChangeAuto(user_t cuser, userSrc_t source);

    /// �������� �� ���������� ������� � �������.
    bool isEmpty() const {
        return eventMsg.isEmpty();
    }

private:
    /// ��������� ��������� ��� ������� ��.
    typedef struct {
        user_t user;        ///< ���� ������������.
        userSrc_t source;    ///< �������� �������.
        event_t event;      ///< �������.
    } eventMsg_t;

    /// �������.
    TFifo<IS_MSG_FIFO_SIZE, eventMsg_t> eventMsg;

    /** ��������� ������ ������� � �������.
     *
     *  @param[in] user ���� ������������.
     *  @param[in] source �������� ������� ������������.
     *  @param[in] event �������.
     *  @return true ���� ������� ��������, ����� false.
     */
    bool push(user_t user, userSrc_t source, event_t event);
};

#endif // TSECURITY_EVENT_H
