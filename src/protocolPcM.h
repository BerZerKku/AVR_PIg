/*
 * protocolBspM.h
 *
 *  Created on: 05.09.2014
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCM_H_
#define PROTOCOLPCM_H_

#include "protocolModbus.h"
#include "glbDefine.h"
#include "paramBsp.h"
#include "debug.h"

class TProtocolPcM : public TProtocolModbus
{
	/// Адреса регистров и флагов устройства
	enum ADR{
		// Дата и время
		ADR_YEAR 			= 1,	///< Год.
		ADR_MONTH,					///< Месяц.
		ADR_DAY,					///< День.
		ADR_HOUR,					///< Часы.
		ADR_MINUTE,					///< Минуты.
		ADR_SECOND,					///< Секунды.
		// Пароли
		ADR_PASSWORD 		= 7,	///< Текущий пароль.
		ADR_NEW_PASSWORD,			///< Новый пароль.
		// Текущее состояние
		ADR_GLB_FAULT		= 10,	///< Код неисправности Общий.
		ADR_GLB_WARNING,			///< Код предупреждения Общий.
		ADR_PRM_FAULT,				///< Код неисправности Приемника.
		ADR_PRM_WARNING,			///< Код предупреждения Приемника.
		ADR_PRM_REGIME,				///< Режим Приемника.
		ADR_PRM_STATE,				///< Состояние Приемника.
		ADR_PRM_DOP,				///< Дополнительный байт Приемника.
		ADR_PRD_FAULT,				///< Код неисправности Передатчика.
		ADR_PRD_WARNING,			///< Код предупреждения Передатчика.
		ADR_PRD_REGIME,				///< Режим Передатчика.
		ADR_PRD_STATE,				///< Состояние Передатчика.
		ADR_PRD_DOP,				///< Дополнительный байт Передатчика.
		ADR_DEF_FAULT,				///< Код неисправности Защиты.
		ADR_DEF_WARNING,			///< Код предупреждения Защиты.
		ADR_DEF_REGIME,				///< Режим Защиты.
		ADR_DEF_STATE,				///< Состояние Защиты.
		ADR_DEF_DOP,				///< Дополнительный байт Защиты.
		// Журнал событий
		ADR_JRN_EVT_CNT_PWR = 27,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_EVT_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_EVT_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_EVT_REZ1,			///< Резерв.
		ADR_JRN_EVT_REZ2,			///< Резерв.
		ADR_JRN_EVT_DEV,			///< Имя устройства.
		ADR_JRN_EVT_TYPE,			///< Тип события.
		ADR_JRN_EVT_REG,			///< Режим работы.
		ADR_JRN_EVT_REZ3,			///< Резерв.
		ADR_JRN_EVT_REZ4,			///< Резерв.
		ADR_JRN_EVT_REZ5,			///< Резерв.
		ADR_JRN_EVT_REZ6,			///< Резерв.
		ADR_JRN_EVT_MSECOND,		///< Миллисекунды.
		ADR_JRN_EVT_SECOND,			///< Секунды.
		ADR_JRN_EVT_MINUTE,			///< Минуты.
		ADR_JRN_EVT_HOUR,			///< Час.
		ADR_JRN_EVT_WDAY,			///< День недели.
		ADR_JRN_EVT_DAY,			///< День.
		ADR_JRN_EVT_MONTH,			///< Месяц.
		ADR_JRN_EVT_YEAR,			///< Год.
		// Журнал приемника
		ADR_JRN_PRM_CNT_PWR = 50,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_PRM_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_PRM_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_PRM_REZ1,			///< Резерв.
		ADR_JRN_PRM_REZ2,			///< Резерв.
		ADR_JRN_PRM_DEV,			///< Имя устройства.
		ADR_JRN_PRM_TYPE,			///< Номер команды.
		ADR_JRN_PRM_REG,			///< Событие.
		ADR_JRN_PRM_REZ3,			///< Резерв.
		ADR_JRN_PRM_REZ4,			///< Резерв.
		ADR_JRN_PRM_REZ5,			///< Резерв.
		ADR_JRN_PRM_REZ6,			///< Резерв.
		ADR_JRN_PRM_MSECOND,		///< Миллисекунды.
		ADR_JRN_PRM_SECOND,			///< Секунды.
		ADR_JRN_PRM_MINUTE,			///< Минуты.
		ADR_JRN_PRM_HOUR,			///< Час.
		ADR_JRN_PRM_WDAY,			///< День недели.
		ADR_JRN_PRM_DAY,			///< День.
		ADR_JRN_PRM_MONTH,			///< Месяц.
		ADR_JRN_PRM_YEAR,			///< Год.
		// Журнал передатчика
		ADR_JRN_PRD_CNT_PWR = 74,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_PRD_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_PRD_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_PRD_REZ1,			///< Резерв.
		ADR_JRN_PRD_REZ2,			///< Резерв.
		ADR_JRN_PRD_DEV,			///< Имя устройства.
		ADR_JRN_PRD_TYPE,			///< Номер команды.
		ADR_JRN_PRD_REG,			///< Событие.
		ADR_JRN_PRD_REZ3,			///< Резерв.
		ADR_JRN_PRD_REZ4,			///< Резерв.
		ADR_JRN_PRD_REZ5,			///< Резерв.
		ADR_JRN_PRD_REZ6,			///< Резерв.
		ADR_JRN_PRD_MSECOND,		///< Миллисекунды.
		ADR_JRN_PRD_SECOND,			///< Секунды.
		ADR_JRN_PRD_MINUTE,			///< Минуты.
		ADR_JRN_PRD_HOUR,			///< Час.
		ADR_JRN_PRD_WDAY,			///< День недели.
		ADR_JRN_PRD_DAY,			///< День.
		ADR_JRN_PRD_MONTH,			///< Месяц.
		ADR_JRN_PRD_YEAR,			///< Год.
		// Журнал защиты
		ADR_JRN_DEF_CNT_PWR = 98,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_DEF_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_DEF_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_DEF_REZ1,			///< Резерв.
		ADR_JRN_DEF_REZ2,			///< Резерв.
		ADR_JRN_DEF_DEV,			///< Имя устройства.
		ADR_JRN_DEF_TYPE,			///< Состояние сигналов.
		ADR_JRN_DEF_REG,			///< Событие.
		ADR_JRN_DEF_REZ3,			///< Резерв.
		ADR_JRN_DEF_REZ4,			///< Резерв.
		ADR_JRN_DEF_REZ5,			///< Резерв.
		ADR_JRN_DEF_REZ6,			///< Резерв.
		ADR_JRN_DEF_MSECOND,		///< Миллисекунды.
		ADR_JRN_DEF_SECOND,			///< Секунды.
		ADR_JRN_DEF_MINUTE,			///< Минуты.
		ADR_JRN_DEF_HOUR,			///< Час.
		ADR_JRN_DEF_WDAY,			///< День недели.
		ADR_JRN_DEF_DAY,			///< День.
		ADR_JRN_DEF_MONTH,			///< Месяц.
		ADR_JRN_DEF_YEAR,			///< Год.
		// Измреения
		ADR_MEAS_U_OUT 		= 123,	///< Выходное напряжение.
		ADR_MEAS_I_OUT,				///< Выходной ток.
		ADR_MEAS_UC,				///< Запас по КЧ (КЧ1).
		ADR_MEAS_UD,				///< Запас по Uш.
		// Индикация команд
		ADR_IND_COM_PRM_16	= 140,	///< Индикация команд приемника 1-16.
		ADR_IND_COM_PRM_32,			///< Индикация команд приемника 17-32.
		ADR_IND_COM_PRD_16,			///< Индикация команд передатчика 1-16.
		ADR_IND_COM_PRD_32,			///< Индикация команд передатчика 16-32.
		// Версии прошивок
		ADR_IC_BSP_MCU 		= 156,
		ADR_IC_BSP_DSP,
		ADR_IC_PI_MCU,
		ADR_IC_BSK_PRD1,
		ADR_IC_BSK_PRM1,
		ADR_IC_BSK_PRD2,
		ADR_IC_BSK_PRM2
	};

public:
	/**	Конструктор.
	 *
	 * 	@param sParam Указатель на структуру параметров.
	 * 	@param buf Указатель на буфер данных.
	 * 	@param size Размер буфера данных.
	 */
	TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	Деструктор.
	 *
	 */
//	~TProtocolPcM() {};

private:
	stGBparam * const sParam_;	///< Структура параметров

	/// Обработка принятой команды чтения флагов
	bool readCoilCom();

	/// Обработка принятой команды чтения регистров
	bool readRegisterCom();

	/// Обработка принятой команды чтения ID
	bool readIdCom();
};


#endif /* PROTOCOLPM_H_ */
