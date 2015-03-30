/*
 * flashParams.h
 *
 *  Created on: 27.03.2015
 *      Author: Shcheblykin
 */

#ifndef FLASHPARAMS_H_
#define FLASHPARAMS_H_

#include <avr/pgmspace.h>
#include "glbDefine.h"
#include "param.h"

#define STRING_LENGHT 11

static const char fcNullBuf[] PROGMEM= "";

/// массив строк размерностей, связан с Param::DIMENSION
static const char fcDimension[] [5] PROGMEM = {
		"",		// Param::DIM_NO
		"мА",	// Param::DIM_MA
		"А",	// Param::DIM_A
		"кА",	// Param::DIM_KA
		"В",	// Param::DIM_V
		"кВ",	// Param::DIM_KV
		"мс",	// Param::MSEC
		"c",	// Param::DIM_SEC
		"час",	// Param::DIM_HOUR
		"дБ",	// Param::DIM_DB
		"Гц",	// Param::DIM_HZ
		"кГц",	// Param::DIM_KHZ
		"град"	// Param::DIM_DEG
};

/// Возможные значения параметра.
static const char fcOnOff[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"выкл.",		///<
		"вкл."			///<
};

/// Тип защиты (параметр Защиты).
static const char fcDefType[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"ДФЗ-ПрПд",		///<
		"ДФЗ-МК1",		///<
		"ДФЗ-МК2",		///<
		"ДФЗ-L60",		///<
		"НЗ-ПрПд",		///<
		"НЗ-МК",		///<
		"ППЗ-ПрПд",		///<
		"ППЗ-МК"		///<
};

/// Тип линии (параметр Общий).
static const char fcNumDevices[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"2 концевая",	///<
		"3 концевая"	///<
};

/// Тип детектора (параметр Общий).
static const char fcPrmType[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"акт+пасс",		///<
		"активный",		///<
		"пассивный"	///<
};

/// Тип совместимости (параметр Защиты).
static const char fcCompatibility[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"АВАНТ Р400",	///<
		"ПВЗ-90",		///<
		"АВЗК-80",		///<
		"ПВЗУ-Е",		///<
		"ПВЗЛ",			///<
		"ЛИНИЯ-Р"		///<
};

/// Тип совместимости (параметр К400).
static const char fcCompK400[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"АВАНТ",		///<
		"АВАНТ ПРД",	///<
		"АВАНТ ПРМ",	///<
		"АКПА ПРД",		///<
		"АКПА ПРМ",		///<
		"КЕДР ПРД",		///<
		"КЕДР ПРМ",		///<
		"УПК-Ц ПРД",	///<
		"УПК-Ц ПРМ",	///<
		"ВЧТО ПРД",		///<
		"ВЧТО ПРМ"		///<
};

/// Количество команд.
static const char fcK400NumCom[][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"0",			///<
		"4",			///<
		"8",			///<
		"12",			///<
		"16",			///<
		"24",			///<
		"32"			///<
};

/// Частота ПРД / ПРМ (параметр Защиты).
static const char fcPvzlFreq[][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"-1000Гц",		///<
		"-500Гц",		///<
		"0Гц",			///<
		"+500Гц",		///<
		"+1000Гц"		///<
};

/// Протокол обмена (параметр Общий).
static
const char fcPvzueProtocol[][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"быстрый",		///<
		"медленный"	///<
};

/// Тип атоконтроля (параметр Общий).
static char fcPvzueTypeAC[][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"тревожный",	///<
		"нормальный",	///<
		"спокойный"		///<
};

/// Признак четности (параметр Общий).
static char fcPvzueParity[][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"вкл.",			///<
		"выкл."			///<
};

//								   				   12345678901234567890
static const char nameBackup[]			PROGMEM = "Резервирование";
static const char nameCompK400[]		PROGMEM = "Совместимость";
static const char nameCompP400[]		PROGMEM = "Совместимость";
static const char nameComPrdKeep[]		PROGMEM = "Удерж. реле ком. ПРД";
static const char nameComPrmKeep[] 		PROGMEM = "Удерж. реле ком. ПРМ";
static const char nameCorI[]			PROGMEM = "Коррекция тока";
static const char nameCorU[]			PROGMEM = "Коррекция напряжения";
static const char nameDefType[] 		PROGMEM = "Тип защиты";
static const char nameDelay[]			PROGMEM = "Компенсация задержки";
static const char nameDetector[]		PROGMEM = "Тип детектора";
static const char nameFreq[]			PROGMEM = "Частота";
static const char nameFreqPrd[]			PROGMEM = "Частота ПРД";
static const char nameFreqPrm[]			PROGMEM = "Частота ПРМ";
static const char nameInDec[]			PROGMEM = "Загрубл. чувств. ПРМ";
static const char nameInDecAc[]			PROGMEM = "Снижение уровня АК";
static const char nameInDecAcAnswer[]	PROGMEM = "Снижение ответа АК";
static const char nameNumOfDevice[] 	PROGMEM = "Номер аппарата";
static const char nameNumOfDevices[]	PROGMEM = "Тип линии";
static const char nameNetAddress[]		PROGMEM = "Сетевой адрес";
static const char nameOutCheck[] 		PROGMEM = "Контроль вых.сигнала";
static const char nameOverlap[]			PROGMEM = "Перекрытие импульсов";
static const char namePrdComBlock[]		PROGMEM = "Блокиров. команды";
static const char namePrdComLong[]		PROGMEM = "Следящие команды";
static const char namePrdComNumbers[]	PROGMEM = "Количество команд";
static const char namePrdComNumbersA[]	PROGMEM = "Количество команд А";
static const char namePrdDrComBlock[]	PROGMEM = "Блокиров. команды ЦС";
static const char namePrdDrEnable[]		PROGMEM = "Трансляция ЦС";
static const char namePrdDuration[]		PROGMEM = "Длительность команды";
static const char namePrdInDelay[]		PROGMEM = "Задержка срабат. ПРД";
static const char namePrmComBlock[]		PROGMEM = "Блокиров. команды";
static const char namePrmComNumbers[]	PROGMEM = "Количество команд";
static const char namePrmDrComToHF[]	PROGMEM = "Команда ВЧ в ЦС";
static const char namePrmDrComBlock[]	PROGMEM = "Блокиров. команды ЦС";
static const char namePrmDrEnable[]		PROGMEM = "Трансляция ЦС";
static const char namePrmTimeOff[]		PROGMEM = "Задержка на выкл.ком";
static const char namePrmTimeOn[]		PROGMEM = "Задержка на фикс.ком";
static const char namePrmType[]			PROGMEM = "Тип приемника";
static const char namePvzueAcType[]		PROGMEM = "Тип автоконтроля";
static const char namePvzueFail[]		PROGMEM = "Допустимые провалы";
static const char namePvzueNoiseLvl[]	PROGMEM = "Допустимая помеха";
static const char namePvzueNoiseTh[]	PROGMEM = "Порог по помехе";
static const char namePvzueParity[]		PROGMEM = "Признак четности";
static const char namePvzuePeriodAC[]	PROGMEM = "Период беглого АК";
static const char namePvzuePeriodReAC[]	PROGMEM = "Период повт.бегл. АК";
static const char namePvzueProtocol[]	PROGMEM = "Протокол обмена";
static const char nameSensDec[]			PROGMEM = "Загрубление чувствит";
static const char nameSensDecRz[]		PROGMEM = "Загрубл. чувств. РЗ";
static const char nameShiftBack[]		PROGMEM = "Сдвиг зад.фронта ПРД";
static const char nameShiftFront[]		PROGMEM = "Сдвиг пер.фронта ПРД";
static const char nameShiftPrd[]		PROGMEM = "Сдвиг ВЧ ПРД от ПУСК";
static const char nameShiftPrm[]		PROGMEM = "Сдвиг ПРМ";
static const char nameTestCom[]			PROGMEM = "Тестовая команда";
static const char nameTimeNoMan[]		PROGMEM = "Доп. время без ман.";
static const char nameTimeRerun[]		PROGMEM = "Время перезапуска";
static const char nameTimeSynch[] 		PROGMEM = "Синхронизация часов";
static const char nameTmK400[]			PROGMEM = "Телемеханика";
static const char nameUOutNom[]			PROGMEM = "Uвых номинальное";
static const char nameWarnThreshold[]	PROGMEM = "Порог предупреждения";
static const char nameWarnThresholdCf[]	PROGMEM = "Порог предупр. по КЧ";
static const char nameWarnThresholdRz[]	PROGMEM = "Порог предупр. по РЗ";

//	GB_PARAM_PRM_DR_COM,		///< команда ВЧ в ЦС
//	GB_PARAM_PRM_COM_NUMS		///< количество команд

/// Параметры (связаны с eGB_PARAM)
static const Param fcParams[] PROGMEM = {
		// ЗАГЛУШКА
		{fcNullBuf,				GB_COM_NO,					Param::PARAM_NO,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ОБЩИЕ ПАРМЕТРЫ
		// синхронизация часов
		{nameTimeSynch,			GB_COM_GET_TIME_SINCHR,		Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// номер аппарата (2-х концевая)
		{nameNumOfDevice,		GB_COM_GET_DEVICE_NUM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		2,		1,		1},
		// номер аппарата (3-х концевая)
		{nameNumOfDevice,		GB_COM_GET_DEVICE_NUM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		3,		1,		1},
		// контроль выходного сигнала
		{nameOutCheck, 			GB_COM_GET_OUT_CHECK,		Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// порог предупреждения
		{nameWarnThreshold,		GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0, 		22,		1,		1},
		// порог предупреждения по КЧ (для РЗСК)
		{nameWarnThresholdCf,	GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0, 		22,		1,		1},
		// время перезапуска
		{nameTimeRerun,			GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC,		fcNullBuf,			1,	 	0,		5,		1,		1},
		// удержание реле команд ПРД
		{nameComPrdKeep,		GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// удержание реле команд ПРМ
		{nameComPrmKeep,		GB_COM_GET_COM_PRM_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// загрубление чувствительности ПРМ (2-х концевая)
		{nameInDec,				GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		22,		1,		1},
		// загрубление чувствительности ПРМ (3-х концевая)
		{nameInDec,				GB_COM_GET_CF_THRESHOLD,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			2,		0,		22,		1,		1},
		// адрес в локальной сети
		{nameNetAddress,		GB_COM_GET_NET_ADR,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		247,	1,		1},
		// номинальноые выходное напряжение
		{nameUOutNom,			GB_COM_GET_COM_PRM_KEEP,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_V,		fcNullBuf,			1,		18,		50,		1,		1},
		// частота
		{nameFreq,				GB_COM_GET_FREQ,			Param::PARAM_INT,	Param::RANGE_INT_NO_DIM,Param::DIM_KHZ,		fcNullBuf,			1,		16,		1000,	1,		1},
		// совместимость (Р400, Р400м)
		{nameCompP400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcCompatibility[0],	1,		0,		5,		1,		1},
		// снижение ответа АК (ПВЗЛ)
		{nameInDecAcAnswer,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		20,		1,		1},
		// тип детектора
		{nameDetector,			GB_COM_GET_TIME_SINCHR,		Param::PARAM_INT,  	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		1,		3,		1,		1},
		// коррекция напряжения
		{nameCorU,				GB_COM_GET_COR_U_I,			Param::PARAM_U,		Param::RANGE_U_COR,		Param::DIM_V,		fcNullBuf,			1,		0,		600,	1,		1},
		// коррекция тока
		{nameCorI,				GB_COM_GET_COR_U_I,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MA,		fcNullBuf,			1,		0,		999,	1,		1},
		// протокол обмена (ПВЗУ-Е)
		{namePvzueProtocol,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueProtocol[0],	1,		1,		2,		1,		1},
		// признак четности (ПВЗУ-Е)
		{namePvzueParity,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueParity[0],	1,		1,		2,		1,		1},
		// допустимые провалы (ПВЗУ-Е)
		{namePvzueFail,			GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		80,		2,		1},
		// порог по помехе (ПВЗУ-Е)
		{namePvzueNoiseTh, 		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC,		fcNullBuf,			1,		0,		255,	1,		1},
		// допустимая помеха (ПВЗУ-Е)
		{namePvzueNoiseLvl,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		20,		80,		20,		1},
		// тип автоконтроля (ПВЗУ-Е)
		{namePvzueAcType,		GB_COM_GET_TIME_RERUN,		Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzueTypeAC[0],	1,		1,		3,		1,		1},
		// период беглого режима АК (ПВЗУ-Е)
		{namePvzuePeriodAC,		GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC, 	fcNullBuf,			1,		1,		255,	1,		1},
		//	период повтора беглого режима АК (ПЗВУ-Е)
		{namePvzuePeriodReAC,	GB_COM_GET_TIME_RERUN,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_SEC, 	fcNullBuf,			1,		1,		255,	1,		1},
		// резервирование
		{nameBackup,			GB_COM_GET_COR_U_I,			Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcNullBuf,			1,		0,		1,		1,		1},
		// совместимость К400
		{nameCompK400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcCompK400[0],		1, 		0,		10,		1,		1},
		// тип линии (кол-во аппаратов в линии)
		{nameNumOfDevices,		GB_COM_DEF_GET_LINE_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcNumDevices[0],	1,		1,		2,		1,		1},
		// телемеханика
		{nameTmK400,			GB_COM_GET_COM_PRD_KEEP,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ПАРАМЕТРЫ ЗАЩИТЫ
		// тип защиты
		{nameDefType, 			GB_COM_DEF_GET_DEF_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcDefType[0],		1,		0,		7,		1,		1},
		// дополнительное время без манипуляции
		{nameTimeNoMan,			GB_COM_DEF_GET_T_NO_MAN,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_HOUR,	fcNullBuf,			1,		0,		99,		1,		1},
		// перекрытие импульсов
		{nameOverlap,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		54,		2,		1},
		// перекрытие импульсов Р400
		{nameOverlap,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		18,		54,		2,		1},
		// компенсация задержки в линии (2-х концевая)
		{nameDelay,				GB_COM_DEF_GET_DELAY, 		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf, 			1, 		0, 		18,		2,		1},
		// компенсация задержки в линии (3-х концевая)
		{nameDelay,				GB_COM_DEF_GET_DELAY, 		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf, 			2, 		0, 		18,		2,		1},
		// порог предупреждения по РЗ
		{nameWarnThresholdRz,	GB_COM_DEF_GET_RZ_THRESH,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		16,		1,		1},
		// загрубление чувствительности
		{nameSensDec,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// загрубление чувствительности РЗ (2-х концевая)
		{nameSensDecRz,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// загрубление чувствительности РЗ (3-х концевая)
		{nameSensDecRz,			GB_COM_DEF_GET_RZ_DEC,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DB,		fcNullBuf,			1,		0,		32,		1,		1},
		// тип приемника
		{namePrmType,			GB_COM_DEF_GET_PRM_TYPE,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPrmType[0],		1,		0,		2,		1,		1},
		// снижение уровня АК
		{nameInDecAc,			GB_COM_DEF_GET_PRM_TYPE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// частота ПРД
		{nameFreqPrd,			GB_COM_DEF_GET_FREQ_PRD,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzlFreq[0],		1,		0,		4,		1,		1},
		// частота ПРД
		{nameFreqPrm,			GB_COM_DEF_GET_RZ_THRESH,	Param::PARAM_LIST,	Param::RANGE_LIST,		Param::DIM_NO,		fcPvzlFreq[0],		1,		0,		4,		1,		1},
		// сдвиг переднего фронта ПРД относительно сигнала МАН (пуск)
		{nameShiftFront,		GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// сдвиг заднего фронта ПРД относительно сигнала МАН (пуск)
		{nameShiftBack,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// сдвиг ПРМ
		{nameShiftPrm,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// сдвиг ВЧ ПРД от ПУСК
		{nameShiftPrd,			GB_COM_DEF_GET_OVERLAP,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_DEG,		fcNullBuf,			1,		0,		72,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ПАРАМЕТРЫ ПЕРЕДАТЧИКА
		// время включения (задержка срабатывания дискретного входа)
		{namePrdInDelay,		GB_COM_PRD_GET_TIME_ON,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		0,		20,		1,		1},
		// длительность команды ВЧ
		{namePrdDuration,		GB_COM_PRD_GET_DURATION,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		30,		100,	1,		1},
		// длительность команды ОПТИКА
		{namePrdDuration,		GB_COM_PRD_GET_DURATION,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		30,		500,	10,		10},
		// тестовая команда
		{nameTestCom,			GB_COM_PRD_GET_TEST_COM,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// следящие команды
		// TODO зависит от количества текущих команд на передачу!!!
		{namePrdComLong,		GB_COM_PRD_GET_LONG_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// блокированные команды
		// TODO зависит от количества текущих команд на передачу!!!
		{namePrdComBlock,		GB_COM_PRD_GET_BLOCK_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// трансляция ЦС
		{namePrdDrEnable,		GB_COM_PRD_GET_DR_STATE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// блокированные команды ЦС
		// TODO зависит от количества текущих команд на передачу!!!
		{namePrdDrComBlock,		GB_COM_PRD_GET_DR_BLOCK,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// количество команд
		{namePrdComNumbers, 	GB_COM_PRD_GET_COM,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		4,		4},
		// количество команд группы А
		// TODO максимум зависит от количества текущих команд на передачу!!!
		{namePrdComNumbersA, 	GB_COM_PRD_GET_COM_A,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		1,		1},
		// ---------------------------------------------------------------------
		//name					com							param				range					dim					listValues			num		min		max		disc	fract
		// ---------------------------------------------------------------------
		// ПАРАМЕТРЫ ПРИЕМНИКА
		// задержка на фиксацию команды (время включения)
		{namePrmTimeOn,			GB_COM_PRM_GET_TIME_ON,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			1,		0,		5,		1,		1},
		// блокированные команды
		// TODO зависит от количества текущих команд на приеме!!!
		{namePrmComBlock,		GB_COM_PRM_GET_BLOCK_COM,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// задержка на выключение
		// TODO зависит от количества текущих команд на приеме!!!
		{namePrmTimeOff,		GB_COM_PRM_GET_TIME_OFF,	Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_MSEC,	fcNullBuf,			32,		0,		1000,	50,		10},
		// трансляция ЦС
		{namePrmDrEnable,		GB_COM_PRM_GET_DR_STATE,	Param::PARAM_LIST,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			1,		0,		1,		1,		1},
		// блокированные команды ЦС
		// TODO зависит от количества текущих команд на приеме!!!
		{namePrmDrComBlock,		GB_COM_PRM_GET_DR_BLOCK,	Param::PARAM_BITES,	Param::RANGE_ON_OFF,	Param::DIM_NO,		fcOnOff[0],			32,		0,		1,		1,		1},
		// команда ВЧ в ЦС
		{namePrmDrComToHF,		GB_COM_PRM_GET_DR_COM,		Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			32,		1,		32,		1,		1},
		// количество команд
		{namePrmComNumbers,		GB_COM_PRM_GET_COM,			Param::PARAM_INT,	Param::RANGE_INT,		Param::DIM_NO,		fcNullBuf,			1,		0,		32,		4,		4}
};

#endif /* FLASHPARAMS_H_ */
