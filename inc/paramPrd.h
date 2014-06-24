/*
 * devicePrd.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRD_H_
#define PARAMPRD_H_

// #include "glbDefine.h"

/// ПРД
/// время включения (задержка срабатывания дискретного входа)
/// было от 5 до 20, изменено по требованию АГ
#define PRD_TIME_ON_MIN		5
#define PRD_TIME_ON_MAX		20
#define PRD_TIME_ON_DISC	1
#define PRD_TIME_ON_FRACT	1
#define PRD_TIME_ON_MIN_F	(PRD_TIME_ON_MIN / PRD_TIME_ON_FRACT)
#define PRD_TIME_ON_MAX_F	(PRD_TIME_ON_MAX / PRD_TIME_ON_FRACT)
#define PRD_TIME_ON_DISC_F	(PRD_TIME_ON_DISC / PRD_TIME_ON_FRACT)
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
/// длительность команды для ОПТИКИ
#define PRD_COM_A_MIN		0
#define PRD_COM_A_MAX		MAX_NUM_COM_PRD
#define PRD_COM_A_DISC		1
#define PRD_COM_A_FRACT		1
#define PRD_COM_A_MIN_F		(PRD_COM_A_MIN / PRD_COM_A_FRACT)
#define PRD_COM_A_MAX_F		(PRD_COM_A_MAX / PRD_COM_A_FRACT)
#define PRD_COM_A_DISC_F	(PRD_COM_A_DISC / PRD_COM_A_FRACT)


/// класс для параметров и настроек передатчика
class TDevicePrd {
public:
	TDevicePrd() {
		numCom_ = 0;
		timeOn_ = PRD_TIME_ON_MIN_F;
		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
			blockCom_[i] = 0;
			longCom_[i] = 0;
			blockComDR_[i] = 0;
		}

		stateDR_ = false;
		durationL_ = PRD_DURAT_L_MIN_F;
		durationO_ = PRD_DURAT_O_MIN_F;
		testCom_ = false;
		numComA_ = PRD_COM_A_MIN_F;

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

	/**	Установка количества команд группы А
	 * 	Если новое значение выходит за диапазон допустимых значений, устанавли
	 * 	вается 0.
	 * 	@param val Количество команд на ПРД.
	 * 	@return True - в случае успешной остановки параметра. False - иначе.
	 */
	uint8_t setNumComA(uint8_t val) {
		bool stat = false;

		if (val >= PRD_COM_A_MIN_F) {
			if (val <= PRD_COM_A_MAX_F) {
				numComA_ = val;
				stat = true;
			}
		}

		return stat;
	}

	/**	Возвращает кол-во команд группы А.
	 * 	@return Количество команд группы А.
	 */
	uint8_t getNumComA() const {
		return numComA_;
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
		if (num < (MAX_NUM_COM_PRD / 8)) {
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

	/**	Установка параметра "Трансляция ЦС"
	 *
	 * 	@param val Выкл(0)/Вкл(1).
	 * 	@param True - в случае успешной записи, False - иначе.
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
		if (num < (MAX_NUM_COM_PRD / 8)) {
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
		if (num < (MAX_NUM_COM_PRD / 8))
			val = blockComDR_[num];
		return val;
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

	// ЦС
	bool stateDR_;

	// команды группы А
	uint8_t numComA_;

	// блокированные команды, true - блокированная
	uint8_t blockComDR_[MAX_NUM_COM_PRD / 8];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// макстмалбное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};

#endif /* PARAMPRD_H_ */
