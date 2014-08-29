/*
 * bsp.h
 *
 *  Created on: 23.05.2014
 *      Author: Shcheblykin
 */

#ifndef __PROTOCOL_MODBUS_H_
#define __PROTOCOL_MODBUS_H_

#include <stdint.h> 

#define INLINE __attribute__((always_inline))

#ifdef AVR
#define VOLATILE volatile
#else
#define VOLATILE
#endif

/**	\brief Класс работы с протоколом MODBUS.
 *
 *
 *
 * 	@see MAX_ERRORS
 */
class TProtocolModbus{
private:
	/// Пауза между принятыми байтами, при которой идет сброс на начало приема.
	static const uint16_t DELAY_RESET = 15000;	// лучше кратно 1500
	/// Пауза между принятыми байтами, при которой определяется окончание приема.
	static const uint16_t DELAY_READ  = 35000;	// лучше кратно 3500
public:
	/// Максимальное количество регистров доступных для чтения в одном запрсе
	static const uint16_t MAX_NUM_REGISTERS = 32;
	/// Максимальное количество флагов доступных для чтения в одном запросе
	static const uint16_t MAX_NUM_COILS = 256;

	/// Адрес устройства в сети по умолчанию (т.е. ошибка)
	static const uint8_t ADDRESS_ERR = 255;
	/// Минимальный адрес устройства в сети
	static const uint8_t ADDRESS_MIN = 1;
	/// Максимальный адрес устройства в сети
	static const uint8_t ADDRESS_MAX = 247;

	/// Состояния работы протокола
	enum STATE {
		STATE_OFF,			///< Протокол выключен.
		STATE_READ,			///< Чтение посылки.
		STATE_READ_ERROR,	///< Ошибка в принятом пакете (1.5мc < пауза < 3.5мс)
		STATE_READ_OK,		///< Посылка принята.
		STATE_WRITE_WAIT,	///< Ожидание нужных данных.
		STATE_WRITE,		///< Отправка посылки.
		STATE_ERROR			///< Ошибка в работе протокола.
	};

	/// Команды доступные в данном классе
	enum COM {
		COM_01H_READ_COIL 					= 0x01,	///< Чтение флагов.
		COM_03H_READ_HOLDING_REGISTER 		= 0x03,	///< Чтение внутренних регистров.
		COM_05H_WRITE_SINGLE_COIL			= 0x05,	///< Запись одного флага.
		COM_06H_WRITE_SINGLE_REGISTER		= 0x06,	///< Запись одного внутреннего регистра.
		COM_0FH_WRITE_MULTIPLIE_COILS		= 0x0F,	///< Запись группы флагов.
		COM_10H_WRITE_MULITPLIE_REGISTERS	= 0x10,	///< Запись группы внутренних регистров.
		COM_11H_SLAVE_ID					= 0x11, ///< Чтение ID подчиненного
	};

	/// Коды исключения
	enum EXCEPTION {
		EXCEPTION_01H_ILLEGAL_FUNCTION	= 0x01,	///< Код функции не поддерживается
		EXCEPTION_02H_ILLEGAL_DATA_ADR	= 0x02,	///< Неверный адрес регистра
		EXCEPTION_03H_ILLEGAL_DATA_VAL	= 0x03,	///< Неверное значение поля данных
		EXCEPTION_04H_DEVICE_FAILURE	= 0x04,	///< В устройстве произошла ошибка
		EXCEPTION_10H_TEMP_INAC_PARAM	= 0x10,	///< Запись по адресу временно невозможна
		EXCEPTION_11H_UNCHANG_PARAM		= 0x11	///< Запись по адресу невозможна
	};
    
    /// Коды ошибок проверки принятого сообщения
    enum CHECK_ERR {
        CHECK_ERR_NO = 0,       ///< Ошибок нет                       
        CHECK_ERR_ADR_DEVICE,   ///< Ошибка проверки адреса устройства
        CHECK_ERR_CRC,          ///< Ошибка контрольной суммы
        CHECK_ERR_FUNCTION,     ///< Ошибка проверки кода команды
        CHECK_ERR_FUNCTION_DATA	///< Ошибка проверки данных команды
    };

	/**	Контструктор.
	 *
	 * 	@param buf Указатель на буфер данных.
	 * 	@param size Размер буфера данных.
	 */
	TProtocolModbus(uint8_t *buf, uint8_t size);

	/**	Запуск работы данного протокола.
	 *
	 *	Протокол будет включен в состоянии "Чтение".
	 *	В случае ошибочного значения, протокол останется в текущем состоянии.
	 *
	 *	@see STATE
	 * 	@param state Начальное состояние протокола.
	 */
	void setEnable() {
		setState(STATE_READ);
	}

	/**	Остановка работы данного протокола.
	 *
	 */
	void setDisable() {
		state_ = STATE_OFF;
	}

	/** Проверка текущего состояния работы данного протокола
	 *
	 *	@retval True - если текущее состояние протокола не STATE_OFF.
	 *	@retval False - если протокол отключен, т.е. состояние STATE_OFF.
	 */
	bool isEnable() {
		return (state_ != STATE_OFF);
	}

	/**	Возвращает текущее состояние работы протокола.
	 *
	 * 	@return Текущее состояние протокола.
	 */
	TProtocolModbus::STATE getState() const {
		return state_;
	}

	/**	Смента состояния работы протокола.
	 *
	 *	В случае ошибочного значения, протокол останется в текущем состоянии.
	 *
	 * 	@param state Новое состояние работы протокола.
	 */
	void setState(TProtocolModbus::STATE state) {
		if ((state >= STATE_OFF) && (state < STATE_ERROR)) {
			if (state == STATE_READ) {
				// при смене режима работы на "Чтение", обнулим счетчик принятых
				// байт
				cnt_ = 0;
				tick_ = 0;
			}
			state_ = state;
		}
	}

	/**	Проверка совпадения нужного состояния с текущим.
	 *
	 * 	@retval True - если протокол в нужном состоянии.
	 * 	@retval False - если состояние протокола отличается от нужного.
	 */
	bool checkState(TProtocolModbus::STATE state) {
		return (state_ == state);
	}

	/**	Проверка наличия принятой посылки.
	 *
	 * 	@retval True - при наличии принятой посылки.
	 * 	@retval False - при отсутствии принятой посылки
	 */
	bool isReadData() {
		return (state_ == STATE_READ_OK);
	}

	/**	Принятый байт даннх.
	 *
	 *	При переполнении буфера посылка будет просто проигнорирована.
	 *	Производится сброс счетчика времени, прошедшего с момента прихода
	 *	последнего байта.
	 *
	 *	@param byte Байт данных.
	 *	@return Возвращает кол-во принятых данных в буфере.
	 */
	uint8_t push(uint8_t byte) {
		uint8_t cnt = cnt_;
		if (state_ == STATE_READ) {
			// сброс счетчика принятых байт, если обнаружено начало новой посылки
			if (tick_ >= DELAY_RESET) {
				cnt = 0;
				setState(STATE_READ_ERROR);
			} else {
				if (cnt < size_) {
					buf_[cnt++] = byte;
				}
			}

			// сброс счетчика времени
			tick_ = 0;
		}
		return (cnt_ = cnt);
	}

	/**	Возвращает стартовый адрес регистра/флага в посылке.
	 *
	 * 	@return Стартовый адрес регистра или флага в принятой посылке.
	 */
	uint16_t getStartAddress() const {
		return ((uint16_t) buf_[2] << 8) + buf_[3];
	}

	/**	Возвращает количество регистров/флагов в посылке.
	 *
	 * 	@return Количество регистров/флагов в посылке.
	 */
	uint16_t getNumOfAddress() const {
		return ((uint16_t) buf_[4] << 8) + buf_[5];
	}

	/**	Проверка полученной посылки на соответствие протоколу.
	 *
	 * 	Проверяются:
	 * 	- адрес устройства;
	 * 	- контрольная сумма;
	 * 	- поддерживаемая команда;
	 * 	- корректность данных в принятой команде.
	 *
	 * 	Не проверяются:
	 * 	- корректность адресов регистров и флагов.
	 *
	 * 	@return Статус проверки.
	 * 	@see CHECK_ERR
	 */
	TProtocolModbus::CHECK_ERR checkReadPackage();

	/**	Считывание полученных данных для записи в регистр.
	 *
	 * 	Для того чтобы забрать полученные по протоколу новые значения регистра,
	 * 	необходимо указать его адрес.
	 *
	 * 	@param num Адрес регистра.
	 * 	@return Значение регистра.
	 * 	@retval 0xFFFF - Значение по-умолчанию.
	 */
	uint16_t getRegister(uint16_t adr);

	/**	Считывание полученных данных для записи флага.
	 *
	 * 	Для того чтобы забрать полученные по протоколу нове значения флага,
	 * 	необходимо указать его адрес.
	 *
	 * 	Возвращается байт данных, младший бит соответствует младшему адресу.
	 * 	Где бит равный 1 означает, что надо установить флаг, а 0 - сбросить.
	 *
	 * 	@param num Адрес флага.
	 * 	@return Значения флага.
	 * 	@retval 0 - Сбросить флаг.
	 * 	@retval 1 - Установить флаг.
	 * 	@retval 0xFF - Ошибочный адрес.
	 */
	uint8_t getCoil(uint16_t adr);

	/**	Запись данных для передачи.
	 *
	 * 	Для передачи значения по протоколу, необходимо указать его адрес.
	 *
	 * 	@param num Номер регистра.
	 * 	@param val Значение регистра.
	 */
	bool writeData(uint16_t num, uint16_t val);

	/**	Отправка сообщения.
	 *
	 *	В случае, если состояние отличается от состояния передачи. Количество
	 *	байт данных на передачу будет равно 0.
	 *
	 * 	@return Кол-во байт данных для передачи
	 */
	uint8_t trCom();

	/**	Настройка счетчика времени.
	 *
	 * 	Исходя из полученной скорости работы протокола, наличия контроля
	 * 	четности и кол-ва стоп-битов, а так же частоты вызова функции tick(),
	 * 	рассчитывается шаг счета. При этом учитывается что кол-во "бит данных"
	 * 	всегда 8, а кол-во бит "контроль четности" + "стоп-биты" равно 2.
	 *
	 * 	Например:
	 * 	для скорости  19200 и частоте вызова функции 1 раз в 50мкс, получим:
	 * 	step = (period / 10^6) * (DELAY_RESET / 1.5) * (baudrate / 11) =
	 * 	= (50 / 10^6) * (15000/1.5) * (19200 / 11) = 872
	 *
	 * 	Для частот выше 19200 рекомендуются значения: 750мкс для полуторного
	 * 	интервала, и 1750 для 3.5 интервала. Поэтому шаг счета для вызова раз в
	 * 	50 мкс будет::
	 * 	step = DELAY_RESET * period / 750 = 15000/750 * period = 20 * 50 = 1000
	 *
	 *	@param baudrate Скорость работы последовательного порта, бит/с.
	 *	@param period Частота вызова функции tick(), мкс.
	 *	@return Рассчитанный шаг счетчика.
	 */
	uint16_t setTick(uint16_t baudrate, uint8_t period);

	/**	Счет времени прошедшего с момента прихода последнего байта.
	 *	
	 *	Счетчик считает только в состояниях \a STATE_READ и \a STATE_READ_ERROR.
	 *	
	 *	Как только счетчик времени достигнет (превысит) \a DELAY_READ, т.е.
	 *	будет обнаружена "3.5 пауза":
	 *	- в состоянии \a STATE_READ, формируется состояние принятой посылки
	 *	\a STATE_READ_OK;
	 *	- в состоянии \a STATE_READ_ERROR, формируется состояние приема посылки
	 *	\a STATE_READ (т.е. ожидание новой посылки).
	 *
	 * 	Вызывается с заданным интервалом.  Например, из прерывания.
	 *
	 *	@see DELAY_READ
	 *	@see STATE_READ
	 *	@see STATE_READ_ERROR
	 */
	void tick() {
		uint16_t tick = tick_;
		TProtocolModbus::STATE state = state_;
		
		if ((state == STATE_READ) || (state == STATE_READ_ERROR)) {
			if (tick < DELAY_READ) {
				tick += tickStep_;

				if (tick >= DELAY_READ) {
					if (state == STATE_READ) {
						setState(STATE_READ_OK);
					} else {
						setState(STATE_READ);
					}
				}
			}
			tick_ = tick;
		}
	}

	/**	Установка адреса устройства в сети.
	 *
	 * 	@param adr Адрес устройства.
	 * 	@retval True - в случае смены адреса устройства.
	 * 	@retval False - в случае ошибочного адреса.
	 */
	bool setAddress(uint8_t adr);

	/**	Возвращает адрес устройства в сети.
	 *
	 * 	@return Адрес устройства.
	 */
	uint8_t getAddress() const;

	/**	Ответ на запрос с кодом исключения.
	 *
	 *	@param code Код исключения.
	 */
	void setException(TProtocolModbus::EXCEPTION code);
	
private:
	const uint8_t size_;	///> Размер буфера данных
	uint8_t * const buf_;	///> Буфер принятых/передаваемых данных

	static const uint8_t CRC_HI[256];	///> Массив значений для подсчета CRC
	static const uint8_t CRC_LOW[256];	///> Массив значений для подсчета CRC

	STATE state_;			///> Текущее состояние работы протокола.

	uint8_t address_;		///> Адрес устройства в сети.
	
	VOLATILE uint8_t cnt_;	///> Кол-во принятых байт.

	VOLATILE uint16_t tick_;///> Время прошедшее с момента приема последнего байта.
	uint16_t tickStep_;		///> Шаг счетчика паузы.

	// Добавляет два байта к посылке приготовленной на передачу.
	void addData(uint16_t val);

	// Проверка соответствия принятой команды поддерживаемым в этом классе.
	TProtocolModbus::CHECK_ERR checkCommand(uint8_t com);

	// Проверка адреса устройства на совпадение с установленным.
	bool checkAddress(uint8_t adr);

	// Подсчет CRC для заданного кол-ва байт данных в буфере.
	uint16_t calcCRC(uint8_t num);
	
	// Возвращает принятый в посылке CRC.
	uint16_t getCRC() const;

	// Добавляет к имеющейся в буфере посылке контрольную сумму.
	void addCRC();
};


#endif /* __PROTOCOL_MODBUS_H_ */
