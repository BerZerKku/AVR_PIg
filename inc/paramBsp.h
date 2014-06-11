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


/// —труктура параметров Ѕ—ѕ
struct stGBparam {
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;
	eGB_TYPE_DEVICE typeDevice;

	// пароль
	TPassword password;

	// дата/врем€
	TDataTime dataTime;

	// измер€емые параметры
	TMeasuredParameters measParam;

	// текущее состо€ние устройств
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// параметры дл€ работы с последовательным портом
	TUartData Uart;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;

	// тесты
	TTest test;
};

#endif /* PARAMBSP_H_ */
