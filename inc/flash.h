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
static const char fcSd[]	PROGMEM = "Sд=%02uг";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// имена устройств аппарата
static const char fcDeviceName00[] PROGMEM = "ЗАЩ";
static const char fcDeviceName01[] PROGMEM = "ПРМ";
static const char fcDeviceName02[] PROGMEM = "ПРД";

// типы автоконтролей для отображения в начальном меню (eGB_TYPE_AC)
static const char fcAcType[GB_TYPE_AC_MAX + 2][9] PROGMEM = {
// 		12345678
		"АК-ошиб",		//
		"АК-норм",		//
		"АК-норм",		//
		"АК-ускор",		//
		"АК-выкл",		//
		"АК-бегл",		//
		"АК-испыт",		//
		"АК-пуск",		//
		"АК-однос",		//
		"АК-ошиб",		//
};
static const char fcTimeToAc[] PROGMEM = "%02d:%02d:%02d";

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
		"БСЗ ПЛИС", //
		"ошибка"
};

// режимы устройств, для ввода с клавиатуры
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"Выведен",	//
		"Введен",	//
		"Тест 1",	//
		"Тест 2"	//
};

// тип защиты
static const char fcDefType[DEF_TYPE_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"ДФЗ-ПрПд",	//
		"ДФЗ-МК1",	//
		"ДФЗ-МК2",	//
		"ДФЗ-L60",	//
		"НЗ-ПрПд",	//
		"НЗ-МК",	//
		"ППЗ-ПрПд",	//
		"ППЗ-МК",	//
		"ошибка"	//
};

static
const char fcNumDevices[GB_NUM_DEVICES_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"ошибка",		//
		"2 концевая",	//
		"3 концевая",	//
		"ошибка"		//
};

static const char fcPrmType[DEF_PRM_TYPE_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"акт+пасс",		//
		"активный",		//
		"пассивный",	//
		"ошибка"		//
};

static const char fcOnOff[3][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"выкл.",	//
		"вкл.",		//
		"ошибка"	//
};

static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
		"ЗАЩ",	//
		"ПРМ",	//
		"ПРД",	//
		"ОБЩ",	//
		"ОШБ"	//
};

static const char fcDevicesK400[GB_DEVICE_K400_MAX + 1][5] PROGMEM = {
// 		 123
		"ЗАЩ",	//
		"ПРМ1",	//
		"ПРМ2",	//
		"ПРД",	//
		"ОБЩ",	//
		"П1,2",
		"ОШБ"	//
};

static
const char fcCompatibility[GB_COMPATIBILITY_MAX + 1][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"АВАНТ Р400",	//
		"ПВЗ-90",		//
		"АВЗК-80",		//
		"ПВЗУ-Е",		//
		"ПВЗЛ",			//
		"ошибка"		//
};

static const char fcPvzlFreq[GB_PVZL_FREQ_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"-1000Гц",	//
		"-500Гц",	//
		"0Гц",		//
		"+500Гц",	//
		"+1000Гц",	//
		"ошибка"	//
};

static
const char fcPvzueProtocol[GB_PVZUE_PROTOCOL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"ошибка",//
		"быстрый",		//
		"медленный",	//
		"ошибка"		//
};

static char fcPvzueTypeAC[GB_PVZUE_TYPE_AC_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"ошибка",//
		"тревожный",	//
		"нормальный",	//
		"спокойный",	//
		"ошибка"		//
};

static char fcPvzueParity[GB_PVZUE_PARITY_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"ошибка",	//
		"вкл.",		//
		"выкл.",	//
		"ошибка"	//

};

static char fcInterface[GB_INTERFACE_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"RS232",	//
		"RS485",	//
		"ошибка"	//
};

static char fcProtocol[GB_PROTOCOL_MAX + 1][STRING_LENGHT] PROGMEM = {
//	 	 1234567890
		"СТАНДАРТ",
		"MODBUS",
		"МЭК-101",
		"ошибка"
};

static char fcBaudRate[UART_BAUD_RATE_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"300",		//
		"600",		//
		"1200",		//
		"2400",		//
		"4800",		//
		"9600",		//
		"19200",	//
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
		"ошибка"
};

static char fcStopBits[UART_STOP_BITS_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"1",		//
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
// состояния Приемника
static const char fcPrmSost00[] PROGMEM = 	"Исходн.";
static const char fcPrmSost01[] PROGMEM = 	"ПРМ КЧ%01u";
static const char fcPrmSost02[] PROGMEM = 	"ПРМ ПА%02u";
static const char fcPrmSost03[] PROGMEM = 	"Нет КЧ";
static const char fcPrmSost04[] PROGMEM = 	"Неиспр.";
static const char fcPrmSost05[] PROGMEM = 	"П.неиспр";
static const char fcPrmSost06[] PROGMEM = 	"Ожидание";
static const char fcPrmSost07[] PROGMEM = 	"БЛК КМ%02u";
static const char fcPrmSost08[] PROGMEM = 	"?0x08?";
static const char fcPrmSost09[] PROGMEM = 	"?0x09?";
static const char fcPrmSost10[] PROGMEM = 	"Речь";
static const char fcPrmSost11[] PROGMEM =	"ПРД";
static const char fcPrmSost12[] PROGMEM =	"ПРМ";
// состояния Передатчика
static const char fcPrdSost00[] PROGMEM = 	"Исходн.";
static const char fcPrdSost01[] PROGMEM =	"ПРД КЧ%01u";
static const char fcPrdSost02[] PROGMEM =	"ПРД ПА%02u";
static const char fcPrdSost03[] PROGMEM =	"Нет КЧ";
static const char fcPrdSost04[] PROGMEM =	"Неиспр.";
static const char fcPrdSost05[] PROGMEM =	"П.неиспр";
static const char fcPrdSost06[] PROGMEM =	"Ожидание";
static const char fcPrdSost07[] PROGMEM = 	"?0x07?";
static const char fcPrdSost08[] PROGMEM = 	"ПРД ЦС%02u";
static const char fcPrdSost09[] PROGMEM =	"Систем.";
static const char fcPrdSost10[] PROGMEM =	"Речь";
static const char fcPrdSost11[] PROGMEM =	"ПРД";
static const char fcPrdSost12[] PROGMEM =	"ПРМ";

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

// неисправности защиты
static const char fcDefFault0001[] 		PROGMEM = "Нет блока БСЗ";
static const char fcDefFault0002[] 		PROGMEM = "Неиспр.верс.БСЗ";
static const char fcDefFault0004[] 		PROGMEM = "Неиспр.перекл.";
static const char fcDefFault0008[] 		PROGMEM = "Неиспр.зап. БСЗ";
static const char fcDefFault0010[] 		PROGMEM = "АК-Нет ответа";
static const char fcDefFault0010rzsko[]	PROGMEM = "Нет сигнала ПРМ";
static const char fcDefFault0020[] 		PROGMEM = "Низкий ур. РЗ";
static const char fcDefFault0040[] 		PROGMEM = "Неиспр.удал. ДФЗ";
static const char fcDefFault0080[] 		PROGMEM = "Неиспр.удал. ВЫХ";
static const char fcDefFault0100[] 		PROGMEM = "Неиспр.вход.ПУСК";
static const char fcDefFault0200[] 		PROGMEM = "Неиспр.вход.СТОП";
static const char fcDefFault0400[] 		PROGMEM = "Уд.ПОСТ без отв.";
static const char fcDefFault0800[] 		PROGMEM = "Неиспр.цепь. ВЫХ";
static const char fcDefFault1000[] 		PROGMEM = "Уд.ПОСТ обн.пом.";
static const char fcDefFault2000[] 		PROGMEM = "Неиспр.зап. ВЫХ";
static const char fcDefFault4000[] 		PROGMEM = "Длительн. помеха";
static const char fcDefFault4000rzsk[]	PROGMEM = "Нет сигнала РЗ";
static const char fcDefFault8000[] 		PROGMEM = "Неиспр. ДФЗ";

// предупреждения защиты
static const char fcDefWarning01[] 		PROGMEM = "АК-Снижен.запаса";
static const char fcDefWarning01rzsk[] 	PROGMEM = "Низкий уров. РЗ";
static const char fcDefWarning01rzsko[] PROGMEM = "Нет КЧ";
static const char fcDefWarning02[] 		PROGMEM = "Нет сигнала МАН";
static const char fcDefWarning04[] 		PROGMEM = "Порог по помехе";
static const char fcDefWarning08[] 		PROGMEM = "Автоконтроль";

// неисправности приемника
static const char fcPrmFault0001rzsk[] 	PROGMEM = "Нет блока БСК";
static const char fcPrmFault0002rzsk[] 	PROGMEM = "Неиспр.верс. БСК";
static const char fcPrmFault0004rzsk[] 	PROGMEM = "Нет КЧ";
static const char fcPrmFault0008rzsk[] 	PROGMEM = "Прев.дл-сти.ком.";
static const char fcPrmFault0100rzsk[] 	PROGMEM = "Неиспр.зап. БСК";
static const char fcPrmFault0200rzsk[] 	PROGMEM = "Неиспр.выкл. КСК";
static const char fcPrmFault0400rzsk[] 	PROGMEM = "Неиспр.вкл. КСК";
static const char fcPrmFault0800rzsk[] 	PROGMEM = "Неиспр.контр.КСК";
static const char fcPrmFault1000rzsk[] 	PROGMEM = "Предупр.нет КЧ";
static const char fcPrmFault8000rzsk[] 	PROGMEM = "Прием блок. Ком";

// предупреждения приемника
static const char fcPrmWarning01rzsk[] 	PROGMEM = "Сниж. уровня ПРМ";
static const char fcPrmWarning01rzsko[] PROGMEM = "Нет КЧ";
static const char fcPrmWarning02k400[] 	PROGMEM = "Ошибка работы ЦС";
static const char fcPrmWarning04k400[]	PROGMEM = "Вход RX ЦС пуст";

// неисправности передатчика
static const char fcPrdFault0001rzsk[] 	PROGMEM = "Нет блока БСК";
static const char fcPrdFault0002rzsk[] 	PROGMEM = "Неиспр.верс. БСК";
static const char fcPrdFault0100rzsk[] 	PROGMEM = "Неиспр.чт.команд";
static const char fcPrdFault0200rzsk[] 	PROGMEM = "Неиспр.выкл.Тест";
static const char fcPrdFault0400rzsk[] 	PROGMEM = "Неиспр.вкл. Тест";
static const char fcPrdFault0800rzsk[] 	PROGMEM = "Неиспр.вход. КСК";

// предупреждения передатчика
static const char fcPrdWarning02k400[] 	PROGMEM = "Ошибка работы ЦС";
static const char fcPrdWarning04k400[] 	PROGMEM = "Вход RX ЦС пуст";

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

//static const char fcOn[]				PROGMEM = "вкл.";
//static const char fcOff[]				PROGMEM = "выкл.";

//	ЖУРНАЛ
static const char fcJrnEmpty[] 			PROGMEM = "ЖУРНАЛ ПУСТ";
static const char fcJrnNumEntries[] 	PROGMEM = "(%u/%u)";
static const char fcNumComJrn[]			PROGMEM = "Ком.%u";
static const char fcDateJrn[]			PROGMEM = "%02u.%02u.%02u";
static const char fcTimeJrn[]			PROGMEM = "%02u:%02u:%02u.%03u";
static const char fcJrnNotReady[]		PROGMEM = "СЧИТЫВАНИЕ ЗАПИСИ";
static const char fcSignalDefJrn[]		PROGMEM = "П%uС%uМ%u ПД%uПМ%uВЫХ%u";

/// Источник передаваемой комадны
static const char fcJrnSourcePrd[GB_SOURCE_COM_MAX + 1][5] PROGMEM = {
		"ПА%u",					// команды с дискретных входов
		"ЦС%u",					// команды с цифровой ретрансляции
		"ОШ%u"					// ошибочное значение
};

/// записи журнала ПРД и ПРМ
static const char fcJrnPrd[GB_STATE_COM_MAX + 1][21] PROGMEM = {
		"Окончание команды",	//
		"Начало команды",		//
		"Ошибочное значение"	//
};

/// записи журнала событий для РЗСК
static char fcJrnEventRZSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"Событие - 0",				// 0 - ошибочное значение
		"Вкл. питания/перезап",		// 1
		"Выключение питания",       // 2
		"Изменение режима раб",     // 3
		"Событие - 4",           	// 4
		"Неиспр теста ПРД",         // 5
		"Неиспр теста ПРМ",         // 6
		"Неиспр блока БСЗ",         // 7
		"Неиспр блока БСК",         // 8
		"Неиспр перекл-ей БСЗ",   	// 9
		"Нет сигнала манипул.",     // 10
		"Неиспр выходной цепи",     // 11
		"Нет РЗ 5 сек на %S",       // 12
		"Отсут-е сигнала Пуск",    	// 13
		"Отсут-е сигнала Стоп",    	// 14
		"Неиспр чтения команд",     // 15
		"Сниж. ур. КЧ на %S",	    // 16
		"Неиспр. работы DSP",		// 17
		"Восстан-е работы DSP",		// 18
		"Низкое напр. выхода",    	// 19
		"Высокое напр. выхода",    	// 20
		"Нет КЧ 5 сек на %S",	    // 21
		"Нет КЧ на %S",				// 22
		"Восстан-е КЧ на %S",   	// 23
		"Восст. ур.КЧ на %S",		// 24
		"Неиспр чт/зап 2RAM",    	// 25
		"Неиспр чт/зап ПЛИС",    	// 26
		"Неиспр чт/зап FLASH",   	// 27
		"Неисправность часов",      // 28
		"Сниж. ур. РЗ на %S",		// 29
		"Восстан-е РЗ на %S",  		// 30
		"Нет РЗ на %S",	        	// 31
		"Восст. ур.РЗ на %S",		// 32
		"Событие - %d"          	// MAX_JRN_EVENT_VALUE - ошибочное значение
};

/// записи журнала событий для Р400М
static char fcJrnEventR400_MSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
		"Событие - 0",				// 0 - ошибочное значение
		"Неиспр чт/зап FLASH",   	// 1
		"ВЧ тракт восстановл.",   	// 2
		"Неиспр чт/зап ПЛИС",		// 3
		"Автоконтроль",				// 4
		"Ток покоя",				// 5
		"Неиспр чт/зап 2RAM",		// 6
		"Неиспр работы DSP",		// 7
		"Восстан-е работы DSP",		// 8
		"Низкое напр. выхода",    	// 9
		"Высокое напр. выхода",    	// 10
		"Нарушен обмен с УМ",		// 11
		"Неисправность часов",      // 12
		"Нет блока БСЗ",         	// 13
		"Ошибка версии БСЗ",        // 14
		"Неиспр перекл-ей БСЗ",   	// 15
		"Нет сигнала МАН",			// 16
		"Вкл.пит/Перезапуск",		// 17
		"Изменение режима",		    // 18
		"Неиспр зап. вых.цепи",		// 19
		"Ошиб контр. вых.цепи",		// 20
		"АК - Снижение запаса",		// 21
		"АК - Нет ответа",			// 22
		"Отсут-е сигнала Пуск",    	// 23
		"Отсут-е сигн Останов",    	// 24
		"Выключение аппарата",      // 25
		"Помеха в полосе",         	// 26
		"Неисправность ДФЗ",       	// 27
		"Уд: АК - нет ответа",     	// 28
		"Уд: Помеха в полосе",		// 29
		"Уд: Неиспр. ДФЗ",         	// 30
		"Уд: Неиспр. цепи вых",    	// 31
		"Порог по помехе",         	// 32
		"Событие - %d"          	// MAX_JRN_EVENT_VALUE - ошибочное значение
};

/// сигналы в тестах
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"выкл.",		//
		"КЧ", 			//
		"КЧ1",			//
		"КЧ2",			//
		"КЧ3",			//
		"КЧ4",			//
		"КЧ1",			//	GB_SIGNAL_CF_NO_RZ
		"КЧ1 с блок",	//	GB_SIGNAL_CF_RZ
		"КЧ2",			//	GB_SIGNAL_CF2_NO_RZ
		"КЧ2 с блок",	//	GB_SIGNAL_CF2_RZ
		"РЗ",			//
		"Команда1",		//	GB_SIGNAL_COM1_NO_RZ
		"Команда2",		//	GB_SIGNAL_COM2_NO_RZ
		"Команда3",		//	GB_SIGNAL_COM3_NO_RZ
		"Команда4",		//	GB_SIGNAL_COM4_NO_RZ
		"Ком1+блок",	//	GB_SIGNAL_COM1_RZ
		"Ком2+блок",	//	GB_SIGNAL_COM2_RZ
		"Ком3+блок",	//	GB_SIGNAL_COM3_RZ
		"Ком4+блок",	//	GB_SIGNAL_COM4_RZ
		"КЧ и РЗ",		//	GB_SIGNAL_CF_RZ_R400M
		"ошибка"		//
};
