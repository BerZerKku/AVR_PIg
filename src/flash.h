/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: Хозяин
 */

#include <avr/pgmspace.h>
#include "glbDefine.h"
#include "flashParams.h"

static const char fcNull[] PROGMEM = "";

// измеряемые параметры, в сумме выводится не более 9 символов
// в крайнем случае 10, но их надо выводить в правом столбце
static const char fcPout[]	PROGMEM = "P=%02uдБм";
static const char fcUctrl[]	PROGMEM = "U=%02dдБ";
static const char fcSN[]	PROGMEM = "SN=%02dдБ";
static const char fcNOut[]	PROGMEM = "Nout=%01d";
static const char fcNIn[]	PROGMEM = "Nin=%01d";
static const char fcFazA[]	PROGMEM = "Fa=%02dгр";
static const char fcFazB[]	PROGMEM = "Fb=%02dгр";
static const char fcFazC[]	PROGMEM = "Fc=%02dгр";
static const char fcIA[] 	PROGMEM = "Ia=%02uкА";
static const char fcIB[] 	PROGMEM = "Ib=%02uкА";
static const char fcIC[] 	PROGMEM = "Ic=%02uкА";
static const char fcUA[]	PROGMEM = "Ua=%02uкВ";
static const char fcUB[]	PROGMEM = "Ub=%02uкВ";
static const char fcUC[]	PROGMEM = "Uc=%02uкВ";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// режимы устройств
static const char fcRegime[GB_REGIME_MAX + 1][8] PROGMEM = {
//		 1234567
		"Выведен",	//
		"Готов",	//
		"Введен",	//
		"Речь",		//
		"Тест",		//
		"Тест",		//
		"ошибка"	//
};

// версии прошивок
static const char fcICname[GB_IC_MAX + 1][9] PROGMEM = {
//		 12345678
		"БСП MCU ",	//
		"БСП DSP ",	//
		"ПИ  MCU ",	//
		"БСК ПРД1",	//
		"БСК ПРД2",	//
		"БСК ПРМ1",	//
		"БСК ПРМ2",	//
		"ошибка"
};

// режимы устройств, для ввода с клавиатуры
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"Выведен",	//
		"Введен",	//
};

static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
		"ЗАЩ",	//
		"ОБЩ",	//
		"ОШБ"	//
};

// состояния устройств
// кол-во состояний для всех устройств должно совпадать с MAX_NUM_DEVICE_STATE
// неизвестное состояние
static const char fcUnknownSost[] PROGMEM = "Ошибка";
// состояния Терминала
static const char fcRpsSost00[] PROGMEM =	"Исходн.";
static const char fcRpsSost01[] PROGMEM =	"Контроль";
static const char fcRpsSost02[] PROGMEM =	"Пуск";
static const char fcRpsSost03[] PROGMEM =	"Останов";
static const char fcRpsSost04[] PROGMEM =	"Неиспр.";
static const char fcRpsSost05[] PROGMEM =	"П.неиспр";
static const char fcRpsSost06[] PROGMEM =	"Ожидание";
static const char fcRpsSost07[] PROGMEM =	"Нал.пуск";
static const char fcRpsSost08[] PROGMEM = 	"Уд.пуск";
static const char fcRpsSost09[] PROGMEM =	"Нет РЗ";
static const char fcRpsSost10[] PROGMEM =	"Речь";
static const char fcRpsSost11[] PROGMEM =	"ПРД";
static const char fcRpsSost12[] PROGMEM =	"ПРМ";

// надписи для неиспользуемых в текущем аппарате кодов
static const char fcUnknownFault[]		PROGMEM = "Неисправность";
static const char fcUnknownWarning[] 	PROGMEM = "Предупреждение";

// строка вывода кода неисправности/предупреждения
// %c - g для глобальной, l - для локальной
static const char fcFaults[] 			PROGMEM = "Неиспр. %c-%04X";
static const char fcWarnings[] 			PROGMEM = "Предупр. %c-%04X";

// общие неисправности
static const char fcGlbFault0001[] 		PROGMEM = "Неиспр.чт. FLASH";
static const char fcGlbFault0002[] 		PROGMEM = "Неиспр.зап.FLASH";
static const char fcGlbFault0004[] 		PROGMEM = "Неиспр.чт. PLIS";
static const char fcGlbFault0008[] 		PROGMEM = "Неиспр.зап.PLIS";
static const char fcGlbFault0010[] 		PROGMEM = "Неиспр.зап.2RAM";
static const char fcGlbFault0020[] 		PROGMEM = "АК-Нет ответа";
static const char fcGlbFault0020rzsk[] 	PROGMEM = "Нет сигнала ПРМ";
static const char fcGlbFault0040[] 		PROGMEM = "АК-Снижен.запаса";
static const char fcGlbFault0080[] 		PROGMEM = "Помеха в линии";
static const char fcGlbFault0100[] 		PROGMEM = "Неиспр.DSP 2RAM ";
static const char fcGlbFault0200[] 		PROGMEM = "Неиспр.чт. 2RAM ";
static const char fcGlbFault0400[] 		PROGMEM = "Ток покоя";
static const char fcGlbFault0400rzsk[] 	PROGMEM = "Установите часы";
static const char fcGlbFault0800[]		PROGMEM = "Низкое напр.вых";
static const char fcGlbFault1000[] 		PROGMEM = "Высокое напр.вых";
static const char fcGlbFault2000[] 		PROGMEM = "Неиспр. МК УМ";
static const char fcGlbFault4000[] 		PROGMEM = "ВЧ тракт восст.";

// общие предупреждения
static const char fcGlbWarning01[] 		PROGMEM = "Установите часы";
static const char fcGlbWarning02[] 		PROGMEM = "Отказ резерв.лин";
static const char fcGlbWarning10[] 		PROGMEM = "Удал.ПОСТ неиспр";
static const char fcGlbWarning20[] 		PROGMEM = "Удал.ПРД неиспр";
static const char fcGlbWarning40[] 		PROGMEM = "Удал.ПРМ неиспр";

// параметры
static const char fcEnterInt[]			PROGMEM = "Ввод: %f";
static const char fcValue[] 			PROGMEM = "Значение: ";
static const char fcRange[] 			PROGMEM = "Диапазон: ";
static const char fcNumCom[] 			PROGMEM = "Номер команды: %u/%u";
static const char fcNumPrm[]			PROGMEM = "Номер ПРМ: %u/%u";
static const char fcRangeDec[] 			PROGMEM = "%d..%d%s";
static const char fcRangeList[] 		PROGMEM = "список";
static const char fcRangeOnOff[]		PROGMEM = "вкл./выкл.";
static const char fcNumPunkt[] 			PROGMEM = "Параметр:%u Всего:%u";

//	ЖУРНАЛ
static const char fcJrnEmpty[] 			PROGMEM = "ЖУРНАЛ ПУСТ";
static const char fcJrnNumEntries[] 	PROGMEM = "Запись %u / %u";
static const char fcRegimeJrn[]			PROGMEM = "Режим:";
static const char fcStateJrn[] 			PROGMEM = "Состояние:";
static const char fcDateJrn[]			PROGMEM = "Дата: %02u.%02u.%02u";
static const char fcTimeJrn[]			PROGMEM = "Время: %02u:%02u:%02u.%03u";
static const char fcJrnNotReady[]		PROGMEM = "СЧИТЫВАНИЕ ЗАПИСИ";

/// записи журнала событий для РЗСК
static char fcJrnEvent[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"Событие - %d",				// 0 - ошибочное значение
		"Вкл. питания/перезап",		// 1
		"Выключение питания",       // 2
		"Изменение режима раб",     // 3
		"Событие - %d",           	// 4
		"Неиспр теста ПРД",         // 5
		"Неиспр теста ПРМ",         // 6
		"Неиспр блока БСЗ",         // 7
		"Неиспр блока БСК",         // 8
		"Неиспр перекл-ей БСЗ",   	// 9
		"Нет сигнала манипул.",     // 10
		"Неиспр выходной цепи",     // 11
		"Нет сигнала РЗ",           // 12
		"Отсут-е сигнала Пуск",    	// 13
		"Отсут-е сигнала Стоп",    	// 14
		"Неиспр чтения команд",     // 15
		"Событие - %d",          	// 16
		"Неиспр. работы DSP",		// 17
		"Восстан-е работы DSP",		// 18
		"Низкое напр. выхода",    	// 19
		"Высокое напр. выхода",    	// 20
		"Нет КЧ 5 сек на ПРМ1",	    // 21
		"Нет КЧ 5 сек на ПРМ2",		// 22
		"Восстан-е КЧ на ПРМ1",     // 23
		"Восстан-е КЧ на ПРМ2",		// 24
		"Неиспр чт/зап 2RAM",    	// 25
		"Неиспр чт/зап ПЛИС",    	// 26
		"Неиспр чт/зап FLASH",   	// 27
		"Неисправность часов",      // 28
		"Событие - %d",       		// 29
		"Событие - %d",        		// 30
		"Событие - %d",         	// 31
		"Событие - %d",    			// 32
		"Событие - %d"          	// MAX_JRN_EVENT_VALUE - ошибочное значение
};
