/*
 * devicePrm.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRM_H_
#define PARAMPRM_H_

// #include "glbDefine.h"

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
/// команда ВЧ в ЦС
#define PRM_COM_DR_MIN		1
#define PRM_COM_DR_MAX		32
#define PRM_COM_DR_DISC		1
#define PRM_COM_DR_FRACT	1
#define PRM_COM_DR_MIN_F	(PRM_COM_DR_MIN / PRM_COM_DR_FRACT)
#define PRM_COM_DR_MAX_F	(PRM_COM_DR_MAX / PRM_COM_DR_FRACT)
#define PRM_COM_DR_DISC_F	(PRM_COM_DR_DISC/ PRM_COM_DR_FRACT)


/// класс для параметров и настроек приемника
class TDevicePrm {
public:
	TDevicePrm() {
		numCom_ = 0;
		timeOn_ = PRM_TIME_ON_MIN_F;

		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			blockCom_[i] = false;
			timeOff_[i] = PRM_TIME_OFF_MIN_F;
			indCom_[i] = 0;
		}

		for(uint_fast8_t i = 0; i < MAX_NUM_COM_PRM; i++) {
			comDR_[i] = i + 1;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
		stateDR_ = false;
	}

	TDeviceStatus status;

	/**	Установка текущего кол-ва команд в ПРМ.
	 *
	 * 	Если новое значение выходит за диапазон допустимых значений, устанавли
	 * 	вается 0. Далее если команд нет, ПРМ отключается. Иначе включается.
	 *
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

			act |= GB_ACT_NEW;
		}
		this->status.setEnable(val != 0);

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

	/**	Установка параметра "Трансляция ЦС"
	 *
	 * 	@param val Выкл(0)/Вкл(1).
	 * 	@return True - в случае успешной записи, False - иначе.
	 */
	bool setStateDR(uint8_t val) {
		bool stat = false;
		if (val <= 1) {
			stateDR_ = (bool) val;
			stat = true;
		}
		return stat;
	}

	/**	Возвращает значение параметра "Трансляция ЦС".
	 *
	 * 	@return Выкл(0)/Вкл(1).
	 */
	/** Возвращает значение параметра Резервирование.
	 * 	@return Резервирование.
	 */
	bool getStateDR() const {
		return stateDR_;
	}


	/** Запись флагов блокировки команд ЦС для восьмерки команд.
	 *
	 *	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 *	@param val Новое значение флагов.
	 *	@return True - в случае успешной записи, False - иначе.
	 */
	bool setBlockComDR8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRM / 8)) {
			blockComDR_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	Возвращает текущий флаг блокировки для заданной команды.
	 *
	 * 	@param num Номер команды.
	 * 	@return True- команда заблокирована, False - иначе.
	 */
	bool getBlockComDR(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (blockComDR_[pl] & (1 << num)) != 0;
	}

	/**	Возвращает текущие флаги блокировки для заданной восьмерки команд.
	 *
	 *	Каждый бит является флагом для соответствующей команды, младший бит -
	 *	младшая команда. Установленный бит означает что команда заблокирована.
	 *
	 * 	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 * 	@return Текущее состояние флагов блокировки команд.
	 */
	uint8_t getBlockComDR8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRM / 8))
			val = blockComDR_[num];
		return val;
	}

	/**	Запись массива значений ЦС-команд для ВЧ-команд.
	 *
	 * 	@param buf Адрес первого элемента массива значений ЦС комманд.
	 * 	@return True - в случае успешной записи, False - Иначе.
	 */
	bool setComDR(uint8_t *buf) {
		bool stat = true;
		for (uint_fast8_t i = 0; i < numCom_; i++) {
			uint8_t val = ((*buf) / PRM_COM_DR_DISC_F) * PRM_COM_DR_DISC_F;

			// записано в таком виде т.к. иначе портится автоформат в Eclipse
			if (val >= PRM_COM_DR_MIN_F) {
				if (val <= PRM_COM_DR_MAX_F)
					comDR_[i] = val;
				else
					stat = false;
			} else
				stat = false;
			buf++;
		}
		return stat;
	}

	/**	Возвращает номер ЦС-команды для заданной ВЧ-команды.
	 *
	 * 	@param num Номер ВЧ-команды, начиная с 0.
	 * 	@return Номер ЦС-команды.
	 *
	 */
	uint8_t getComDR(uint8_t num) const {
		uint8_t com = 0;
		if (num < numCom_) {
			com = comDR_[num] * PRM_COM_DR_FRACT;
		}
		return com;
	}

	/** Установка текущего стостояния светодиодов на блоке БСК
	 *
	 *	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 *	@param val Состояние светодиодов.
	 *	@return True - в случае успешной записи, False - иначе.
	 */
	bool setIndCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			indCom_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	Возвращает текущее состояние светодиода комадны на блоке БСК.
	 *
	 * 	@param num Номер команды.
	 * 	@return True- светодиод горит, False - иначе.
	 */
	bool getIndCom(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (indCom_[pl] & (1 << num)) != 0;
	}

	/**	Устанавливает текущее состояние светодиодов команд на блоке БСК.
	 *
	 * 	Каждый бит является флагом для соответствующей команды, младший бит -
	 * 	младшая команда. Установленный бит означает что светодиод горит.
	 *
	 * 	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 * 	@return Текущее состояние флагов блокировки команд.
	 */
	uint8_t getIndCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = indCom_[num];
		return val;
	}

	/**	Провекра наличия горящих светодиодов индикации команд на блоке БСК,
	 *
	 * 	@retval True - если горит светодиорд хотя бы одной команда.
	 * 	@retval False - горящих светодиодов команд нет.
	 */
	bool isIndCom() const {
		uint8_t ind = 0;

		for(uint8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			ind |= indCom_[i];
		}

		return (ind != 0);
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

	// ЦС
	bool stateDR_;

	// блокированные команды, true - блокированная
	uint8_t blockComDR_[MAX_NUM_COM_PRM / 8];

	// команда ВЧ в ЦС
	uint8_t comDR_[MAX_NUM_COM_PRM];

	// состояние индикации светодиодов на блоке БСК
	uint8_t indCom_[MAX_NUM_COM_PRM / 8];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};

#endif /* PARAMPRM_H_ */
