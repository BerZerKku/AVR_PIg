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


/// Структура параметров БСП
struct stGBparam {
	eGB_TYPE_DEVICE 	typeDevice;		///< Тип устройства

	TDataTime			DataTime;		///< Дата и время
	TDeviceGlb 			Glb;			///< Параметры общие
	TDeviceRps 			Rps;			///< Параметры Терминала
	TJournalEntry 		JrnEntry;		///< Запись журнала
	TMeasuredParameters MeasParam;		///< Измеряемые параметры
	TPassword			Password;		///< Пароль
	TTxCom 				TxComBuf;		///< Буфер команд
	TUartData 			Uart;			///< Параметры последовательного порта

	LocalParams local;					///< локальные параметры
};

#endif /* PARAMBSP_H_ */
