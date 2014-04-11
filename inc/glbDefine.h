/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 * 	Максимальное значение для строковых параметров на 1 больше возможного,
 * 	для хранения в массиве ошибочного значения и удобства пользования
 * 	перечислениями.
 * 	Так что для ввода строкового значения максимум должен быть меньше на 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "debug.h"

/// пароль администратора
#define PASSWORD_ADMIN 6352

/// пароль пользователя по умолчанию
#define PASSWORD_USER 0

/// версия текущей прошивки
#define VERS 0x0107

/// максимально кол-во команд на прием (должно быть кратно 8)
#define MAX_NUM_COM_PRM 32

/// максимальное кол-во команд на передачу (должно быть кратно 8)
#define MAX_NUM_COM_PRD 32

/// преобразование двух CHAR в INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// преобразование двоично-десятичного числа в целое
#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))

/// преобразование целого числа в двоично-десятичный вид
#define BIN_TO_BCD(val) (((val / 10) << 4) + (val % 10))

/// максимально возможное кол-во состояний устройств
#define MAX_NUM_DEVICE_STATE 13

/// максимальное кол-во неисправностей для любого устройства
#define MAX_NUM_FAULTS 16

/// максимальное кол-во предупреждений для любого устройства
#define MAX_NUM_WARNINGS 16

///	максимальное кол-во быстрых команды
// на данный момент надо только для ТЕСТов РЗСК, т.к. там посылаются 2 команды
#define MAX_NUM_FAST_COM 2

/// максимальное кол-во команд в первом буфере
#define MAX_NUM_COM_BUF1 5

/// максимальное кол-во команд во втором буфере
#define MAX_NUM_COM_BUF2 5

/// максимальное кол-во сигналов в тестах
#define MAX_NUM_TEST_SIGNAL 40

/// Минимальные, максимальные значения параметров приемника и их дискретность
/// ЗАЩИТА
/// тип защиты
#define DEF_TYPE_MIN		0
#define DEF_TYPE_MAX		8
/// допустимое время без манипуляции
#define DEF_T_NO_MAN_MIN	0
#define DEF_T_NO_MAN_MAX	99
#define DEF_T_NO_MAN_DISC	1
#define DEF_T_NO_MAN_FRACT	1
#define DEF_T_NO_MAN_MIN_F	(DEF_T_NO_MAN_MIN / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_MAX_F	(DEF_T_NO_MAN_MAX / DEF_T_NO_MAN_FRACT)
#define DEF_T_NO_MAN_DISC_F	(DEF_T_NO_MAN_DISC / DEF_T_NO_MAN_FRACT)
/// перекрытие импульсов
#define DEF_OVERLAP_MIN		0
#define DEF_OVERLAP_MIN2	18	// минимум в Р400м
#define DEF_OVERLAP_MAX		54
#define DEF_OVERLAP_DISC	2
#define DEF_OVERLAP_FRACT	1
#define DEF_OVERLAP_MIN_F	(DEF_OVERLAP_MIN / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_MAX_F	(DEF_OVERLAP_MAX / DEF_OVERLAP_FRACT)
#define DEF_OVERLAP_DISC_F	(DEF_OVERLAP_DISC / DEF_OVERLAP_FRACT)
/// компенсация задержки в линии
#define DEF_DELAY_MIN		0
#define DEF_DELAY_MAX		18
#define DEF_DELAY_DISC		2
#define DEF_DELAY_FRACT		1
#define DEF_DELAY_MIN_F		(DEF_DELAY_MIN / DEF_DELAY_FRACT)
#define DEF_DELAY_MAX_F		(DEF_DELAY_MAX / DEF_DELAY_FRACT)
#define DEF_DELAY_DISC_F	(DEF_DELAY_DISC / DEF_DELAY_FRACT)
/// порог предупреждения по РЗ
#define DEF_RZ_THRESH_MIN	0
#define DEF_RZ_THRESH_MAX	16
#define DEF_RZ_THRESH_DISC	1
#define DEF_RZ_THRESH_FRACT	1
#define DEF_RZ_THRESH_MIN_F	(DEF_RZ_THRESH_MIN / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_MAX_F	(DEF_RZ_THRESH_MAX / DEF_RZ_THRESH_FRACT)
#define DEF_RZ_THRESH_DISC_F (DEF_RZ_THRESH_DISC / DEF_RZ_THRESH_FRACT)
/// загрубление чувствительности по РЗ
#define DEF_RZ_DEC_MIN		0
#define DEF_RZ_DEC_MAX		32
#define DEF_RZ_DEC_DISC		1
#define DEF_RZ_DEC_FRACT	1
#define DEF_RZ_DEC_MIN_F	(DEF_RZ_DEC_MIN / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_MAX_F	(DEF_RZ_DEC_MAX / DEF_RZ_DEC_FRACT)
#define DEF_RZ_DEC_DISC_F	(DEF_RZ_DEC_DISC / DEF_RZ_DEC_FRACT)
/// тип приемника
#define DEF_PRM_TYPE_MIN	0
#define DEF_PRM_TYPE_MAX	3

/// ПРМ
/// время включения
#define PRM_TIME_ON_MIN		0
#define PRM_TIME_ON_MAX		10
#define PRM_TIME_ON_DISC	1
#define PRM_TIME_ON_FRACT	1
#define PRM_TIME_ON_MIN_F	(PRM_TIME_ON_MIN / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_MAX_F	(PRM_TIME_ON_MAX / PRM_TIME_ON_FRACT)
#define PRM_TIME_ON_DISC_F 	(PRM_TIME_ON_DISC / PRM_TIME_ON_FRACT)
/// задержка на выключение
#define PRM_TIME_OFF_MIN	0
#define PRM_TIME_OFF_MAX	1000
#define PRM_TIME_OFF_DISC	50
#define PRM_TIME_OFF_FRACT	10
#define PRM_TIME_OFF_MIN_F	(PRM_TIME_OFF_MIN / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_MAX_F	(PRM_TIME_OFF_MAX / PRM_TIME_OFF_FRACT)
#define PRM_TIME_OFF_DISC_F	(PRM_TIME_OFF_DISC/ PRM_TIME_OFF_FRACT)
/// ПРД
/// время включения (задержка срабатывания дискретного входа)
/// было от 5 до 20, изменено по требованию АГ
#define PRD_TIME_ON_MIN		5
#define PRD_TIME_ON_MAX		20
#define PRD_TIME_ON_DISC	1
#define PRD_TIME_ON_FRACT	1
#define PRD_TIME_ON_MIN_F	(PRD_TIME_ON_MIN / PRM_TIME_ON_FRACT)
#define PRD_TIME_ON_MAX_F	(PRD_TIME_ON_MAX / PRM_TIME_ON_FRACT)
#define PRD_TIME_ON_DISC_F	(PRD_TIME_ON_DISC / PRM_TIME_ON_FRACT)
/// длительность команды для ВЧ
#define PRD_DURAT_L_MIN		20
#define PRD_DURAT_L_MAX		100
#define PRD_DURAT_L_DISC	1
#define PRD_DURAT_L_FRACT	1
#define PRD_DURAT_L_MIN_F	(PRD_DURAT_L_MIN / PRD_DURAT_L_FRACT)
#define PRD_DURAT_L_MAX_F	(PRD_DURAT_L_MAX / PRD_DURAT_L_FRACT)
#define PRD_DURAT_L_DISC_F	(PRD_DURAT_L_DISC / PRD_DURAT_L_FRACT)
/// длительность команды для ОПТИКИ
#define PRD_DURAT_O_MIN		20
#define PRD_DURAT_O_MAX		500
#define PRD_DURAT_O_DISC	10
#define PRD_DURAT_O_FRACT	10
#define PRD_DURAT_O_MIN_F	(PRD_DURAT_O_MIN / PRD_DURAT_O_FRACT)
#define PRD_DURAT_O_MAX_F	(PRD_DURAT_O_MAX / PRD_DURAT_O_FRACT)
#define PRD_DURAT_O_DISC_F	(PRD_DURAT_O_DISC / PRD_DURAT_O_FRACT)
/// ОБЩИЕ
/// номер аппарата
#define GLB_DEV_NUM_MIN		1
#define GLB_DEV_NUM_MAX 	3
#define GLB_DEV_NUM_DISC 	1
#define GLB_DEV_NUM_FRACT	1
#define GLB_DEV_NUM_MIN_F	(GLB_DEV_NUM_MIN / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_MAX_F 	(GLB_DEV_NUM_MAX / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_DISC_F	(GLB_DEV_NUM_DISC / GLB_DEV_NUM_FRACT)
/// порог предупреждения по КЧ
#define GLB_CF_THRESH_MIN	0
#define GLB_CF_THRESH_MAX	22
#define GLB_CF_THRESH_DISC	1
#define GLB_CF_THRESH_FRACT	1
#define GLB_CF_THRESH_MIN_F	(GLB_CF_THRESH_MIN / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_MAX_F	(GLB_CF_THRESH_MAX / GLB_CF_THRESH_FRACT)
#define GLB_CF_THRESH_DISC_F (GLB_CF_THRESH_DISC / GLB_CF_THRESH_FRACT)
/// время перезапуска
#define GLB_T_RERUN_MIN		0
#define GLB_T_RERUN_MAX		5
#define GLB_T_RERUN_DISC	1
#define GLB_T_RERUN_FRACT	1
#define GLB_T_RERUN_MIN_F	(GLB_T_RERUN_MIN / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_MAX_F	(GLB_T_RERUN_MAX / GLB_T_RERUN_FRACT)
#define GLB_T_RERUN_DISC_F	(GLB_T_RERUN_DISC / GLB_T_RERUN_FRACT)
/// уменьшение усиления канала
#define GLB_IN_DEC_MIN		0
#define GLB_IN_DEC_MAX		22
#define GLB_IN_DEC_DISC 	1
#define GLB_IN_DEC_FRACT	1
#define GLB_IN_DEC_MIN_F	(GLB_IN_DEC_MIN / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_MAX_F	(GLB_IN_DEC_MAX / GLB_IN_DEC_FRACT)
#define GLB_IN_DEC_DISC_F	(GLB_IN_DEC_DISC / GLB_IN_DEC_FRACT)
/// сетевой адрес
#define GLB_NET_ADR_MIN		0
#define GLB_NET_ADR_MAX		255
#define GLB_NET_ADR_DISC 	1
#define GLB_NET_ADR_FRACT	1
#define GLB_NET_ADR_MIN_F	(GLB_NET_ADR_MIN / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_MAX_F	(GLB_NET_ADR_MAX / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_DISC_F	(GLB_NET_ADR_DISC / GLB_NET_ADR_FRACT)
/// номинальное выходное напряжение
#define GLB_U_OUT_NOM_MIN	18
#define GLB_U_OUT_NOM_MAX	50
#define GLB_U_OUT_NOM_DISC 	1
#define GLB_U_OUT_NOM_FRACT	1
#define GLB_U_OUT_NOM_MIN_F	(GLB_U_OUT_NOM_MIN / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_MAX_F	(GLB_U_OUT_NOM_MAX / GLB_U_OUT_NOM_FRACT)
#define GLB_U_OUT_NOM_DISC_F (GLB_U_OUT_NOM_DISC / GLB_U_OUT_NOM_FRACT)
// частота
#define GLB_FREQ_MIN		16
#define GLB_FREQ_MAX		1000
#define GLB_FREQ_DISC 		1
#define GLB_FREQ_FRACT		1
#define GLB_FREQ_MIN_F		(GLB_FREQ_MIN / GLB_FREQ_FRACT)
#define GLB_FREQ_MAX_F		(GLB_FREQ_MAX / GLB_FREQ_FRACT)
#define GLB_FREQ_DISC_F 	(GLB_FREQ_DISC / GLB_FREQ_FRACT)
// тип детектора
#define GLB_DETECTOR_MIN	1
#define GLB_DETECTOR_MAX	3
#define GLB_DETECTOR_DISC	1
#define GLB_DETECTOR_FRACT	1
#define GLB_DETECTOR_MIN_F	(GLB_DETECTOR_MIN / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_MAX_F	(GLB_DETECTOR_MAX / GLB_DETECTOR_FRACT)
#define GLB_DETECTOR_DISC_F (GLB_DETECTOR_DISC / GLB_DETECTOR_FRACT)
// снижение ответа АК (ПВЗЛ)
#define GLB_AC_IN_DEC_MIN	0
#define GLB_AC_IN_DEC_MAX	20
#define GLB_AC_IN_DEC_DISC 	1
#define GLB_AC_IN_DEC_FRACT	1
#define GLB_AC_IN_DEC_MIN_F	(GLB_AC_IN_DEC_MIN / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_MAX_F	(GLB_AC_IN_DEC_MAX / GLB_AC_IN_DEC_FRACT)
#define GLB_AC_IN_DEC_DISC_F (GLB_AC_IN_DEC_DISC / GLB_AC_IN_DEC_FRACT)
// коррекция тока
// при вводе минимум 0, т.к. вводится реальное значение напряжения
#define GLB_COR_I_DEC_MIN	-999
#define GLB_COR_I_DEC_MAX	999
#define GLB_COR_I_DEC_DISC	1
#define GLB_COR_I_DEC_FRACT	1
#define GLB_COR_I_DEC_MIN_F	(GLB_COR_I_DEC_MIN / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_MAX_F	(GLB_COR_I_DEC_MAX / GLB_COR_I_DEC_FRACT)
#define GLB_COR_I_DEC_DISC_F (GLB_COR_I_DEC_DISC / GLB_COR_I_DEC_FRACT)
// коррекция напряжения (в 10 раз больше)
// при вводе минимум 0, т.к. вводится реальное значение напряжения
#define GLB_COR_U_DEC_MIN	-400
#define GLB_COR_U_DEC_MAX	400
#define GLB_COR_U_DEC_DISC	1
#define GLB_COR_U_DEC_FRACT	1
#define GLB_COR_U_DEC_MIN_F	(GLB_COR_U_DEC_MIN / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_MAX_F	(GLB_COR_U_DEC_MAX / GLB_COR_U_DEC_FRACT)
#define GLB_COR_U_DEC_DISC_F (GLB_COR_U_DEC_DISC / GLB_COR_U_DEC_FRACT)
// допустимые провалы (ПВЗУ-Е)
#define GLB_PVZUE_FAIL_MIN		0
#define GLB_PVZUE_FAIL_MAX		90
#define GLB_PVZUE_FAIL_DISC		18
#define GLB_PVZUE_FAIL_FRACT	1
#define GLB_PVZUE_FAIL_MIN_F	(GLB_PVZUE_FAIL_MIN / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_MAX_F	(GLB_PVZUE_FAIL_MAX / GLB_PVZUE_FAIL_FRACT)
#define GLB_PVZUE_FAIL_DISC_F	(GLB_PVZUE_FAIL_DISC / GLB_PVZUE_FAIL_FRACT)
// порог по помехе (ПВЗУ-Е)
#define GLB_PVZUE_N_TH_MIN		0
#define GLB_PVZUE_N_TH_MAX		255
#define GLB_PVZUE_N_TH_DISC		1
#define GLB_PVZUE_N_TH_FRACT	1
#define GLB_PVZUE_N_TH_MIN_F	(GLB_PVZUE_N_TH_MIN / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_MAX_F	(GLB_PVZUE_N_TH_MAX / GLB_PVZUE_N_TH_FRACT)
#define GLB_PVZUE_N_TH_DISC_F	(GLB_PVZUE_N_TH_DISC / GLB_PVZUE_N_TH_FRACT)
// допустимая помеха (ПВЗУ-Е)
#define GLB_PVZUE_N_LVL_MIN		18
#define GLB_PVZUE_N_LVL_MAX		90
#define GLB_PVZUE_N_LVL_DISC	18
#define GLB_PVZUE_N_LVL_FRACT	1
#define GLB_PVZUE_N_LVL_MIN_F	(GLB_PVZUE_N_LVL_MIN / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_MAX_F	(GLB_PVZUE_N_LVL_MAX / GLB_PVZUE_N_LVL_FRACT)
#define GLB_PVZUE_N_LVL_DISC_F	(GLB_PVZUE_N_LVL_DISC / GLB_PVZUE_N_LVL_FRACT)

/// максимальное и минимальный код типа событий в журнале событий
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 33

/// кол-во записей в журнале событый
#define GLB_JRN_EVENT_K400_MAX 		512
#define GLB_JRN_EVENT_R400_MSK_MAX 	512
#define GLB_JRN_EVENT_RZSK_MAX 		256
#define GLB_JRN_EVENT_OPTO_MAX 		256

/// кол-во записей в журнале защиты
#define GLB_JRN_DEF_K400_MAX 		0
#define GLB_JRN_DEF_R400_MSK_MAX 	1024
#define GLB_JRN_DEF_RZSK_MAX 		256
#define GLB_JRN_DEF_OPTO_MAX 		2048

/// кол-во записей в журнале приемника
#define GLB_JRN_PRM_K400_MAX 		512
#define GLB_JRN_PRM_RZSK_MAX 		256
#define GLB_JRN_PRM_OPTO_MAX		256

/// кол-во записей в журнале передатчика
#define GLB_JRN_PRD_K400_MAX 		512
#define GLB_JRN_PRD_RZSK_MAX 		256
#define GLB_JRN_PRD_OPTO_MAX		256

/// максимально возможное кол-во записей в одном архиве
#define GLB_JRN_MAX 2048

/// Тип аппарата
enum eGB_TYPE_DEVICE {
	AVANT_NO = 0,	// ошибочное значение
	AVANT_R400M,	//
	AVANT_RZSK,		//
	AVANT_OPTO,		//
	AVANT_K400,		//
	AVANT_MAX

// TODO ОПТИКА - у Женьки программа одна, так что можно не делить
// подстравиваться только под наличие команд (и их кол-ва) и защиты.
// на данный момент сделаны только применик/передатчик 8 команд
};

/// Устройство
enum eGB_DEVICE {
	GB_DEVICE_MIN = 0,
	GB_DEVICE_DEF = 0,
	GB_DEVICE_PRM,
	GB_DEVICE_PRD,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// Микросхемы имеющие прошивку
enum eGB_IC {
	GB_IC_MIN = 0,
	GB_IC_BSP_MCU = 0,
	GB_IC_BSP_DSP,
	GB_IC_PI_MCU,
	GB_IC_BSK_PLIS_PRD1,
	GB_IC_BSK_PLIS_PRD2,
	GB_IC_BSK_PLIS_PRM1,
	GB_IC_BSK_PLIS_PRM2,
	GB_IC_MAX
};

/// Тип канала связи
enum eGB_TYPE_LINE {
	GB_TYPE_LINE_MIN = 1,
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTO = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// Кол-во аппаратов в линии
// 	Учесть что 1 и 2 используются для параметров в 3-х концевой. Не менять !!!
enum eGB_NUM_DEVICES {
	GB_NUM_DEVICES_MIN = 1,
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// Совместимость
enum eGB_COMPATIBILITY {
	GB_COMPATIBILITY_MIN = 0,
	GB_COMPATIBILITY_AVANT = 0,
	GB_COMPATIBILITY_PVZ90 = 1,
	GB_COMPATIBILITY_AVZK80 = 2,
	GB_COMPATIBILITY_PVZUE = 3,
	GB_COMPATIBILITY_PVZL = 4,
	GB_COMPATIBILITY_MAX
};

/// Тип автоконтроля
enum eGB_TYPE_AC {
	GB_TYPE_AC_MIN = 1, 		//
	GB_TYPE_AC_AUTO_NORM = 1,	//	АК нормальный / АК включен
	GB_TYPE_AC_AUTO_REPEAT = 2,	//	АК повторный
	GB_TYPE_AC_FAST = 3,		//	АК ускоренный
	GB_TYPE_AC_OFF = 4,			//	АК выключен
	GB_TYPE_AC_CHECK = 5,		// 	АК беглый
	GB_TYPE_AC_PUSK_SELF = 6,	// 	Пуск АК свой / АК испытания / АК контр.пров.
	GB_TYPE_AC_PUSK = 7,		//	АК пуск
	GB_TYPE_AC_ONE_SIDE = 8,	// 	АК односторонний
	GB_TYPE_AC_MAX				// 	максимальное принимаемое значение
};

/// Частоты ПРМ и ПРД в ПВЗЛ
enum eGB_PVZL_FREQ {
	GB_PVZL_FREQ_MIN 	= 0,
	GB_PVZL_FREQ_M1000 	= 0,
	GB_PVZL_FREQ_M500 	= 1,
	GB_PVZL_FREQ_0 		= 2,
	GB_PVZL_FREQ_P500 	= 3,
	GB_PVZL_FREQ_P1000 	= 4,
	GB_PVZL_FREQ_MAX
};

// Протокол обмена (ПВЗУ-Е)
enum eGB_PVZUE_PROTOCOL {
	GB_PVZUE_PROTOCOL_MIN = 1,
	GB_PVZUE_PROTOCOL_FAST = 1,
	GB_PVZUE_PROTOCOL_SLOW = 2,
	GB_PVZUE_PROTOCOL_MAX
};

// Тип автоконтроля (ПВЗУ-Е)
enum eGB_PVZUE_TYPE_AC {
	GB_PVZUE_TYPE_AC_MIN = 1,
	GB_PVZUE_TYPE_AC_ALARM = 1,
	GB_PVZUE_TYPE_AC_NORM = 2,
	GB_PVZUE_TYPE_AC_CALM = 3,
	GB_PVZUE_TYPE_AC_MAX
};

// Признак четности (ПВЗУ-Е)
enum eGB_PVZUE_PARITY {
	GB_PVZUE_PARITY_MIN = 1,
	GB_PVZUE_PARITY_ON = 1,
	GB_PVZUE_PARITY_OFF = 2,
	GB_PVZUE_PARITY_MAX
};

/// Режимы работы
enum eGB_REGIME {
	GB_REGIME_MIN = 0, 				//
	GB_REGIME_DISABLED = 0,			// выведен
	GB_REGIME_READY,				// готов
	GB_REGIME_ENABLED,				// введен
	GB_REGIME_TALK,					// речь
	GB_REGIME_TEST_1,				// тест ПРД
	GB_REGIME_TEST_2,				// тест ПРМ
	GB_REGIME_MAX,
};

// Режимы работы для ввода с клавиатуры
// порядок не совпадает с eGB_REGIME
enum eGB_REGIME_ENTER {
	GB_REGIME_ENTER_MIN = 0,
	GB_REGIME_ENTER_DISABLED = 0,
	GB_REGIME_ENTER_ENABLED = 1,
	GB_REGIME_ENTER_TEST_1,
	GB_REGIME_ENTER_TEST_2,
	GB_REGIME_ENTER_MAX
};

/// Команды
enum eGB_COM {
										// + означает что команда задукоментирована
	GB_COM_NO = 0,
	GB_COM_DEF_GET_DEF_TYPE = 0x01,		// +
	GB_COM_DEF_GET_LINE_TYPE = 0x02,	// +
	GB_COM_DEF_GET_T_NO_MAN = 0x03,		// +
	GB_COM_DEF_GET_DELAY = 0x04,		// +
	GB_COM_DEF_GET_OVERLAP = 0x05,		// +
	GB_COM_DEF_GET_RZ_DEC = 0x06,		// +
	GB_COM_DEF_GET_PRM_TYPE = 0x07,		// + ! в Р400М это снижение уровня АК
	GB_COM_DEF_GET_FREQ_PRD = 0x08,		// +
	GB_COM_DEF_GET_RZ_THRESH = 0x09,	// + ! в Р400М это частота ПРМ
	GB_COM_DEF_GET_TYPE_AC = 0x0A,		// +
	GB_COM_PRM_GET_TIME_ON = 0x11,		// +
	GB_COM_PRM_GET_TIME_OFF = 0x13,		// +
	GB_COM_PRM_GET_BLOCK_COM = 0x14,	// +
	GB_COM_PRD_GET_TIME_ON = 0x21,		// +
	GB_COM_PRD_GET_DURATION = 0x22,		// +
	GB_COM_PRD_GET_BLOCK_COM = 0x24,	// +
	GB_COM_PRD_GET_LONG_COM = 0x25,		// +
	GB_COM_PRD_GET_TEST_COM = 0x26,		// +
	GB_COM_GET_SOST = 0x30,				// +
	GB_COM_GET_FAULT = 0x31,			// +
	GB_COM_GET_TIME = 0x32,				// +
	GB_COM_GET_COR_U_I = 0x33,			// + ! в ОПТИКЕ это Резервирование
	GB_COM_GET_MEAS = 0x34,				// +
	GB_COM_GET_TIME_SINCHR = 0x35,		// +
	GB_COM_GET_COM_PRM_KEEP = 0x36, 	// + ! в Р400М это Uвых номинальное
	GB_COM_GET_COM_PRD_KEEP = 0x37,		// + ! в Р400М это тип удаленного аппарата
	GB_COM_GET_NET_ADR = 0x38,			// +
	GB_COM_GET_TIME_RERUN = 0x39,		// + ! в Р400М это параметры для совместимостей
	GB_COM_GET_FREQ = 0x3A,				// +
	GB_COM_GET_DEVICE_NUM = 0x3B,		// +
	GB_COM_GET_CF_THRESHOLD = 0x3C,		// + ! порог предупреждения и загрубления
	GB_COM_GET_OUT_CHECK = 0x3D,		// +
	GB_COM_GET_TEST = 0x3E,				// +
	GB_COM_GET_VERS = 0x3F,				// +
	GB_COM_PRM_ENTER = 0x51,			// +
	GB_COM_SET_REG_DISABLED = 0x70,		// +
	GB_COM_SET_REG_ENABLED = 0x71,		// +
	GB_COM_SET_CONTROL = 0x72,			// +
	GB_COM_SET_PASSWORD = 0x73,			// + ! только с ПК
	GB_COM_GET_PASSWORD = 0x74,			// + ! только с ПК
	GB_COM_SET_REG_TEST_2 = 0x7D,		// +
	GB_COM_SET_REG_TEST_1 = 0x7E,		// +
	GB_COM_DEF_SET_DEF_TYPE = 0x81,		// +
	GB_COM_DEF_SET_LINE_TYPE = 0x82,	// +
	GB_COM_DEF_SET_T_NO_MAN = 0x83,		// +
	GB_COM_DEF_SET_DELAY = 0x84,		// +
	GB_COM_DEF_SET_OVERLAP = 0x85,		// +
	GB_COM_DEF_SET_RZ_DEC = 0x86,		// +
	GB_COM_DEF_SET_PRM_TYPE = 0x87,		// + ! в Р400М это снижение уровня АК
	GB_COM_DEF_SET_FREQ_PRD = 0x88,		// +
	GB_COM_DEF_SET_RZ_THRESH = 0x89,	// + ! в Р400М это частота ПРМ
	GB_COM_DEF_SET_TYPE_AC = 0x8A,		// +
	GB_COM_PRM_SET_TIME_ON = 0x91,		// +
	GB_COM_PRM_SET_TIME_OFF = 0x93,		// +
	GB_COM_PRM_SET_BLOCK_COM = 0x94,	// +
	GB_COM_PRM_RES_IND = 0x9A,			// +
	GB_COM_PRD_SET_TIME_ON = 0xA1,		// +
	GB_COM_PRD_SET_DURATION = 0xA2,		// +
	GB_COM_PRD_SET_BLOCK_COM = 0xA4,	// +
	GB_COM_PRD_SET_LONG_COM = 0xA5,		// +
	GB_COM_PRD_SET_TEST_COM = 0xA6,		// +
	GB_COM_PRD_RES_IND = 0xAA,			// +
	GB_COM_SET_TIME = 0xB2,				// +
	GB_COM_SET_COR_U_I = 0xB3,			// +
	GB_COM_SET_TIME_SINCHR = 0xB5,		// +
	GB_COM_SET_COM_PRM_KEEP = 0xB6, 	// + ! в Р400М это Uвых номинальное
	GB_COM_SET_COM_PRD_KEEP = 0xB7, 	// + ! в Р400М это тип удаленного аппарата
	GB_COM_SET_NET_ADR = 0xB8,			// +
	GB_COM_SET_TIME_RERUN = 0xB9,		// +
	GB_COM_SET_FREQ = 0xBA,				// +
	GB_COM_SET_DEVICE_NUM = 0xBB,		// +
	GB_COM_SET_CF_THRESHOLD = 0xBC,		// +
	GB_COM_SET_OUT_CHECK = 0xBD,		// +
	GB_COM_DEF_GET_JRN_CNT = 0xC1,		//
	GB_COM_DEF_GET_JRN_ENTRY = 0xC2,	//
	GB_COM_DEF_JRN_CLR = 0xCA,			// ! стирание журнала ЗАЩ, только с ПК
	GB_COM_PRM_GET_JRN_CNT = 0xD1,		//
	GB_COM_PRM_GET_JRN_ENTRY = 0xD2,	//
	GB_COM_PRM_JRN_CLR = 0xDA,			// ! стирание журнала ПРМ, только с ПК
	GB_COM_PRD_GET_JRN_CNT = 0xE1,		//
	GB_COM_PRD_GET_JRN_ENTRY = 0xE2,	//
	GB_COM_PRD_JRN_CLR = 0xEA,			// ! стирание журнала ПРД, только с ПК
	GB_COM_GET_JRN_CNT = 0xF1,			//
	GB_COM_GET_JRN_ENTRY = 0xF2,		//
	GB_COM_JRN_CLR = 0xFA				// ! стирание журнала событий, только с ПК
};

/// Маски команд
enum eGB_COM_MASK {
	// Тип устройства
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// группа команды
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

/// Значения команд управления
enum eGB_CONTROL {
	GB_CONTROL_RESET_SELF 	= 1,	//	сброс своего
	GB_CONTROL_RESET_UD 	= 2,	//	сброс удаленного (-ых)
	GB_CONTROL_RESET_AC		= 3,	//	сброс АК
	GB_CONTROL_PUSK_UD_1 	= 4,	//	пуск удаленного (1)
	GB_CONTROL_PUSK_UD_2 	= 5,	//	пуск удаленного 2
	GB_CONTROL_PUSK_UD_ALL 	= 6,	//	пуск удаленных
	GB_CONTROL_CALL 		= 7,	//	вызов
	GB_CONTROL_PUSK_ON 		= 8,	//	пуск наладочный вкл.
	GB_CONTROL_PUSK_OFF 	= 9,	//	пуск наладочный выкл.
	GB_CONTROL_PUSK_AC_UD 	= 10,	//	пуск АК удаленный
	// 						= 11
	// 						= 12
	GB_CONTROL_PUSK_UD_3 	= 13,	// пуск удаленного 3
	GB_CONTROL_REG_AC 		= 14,	//
	// 						= 15
	GB_CONTROL_MAN_1 		= 16,	// пуск МАН удаленного (1)
	GB_CONTROL_MAN_2 		= 17,	// пуск МАН удаленного 2
	GB_CONTROL_MAN_3 		= 18,	// пуск МАН удаленного 3
	GB_CONTROL_MAN_ALL 		= 19,	// пуск МАН удаленных
	GB_CONTROL_MAX					//

};

/// События журнала передатчика/приемника - конец и начало команды
enum eGB_STATE_COM {
	GB_STATE_COM_MIN = 0,
	GB_STATE_COM_END = 0,
	GB_STATE_COM_START,
	GB_STATE_COM_MAX
};

/// сигналы в тест1 и тест2
enum eGB_TEST_SIGNAL {
	GB_SIGNAL_OFF = 0, 				//
	GB_SIGNAL_CF,					// РЗСК / Р400М
	GB_SIGNAL_CF1,
	GB_SIGNAL_CF2,
	GB_SIGNAL_CF3,
	GB_SIGNAL_CF4,
	GB_SIGNAL_CF_NO_RZ,				// РЗСК
	GB_SIGNAL_CF_RZ,				// РЗСК
	GB_SIGNAL_RZ,
	GB_SIGNAL_COM1_NO_RZ,			// РЗСК
	GB_SIGNAL_COM2_NO_RZ,			// РЗСК
	GB_SIGNAL_COM3_NO_RZ,			// РЗСК
	GB_SIGNAL_COM4_NO_RZ,			// РЗСК
	GB_SIGNAL_COM1_RZ,				// РЗСК
	GB_SIGNAL_COM2_RZ,				// РЗСК
	GB_SIGNAL_COM3_RZ,				// РЗСК
	GB_SIGNAL_COM4_RZ,				// РЗСК
	GB_SIGNAL_COM1,					// сигналы команд должны идти
	GB_SIGNAL_COM2,					// подряд для заполнения К400
	GB_SIGNAL_COM3,					// vvvvvvvvvvvvvvvvvvvvvvvvvv
	GB_SIGNAL_COM4,
	GB_SIGNAL_COM5,
	GB_SIGNAL_COM6,
	GB_SIGNAL_COM7,
	GB_SIGNAL_COM8,
	GB_SIGNAL_COM9,
	GB_SIGNAL_COM10,
	GB_SIGNAL_COM11,
	GB_SIGNAL_COM12,
	GB_SIGNAL_COM13,
	GB_SIGNAL_COM14,
	GB_SIGNAL_COM15,
	GB_SIGNAL_COM16,
	GB_SIGNAL_COM17,
	GB_SIGNAL_COM18,
	GB_SIGNAL_COM19,
	GB_SIGNAL_COM20,
	GB_SIGNAL_COM21,
	GB_SIGNAL_COM22,
	GB_SIGNAL_COM23,
	GB_SIGNAL_COM24,
	GB_SIGNAL_COM25,
	GB_SIGNAL_COM26,
	GB_SIGNAL_COM27,
	GB_SIGNAL_COM28,
	GB_SIGNAL_COM29,
	GB_SIGNAL_COM30,				// ^^^^^^^^^^^^^^^^^^^^^^^^^^
	GB_SIGNAL_COM31,				// сигналы команд должны идти
	GB_SIGNAL_COM32,				// подряд для заполнения К400
	GB_SIGNAL_CF_RZ_R400M,
	GB_SIGNAL_MAX
};

// интерфейс связи
enum eGB_INTERFACE {
	GB_INTERFACE_MIN = 0,	//
	GB_INTERFACE_USB = 0,	// подключение через USB на передней панели
	GB_INTERFACE_RS485,		// подключение через 485 интерфейс на задней панели
	GB_INTERFACE_MAX		//
};

/// скорость передачи
enum eUART_BAUD_RATE {
	UART_BAUD_RATE_MIN = 0,		//
	UART_BAUD_RATE_1200 = 0,	//
	UART_BAUD_RATE_2400,		//
	UART_BAUD_RATE_4800,		//
	UART_BAUD_RATE_9600,		//
	UART_BAUD_RATE_19200,		//
	UART_BAUD_RATE_38400,		//
	UART_BAUD_RATE_57600,		//
	UART_BAUD_RATE_MAX			//
};

/// кол-во байт данных
enum eUART_DATA_BITS {
	UART_DATA_BITS_MIN = 0,		//
	UART_DATA_BITS_8 = 0,		//
	UART_DATA_BITS_MAX			//
};

/// четность
enum eUART_PARITY {
	UART_PARITY_MIN = 0,		//
	UART_PARITY_NONE = 0,		// нет
	UART_PARITY_EVEN,			// число установленных битов всегда четно
	UART_PARITY_ODD,			// число установленных битов всегда нечетно
//	UART_PARITY_MARK,			// бит четности всегда 1
//	UART_PARITY_SPACE,			// бит четности всегда 0
	UART_PARITY_MAX
};

/// кол-во стоп-бит
enum eUART_STOP_BITS {
	UART_STOP_BITS_MIN = 0,		//
	UART_STOP_BITS_ONE = 0,		//
//	UART_STOP_BITS_ONEPONTFIVE,	//
	UART_STOP_BITS_TWO,			//
	UART_STOP_BITS_MAX			//
};

///
enum eGB_ACT {
	GB_ACT_NO 		= 0,		// значение по умолчанию
	GB_ACT_OLD		= 0x01,		// новое значение совпадает с предыдущим
	GB_ACT_NEW		= 0x02,		// установлено новое значение
	GB_ACT_ERROR	= 0x04		// ошибочное значение
};

struct sEeprom {
	uint16_t 		password;
	eGB_INTERFACE 	interface;
	eUART_BAUD_RATE baudRate;
	eUART_DATA_BITS dataBits;
	eUART_PARITY 	parity;
	eUART_STOP_BITS stopBits;
};

/// Интерфейс связи
class TInterface {
public:
	/**	Конструктор.
	 * 	По умолчанию устанваливает связь по USB.
	 */
	TInterface(eGB_INTERFACE val = GB_INTERFACE_USB) {
		if (!set(val))
			interface_ = GB_INTERFACE_USB;
	}

	/**	Запись
	 * 	@param val Интерфейс связи.
	 * 	@return False в случае ошибочного значения.
	 */
	bool set(eGB_INTERFACE val) {
		bool stat = false;
		if ((val >= GB_INTERFACE_MIN) && (val < GB_INTERFACE_MAX)) {
			interface_ = val;
			stat = true;
		}
		return stat;
	}

	/**	Чтение
	 * 	@return Интерфейс связи
	 */
	eGB_INTERFACE get() const {
		return interface_;
	}

private:
	// Кол-во битов данных
	eGB_INTERFACE interface_;
};

/// Скорость передачи.
class TBaudRate {
public:
	/**	Конструктор.
	 * 	По умолчанию устанваливает скорость 19200 бит/с.
	 */
	TBaudRate(eUART_BAUD_RATE val = UART_BAUD_RATE_19200) {
		if (!set(val))
			baudRate_ = UART_BAUD_RATE_19200;
	}

	/**	Запись
	 * 	@param val Скорость передачи
	 * 	@return
	 * 	@retval False в случае ошибочного значения
	 */
	bool set(eUART_BAUD_RATE val) {
		bool stat = false;
		if ((val >= UART_BAUD_RATE_MIN) && (val < UART_BAUD_RATE_MAX)) {
			baudRate_ = val;
			stat = true;
		}
		return stat;
	}

	/**	Чтение
	 * 	@return Скорость передачи
	 */
	eUART_BAUD_RATE get() const {
		return baudRate_;
	}

private:
	// Скорость передачи
	eUART_BAUD_RATE baudRate_;
};

/// Количество битов данных
class TDataBits {
public:
	/**	Конструктор.
	 * 	По умолчанию устанваливает 8 бит данных.
	 */
	TDataBits(eUART_DATA_BITS val = UART_DATA_BITS_8) {
		if (!set(val))
			dataBits_ = UART_DATA_BITS_8;
	}

	/**	Запись
	 * 	@param val Кол-во битов данных.
	 * 	@return False в случае ошибочного значения.
	 */
	bool set(eUART_DATA_BITS val) {
		bool stat = false;
		if ((val >= UART_DATA_BITS_MIN) && (val < UART_DATA_BITS_MAX)) {
			dataBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	Чтение
	 * 	@param Нет
	 * 	@return Кол-во битов данных
	 */
	eUART_DATA_BITS get() const {
		return dataBits_;
	}

private:
	// Кол-во битов данных
	eUART_DATA_BITS dataBits_;
};

///	Протокол контроля четности
class TParity {
public:
	/**	Конструктор.
	 * 	По умолчанию проверка четности отключена.
	 */
	TParity(eUART_PARITY val = UART_PARITY_NONE) {
		if (!set(val))
			parity_ = UART_PARITY_NONE;
	}

	/**	Запись
	 * 	@param val Контроль четности
	 * 	@return False в случае ошибочного значения
	 */
	bool set(eUART_PARITY val) {
		bool stat = false;
		if ((val >= UART_PARITY_MIN) && (val < UART_PARITY_MAX)) {
			parity_ = val;
			stat = true;
		}
		return stat;
	}

	/**	Чтение
	 * 	@param Нет
	 * 	@return Контроль четности
	 */
	eUART_PARITY get() const {
		return parity_;
	}

private:
	// Контроль четности
	eUART_PARITY parity_;
};

///	Число стоповых битов
class TStopBits {
public:
	/**	Конструктор.
	 * 	По умолчанию 2 стоп бита.
	 */
	TStopBits(eUART_STOP_BITS val = UART_STOP_BITS_TWO) {
		if (!set(val))
			stopBits_ = UART_STOP_BITS_TWO;
	}

	/**	Запись
	 * 	@param val Число стоповых битов
	 * 	@return False в случае ошибочного значения
	 */
	bool set(eUART_STOP_BITS val) {
		bool stat = false;
		if ((val >= UART_STOP_BITS_MIN) && (val < UART_STOP_BITS_MAX)) {
			stopBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	Чтение
	 * 	@param Нет
	 * 	@return  Число стоповых битов
	 */
	eUART_STOP_BITS get() const {
		return stopBits_;
	}

private:
	// Число стоповых битов
	eUART_STOP_BITS stopBits_;
};

/// Класс для даты и времени
class TDataTime {
public:
	// Считывание и установка секунд
	uint8_t getSecond() const {
		return second_;
	}
	bool setSecond(uint8_t val) {
		bool stat = false;
		if (val < 60) {
			second_ = val;
			stat = true;
		} else {
			second_ = 61;
		}
		return stat;
	}

	// считывание и установка минут
	uint8_t getMinute() const {
		return minute_;
	}
	bool setMinute(uint8_t val) {
		bool stat = false;
		if (val < 60) {
			minute_ = val;
			stat = true;
		} else {
			minute_ = 61;
		}
		return stat;
	}

	// считывание и установка часов
	uint8_t getHour() const {
		return hour_;
	}
	bool setHour(uint8_t val) {
		bool stat = false;
		if (val < 24) {
			hour_ = val;
			stat = true;
		} else {
			hour_ = 25;
		}
		return stat;
	}

	// считывание и установка дня
	uint8_t getDay() const {
		return day_;
	}
	bool setDay(uint8_t val) {
		bool stat = false;

		if ((val > 0) && (val <= getNumDaysInMonth())) {
			day_ = val;
			stat = true;
		} else {
			day_ = 32;
		}

		return stat;
	}

	// возвращает кол-во дней в месяце
	// если месяц или год не заданы, возвращается для текущего
	uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const {
		uint8_t num = 0;

		if (month == 0) {
			month = month_;
		}

		if (year == 0) {
			year = year_;
		}

		if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
			num = 30;
		} else if (month == 2) {
			num = ((year % 4) == 0) ? 29 : 28;
		} else if ((month != 0) && (month < 13)) {
			num = 31;
		}

		return num;
	}

	// считывание и установка месяца
	uint8_t getMonth() const {
		return month_;
	}
	bool setMonth(uint8_t val) {
		bool stat = false;
		if ((val > 0) && (val <= 12)) {
			month_ = val;
			stat = true;
		} else {
			month_ = 13;
		}
		return stat;
	}

	// считывание и установка года
	uint8_t getYear() const {
		return year_;
	}
	bool setYear(uint8_t val) {
		bool stat = false;
		if (val < 100) {
			year_ = val;
			stat = true;
		} else {
			year_ = 0;
		}
		return stat;
	}

	// считывание и установка миллисекунд
	uint16_t getMsSecond() const {
		return msSecond_;
	}
	bool setMsSecond(uint16_t val) {
		bool stat = false;
		if (val < 1000) {
			msSecond_ = val;
			stat = true;
		} else {
			msSecond_ = 1000;
		}
		return stat;
	}

private:
	uint16_t msSecond_;
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;
};

/// Класс для пароля
class TPassword {
public:
	TPassword() {
		password_ = 10000;
		admin_ = PASSWORD_ADMIN;
	}

	// возвращает текущий пароль пользователя
	uint16_t get() const {
		return password_;
	}

	// устанавливает пароль пользователя
	bool set(uint16_t pas) {
		bool stat = false;
		if (pas < 10000) {
			password_ = pas;
			stat = true;
		}
		return stat;
	}

	// начальная инициализация пароля пользователя
	// если введено неверное значение, пароль будет PASSWORD_USER
	void init(uint16_t pas) {
		if (!set(pas))
			set(PASSWORD_USER);
	}

	// проверка значения на совпадение с паролем пользователя и администратора
	bool check(uint16_t pas) const {
		return ((pas == password_) || (pas == admin_));
	}

private:
	// пароль пользователя
	uint16_t password_;

	// пароль администратора
	uint16_t admin_;
};

/// Класс для текущего состояния аппарата
class TDeviceStatus {
public:
	TDeviceStatus() {
		// присваивание иемени по умолчанию
		static const char nameDev[] PROGMEM = "НЕТ";
		name = nameDev;

		enable_ = false;

		fault_ = 0;
		faults_ = 0;
		numFaults_ = 0;

		warning_ = 0;
		warnings_ = 0;
		numWarnings_ = 0;

		regime_ = GB_REGIME_MAX;
		state_ = MAX_NUM_DEVICE_STATE;
		dopByte_ = 0;
	}

	// аварии
	bool setFault(uint16_t faults) {
		fault_ = getFirstSetBit(faults);
		faults_ = faults;
		numFaults_ = getNumSetBits(faults);
		return true;
	}
	uint8_t getFault() const {
		return fault_;
	}
	uint16_t getFaults() const {
		return faults_;
	}
	uint8_t getNumFaults() const {
		return numFaults_;
	}

	// неисправности
	bool setWarning(uint16_t warnings) {
		warning_ = getFirstSetBit(warnings);
		warnings_ = warnings;
		numWarnings_ = getNumSetBits(warnings);
		return true;
	}
	uint8_t getWarning() const {
		return warning_;
	}
	uint16_t getWarnings() const {
		return warnings_;
	}
	uint8_t getNumWarnings() const {
		return numWarnings_;
	}

	// режим работы
	// для GLB возвращается GB_REGIME_DISABLED, в случае если все имеющиеся
	// устройства выведены, иначе GB_REGIME_MAX
	bool setRegime(eGB_REGIME regime) {
		bool stat = false;
		if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
			stat = true;
		} else
			regime = GB_REGIME_MAX;
		regime_ = regime;
		return stat;
	}
	eGB_REGIME getRegime() const {
		return regime_;
	}

	//состояние
	bool setState(uint8_t state) {
		bool stat = false;
		if (state < MAX_NUM_DEVICE_STATE) {
			state_ = state;
			stat = true;
		} else
			state = MAX_NUM_DEVICE_STATE;

		return stat;
	}
	uint8_t getState() const {
		return state_;
	}

	uint8_t getDopByte() const {
		return dopByte_;
	}
	bool setDopByte(uint8_t byte) {
		dopByte_ = byte;
		return true;
	}

	// работа с флагом наличия устройства
	// возвращает true если новое значение отличается от текущего
	bool setEnable(bool val) {
		uint8_t act = GB_ACT_NO;

		if (enable_ == val) {
			act |= GB_ACT_OLD;
		} else {
			enable_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}
	bool isEnable() const {
		return enable_;
	}

	// массивы расшифровок аварий и предупреждений
	PGM_P faultText[MAX_NUM_FAULTS];
	PGM_P warningText[MAX_NUM_WARNINGS];
	PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
	PGM_P name;

private:
	// текущая приоритетная неисправность, неисправности и кол-во неисправностей
	uint8_t fault_;
	uint16_t faults_;
	uint8_t numFaults_;

	// текущее приоритетное предупреждение, предупреждения и кол-во предупрежд.
	uint8_t warning_;
	uint16_t warnings_;
	uint8_t numWarnings_;

	eGB_REGIME regime_;
	uint8_t state_;
	uint8_t dopByte_;

	// True - означает наличие данного устройства в текущем исполнении АВАНТа
	bool enable_;

	// возвращает кол-во установленных в 1 бит
	uint8_t getNumSetBits(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
			cnt++;
		}
		return cnt;
	}

	// возвращает номер младшего установленного бита
	uint8_t getFirstSetBit(uint16_t val)
	{
		uint8_t cnt = 0;
		for(; val > 0; val >>= 1)
		{
			if (val & 0x0001)
			break;
			cnt++;
		}
		return cnt;
	}
};

/// класс для общих параметров и настроек
class TDeviceGlb {
public:
	TDeviceGlb() {
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeLine_ = GB_TYPE_LINE_MAX;
		compatibility_ = GB_COMPATIBILITY_MAX;
		versBsp_ = 0;
		versDsp_ = 0;

		timeSinchr_ = false;
		backup_ = false;
		deviceNum_ = GLB_DEV_NUM_MIN_F;
		outCheck_ = false;
		cfThreshold_ = GLB_CF_THRESH_MIN_F;
		timeRerun_ = GLB_T_RERUN_MIN_F;
		comPrdKeep_ = false;
		comPrmKeep_ = false;
		inDecrease_ = GLB_IN_DEC_MIN_F;
		freq_ = GLB_FREQ_MIN_F;
		uOutNom_ = GLB_U_OUT_NOM_MIN_F;
		netAdr_ = GLB_NET_ADR_MIN_F;
		acInDec_ = GLB_AC_IN_DEC_MIN_F;
		pvzueProtocol_ = GB_PVZUE_PROTOCOL_MAX;
		pvzueParity_ = GB_PVZUE_PARITY_MAX;
		pvzueFail_ = GLB_PVZUE_FAIL_MIN_F;
		pvzueNoiseTH_ = GLB_PVZUE_N_TH_MIN_F;
		pvzueNoiseLvl_ = GLB_PVZUE_N_LVL_MIN_F;
		pvzueTypeAC_ = GB_PVZUE_TYPE_AC_MAX;
		detector_ = GLB_DETECTOR_MIN_F;
		corI_ = GLB_COR_I_DEC_MIN_F;
		corU_ = GLB_COR_U_DEC_MIN_F;

		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}

	}

	TDeviceStatus status;

	/**	Возвращает максимальное кол-во аппаратов в линии.
	 * 	@retval 3 В случае 3-х концевой версии
	 * 	@retval 2 Во всех остальных случаях.
	 */
	uint8_t getMaxNumDevices() const {
		return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2;
	}

	/**	Возвращает кол-во аппаратов в линии (2-х, 3-х концевая).
		 * 	@return Кол-во аппаратов в линии.
		 */
		eGB_NUM_DEVICES getNumDevices() const {
			return numDevices_;
		}

	/** Установка кол-ва аппаратов в линии (2-х, 3-х концевая.).
	 * 	@param val Кол-во аппаратов в линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setNumDevices(eGB_NUM_DEVICES val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_NUM_DEVICES_MIN) || (val >= GB_NUM_DEVICES_MAX)) {
			val = GB_NUM_DEVICES_MAX;
			act = GB_ACT_ERROR;
		}

		if (numDevices_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numDevices_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

	/**	Возвращает текущий тип линии (вч/оптика и т.д.).
	 * 	@return Тип линии.
	 */
	eGB_TYPE_LINE getTypeLine() const {
		return typeLine_;
	}

	/** Установка типа линии (вч/оптика и т.д.).
	 * 	@param val Тип линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setTypeLine(eGB_TYPE_LINE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_TYPE_LINE_MIN) || (val >= GB_TYPE_LINE_MAX)) {
			val = GB_TYPE_LINE_MAX;
			act = GB_ACT_ERROR;
		}

		if (typeLine_ == val) {
			act |= GB_ACT_OLD;
		} else {
			typeLine_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

//	// версия прошивки AtMega BSP
//	void setVersBsp(uint16_t versBsp) {
//		versBsp_ = versBsp;
//	}
//	uint16_t getVersBsp() const {
//		return versBsp_;
//	}
//
//	//  версия прошивки DSP
//	void setVersDsp(uint16_t versDsp) {
//		versDsp_ = versDsp;
//	}
//	uint16_t getVersDsp() const {
//		return versDsp_;
//	}
//
//	// версия прошивки БСК ПРД1
//	void setVersBskPrd1(uint8_t vers) {
//		versBskPrd1_ = vers;
//	}
//	uint16_t getVersBskPrd1() const {
//		return versBskPrd1_;
//	}
//
//	// версия прошивки БСК ПРД2
//	void setVersBskPrd2(uint8_t vers) {
//		versBskPrd2_ = vers;
//	}
//	uint16_t getVersBskPrd2() const {
//		return versBskPrd2_;
//	}

	/**	Запись версии прошивки для микросхем АВАНТа.
	 * 	Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC16(uint16_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			versProgIC_[ic] = vers;
		}
	}

	/**	Запись версии прошивки для микросхема АВАНТа.
	  * Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC8(uint8_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			uint8_t hi = vers >> 4;
			uint8_t low = vers & 0x0F;
			versProgIC_[ic] = TO_INT16(hi, low);
		}
	}

	/**	Возвращает версию прошивки для указанной микросхемы АВАНТа.
	 * 	Старший байт int16_t - версия прошивки, младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@return Версия прошивки микросхемы.
	 * 	@retval 0 В случае ошибочного номера микросхемы.
	 */
	uint16_t getVersProgIC(eGB_IC ic) {
		return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0);
	}


	/**	Установка совместимости (тип удаленного аппарата).
	 * 	@param val Совместимость.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setCompatibility(eGB_COMPATIBILITY val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMPATIBILITY_MIN) || (val >= GB_COMPATIBILITY_MAX)) {
			val = GB_COMPATIBILITY_MAX;
			act = GB_ACT_ERROR;
		}

		if (compatibility_ == val) {
			act |= GB_ACT_OLD;
		} else {
			compatibility_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

	/**	Возвращает текущую совместимость.
	 * 	@return Совместимость.
	 */
	eGB_COMPATIBILITY getCompatibility() const {
		return compatibility_;
	}

	// синхронизация часов
	// True - включена, False - выключена
	bool setTimeSinchr(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			timeSinchr_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTimeSinchr() const {
		return timeSinchr_;
	}

	/** Установка параметра Резервирование.
	 * 	@param val Резервирование
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setBackup(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			backup_ = (bool) val;
			stat = true;
		}
		return stat;
	}

	/** Возвращает значение параметра Резервирование.
	 * 	@return Резервирование.
	 */
	bool getBackup() const {
		return backup_;
	}

	// номер аппарата
	bool setDeviceNum(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_DEV_NUM_MIN_F) {
			if (val <= getMaxNumDevices()) {
				deviceNum_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getDeviceNum() const {
		return (deviceNum_ * GLB_DEV_NUM_FRACT);
	}

	// контроль выходного сигнала
	bool setOutCheck(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			outCheck_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getOutCheck() const {
		return outCheck_;
	}

	// порог предупреждения по КЧ
	bool setCfThreshold(uint8_t val) {
		bool stat = false;
		val = (val / GLB_CF_THRESH_DISC_F) * GLB_CF_THRESH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_CF_THRESH_MIN_F) {
			if (val <= GLB_CF_THRESH_MAX_F) {
				cfThreshold_ = val;
				stat = true;
			}
		}

		return stat;
	}
	uint8_t getCfThreshold() const {
		return (cfThreshold_ * GLB_CF_THRESH_FRACT);
	}

	// время перезапуска
	bool setTimeRerun(uint8_t val) {
		bool stat = false;
		val = (val / GLB_T_RERUN_DISC_F) * GLB_T_RERUN_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_T_RERUN_MIN_F) {
			if (val <= GLB_T_RERUN_MAX_F) {
				timeRerun_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getTimeRerun() const {
		return (timeRerun_ * GLB_T_RERUN_FRACT);
	}

	// Удержание реле команд ПРД
	bool setComPrdKeep(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			comPrdKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrdKeep() const {
		return comPrdKeep_;
	}

	// Удержание реле команд ПРМ
	bool setComPrmKeep(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			comPrmKeep_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getComPrmKeep() const {
		return comPrmKeep_;
	}

	// уменьшение усиления входного сигнала
	bool setInDecrease(uint8_t val) {
		bool stat = false;
		val = (val / GLB_IN_DEC_DISC_F) * GLB_IN_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_IN_DEC_MIN_F) {
			if (val <= GLB_IN_DEC_MAX_F) {
				inDecrease_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getInDecrease() const {
		return (inDecrease_ * GLB_IN_DEC_FRACT);
	}

	// сетевой адрес
	bool setNetAddress(uint8_t val) {
		bool stat = false;
		val = (val / GLB_NET_ADR_DISC_F) * GLB_NET_ADR_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_NET_ADR_MIN_F) {
			if (val <= GLB_NET_ADR_MAX_F) {
				netAdr_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getNetAddress() const {
		return (netAdr_ * GLB_NET_ADR_FRACT);
	}

	// Uвых номинальное
	bool setUoutNom(uint8_t val) {
		bool stat = false;
		val = (val / GLB_U_OUT_NOM_DISC_F) * GLB_U_OUT_NOM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_U_OUT_NOM_MIN_F) {
			if (val <= GLB_U_OUT_NOM_MAX_F) {
				uOutNom_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getUoutNom() const {
		return (uOutNom_ * GLB_U_OUT_NOM_FRACT);
	}

	// частота
	bool setFreq(uint16_t val) {
		bool stat = false;
		val = (val / GLB_FREQ_DISC_F) * GLB_FREQ_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_FREQ_MIN_F) {
			if (val <= GLB_FREQ_MAX_F) {
				freq_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint16_t getFreq() const {
		return (freq_ * GLB_FREQ_FRACT);
	}

	// снижение ответа АК (ПВЗЛ)
	bool setAcInDec(uint8_t val) {
		bool stat = false;
		val = (val / GLB_AC_IN_DEC_DISC_F) * GLB_AC_IN_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_AC_IN_DEC_MIN_F) {
			if (val <= GLB_AC_IN_DEC_MAX_F) {
				acInDec_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getAcInDec() const {
		return (acInDec_ * GLB_AC_IN_DEC_FRACT);
	}

	// Протокол обмена (ПВЗУ-Е)
	bool setPvzueProtocol(eGB_PVZUE_PROTOCOL val) {
		bool stat = false;

		if ((val >= GB_PVZUE_PROTOCOL_MIN) && (val <= GB_PVZUE_PROTOCOL_MAX)) {
			pvzueProtocol_ = val;
			stat = true;
		} else
			pvzueProtocol_ = GB_PVZUE_PROTOCOL_MAX;

		return stat;
	}
	eGB_PVZUE_PROTOCOL getPvzueProtocol() const {
		return pvzueProtocol_;
	}

	// Признак четности (ПВЗУ-Е)
	bool setPvzueParity(eGB_PVZUE_PARITY val) {
		bool stat = false;

		if ((val >= GB_PVZUE_PARITY_MIN) && (val <= GB_PVZUE_PARITY_MAX)) {
			pvzueParity_ = val;
			stat = true;
		} else
			pvzueParity_ = GB_PVZUE_PARITY_MAX;

		return stat;
	}
	eGB_PVZUE_PARITY getPvzueParity() const {
		return pvzueParity_;
	}

	// Допустимые провалы (ПВЗУ-Е)
	bool setPvzueFail(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_FAIL_DISC_F) * GLB_PVZUE_FAIL_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_FAIL_MIN_F) {
			if (val <= GLB_PVZUE_FAIL_MAX_F) {
				pvzueFail_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getPvzueFail() const {
		return pvzueFail_;
	}

	// Порог по помехе (ПВЗУ-Е)
	bool setPvzueNoiseTH(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_TH_DISC_F) * GLB_PVZUE_N_TH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_N_TH_MIN_F) {
			if (val <= GLB_PVZUE_N_TH_MAX_F) {
				pvzueNoiseTH_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getPvzueNoiseTH() const {
		return pvzueNoiseTH_;
	}

	// Допустимая помеха (ПВЗУ-Е)
	bool setPvzueNoiseLvl(uint8_t val) {
		bool stat = false;

		val = (val / GLB_PVZUE_N_LVL_DISC_F) * GLB_PVZUE_N_LVL_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_PVZUE_N_LVL_MIN_F) {
			if (val <= GLB_PVZUE_N_LVL_MAX_F) {
				pvzueNoiseLvl_ = val;
				stat = true;
			}
		}

		return stat;
	}
	uint8_t getPvzueNoiseLvl() const {
		return pvzueNoiseLvl_;
	}

	// Тип автоконтроля
	bool setPvzueTypeAC(eGB_PVZUE_TYPE_AC val) {
		bool stat = false;

		if (val >= GB_PVZUE_TYPE_AC_MIN) {
			if (val <= GB_PVZUE_TYPE_AC_MAX) {
				pvzueTypeAC_ = val;
				stat = true;
			}
		}

		// установка ошибочного значения
		if (!stat)
			pvzueTypeAC_ = GB_PVZUE_TYPE_AC_MAX;

		return stat;
	}
	eGB_PVZUE_TYPE_AC getPvzueTypeAC() const {
		return pvzueTypeAC_;
	}

	/** Установка параметра Тип детектора.
	 * 	@param val Тип детектора.
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setDetector(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DETECTOR_DISC_F) * GLB_DETECTOR_DISC_F;

		if (val >= GLB_DETECTOR_MIN_F) {
			if (val <= GLB_DETECTOR_MAX_F) {
				detector_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/** Возвращает значение Типа детектора.
	 * 	@return Тип детектора.
	 */
	uint8_t getDetector() const {
		return detector_;
	}

	// коррекция тока
	bool setCorI(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_I_DEC_DISC_F) * GLB_COR_I_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_COR_I_DEC_MIN_F) {
			if (val <= GLB_COR_I_DEC_MAX_F) {
				corI_ = val;
				stat = true;
			}
		}
		return stat;
	}
	int16_t getCorI() const {
		return (corI_ * GLB_COR_I_DEC_FRACT);
	}

	// коррекция напряжения
	bool setCorU(int16_t val) {
		bool stat = false;
		val = (val / GLB_COR_U_DEC_DISC_F) * GLB_COR_U_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_COR_U_DEC_MIN_F) {
			if (val <= GLB_COR_U_DEC_MAX_F) {
				corU_ = val;
				stat = true;
			}
		}
		return stat;
	}
	int16_t getCorU() const {
		return (corU_ * GLB_COR_U_DEC_FRACT);
	}

private:
	// версии прошивок микросхем
	uint16_t versProgIC_[GB_IC_MAX];

	// кол-во аппаратов в линии 2 или 3
	eGB_NUM_DEVICES numDevices_;

	// тип линии
	eGB_TYPE_LINE typeLine_;

	// версия прошивки БСП
	uint16_t versBsp_;

	// версия прошивки DSP
	uint16_t versDsp_;

	// совместимость
	eGB_COMPATIBILITY compatibility_;

	// синхронизация часов
	bool timeSinchr_;

	// резервирование
	bool backup_;

	// номер аппарата
	uint8_t deviceNum_;

	// контроль выходного сигнала
	bool outCheck_;

	// порог предупреждения по КЧ
	uint8_t cfThreshold_;

	// время перезапуска
	uint8_t timeRerun_;

	// удержание реле команд Передатчика
	bool comPrdKeep_;

	// удержание реле команд Передатчика
	bool comPrmKeep_;

	// уменьшение усиления входного сигнала
	uint8_t inDecrease_;

	// сетевой адрес
	uint8_t netAdr_;

	// Uвых номинальное
	uint8_t uOutNom_;

	// Частота
	uint16_t freq_;

	// Снижение ответа АК (ПВЗЛ)
	uint8_t acInDec_;

	// Протокол обмена (ПВЗУ-Е)
	eGB_PVZUE_PROTOCOL pvzueProtocol_;

	// Признак четности (ПВЗУ-Е)
	eGB_PVZUE_PARITY pvzueParity_;

	// Допустимые провалы (ПВЗУ-Е)
	uint8_t pvzueFail_;

	// Порог по помехе (ПВЗУ-Е)
	uint8_t pvzueNoiseTH_;

	// Допустимая помеха (ПВЗУ-Е)
	uint8_t pvzueNoiseLvl_;

	// Тип автоконтроля (ПВЗУ-Е)
	eGB_PVZUE_TYPE_AC pvzueTypeAC_;

	// Тип детектора
	uint8_t detector_;

	// Коррекция тока
	int16_t corI_;

	// Коррекция напряжения
	int16_t corU_;
};

/// класс для параметров и настроек защиты
class TDeviceDef {
public:
	TDeviceDef() {
		defType_ = DEF_TYPE_MIN;
		numDevices_ = GB_NUM_DEVICES_MAX;
		timeNoMan_ = DEF_T_NO_MAN_MIN_F;
		overlap_ = DEF_OVERLAP_MIN_F;
		delay_[0] = DEF_DELAY_MIN_F;
		delay_[1] = DEF_DELAY_MIN_F;
		rzThreshold_ = DEF_RZ_THRESH_MIN_F;
		rzDec_ = DEF_RZ_DEC_MIN_F;
		prmType_ = DEF_PRM_TYPE_MIN;
		typeAc_ = GB_TYPE_AC_AUTO_NORM;
		timeToAc_ = 0;
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		acDec_ = false;
		overflow_ = false;
		freqPrm_ = GB_PVZL_FREQ_MAX;
		freqPrd_ = GB_PVZL_FREQ_MAX;
	}

	TDeviceStatus status;

	// тип защиты
	bool setDefType(uint8_t val) {
		bool stat = false;
		if ((val >= DEF_TYPE_MIN) && (val < DEF_TYPE_MAX)) {
			defType_ = val;
			stat = true;
		} else {
			defType_ = DEF_TYPE_MAX;
		}

		return stat;
	}
	uint8_t getDefType() const {
		return defType_;
	}

	// тип линии (кол-во аппаратов в линии)
	bool setNumDevices(eGB_NUM_DEVICES val) {
		bool stat = false;
		if (val >= GB_NUM_DEVICES_MIN) {
			if (val < GB_NUM_DEVICES_MAX) {
				numDevices_ = val;
				stat = true;
			}
		}

		if (!stat)
			val = GB_NUM_DEVICES_MAX;
		numDevices_ = val;
		return stat;
	}
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	// допустимое время без манипуляции
	bool setTimeNoMan(uint8_t val) {
		bool stat = false;
		val = (val / DEF_T_NO_MAN_DISC_F) * DEF_T_NO_MAN_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_T_NO_MAN_MIN_F) {
			if (val <= DEF_T_NO_MAN_MAX_F) {
				timeNoMan_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getTimeNoMan() const {
		return (timeNoMan_ * DEF_T_NO_MAN_FRACT);
	}

	// перекрытие импульсов
	bool setOverlap(uint8_t val) {
		bool stat = false;
		val = (val / DEF_OVERLAP_DISC_F) * DEF_OVERLAP_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_OVERLAP_MIN_F) {
			if (val <= DEF_OVERLAP_MAX_F) {
				overlap_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getOverlap() const {
		return (overlap_ * DEF_OVERLAP_FRACT);
	}

	/** Установка значения "Компенсация задержки на линии".
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Записывается значение первого параметра.
	 * 	@param val Значение параметра.
	 * 	@retval True Параметр успешно записан.
	 * 	@retval False Ошибка записи.
	 */
	bool setDelay(uint8_t num, uint8_t val) {
		bool stat = false;
		val = (val / DEF_DELAY_DISC_F) * DEF_DELAY_DISC_F;

		if ((num < 1) || (num > 2))
			num = 1;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_DELAY_MIN_F) {
			if (val <= DEF_DELAY_MAX_F) {
				delay_[num] = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	Считывание значения "Компенсация задержки на линии".
	 * 	В 3-х концевой два параметра, в 2-х концевой один.
	 * 	@param num 	Номер параметра.
	 * 	@arg 1	Первый.
	 * 	@arg 2	Второй (только в 3-х концевой).
	 * 	@arg Other Возвращается значение первого параметра.
	 * 	@return Значени параметра.
	 */
	uint8_t getDelay(uint8_t num) const {
		if ((num < 1) || (num > 2))
			num = 1;
		return (delay_[num] * DEF_DELAY_FRACT);
	}

	// порог предуреждения по РЗ
	bool setRzThreshold(uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_THRESH_DISC_F) * DEF_RZ_THRESH_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_RZ_THRESH_MIN_F) {
			if (val <= DEF_RZ_THRESH_MAX_F) {
				rzThreshold_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getRzThreshold() const {
		return (rzThreshold_ * DEF_RZ_THRESH_FRACT);
	}

	// загрубление чувствительности по РЗ
	bool setRzDec(uint8_t val) {
		bool stat = false;
		val = (val / DEF_RZ_DEC_DISC_F) * DEF_RZ_DEC_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= DEF_RZ_DEC_MIN_F) {
			if (val <= DEF_RZ_DEC_MAX_F) {
				rzDec_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getRzDec() const {
		return (rzDec_ * DEF_RZ_DEC_FRACT);
	}

	// тип приемника
	bool setPrmType(uint8_t val) {
		bool stat = false;
		if ((val >= DEF_PRM_TYPE_MIN) && (val < DEF_PRM_TYPE_MAX)) {
			prmType_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getPrmType() const {
		return prmType_;
	}

	// тип автоконтроля
	bool setTypeAC(eGB_TYPE_AC val) {
		bool stat = false;
		if ((val >= GB_TYPE_AC_MIN) && (val <= GB_TYPE_AC_MAX)) {
			typeAc_ = val;
			stat = true;
		}

		return stat;
	}
	eGB_TYPE_AC getTypeAC() const {
		return typeAc_;
	}

	// время до автоконтроля
	bool setTimeToAC(uint32_t val) {
		bool stat = false;

		val /= 1000;

		if (val <= 60000) {
			timeToAc_ = val;
			stat = true;
		}

		return stat;
	}
	uint64_t getTimeToAC() const {
		return timeToAc_;
	}

	// снижение уровня автоконтроля
	// True - включена, False - выключена
	bool setAcDec(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			acDec_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getAcDec() const {
		return acDec_;
	}

	// частота ПРМ (ПВЗЛ)
	// True - включена, False - выключена
	bool setFreqPrm(eGB_PVZL_FREQ val) {
		bool stat = false;
		if ((val >= GB_PVZL_FREQ_MIN) && (val <= GB_PVZL_FREQ_MAX)) {
			freqPrm_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getFreqPrm() const {
		return freqPrm_;
	}

	// частота ПРД (ПВЗЛ)
	// True - включена, False - выключена
	bool setFreqPrd(eGB_PVZL_FREQ val) {
		bool stat = false;
		if ((val >= GB_PVZL_FREQ_MIN) && (val <= GB_PVZL_FREQ_MAX)) {
			freqPrd_ = val;
			stat = true;
		}
		return stat;
	}
	uint8_t getFreqPrd() const {
		return freqPrd_;
	}

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// тип защиты
	uint8_t defType_;

	// тип линии (кол-во аппаратов)
	eGB_NUM_DEVICES numDevices_;

	// допустимое время без манипуляции
	uint8_t timeNoMan_;

	// перекрытие импульсов
	uint8_t overlap_;

	// компенсация задержки в линии
	uint8_t delay_[];

	// порог предупреждения по РЗ
	uint8_t rzThreshold_;

	// загрубление чувствительности по РЗ
	uint8_t rzDec_;

	// тип приемника
	uint8_t prmType_;

	// тип автоконтроля
	eGB_TYPE_AC typeAc_;

	// время до автоконтроля в секундах
	uint16_t timeToAc_;

	// снижение уровня АК
	bool acDec_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;

	// частота ПРМ (ПВЗЛ)
	eGB_PVZL_FREQ freqPrm_;

	// частота ПРД (ПВЗЛ)
	eGB_PVZL_FREQ freqPrd_;
};

/// класс для параметров и настроек приемника
class TDevicePrm {
public:
	TDevicePrm() {
		numCom_ = 0;
		timeOn_ = PRM_TIME_ON_MIN_F;

		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			blockCom_[i] = false;
			timeOff_[i] = PRM_TIME_OFF_MIN_F;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	/**	Установка текущего кол-ва команд в ПРМ.
	 * 	Если новое значение выходит за диапазон допустимых значений, устанавли
	 * 	вается 0. Далее если команд нет, ПРМ отключается. Иначе включается.
	 * 	@param val Количество команд на ПРМ.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	bool setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRM) {
			val = 0;
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// если кол-во команд не равно 0, то включается ПРМ
			this->status.setEnable(val != 0);
			act |= GB_ACT_NEW;
		}

		return act;
	}
	uint8_t getNumCom() const {
		return numCom_;
	}

	// время включения команды
	bool setTimeOn(uint8_t val) {
		bool stat = false;
		val = (val / PRM_TIME_ON_DISC_F) * PRM_TIME_ON_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= PRM_TIME_ON_MIN_F) {
			if (val <= PRM_TIME_ON_MAX_F) {
				timeOn_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getTimeOn() const {
		return (timeOn_ * PRM_TIME_ON_FRACT);
	}

	// блокированные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8)) {
			blockCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (blockCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getBlockCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRM / 8))
			val = blockCom_[num];
		return val;
	}

	// задержка на выключение
	// buf - адрес первого элемента массива
	// значение передается в 10 раз меньше реального !!
	bool setTimeOff(uint8_t *buf) {
		bool stat = true;
		uint16_t val;
		for (uint_fast8_t i = 0; i < numCom_; i++) {
			val = ((*buf) / PRM_TIME_OFF_DISC_F) * PRM_TIME_OFF_DISC_F;

			// записано в таком виде т.к. иначе портится автоформат в Eclipse
			if (val >= PRM_TIME_OFF_MIN_F) {
				if (val <= PRM_TIME_OFF_MAX_F)
					timeOff_[i] = val;
				else
					stat = false;
			} else
				stat = false;
			buf++;
		}
		return stat;
	}
	uint16_t getTimeOff(uint8_t num) const {
		return timeOff_[num] * PRM_TIME_OFF_FRACT;
	}

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// кол-во команд приемника
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	uint8_t blockCom_[MAX_NUM_COM_PRM / 8];

	// задержка на выключение
	uint8_t timeOff_[MAX_NUM_COM_PRM];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};

/// класс для параметров и настроек передатчика
class TDevicePrd {
public:
	TDevicePrd() {
		numCom_ = 0;
		timeOn_ = PRD_TIME_ON_MIN_F;
		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
			blockCom_[i] = false;
			longCom_[i] = false;
		}
		durationL_ = PRD_DURAT_L_MIN_F;
		durationO_ = PRD_DURAT_O_MIN_F;
		testCom_ = false;

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	/**	Установка текущего кол-ва команд в ПРД.
	 * 	Если новое значение выходит за диапазон допустимых значений, устанавли
	 * 	вается 0. Далее если команд нет, ПРД отключается. Иначе включается.
	 * 	@param val Количество команд на ПРД.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRD) {
			val = 0;
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// если кол-во команд не равно 0, то включается ПРМ
			this->status.setEnable(val != 0);
			act |= GB_ACT_NEW;
		}

		return act;
	}
	uint8_t getNumCom() const {
		return numCom_;
	}

	// время включения команды
	bool setTimeOn(uint8_t val) {
		bool stat = false;
		val = (val / PRD_TIME_ON_DISC_F) * PRD_TIME_ON_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= PRD_TIME_ON_MIN_F) {
			if (val <= PRD_TIME_ON_MAX_F) {
				timeOn_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getTimeOn() const {
		return timeOn_ * PRD_TIME_ON_FRACT;
	}

	// блокированные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setBlockCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8)) {
			blockCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getBlockCom(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (blockCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getBlockCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = blockCom_[num];
		return val;
	}

	// длительные команды, каждый бит отвечает за отдельную команду
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	// val - значение
	bool setLongCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			longCom_[num] = val;
			stat = true;
		}
		return stat;
	}
	// возвращает True, если команда заблокирована
	bool getLongCom(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (longCom_[pl] & (1 << num)) != 0;
	}
	// возвращает воьсмерку команд
	// num - номер восьмерки (0 - с 1 по 8 команды, 1 - с 9 по 16 и т.д.)
	uint8_t getLongCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = longCom_[num];
		return val;
	}

	// тестовая команда
	// True - включена, False - выключена
	bool setTestCom(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			testCom_ = (bool) val;
			stat = true;
		}
		return stat;
	}
	bool getTestCom() const {
		return testCom_;
	}

	/**	Установка параметра "Длительность команды" для ВЧ-линий.
	 * 	@param val Длительность команды.
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setDurationL(uint8_t val) {
		bool stat = false;

		val = (val / PRD_DURAT_L_DISC_F) * PRD_DURAT_L_DISC_F;
		if (val >= PRD_DURAT_L_MIN_F) {
			if (val <= PRD_DURAT_L_MAX_F) {
				durationL_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	Возвращает значение параметра "Длительность команды" для ВЧ-линий.
	 * 	@return Длительность команды.
	 */
	uint16_t getDurationL() const {
		return durationL_ * PRD_DURAT_L_FRACT;
	}

	/**	Установка параметра "Длительность команды" для ОПТИКИ.
	 * 	@param val Длительность команды.
	 * 	@retval True - в случае успешной остановки.
	 * 	@retval False - если установка нового значения не удалась.
	 */
	bool setDurationO(uint8_t val) {
		bool stat = false;

		val = (val / PRD_DURAT_O_DISC_F) * PRD_DURAT_O_DISC_F;
		if (val >= PRD_DURAT_O_MIN_F) {
			if (val <= PRD_DURAT_O_MAX_F) {
				durationL_ = val;
				stat = true;
			}
		}
		return stat;
	}

	/**	Возвращает значение параметра "Длительность команды" для ВЧ-линий.
	 * 	@return Длительность команды.
	 */
	uint16_t getDurationO() const {
		return durationL_ * PRD_DURAT_O_FRACT;
	}

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			maxNumJrnEntry_ = max;
			stat = true;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// текущее кол-во команд
	uint8_t numCom_;

	// время включения команды
	uint8_t timeOn_;

	// блокированные команды, true - блокированная
	uint8_t blockCom_[MAX_NUM_COM_PRD / 8];

	// длительные команды, true - длительная
	uint8_t longCom_[MAX_NUM_COM_PRD / 8];

	// тестовая команда. true - вкл.
	bool testCom_;

	// длительность команды для ВЧ линий
	uint8_t durationL_;

	// длительность команды для ОПТИКИ
	uint8_t durationO_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// макстмалбное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;

};

/// класс для измеряемых параметров
class TMeasuredParameters {
public:
	TMeasuredParameters() {
		voltDef_ = 0;
		voltDef2_ = 0;
		voltCf_ = 0;
		voltCf2_ = 0;
		voltNoise_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
		pulseWidth_ = 0;
	}

	// запас по защите
	int8_t getVoltageDef() const {
		return voltDef_;
	}
	bool setVoltageDef(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// запас по защите второй
	int8_t getVoltageDef2() const {
		return voltDef2_;
	}
	bool setVoltageDef2(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef2_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// запас по КЧ
	int8_t getVoltageCf() const {
		return voltCf_;
	}
	bool setVoltageCf(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// запас по КЧ кторой
	int8_t getVoltageCf2() const {
		return voltCf2_;
	}
	bool setVoltageCf2(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf2_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// напряжение выхода
	uint8_t getVoltageOutInt() const {
		return (voltOut_ / 10);
	}
	uint8_t getVoltageOutFract() const {
		return (voltOut_ % 10);
	}
	uint16_t getVoltageOut() const {
		return voltOut_;
	}
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract) {
		bool stat = false;
		if ((voltOutInt < 100) && (voltOutFract < 10)) {
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			stat = true;
		}
		return stat;
	}

	// ток выхода
	uint16_t getCurrentOut() const {
		return curOut_;
	}
	bool setCurrentOut(uint16_t curOut) {
		bool stat = false;
		if (curOut <= 999) {
			curOut_ = curOut;
			stat = true;
		}
		return stat;
	}

	// сопротивление выхода
	uint16_t getResistOut() const {
		return resistOut_;
	}
	bool setResistOut(uint16_t resistOut) {
		bool stat = false;
		if (resistOut <= 999) {
			resistOut_ = resistOut;
			stat = true;
		}
		return stat;
	}

	// уровень шума
	int8_t getVoltageNoise() const {
		return voltNoise_;
	}
	bool setVoltageNoise(int8_t voltNoise) {
		bool stat = false;
		if ((voltNoise > -100) && (voltNoise < 100)) {
			voltNoise_ = voltNoise;
			stat = true;
		}
		return stat;
	}

	// длительность импульсов ВЧ блокировки на выходе применика
	uint16_t getPulseWidth() const {
		return pulseWidth_;
	}
	bool setPulseWidth(uint16_t val) {
		bool stat = false;
		if (val <= 360) {
			pulseWidth_ = val;
			stat = true;
		}
		return stat;
	}

private:
	// запас по защите (-99 .. 99)дБ
	int8_t voltDef_;
	// запас по защите второго канала (-99..99)дБ
	int8_t voltDef2_;
	// запас по КЧ (-99 .. 99)дБ
	int8_t voltCf_;
	// запас по КЧ второго канала (-99..99)дБ
	int8_t voltCf2_;
	// уровень шумов (-99 .. 99)дБ
	int8_t voltNoise_;
	// выходное напряжение умноженное на 10 (0 .. 999)В
	uint16_t voltOut_;
	// выходной ток (0 .. 999)мА
	uint16_t curOut_;
	// выходное сопротивление (0 .. 999)Ом
	uint16_t resistOut_;
	// длительность импульсов ВЧ блокировки на выходе применика (0..360)°
	uint16_t pulseWidth_;
};

// класс для передачи команд
class TTxCom {
public:
	TTxCom() {
		clear();
	}

	// очистка буфера
	void clear() {
		numCom1_ = numCom2_ = 0;
		cnt1_ = cnt2_ = 0;
		for(uint_fast8_t i = 0; i < MAX_NUM_FAST_COM; i++)
			comFast_[i] = GB_COM_NO;
		com1_[0] = com2_[0] = GB_COM_NO;
	}

	/** Запись команды в буфер 1.
	 * 	Если num > 0 то происходит замена имеющейся команды в буфере,
	 * 	если num < 0 команда добавляется.
	 * 	@param com Код команды.
	 * 	@param num Индекс элемента в буфере.
	 * 	@retval True - в случае успешной записи.
	 * 	@retval False - если не удалось поместить команду в буфер.
	 */
	bool addCom1(eGB_COM com, int8_t num = -1) {
		bool stat = false;
		if (numCom1_ < MAX_NUM_COM_BUF1) {
			if (num >= 0) {
				if (num > numCom1_)
					numCom1_ = num;
				com1_[num] = com;
				stat = true;
			} else {
				com1_[numCom1_++] = com;
				stat = true;
			}
		}
		return stat;
	}

	/** Последовательная выдача имеющихся в буфере 1 команд (по кругу).
	 * 	@return Код текущей команды.
	 */
	eGB_COM getCom1() {
		if (cnt1_ >= numCom1_)
			cnt1_ = 0;
		return com1_[cnt1_++];
	}

	/** Запись команды в буфер 2.
	 * 	@param com Код команды.
	 * 	@retval True - в случае успешной записи.
	 * 	@retval False - если не удалось поместить команду в буфер.
	 */
	bool addCom2(eGB_COM com) {
		bool stat = false;
		if (numCom2_ < MAX_NUM_COM_BUF2) {
			com2_[numCom2_++] = com;
			stat = true;
		}
		return stat;
	}

	/** Последовательная выдача имеющихся в буфере 2 команд (по кругу).
	 * 	@return Код текущей команды.
	 */
	eGB_COM getCom2() {
		if (cnt2_ >= numCom2_)
			cnt2_ = 0;
		return com2_[cnt2_++];
	}

	/**	Запись срочной команды в конец очереди.
	 * 	@param com Код срочной команды
	 */
	void addFastCom(eGB_COM com) {
		for(uint_fast8_t i = 0; i < MAX_NUM_FAST_COM; i++) {
			if (comFast_[i] == GB_COM_NO) {
				comFast_[i] = com;
				break;
			}
		}
	}

	/**	Считывание срочной команды из начала очереди.
	 * 	@return Код срочной команды.
	 */
	eGB_COM getFastCom() {
		eGB_COM com = comFast_[0];
		// проверим, была ли команда в буфере
		if (com != GB_COM_NO) {
			// размер буфера команд достаточно мал
			// просто свдинем весь буфер на одну позицию влево,
			// а последнюю команду "обнулим"
			for(uint_fast8_t i = 1; i < MAX_NUM_FAST_COM; i++) {
				comFast_[i - 1] = comFast_[i];
			}
			comFast_[MAX_NUM_FAST_COM - 1] = GB_COM_NO;
		}
		return com;
	}

	/**	Запись байт данных в буфер.
	 * 	@param byte	Байт данных.
	 * 	@param num Индекс элемента массива.
	 */
	void setInt8(uint8_t byte, uint8_t num = 0) {
		if (num < 6)
			buf_[num] = byte;
	}

	/** Считывание байта данных.
	 * 	@param num Индекс элемента массива.
	 * 	@return Байт данных.
	 */
	uint8_t getInt8(uint8_t num = 0) const {
		uint8_t byte = 0;
		if (num < 6)
			byte = buf_[num];
		return byte;
	}

	/**	Запись 2-х байтного числа (uint16_t) в буфер.
	 * 	В буфере данные записываются в 1 и 2 элемент массива uint8_t.
	 * 	@param val Данные.
	 */
	void setInt16(uint16_t val) {
		*((uint16_t *) (buf_ + 1)) = val;
	}

	/** Считывание 2-х абйтного числа (uint16_t) из буфера.
	 * 	Данные хранятся в 1 и 2 элементах массива uint8_t.
	 * 	@return Данные.
	 */
	uint16_t getInt16() const {
		return *((uint16_t *) (buf_ + 1));
	}

//	// запись нескольких байт данных
//	// source - указатель на массив данных
//	// num - кол-во копируемых байт данных
//	void copyToBufer(uint8_t *source, uint8_t num) {
//		for (uint_fast8_t i = 0; i < num; i++) {
//			// учет размера буфера
//			if (i >= 6)
//				break;
//			byte_[i] = *source++;
//
//		}
//	}
//	// считывание нескольких байт данных
//	// destination- указатель на массив данных
//	// num - кол-во копируемых байт данных
//	void copyFromBufer(uint8_t *destination, uint8_t num) const {
//		for (uint_fast8_t i = 0; i < num; i++) {
//			// учет размера буфера
//			if (i >= 6)
//				break;
//			*destination++ = byte_[i];
//		}
//	}

	/**	Возвращает указатель на буфер данных.
	 * 	@return Указатель на буфер данных.
	 */
	uint8_t* getBuferAddress() {
		return buf_;
	}

private:
	// срочная команда (на изменение)
	eGB_COM comFast_[MAX_NUM_FAST_COM];
	// первый буфер команд
	eGB_COM com1_[MAX_NUM_COM_BUF1];
	// кол-во команд в первом буфере
	uint8_t numCom1_;
	// номер текущей команды в первом буфере
	uint8_t cnt1_;
	// второй буфер команд
	eGB_COM com2_[MAX_NUM_COM_BUF2];
	// кол-во команд во втором буфере
	uint8_t numCom2_;
	// номер текущей команды во втором буфере
	uint8_t cnt2_;
	// буфер данных
	uint8_t buf_[6];
};

class TJournalEntry {
public:
	TJournalEntry() {
		clear();
	}

	void clear() {
		currentDevice_ = GB_DEVICE_MAX;
		deviceJrn_ = GB_DEVICE_MAX;
		eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
		regime_ = GB_REGIME_MAX;
		numCom_ = 0;
		signalPusk_ = false;
		signalStop_ = false;
		signalMan_ = false;
		signalPrm_ = false;
		signalPrd_ = false;
		signalOut_ = false;

		numJrnEntries_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		addressFirstEntry_ = 0;

		currentEntry_ = 1;
		ready_ = false;
	}

	TDataTime dataTime;

	bool setCurrentDevice(eGB_DEVICE device) {
		bool stat = false;
		if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
			currentDevice_ = device;
			stat = true;
		} else
			currentDevice_ = GB_DEVICE_MAX;
		return stat;
	}
	eGB_DEVICE getCurrentDevice() const {
		return currentDevice_;
	}

	// запись\считывание устройства для которого сделана запись
	bool setDeviceJrn(eGB_DEVICE device) {
		bool stat = false;
		if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
			deviceJrn_ = device;
			stat = true;
		} else
			deviceJrn_ = GB_DEVICE_MAX;
		return stat;
	}
	eGB_DEVICE getDeviceJrn() const {
		return deviceJrn_;
	}

	// тип события
	bool setEventType(uint8_t val) {
		bool state = false;

		uint8_t min = 255;
		uint8_t max = 0;
		// установка мин/макс значения события, в зависимости от
		// текущего журнала
		if (currentDevice_ == GB_DEVICE_GLB) {
			min = MIN_JRN_EVENT_VALUE;
			max = MAX_JRN_EVENT_VALUE;
		} else if (currentDevice_ == GB_DEVICE_PRD) {
			min = GB_STATE_COM_MIN;
			max = GB_STATE_COM_MAX;
		} else if (currentDevice_ == GB_DEVICE_PRM) {
			min = GB_STATE_COM_MIN;
			max = GB_STATE_COM_MAX;
		} else if (currentDevice_ == GB_DEVICE_DEF) {
			min = 0;
			max = MAX_NUM_DEVICE_STATE;
		}

		if ((val >= min) && (val <= max)) {
			eventType_ = val;
			state = true;
		} else
			eventType_ = max;

		return state;
	}
	uint8_t getEventType() const {
		return eventType_;
	}


	/**	Установка записей для журналов ОПТИКИ.
	 * 	Считываются 4 байта, старшим вперед.
	 * 	Каждый установленный бит в них отвечает за свое событие.
	 * 	@param buf Указатель на массив из 4 байт данных.
	 * 	@retval True - всегда.
	 */
	bool setOpticEntry(uint8_t *buf) {
		uint8_t num = 0;
		uint8_t byte = 0;

		// В каждом байте записи считается кол-во установленных битов
		for(uint_fast8_t i = 0; i <= 3; i++) {
			byte = *buf++;
			opticEntry_[i] = byte;
			for(uint_fast8_t j = 1; j > 0; j <<= 1) {
				if (byte & j) {
					num++;
				}
			}
		}
		numOpticEntries_ = num;

		return true;
	}

	/** Возвращает кол-во активных событий в журнале ОПТИКИ.
	 * 	Для журнала событий это кол-во событий, для команд - команд.
	 * 	@return Кол-во активных собыйтив журнале ОПТИКИ
	 */
	uint8_t getNumOpticsEntries() const {
		return numOpticEntries_;
	}

	/** Ищет и возвращает код события, исходя из номера события.
	 * 	Т.е. если у нас в записи 5 активных событий, для каждого из них
	 * 	можно получить код записи (например номер команды).
	 * 	@param num Номер события, начиная с 1.
	 * 	@return Код события (0 - в случае ошибки).
	 */
	uint8_t getOpticEntry(uint8_t num) {
		uint8_t val = 0;
		uint8_t byte = 0;

		// проверка на допустимое значение
		if ((num >= 1) && (num <= numOpticEntries_)) {
			for(uint_fast8_t i = 0; i <= 3; i++) {
				// перебор 4-х байт, начиная с младшего
				byte = opticEntry_[3 - i];
				for(uint_fast8_t j = 0; j < 8; j++) {
					if (byte & (1 << j)) {
						if (--num == 0) {
							// номер установленного бита, от 1 до 32
							val = 1 + ((i * 8) + j);
						}
					}
				}
			}
		}
		return val;
	}

	// режим
	bool setRegime(eGB_REGIME regime) {
		bool state = false;
		if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
			regime_ = regime;
			state = true;
		} else
			regime_ = GB_REGIME_MAX;
		return state;
	}
	eGB_REGIME getRegime() const {
		return regime_;
	}

	// сигналы для журнала защиты
	bool setSignalDef(uint8_t val) {
		bool state = true;

		signalPusk_ = (val & 0x10) ? 1 : 0;
		signalStop_ = (val & 0x20) ? 1 : 0;
		signalMan_ = (val & 0x40) ? 1 : 0;
		signalPrm_ = (val & 0x01) ? 1 : 0;
		signalPrd_ = (val & 0x02) ? 1 : 0;
		signalOut_ = (val & 0x03) ? 1 : 0;

		return state;
	}
	uint8_t getSignalPusk() const {
		return signalPusk_;
	}
	uint8_t getSignalStop() const {
		return signalStop_;
	}
	uint8_t getSignalMan() const {
		return signalMan_;
	}
	uint8_t getSignalPrm() const {
		return signalPrm_;
	}
	uint8_t getSignalPrd() const {
		return signalPrd_;
	}
	uint8_t getSignalOut() const {
		return signalOut_;
	}

	// номер команды
	bool setNumCom(uint8_t num) {
		bool stat = false;
		if ((num > 0) && (num <= MAX_NUM_COM_PRD)) {
			numCom_ = num;
			stat = true;
		}
		return stat;
	}
	uint8_t getNumCom() const {
		return numCom_;
	}

	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;

		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			if (overflow_) {
				numJrnEntries_ = maxNumJrnEntry_;
				addressFirstEntry_ = val;
			} else {
				numJrnEntries_ = val;
				addressFirstEntry_ = 0;
			}
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntries() const {
		return numJrnEntries_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntries(uint16_t max) {
		bool stat = false;
		if (max <= GLB_JRN_MAX) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

	// переполнение журнала
	bool isOverflow() const {
		return overflow_;
	}

	// номер адреса текущей записи в журнале
	uint16_t getEntryAdress() const {
		return (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
	}

	// текущая запись
	uint16_t getCurrentEntry() const {
		return currentEntry_;
	}
	// следующая/предыдущая запись возвращает true если новое значение
	// отличается от предыдущего
	bool setNextEntry() {
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
		if (tmp != currentEntry_) {
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}
	bool setPreviousEntry() {
		bool stat = false;
		uint16_t tmp = currentEntry_;
		tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
		if (tmp != currentEntry_) {
			currentEntry_ = tmp;
			ready_ = false;
			stat = true;
		}
		return stat;
	}

	// утстановка и считывание флага получения информации о текущей записи
	bool setReady() {
		return (ready_ = true);
	}
	bool isReady() const {
		return ready_;
	}

private:
	// текущий журнал
	eGB_DEVICE currentDevice_;

	// устройство
	eGB_DEVICE deviceJrn_;

	// тип события
	uint8_t eventType_;

	// режим
	eGB_REGIME regime_;

	// сигналы для журнала защиты
	bool signalPusk_;
	bool signalStop_;
	bool signalMan_;
	bool signalPrm_;
	bool signalPrd_;
	bool signalOut_;

	// буфер записи для журналов ОПТИКИ
	uint8_t opticEntry_[4];

	// кол-во событий в одной записи журнала ОПТИКИ
	uint8_t numOpticEntries_;

	// кол-во записей в журнале
	uint16_t numJrnEntries_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// переполнение журнала
	bool overflow_;

	// адрес первой записи
	uint16_t addressFirstEntry_;

	// адрес текущей записи (отображаемой на экране)
	uint16_t currentEntry_;

	// номер команды в текущей записи
	uint16_t numCom_;

	// флаг получения информации о текущей записи
	bool ready_;
};

class TTest {
public:
	TTest() {
		clear();
	}
	// очистка списка сигналов
	void clear() {
		for(uint_fast8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++)
			signalList[i] = GB_SIGNAL_OFF;
		num_ = 1;
		currentSignal_ = GB_SIGNAL_OFF;
	}

	bool addSignalToList(eGB_TEST_SIGNAL signal) {
		bool stat = false;
		if ((num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX)) {
			signalList[num_++] = signal;
			stat = true;
		}
		return stat;
	}

	// записывает в cf, rz значения КЧ и РЗ, соответственно, для передачи в БСП
	/**	Преобразование тестового сигнала в значения для передачи в БСП.
	 * 	@param [out] cf	Код сигнала КЧ/Команды.
	 * 	@param [out] rz	Код сигнала РЗ.
	 *	@param sig Тестовый сигнал.
	 *
	 */
	void getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig) {
		if ((sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1; // 3 - кол-во кч ?!
		} else if ((sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4)) {
			rz = 0;
			cf = 1 + sig - GB_SIGNAL_CF1;
		} else if (sig == GB_SIGNAL_CF_NO_RZ) {
			rz = 1;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF_RZ) {
			rz = 2;
			cf = 1;
		} else if (sig == GB_SIGNAL_RZ) {
			rz = 1;
			cf = 0;
		} else if ((sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM4_RZ)) {
			rz = 2;
			cf = 3 + sig - GB_SIGNAL_COM1_RZ; // 3 - кол-во кч ?!
		} else if ((sig>=GB_SIGNAL_COM1_NO_RZ)&&(sig<=GB_SIGNAL_COM4_NO_RZ)) {
			rz = 1;
			cf = 3 + sig - GB_SIGNAL_COM1_NO_RZ; // 3 - кол-во кч ?!
		} else if (sig == GB_SIGNAL_CF) {
			rz = 0;
			cf = 1;
		} else {
			rz = 0;
			cf = 0;
		}
	}

	// возвращает кол-во сигналов в списке
	uint8_t getNumSignals() const {
		return num_;
	}

	// установка текущего сигнала
	void setCurrentSignal(uint8_t *s, eGB_TYPE_DEVICE type) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;

		if (type == AVANT_K400) {
			signal = getCurrentSignalK400(s);
		} else if (type == AVANT_RZSK) {
			signal = getCurrentSignalRZSK(s);
		} else if (type == AVANT_R400M) {
			signal = getCurrentSignalR400M(s);
		} else if (type == AVANT_OPTO) {
			signal = getCurrentSignalOpto(s);
		}
		currentSignal_ = signal;
	}
	eGB_TEST_SIGNAL getCurrentSignal() const {
		return currentSignal_;
	}

	// список сигналов
	uint8_t signalList[MAX_NUM_TEST_SIGNAL];

private:
	// кол-во возможных сигналов в тесте
	uint8_t num_;

	// текущий сигнал
	eGB_TEST_SIGNAL currentSignal_;

	// возвращает номер первого установленного бита 1..8, либо 0
	// проверка начинается с 0-ого бита
	uint8_t getSetBit(uint8_t byte) {
		uint8_t bit = 0;

		if (byte) {
			uint8_t b = 0;
			for (uint_fast8_t i = 1; i > 0; i <<= 1) {
				b++;
				if (byte & i) {
					bit = b;
					break;
				}
			}
		}
		return bit;
	}

	// добавление сигнала в список для К400
	eGB_TEST_SIGNAL getCurrentSignalK400(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		// сначала проверяется наличие КЧ1-КЧ2
		if (t & 0x01)
			signal = GB_SIGNAL_CF1;
		else if (t & 0x02)
			signal = GB_SIGNAL_CF2;
		else {
			// проверяется начичие команд с 1 по 8
			t = getSetBit(*(++s));
			if (t) {
				t = GB_SIGNAL_COM1 + t - 1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// проверяется начичие команд с 9 по 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// проверяется начичие команд с 17 по 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// проверяется начичие команд с 25 по 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** Добавление сигнала в список для РЗСК.
	 * 	бит: 7		6		5		4		3		2		1		0		;
	 * 	b1 : 0 		0 		0 		0 		[рз2] 	[рз1] 	[кч2] 	[кч1]	;
	 * 	b2 : [ком8] [ком7]	[ком6] 	[ком5] 	[ком4] 	[ком3] 	[ком2] 	[ком1]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 * 	Если есть сигнал РЗ, но нет ни команд ни КЧ. Считаем что КЧ есть!
	 * 	рз1 + кч1 = кч без блок			;
	 * 	рз2 + кч1 = кч с блок			;
	 * 	рз1 + комN = команда без блок 	;
	 * 	рз2 + комN = команда с блок		.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalRZSK(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if ((t & 0x05) == 0x05)
			signal = GB_SIGNAL_CF_NO_RZ;
		else if ((t & 0x09) == 0x09)
			signal = GB_SIGNAL_CF_RZ;
		else {
			if (t & 0x04) {
				t = getSetBit((*(++s)) & 0x0F);
				if (t != 0) {
					t = (t - 1) + GB_SIGNAL_COM1_NO_RZ;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// сигнал РЗ есть, а команд и кч нет -> считаем что КЧ есть
					signal = GB_SIGNAL_CF_NO_RZ;
				}
			} else if (t & 0x08) {
				t = getSetBit((*(++s)) & 0x0F);
				if (t != 0) {
					t = (t - 1) + GB_SIGNAL_COM1_RZ;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// сигнал РЗ есть, а команд и кч нет -> считаем что КЧ есть
					signal = GB_SIGNAL_CF_RZ;
				}
			}
		}
		return signal;
	}

	// добавление сигнала в список для Р400м
	eGB_TEST_SIGNAL getCurrentSignalR400M(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		// TODO Р400М 3-х концевая вывод сигналов теста
		// подумать как выводить при наличии РЗ + КЧ1, КЧ2 и т.д.

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;

			// выводится "КЧ и РЗ" при их одновременно наличии
			if (t & 0x01)
				signal = GB_SIGNAL_CF_RZ_R400M;
		} else if (t & 0x01)
			signal = GB_SIGNAL_CF;

		return signal;
	}

	/** Добавление сигнала в список для ОПТИКИ.
	 * 	TODO ОПТИКА сигналы сделаны только для 8-и командного варианта.
	 * 	бит: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		x 		x		x 		x 		[кч]	;
	 * 	b2 : [ком8] [ком7]	[ком6] 	[ком5] 	[ком4] 	[ком3] 	[ком2] 	[ком1]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalOpto(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x01) {
			signal = GB_SIGNAL_CF;
		} else {
			t = getSetBit(*(++s));
			if (t != 0) {
				t = (t - 1) + GB_SIGNAL_COM1;
				signal = (eGB_TEST_SIGNAL) t;
			}
		}
		return signal;
	}
};

/// структура параметров работы с последовательным портом
class TUartData {
public:
	/// Интерфейс связи
	TInterface Interface;
	/// Скорость передачи
	TBaudRate BaudRate;
	/// Количество битов данных
	TDataBits DataBits;
	/// Протокол контроля четности
	TParity Parity;
	/// Количество стоп битов
	TStopBits StopBits;
};

/// Структура параметров БСП
struct stGBparam {
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;
	eGB_TYPE_DEVICE typeDevice;

	// пароль
	TPassword password;

	// дата/время
	TDataTime dataTime;

	// измеряемые параметры
	TMeasuredParameters measParam;

	// текущее состояние устройств
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// параметры для работы с последовательным портом
	TUartData Uart;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;

	// тесты
	TTest test;
};

#endif /* GLBDEF_H_ */
