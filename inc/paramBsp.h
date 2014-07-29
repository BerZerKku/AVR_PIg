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
#include "paramUart.h"


/// Структура параметров БСП
struct stGBparam {
	eGB_TYPE_DEVICE typeDevice;

	// пароль
	TPassword password;

	// дата/время
	TDataTime dataTime;

	// измеряемые параметры
	TMeasuredParameters measParam;

	// общие параметры
	TDeviceGlb glb;

	// параметры для работы с последовательным портом
	TUartData Uart;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;
};

#endif /* PARAMBSP_H_ */
