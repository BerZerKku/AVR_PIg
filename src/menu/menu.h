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

/// ����� ���� ����� ������ � ����, ��
#define MENU_TIME_CYLCE 200

/// ����� �� ������ ������������� �������, ��
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)
/// ����� �� ����������������� �������, ��
#define TIME_TO_REINIT_LCD (10000 / MENU_TIME_CYLCE)

/// ������������ ���-�� ������������ �� ������ ����������
#define MAX_NUM_MEAS_PARAM 11

/// ������������ ���-�� ������������ ���������� � ���� "���������"
#define MAX_NUM_MEAS_PARAM_LVL 6

/// ����� ������ ����������� ��������� �� �����, ��
#define TIME_MEAS_PARAM (3000 / MENU_TIME_CYLCE)

/// ����� ������ ���.��������� �� �����, ��
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)

/// ����� ������ ������ �� �����, �� (�������� ������/��� ������)
#define TIME_TEXT (1000 / MENU_TIME_CYLCE)

/// ����� �� ������ �� ���� 1 / ���� 2 � �.�. ���� ����� != ������� �����, ��
#define TIME_TEST_EXIT (1000 / MENU_TIME_CYLCE)

/// ���������� ���������
enum eMENU_MEAS_PARAM {
	MENU_MEAS_PARAM_NO,			///< ��� ����������� ���������
	MENU_MEAS_PARAM_DATE,		///< ����
	MENU_MEAS_PARAM_TIME,		///< �����
	MENU_MEAS_PARAM_UZ,			///< ����� �� ������
	MENU_MEAS_PARAM_UZ1,		///< ����� �� ������ 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UZ2,		///< ����� �� ������ 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UC,			///< ����� �� ��
	MENU_MEAS_PARAM_UC1,		///< ����� �� �� 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UC2,		///< ����� �� �� 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UN,			///< ������� �����
	MENU_MEAS_PARAM_UN1,		///< ������� ����� 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UN2,		///< ������� ����� 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UOUT,		///< ��������� ������
	MENU_MEAS_PARAM_IOUT,		///< ��� ������
	MENU_MEAS_PARAM_ROUT,		///< ������������� �����
	MENU_MEAS_PARAM_SD,			///< �������� � �������
	MENU_MEAS_PARAM_D,			///< ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
	MENU_MEAS_PARAM_MAX
};

/// ����� ������ ������� ����� ����������
enum eMENU_ENTER_PARAM {
	MENU_ENTER_PARAM_NO,		///< ������ ��������� ���������
	MENU_ENTER_PARAM_INT,		///< ��������� ���������, ����� ��������
	MENU_ENTER_PARAM_LIST,		///< ��������� ���������, ����� �� ������
	MENU_ENTER_PARAM_LIST_2,	///< ��������� ���������, ����� �� ������ ��������
	MENU_ENTER_PARAM_U_COR,		///< ��������� ���������, ��������� ����������
	MENU_ENTER_PASSWORD,		///< ���� ������
	MENU_ENTER_PASSWORD_NEW,	///< ���� ������ ������
	MENU_ENTER_PASSWORD_READY,	///< ������ ������ ������
	MENU_ENTER_PASSWORD_N_READY,///< ����� ������ ������ ���������
	MENU_ENTER_PARAM_READY,		///< ���������� �������� ��������
	MENU_ENTER_PARAM_MESSAGE,	///< ����� ��������� �� ����� ��� ����� ���������
};

// ��������� ���������� ��� ����� ��������
class TEnterParam {

public:
	TEnterParam() {
		setDisable();
	}

	/**	�������� �������� ������� ������ � ����������.
	 * 	@param ���
	 * 	@return True - ���� ���� ���� ������ ��������, ����� False.
	 */
	bool isEnable() {
		// �������� �������� ������� �� ����������� ��������
		if ((status_ < MENU_ENTER_PARAM_NO)
				|| (status_ > MENU_ENTER_PARAM_MESSAGE))
			status_ = MENU_ENTER_PARAM_NO;
		return (status_ != MENU_ENTER_PARAM_NO);
	}

	// ������ ������, ���������� ������ ����� ����������
	// ���� ���� �� ������, ������������� ������������ � �������� ����� 1
	// ��� ������ ������������� ������������ �������� ����� � ��������� ��������
	void setEnable(eMENU_ENTER_PARAM s = MENU_ENTER_PARAM_INT) {
		if ((s >= MENU_ENTER_PARAM_INT) && (s <= MENU_ENTER_PASSWORD_NEW)) {
			if ((s == MENU_ENTER_PARAM_LIST)
					|| (s == MENU_ENTER_PARAM_LIST_2)) {
				disc_ = 1;
				fract_ = 1;
			} else if ((s == MENU_ENTER_PASSWORD)
					|| (s == MENU_ENTER_PASSWORD_NEW)) {
				disc_ = 1;
				fract_ = 1;
			}
			status_ = s;
		}
	}

	/**	���������� ��������� ���������.
	 *
	 * 	@param ���
	 * 	@return ���
	 */
	void setDisable() {
		status_ = MENU_ENTER_PARAM_NO;
		cnt_ = TIME_MESSAGE;
		com = GB_COM_NO;
		disc_ = 1;
	}

	// �������� ��������
	void setValueRange(int16_t min, int16_t max) {
		max_ = max;
		min_ = min;
	}

	int16_t getValueMin() const {
		return min_;
	}
	int16_t getValueMax() const {
		return max_;
	}

	/** ��������� �������� ��������
	 * 	�������� �������� ������ ���� ����� �� !
	 *
	 * 	��� ���������� ���������� �� ������, ��������������� ����������
	 * 	��������, ���� �����������.
	 *
	 * 	@param val ������� ��������.
	 */
	void setValue(int16_t val) {
		if ((status_ == MENU_ENTER_PARAM_LIST) ||
				(status_ == MENU_ENTER_PARAM_LIST_2)) {
			if ((val < min_) || (val > max_)) {
				val = min_;
			}
			val_ = val;
		} else {
			val_ = 0;
		}
	}

	// ���������� ������� ��������
	int16_t getValue() const {
		return val_;
	}

	// ���������� �������� �������� � ������ ������������ � ��������
	int16_t getValueEnter() const {
		return ((val_ / disc_) * disc_) / fract_;
	}

	/**	���� ������ ������� ��������.
	 *
	 *	���� ���������� �� �������� �������. ����� ���� �������� �� ������������
	 *	���-�� �������� � �������� �� ������������ ��������.
	 *
	 *	� ������ ���� �������� ����� 0, ���� ��������� �������� �� �������.
	 *
	 * 	@param dig �����. �� 0 �� 9 ������������.
	 */
	void setDigit(uint8_t dig) {
		eMENU_ENTER_PARAM s = status_;

		if ((s == MENU_ENTER_PARAM_INT) || (s == MENU_ENTER_PARAM_U_COR) ||
				(s == MENU_ENTER_PASSWORD) || (s == MENU_ENTER_PASSWORD_NEW)) {

			int16_t tmp = val_ * 10 + dig;

			if (tmp <= max_) {
				val_ = tmp;
			}
		}
	}

	/** �������� ���������� �������.
	 *
	 * 	��������� ��������� �������� ������.
	 */
	void delDigit() {
		eMENU_ENTER_PARAM s = status_;

		if ((s == MENU_ENTER_PARAM_INT) || (s == MENU_ENTER_PARAM_U_COR) ||
				(s == MENU_ENTER_PASSWORD) || (s == MENU_ENTER_PASSWORD_NEW)) {

			if (val_ > 0) {
				val_ /= 10;
			}
		}
	}

	/** ���������� �������� ��������.
	 *
	 * 	�������� ������ ���������� �����.
	 */
	void incValue() {
		eMENU_ENTER_PARAM s = status_;

		if ((s == MENU_ENTER_PARAM_LIST) ||
				(s == MENU_ENTER_PARAM_LIST_2)) {
			// � ������ ������� �������� (���������� ������� �������)
			val_ = (val_ > min_) ? val_ - 1 : max_;
		}
	}

	/**	���������� �������� ��������.
	 *
	 * 	�������� ������ ���������� ����.
	 */
	void decValue(uint8_t velocity=0) {
		eMENU_ENTER_PARAM s = status_;

		if ((s == MENU_ENTER_PARAM_LIST)
				|| (s == MENU_ENTER_PARAM_LIST_2)) {
			// � ������ ������� �������� (�������� ������� �������)
			val_ = (val_ < max_) ? val_ + 1 : min_;
		}
	}

	// ������/���������� ��������������� �����
	void setDopValue(uint16_t byte) {
		dopValue_ = byte;
	}
	uint16_t getDopValue() const {
		return dopValue_;
	}

	// ������/���������� ������������
	void setDisc(uint16_t disc) {
		disc_ = disc;
	}
	uint16_t getDisc() const {
		return disc_;
	}

	// ������/���������� ��������
	void setFract(uint8_t fract) {
		fract_ = fract;
	}
	uint8_t getFract() const {
		return fract_;
	}

	// ����� ��������� �� �����
	// �� ��������� �������� ��� ������� ����� ���������
	void printMessage() {
		status_ = MENU_ENTER_PARAM_MESSAGE;
		cnt_ = 0;
	}

	// ���������� ��������� �������
	eMENU_ENTER_PARAM getStatus() const {
		return status_;
	}

	// ��������� ����� ��������� ����� ���������
	void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY) {
		if ((val_ >= min_) && (val_ <= max_)) {
			status_ = status;
		} else {
			status_ = MENU_ENTER_PARAM_NO;
		}
	}

	// ��������� �� ������ ������� ������
	PGM_P list;

	// ��������� �� ������ ��������
	uint8_t *listValue;

	// ������� �� ��������
	eGB_COM com;

	// ������� �������
	uint8_t cnt_;

private:
	// ������� ��������
	int16_t val_;

	// ������������ ��������
	int16_t max_;

	// ����������� ��������
	int16_t min_;

	// ���� � ������������� �����������
	uint16_t dopValue_;

	// ������������
	uint16_t disc_;

	// ��������
	uint8_t fract_;

	// ������� ������ ����������
	eMENU_ENTER_PARAM status_;
};

/// ��������� ������ ����
class TMenuPunkt {
	/// ������������ ���-�� ������� � ����
	static const uint8_t  MAX_NUM_PUNKTS = 20;
public:
	/**	�����������.
	 */
	TMenuPunkt() {
		clear();

		for (uint_fast8_t i = 0; i < MAX_NUM_PUNKTS; i++)
			com_[i] = GB_COM_NO;
	}

	/**	���������� ������ ����, ����� ���������.
	 * 	@param name ��� ������.
	 * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
	 */
	bool add(PGM_P name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			name_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/**	���������� ������ ����, ����� ����� �������� �������.
	 *
	 * 	@param name	����� ������ (���).
	 * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
	 */
	bool add(uint8_t name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			number_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/** ������� �������� ������ ������� ����.
	 */
	void clear() {
		cnt_ = 0;
	}

	/** ������ ���������� ������ ������ ����.
	 * 	@param name ��� �����.
	 * 	@param com ������� ��� ������� �� ���.
	 * 	@param num ����� ������.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, �� ������ �����.
	 */
	bool change(PGM_P name, eGB_COM com, uint8_t num) {
		bool stat = false;
		if (num < cnt_) {
			name_[num] = name;
			com_[num] = com;
			stat = true;
		}
		return stat;
	}

	/** ���������� �������� ��������� ������ ����.
	 *	@param num	����� ������.
	 * 	@return �������� ������.
	 */
	PGM_P getName(uint8_t num) {
//		if (num >= cnt_)
//			num = 0;
//		return name_[num];
		return ((num < cnt_) ? name_[num] : 0);
	}

	/**	���������� ����� ��������� ������� � ������ ���������� ������ ����.
	 *	@param num	����� ������.
	 * 	@return �������� ������.
	 */
	uint8_t getNumber(uint8_t num) {
		return ((num < cnt_) ? number_[num] : 0);
	}

	/** ���������� ������� ���������� ������ ����.
	 * 	@param num ����� ������.
	 * 	@return ������� ��� ������� �� ���.
	 */
	eGB_COM getCom(uint8_t num) {
		if (num >= cnt_)
			num = 0;
		return com_[num];
	}

	/** ���������� ������� ���-�� ������ ����.
	 * 	@return ������� ���-�� ������� ����.
	 * 	@retval 0 - �����.
	 */
	uint8_t getMaxNumPunkts() {
		return cnt_;
	}

private:
	/// ������� ���-�� �������
	uint8_t cnt_;
	/// ��������� �� ��� ������
	PGM_P name_[MAX_NUM_PUNKTS];
	/// ����� ������, ������������ � ���������
	uint8_t number_[MAX_NUM_PUNKTS];
	/// ������� ��� ������� �� ���, ����������� ��� ������� ������ ����
	eGB_COM com_[MAX_NUM_PUNKTS];
};

// ����� ����
class clMenu {

	/// ���������� ����������� �� ������ ���������� ����������
	static const uint8_t NUM_VIEW_PARAM = 2;

public:

	/**	�����������.
	 * 	���������������� �� ��������� �������������� ��� ��������.
	 * 	����������� ����������� ��� ������ �������.
	 * 	������������ ������� �� ��������� ������� ����.
	 */
	clMenu();

	/**	������ � ����.
	 */
	void main();

	/** ��������� ���� �������� � ��������� ���� � ��� ������.
	 * 	�� ��������� ����� ������ ����� ������ �� �������� ���������� �
	 * 	���������� ��������� ���������.
	 * 	@param device ��� ����������
	 * 	@return False � ������ ������, ����� True
	 */
	bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

	/**	���������� ��������� ������� �� ����������.
	 * 	������� ����������� ������� �������, ���� �� ��� ���� ������� �������.
	 * 	������ ���� (��������� �� 4-� �������) ������������:
	 * 	������� ���������
	 * 	������� �� ������ 1
	 * 		������ ������ ���� ������� ��������, ���� ����
	 * 		����� ������� ����������� ��� ��������� ���� (������������� � �.�.)
	 * 		���� ������ � ������ ���, �� ���������� ������� �� ������ 2
	 * 	������� �� ������ 2
	 * 		������� ������ ��������� ���������� � ������ ������ ����
	 * 	@param ���
	 * 	@return �������
	 */
	eGB_COM getTxCommand();

	/** ���������� ������� ��������� ����� � ���.
	 * 	@retval True - ����� � ��� ����.
	 * 	@retval False - ����� � ��� ���.
	 */
	bool isConnectionBsp() const {
		return connectionBsp_;
	}

	/** ��������� ����� ������� ����� � ���.
	 * 	@retval True - ����� ����.
	 * 	@retval False - ����� ���.
	 */
	void setConnectionBsp(bool f) {
		connectionBsp_ = f;
	}

	/**	������ ���������� �������� ������������ �� ������.
	 *
	 *	� ������ ���������� ���������, ��� ������ �������� ������� �����
	 *	����������� ����, ��� �������� �����.
	 *
	 * 	@param num ������� ������������� ���������.
	 * 	@arg 1 ����� ��������.
	 * 	@arg 2 ������ ��������.
	 * 	@param param ������������ ��������.
	 */
	void setViewParam(uint8_t num, eMENU_MEAS_PARAM param);

	/**	���������� �������� ������������� ���������.
	 *
	 *	���� ����� ��������� ������ �� �����, ����� ���������� ��������
	 *	\a MENU_MEAS_PARAM_NO.
	 *
	 * 	@param num ������� ������������� ���������.
	 * 	@param 1 ����� ��������.
	 * 	@param 2 ������ ��������.
	 * 	@return	������������ ��������.
	 *
	 */
	eMENU_MEAS_PARAM getViewParam(uint8_t num);

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

	// ���� �������� ���������� ����� � ���, True - ����
	bool connectionBsp_;

	// true - ���������� ������� �� ����� ������
	bool cursorEnable_;

	// true - ����� �� ����� ��������� ��������
	bool viewRange_;

	// ������� ��������� ������� (����� ������)
	uint8_t cursorLine_;

	// ���-�� ������������ ����������
	uint8_t lineParam_;

	// ������� �������� (�������� ����� ������� ������� ���������, ������� � 1)
	uint8_t curCom_;

	// ���� true - ���������� ������� ������� ����
	bool lvlCreate_;

	// ����� ������ ���.��������� �� ����� (�������� ��������� �� ������)
	uint8_t delay_;

	// ���-�� ���������� ���������� � ������� ������ ��������
	uint8_t maxViewParam_;

	// ���������� ��������
	eMENU_MEAS_PARAM viewParam_[NUM_VIEW_PARAM];

	// ���������� ���������
	eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM];

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

	// ����� ��������� �� �����
	void printMessage() {
		delay_ = 0;
	}

	// ���������� true - � ������ ������������� ������ ���������
	bool isMessage() const {
		return (delay_ < TIME_MESSAGE);
	}

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
	void lvlControl();
	void lvlSetup();
	void lvlRegime();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupDT();
	void lvlMeasure();
	void lvlTest();
	void lvlTest1();
	void lvlTest2();
	void lvlSetupInterface();

	// ���� ���������
	eMENU_ENTER_PARAM enterValue();

	eMENU_ENTER_PARAM enterPassword();

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

	///	����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
	void printSameNumber(uint8_t pos);

	///	����� �� ����� ��������� �������� ���������.
	void printRange(uint8_t pos);

	/**	����� �� ����� �������� �������� ���������.
	 *
	 * 	���� �������� ��������� �������� � �������, ����� ���������� ���������
	 * 	��������������� ������� � ������� �������� ���������.
	 *
	 * 	��� ��������� ���������� ����������� ����������� ��������. �.�. ��
	 * 	�������� �������� ��������� ���������� ����������� ��������, � �����
	 * 	�������������� ����� �� ������� ����� �������� ���������.
	 */
	void printValue(uint8_t pos);

	/**	��������� ���������� ��� ����� �������� � ����������.
	 *
	 *	��� ���������� ���� \a Param::PARAM_NO ������� ����� �������� ��
	 *	����������.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_LIST ���������� ������� ������
	 * 	�������� �� ������.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_INT ���������� ������� �����
	 * 	������ �����.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_U_COR ���������� ������� �����
	 * 	��������� ����������, � ����� ����������� �������� ����������� �� 0.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_I_COR ���������� ������� ����� ������
	 * 	�����, ��� ���� ����������� �������� ����������� �� 0.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_BITES ���������� ������� ������
	 * 	�������� �� ������.
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

	// �������� ������ ���������� ������������ �� ������.
	void scrollViewParam(uint8_t num, int8_t dir=1);

	// �������� ������������ ������������ ����������.
	void checkViewParams();

	// ������� ������� ����
	void (clMenu::*lvlMenu)();

	/** ������ � ���� ��������� ����������.
	 *
	 * 	���� ������� ���������� ���������� ����� 0, �� ����� ����������� �������
	 * 	� ����������� ������.
	 */
	void setupParam();
};

#endif /* MENU_H_ */
