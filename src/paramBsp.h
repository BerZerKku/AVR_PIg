/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "glbDefine.h"
#include "paramPrm.h"
#include "paramPrd.h"
#include "paramDef.h"
#include "paramGlb.h"
#include "paramUart.h"


/// ��������� ���������� ���
struct stGBparam {
	// false - �������� ��� ���� ��������� ���� ��� ������� ��� ��������
	bool device;
	eGB_TYPE_DEVICE typeDevice;

	// ������
	TPassword password;

	// ����/�����
	TDataTime dataTime;

	// ���������� ���������
	TMeasuredParameters measParam;

	// ������� ��������� ���������
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// ��������� ��� ������ � ���������������� ������
	TUartData Uart;

	// ����� ������
	TTxCom txComBuf;

	// ������ � �������
	TJournalEntry jrnEntry;

	// �����
	TTest test;
};

#endif /* PARAMBSP_H_ */
