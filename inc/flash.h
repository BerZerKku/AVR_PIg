/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: Хозяин
 */

#include <avr/pgmspace.h>

static const char fcNull[] PROGMEM = "";

// измеряемые параметры
static const char fcUout[] 	PROGMEM = "U=%02u.%01uВ";
static const char fcIout[] 	PROGMEM	= "I=%03uмА";
static const char fcRout[] 	PROGMEM = "R=%03uОм";
static const char fcUz[]	PROGMEM	= "Uз=%02dдБ";
static const char fcUcf[] 	PROGMEM	= "Uк=%02dдБ";
static const char fcUn[] 	PROGMEM	= "Uш=%02dдБ";
static const char fcDate[] 	PROGMEM = "%02u.%02u.%02u";
static const char fcTime[] 	PROGMEM = "%02u:%02u:%02u";

// имена устройств аппарата
static const char fcDeviceName00[] PROGMEM = "ЗАЩ";
static const char fcDeviceName01[] PROGMEM = "ПРМ";
static const char fcDeviceName02[] PROGMEM = "ПРД";

// режимы устройств
// кол-во режимов (без учета "Ошибка") должно совпадать с MAX_NUM_REGIME
static const char fcRegime[] [8] PROGMEM =
{
	"Выведен",
	"Готов",
	"Введен",
	"Речь",
	"Тест 1",
	"Тест 2",
	"Ошибка"
};

// состояния устройств
// кол-во состояний для всех устройств должно совпадать с MAX_NUM_DEVICE_STATE
// неизвестное состояние
static const char fcUnknownSost[] PROGMEM = "Ошибка";
// состояния Защиты
static const char fcDefSost00[] PROGMEM =	"Исходн.";
static const char fcDefSost01[] PROGMEM =	"Контроль";
static const char fcDefSost02[] PROGMEM =	"Пуск";
static const char fcDefSost03[] PROGMEM =	"Работа";
static const char fcDefSost04[] PROGMEM =	"Неиспр.";
static const char fcDefSost05[] PROGMEM =	"П.неиспр";
static const char fcDefSost06[] PROGMEM =	"Ожидание";
static const char fcDefSost07[] PROGMEM =	"Нал.пуск";
static const char fcDefSost08[] PROGMEM = 	"?0x08?";
static const char fcDefSost09[] PROGMEM =	"Нет РЗ";
static const char fcDefSost10[] PROGMEM =	"Речь";
// состояния Приемника
static const char fcPrmSost00[] PROGMEM = 	"Исходн.";
static const char fcPrmSost01[] PROGMEM = 	"ПРМ КЧ";
static const char fcPrmSost02[] PROGMEM = 	"ПРМ ПА";
static const char fcPrmSost03[] PROGMEM = 	"Нет КЧ";
static const char fcPrmSost04[] PROGMEM = 	"Неиспр.";
static const char fcPrmSost05[] PROGMEM = 	"П.неиспр";
static const char fcPrmSost06[] PROGMEM = 	"Ожидание";
static const char fcPrmSost07[] PROGMEM = 	"БЛК КМ";
static const char fcPrmSost08[] PROGMEM = 	"?0x08?";
static const char fcPrmSost09[] PROGMEM = 	"?0x09?";
static const char fcPrmSost10[] PROGMEM = 	"Речь";
// состояния Передатчика
static const char fcPrdSost00[] PROGMEM = 	"Исходн.";
static const char fcPrdSost01[] PROGMEM =	"ПРД КЧ";
static const char fcPrdSost02[] PROGMEM =	"ПРД ПА";
static const char fcPrdSost03[] PROGMEM =	"Нет КЧ";
static const char fcPrdSost04[] PROGMEM =	"Неиспр.";
static const char fcPrdSost05[] PROGMEM =	"П.неиспр";
static const char fcPrdSost06[] PROGMEM =	"Ожидание";
static const char fcPrdSost07[] PROGMEM = 	"?0x07?";
static const char fcPrdSost08[] PROGMEM = 	"?0x08?";
static const char fcPrdSost09[] PROGMEM =	"Систем.";
static const char fcPrdSost10[] PROGMEM =	"Речь";

// надписи для неиспользуемых в текущем аппарате кодов
static const char fcUnknownFault[] PROGMEM = 		"Неисправность";
static const char fcUnknownWarning[] PROGMEM = 		"Предупреждение";

// строка вывода кода неисправности/предупреждения
// %c - g для глобальной, l - для локальной
static const char fcFaults[] PROGMEM 	= 			"Неиспр. %c-%04X";
static const char fcWarnings[] PROGMEM  =	 		"Предупр. %c-%04X";

// общие неисправности
static const char fcGlbFault0001[] PROGMEM = 		"Неиспр.чт. FLASH";
static const char fcGlbFault0002[] PROGMEM = 		"Неиспр.зап.FLASH";
static const char fcGlbFault0004[] PROGMEM = 		"Неиспр.чт. PLIS";
static const char fcGlbFault0008[] PROGMEM = 		"Неиспр.зап.PLIS";
static const char fcGlbFault0010[] PROGMEM = 		"Неиспр.зап.2RAM";
static const char fcGlbFault0020[] PROGMEM = 		"АК-Нет ответа";
static const char fcGlbFault0020rzsk[] PROGMEM =	"Нет сигнала ПРМ";
static const char fcGlbFault0040[] PROGMEM = 		"АК-Снижен.запаса";
static const char fcGlbFault0080[] PROGMEM =		"Помеха в линии";
static const char fcGlbFault0100[] PROGMEM = 		"Неиспр.DSP 2RAM ";
static const char fcGlbFault0200[] PROGMEM = 		"Неиспр.чт. 2RAM ";
static const char fcGlbFault0400[] PROGMEM = 		"Ток покоя";
static const char fcGlbFault0400rzsk[] PROGMEM = 	"Установите часы";
static const char fcGlbFault0800[] PROGMEM = 		"Низкое напр.вых";
static const char fcGlbFault1000[] PROGMEM = 		"Высокое напр.вых";
static const char fcGlbFault2000[] PROGMEM = 		"Неиспр. МК УМ";
static const char fcGlbFault4000[] PROGMEM = 		"ВЧ тракт восст.";

// общие предупреждения
static const char fcGlbWarning01[] PROGMEM = 		"Установите часы";
static const char fcGlbWarning02[] PROGMEM = 		"Отказ резерв.лин";
static const char fcGlbWarning10[] PROGMEM = 		"Удал.ПОСТ неиспр";
static const char fcGlbWarning20[] PROGMEM = 		"Удал.ПРД неиспр";
static const char fcGlbWarning40[] PROGMEM = 		"Удал.ПРМ неиспр";

// неисправности защиты
static const char fcDefFault0001[] PROGMEM =		"Нет блока БСЗ";
static const char fcDefFault0002[] PROGMEM = 		"Неиспр.верс.БСЗ";
static const char fcDefFault0004[] PROGMEM = 		"Неиспр.перекл.";
static const char fcDefFault0008[] PROGMEM = 		"Неиспр.зап. БСЗ";
static const char fcDefFault0010[] PROGMEM = 		"АК-Нет ответа";
static const char fcDefFault0010rzsk[] PROGMEM =	"Нет сигнала ПРМ";
static const char fcDefFault0020[] PROGMEM =		"Низкий ур. РЗ";
static const char fcDefFault0040[] PROGMEM =		"Неиспр.удал. ДФЗ";
static const char fcDefFault0080[] PROGMEM =		"Неиспр.удал. ВЫХ";
static const char fcDefFault0100[] PROGMEM =		"Неиспр.вход.ПУСК";
static const char fcDefFault0200[] PROGMEM =		"Неиспр.вход.СТОП";
static const char fcDefFault0400[] PROGMEM =		"Уд.ПОСТ без отв.";
static const char fcDefFault0800[] PROGMEM = 		"Неиспр.цепь. ВЫХ";
static const char fcDefFault1000[] PROGMEM = 		"Уд.ПОСТ обн.пом.";
static const char fcDefFault2000[] PROGMEM =		"Неиспр.зап. ВЫХ";
static const char fcDefFault4000[] PROGMEM =		"Длительн. помеха";
static const char fcDefFault8000[] PROGMEM =		"Неиспр. ДФЗ";

// предупреждения защиты
static const char fcDefWarning01[] PROGMEM = 		"АК-Снижен.запаса";
static const char fcDefWarning01rzsk[] PROGMEM = 	"Низкий уров. РЗ";
static const char fcDefWarning01rzsko[] PROGMEM =	"Нет КЧ";
static const char fcDefWarning02[] PROGMEM = 		"Нет сигнала МАН";
static const char fcDefWarning04[] PROGMEM = 		"Порог по помехе";
static const char fcDefWarning08[] PROGMEM = 		"Автоконтроль";

// неисправности приемника
static const char fcPrmFault0001rzsk[] PROGMEM =	"Нет блока БСК";
static const char fcPrmFault0002rzsk[] PROGMEM =	"Неиспр.верс. БСК";
static const char fcPrmFault0004rzsk[] PROGMEM =	"Нет КЧ";
static const char fcPrmFault0008rzsk[] PROGMEM =	"Прев.дл-сти.ком.";
static const char fcPrmFault0100rzsk[] PROGMEM =	"Неиспр.зап. БСК";
static const char fcPrmFault0200rzsk[] PROGMEM =	"Неиспр.выкл. КСК";
static const char fcPrmFault0400rzsk[] PROGMEM =	"Неиспр.вкл. КСК";
static const char fcPrmFault0800rzsk[] PROGMEM =	"Неиспр.контр.КСК";
static const char fcPrmFault1000rzsk[] PROGMEM =	"Предупр.нет КЧ";
static const char fcPrmFault8000rzsk[] PROGMEM =	"Прием блок. Ком";

// предупреждения приемника
static const char fcPrmWarning01rzsk[] PROGMEM = 	"Сниж. уровня ПРМ";
static const char fcPrmWarning01rzsko[] PROGMEM = 	"Нет КЧ";

// неисправности передатчика
static const char fcPrdFault0001rzsk[] PROGMEM = 	"Нет блока БСК";
static const char fcPrdFault0002rzsk[] PROGMEM = 	"Неиспр.верс. БСК";
static const char fcPrdFault0100rzsk[] PROGMEM = 	"Неиспр.чт.команд";
static const char fcPrdFault0200rzsk[] PROGMEM = 	"Неиспр.выкл.Тест";
static const char fcPrdFault0400rzsk[] PROGMEM =	"Неиспр.вкл. Тест";
static const char fcPrdFault0800rzsk[] PROGMEM = 	"Неиспр.вход. КСК";

