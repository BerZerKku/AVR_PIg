#ifndef USER_H
#define USER_H

#include "src/glbDefine.h"

class TUser {
// ����� �� ������ ������������ � ��������
#ifdef NDEBUG
#define kTimeToReset 900
#else
#define kTimeToReset 150
#endif

    /// ��������� ������������ ��� ��������� �������.
    typedef struct {
        user_t user;    ///< ������������.
        uint16_t time;  ///< ������.
        user_t olduser; ///< ���������� �������� ������������.
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
     *  @param[in] src �������� ������� ������������.
     * 	@return ������������.
     */
    user_t get(userSrc_t src) const;

    /// ��� �������.
    void tick();

    /** ��������� ������� ��������������� ���������� ������ ������.
     *
     *  @param[in] src �������� ������� ������������.
     *  @param[out] olduser ���������� �������� ������������.
     *  @return true ���� ������������ ��� �������, ����� false.
     */
    bool isAutoReset(userSrc_t src, user_t &olduser);

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
};

#endif // USER_H
