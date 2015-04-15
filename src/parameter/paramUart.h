/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

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

#endif /* PARAMUART_H_ */
