/*
 * File:   testProtocolModbus.cpp
 * Author: Shcheblykin
 *
 * Created on Aug 22, 2014, 9:56:28 AM
 */

#include "testProtocolModbus.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testProtocolModbus);

testProtocolModbus::testProtocolModbus() {
}

testProtocolModbus::~testProtocolModbus() {
}

void testProtocolModbus::setUp() {
}

void testProtocolModbus::tearDown() {
}

/**	Проверка констант
 * 
 */
void testProtocolModbus::testConstant() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	if (tProtocolModbus.getMaxNumRegisters() != 32) {
		sprintf(msg, "1.1 Max number registers is not 32.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	if (tProtocolModbus.getMaxNumCoils() != 256) {
		sprintf(msg, "1.2 Max number coils is not 256.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	if (tProtocolModbus.getAddressError() != 0xFF) {
		sprintf(msg, "1.3 Wrong address is not 0xFF.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	if (tProtocolModbus.getAddressMin() != 1) {
		sprintf(msg, "1.4 Minimum address is not 1.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	if (tProtocolModbus.getAddressMax() != 247) {
		sprintf(msg, "1.5 Maximum address is not 247.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
}

/** Тестирование установки/чтения адреса устройства.
 * 
 */
void testProtocolModbus::testAddress() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	uint8_t adrError = tProtocolModbus.getAddressError();
	uint8_t adrMin = tProtocolModbus.getAddressMin();
	uint8_t adrMax = tProtocolModbus.getAddressMax();
	
	// 1. Проверка адреса по-умолчанию.
	if (tProtocolModbus.getAddress() != adrError) {
		sprintf(msg, "1.1 Address by default is not ADDRESS_ERR.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. Проверка установки/считывания адреса.
	for (uint16_t i = 0; i <= 255; i++) {

		bool t = tProtocolModbus.setAddress(i);

		// проверка возвращаемого значения функцией serAddress())
		if (((i < adrMin) || (i > adrMax)) == t) {
			sprintf(msg, "2.1 Function setAddress() returned error state on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		// проверка установленного значения
		if (i < adrMin) {
			if (tProtocolModbus.getAddress() != adrError) {
				sprintf(msg, "2.2 Set Address less ADDRESS_MIN get not ADDRESS_ERR on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		} else if (i > adrMax) {
			if (tProtocolModbus.getAddress() != adrMax) {
				sprintf(msg, "2.3 Set Address greater ADDRESS_MAX get not ADDRESS_MAX on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		} else if (i != tProtocolModbus.getAddress()) {
			sprintf(msg, "2.4 Set Address not equivalent get Address on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

/** Тестирование функции проверки принятого пакета.
 * 
 */
void testProtocolModbus::testCheckReadPackage() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	uint8_t buf1[] = {
					0x01, // адрес устройства
					0x01, // команда
					0x00, 0x00, // адрес регистра, hi/low
					0x00, 0x01, // количество регистров, hi/low
					0xFD, 0xCA
	}; // контрольная сумма, hi/low

	uint8_t buf2[] = {
					0x01, // адрес устройства
					0x06, // команда
					0x80, 0x12, // адрес регистра, hi/low
					0x00, 0x13, // количество регистров, hi/low
					0x41, 0xC2
	};


	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	// 1. Проверка правильного определения адреса устройства

	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		sprintf(msg, "1.1 A positive result in default address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	tProtocolModbus.setAddress(buf[0] + 2);
	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		sprintf(msg, "1.2 A positive result in incorrect address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	tProtocolModbus.setAddress(buf[0]);
	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		printf(msg, "1.3 A negative result in correct address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. Проверка контрольной суммы.
	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}
	tProtocolModbus.setAddress(buf[0]);

	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_CRC) {
		uint8_t cnt = sprintf(msg, "2.1 A negative check crc for correct data.");
		//        cnt += sprintf(&msg[cnt], "\n calccrc = 0x%X", tProtocolModbus.calcCRC(sizeof(buf[1]) - 2));
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf2); i++) {
		tProtocolModbus.push(buf2[i]);
	}
	tProtocolModbus.setAddress(buf[0]);

	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_CRC) {
		sprintf(msg, "2.2 A negative check crc for correct data.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	buf[2] += 1;
	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_CRC) {
		sprintf(msg, "2.3 A positive check crc for incorrect data.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 3. Проверка правильного определения команды, а также корректности данных.
	// first byte - number of bytes in package
	
	// проверка максимального кол-ва регистров доступных для одновременного чтения/записи
	if (tProtocolModbus.getMaxNumRegisters() != 32) {
		sprintf(msg, "3.1 Constant MAX_NUM_REGISTER is not 32, it is %d", tProtocolModbus.getMaxNumRegisters());
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// проверка максимального кол-ва флагов доступных для одновременного чтения/записи
	if (tProtocolModbus.getMaxNumCoils() != 256) {
		sprintf(msg, "3.2 Constant MAX_NUM_COILS is not 32, it is %d", tProtocolModbus.getMaxNumRegisters());
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	struct sData {
		TProtocolModbus::CHECK_ERR err; // ошибка 
		uint8_t num; // кол-во байт в посылке
		uint8_t buf[100]; // посылка

	};

	sData data[] = {
					// check command 0x01
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов минимум, 1
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов - в диапазоне
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов максимум, 256
			{0x11, 0x01, 0x00, 0x13, 0x01, 0x00, 0xCE, 0xCF}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // кол-во адресов 0
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // кол-во адресов 257
			{0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0xFC, 0x5A}}, 

					// check command 0x03
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов минимум, 1
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов - в диапазоне
			{0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // кол-во адресов максимум, 32
			{0x01, 0x03, 0x03, 0x30, 0x00, 0x20, 0x44, 0x59}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // кол-во адресов 0
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x00, 0x14, 0x0A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // кол-во адресов 33
			{0x01, 0x03, 0x00, 0x02, 0x00, 0x21, 0x24, 0x12}},

					// check command 0x05
		{TProtocolModbus::CHECK_ERR_NO, 8,	// значение 0x0000 = 0
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}},
		
		{TProtocolModbus::CHECK_ERR_NO, 8, // значение 0xFF00 = 1
			{0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ошибочное значение
			{0x13, 0x05, 0x00, 0xAC, 0x01, 0x00, 0x0F, 0x09}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ошибочное значение
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0xFF, 0x4E, 0xD9}},

					// check command 0x06
		{TProtocolModbus::CHECK_ERR_NO, 8, //
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}},
		{TProtocolModbus::CHECK_ERR_NO, 8, //
			{0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3}},

					// check command 0x0F
		{TProtocolModbus::CHECK_ERR_NO, 10, // кол-во адресов минимум, 1  
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x01, 0x01, 0x00, 0xAA, 0x58}},
		{TProtocolModbus::CHECK_ERR_NO, 11, // кол-во адресов в диапазоне, не кратно 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF,
			 0x0B}}, 
		{TProtocolModbus::CHECK_ERR_NO, 10, // кол-во адресов в диапазоне, равно 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x08, 0x01, 0xCD, 0xBB, 0xCF}},
		{TProtocolModbus::CHECK_ERR_NO, 12, // кол-во адресов в диапазоне, ,больше и не кратно 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x11, 0x03, 0xCD, 0xCD, 0xCD, 0xBB, 0xDB}},	 
		{TProtocolModbus::CHECK_ERR_NO, 12, // кол-во адресов в диапазоне, кратно 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x18, 0x03, 0xCD, 0x01, 0x01, 0x32, 0x8F}},
		{TProtocolModbus::CHECK_ERR_NO, 41, // кол-во адресов максимум, 256
			{0x11, 0x0F, 0x00, 0x13, 0x01, 0x00, 0x20,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			0xBE, 0xE6}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 9, // кол-во адресов 0
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1E, 0x7A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 42,
			{0x11, 0x0F, 0x00, 0x13, 0x01, 0x00, 0x21,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0x05, 0xA7, 0x9E}}, // кол-во адресов 257
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 11, // кол-во заявленных байт меньше
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x01, 0xCD, 0x01, 0x4F, 0x0B}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 11, // кол-во заявленных байт больше
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x03, 0xCD, 0x01, 0xEE, 0xCB}},

					// check command 0x10
		{TProtocolModbus::CHECK_ERR_NO, 11, // кол-во адресов минимум, 1
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x0A, 0xEA, 0x46}},
		{TProtocolModbus::CHECK_ERR_NO, 13, // кол-во адресов в диапазоне, кратно 2
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}},
		{TProtocolModbus::CHECK_ERR_NO, 15, // кол-во адресов в диапазоне, не кратно 2
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x03, 0x06, 0x00, 0x0A, 0x01, 0x02, 0x01, 0x02, 0x31, 0xB9}},
		{TProtocolModbus::CHECK_ERR_NO, 41, // кол-во адресов максимум, 32
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x20, 0x40,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0xED, 0xDE}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 9, // кол-во адресов 0
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x6D}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 75, // кол-во адресов 33
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x21, 0x42,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
			 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
			 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
			 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
			 0x09, 0x09, 0x19, 0xEC}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 13, // кол-во заявленных байт меньше
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x0A, 0x01, 0x02, 0x73, 0x30}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 13, // кол-во заявленных байт больше
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x05, 0x00, 0x0A, 0x01, 0x02, 0xFB, 0x30}},
			
				// check command 0x11
		{TProtocolModbus::CHECK_ERR_NO, 4,
			{0x03, 0x11, 0xC1, 0x4C}},

					// check incorrect function
		{TProtocolModbus::CHECK_ERR_FUNCTION, 4,
			{0x11, 0x04, 0x0C, 0x23}},
		{TProtocolModbus::CHECK_ERR_FUNCTION, 8,
			{0x03, 0x07, 0x00, 0x01, 0x00, 0x01, 0x25, 0xE8}}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].num; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddress(buf[0]);

		TProtocolModbus::CHECK_ERR check = tProtocolModbus.checkReadPackage();
		if (check != data[i].err) {
			uint8_t cnt = sprintf(msg, "3.3 Error check function or function data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n com = 0x%X", data[i].buf[1]);
			cnt += sprintf(&msg[cnt], "\n check = %d, need = %d", check, data[i].err);
			cnt += sprintf(&msg[cnt], "\n startadr = 0x%X", tProtocolModbus.getStartAddress());
			cnt += sprintf(&msg[cnt], "\n numofadr = 0x%X", tProtocolModbus.getNumOfAddress());
//			uint16_t crc = tProtocolModbus.calcCRC(data[i].num - 2);
//			cnt += sprintf(&msg[cnt], "\n calccrc = 0x%X", crc);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

/** Тестирование функции формирования исключения
 * 
 */
void testProtocolModbus::testCheckReadPackage() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData{
		uint8_t bufStart[10]; // данные посылки
		uint8_t numTrBytes; // кол-во байт в сформированной посылке
		TProtocolModbus::EXCEPTION ex; // формируемое исключение
	};
	
	
}

void testProtocolModbus::testPush() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);

	// 1. Проверка записи максимально возможного кол-ва байт данных
	// без проверки срабатывания tick 
	for (uint16_t i = 1; i <= 2 * sizeof (buf); i++) {
		uint16_t t = tProtocolModbus.push(i);
		if (i <= sizeof (buf)) {
			if (t != i) {
				sprintf(msg, "1.1 Error correct writing to the buffer on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		} else {
			if (t != sizeof (buf)) {
				sprintf(msg, "1.2 Error incorrect writing to the buffer on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
	}

	// 2. Проверка сброса посылки при паузе 1.5 интервала.
	// TODO Проверку сброса посылки при паузе 1.5 интервала.
	
	struct sData {
		TProtocolModbus::STATE stateStart;	// состояние протокола в начале
		TProtocolModbus::STATE stateStop;	// состояние протокола в конце
		uint8_t numOfPush;	// кол-во принятых данных
		uint8_t stepToReset;	// шаг, на котором формируется пауза сброса
		uint8_t numOfByte;	// кол-во принятых байт по окончанию
	}; 
	
	sData data[] = {
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 20, 21, 0},
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 20, 5, 0},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 20, 20, 20},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_ERROR, 20, 18, 0},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_ERROR, 20, 2, 0},
		{TProtocolModbus::STATE_READ_ERROR, TProtocolModbus::STATE_READ_ERROR, 20, 18, 0},
		{TProtocolModbus::STATE_READ_OK, TProtocolModbus::STATE_READ_OK, 20, 18, 0},
		{TProtocolModbus::STATE_WRITE_WAIT, TProtocolModbus::STATE_WRITE_WAIT, 20, 18, 0},
		{TProtocolModbus::STATE_WRITE, TProtocolModbus::STATE_WRITE, 20, 21, 0}
	};
	
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = tProtocolModbus.setTick(57600, 100);
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);	// для сброса счетчика
		tProtocolModbus.setState(data[i].stateStart);	// нужное для теста состояние
		
		uint8_t num = 0;
		for (uint8_t j = 0; j < data[i].numOfPush; j++) {
			// формирование паузы сброса
			if (j == data[i].stepToReset) {
				for(uint8_t k = 0; k < 15; k++) {
					tProtocolModbus.tick();
				}
			}	
			num = tProtocolModbus.push(0x00);	
		}

		TProtocolModbus::STATE state = tProtocolModbus.getState();
		if (state != data[i].stateStop) {
			uint8_t cnt = sprintf(msg, "2.1 Error read state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d", data[i].stateStart, state, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d", num);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
		
		if (num != data[i].numOfByte) {
			uint8_t cnt = sprintf(msg, "2.2 Error read number of bytes on step %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d", data[i].stateStart, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].numOfByte);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
	
}

//void testProtocolModbus::testTick()
//{
//    TProtocolModbus tProtocolModbus(buf, sizeof (buf));
//
//    tProtocolModbus.tick();
//    if (true /*check result*/) {
//        CPPUNIT_ASSERT(false);
//    }
//}
//
//void testProtocolModbus::testTrCom()
//{
//    TProtocolModbus tProtocolModbus(buf, sizeof (buf));
//
//    uint8_t result = tProtocolModbus.trCom();
//    if (true /*check result*/) {
//        CPPUNIT_ASSERT(false);
//    }
//}

/** Тестирование функций управления / проверки текущего состояния:
 * 
 *  - void testSetDisable();
 *  - void testSetEnable();
 *  - void testSetState(); 
 *  - void testCheckState();
 *  - void testGetState();
 *  - void testIsEnable();
 *  - void testIsReadData();
 */
void testProtocolModbus::testStates() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	// 1. протокол должен быть отключен после инициализации
	if (tProtocolModbus.isEnable()) {
		sprintf(msg, "1.1. State is not OFF after initialization.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_OFF)) {
		sprintf(msg, "1.2. State is not OFF after initialization.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. проверка работы запуска протокола
	tProtocolModbus.setEnable();

	if (!tProtocolModbus.isEnable()) {
		sprintf(msg, "2.1. State is not ON after setEnable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_READ)) {
		sprintf(msg, "2.1. State is not READ after setEnable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 3. проверка отключения протокола
	tProtocolModbus.setDisable();

	if (tProtocolModbus.isEnable()) {
		sprintf(msg, "3.1. State is not OFF after setDisable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_OFF)) {
		sprintf(msg, "3.1. State is not OFF after setDisable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 4. комплексная проверка работы функций изменения и проверки состояний 
	// STATE_ERROR не проверяется
	for (uint8_t i = TProtocolModbus::STATE_OFF; i < TProtocolModbus::STATE_ERROR; i++) {
		TProtocolModbus::STATE state = static_cast<TProtocolModbus::STATE> (i);

		tProtocolModbus.setState(state);

		if (!tProtocolModbus.checkState(state)) {
			sprintf(msg, "4.1 State is not equal in checkState() on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		if (state != tProtocolModbus.getState()) {
			sprintf(msg, "4.2 State is not equal in getStatee() on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		if ((state == TProtocolModbus::STATE_OFF) == (tProtocolModbus.isEnable())) {
			sprintf(msg, "4.3 Error in isEnable() on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		if ((state == TProtocolModbus::STATE_READ_OK) != (tProtocolModbus.isReadData())) {
			sprintf(msg, "4.4 Error in isReadData() on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}

	// 5. Проверка на невозможность установить состояния STATE_ERROR и выше
	tProtocolModbus.setState(TProtocolModbus::STATE_READ);

	for (uint8_t i = TProtocolModbus::STATE_ERROR; i <= (TProtocolModbus::STATE_ERROR + 5); i++) {
		TProtocolModbus::STATE state = static_cast<TProtocolModbus::STATE> (i);

		tProtocolModbus.setState(state);

		if (tProtocolModbus.checkState(state)) {
			sprintf(msg, "5.1 Error in set on check state on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		if (tProtocolModbus.getState() != TProtocolModbus::STATE_READ) {
			sprintf(msg, "5.2 Error in set or get state on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetStartAddress() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t adr;
		uint8_t buf[9];
	};

	sData data[] = {
		{0,
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01}},
		{19,
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25}},
		{0,
			{0x01, 0x03, 0x00, 0x00, 0x00, 0x07}},
		{1027,
			{0x11, 0x03, 0x04, 0x03, 0x02, 0x05}}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j <= sizeof (data[i].buf); j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddress(buf[0]);

		uint16_t adr = tProtocolModbus.getStartAddress();
		if (adr != data[i].adr) {
			uint8_t cnt = sprintf(msg, "1.1 Error in start address on step %d", i);
			cnt += sprintf(&msg[cnt], "\n adr = %d", data[i].adr);
			cnt += sprintf(&msg[cnt], "\n get = %d", adr);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetNumOfAddress() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t num;
		uint8_t buf[9];
	};

	sData data[] = {
		{1,
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01}},
		{37,
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25}},
		{7,
			{0x01, 0x03, 0x00, 0x00, 0x00, 0x07}},
		{1535,
			{0x11, 0x03, 0x04, 0x00, 0x05, 0xFF}}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j <= sizeof (data[i].buf); j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddress(buf[0]);

		uint16_t num = tProtocolModbus.getNumOfAddress();
		if (num != data[i].num) {
			uint8_t cnt = sprintf(msg, "1.1 Error in start address on step %d", i);
			cnt += sprintf(&msg[cnt], "\n adr = %d", data[i].num);
			cnt += sprintf(&msg[cnt], "\n get = %d", num);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetCoil() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint8_t flag; // ожидаемый результат (0х00, 0х01 или 0хFF)
		uint16_t adr; // адрес флага для считывания
		uint8_t num; // кол-во байт в посылке
		uint8_t buf[12]; // посылка
	};

	sData data[] = {
					// write single coil. CRC not important
		{0x00, 0x00AC, 8,
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}}, // clear flag      
		{0xFF, 0x00AB, 8,
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}}, // wrong address for clear flag
		{0x01, 0x00AC, 8,
			{0x13, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x0E, 0x99}}, // set flag
		{0xFF, 0xAC00, 8,
			{0x13, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x0E, 0x99}}, // wrong address for set flag

					// write multiple coil. CRC and address are not important
		{0x01, 0x0013, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in low first byte
		{0x00, 0x0014, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // clear flag in first byte
		{0x01, 0x001A, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in high first byte
		{0x01, 0x001B, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in second byte
		{0x00, 0x001C, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // clear flag in second byte
		{0xFF, 0x0012, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // wrong: address is less by 1
		{0xFF, 0x001D, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // wrong: address in more by 1
		{0xFF, 0x003D, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}} // wrong address

	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].num; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddress(buf[0]);

		uint8_t flag = tProtocolModbus.getCoil(data[i].adr);
		if (flag != data[i].flag) {
			uint8_t cnt = sprintf(msg, "1.1 Error read value on step %d", i);
			cnt += sprintf(&msg[cnt], "\n flag = 0x%X, need = 0x%X", flag, data[i].flag);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetRegister() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t data; // ожидаемый результат 
		uint16_t adr; // адрес флага для считывания
		uint8_t num; // кол-во байт в посылке
		uint8_t buf[14]; // посылка
	};

	sData data[] = {
					// write single register. CRC not important
		{0x0003, 0x0001, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, //   
		{0x0127, 0x0051, 8,
			{0x11, 0x06, 0x00, 0x51, 0x01, 0x27, 0x9A, 0x9B}}, // 
		{0xF00F, 0x1317, 8,
			{0x11, 0x06, 0x13, 0x17, 0xF0, 0x0F, 0x9A, 0x9B}}, // 
		{0xFFFF, 0xAC00, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, // 
		{0xFFFF, 0x0100, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, // 

					// write multiple registers. CRC and address are not important
		{0x000A, 0x0001, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // read first register 
		{0x0102, 0x0002, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // read second register
		{0x0102, 0x0018, 13,
			{0x11, 0x10, 0x00, 0x17, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // read second register
		{0xFFFF, 0x0004, 13,
			{0x11, 0x10, 0x00, 0x05, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // wrong: address is less by 1
		{0xFFFF, 0xF002, 13,
			{0x11, 0x10, 0xF0, 0x00, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // wrong: address is more by 1
		{0xFFFF, 0x0101, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}} // wrong: address 
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].num; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddress(buf[0]);

		uint16_t val = tProtocolModbus.getRegister(data[i].adr);
		if (val != data[i].data) {
			uint8_t cnt = sprintf(msg, "1.1 Error read value on step %d", i);
			cnt += sprintf(&msg[cnt], "\n data = 0x%X, need = 0x%X", val, data[i].data);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testSetTick() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		uint16_t baudrate; // скорость работы порта 
		uint16_t period; // период вызова функции tick()
		uint16_t step; // полученный шаг 
	};
	
	sData data[] = {
		{57600, 50,  1000}, 
		{57600, 100, 2000},
		{19200, 50,	 872}, 
		{19200, 100, 1745},	
		{300,	50,  13},
		{300,	100, 27}
	};
	
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = tProtocolModbus.setTick(data[i].baudrate, data[i].period);

		if (step != data[i].step) {
			uint8_t cnt = sprintf(msg, "1.1 Error read value on step %d", i);
			cnt += sprintf(&msg[cnt], "\n step = %d, need = %d", step, data[i].step);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testTick() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		TProtocolModbus::STATE startState;	// начальное состояние протокола
		TProtocolModbus::STATE stopState;	// конечное состояние протокола
		uint16_t baudrate;	// скорость работы порта
		uint8_t  period; // период вызова функции tick()
		uint16_t numTicks; // кол-во тиков
		
	};

	sData data[] = {
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 19200, 50, 50},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 57600, 50, 35},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 19200, 50, 41},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 19200, 50, 40},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 19200, 50, 41},
		{TProtocolModbus::STATE_READ_ERROR, TProtocolModbus::STATE_READ, 19200, 50, 41},
		{TProtocolModbus::STATE_READ_OK, TProtocolModbus::STATE_READ_OK, 19200, 50, 50},
		{TProtocolModbus::STATE_WRITE_WAIT, TProtocolModbus::STATE_WRITE_WAIT, 19200, 50, 50},
		{TProtocolModbus::STATE_WRITE, TProtocolModbus::STATE_WRITE, 19200, 50, 50}
	};
	
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = tProtocolModbus.setTick(data[i].baudrate, data[i].period);
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);	// для сброса счетчика
		tProtocolModbus.setState(data[i].startState);	// нужное для теста состояние

		for (uint8_t j = 0; j < data[i].numTicks; j++) {
			tProtocolModbus.tick();
		}

		TProtocolModbus::STATE state = tProtocolModbus.getState();
		if (state != data[i].stopState) {
			uint8_t cnt = sprintf(msg, "1.1 Error read state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d", data[i].startState, state, data[i].stopState);
			cnt += sprintf(&msg[cnt], "\n step tick = %d", step);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}
