/*
 * protocolPcI.h
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCI_H_
#define PROTOCOLPCI_H_

#include "CIec101.h"
#include "glbDefine.h"
#include "paramBsp.h"
#include "debug.h"

#ifdef AVR
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
	#define pgm_read_byte(a) (*a)
#endif

/**	\defgroup Notation ����� ����������.
 *
 *	�������:
 *	- b - ������ ����������, bError;
 *	- cl - �����, clIec101;
 *	- e - ������������, eTypeId;
 *	- i8 - ������������� ���������� �������� 8 ���, i8Error;
 *	- i16 - ������������� ���������� �������� 16 ���, i16Error;
 *	- p - ���������, pArray;
 *	- r - ������, rArray;
 *	- s - ������ � RAM, sDate;
 *	- sf - ������ � FLASH, sfDate;
 *	- st - ���������, SData;
 *	- u - ����������� union, uAsdu;
 *	- u8 - ����������� ���������� �������� 8 ���, u8Size;
 *	- u16 - ����������� ���������� �������� 8 ���, u16Size;
 *	- v - ���������� ���� void, vReserved.
 *
 *	�������:
 *	- c_ - ����������� ���������� �������, c_u8Address.
 *	- g_ - ���������� ����������, g_u8Address;
 *	- m_ - ����������-����, m_u8Address;
 *	- s_ - ����������� ����������-����, s_u8AddressMax;
 *
 *	�������:
 *	- C - �����, CIec101;
 *	- E - ������������, ECot;
 *	- S - ���������, SDate;
 *	- T - ����� ��� ������ (typedef);
 *	- U - �����������, UAsdu.
 *
 *	����� ��������� ���������� ������ ��� ������ ���������.
 */

class TProtocolPcI : public CIec101{

//	/// �������� ����������
//	typedef enum __attribute__ ((__packed__)) {
//		IE_MIN				= 200,	///< ����������� ������������ ����� - 1.
//		// ^^^ ������ � ������.
//		// ����� ��������
//		IE_ERROR			= 201,	///< ���� ������� ������������� � ��������.
//		IE_WARNING 			= 202,	///< ���� ������� �������������� � ��������.
//		IE_PRD_COM			= 203,	///< ���� ������� ������� �����������.
//		IE_PRM_COM			= 204,	///< ���� ������� ������� ���������.
//		// ����� ����� �������������� � ��������������
//		IE_GLB_ERROR_START 	= 301,	///< ��������� ����� ����� ��������������.
//		IE_GLB_ERROR_END	= 316,	///< �������� ����� ����� ��������������.
//		IE_GLB_WARNING_START= 317,	///< ��������� ����� ����� ��������������.
//		IE_GLB_WARNING_END	= 332,	///< �������� ����� ����� ��������������.
//		// ����� �����������
//		IE_PRD_ON			= 400,	///< ���� ������� ����������� � ��������.
//		IE_PRD_ERROR_START	= 401,	///< ��������� ����� �������������� �����������.
//		IE_PRD_ERROR_END	= 416,	///< �������� ����� �������������� �����������.
//		IE_PRD_WARNING_START= 417,	///< ��������� ����� �������������� �����������.
//		IE_PRD_WARNING_END	= 432,	///< �������� ����� �������������� �����������.
//		IE_PRD_COM_START	= 450,	///< ��������� ����� ������ �����������.
//		IE_PRD_COM_END		= 481,	///< �������� ����� ������ �����������.
//		IE_PRD_DISABLED		= 485,	///< ���� ������ "�������"
//		IE_PRD_ENABLED		= 486,	///< ���� ������ "������"
//		IE_PRD_TEST			= 487,	///< ���� ������ "����"
//		// ����� ���������
//		IE_PRM_ON			= 500,	///< ���� ������� ��������� � ��������.
//		IE_PRM_ERROR_START	= 501,	///< ��������� ����� �������������� ���������.
//		IE_PRM_ERROR_END	= 516,	///< �������� ����� �������������� ���������.
//		IE_PRM_WARNING_START= 517,	///< ��������� ����� �������������� ���������.
//		IE_PRM_WARNING_END	= 532,	///< �������� ����� �������������� ���������.
//		IE_PRM_COM_START	= 550,	///< ��������� ����� ������ ���������.
//		IE_PRM_COM_END		= 581,	///< �������� ����� ������ ���������.
//		IE_PRM_DISABLED		= 585,	///< ���� ������ "�������"
//		IE_PRM_READY		= 586,	///< ���� ������ "�����"
//		IE_PRM_ENABLED		= 587,	///< ���� ������ "������"
//		IE_PRM_TEST			= 588,	///< ���� ������ "����"
//		// ����� ������
//		IE_DEF_ON			= 600,	///< ���� ������� ������ � ��������.
//		IE_DEF_ERROR_START	= 601,	///< ��������� ����� �������������� ������.
//		IE_DEF_ERROR_END	= 616,	///< �������� ����� �������������� ������.
//		IE_DEF_WARNING_START= 617,	///< ��������� ����� �������������� ������.
//		IE_DEF_WARNING_END	= 632,	///< �������� ����� �������������� ������.
//		// TODO �������� ������� ������
//		//		IE_DEF_PUSK			= 650,	///< ������ ����.
//		//		IE_DEF_STOP			= 652,	///< ������ ����.
//		//		IE_DEF_MAN			= 654,	///< ������ �����������.
//		//		IE_DEF_PRD			= 656,	///< ������ ���.
//		//		IE_DEF_PRM			= 658,	///< ������ ���.
//		//		IE_DRF_OUT			= 670,	///< ������ ��-�����.
//		//		IE_DEF_DISABLED		= 675,	///< ���� ������ "�������"
//		//		IE_DEF_ENABLED		= 676,	///< ���� ������ "������"
//		//		IE_DEF_TEST			= 677,	///< ���� ������ "����"
//		// vvv ������ � �����.
//		IE_MAX						///< ������������ ������������ ����� + 1.
//	} EInfoElement;

	/// ������ ��������� ���������� ������ 2.
	typedef enum __attribute__ ((__packed__)) {
		IE2_ERROR = 0,
		IE2_WARNING,
		IE2_PRD_COM,
		IE2_PRM_COM,
		IE2_GLB_ERROR_H0001,
		IE2_GLB_ERROR_H0002,
		IE2_GLB_ERROR_H0004,
		IE2_GLB_ERROR_H0008,
		IE2_GLB_ERROR_H0010,
		IE2_GLB_ERROR_H0020,
		IE2_GLB_ERROR_H0040,
		IE2_GLB_ERROR_H0080,
		IE2_GLB_ERROR_H0100,
		IE2_GLB_ERROR_H0200,
		IE2_GLB_ERROR_H0400,
		IE2_GLB_ERROR_H0800,
		IE2_GLB_ERROR_H1000,
		IE2_GLB_ERROR_H2000,
		IE2_GLB_ERROR_H4000,
		IE2_GLB_ERROR_H8000,
		IE2_GLB_WARNING_H0001,
		IE2_GLB_WARNING_H0002,
		IE2_GLB_WARNING_H0004,
		IE2_GLB_WARNING_H0008,
		IE2_GLB_WARNING_H0010,
		IE2_GLB_WARNING_H0020,
		IE2_GLB_WARNING_H0040,
		IE2_GLB_WARNING_H0080,
		IE2_GLB_WARNING_H0100,
		IE2_GLB_WARNING_H0200,
		IE2_GLB_WARNING_H0400,
		IE2_GLB_WARNING_H0800,
		IE2_GLB_WARNING_H1000,
		IE2_GLB_WARNING_H2000,
		IE2_GLB_WARNING_H4000,
		IE2_GLB_WARNING_H8000,
		IE2_PRD_ON,
		IE2_PRD_ERROR_H0001,
		IE2_PRD_ERROR_H0002,
		IE2_PRD_ERROR_H0004,
		IE2_PRD_ERROR_H0008,
		IE2_PRD_ERROR_H0010,
		IE2_PRD_ERROR_H0020,
		IE2_PRD_ERROR_H0040,
		IE2_PRD_ERROR_H0080,
		IE2_PRD_ERROR_H0100,
		IE2_PRD_ERROR_H0200,
		IE2_PRD_ERROR_H0400,
		IE2_PRD_ERROR_H0800,
		IE2_PRD_ERROR_H1000,
		IE2_PRD_ERROR_H2000,
		IE2_PRD_ERROR_H4000,
		IE2_PRD_ERROR_H8000,
		IE2_PRD_WARNING_H0001,
		IE2_PRD_WARNING_H0002,
		IE2_PRD_WARNING_H0004,
		IE2_PRD_WARNING_H0008,
		IE2_PRD_WARNING_H0010,
		IE2_PRD_WARNING_H0020,
		IE2_PRD_WARNING_H0040,
		IE2_PRD_WARNING_H0080,
		IE2_PRD_WARNING_H0100,
		IE2_PRD_WARNING_H0200,
		IE2_PRD_WARNING_H0400,
		IE2_PRD_WARNING_H0800,
		IE2_PRD_WARNING_H1000,
		IE2_PRD_WARNING_H2000,
		IE2_PRD_WARNING_H4000,
		IE2_PRD_WARNING_H8000,
		IE2_PRD_COM_01,
		IE2_PRD_COM_02,
		IE2_PRD_COM_03,
		IE2_PRD_COM_04,
		IE2_PRD_COM_05,
		IE2_PRD_COM_06,
		IE2_PRD_COM_07,
		IE2_PRD_COM_08,
		IE2_PRD_COM_09,
		IE2_PRD_COM_10,
		IE2_PRD_COM_11,
		IE2_PRD_COM_12,
		IE2_PRD_COM_13,
		IE2_PRD_COM_14,
		IE2_PRD_COM_15,
		IE2_PRD_COM_16,
		IE2_PRD_COM_17,
		IE2_PRD_COM_18,
		IE2_PRD_COM_19,
		IE2_PRD_COM_20,
		IE2_PRD_COM_21,
		IE2_PRD_COM_22,
		IE2_PRD_COM_23,
		IE2_PRD_COM_24,
		IE2_PRD_COM_25,
		IE2_PRD_COM_26,
		IE2_PRD_COM_27,
		IE2_PRD_COM_28,
		IE2_PRD_COM_29,
		IE2_PRD_COM_30,
		IE2_PRD_COM_31,
		IE2_PRD_COM_32,
		IE2_PRD_DISABLED,
		IE2_PRD_ENABLED,
		IE2_PRD_TEST,
		IE2_PRM_ON,
		IE2_PRM_ERROR_H0001,
		IE2_PRM_ERROR_H0002,
		IE2_PRM_ERROR_H0004,
		IE2_PRM_ERROR_H0008,
		IE2_PRM_ERROR_H0010,
		IE2_PRM_ERROR_H0020,
		IE2_PRM_ERROR_H0040,
		IE2_PRM_ERROR_H0080,
		IE2_PRM_ERROR_H0100,
		IE2_PRM_ERROR_H0200,
		IE2_PRM_ERROR_H0400,
		IE2_PRM_ERROR_H0800,
		IE2_PRM_ERROR_H1000,
		IE2_PRM_ERROR_H2000,
		IE2_PRM_ERROR_H4000,
		IE2_PRM_ERROR_H8000,
		IE2_PRM_WARNING_H0001,
		IE2_PRM_WARNING_H0002,
		IE2_PRM_WARNING_H0004,
		IE2_PRM_WARNING_H0008,
		IE2_PRM_WARNING_H0010,
		IE2_PRM_WARNING_H0020,
		IE2_PRM_WARNING_H0040,
		IE2_PRM_WARNING_H0080,
		IE2_PRM_WARNING_H0100,
		IE2_PRM_WARNING_H0200,
		IE2_PRM_WARNING_H0400,
		IE2_PRM_WARNING_H0800,
		IE2_PRM_WARNING_H1000,
		IE2_PRM_WARNING_H2000,
		IE2_PRM_WARNING_H4000,
		IE2_PRM_WARNING_H8000,
		IE2_PRM_COM_01,
		IE2_PRM_COM_02,
		IE2_PRM_COM_03,
		IE2_PRM_COM_04,
		IE2_PRM_COM_05,
		IE2_PRM_COM_06,
		IE2_PRM_COM_07,
		IE2_PRM_COM_08,
		IE2_PRM_COM_09,
		IE2_PRM_COM_10,
		IE2_PRM_COM_11,
		IE2_PRM_COM_12,
		IE2_PRM_COM_13,
		IE2_PRM_COM_14,
		IE2_PRM_COM_15,
		IE2_PRM_COM_16,
		IE2_PRM_COM_17,
		IE2_PRM_COM_18,
		IE2_PRM_COM_19,
		IE2_PRM_COM_20,
		IE2_PRM_COM_21,
		IE2_PRM_COM_22,
		IE2_PRM_COM_23,
		IE2_PRM_COM_24,
		IE2_PRM_COM_25,
		IE2_PRM_COM_26,
		IE2_PRM_COM_27,
		IE2_PRM_COM_28,
		IE2_PRM_COM_29,
		IE2_PRM_COM_30,
		IE2_PRM_COM_31,
		IE2_PRM_COM_32,
		IE2_PRM_DISABLED,
		IE2_PRM_READY,
		IE2_PRM_ENABLED,
		IE2_PRM_TEST,
		IE2_DEF_ON,
		IE2_DEF_ERROR_H0001,
		IE2_DEF_ERROR_H0002,
		IE2_DEF_ERROR_H0004,
		IE2_DEF_ERROR_H0008,
		IE2_DEF_ERROR_H0010,
		IE2_DEF_ERROR_H0020,
		IE2_DEF_ERROR_H0040,
		IE2_DEF_ERROR_H0080,
		IE2_DEF_ERROR_H0100,
		IE2_DEF_ERROR_H0200,
		IE2_DEF_ERROR_H0400,
		IE2_DEF_ERROR_H0800,
		IE2_DEF_ERROR_H1000,
		IE2_DEF_ERROR_H2000,
		IE2_DEF_ERROR_H4000,
		IE2_DEF_ERROR_H8000,
		IE2_DEF_WARNING_H0001,
		IE2_DEF_WARNING_H0002,
		IE2_DEF_WARNING_H0004,
		IE2_DEF_WARNING_H0008,
		IE2_DEF_WARNING_H0010,
		IE2_DEF_WARNING_H0020,
		IE2_DEF_WARNING_H0040,
		IE2_DEF_WARNING_H0080,
		IE2_DEF_WARNING_H0100,
		IE2_DEF_WARNING_H0200,
		IE2_DEF_WARNING_H0400,
		IE2_DEF_WARNING_H0800,
		IE2_DEF_WARNING_H1000,
		IE2_DEF_WARNING_H2000,
		IE2_DEF_WARNING_H4000,
		IE2_DEF_WARNING_H8000,
		IE2_MAX
	} EInfoElement2;

	///	\brief ������� ����������.
	typedef struct __attribute__ ((__packed__)) {
		bool send;		///< True - ������ ������ ��� ��������.
		bool val;		///< ��������.
		uint16_t adr;	///< �����.
	} SEI;

public:
	TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	������� �������� ���������.
	 *
	 * 	@return ���-�� ���� ������ ��� ��������.
	 */
	uint8_t send();

	/**	�������� ������� ������ ������ 1(2) �� ��������.
	 *
	 *	� ������ ������� �����������:
	 *	- ��������� IE_ERROR;
	 *	- ��������� IE_WARNING;
	 *	- ��������� IE_PRD_COM;
	 *	- ��������� IE_PRM_COM;
	 *	- TODO ����������� ������� ������ ��� ��������.
	 *	- TODO ����������� �������� �������� � ��� ������������� �����������
	 *	������ � ���.
	 *
	 * 	@retval False ��� ������ �� ��������.
	 * 	@retval True ���� ������ �� ��������.
	 */
	virtual bool checkEvent();

	/**	�������� �������.
	 *
	 * 	� ������ ������� ������������:
	 * 	- ��������� IE_ERROR;
	 *	- ��������� IE_WARNING;
	 *	- ��������� IE_PRD_COM;
	 *	- ��������� IE_PRM_COM;
	 *
	 * 	@retval True ������������ ������ ��� ��������.
	 *	@retval False ������ ���.
	 *
	 */
	virtual bool procEvent();



private:

	/// ��������� ����������.
	stGBparam * const sParam_;

	/// ��������� ��������� ���������� (EInfoElement).
	bool m_flags[IE2_MAX];

	/// ������ ��������� ���������� (EInfoElement).
	static const uint16_t c_adrIE2[IE2_MAX];

	/// ������� ���������� ������ 2 ��� ��������.
	SEI ei2;

	/**	��������� ������ �� ������� ������.
	 *
	 *	��� ���������� ������ ���������������� ���� C_IC_NA_1 � ������������ \a
	 *	True, ����� False.
	 *
	 *	@param adr[out] ����� �����.
	 *	@param val[out] �������� �����.
	 *	@retval True ������������ ������ ��� ��������.
	 *	@retval False ������ ���.
	 */
	virtual bool procInterrog(uint16_t &adr, bool &val);

	/**	��������� �������.
	 *
	 *	���������� �������������, ��� ��������� ����� ������������� �������.
	 *
	 *	���������������� ������� ��� �������� ������� � ���.
	 *
	 * 	@retval True ����� �����������.
	 * 	@retval False ����� �� �����������.
	 */
	virtual bool procSetTime();

	/**	��������� �������, ��������� �� ���������.
	 *
	 * 	���������� ������������� ��� �������� ������� ������������ �����, ������
	 * 	����� ������������� �������.
	 *
	 * 	���� �� ����� ���� ������� ���� ������������� �������, ����� �����������
	 * 	���� ��������� �� ���������.
	 *
	 * 	@retval True ���� ��� ������� ����� �� ��� � �������� �� ����� �������������.
	 * 	@retval False ���� ������ ������.
	 */
	virtual bool procSetTimeEnd();

	/**	���������� ��������� �������� ����������.
	 *
	 * 	@param[in] ei ������� ����������.
	 * 	@retval True ���� ����������.
	 * 	@retval False ���� �������.
	 *
	 */
	bool getValue(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ���������� ��������.
	 *
	 * 	@param[in] ei ������� ����������.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getDevice(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ����� ����������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getGlb(EInfoElement2 ei) const;

	/**	���������� ��������� ����� �����������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getPrd(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ���������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getPrm(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getDef(EInfoElement2 ei) const;
};

#endif /* PROTOCOLPCI_H_ */
