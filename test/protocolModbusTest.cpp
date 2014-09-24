#include "gtest\gtest.h"
#include "protocolModbus.h"
#include <cstdio>
#include <iostream>
using namespace std;

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

// чтобы было короче
#define TPM TProtocolModbus

char msg[1000];	// буфер сообщений

class ProtocolModbusTest_Constants: public ::testing::Test {
public:
	static const uint16_t SIZE_BUF = 255;	// размер буфера данных
	uint8_t buf[SIZE_BUF];	// буфер данных

private:
	virtual void SetUp() {};
	virtual void TearDown() {};
};

TEST_F(ProtocolModbusTest_Constants, getMaxNumRegisters) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(32, mb.getMaxNumRegisters());
}

TEST_F(ProtocolModbusTest_Constants, getMaxNumCoils) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(256, mb.getMaxNumCoils());
}

TEST_F(ProtocolModbusTest_Constants, getAddressError) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(255, mb.getAddressError());
}

TEST_F(ProtocolModbusTest_Constants, getAddressMin) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(1, mb.getAddressMin());
}

TEST_F(ProtocolModbusTest_Constants, getAddressMax) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(247, mb.getAddressMax());
}

class ProtocolModbusTest_State: public ::testing::Test {
public:
	static const uint16_t SIZE_BUF = 255;
	uint8_t buf[SIZE_BUF];


private:
	virtual void SetUp() {};
	virtual void TearDown() {};
};

// Проверка состояния при инициализации класса
TEST_F(ProtocolModbusTest_State, protocolModbus) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_TRUE(mb.checkState(TPM::STATE_OFF));
}

TEST_F(ProtocolModbusTest_State, setState_and_checkState) {
	TPM mb(buf, SIZE_BUF);

	// Тестирование функций установки и проверки текущего состояния.

	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb.setState(t);
		if (!mb.checkState(t)) {
			sprintf(msg, "  >>> Ошибка на шаге %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(ProtocolModbusTest_State, getState) {
	TPM mb(buf, SIZE_BUF);

	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb.setState(t);
		if (t != mb.getState()) {
			sprintf(msg, "  >>> Ошибка на шаге %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(ProtocolModbusTest_State, setEnable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	ASSERT_TRUE(mb.checkState(TPM::STATE_READ));
}

TEST_F(ProtocolModbusTest_State, setDisable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	mb.setDisable();
	ASSERT_TRUE(mb.checkState(TPM::STATE_OFF));
}

TEST_F(ProtocolModbusTest_State, isEnable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	ASSERT_TRUE(mb.isEnable());
	mb.setDisable();
	ASSERT_TRUE(!mb.isEnable());
}

TEST_F(ProtocolModbusTest_State, setReadState) {
	TPM mb(buf, SIZE_BUF);


	mb.setReadState();
	ASSERT_TRUE(mb.checkState(TPM::STATE_READ));
}

TEST_F(ProtocolModbusTest_State, isReadData) {
	TPM mb(buf, SIZE_BUF);

	// Полноценной првоерки не проводится!
	// Просто проверяется пара состояний и все.
	// Работа данной функции будет проверена уже в комплексной проверке.

	mb.setDisable();
	ASSERT_TRUE(!mb.isReadData());
	mb.setReadState();
	ASSERT_TRUE(!mb.isReadData());
}

// Проверка проводится при помощи "вспомогательной" функции checkState.
class ProtocolModbusTest_Push_and_Tick: public ::testing::Test {
public:
	static const uint16_t SIZE_BUF = 255;
	uint8_t buf[SIZE_BUF];

private:
	virtual void SetUp() {};
	virtual void TearDown() {};
};

TEST_F(ProtocolModbusTest_Push_and_Tick, setTick) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(1000, mb.setTick(57600, 50));
	ASSERT_EQ(2000, mb.setTick(57600, 100));
	ASSERT_EQ(872, mb.setTick(19200, 50));
	ASSERT_EQ(1745, mb.setTick(19200, 100));
	ASSERT_EQ(13, mb.setTick(300, 50));
	ASSERT_EQ(27, mb.setTick(300, 100));
}

TEST_F(ProtocolModbusTest_Push_and_Tick, tick) {
	TPM mb(buf, SIZE_BUF);

	// Работа этих двух функций завязана друг на друге
	// поэтому они тестируются в паре

	struct sData {
		TPM::STATE startState;	// начальное состояние протокола
		TPM::STATE stopState;	// конечное состояние протокола
		uint8_t	 numBytes;		// кол-во байт данных в буфере
		uint16_t baudrate;		// скорость работы порта
		uint8_t  period; 		// период вызова функции tick()
		uint16_t numTicks; 		// кол-во тиков

	};

	sData data[] = {
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		8, 19200, 50, 50},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		3, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	4, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	5, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	6, 19200, 50, 41},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		8, 19200, 50, 40},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	8, 19200, 50, 41},
			{TPM::STATE_READ_ERROR, TPM::STATE_READ, 		8, 19200, 50, 41},
			{TPM::STATE_READ_OK, 	TPM::STATE_READ_OK, 	8, 19200, 50, 50},
			{TPM::STATE_WRITE_WAIT, TPM::STATE_WRITE_WAIT, 	8, 19200, 50, 50},
			{TPM::STATE_WRITE, 		TPM::STATE_WRITE, 		8, 19200, 50, 50}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = mb.setTick(data[i].baudrate, data[i].period);
		mb.setState(TPM::STATE_READ);	// для сброса счетчика
		mb.setState(data[i].startState);	// нужное для теста состояние

		// закинем в буфер заданное кол-во байт данных
		for(uint8_t j = 0; j < data[i].numBytes; j++) {
			mb.push(0x00);
		}

		for (uint8_t j = 0; j < data[i].numTicks; j++) {
			mb.tick();
		}

		TPM::STATE state = mb.getState();
		if (state != data[i].stopState) {
			uint8_t cnt = sprintf(msg, "  >>> Ошибка на шаге %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d",
					data[i].startState, state, data[i].stopState);
			cnt += sprintf(&msg[cnt], "\n step tick = %d", step);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(ProtocolModbusTest_Push_and_Tick, push) {
	TPM mb(buf, SIZE_BUF);

	mb.setState(TPM::STATE_READ);

	// 1. Проверка записи максимально возможного кол-ва байт данных
	// без проверки срабатывания tick
	for (uint16_t i = 1; i <= 2 * sizeof (buf); i++) {
		uint16_t t = mb.push(i);
		if (i <= sizeof (buf)) {
			if (t != i) {
				sprintf(msg, "  <<< Неверное количество принятых байт на шаге %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else {
			if (t != sizeof (buf)) {
				sprintf(msg, "  <<< Переполнение буфера на шаге %d", i);
				ASSERT_TRUE(false) << msg;
			}
		}
	}

	// 2. Проверка сброса посылки при паузе 1.5 интервала.
	struct sData {
		TPM::STATE stateStart;	// состояние протокола в начале
		TPM::STATE stateStop;	// состояние протокола в конце
		uint8_t numOfPush;	// кол-во принятых данных
		uint8_t stepToReset;	// шаг, на котором формируется пауза сброса
		uint8_t numOfByte;	// кол-во принятых байт по окончанию
	};

	sData data[] = {
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		20, 21,	0},
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		20, 5, 	0},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		20, 20,	20},
			{TPM::STATE_READ, 		TPM::STATE_READ_ERROR, 	20, 18,	18},
			{TPM::STATE_READ, 		TPM::STATE_READ_ERROR, 	20, 2,	2},
			{TPM::STATE_READ_ERROR,	TPM::STATE_READ_ERROR, 	20, 18,	0},
			{TPM::STATE_READ_OK, 	TPM::STATE_READ_OK, 	20, 18,	0},
			{TPM::STATE_WRITE_WAIT,	TPM::STATE_WRITE_WAIT, 	20, 18,	0},
			{TPM::STATE_WRITE, 		TPM::STATE_WRITE, 		20, 21,	0}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		mb.setTick(57600, 100);
		mb.setState(TPM::STATE_READ);	// для сброса счетчика
		mb.setState(data[i].stateStart);	// нужное для теста состояние

		uint8_t num = 0;
		for (uint8_t j = 0; j < data[i].numOfPush; j++) {
			// формирование паузы сброса
			if (j == data[i].stepToReset) {
				for(uint8_t k = 0; k < 15; k++) {
					mb.tick();
				}
			}
			num = mb.push(0x00);
		}

		TPM::STATE state = mb.getState();
		if (state != data[i].stateStop) {
			uint8_t cnt = sprintf(msg, "  >>> Ошибочное состояние на шаге %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d", data[i].stateStart, state, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d", num);
			ASSERT_TRUE(false) << msg;
		}

		if (num != data[i].numOfByte) {
			uint8_t cnt = sprintf(msg, "  >>> Ошибочное кол-во принятых байт на шаге %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d", data[i].stateStart, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].numOfByte);
			ASSERT_TRUE(false) << msg;
		}
	}
}

// Проверка проводится при помощи "вспомогательной" функции checkState.
class ProtocolModbusTest_AddressLan: public ::testing::Test {
public:
	static const uint16_t SIZE_BUF = 255;
	uint8_t buf[SIZE_BUF];

private:
	virtual void SetUp() {};
	virtual void TearDown() {};
};

TEST_F(ProtocolModbusTest_AddressLan, get_and_set) {
	TPM mb(buf, sizeof (buf));

	uint8_t adrError = mb.getAddressError();
	uint8_t adrMin = mb.getAddressMin();
	uint8_t adrMax = mb.getAddressMax();

	// 1. Проверка адреса по-умолчанию.
	if (mb.getAddressLan() != adrError) {
		sprintf(msg, "  >>> Адрес по умолчанию не совпадает с ADDRESS_ERR.");
		ASSERT_TRUE(false) << msg;
	}

	// 2. Проверка установки/считывания адреса.
	for (uint16_t i = 0; i <= 255; i++) {
		bool t = mb.setAddressLan(i);

		// проверка возвращаемого значения функцией serAddress())
		if (((i < adrMin) || (i > adrMax)) == t) {
			sprintf(msg, "  >>> setAddress() возвращает ошибочное состояние на шаге %d", i);
			ASSERT_TRUE(false) << msg;
		}

		// проверка установленного значения
		if (i < adrMin) {
			if (mb.getAddressLan() != adrError) {
				sprintf(msg, "  >>> Значение меньше ADDRESS_MIN не ведет к ADDRESS_ERR на шаге %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i > adrMax) {
			if (mb.getAddressLan() != adrMax) {
				sprintf(msg, "  >>> Значение больше ADDRESS_MAX не ведет к ADDRESS_MAX на шаге %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i != mb.getAddressLan()) {
			sprintf(msg, "  >>> Установленный адрес не совпадает со считанным на шаге %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

class ProtocolModbusTest_readData: public ::testing::Test {
public:
	static const uint16_t SIZE_BUF = 255; 	// размер буфера данных
	uint8_t buf[SIZE_BUF];					// буфер данных
	TPM *mb;								// тестируемый класс

	// конструктор
	ProtocolModbusTest_readData() {
		mb = new TPM(buf, SIZE_BUF);
		mb->setTick(57600, 100);
	}

	// деструктор
	virtual ~ProtocolModbusTest_readData() {
		delete mb;
	}

	// формирует процесс получения посылки
	void readCom(uint8_t *buf, uint16_t size) {

		mb->setReadState();

		// пересылка данных в буфер
		for (uint8_t i = 0; i < size; i++) {
			mb->push(*buf++);
		}

		// формирование сигнала окончания посылки
		for(uint8_t k = 0; k < 50; k++) {
			mb->tick();
		}
	}

	// сравнение массивов, true - совпадают
	bool checkArray(uint8_t *buf1, uint16_t size1, uint8_t *buf2, uint16_t size2) {
		if (size1 != size2) {
			cout << "Отличаются размеры массивов size1 = " << size1 <<
					", size2 = " << size2 << endl;;
			return false;
		}


		for (uint16_t i = 0; i < size1; i++) {
			if (buf1[i] != buf2[i]) {
				cout << "Не совпали данные с номером " << i << " buf1 = " <<
						(uint16_t) buf1[i] << ", buf2 = " << (uint16_t) buf2[i];
				cout << endl;
				return false;
			}
		}

		return true;
	}

	// тестирование
	bool test(uint8_t *request, uint16_t size1, uint8_t *response, uint16_t size2) {
		readCom(request, size1);
		mb->setAddressLan(request[0]);
		mb->readData();
		return checkArray(response, size2, buf, mb->getNumOfBytes());
	}

	char* printArray(uint8_t *arr, uint16_t size) {
		uint16_t cnt = 0;
		for(uint8_t i = 0; i < size; i++) {
			cnt += sprintf(&msg[cnt], "0x%02X ", arr[i]);
		}
		return msg;
	}

private:
	virtual void SetUp() {};
	virtual void TearDown() {};
};

TEST_F(ProtocolModbusTest_readData, getStartAddress) {
	// проверяется только значение по умолчанию
	// дополнительная проверка в readData

	ASSERT_EQ(0, mb->getStartAddress());
}

TEST_F(ProtocolModbusTest_readData, getNumOfAddress) {
	// проверяется только значение по умолчанию
	// дополнительная проверка в readData
	ASSERT_EQ(0, mb->getNumOfAddress());
}

TEST_F(ProtocolModbusTest_readData, getNumOfBytes) {
	// проверяется только значение по умолчанию
	// дополнительная проверка в readData
	ASSERT_EQ(0, mb->getNumOfBytes());
}

// проверка команды чтения флагов
TEST_F(ProtocolModbusTest_readData, com_0x01_read_coil) {

	// считывание одного адреса со значением true
	uint8_t req1[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA};
	uint8_t res1[] = {0x01, 0x01, 0x01, 0x01};

	ASSERT_TRUE(test(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1)));

	// считывание одного адреса со значением false
	uint8_t req2[] = {0x01, 0x01, 0x00, 0x65, 0x00, 0x01, 0xED, 0xD5};
	uint8_t res2[] = {0x01, 0x01, 0x01, 0x00};

	ASSERT_TRUE(test(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2)));

	// считывание группы адресов
	uint8_t req3[] = {0x11, 0x01, 0x00, 0x50, 0x00, 0x29, 0xFF, 0x55};
	uint8_t res3[] = {0x11, 0x01, 0x06, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00};

	// cчитывание максимально возможного кол-ва адресов


	ASSERT_TRUE(test(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3)));
}

// проверка команды ID (без CRC)
TEST_F(ProtocolModbusTest_readData, com_0x11_slave_ID) {

	uint8_t request[] = {0x03, 0x11, 0xC1, 0x4C};
	uint8_t response[] = {0x03, 0x11, 0x08, 'V','i','r','t','u','a','l', 0x00};

	readCom(request, SIZE_ARRAY(request));
	mb->setAddressLan(request[0]);
	mb->readData();
	ASSERT_TRUE(checkArray(response, SIZE_ARRAY(response), buf, mb->getNumOfBytes()));
}
