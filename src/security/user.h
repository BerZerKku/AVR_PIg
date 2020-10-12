#ifndef USER_H
#define USER_H

#include "glbDefine.h"

/// ������������ (����).
typedef enum {
    USER_operator = 0,  ///< ��������. ������ ���� � ������!
    USER_engineer,      ///< �������.
    USER_admin,         ///< �������������.
    USER_MAX,           ///< ������������ ���������� �������������.
    USER_factory        ///< �������������. ������ ���� ������ USER_MAX!
} user_t;

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

#endif // USER_H
