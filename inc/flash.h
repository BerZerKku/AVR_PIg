/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: Хозяин
 */

#include <avr/pgmspace.h>
#include "glbDefine.h"

#define STRING_LENGHT 11

static const char fcNull[] PROGMEM = "";

// измеряемые параметры
static const char fcUout[]  PROGMEM = "U=%02u.%01uВ";
static const char fcIout[] 	PROGMEM	= "I=%03uмА";
static const char fcRout[] 	PROGMEM = "R=%03uОм";
static const char fcUz[]	PROGMEM	= "Uз=%02dдБ";
static const char fcUz1[] 	PROGMEM = "Uз1=%02dдБ";
static const char fcUz2[] 	PROGMEM = "Uз2=%02dдБ";
static const char fcUcf[] 	PROGMEM	= "Uк=%02dдБ";
static const char fcUcf1[] 	PROGMEM	= "Uк1=%02dдБ";
static const char fcUcf2[] 	PROGMEM	= "Uк2=%02dдБ";
static const char fcUn[] 	PROGMEM	= "Uш=%02dдБ";
static const char fcUn1[] 	PROGMEM	= "Uш1=%02dдБ";
static const char fcUn2[] 	PROGMEM	= "Uш2=%02dдБ";
static const char fcSd[]	PROGMEM = "Sд=%02u°";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// имена устройств аппарата
static const char fcDeviceName00[] PROGMEM = "ТРЗ";

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
static const char fcIC[GB_IC_MAX + 1][9] PROGMEM = {
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


static const char fcOnOff[3][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"выкл.",	//
		"вкл.",		//
		"ошибка"	//
};

static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
		"ОБЩ",	//
		"ОШБ"	//
};

static char fcInterface[GB_INTERFACE_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"USB",		//
		"RS485",	//
		"ошибка"	//
};

static char fcBaudRate[UART_BAUD_RATE_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1200",		//
		"2400",		//
		"4800",		//
		"9600",		//
		"19200",	//
		"38400",	//
		"57600",	//
		"ошибка"	//
};

static char fcDataBits[UART_DATA_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"8",		//
		"ошибка"	//
};

static char fcParity[UART_PARITY_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"нет",		//
		"чет",		//
		"нечет",	//
#ifdef UART_PARITY_MARK
		"Маркер",
#endif
#ifdef UART_PARITY_SPACE
		"Пробел",
#endif
		"ошибка"
};

static char fcStopBits[UART_STOP_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",		//
#ifdef UART_STOP_BITS_ONEPONTFIVE
		"1.5",		//
#endif
		"2",		//
};

// состояния устройств
// кол-во состояний для всех устройств должно совпадать с MAX_NUM_DEVICE_STATE
// неизвестное состояние
static const char fcUnknownSost[] PROGMEM = "Ошибка";
// состояния Защиты
static const char fcDefSost00[] PROGMEM =	"Исходн.";
static const char fcDefSost01[] PROGMEM =	"Контроль";
static const char fcDefSost02[] PROGMEM =	"Пуск";
static const char fcDefSost03[] PROGMEM =	"Останов";
static const char fcDefSost04[] PROGMEM =	"Неиспр.";
static const char fcDefSost05[] PROGMEM =	"П.неиспр";
static const char fcDefSost06[] PROGMEM =	"Ожидание";
static const char fcDefSost07[] PROGMEM =	"Нал.пуск";
static const char fcDefSost08[] PROGMEM = 	"Уд.пуск";
static const char fcDefSost09[] PROGMEM =	"Нет РЗ";
static const char fcDefSost10[] PROGMEM =	"Речь";
static const char fcDefSost11[] PROGMEM =	"ПРД";
static const char fcDefSost12[] PROGMEM =	"ПРМ";

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
