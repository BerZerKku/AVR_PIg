/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: Хозяин
 */

#include <avr/pgmspace.h>
#include "glbDefine.h"
#include "flashParams.h"

/// Пустая строка.
static const char fcNull[] PROGMEM = "";

/// Типы автоконтролей для отображения в начальном меню.
static const char fcAcType[GB_TYPE_AC_MAX + 2][9] PROGMEM = {
// 		12345678
		"АК-ошиб",		///< Ошибочное значение.
		"АК-норм",		///< АК нормальный (ускоренный).
		"АК-норм",		///< АК нормальный (обычный).
		"АК-ускор",		///< АК ускоренный.
		"АК-выкл",		///< АК выключен.
		"АК-бегл",		///< АК беглый (для совместимости).
		"АК-испыт",		///< АК испытание (для совместимости).
		"АК-пуск",		///< АК пуск (для совместимости)..
		"АК-однос",		///< АК односторонний (для совместимости).
		"АК-ошиб",		///< Ошибочное значение.
		"АК-авто"		///< Подмена  GB_TYPE_AC_AUTO_NORM в Р400м->ЛинияР.
};

/// Режимы работы устройств.
static const char fcRegime[GB_REGIME_MAX + 1][8] PROGMEM = {
//		 1234567
		"Выведен",		///<
		"Готов",		///<
		"Введен",		///<
		"Речь",			///<
		"Тест",			///< Тест + ПРД в состоянии
		"Тест",			///< Тест + ПРМ в состоянии
		"ошибка"		///< ошибочное значение
};

/// Версии прошивок.
static const char fcIC[GB_IC_MAX + 1][9] PROGMEM = {
//		 12345678
		"БСП MCU ",		///< версия прошивки МК платы БСП
		"БСП DSP ",		///< версия прошивки DSP платы БСП
		"ПИ  MCU ",		///< версия прошивки МК платы ПИ
		"БСК ПРД1",		///< версия прошивки ПЛИС ПРД платы БСК 1-16 ком
		"БСК ПРД2",		///< версия прошивки ПЛИС ПРД платы БСК 17-32 ком
		"БСК ПРМ1",		///< версия прошивки ПЛИС ПРМ платы БСК 1-16 ком
		"БСК ПРМ2",		///< версия прошивки ПЛИС ПРМ платы БСК 17-32 ком
		"БСЗ ПЛИС", 	///< версия прошивки платы БСЗ
		"ошибка"		///< ошибочное значение
};

/// Режимы устройств, для ввода с клавиатуры.
static const char fcRegimeEnter[GB_REGIME_ENTER_MAX][STRING_LENGHT] PROGMEM = {
// 		 1234567890
		"Выведен",		///<
		"Введен",		///<
		"Тест 1",		///< Тест передатчика
		"Тест 2"		///< Тест приемника
};

/// Имена устройств (для Журналов).
static const char fcDevices[GB_DEVICE_MAX + 1][4] PROGMEM = {
// 		 123
		"ЗАЩ",			///<
		"ПРМ",			///<
		"ПРД",			///<
		"ОБЩ",			///< общее
		"ОШБ"			///< ошибочное значение
};

/// Имена устройств в К400 (для Журналов).
static const char fcDevicesK400[GB_DEVICE_K400_MAX + 1][5] PROGMEM = {
// 		 1234
		"ЗАЩ",			///<
		"ПРМ1",			///< приемник 1
		"ПРМ2",			///< приемник 2 (в 3-х концевой)
		"ПРД",			///<
		"ОБЩ",			///<
		"П1,2",			///< приемник 1 и 2
		"ОШБ"			///< ошибочное значение
};

/// Номера удаленного(ых) аппарата
static char fcRemoteNum[TDeviceStatus::GB_MAX_REM_NUM + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"   ",			///< 000
		" 1 ",			///< 001
		" 2 ",			///< 010
		"1 2",			///< 011
		" 3 ",			///< 100
		"1 3",			///< 101
		"2 3",			///< 110
		" ? "			///< 111
};

//Состояния устройств
// 	Количество состояний для всех устройств должно совпадать с MAX_NUM_DEVICE_STATE
static const char fcUnknownSost[] PROGMEM = "Ошибка";		///< Неизвестное состояние
// состояния Защиты
static const char fcDefSost00[] PROGMEM =	"Исходн.";		///<
static const char fcDefSost01[] PROGMEM =	"Контроль";		///<
static const char fcDefSost02[] PROGMEM =	"Пуск";			///<
static const char fcDefSost03[] PROGMEM =	"Останов";		///<
static const char fcDefSost04[] PROGMEM =	"Неиспр.";		///<
static const char fcDefSost05[] PROGMEM =	"П.неиспр";		///<
static const char fcDefSost06[] PROGMEM =	"Ожидание";		///<
static const char fcDefSost07[] PROGMEM =	"Нал.пуск";		///<
static const char fcDefSost08[] PROGMEM = 	"Уд.пуск";		///<
static const char fcDefSost09[] PROGMEM =	"Нет РЗ";		///<
static const char fcDefSost10[] PROGMEM =	"Речь";			///<
static const char fcDefSost11[] PROGMEM =	"ПРД";			///<
static const char fcDefSost12[] PROGMEM =	"ПРМ";			///<
// состояния Приемника
static const char fcPrmSost00[] PROGMEM = 	"Исходн.";		///<
static const char fcPrmSost01[] PROGMEM = 	"ПРМ КЧ%01u";	///<
static const char fcPrmSost02[] PROGMEM = 	"ПРМ ПА%02u";	///<
static const char fcPrmSost03[] PROGMEM = 	"Нет КЧ";		///<
static const char fcPrmSost04[] PROGMEM = 	"Неиспр.";		///<
static const char fcPrmSost05[] PROGMEM = 	"П.неиспр";		///<
static const char fcPrmSost06[] PROGMEM = 	"Ожидание";		///<
static const char fcPrmSost07[] PROGMEM = 	"БЛК КМ%02u";	///<
static const char fcPrmSost08[] PROGMEM = 	"?0x08?";		///<
static const char fcPrmSost09[] PROGMEM = 	"?0x09?";		///<
static const char fcPrmSost10[] PROGMEM = 	"Речь";			///<
static const char fcPrmSost11[] PROGMEM =	"ПРД";			///<
static const char fcPrmSost12[] PROGMEM =	"ПРМ";			///<
// состояния Передатчика
static const char fcPrdSost00[] PROGMEM = 	"Исходн.";		///<
static const char fcPrdSost01[] PROGMEM =	"ПРД КЧ%01u";	///<
static const char fcPrdSost02[] PROGMEM =	"ПРД ПА%02u";	///<
static const char fcPrdSost03[] PROGMEM =	"Нет КЧ";		///<
static const char fcPrdSost04[] PROGMEM =	"Неиспр.";		///<
static const char fcPrdSost05[] PROGMEM =	"П.неиспр";		///<
static const char fcPrdSost06[] PROGMEM =	"Ожидание";		///<
static const char fcPrdSost07[] PROGMEM = 	"?0x07?";		///<
static const char fcPrdSost08[] PROGMEM = 	"ПРД ЦС%02u";	///<
static const char fcPrdSost09[] PROGMEM =	"Систем.";		///<
static const char fcPrdSost10[] PROGMEM =	"Речь";			///<
static const char fcPrdSost11[] PROGMEM =	"ПРД";			///<
static const char fcPrdSost12[] PROGMEM =	"ПРМ";			///<

// общие неисправности
static const char fcGlbFault0001[] 		PROGMEM = "Неиспр.чт. FLASH";	///<
static const char fcGlbFault0002[] 		PROGMEM = "Неиспр.зап.FLASH";	///<
static const char fcGlbFault0004[] 		PROGMEM = "Неиспр.чт. PLIS";	///<
static const char fcGlbFault0008[] 		PROGMEM = "Неиспр.зап.PLIS";	///<
static const char fcGlbFault0010[] 		PROGMEM = "Неиспр.зап.2RAM";	///<
static const char fcGlbFault0020[] 		PROGMEM = "АК-Нет ответа";		///<
static const char fcGlbFault0020rzsk[] 	PROGMEM = "Нет сигнала ПРМ";	///<
static const char fcGlbFault0040[] 		PROGMEM = "АК-Снижен.запаса";	///<
static const char fcGlbFault0080[] 		PROGMEM = "Помеха в линии";		///<
static const char fcGlbFault0100[] 		PROGMEM = "Неиспр.DSP 2RAM ";	///<
static const char fcGlbFault0200[] 		PROGMEM = "Неиспр.чт. 2RAM ";	///<
static const char fcGlbFault0400[] 		PROGMEM = "Ток покоя";			///<
static const char fcGlbFault0400rzsk[] 	PROGMEM = "Установите часы";	///<
static const char fcGlbFault0800[]		PROGMEM = "Низкое напр.вых";	///<
static const char fcGlbFault1000[] 		PROGMEM = "Высокое напр.вых";	///<
static const char fcGlbFault2000[] 		PROGMEM = "Неиспр. МК УМ";		///<
static const char fcGlbFault4000[] 		PROGMEM = "ВЧ тракт восст.";	///<

// общие предупреждения
static const char fcGlbWarning01[] 		PROGMEM = "Установите часы";	///<
static const char fcGlbWarning02[] 		PROGMEM = "Отказ резерв.лин";	///<
static const char fcGlbWarning04[]		PROGMEM = "Кольцо 1 наруш.";	///<
static const char fcGlbWarning08[]		PROGMEM = "Кольцо 2 наруш.";	///<
static const char fcGlbWarning10[] 		PROGMEM = "Удал.ПОСТ неиспр";	///<
static const char fcGlbWarning20[] 		PROGMEM = "Удал.ПРД неиспр";	///<
static const char fcGlbWarning40[] 		PROGMEM = "Удал.ПРМ неиспр";	///<


// неисправности защиты
static const char fcDefFault0001[] 		PROGMEM = "Нет блока БСЗ";		///<
static const char fcDefFault0002[] 		PROGMEM = "Неиспр.верс.БСЗ";	///<
static const char fcDefFault0004[] 		PROGMEM = "Неиспр.перекл.";		///<
static const char fcDefFault0008[] 		PROGMEM = "Неиспр.зап. БСЗ";	///<
static const char fcDefFault0010[] 		PROGMEM = "АК-Нет ответа%S";	///<
static const char fcDefFault0010rzsko[]	PROGMEM = "Нет сигнала ПРМ";	///<
static const char fcDefFault0020[] 		PROGMEM = "Низкий ур. РЗ";		///<
static const char fcDefFault0040[] 		PROGMEM = "Неиспр.уд.ДФЗ%S";	///<
static const char fcDefFault0080[] 		PROGMEM = "Неиспр.уд.ВЫХ%S";	///<
static const char fcDefFault0100[] 		PROGMEM = "Неиспр.вход.ПУСК";	///<
static const char fcDefFault0200[] 		PROGMEM = "Неиспр.вход.СТОП";	///<
static const char fcDefFault0400[] 		PROGMEM = "Удал.без отв.%S";	///<
static const char fcDefFault0800[] 		PROGMEM = "Неиспр.цепь. ВЫХ";	///<
static const char fcDefFault1000[] 		PROGMEM = "Удал.обн.пом.%S";	///<
static const char fcDefFault2000[] 		PROGMEM = "Неиспр.зап. ВЫХ";	///<
static const char fcDefFault4000[] 		PROGMEM = "Длительн. помеха";	///<
static const char fcDefFault4000rzsk[]	PROGMEM = "Нет сигнала РЗ";		///<
static const char fcDefFault8000[] 		PROGMEM = "Неиспр. ДФЗ";		///<

// предупреждения защиты
static const char fcDefWarning01[] 		PROGMEM = "АК-Сн.запаса %S";	///<
static const char fcDefWarning01rzsk[] 	PROGMEM = "Низкий уров. РЗ";	///<
static const char fcDefWarning01rzsko[] PROGMEM = "Нет КЧ";				///<
static const char fcDefWarning02[] 		PROGMEM = "Нет сигнала МАН";	///<
static const char fcDefWarning04[] 		PROGMEM = "Порог по помехе";	///<
static const char fcDefWarning08[] 		PROGMEM = "Автоконтроль";		///<

// неисправности приемника
static const char fcPrmFault0001rzsk[] 	PROGMEM = "Нет блока БСК";		///<
static const char fcPrmFault0002rzsk[] 	PROGMEM = "Неиспр.верс. БСК";	///<
static const char fcPrmFault0004rzsk[] 	PROGMEM = "Нет КЧ";				///<
static const char fcPrmFault0008rzsk[] 	PROGMEM = "Прев.дл-сти.ком.";	///<
static const char fcPrmFault0100rzsk[] 	PROGMEM = "Неиспр.зап. БСК";	///<
static const char fcPrmFault0200rzsk[] 	PROGMEM = "Неиспр.выкл. КСК";	///<
static const char fcPrmFault0400rzsk[] 	PROGMEM = "Неиспр.вкл. КСК";	///<
static const char fcPrmFault0800rzsk[] 	PROGMEM = "Неиспр.контр.КСК";	///<
static const char fcPrmFault1000rzsk[] 	PROGMEM = "Предупр.нет КЧ";		///<
static const char fcPrmFault8000rzsk[] 	PROGMEM = "Прием блок. Ком";	///<

// предупреждения приемника
static const char fcPrmWarning01rzsk[] 	PROGMEM = "Сниж. уровня ПРМ";	///<
static const char fcPrmWarning01rzsko[] PROGMEM = "Нет КЧ";				///<
static const char fcPrmWarning02k400[] 	PROGMEM = "Ошибка работы ЦС";	///<
static const char fcPrmWarning04k400[]	PROGMEM = "Вход RX ЦС пуст";	///<

// неисправности передатчика
static const char fcPrdFault0001rzsk[] 	PROGMEM = "Нет блока БСК";		///<
static const char fcPrdFault0002rzsk[] 	PROGMEM = "Неиспр.верс. БСК";	///<
static const char fcPrdFault0100rzsk[] 	PROGMEM = "Неиспр.чт.команд";	///<
static const char fcPrdFault0200rzsk[] 	PROGMEM = "Неиспр.выкл.Тест";	///<
static const char fcPrdFault0400rzsk[] 	PROGMEM = "Неиспр.вкл. Тест";	///<
static const char fcPrdFault0800rzsk[] 	PROGMEM = "Неиспр.вход. КСК";	///<

// предупреждения передатчика
static const char fcPrdWarning02k400[] 	PROGMEM = "Ошибка работы ЦС";	///<
static const char fcPrdWarning04k400[] 	PROGMEM = "Вход RX ЦС пуст";	///<

/// Текущее значение параметра.
static const char fcValue[] 			PROGMEM = "Значение: ";
/// Диапазон допустимых значений параметра.
static const char fcRange[] 			PROGMEM = "Диапазон: ";
/// Номер приемника (для 3-ж концевой линии).
//static const char fcNumPrm[]			PROGMEM = "Номер ПРМ: %u/%u";
/// Диапазон допустимых значений параметра.
static const char fcRangeDec[] 			PROGMEM = "%d..%d%S";
/// Диапазон допустимых значений параметра: для списков.
static const char fcRangeList[] 		PROGMEM = "список";
/// Текущий параметр и общее их количество в данном пункте меню.
static const char fcNumPunkt[] 			PROGMEM = "Параметр:%u Всего:%u";


//	ЖУРНАЛ
/// Сообщение о том, что в текущем журнале нет записей.
static const char fcJrnEmpty[] 			PROGMEM = "ЖУРНАЛ ПУСТ";
/// Вывод номера текущей записи журнала и их количества.
static const char fcJrnNumEntries[] 	PROGMEM = "Запись %u / %u";
/// Вывод номера текущей записи журнала, кол-во событий в текущей записи и общего
/// количества записей в журнале (для оптики).
static const char fcJrnNumEntriesOpto[]	PROGMEM = "Запись %u / %u (%u)";
/// Режим устройства в котором была сделана запись.
static const char fcRegimeJrn[]			PROGMEM = "Режим:";
/// Состояние устройства в котором была сделана запись.
static const char fcStateJrn[] 			PROGMEM = "Состояние:";
/// Номер команды, для которой была сделана запись.
static const char fcNumComJrn[]			PROGMEM = "Команда: %u";
/// Дата, когда была сделана запись.
static const char fcDateJrn[]			PROGMEM = "Дата: %02u.%02u.%02u";
/// Время, когда была сделана запись.
static const char fcTimeJrn[]			PROGMEM = "Время: %02u:%02u:%02u.%03u";
/// Сообщение о том. что идет считывание выбранной записи журнала.
static const char fcJrnNotReady[]		PROGMEM = "СЧИТЫВАНИЕ ЗАПИСИ";
/// Вывод состояний входов/выходов защиты в момент записи журнала.
static const char fcSignalDefJrn[]		PROGMEM = "П%dС%dМ%d ПД%dПМ%dВЫХ%d";
/// Источник передаваемой комадны (для различия команд с ДВ и ЦС).
static const char fcJrnSourcePrd[GB_SOURCE_COM_MAX + 1][5] PROGMEM = {
		"",						///< команды с дискретных входов
		"(ЦС)",					///< команды с цифровой ретрансляции
		"(ОШ)"					///< ошибочное значение
};

/// Состояние команды ПРМ/ПРД для которого была сделана запись.
static const char fcJrnPrd[GB_STATE_COM_MAX + 1][21] PROGMEM = {
		"Окончание команды",	///<
		"Начало команды",		///<
		"Ошибочное значение"	///< ошибочное значение
};
/// Запись об окончании всех команд ПРМ/ПРД (оптика).
static const char fcJrnPrdOptoComNo[] PROGMEM = "Команд нет";
/// Запись о наличии команд на ПРД/ПРМ (оптика).
static const char fcJrnPrdOptoComYes[] PROGMEM = "Команда передается";

/// Записи журнала событий для К400.
static char fcJrnEventK400[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"Событие - 0",				///< 0 - ошибочное значение
		"Вкл. питания/перезап",		///< 1
		"Выключение питания",       ///< 2
		"Изменение режима раб",     ///< 3
		"Изменение параметров",    	///< 4
		"Неиспр теста ПРД",         ///< 5
		"Неиспр теста ПРМ",         ///< 6
		"Неиспр блока БСЗ",         ///< 7
		"Неиспр блока БСК",         ///< 8
		"Изменение дата/время",   	///< 9
		"Нет сигнала манипул.",     ///< 10
		"Неиспр выходной цепи",     ///< 11
		"Нет сигнала РЗ",           ///< 12
		"Отсут-е сигнала Пуск",    	///< 13
		"Отсут-е сигнала Стоп",    	///< 14
		"Неиспр чтения команд",     ///< 15
		"Событие - 16",          	///< 16
		"Неиспр. работы DSP",		///< 17
		"Восстан-е работы DSP",		///< 18
		"Низкое напр. выхода",    	///< 19
		"Высокое напр. выхода",    	///< 20
		"Нет КЧ 5 сек на %S",	    ///< 21
		"Нет КЧ на %S", 		    ///< 22
		"Восстан-е КЧ на %S",       ///< 23
		"Восст.ур. КЧ на %S",    	///< 24
		"Неиспр чт/зап 2RAM",    	///< 25
		"Неиспр чт/зап ПЛИС",    	///< 26
		"Неиспр чт/зап FLASH",   	///< 27
		"Неисправность часов",      ///< 28
		"Сниж.ур. КЧ на %S",       	///< 29
		"Ошибка работы ЦС",        	///< 30
		"Вход RX ЦС пуст",         	///< 31
		"Работа ЦС восстановл",    	///< 32
		"Событие - %d"          	///< ошибочное значение
};

/// Записи журнала событий для РЗСК.
static char fcJrnEventRZSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 01234567890123456789
		"Событие - %d",				///< 0 - ошибочное значение
		"Вкл. питания/перезап",		///< 1
		"Выключение питания",       ///< 2
		"Изменение режима раб",     ///< 3
		"Событие - %d",           	///< 4
		"Неиспр теста ПРД",         ///< 5
		"Неиспр теста ПРМ",         ///< 6
		"Неиспр блока БСЗ",         ///< 7
		"Неиспр блока БСК",         ///< 8
		"Неиспр перекл-ей БСЗ",   	///< 9
		"Нет сигнала манипул.",     ///< 10
		"Неиспр выходной цепи",     ///< 11
		"Нет РЗ 5 сек на %S",		///< 12
		"Отсут-е сигнала Пуск",    	///< 13
		"Отсут-е сигнала Стоп",    	///< 14
		"Неиспр чтения команд",     ///< 15
		"Сниж. ур. КЧ на %S",		///< 16
		"Неиспр. работы DSP",		///< 17
		"Восстан-е работы DSP",		///< 18
		"Низкое напр. выхода",    	///< 19
		"Высокое напр. выхода",    	///< 20
		"Нет КЧ 5 сек на %S",	    ///< 21
		"Нет КЧ на %S",				///< 22
		"Восстан-е КЧ на %S",	    ///< 23
		"Восст. ур.КЧ на %S",		///< 24
		"Неиспр чт/зап 2RAM",    	///< 25
		"Неиспр чт/зап ПЛИС",    	///< 26
		"Неиспр чт/зап FLASH",   	///< 27
		"Неисправность часов",      ///< 28
		"Сниж. ур. РЗ на %S",      	///< 29
		"Восстан-е РЗ на %S",      	///< 30
		"Нет РЗ на %S",         	///< 31
		"Восст. ур.РЗ на %S",    	///< 32
		"Событие - %d"          	///< ошибочное значение
};

/// Записи журнала событий для Р400М.
static char fcJrnEventR400_MSK[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
		"Событие - %d",				///< 0 - ошибочное значение
		"Неиспр чт/зап FLASH",   	///< 1
		"ВЧ тракт восстановл.",   	///< 2
		"Неиспр чт/зап ПЛИС",		///< 3
		"Автоконтроль",				///< 4
		"Ток покоя",				///< 5
		"Неиспр чт/зап 2RAM",		///< 6
		"Неиспр работы DSP",		///< 7
		"Восстан-е работы DSP",		///< 8
		"Низкое напр. выхода",    	///< 9
		"Высокое напр. выхода",    	///< 10
		"Нарушен обмен с УМ",		///< 11
		"Неисправность часов",      ///< 12
		"Нет блока БСЗ",         	///< 13
		"Ошибка версии БСЗ",        ///< 14
		"Неиспр перекл-ей БСЗ",   	///< 15
		"Нет сигнала МАН",			///< 16
		"Вкл.пит/Перезапуск",		///< 17
		"Изменение режима",		    ///< 18
		"Неиспр зап. вых.цепи",		///< 19
		"Ошиб контр. вых.цепи",		///< 20
		"АК - Снижение запаса",		///< 21
		"АК - Нет ответа",			///< 22
		"Отсут-е сигнала Пуск",    	///< 23
		"Отсут-е сигн Останов",    	///< 24
		"Выключение аппарата",      ///< 25
		"Помеха в полосе",         	///< 26
		"Неисправность ДФЗ",       	///< 27
		"Уд: АК - нет ответа",     	///< 28
		"Уд: Помеха в полосе",		///< 29
		"Уд: Неиспр. ДФЗ",         	///< 30
		"Уд: Неиспр. цепи вых",    	///< 31
		"Порог по помехе",         	///< 32
		"Событие - %d"          	///< ошибочное значение
};

/// Записи журнала событий для ОПТИКИ.
static char fcJrnEventOPTO[MAX_JRN_EVENT_VALUE + 1][21] PROGMEM = {
//		 12345678901234567890
		"Событие - %d",				///< 0 - ошибочное значение
		"Неиспр чт/зап FLASH",   	///< 1
		"Неиспр чт/зап 2RAM",		///< 2
		"Неиспр чт/зап ПЛИС",		///< 3
		"Событие - %d",				///< 4
		"Неиспр теста ПРД",         ///< 5
		"Неиспр выкл.тест ПРД",		///< 6
		"Неиспр теста ПРМ",			///< 7
		"Неиспр чт.команд БСК",		///< 8
		"Нет блока БСК",			///< 9
		"Ошибка версии БСК",        ///< 10
		"Отказ резерв. линии",      ///< 11
		"Неисправность часов",      ///< 12
		"Нет блока БСЗ",            ///< 13
		"Ошибка версии БСЗ",        ///< 14
		"Неиспр перекл-ей БСЗ",     ///< 15
		"Нет сигнала МАН",          ///< 16
		"Вкл.пит/Перезапуск",       ///< 17
		"Изменение режима",         ///< 18
		"Неиспр зап. вых.цепи",  	///< 19
		"Ошиб контр. вых.цепи", 	///< 20
		"Пропала КЧ",               ///< 21
		"Нет КЧ 5 секунд",          ///< 22
		"Отсут-е сигнала Пуск",     ///< 23
		"Отсут-е сигн Останов",     ///< 24
		"Выключение питания",       ///< 25
		"Пропала КЧ",               ///< 26
		"Нет КЧ 5 секунд",          ///< 27
		"Уд: пост неисправен",      ///< 28
		"Уд: ПРД неисправен",       ///< 29
		"Уд: ПРМ неисправен",       ///< 30
		"Изменение дата/время",		///< 31
		"Изменение параметров",		///< 32
		"Событие - %d"          	///< ошибочное значение
};

/// Сигналы в Тестах ПРМ/ПРД.
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
//		 1234567890
		"выкл.",		///<
		"КЧ", 			///<
		"КЧ1",			///<
		"КЧ2",			///<
		"КЧ3",			///<
		"КЧ4",			///<
		"КЧ1",			///< GB_SIGNAL_CF_NO_RZ
		"КЧ1 с блок",	///< GB_SIGNAL_CF_RZ
		"КЧ2",			///< GB_SIGNAL_CF2_NO_RZ
		"КЧ2 с блок",	///< GB_SIGNAL_CF2_RZ
		"РЗ",			///<
		"Команда1",		///< GB_SIGNAL_COM1_NO_RZ
		"Команда2",		///< GB_SIGNAL_COM2_NO_RZ
		"Команда3",		///< GB_SIGNAL_COM3_NO_RZ
		"Команда4",		///< GB_SIGNAL_COM4_NO_RZ
		"Ком1+блок",	///< GB_SIGNAL_COM1_RZ
		"Ком2+блок",	///< GB_SIGNAL_COM2_RZ
		"Ком3+блок",	///< GB_SIGNAL_COM3_RZ
		"Ком4+блок",	///< GB_SIGNAL_COM4_RZ
		"Команда1",		///<
		"Команда2",		///<
		"Команда3",		///<
		"Команда4",		///<
		"Команда5",		///<
		"Команда6",		///<
		"Команда7",		///<
		"Команда8",		///<
		"Команда9",		///<
		"Команда10",	///<
		"Команда11",	///<
		"Команда12",	///<
		"Команда13",	///<
		"Команда14",	///<
		"Команда15",	///<
		"Команда16",	///<
		"Команда17",	///<
		"Команда18",	///<
		"Команда19",	///<
		"Команда20",	///<
		"Команда21",	///<
		"Команда22",	///<
		"Команда23",	///<
		"Команда24",	///<
		"Команда25",	///<
		"Команда26",	///<
		"Команда27",	///<
		"Команда28",	///<
		"Команда29",	///<
		"Команда30",	///<
		"Команда31",	///<
		"Команда32",	///<
		"КЧ и РЗ",		///< GB_SIGNAL_CF_RZ_R400M
		"КЧ1 и РЗ",		// GB_SIGNAL_CF1_RZ_R400M
		"КЧ2 и РЗ",		// GB_SIGNAL_CF2_RZ_R400M
		"КЧ3 и РЗ",		// GB_SIGNAL_CF3_RZ_R400M
		"КЧ4 и РЗ",		// GB_SIGNAL_CF4_RZ_R400M
		"ошибка"		///< ошибочное значение
};


