/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "dateTime.hpp"
#include "glbDefine.h"
#include "journalEntry.hpp"
#include "jrnScada.hpp"
#include "LocalParams.h"
#include "paramPrm.h"
#include "paramPrd.h"
#include "paramDef.h"
#include "paramGlb.h"
#include "paramUart.h"
#include "txCom.hpp"
#include "infosecurity.h"

/// ��������� ���������� ���
struct stGBparam {
	// false - �������� ��� ���� ��������� ���� ��� ������� ��� ��������
	bool device;

	// ��� ��������
	eGB_TYPE_DEVICE typeDevice;

	// ��� ����������� ��������
	eGB_TYPE_OPTO typeOpto;

	// ����/�����
	TDateTime DateTime;

	// ����/����� ��� � ������ ��������� ����� �������
	TDateTime DateTimeReq;

	// ���������� ���������
	TMeasuredParameters measParam;

	// ������� ��������� ���������
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// ��������� ���������
	LocalParams local;
    // ���������� ����������
    save_t save;

	// ��������� ��� ������ � ���������������� ������
	TUartData Uart;

    // ��������� ��� ������������� ������������
    TInfoSecurity security;

	// ����� ������
	TTxCom txComBuf;

	// ������ � �������
	TJournalEntry jrnEntry;

	// ������ ��� SCADA
	TJrnSCADA jrnScada;

	// �����
	TTest test;
};

#endif /* PARAMBSP_H_ */
