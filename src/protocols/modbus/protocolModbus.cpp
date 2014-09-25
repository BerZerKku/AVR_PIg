//#include "protocolModbus.h"
#include "protocolModbus.h"
#ifdef DEBUG
#include "debug.h"
#endif

const uint8_t TProtocolModbus::CRC_HI[256] = { 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
		0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

const uint8_t TProtocolModbus::CRC_LOW[256] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3,
		0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC,
		0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9,
		0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E,
		0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7,
		0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0,
		0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5,
		0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA,
		0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB,
		0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4,
		0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1,
		0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66,
		0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF,
		0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78,
		0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D,
		0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72,
		0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93,
		0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C,
		0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99,
		0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E,
		0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87,
		0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };

// Конструктор
TProtocolModbus::TProtocolModbus(uint8_t *buf, uint8_t size) :
				buf_(buf), size_(size) {

	state_ = STATE_OFF;

	address_ = ADDRESS_ERR;

	startAdr_ = 0;
	numOfAdr_ = 0;
	cnt_ = 0;

	tick_ = 0;
	tickStep_ = 0;
}

// принятый байт данных
uint8_t TProtocolModbus::push(uint8_t byte) {
	uint8_t cnt = cnt_;

	if (checkState(STATE_READ)) {
		// сброс счетчика принятых байт, если обнаружено начало новой посылки
		if ((cnt != 0) && (tick_ >= DELAY_RESET)) {
			setState(STATE_READ_ERROR);
		} else {
			if (cnt < size_) {
				buf_[cnt++] = byte;
			}
		}
	}
	tick_ = 0;	// сброс счетчика времени

	return (cnt_ = cnt);
}

// Настройка счетчика времени
uint16_t TProtocolModbus::setTick(uint16_t baudrate, uint8_t period) {
	uint16_t step = 0;

	if (baudrate > 19200) {
		step = (1UL * DELAY_RESET * period) / 750;
	} else {
		step = (((1UL * baudrate * period) / 1100) * DELAY_RESET) / 15000;
	}

	return (tickStep_ = step);
}

// Счет времени прошедшего с момента прихода последнего байта.
void TProtocolModbus::tick() {
	uint16_t tick = tick_;

	if (checkState(STATE_READ) || checkState(STATE_READ_ERROR)) {
		if (tick < DELAY_READ) {
			tick += tickStep_;
			if (tick >= DELAY_READ) {
				if (checkState(STATE_READ) && (cnt_ >= 4)) {
						setState(STATE_READ_OK);
				} else {
					setState(STATE_READ);
				}
			}
		}
		tick_ = tick;
	}
}

// Возвращает адрес устройства в сети.
uint8_t TProtocolModbus::getAddressLan() const {
	return address_;
}

// Установка адреса устройства в сети.
bool TProtocolModbus::setAddressLan(uint8_t adr) {
	bool state = false;

	if ((adr >= ADDRESS_MIN) && (adr <= ADDRESS_MAX)) {
		address_ = adr;
		state = true;
	}

	return state;
}

// Отправка сообщения.
uint8_t TProtocolModbus::trResponse() {
	uint8_t cnt = 0;

	if (checkState(STATE_WRITE_READY)) {
		addCRC();
		setState(STATE_WRITE);
		cnt = cnt_;
	}

	return cnt;
}

// Обработка принятых данных
bool TProtocolModbus::readData() {
	bool state = false;

	if (checkReadPackage() == CHECK_ERR_NO) {
		TProtocolModbus::COM com = static_cast<TProtocolModbus::COM> (buf_[1]);

		switch(com) {
			case COM_01H_READ_COIL:
				state = comReadCoils();
				break;
			case COM_03H_READ_HOLDING_REGISTER:
				state = comReadRegisters();
				break;
			case COM_05H_WRITE_SINGLE_COIL :
				state = writeCoilsCom();
				prepareResponse();
				break;
			case COM_06H_WRITE_SINGLE_REGISTER:
				state = writeCoilsCom();
				prepareResponse();
				break;
			case COM_0FH_WRITE_MULTIPLIE_COILS:
				// TODO Обработка записи группы флагов по MODBUS
				prepareResponse();
				break;
			case COM_10H_WRITE_MULITPLIE_REGISTERS:
				// TODO Обработка записи группы регистров по MODBUS
				prepareResponse();
				break;
			case COM_11H_SLAVE_ID:
				state = comReadID();
				break;
			default:
				// данная команда не поддерживается
				setException(EXCEPTION_01H_ILLEGAL_FUNCTION);
		}
	}

//	if (!state) {
//		// формирование исключения в случае ошибочного адреса
//		setException(EXCEPTION_02H_ILLEGAL_DATA_ADR);
//	} else if (!checkState(STATE_WRITE_READY)) {
//			// отправка подготовленного сообщения
//			setState(STATE_WRITE_READY);
//		}
//	}


	if (state) {
		setState(STATE_WRITE_READY);
	}

	return state;
}

//	Считывание полученных данных для записи флага.
//uint8_t TProtocolModbus::getCoil(uint16_t adr) {
//	uint8_t data = 0xFF;
//	uint16_t startadr = getStartAddress();
//	uint16_t numofadr = getNumOfAddress();
//	TProtocolModbus::COM com = static_cast<TProtocolModbus::COM>(buf_[1]);
//
//	if (com == COM_05H_WRITE_SINGLE_COIL) {
//		if (adr == startadr) {
//			data = (buf_[4] == 0xFF) ? 1 : 0;
//		}
//	} else if (com == COM_0FH_WRITE_MULTIPLIE_COILS) {
//		if ((adr >= startadr) && (adr < (startadr + numofadr))) {
//			uint8_t num = adr - startadr;
//			data = (buf_[num / 8 + 7] & (1 << (num % 8))) ? 1 : 0;
//		}
//	}
//
//	return data;
//}

//	Считывание полученных данных для записи в регистр.
//uint16_t TProtocolModbus::getRegister(uint16_t adr) {
//	uint16_t data = 0xFFFF;
//	uint16_t startadr = getStartAddress();
//	uint16_t numofadr = getNumOfAddress();
//
//	TProtocolModbus::COM com = static_cast<TProtocolModbus::COM>(buf_[1]);
//
//	if (com == COM_06H_WRITE_SINGLE_REGISTER) {
//		if (adr == startadr) {
//			data = ((uint16_t) buf_[4] << 8) + buf_[5];
//		}
//	} else if (com == COM_10H_WRITE_MULITPLIE_REGISTERS) {
//		if ((adr >= startadr) && (adr < (startadr + numofadr))) {
//			uint8_t num = (adr - startadr) * 2;
//			data = ((uint16_t) buf_[num + 7] << 8) + buf_[num + 8];
//		}
//	}
//
//	return data;
//}

// запись флага для передачи
//bool TProtocolModbus::sendCoil(uint16_t adr, bool val) {
//	bool state = false;
//
//	if ((adr >= startAdr_) && (adr < (startAdr_ + numOfAdr_))) {
//		uint8_t a = (adr - startAdr_) / 8;
//		uint8_t n = (adr - startAdr_) % 8;
//		if (val) {
//			buf_[a + 3] |= (1 << n);
//		} else {
//			buf_[a + 3] &= ~(1 << n);
//		}
//		state = true;
//	}
//	return state;
//}

// запись группы флагов на передачу
//bool TProtocolModbus::sendCoils(uint16_t adr, uint8_t val) {
//	bool state = false;
//
//	for(uint8_t i = 1; i > 0; i <<= 1, adr++) {
//		state |= sendCoil(adr, (val&i));
//	}
//
//	return state;
//}

// Запись регистра для передачи.
//bool TProtocolModbus::sendRegister(uint16_t adr, uint16_t val) {
//	bool state = false;
//
//	if ((adr >= startAdr_) && (adr < (startAdr_ + numOfAdr_))) {
//		buf_[(adr - startAdr_)*2 + 3] = val >> 8;
//		buf_[(adr - startAdr_)*2 + 4] = val;
//		state = true;
//	}
//
//	return state;
//}

/**	Проверка соответствия принятой команды поддерживаемым в этом классе.
 *
 * 	Проверяются:
 * 	- код команды;
 * 	- данные команды;
 *
 * 	Так же для команд чтения/запись определяются начальный адрес и количество
 * 	необходимых адресов.
 *
 *	@retval CHECK_ERR_NO - ошибок нет.
 *	@retval CHECK_ERR_FUNCTION - данная команда не поддерживается.
 *	@retval CHECK_ERR_FUNCTION_DATA - ошибка данных в команде.
 */
TProtocolModbus::CHECK_ERR TProtocolModbus::checkCommand(uint8_t com) {
	TProtocolModbus::CHECK_ERR state = CHECK_ERR_FUNCTION;
	uint16_t adr = ((uint16_t) buf_[2] << 8) + buf_[3];	// начальный адрес
	uint16_t num = ((uint16_t) buf_[4] << 8) + buf_[5];	// кол-во адресов

	switch (static_cast<TProtocolModbus::COM>(com)) {
//		case COM_01H_READ_COIL: {
//			// ограничение на минимум/максимум считывемых флагов
//			if ((num >= 1) && (num <= MAX_NUM_COILS)) {
//				startAdr_ = adr + 1;
//				numOfAdr_ = num;
//				state = CHECK_ERR_NO;
//			} else
//				state = CHECK_ERR_FUNCTION_DATA;
//		}
//		break;
//		case COM_03H_READ_HOLDING_REGISTER: {
//			// ограничение на минимум/максимум считываемых регистров
//			if ((num >= 1) && (num <= MAX_NUM_REGISTERS)) {
//				startAdr_ = adr + 1;
//				numOfAdr_ = num;
//				state = CHECK_ERR_NO;
//			} else
//				state = CHECK_ERR_FUNCTION_DATA;
//		}
//		break;
		case COM_05H_WRITE_SINGLE_COIL: {
			// допускаются значения флага 0x0000 и 0xFF00 (=1)
			if ((buf_[5] == 0x00) && ((buf_[4] == 0x00) || (buf_[4] == 0xFF))) {
				startAdr_ = adr + 1;
				numOfAdr_ = 1;
				state = CHECK_ERR_NO;
			} else
				state = CHECK_ERR_FUNCTION_DATA;
		}
		break;
		case COM_06H_WRITE_SINGLE_REGISTER: {
			// допустимое значение адреса для записи от 0х0000 до 0хFFFF
			startAdr_ = adr + 1;
			numOfAdr_ = 1;
			state = CHECK_ERR_NO;
		}
		break;
		case COM_0FH_WRITE_MULTIPLIE_COILS: {
			// ограничение на минимум/максимум устанавливаемых флагов
			// а также сравнение с заявленным кол-вом байт данных в посылке
			if ((num >= 1) && (num <= MAX_NUM_COILS)) {
				if (((num + 7) / 8) != buf_[6]) {
					state = CHECK_ERR_FUNCTION_DATA;
				} else {
					startAdr_ = adr + 1;
					numOfAdr_ = num;
					state = CHECK_ERR_NO;
				}
			} else {
				state = CHECK_ERR_FUNCTION_DATA;
			}
		}
		break;
		case COM_10H_WRITE_MULITPLIE_REGISTERS: {
			// ограничение на минимум/максимум записываемых регистров
			// а также сравнение с заявленным кол-вом байт данных в посылке
			if ((num >= 1) && (num <= MAX_NUM_REGISTERS)) {
				if ((num * 2) != buf_[6]) {
					state = CHECK_ERR_FUNCTION_DATA;
				} else {
					startAdr_ = adr + 1;
					numOfAdr_ = num;
					state = CHECK_ERR_NO;
				}
			} else
				state = CHECK_ERR_FUNCTION_DATA;
		}
		break;
//		case COM_11H_SLAVE_ID:
//			// нет ограничений
//			startAdr_ = 0;
//			numOfAdr_ = 0;
//			state = CHECK_ERR_NO;
//			break;
	}
	return state;
}

/**	Проверка адреса устройства на совпадение с установленным
 *
 *	Дополнительно проводится проверка на ошибочный адрес \a ADDRESS_ERR.
 *
 *	@see ADDRESS_ERR
 * 	@adr Адрес, который будет сравниваться с установленным.
 */
bool TProtocolModbus::checkAddressLan(uint8_t adr) {
	return ((adr != ADDRESS_ERR) && (address_ == adr));
}

/**	Рассчет контрольной суммы для заданного кол-ва байт данных в буфере
 *
 * 	@param num Кол-во байт данных.
 * 	@return Рассчитанная контрольная сумма.
 */
uint16_t TProtocolModbus::calcCRC(uint8_t num) {
	uint8_t low = 0xFF;
	uint8_t hi = 0xFF;
	for (uint8_t i = 0, tmp; i < num; i++) {
		tmp = hi ^ buf_[i];
		hi = low ^ CRC_HI[tmp];
		low = CRC_LOW[tmp];
	}
	return ((hi << 8) | low);
}

/**	Возвращает контрольную сумму переданную в посылке.
 *
 * 	@return Контрольная сумма посылки.
 */
uint16_t TProtocolModbus::getCRC() const {
	uint16_t crc = 0xFFFF;
	if (cnt_ >= 3) {
		crc = (((uint16_t) buf_[cnt_ - 2] << 8) + buf_[cnt_ - 1]);
	}
	return crc;
}

/**	Добавляет к имеющейся в буфере послылке контрольную сумму.
 *
 * 	Рассчитывается контрольная сумма для имеющегося количества данных в буфере.
 *	При этом увеличиваетс счетчик байт в посылке.
 */
void TProtocolModbus::addCRC() {
	uint16_t crc = calcCRC(cnt_);
	buf_[cnt_++] = crc >> 8;
	buf_[cnt_++] = crc;
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
 *	В случае обнаружения ошибки подготавливется ответ с кодом исключения.
 *	Иначе в зависимости от кода команды подготавливается подготавливается
 *	основа для ответа.
 *
 *	Так же для команд чтения/записи определяется стартовый адрес и
 *	количество необходимых адресов.
 *
 *	В ходе проверки статус работы протокола может измениться и это надо
 *	проверить в основной программе.
 *
 * 	@return Статус проверки.
 * 	@see CHECK_ERR
 */
TProtocolModbus::CHECK_ERR TProtocolModbus::checkReadPackage() {
	CHECK_ERR state = CHECK_ERR_NO;

	if (!checkAddressLan(buf_[0])) {
		// Не совпал адрес устройства.
		setState(STATE_READ);
		state = CHECK_ERR_ADR_DEVICE;
	} else if (getCRC() != calcCRC(cnt_ - 2)) {
		// не совпала контрольная сумма
		setState(STATE_READ);
		state = CHECK_ERR_CRC;
	}
//	} else {
//		state = checkCommand(buf_[1]);
//		if (state == CHECK_ERR_FUNCTION) {
//			// ошибочный код команды
//			setException(EXCEPTION_01H_ILLEGAL_FUNCTION);
//			state = CHECK_ERR_FUNCTION;
//		} else if (state == CHECK_ERR_FUNCTION_DATA) {
//			// ошибочные данные в команде
//			setException(EXCEPTION_03H_ILLEGAL_DATA_VAL);
//			state = CHECK_ERR_FUNCTION_DATA;
//		}
//	}

	return state;
}

/**	Ответ на запрос с кодом исключения.
 *
 *	К коду принятой команды добавляется 0х80.
 *	Добавляется один байт данных - код исключения.
 *	Подсчитывается контрольная сумма.
 *	Флаг состояния протокола устанавливаетс \a STATE_WRITE, т.е. готовность
 *	к отправке данных.
 *
 *	@param code Код исключения.
 */
void TProtocolModbus::setException(TProtocolModbus::EXCEPTION code) {
	cnt_ = 1;	// адрес остается прежним
	buf_[cnt_++] |= 0x80;
	buf_[cnt_++]  = code;
	setState(STATE_WRITE_READY);
}

/**	Подготовка посылки для ответа на запрос.
 *
 * 	Для команды полученной в запросе формируется заготовка ответа.
 * 	Все необходимые для передачи регистры/флаги записываются нулями.
 *
 * 	@return Кол-во подготовленных байт данных для передачи.
 */
uint8_t TProtocolModbus::prepareResponse() {
	uint8_t cnt = 0;

	switch (static_cast<TProtocolModbus::COM>(buf_[1])) {
//		case COM_01H_READ_COIL: {
//			cnt = 2;
//			buf_[cnt++] = (numOfAdr_ + 7) / 8;
//			for(uint8_t i = 0; i < buf_[2]; i++) {
//				buf_[cnt++] = 0;
//			}
//		}
//		break;
//		case COM_03H_READ_HOLDING_REGISTER: {
//			cnt = 2;
//			buf_[cnt++] = numOfAdr_ * 2;
//			for(uint8_t i = 0; i < buf_[2]; i++) {
//				buf_[cnt++] = 0x00;
//			}
//		}
//		break;
		case COM_05H_WRITE_SINGLE_COIL: {
			// команда возвращается без изменения
			cnt = cnt_ - 2;
		}
		break;
		case COM_06H_WRITE_SINGLE_REGISTER: {
			// команда возвращается без изменения
			cnt = cnt_ - 2;
		}
		break;
		case COM_0FH_WRITE_MULTIPLIE_COILS: {
			// возвращается адрес первого флага и их количество
			cnt = 6;
		}
		break;
		case COM_10H_WRITE_MULITPLIE_REGISTERS: {
			// возвращается адрес первого регистра и их количество
			cnt = 6;
		}
		break;
//		case COM_11H_SLAVE_ID: {
//			cnt = 2;	// адрес и команда
//		}
//		break;
	}
	return (cnt_ = cnt);
}

// 	Смена состояния работы протокола.
void TProtocolModbus::setState(TProtocolModbus::STATE state) {
	if ((state >= STATE_OFF) && (state < STATE_ERROR)) {
		if (state == STATE_READ) {
			// при смене режима работы на "Чтение", обнулим счетчик принятых
			// байт
			cnt_ = 0;
			tick_ = 0;
			startAdr_ = 0;
			numOfAdr_ = 0;
		}
		state_ = state;
	}
}

/** Обработка команды чтения флагов 0x01.
 *
 * 	В случае обнаружения ошибок в принятом пакете, будет сформировано
 *	исключение.
 *
 *	Если ошибок в принятой посылке нет, будет сформирован ответ без КС.
 *
 *	@retval True - если посылка обработана без ошибок.
 *	@retval False - если при обработке посылки возникли ошбки.
 */
bool TProtocolModbus::comReadCoils() {
	uint16_t adr = ((uint16_t) buf_[2] << 8) + buf_[3] + 1;	// начальный адрес
	uint16_t num = ((uint16_t) buf_[4] << 8) + buf_[5];		// кол-во адресов

	// проверка количества регистров
	if ((num == 0) || (num > MAX_NUM_COILS)) {
		setException(EXCEPTION_03H_ILLEGAL_DATA_VAL);
		return false;
	}

	startAdr_ = adr;
	numOfAdr_ = num;

	// подготовка ответа
	uint8_t cnt = 2;
	buf_[cnt++] = (numOfAdr_ + 7) / 8;		// кол-во передаваемыб байт данных
	for(uint8_t i = 0; i < buf_[2]; i++) {	// обнуление передаваемых данных
		buf_[cnt++] = 0;
	}
	cnt_ = cnt;

	// заполнение ответа теущими сотстояниями флагов
	// если встретится хотя бы один недопустимый адрес, будет сформировано
	// исключение
	for(uint16_t i = 0; i < num; i++, adr++) {
		bool val = false;

		if (!readCoilCom(adr, val)) {
			// ошибочный адрес
			setException(EXCEPTION_02H_ILLEGAL_DATA_ADR);
			return false;
		}

		uint8_t a = (adr - startAdr_) / 8;
		uint8_t n = (adr - startAdr_) % 8;

		// При необходимости устанавливаем текущий флаг. Обнулять не надо
		// т.к. в ответ изначально записаны 0х00.
		if (val) {
			buf_[3 + a] |= (1 << n);
		}

		// внутренняя ошибка, в даннй реализации не формируется
		// setException(EXCEPTION_04H_DEVICE_FAILURE);
	}

	return true;
}

/** Обработка команды чтения регистров 0x03.
 *
 * 	В случае обнаружения ошибок в принятом пакете, будет сформировано
 *	исключение.
 *
 *	Если ошибок в принятой посылке нет, будет сформирован ответ без КС.
 *
 *	@retval True - если посылка обработана без ошибок.
 *	@retval False - если при обработке посылки возникли ошбки.
 */
bool TProtocolModbus::comReadRegisters() {
	uint16_t adr = ((uint16_t) buf_[2] << 8) + buf_[3] + 1;	// начальный адрес
	uint16_t num = ((uint16_t) buf_[4] << 8) + buf_[5];		// кол-во адресов

	// проверка количества регистров
	if ((num == 0) || (num > MAX_NUM_REGISTERS)) {
		setException(EXCEPTION_03H_ILLEGAL_DATA_VAL);
		return false;
	}

	startAdr_ = adr;
	numOfAdr_ = num;

	// подготовка ответа, изначально все данные заполнены нулями
	uint8_t cnt = 2;
	buf_[cnt++] = numOfAdr_ * 2;
	for(uint8_t i = 0; i < buf_[2]; i++) {
		buf_[cnt++] = 0x00;
	}
	cnt_ = cnt;

	// заполнение ответа теущими значениями регистров
	// если встретится хотя бы один недопустимый адрес, будет сформировано
	// исключение
	for(uint16_t i = 0; i < num; i++, adr++) {
		uint16_t val = 0xFFFF;

		if (!readRegisterCom(adr, val)) {
			// ошибочный адрес
			setException(EXCEPTION_02H_ILLEGAL_DATA_ADR);
			return false;
		}

		buf_[(adr - startAdr_)*2 + 3] = val >> 8;
		buf_[(adr - startAdr_)*2 + 4] = val;

		// внутренняя ошибка, в даннй реализации не формируется
		// setException(EXCEPTION_04H_DEVICE_FAILURE);
	}

	return true;
}

/** Обработка команды чтения инфорации об устройстве 0x11.
 *
 * 	В случае обнаружения ошибок в принятом пакете, будет сформировано
 *	исключение.
 *
 *	Если ошибок в принятой посылке нет, будет сформирован ответ без КС.
 *
 *	@retval True - если посылка обработана без ошибок.
 *	@retval False - если при обработке посылки возникли ошбки.
 */
bool TProtocolModbus::comReadID() {

	startAdr_ = 0;
	numOfAdr_ = 0;

	uint8_t num = MAX_ID_CHARS; // максимальное кол-во байт данных на передачу

	if (!readIdCom(&buf_[2], num)) {
		setException(EXCEPTION_04H_DEVICE_FAILURE);
		return false;
	}
	cnt_ = 2 + num;

	return true;
}

// Обработка принятой команды чтения флагов.
bool TProtocolModbus::readCoilCom(uint16_t adr, bool &val) {

	// диапазон допустимых адресов
	if ((adr == 0) || (adr > 300))
		return false;

	// считывание текущего состояния флага
	if (adr <= 100) {
		val = true;
	} else if (adr <= 200) {
		val = false;
	} else if (adr <= 300) {
		val = true;
	}

	return true;
}

// Обработка принятой команды чтения регистров.
bool TProtocolModbus::readRegisterCom(uint16_t adr, uint16_t &val) {

	if ((adr == 0) || (adr > 300))
		return false;

	if (adr <= 100) {
		val = adr;
	} else if (adr <= 200) {
		val = 0xFFFF;
	} else if (adr <= 300) {
		val = adr;
	}

	return true;
}

///	Обработка принятой команды чтения ID.
bool TProtocolModbus::readIdCom(uint8_t *buf, uint8_t &size) {

//	static const char ID[] PROGMEM = "Virtual";

//	uint8_t cnt = 0;
//	for(; cnt < (sizeof(ID) / sizeof(ID[0])); cnt++) {
//		if (cnt >= size)
//			break;
//		buf[cnt] = pgm_read_byte(&ID[cnt]);
//	}
//
//	// последний байт данных - индикатор состояния.
//	buf[cnt++] = 0x00;
//
//	// количество передаваемых данных
//	size = cnt;
	size = 8;

	return true;
}

/// Обработка команды записи одного флага.
bool TProtocolModbus::writeCoilsCom() {
	uint16_t adr = getStartAddress();
	uint16_t num = getNumOfAddress();

	if ((adr == 0) || ((adr + num)  > 300)) {
		return false;
	}

	for(uint8_t i = 0; i < num; i++, adr++) {
//		if (getCoil(adr) == 0xFF)
			return false;
	}
	return true;
}
