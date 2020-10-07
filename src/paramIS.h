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
static const uint16_t kTickToDecCounter PROGMEM = 600000UL / MENU_TIME_CYLCE;
#else
static const uint16_t kTickToDecCounter PROGMEM = 10000UL / MENU_TIME_CYLCE;
#endif
public:
    // �����������.
    TPwd() {
        COMPILE_TIME_ASSERT(SIZE_OF(pwd_) == PWD_LEN);
        COMPILE_TIME_ASSERT(PWD_CNT_BLOCK == 0x03);

        reset();
    }

    /** ������������� ����� �������� ������.
     *
     *  @param[in] pwd ������.
     *  @return false � ������ ���������� ��������.
     */
    bool set(uint8_t *pwd) {
        bool check = checkValue(pwd);

        if (check) {
            setValue(pwd);
        }

        return check;
    }

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
    bool setCounter(uint8_t value) {
        counter_ = (value <= 2*PWD_CNT_BLOCK) ? value : 2*PWD_CNT_BLOCK;

        if (!isInit()) {
            if (counter_ == PWD_CNT_BLOCK) {
                counter_ = 2 * PWD_CNT_BLOCK;
            }
            tcounter_ = counter_;
            time_ = (counter_ > 0) ? pgm_read_word(&kTickToDecCounter) : 0;
            init_ = true;
        }

        return (value == counter_);
    }

    /// ����� ��������.
    void clrCounter() {
        if (isInit()) {
            tcounter_ = 0;
        }
    }

    /** ����������� ������� ������ ����� ������.
     *
     *  ���� ��� ������ ��������� ���� ������ ����� �������.
     */
    void incCounter() {
        if (tcounter_ == 0) {
            time_ = pgm_read_word(&kTickToDecCounter);
        }

        if (isInit()) {
            if (tcounter_ <= PWD_CNT_BLOCK) {
                tcounter_++;
            }

            if (tcounter_ >= PWD_CNT_BLOCK) {
                tcounter_ = 2*PWD_CNT_BLOCK;
            }
        }
    }

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
    void reset() {
        init_ = false;
        counter_ = 2*PWD_CNT_BLOCK;
        tcounter_ = counter_;
        time_ = pgm_read_word(&kTickToDecCounter);

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = 0;
        }
    }

    /** ��� �������.
     *
     *  ������� ����� �� ������ ������ ��������.
     *  ��������� ��������� ��������.
     *
     *  @return true ���� �������� �������� ���� ��������.
     */
    bool tick() {
        bool change = false;

        if (isInit()) {
            if (tcounter_ > 0) {
                if (time_ > 0) {
                    time_--;
                }

                if (time_ == 0) {
                    tcounter_--;

                    if (tcounter_ == PWD_CNT_BLOCK) {
                        tcounter_ = 0;
                    }

                    if (tcounter_ > 0) {
                        time_ = pgm_read_word(&kTickToDecCounter);
                    }
                }
            }

            if (tcounter_ != counter_) {
                change = true;
            }
        }

        return change;
    }

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
    bool checkValue(uint8_t *pwd) const {
        bool check = true;

        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            check &= ((pwd[i] >= '0') && (pwd[i] <= '9'));
        }

        return check;
    }

    /** ������������� �������� ������.
     *
     *  @param[in] pwd ������.
     */
    void setValue(uint8_t *pwd) {
        for(uint8_t i = 0; i < SIZE_OF(pwd_); i++) {
            pwd_[i] = pwd[i];
        }
    }

    bool init_;             ///< ������������� (������ ����������).
    uint8_t pwd_[PWD_LEN];  ///< ������.
    uint8_t counter_;       ///< ������� ����� ��������� ������ �� ���.
    uint8_t tcounter_;      ///< ������� ����� ��������� ������ � ���-��.
    uint16_t time_;         ///< ������� ����� �� ���������� �������� ������.
};

class TUser {


#ifdef NDEBUG
    static const uint16_t kTimeToReset PROGMEM = (900000UL / MENU_TIME_CYLCE);
#else
    static const uint16_t kTimeToReset PROGMEM = (300000UL / MENU_TIME_CYLCE);
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

    TUser() {
        reset();
    }

    /**	������.
     *
     * 	@param val ������������.
     * 	@return False � ������ ���������� ��������.
     */
    bool set(user_t val) {
        if ((val >= MIN) && (val < MAX)) {
            if (user_ != val) {
                time_ = pgm_read_word(&kTimeToReset);
                user_ = val;
            }
        }

        return (user_ == val);
    }

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
    void tick() {
        if (time_ > 0) {
            time_--;
        }

        if (time_ == 0) {
            reset();
        }
    }

    /// ����� �������.
    void resetTimer() {
        if (user_ != OPERATOR) {
            time_ = pgm_read_word(&kTimeToReset);
        }
    }

    /// ���������� ������� �������� �������.
    uint16_t getTimer() {
        return time_;
    }

    /// ����� ���� �� ���������.
    void reset() {
        user_ = OPERATOR;
        time_ = 0;
    }

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
        User.set(TUser::OPERATOR);
    }

    TIsEvent EventAdmin;
    TIsEvent EventEngineer;

    TUser User;

    TPwd pwdAdmin;
    TPwd pwdEngineer;
};

#endif /* PARAMIS_H_ */
