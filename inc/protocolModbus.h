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
	/// Пауза между принятыми байтами, при которой идет сброс на начало приема.
	static const uint16_t DELAY_RESET = 1500;
	/// Пауза между принятыми байтами, при которой определяется окончание приема.
	static const uint16_t DELAY_READ  = 3500;

public:
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
		COM_0EH_READ_DEVICE_INFORMATION		= 0x0E,	///< Чтение информации об устройстве.
		COM_0FH_WRITE_MULTIPLIE_COILS		= 0x0F,	///< Запись группы флагов.
		COM_10H_WRITE_MULITPLIE_REGISTERS	= 0x10,	///< Запись группы внутренних регистров.
		COM_2BH_READ_DEVICE_INDENTIFICATION = 0x2B	///< Чтение информации об устройстве.
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

	/**	Принятый байт даннх.
	 *
	 *	При переполнении буфера, будет сгенерирована ошибка.
	 *	Производится сброс счетчика времени, прошедшего с момента прихода
	 *	последнего байта.
	 *
	 *	@param byte Байт данных.
	 */
	void push(uint8_t byte) {

		if (state_ == STATE_READ) {
			// сброс счетчика принятых байт, если обнаружено начало новой посылки
			if (tick_ >= DELAY_RESET) {
				cnt_ = 0;
			}

			if (cnt_ < size_) {
				buf_[cnt_++] = byte;
			} else {
				// TODO MODBUS Сделать обработку ошибки при переполнении
			}

			// сброс счетчика времени
			tick_ = 0;
		}
	}

	/**	Проверка наличия принятой посылки.
	 *
	 * 	@retval True - при наличии принятой посылки.
	 * 	@retval False - при отсутствии принятой посылки
	 */
	bool isReadData() {
		return (state_ == STATE_READ_OK);
	}

	/**	Отправка сообщения.
	 *
	 *	В случае, если состояние отличается от состояния передачи. Количество
	 *	байт данных на передачу будет равно 0.
	 *
	 * 	@return Кол-во байт данных для передачи
	 */
	uint8_t trCom();

	/**	Возвращает номер младшего регистра для чтения/записи.
	 *
	 * 	Если 0, то ничего не надо.
	 *
	 * 	@return Номер младшего регистра.
	 */
	uint16_t getMinNumRegisters() const {
		return minReg_;
	}

	/**	Возвращает номер старшего регистра для чтения/записи.
	 *
	 * 	Если 0, то ничего не надо.
	 *
	 * 	@return Номер младшего регистра.
	 */
	uint16_t getMaxNumRegisters() const {
		return maxReg_;
	}

	/**	Проверка полученной посылки на корректность данных.
	 *
	 * 	?!
	 *
	 * 	@retval True - если ошибок в полученной посылке нет.
	 * 	@retval False - если есть ошибки в полученной посылке.
	 */
	bool checkReadPackage();

	/**	Считывание полученных данных.
	 *
	 * 	Для того чтобы забрать полученные по протоколу новые значения регистра,
	 * 	необходимо указать его номер.
	 *
	 * 	@param num Номер регистра.
	 */
	uint16_t readData(uint16_t num);

	/**	Запись данных для передачи.
	 *
	 * 	Для передачи значения по протоколу, необходимо указать его адрес.
	 *
	 * 	@param num Номер регистра.
	 * 	@param val Значение регистра.
	 */
	bool writeData(uint16_t num, uint16_t val);

	/**	Счет времени прошедшего с момента прихода последнего байта.
	 *
	 * 	Как только счетчик времени достигнет (превысит) \a DELAY_READ,
	 * 	это послужит флагом принятия посылки.
	 *
	 * 	Вызывается с заданным интервалом. Например, из прерывания.
	 *
	 *	@see DELAY_READ
	 */
	void tick() {
		if (state_ == STATE_READ) {
			if (tick_ < DELAY_READ) {
				tick_ += tickStep_;

				if (tick_ >= DELAY_READ) {
					setState(STATE_READ_OK);
				}
			}
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



	// Подсчет CRC для заданного кол-ва байт данных в буфере.
	uint16_t calcCRC(uint8_t num);

	// Возвращает принятый в посылке CRC.
	uint16_t getCRC() const;

	// Добавляет к имеющейся в буфере посылке контрольную сумму.
	void addCRC();

protected:
	const uint8_t size_;	///> Размер буфера данных
	uint8_t * const buf_;	///> Буфер принятых/передаваемых данных

	STATE state_;			///> Текущее состояние работы протокола.

	uint8_t address_;		///> Адрес устройства в сети.

	uint16_t minReg_;		///> Номер младшего регистра для чтения/записи.
	uint16_t maxReg_;		///> Номер старшего регистра для чтения/записи.

	uint8_t cnt_;			///> Кол-во принятых байт.

	uint16_t tick_;			///> Время прошедшее с момента приема последнего байта.
	uint16_t tickStep_;		///> Шаг счетчика паузы.

	// Добавляет два байта к посылке приготовленной на передачу.
	void addData(uint16_t val);



	// Проверка соответствия принятой команды поддерживаемым в этом классе.
	bool checkCommand(uint8_t com);

	// Проверка адреса устройства на совпадение с установленным.
	bool checkAddress(uint8_t adr);

	// Проверка
};


#endif /* __PROTOCOL_MODBUS_H_ */
