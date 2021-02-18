/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef JOURNAL_ENTRY_HPP_
#define JOURNAL_ENTRY_HPP_

#include "dateTime.hpp"
#include "glbDefine.h"

class TJournalEntry {
#define MAX_LOG_SIZE 16

    /// Структура записи для журнала безопасности.
    typedef struct __attribute__ ((__packed__)) {
        bool ready;         ///< Флаг наличия записи (получена из БСП).
        uint8_t user;
        uint8_t userSrc;
        uint8_t event;
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint16_t msecond;
        uint8_t regime;
    } log_t;

public:
    TJournalEntry() { clear(); }

	void clear() {
        currentDevice_ = GB_DEVICE_MAX;
        deviceJrn_ = GB_DEVICE_K400_MAX;
        eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
        numCom_ = 0;
        sourceCom_ = GB_SOURCE_COM_MAX;
        srcCom_ = 0;
        signalPusk_ = false;
        signalStop_ = false;
        signalMan_ = false;
        signalPrm_ = false;
        signalPrd_ = false;
        signalOut_ = false;
        signals_ = 0;

        numJrnEntries_ = 0;
        maxNumJrnEntry_ = 0;
        overflow_ = false;
        addressFirstEntry_ = 0;

        currentEntry_ = 1;

        log.ready = false;
        log.user = USER_MAX;
        log.userSrc = USER_SOURCE_MAX;
        log.event = 0;
        log.year = 0;
        log.month = 0;
        log.day = 0;
        log.hour = 0;
        log.minute = 0;
        log.second = 0;
        log.msecond = 0;

        val = false;
    }

	// время для передачи по протоколам
	TDateTime dateTimeTr;

	// значение для передачи по протоколу
    bool val;

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
	bool setDeviceJrn(eGB_DEVICE_K400 device) {
		bool stat = false;
		if ((device >= GB_DEVICE_K400_MIN) && (device < GB_DEVICE_K400_MAX)) {
			deviceJrn_ = device;
			stat = true;
		} else
			deviceJrn_ = GB_DEVICE_K400_MAX;
		return stat;
	}
	eGB_DEVICE_K400 getDeviceJrn() const {
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

	// источник команды
	bool setSrcCom(uint8_t val) {
		srcCom_ = val & 0x07;
		return true;
	}
	uint8_t getSrcCom() const {
		return srcCom_;
	}


	/**	Установка записей для журналов ОПТИКИ.
	 * 	Считываются 4 байта, старшим вперед.
	 * 	Каждый установленный бит в них отвечает за свое событие.
	 * 	@param buf Указатель на массив из 4 байт данных.
	 * 	@retval True - всегда.
	 */
	bool setOpticEntry(uint8_t *buf) {
		uint8_t num = 0;

		// В каждом байте записи считается кол-во установленных битов
		for(uint_fast8_t i = 0; i <= 3; i++) {
			uint8_t byte = *buf++;
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
	 *
	 * 	@param num Номер события, начиная с 1.
	 * 	@return Код события (0 - в случае ошибки).
	 */
	uint8_t getOpticEntry(uint8_t num) {
		uint8_t val = 0;

		// проверка на допустимое значение
		if ((num >= 1) && (num <= numOpticEntries_)) {
			for(uint_fast8_t i = 0; i <= 3; i++) {
				// перебор 4-х байт, начиная с младшего
				uint8_t byte = opticEntry_[3 - i];
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

	/**	Установка записей для журналов ОПТИКИ ЦПП.
	 *
	 * 	Считываются 4 байта, СТАРШИМ вперед.
	 * 	Каждый установленный бит в них отвечает за свое событие.
	 *
	 * 	@param buf Указатель на массив из 4 байт данных.
	 * 	@retval True - всегда.
	 */
	bool setOpticEntryDR(uint8_t *buf) {
		uint8_t num = 0;

		// В каждом байте записи считается кол-во установленных битов
		for(uint_fast8_t i = 0; i <= 3; i++) {
			uint8_t byte = *buf++;
			opticEntryDR_[i] = byte;
			for(uint_fast8_t j = 1; j > 0; j <<= 1) {
				if (byte & j) {
					num++;
				}
			}
		}
		numOpticEntriesDR_ = num;

		return true;
	}

	/** Возвращает флаг команды ЦПП.
	 *
	 * 	@param com Номер команды для которой проверяется флаг 1..32.
	 * 	@return 1 - если команда передана по ЦПП, 0 - с дискретного входа.
	 */
	uint8_t getOpticEntryDR(uint8_t com) {
		eGB_SOURCE_COM val = GB_SOURCE_COM_DI;

		if ((com > 0) && (com < MAX_NUM_COM_PRD)) {
			com--;

			uint8_t i = (MAX_NUM_COM_PRD - 1) / 8;
			while(com > 8) {
				com -= 8;
				i--;
			}

			if (opticEntryDR_[i] & (1 << com)) {
				val = GB_SOURCE_COM_DR;
			}
		}

		return val;
	}

    /// Устанавливает режим работы.
    void setRegime(eGB_REGIME regime) {
        log.regime = (regime < GB_REGIME_MAX) ? regime : GB_REGIME_MAX;
    }
    /// Возвращает режим работы.
    eGB_REGIME getRegime() const { return static_cast<eGB_REGIME> (log.regime); }

	// сигналы для журнала защиты
	bool setSignalDef(uint8_t val) {
		bool state = true;

		signalPusk_ = (val & 0x10) ? 1 : 0;
		signalStop_ = (val & 0x20) ? 1 : 0;
		signalMan_ = (val & 0x40) ? 1 : 0;
		signalPrm_ = (val & 0x01) ? 1 : 0;
		signalPrd_ = (val & 0x02) ? 1 : 0;
		signalOut_ = (val & 0x04) ? 1 : 0;
		signals_ = val;

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
	uint8_t getSignals() const {
		return signals_;
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

    /** Устанавливает источник передаваемой команды.
     *
	 *	В случае не корректного значения, в параметр будет записано значение
	 *	\a GB_SOURCE_COM_MAX.
	 *
	 *	@see eGB_SOURCE_COM
	 *	@param source Источник передаваемой команды.
	 *	@return True - в случае корректного значения, False - иначе.
	 */
	bool setSourceCom(eGB_SOURCE_COM source) {
		bool stat = false;

		switch(source) {
			case GB_SOURCE_COM_DI:
			case GB_SOURCE_COM_DR:
			case GB_SOURCE_COM_MAX: {
				stat = true;
			}
			break;
		}

		if (!stat) {
			source = GB_SOURCE_COM_MAX;
		}

		sourceCom_ = source;
		return stat;
	}

    /** Возвращает источник передаваемой команды.
	 *
	 *	@return Источник передаваемой команды.
	 */
	eGB_SOURCE_COM getSourceCom() const {
		return sourceCom_;
	}

    /** Устанавливает количество записей в журнале.
     *
     *  В случае переполнения журнала, новые записи начинают перезаписывать
     *  старые. При этом выставляется флаг переполнения 0xC000 и в принимаемом
     *  значении указывается адрес "первой" т.е. самой новой записи.
     *
     *  @param[in] val Количество записей в журнале или адрес первой записи.
     *  @return true если количество записей не превышает допустимое, иначе false.
     */
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

    /** Возвращает количесто записей в журнале.
     *
     *  @return Количество записей в журнале.
     */
	uint16_t getNumJrnEntries() const {
		return numJrnEntries_;
	}

    /** Устанавливает максимальное количество записей в журнале.
     *
     *
     *  @param[in] max Максимальное количество записей в журнале.
     *  @return true если занчение не превышает допустимое, иначе false.
     */
	bool setMaxNumJrnEntries(uint16_t max) {
		bool stat = false;

		if (max <= GLB_JRN_MAX) {
			stat = true;
			maxNumJrnEntry_ = max;
		}

		return stat;
	}

    /** Возвращает максимальное количество записей в журнале.
     *
     *  @return Максимальное количество записей в журнале.
     */
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

    /** Возвращает абсолютный адрес текущей записи в журнале.
     *
     *  Журнал перезаписывается по кругу, поэтому начало и конец журнала
     *  постоянно перезаписываются.
     *  В данной функции вычисляется абсолютный адрес в журнале, чтобы при
     *  добавлении новой записи в журнал, текущая запись на экране не менялась.
     *
     *  @return Абсолютный адрес записи в журнале.
     */
	uint16_t getEntryAdress() const {
        uint16_t address = 0;

        if (numJrnEntries_ > 0) {
            address= (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
        }

        return address;
	}

    /** Возвращает адрес текущей записи в журнале.
     *
     *  @return Адрес текущей записи.
     */
	uint16_t getCurrentEntry() const {
		return currentEntry_;
	}

    /** Переход к следующей записи в журнале.
     *
     *  @return true если переход был, иначе false.
     */
	bool setNextEntry() {
		bool stat = false;        
        uint16_t tmp = currentEntry_;

        tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
        if (tmp != currentEntry_) {
			currentEntry_ = tmp;
            log.ready = false;
			stat = true;
		}
		return stat;
	}

    /** Переход к предыдущей записи в журнале.
     *
     *  @return true если переход был, иначе false.
     */
	bool setPreviousEntry() {
		bool stat = false;
		uint16_t tmp = currentEntry_;

		tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
		if (tmp != currentEntry_) {
			currentEntry_ = tmp;
            log.ready = false;
			stat = true;
		}

		return stat;
	}

    /** Устанавливает номер записи.
     *
     *  @param[in] num Номер записи.
     *  @return true если номер не превышает допустимый, иначе false.
     */
	bool setNumEntry(uint16_t num) {
		bool stat = false;

		if (num <= numJrnEntries_) {
			currentEntry_ = num;
            log.ready = false;
			stat = true;
		}

		return stat;
	}

    /// Устанавливает флаг наличия записи (запись получена).
    void setReady() { log.ready = true; }
    /// Проверяет флаг наличия записи.
    bool isReady() const { return log.ready; }

    /// Устанавливает год (BCD формат).
    void setYear(uint8_t year) { log.year = year; }
    /// Возвращает год в BCD формате.
    uint8_t getYear() const { return log.year; }

    /// Устанавливает месяц (BCD формат).
    void setMonth(uint8_t month) { log.month = month; }
    /// Возвращает месяц в BCD формате.
    uint8_t getMonth() const { return log.month; }

    /// Устанавливает день (BCD формат).
    void setDay(uint8_t day) { log.day = day; }
    /// Вовзращает день в BCD формате.
    uint8_t getDay() const { return log.day; }

    /// Устанавливает часы (BCD формат).
    void setHour(uint8_t hour) { log.hour = hour; }
    /// Возвращает час в BCD формате.
    uint8_t getHour() const { return log.hour; }

    /// Устанавливает минуты (BCD формат).
    void setMinute(uint8_t minute) { log.minute = minute; }
    /// Возвращает минуты в BCD формате.
    uint8_t getMinute() const { return log.minute; }

    /// Устанавливает секунды (BCD формат).
    void setSecond(uint8_t second) { log.second = second; }
    /// Возвращает секунды в BCD формате.
    uint8_t getSecond() const { return log.second; }

    /// Устанавливает миллисекунды.
    void setMSecond(uint16_t msecond) { log.msecond = msecond; }
    /// Возвращает миллисекунды.
    uint16_t getMSecond() const { return log.msecond; }

    /// Устанавливает пользовтеля.
    void setUser(uint8_t user) {
        log.user = user < USER_MAX ? user : USER_MAX;
    }
    /// Возвращает пользовтеля.
    user_t getUser() const { return static_cast<user_t> (log.user); }

    /// Устанавливает источник доступа пользователя.
    void setUserSrc(uint8_t usersrc) {
        log.userSrc = usersrc < USER_SOURCE_MAX ? usersrc : USER_SOURCE_MAX;
    }
    /// Возвращает источник доступа пользователя.
    userSrc_t getUserSrc() const { return static_cast<userSrc_t> (log.userSrc); }

    /// Устанавливает событие.
    void setEvent(uint8_t event) { log.event = event; }
    /// Вовзращает событие.
    uint8_t getEvent() const { return log.event; }

private:
    log_t log;

	// текущий журнал
	eGB_DEVICE currentDevice_;

	// устройство
	eGB_DEVICE_K400 deviceJrn_;

	// тип события
	uint8_t eventType_;

	// источник команды (номер аппарата с которого пришла команда)
	uint8_t srcCom_;

	// сигналы для журнала защиты
	bool signalPusk_;
	bool signalStop_;
	bool signalMan_;
	bool signalPrm_;
	bool signalPrd_;
	bool signalOut_;	// Выход приемника (ПРМ2 или РЗвых)
	uint8_t signals_;

	// буфер записи для журналов ОПТИКИ
	uint8_t opticEntry_[4];

	// буфер записи для журналов ОПТИКИ ЦПП
	uint8_t opticEntryDR_[4];

	// кол-во событий в одной записи журнала ОПТИКИ
	uint8_t numOpticEntries_;

	// кол-во событий в одной записи журнала ОПТИКИ ЦПП
	uint8_t numOpticEntriesDR_;

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

	// источник передаваемой команды
	eGB_SOURCE_COM sourceCom_;
};

#endif /* JOURNAL_ENTRY_HPP_ */
