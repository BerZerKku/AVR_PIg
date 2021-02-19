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
#include "menu/dateTime.hpp"
#include "menu/journalEntry.hpp"
#include "menu/jrnScada.hpp"
#include "menu/txCom.hpp"
#include "parameter/LocalParams.h"
#include "security/infosecurity.h"

/// Структура параметров БСП
struct stGBparam {
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;

    // Флаг наличия связи с БСП.
    bool connectionBsp;
    // Флаг наличия связи с ПК.
    bool connectionPc;

	// тип аппарата
	eGB_TYPE_DEVICE typeDevice;

	// тип оптического аппарата
	eGB_TYPE_OPTO typeOpto;

	// дата/время
	TDateTime DateTime;

	// дата/время БСП в момент установки новго времени
	TDateTime DateTimeReq;

	// измеряемые параметры
	TMeasuredParameters measParam;

	// текущее состояние устройств
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// локальные параметры
	LocalParams local;
    // сохранение параметров
    save_t save;

	// параметры для работы с последовательным портом
	TUartData Uart;

    // параметры для инфорационной безопасности
    TInfoSecurity security;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;

	// запись для SCADA
	TJrnSCADA jrnScada;

	// тесты
	TTest test;
};

#endif /* PARAMBSP_H_ */
