#ifndef USER_H
#define USER_H

#include "glbDefine.h"
#include "securityevent.h"

class TUser {

// ����� �� ������ ������������ � ��������
#ifdef NDEBUG
#define kTimeToResetPi 1800
#define kTimeToResetPc 900
#else
#define kTimeToResetPi 300
#define kTimeToResetPc 150
#endif

    /// ��������� ������������ ��� ��������� �������.
    typedef struct {
        user_t user;    ///< ������������.
        uint16_t time;  ///< ������.
    } stUser_t;

public:

    TUser();

    /**	������������� ������������.
     *
     *  @param[in] src �������� �������.
     * 	@param val ������������.
     * 	@return true ���� �������� �����������, ����� false.
     */
    bool set(userSrc_t src, user_t val);

    /**	������.
     *
     *  @param[in] src �������� �������.
     * 	@return ������������.
     */
    user_t get(userSrc_t src) const;

    /// ��� �������.
    void tick();

    /** ����� �������.
     *
     *  @param[in] src �������� �������.
     */
    void resetTimer(userSrc_t src);

    /** ���������� ������� �������� �������.
     *
     *  @param[in] src �������� �������.
     */
    uint16_t getTimer(userSrc_t src) const;

    /// ����� ���� �������������.
    void reset();

    /** ����� ���� �� ���������.
     *
     *  @param[in] src �������� �������.
     */
    void reset(userSrc_t src);

    /** ��������� ����������� ������� ������� ���� � ���������.
     *
     *  @param[in] src �������� �������.
     *  @param[in] chuser ����������� ������������.
     *  @return True ���� ����� ������.
     */
    bool checkAccess(userSrc_t src, user_t chuser) const;

    /** ������������� ��������� �� ������� �������.
     *
     *  @param[in] sevent ������� �������.
     */
    void setInfoSecurity(TSecurityEvent *sevent) {
        this->sevent = sevent;
    }

private:
    /** ��� �������.
     *
     *  ����������� ����� �� ������ ������� ����.
     *
     *  @param[in] src �������� �������.
     */
    void tick(userSrc_t src);

    /// ��������� �������������.
    stUser_t stuser[USER_SOURCE_MAX];

    /// ������� �������.
    TSecurityEvent *sevent;
};

#endif // USER_H
