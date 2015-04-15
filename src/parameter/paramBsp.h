/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "glbDefine.h"
#include "paramGlb.h"
#include "paramRps.h"
#include "paramUart.h"
#include "LocalParams.h"


/// ��������� ���������� ���
struct stGBparam {
	eGB_TYPE_DEVICE 	typeDevice;		///< ��� ����������

	TDataTime			DataTime;		///< ���� � �����
	TDeviceGlb 			Glb;			///< ��������� �����
	TDeviceRps 			Rps;			///< ��������� ���������
	TJournalEntry 		JrnEntry;		///< ������ �������
	TMeasuredParameters MeasParam;		///< ���������� ���������
	TPassword			Password;		///< ������
	TTxCom 				TxComBuf;		///< ����� ������
	TUartData 			Uart;			///< ��������� ����������������� �����

	LocalParams local;					///< ��������� ���������
};

#endif /* PARAMBSP_H_ */
