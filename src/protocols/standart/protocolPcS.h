/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCS_H_
#define PROTOCOLPCS_H_

#include "protocolS.h"

class clProtocolPcS : public clProtocolS {

    /// ���� ��� ������� �������������
    enum error_t {
        ERROR_noAccess = 1,     ///< ��� ������� (�������������� ������������).
        ERROR_noConnectionBsp,  ///< ��� ����� � ���.
    };

public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/// ��������� ��������� ���������. � ������ ������� ���������� False.
	bool getData();

	/**	��������� ������� ������ �������� ������������ �� ��� �� ��.
	 *
	 *	� ������� ����������� ������ �������� ���-��. ���� ���-�� ���� ������
	 *	������, ��� ����������, �.�. ��� ������� ��� ��������, ����������
	 *	���������� ���-�� ���� ������ �� ������������. ��� ����� ����� ������,
	 *	�� ����������� ����� ��������, ����� 0�00.
	 *
	 *	@retval True - ������� GB_COM_GET_VERS, ��������� �������.
	 *	@retval False - ������ ������� ��� ��������� �� �������.
	 */
	bool modifyVersionCom();

private:

    /** ��������� �������� ������� ������ ������������.
     *
     *  @param[in] com �������.
     */
    bool hdlrComGetUser(eGB_COM com);

    /** ��������� �������� ������� ������ ������������.
     *
     *  @param[in] com �������.
     */
    bool hdlrComSetUser(eGB_COM com);

};


#endif /* PROTOCOLPC_H_ */
