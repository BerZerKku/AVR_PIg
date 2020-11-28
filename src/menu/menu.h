/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "glbDefine.h"
#include "paramBsp.h"
#include "LocalParams.h"
#include "menuPunkt.h"
#include "enterParam.h"

/// ����� �� ������ ������������� �������, ��
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)

/// ����� �� ����������������� �������, ��
#define TIME_TO_REINIT_LCD (10000UL / MENU_TIME_CYLCE)

/// ������������ ���-�� ������������ �� ������ ����������
#define MAX_NUM_MEAS_PARAM 6

/// ������������ ���-�� ������������ ���������� � ���� "���������"
#define MAX_NUM_MEAS_PARAM_LVL 10

/// ����� ������ ����������� ��������� �� �����, ��
#define TIME_MEAS_PARAM (3000UL / MENU_TIME_CYLCE)

/// ����� ������ ������ �� �����, �� (�������� ������/��� ������)
#define TIME_TEXT (1000UL / MENU_TIME_CYLCE)

/// ����� �� ������ �� ���� 1 / ���� 2 � �.�. ���� ����� != ������� �����, ��
#define TIME_TEST_EXIT (1000UL / MENU_TIME_CYLCE)

/// ������ �������� ������������� ���������� ���������� ������, ��
#define TIME_SECURITY (1000UL / MENU_TIME_CYLCE)

/// ���������� ���������
enum eMENU_MEAS_PARAM {
    MENU_MEAS_PARAM_NO,         ///< ��� ����������� ���������
    MENU_MEAS_PARAM_DATE,       ///< ����
    MENU_MEAS_PARAM_TIME,       ///< �����
    MENU_MEAS_PARAM_UZ,         ///< ����� �� ������
    MENU_MEAS_PARAM_UZ1,        ///< ����� �� ������ 1 (��� 3-� ��������)
    MENU_MEAS_PARAM_UZ2,        ///< ����� �� ������ 2 (��� 3-� ��������)
    MENU_MEAS_PARAM_UC,         ///< ����� �� ��
    MENU_MEAS_PARAM_UC1,        ///< ����� �� �� 1 (��� 3-� ��������)
    MENU_MEAS_PARAM_UC2,        ///< ����� �� �� 2 (��� 3-� ��������)
    MENU_MEAS_PARAM_UN,         ///< ������� �����
    MENU_MEAS_PARAM_UN1,        ///< ������� ����� 1 (��� 3-� ��������)
    MENU_MEAS_PARAM_UN2,        ///< ������� ����� 2 (��� 3-� ��������)
    MENU_MEAS_PARAM_UOUT,       ///< ��������� ������
    MENU_MEAS_PARAM_IOUT,       ///< ��� ������
    MENU_MEAS_PARAM_ROUT,       ///< ������������� �����
    MENU_MEAS_PARAM_SD,         ///< �������� � �������
    MENU_MEAS_PARAM_D,          ///< ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
    MENU_MEAS_PARAM_TEMPERATURE,///< ����������� �� ����� ���-��
    MENU_MEAS_PARAM_DF          ///< ���������� ������� �� �� ��� (�400)
};

// ����� ����
class clMenu {
    /// ���������.
    enum msg_t {
        MSG_NO = 0,             ///< ��������� ���.
        MSG_WRONG_REGIME,       ///< �������� ���������� � ������ "�������".
        MSG_WRONG_USER_EDIT,    ///< ������������ ���� ��� ���������.
        MSG_WRONG_USER_READ,    ///< ������������ ���� ��� ���������.
        MSG_WRONG_PWD,          ///< ������ �� ������ ������.
        MSG_WRONG_DEVICE,       ///< ��� �������� �� ���������.
        MSG_BLOCK_USER,         ///< ����� ���� ������������ ������������.
        MSG_DISABLE,            ///< ����� �������� ����� �� "�������".
        MSG_INIT,               ///< �������������.
        MSG_RESET_PWD,          ///< ����� ������� �� ��������� ��������.
        //
        MSG_MAX
    };

public:

    /** �����������.
     *  ���������������� �� ��������� �������������� ��� ��������.
     *  ����������� ����������� ��� ������ �������.
     *  ������������ ������� �� ��������� ������� ����.
     */
    clMenu();

    /** ������ � ����.
     */
    void proc();

    /** ��������� ���� �������� � ��������� ���� � ��� ������.
     *  �� ��������� ����� ������ ����� ������ �� �������� ���������� �
     *  ���������� ��������� ���������.
     *  @param device ��� ����������
     *  @return False � ������ ������, ����� True
     */
    bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

    /** ���������� ��������� ������� �� ����������.
     *
     *  ������� ����������� ������� �������, ���� �� ��� ���� ������� �������.
     *
     *  ������ ���� ��������� �� 8 ������ ������������:
     *  - ������� ���������;
     *  - �����;
     *  - ������� �� ������ 2
     *      - ������ ������ ���� ������� ��������, ���� ����
     *      - ����� ������� ����������� ��� ��������� ���� (������������� � �.�.)
     *  - ������� �� ������ 1
     *      ������� ������ ��������� ���������� � ������ ������ ����
     *  - ���� ������ ���, �� ������� ����� ������������.
     *
     *  @param ���
     *  @return �������
     */
    eGB_COM getTxCommand();

    /** ���������� ������� ��������� ����� � ���.
     *
     *  @return true ���� ���� ����� � ���, ����� false.
     */
    bool isConnectionBsp() const {
        return connectionBsp_;
    }

    /** ������������� ���� ������� ����� � ���.
     *
     *  @param[in] connection ���� ������� �����.
     */
    void setConnectionBsp(bool connection) {
        connectionBsp_ = connection;
    }

    /** ���������� ������� ��������� ����� � ��.
     *
     *  @return true ���� ���� ����� � ��, ����� false.
     */
    bool isConnectionPc() const {
        return connectionPc_;
    }

    /** ������������� ����� ������� ����� � ��.
     *
     *  @param[in] connection ���� ������� �����.
     */
    void setConnectionPc(bool connection) {
        connectionPc_ = connection;
    }

    /** ���������
     */
    stGBparam sParam;

private:
        // ��� ������
    eKEY key_;

    // ���� ������� ��������
    bool blink_;

    // ���� ����� ���������� ����������
    bool blinkMeasParam_;

    // ���� �������� ���������� ����� � ���, true - ����
    bool connectionBsp_;
    // ���� �������� ��������� ����� � ��, true - ����
    bool connectionPc_;

    // true - ���������� ������� �� ����� ������
    bool cursorEnable_;

    // ������� ��������� ������� (����� ������)
    uint8_t cursorLine_;

    // ���-�� ������������ ����������
    uint8_t lineParam_;

    // ������� �������� (�������� ����� ������� ������� ���������, ������� � 1)
    uint8_t curCom_;

    // ���� true - ���������� ������� ������� ����
    bool lvlCreate_;

    // ����� ������ ���.��������� �� ����� (�������� ��������� �� ������)
    msg_t msg_;
    uint8_t delay_;

    // ���������� ���������
    eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM * 2];

    // ���������� ��������� ��� ������ ���� "���������"
    eMENU_MEAS_PARAM measParamLvl[MAX_NUM_MEAS_PARAM_LVL];

    // ������� ������ ����
    TMenuPunkt Punkts_;

    // ��������� ��� ����� ����� ��������
    TEnterParam EnterParam;

    //  ��������� ��� ��������������� ���������
    bool setDeviceK400();
    bool setDeviceRZSK();
    bool setDeviceR400M();
    bool setDeviceOPTO();

    // ������� ���������� ������
    void clearTextBuf();

    // ������� ������
    void clearLine(uint8_t line);

    /// ����� ���������.
    void clrMessages();

    /** ���������� ���������.
     *
     *  @param[in] msg ���������.
     */
    void setMessage(msg_t msg);

    /** ������� ��������� �� �����
     *
     *  @return True ���� ��������� �������� �� �����.
     */
    bool printMessage();

    /** ��������� ������� ���������.
     *
     *  @return true ���� ���� ���������.
     */
    bool isMessage() const;

        // ����� �� ����� ����������� ���������
    void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

    // ����� �� ����� �������� ��������� ���������
    void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

    // ����� � ������ ���� "�����" �������� ������ ���������
    void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

    // ������ ����
    void lvlError();
    void lvlStart();
    void lvlFirst();
    void lvlInfo();
    void lvlJournal();
    void lvlJournalEvent();
    void lvlJournalDef();
    void lvlJournalPrm();
    void lvlJournalPrd();
    void lvlJournalSecurity();
    void lvlControl();
    void lvlSetup();
    void lvlRegime();
    void lvlSetupParam();
    void lvlSetupParamDef();
    void lvlSetupParamPrm();
    void lvlSetupParamPrd();
    void lvlSetupParamGlb();
    void lvlSetupParamRing();
    void lvlSetupDT();
    void lvlMeasure();
    void lvlTest();
    void lvlTest1();
    void lvlTest2();
    void lvlSetupInterface();
    void lvlUser();

    /** ���� ���������.
     *
     *  @return True - �� ���������
     */
    eMENU_ENTER_PARAM inputValue();

    // ����������� ������� �����
    void cursorLineUp() {
        cursorLine_=(cursorLine_>1)? cursorLine_-1 : Punkts_.getMaxNumPunkts();
    }

    // ���������� ������� ����
    void cursorLineDown() {
        cursorLine_=(cursorLine_<Punkts_.getMaxNumPunkts())? cursorLine_+1 : 1;
    }

    // ����� �� ����� ������� ������� ���� � �������
    void printPunkts();

    /// ����� �� ����� �������� ���������.
    void printParam();

    /// ����� �� ����� �������� ���������.
    void printName();

    /// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
    void printSameNumber(uint8_t pos);

    /// ����� �� ����� ��������� �������� ���������.
    void printRange(uint8_t pos);

    /** ������� ������ ��� ����� ������ ��������.
     *
     *  @param[in] start ��������� ������� ������ ��������.
     *  @param[in] stop �������� ������� ������ ��������.
     */
    void printCursor(uint8_t start, uint8_t stop);

    /** ����� �� ����� �������� �������� ���������.
     *
     *  ���� �������� ��������� �������� � �������, ����� ���������� ���������
     *  ��������������� ������� � ������� �������� ���������.
     *
     *  ��� ��������� ���������� ����������� ����������� ��������. �.�. ��
     *  �������� �������� ��������� ���������� ����������� ��������, � �����
     *  �������������� ����� �� ������� ����� �������� ���������.
     */
    void printValue(uint8_t pos);

    /** ������� �� ����� ����� � ���� ��� ������ �������.
     *
     *  @param[in] pos ��������� ������� �� ������.
     *  @return ���������� ������������ ��������.
     */
    uint8_t printDateAdnTimeInJournal(uint8_t pos);

    /** ��������� ������ ������ �� ����������� ��������� ���������.
     *
     *  @return True ���� ����� ������.
     */
    bool checkChangeReg() const;

    /** ��������� ��������� ������.
     *
     *  @param[in] user ���� ������������ ��� �������� �������� ������.
     *  @param[in] pwd ������ ��� ��������.
     *  @return true ���� ������ ������, ����� false.
     */
    bool checkPwdInput(user_t user, const uint8_t *pwd);

    /** �������� ������� �� ������� ���� ��� ���������� ������������
     *
     *  ������:
     *  - lvlJournalSecurity, ����� �������������
     *  - lvlTest1, ����� ������� ��� �������������;
     *  - lvlTest2, ����� ������� ��� �������������
     *
     *  � ������ ���� ������ �� �������, ����� ���������� ��� ���������.
     *
     *  @param[in] lvl ������� ����.
     *  @return
     */
    bool checkUserReq(void (clMenu::*lvl)());

    /** ��������� ������������� ����� ������ ����� ��������� ���������.
     *
     *  ������ ������ ����������:
     *  - ��� ����� ����, ����� ������ ���������.
     *
     *  @param[in] param ��������
     *  @param[in] value ������� ���������.
     *  @return ����������� ������������ ��� ���������.
     */
    user_t checkPwdReq(eGB_PARAM param, int16_t value) const;

    /** ��������� ���������� ��� ����� �������� � ����������.
     *
     *  ��� ���������� ���� \a Param::PARAM_NO ������� ����� �������� ��
     *  ����������.
     *
     *  ��� ���������� ���� \a Param::PARAM_LIST ���������� ������� ������
     *  �������� �� ������.
     *
     *  ��� ���������� ���� \a Param::PARAM_INT ���������� ������� �����
     *  ������ �����.
     *
     *  ��� ���������� ���� \a Param::PARAM_U_COR ���������� ������� �����
     *  ��������� ����������, � ����� ����������� �������� ����������� �� 0.
     *
     *  ��� ���������� ���� \a Param::PARAM_I_COR ���������� ������� ����� ������
     *  �����, ��� ���� ����������� �������� ����������� �� 0.
     *
     *  ��� ���������� ���� \a Param::PARAM_BITES ���������� ������� ������
     *  �������� �� ������.
     */
    void enterParameter();

    /// ������������ ������ ��������� ���������� ��� ������.
    void crtListOfParamsDef();

    /// ������������ ������ ��������� ���������� ��� ���������.
    void crtListOfParamsPrm();

    /// ������������ ������ ��������� ���������� ��� �����������.
    void crtListOfParamsPrd();

    /// ������������ ������ ��������� ���������� ��� �����.
    void crtListOfParamsGlb();

    // ������� ������� �����
    eMENU_ENTER_PARAM (clMenu::*enterFunc)();

    // ������� ������� ����
    void (clMenu::*lvlMenu)();

    /// ��������� ��������.
    void saveParam();

    /** ��������� �������� � ���.
     *
     *  ����������� ������� ��� �������� � ���.
     */
    void saveParamToBsp();

    /// �������� �������� � ����������� ������.
    void saveParamToRam();

    /// ������ ����� �������������� ������������.
    void security();

    /** ������ � ���� ��������� ����������.
     *
     *  ���� ������� ���������� ���������� ����� 0, �� ����� ����������� �������
     *  � ����������� ������.
     */
    void setupParam();    

    /** �������� ������������� � ���������.
     *
     *  ������������� ��������� ��������� ���������� ��� ������� ����������
     *  ����������.
     *
     *  ���:
     *  - ����� �� "������";
     *  - ������� ������������� ��� ��������������.
     *  ���:
     *  - ��������� �� "��������";
     *  - ������� ������������� ��� ��������������.
     *  ���:
     *  - ��������� �� "��� ��";
     *  - ������� ������������� ��� ��������������.
     *  ���:
     *  - ��������� �� "��� ��";
     *  - ������� ������������� ��� ��������������.
     *
     *  @retval True �������� ���������.
     *  @retval False ������������� � ��������� ���.
     */
    bool checkLedOn();
};

#endif /* MENU_H_ */
