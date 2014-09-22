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

/**	�������� ��������
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

/** ������������ ���������/������ ������ ����������.
 * 
 */
void testProtocolModbus::testAddressLan() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	uint8_t adrError = tProtocolModbus.getAddressError();
	uint8_t adrMin = tProtocolModbus.getAddressMin();
	uint8_t adrMax = tProtocolModbus.getAddressMax();
	
	// 1. �������� ������ ��-���������.
	if (tProtocolModbus.getAddressLan() != adrError) {
		sprintf(msg, "1.1 Address by default is not ADDRESS_ERR.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. �������� ���������/���������� ������.
	for (uint16_t i = 0; i <= 255; i++) {
		bool t = tProtocolModbus.setAddressLan(i);

		// �������� ������������� �������� �������� serAddress())
		if (((i < adrMin) || (i > adrMax)) == t) {
			sprintf(msg, "2.1 Function setAddress() returned error state on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		// �������� �������������� ��������
		if (i < adrMin) {
			if (tProtocolModbus.getAddressLan() != adrError) {
				sprintf(msg, "2.2 Set Address less ADDRESS_MIN get not ADDRESS_ERR on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		} else if (i > adrMax) {
			if (tProtocolModbus.getAddressLan() != adrMax) {
				sprintf(msg, "2.3 Set Address greater ADDRESS_MAX get not ADDRESS_MAX on step %d", i);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		} else if (i != tProtocolModbus.getAddressLan()) {
			sprintf(msg, "2.4 Set Address not equivalent get Address on step %d", i);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

/** ������������ ������� �������� ��������� ������.
 * 
 */
void testProtocolModbus::testCheckReadPackage() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	uint8_t buf1[] = {
					0x01, // ����� ����������
					0x01, // �������
					0x00, 0x00, // ����� ��������, hi/low
					0x00, 0x01, // ���������� ���������, hi/low
					0xFD, 0xCA
	}; // ����������� �����, hi/low

	uint8_t buf2[] = {
					0x01, // ����� ����������
					0x06, // �������
					0x80, 0x12, // ����� ��������, hi/low
					0x00, 0x13, // ���������� ���������, hi/low
					0x41, 0xC2
	};


	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	// 1. �������� ����������� ����������� ������ ����������

	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		sprintf(msg, "1.1 A positive result in default address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	tProtocolModbus.setAddressLan(buf[0] + 2);
	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		sprintf(msg, "1.2 A positive result in incorrect address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}

	tProtocolModbus.setAddressLan(buf[0]);
	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
		printf(msg, "1.3 A negative result in correct address.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. �������� ����������� �����.
	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf1); i++) {
		tProtocolModbus.push(buf1[i]);
	}
	tProtocolModbus.setAddressLan(buf[0]);

	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_CRC) {
		uint8_t cnt = sprintf(msg, "2.1 A negative check crc for correct data.");
		//        cnt += sprintf(&msg[cnt], "\n calccrc = 0x%X", tProtocolModbus.calcCRC(sizeof(buf[1]) - 2));
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);
	for (uint8_t i = 0; i < sizeof (buf2); i++) {
		tProtocolModbus.push(buf2[i]);
	}
	tProtocolModbus.setAddressLan(buf[0]);

	if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_CRC) {
		sprintf(msg, "2.2 A negative check crc for correct data.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	buf[2] += 1;
	if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_CRC) {
		sprintf(msg, "2.3 A positive check crc for incorrect data.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 3. �������� ����������� ����������� �������, � ����� ������������ ������.
	// first byte - number of bytes in package
	
	// �������� ������������� ���-�� ��������� ��������� ��� �������������� ������/������
	if (tProtocolModbus.getMaxNumRegisters() != 32) {
		sprintf(msg, "3.1 Constant MAX_NUM_REGISTER is not 32, it is %d", tProtocolModbus.getMaxNumRegisters());
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// �������� ������������� ���-�� ������ ��������� ��� �������������� ������/������
	if (tProtocolModbus.getMaxNumCoils() != 256) {
		sprintf(msg, "3.2 Constant MAX_NUM_COILS is not 32, it is %d", tProtocolModbus.getMaxNumRegisters());
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}
	
	struct sData {
		TProtocolModbus::CHECK_ERR err; // ������ 
		uint8_t num; // ���-�� ���� � �������
		uint8_t buf[100]; // �������

	};

	sData data[] = {
					// check command 0x01
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� �������, 1
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� - � ���������
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� ��������, 256
			{0x11, 0x01, 0x00, 0x13, 0x01, 0x00, 0xCE, 0xCF}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ���-�� ������� 0
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ���-�� ������� 257
			{0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0xFC, 0x5A}}, 

					// check command 0x03
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� �������, 1
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� - � ���������
			{0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39}},
		{TProtocolModbus::CHECK_ERR_NO, 8, // ���-�� ������� ��������, 32
			{0x01, 0x03, 0x03, 0x30, 0x00, 0x20, 0x44, 0x59}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ���-�� ������� 0
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x00, 0x14, 0x0A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ���-�� ������� 33
			{0x01, 0x03, 0x00, 0x02, 0x00, 0x21, 0x24, 0x12}},

					// check command 0x05
		{TProtocolModbus::CHECK_ERR_NO, 8,	// �������� 0x0000 = 0
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}},
		
		{TProtocolModbus::CHECK_ERR_NO, 8, // �������� 0xFF00 = 1
			{0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ��������� ��������
			{0x13, 0x05, 0x00, 0xAC, 0x01, 0x00, 0x0F, 0x09}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, // ��������� ��������
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0xFF, 0x4E, 0xD9}},

					// check command 0x06
		{TProtocolModbus::CHECK_ERR_NO, 8, //
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}},
		{TProtocolModbus::CHECK_ERR_NO, 8, //
			{0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3}},

					// check command 0x0F
		{TProtocolModbus::CHECK_ERR_NO, 10, // ���-�� ������� �������, 1  
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x01, 0x01, 0x00, 0xAA, 0x58}},
		{TProtocolModbus::CHECK_ERR_NO, 11, // ���-�� ������� � ���������, �� ������ 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF,
			 0x0B}}, 
		{TProtocolModbus::CHECK_ERR_NO, 10, // ���-�� ������� � ���������, ����� 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x08, 0x01, 0xCD, 0xBB, 0xCF}},
		{TProtocolModbus::CHECK_ERR_NO, 12, // ���-�� ������� � ���������, ,������ � �� ������ 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x11, 0x03, 0xCD, 0xCD, 0xCD, 0xBB, 0xDB}},	 
		{TProtocolModbus::CHECK_ERR_NO, 12, // ���-�� ������� � ���������, ������ 8
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x18, 0x03, 0xCD, 0x01, 0x01, 0x32, 0x8F}},
		{TProtocolModbus::CHECK_ERR_NO, 41, // ���-�� ������� ��������, 256
			{0x11, 0x0F, 0x00, 0x13, 0x01, 0x00, 0x20,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			0xBE, 0xE6}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 9, // ���-�� ������� 0
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1E, 0x7A}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 42,
			{0x11, 0x0F, 0x00, 0x13, 0x01, 0x00, 0x21,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0x05, 0xA7, 0x9E}}, // ���-�� ������� 257
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 11, // ���-�� ���������� ���� ������
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x01, 0xCD, 0x01, 0x4F, 0x0B}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 11, // ���-�� ���������� ���� ������
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x03, 0xCD, 0x01, 0xEE, 0xCB}},

					// check command 0x10
		{TProtocolModbus::CHECK_ERR_NO, 11, // ���-�� ������� �������, 1
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x0A, 0xEA, 0x46}},
		{TProtocolModbus::CHECK_ERR_NO, 13, // ���-�� ������� � ���������, ������ 2
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}},
		{TProtocolModbus::CHECK_ERR_NO, 15, // ���-�� ������� � ���������, �� ������ 2
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x03, 0x06, 0x00, 0x0A, 0x01, 0x02, 0x01, 0x02, 0x31, 0xB9}},
		{TProtocolModbus::CHECK_ERR_NO, 41, // ���-�� ������� ��������, 32
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x20, 0x40,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0xED, 0xDE}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 9, // ���-�� ������� 0
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x6D}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 75, // ���-�� ������� 33
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
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 13, // ���-�� ���������� ���� ������
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x0A, 0x01, 0x02, 0x73, 0x30}},
		{TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 13, // ���-�� ���������� ���� ������
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
		tProtocolModbus.setAddressLan(buf[0]);

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
		
		// �������� ����������� ������, ��� ������� ������ �������� �������
		if (data[i].err == TProtocolModbus::CHECK_ERR_FUNCTION) {
			if (!tProtocolModbus.checkState(TProtocolModbus::STATE_WRITE_READY)) {
				uint8_t cnt = sprintf(msg, "3.4 Error state on step %d", i);
				cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", tProtocolModbus.getState(), TProtocolModbus::STATE_WRITE);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
			
			if ((buf[1] != (data[i].buf[1] | 0x80)) ||
				(buf[2] != TProtocolModbus::EXCEPTION_01H_ILLEGAL_FUNCTION)) {
				uint8_t cnt = sprintf(msg, "3.5 Error data in buffer for transferring on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[1] = 0x%X, need 0x%X", buf[1], (data[i].buf[1] | 0x80));
				cnt += sprintf(&msg[cnt], "\n buf[2] = 0x%X, need 0x%X", buf[2], TProtocolModbus::EXCEPTION_01H_ILLEGAL_FUNCTION);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
		
		// �������� ����������� ������, ��� ������� ������ �������� ������
		if (data[i].err == TProtocolModbus::CHECK_ERR_FUNCTION_DATA) {
			if (!tProtocolModbus.checkState(TProtocolModbus::STATE_WRITE_READY)) {
				uint8_t cnt = sprintf(msg, "3.6 Error state on step %d", i);
				cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", tProtocolModbus.getState(), TProtocolModbus::STATE_WRITE);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
			if ((buf[1] != (data[i].buf[1] | 0x80)) ||
				(buf[2] != TProtocolModbus::EXCEPTION_03H_ILLEGAL_DATA_VAL)) {
				uint8_t cnt = sprintf(msg, "3.7 Error data in buffer for transferring on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[1] = 0x%X, need 0x%X", buf[1], (data[i].buf[1] | 0x80));
				cnt += sprintf(&msg[cnt], "\n buf[2] = 0x%X, need 0x%X", buf[2], TProtocolModbus::EXCEPTION_03H_ILLEGAL_DATA_VAL);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
	}
}

void testProtocolModbus::testGetStartAddress() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t adr;		// ��������� �����
		uint16_t num;		// ���������� �������
		uint8_t numb;		// ���-�� ���� � �������
		uint8_t buf[40];	// �������
	};

	sData data[] = {
					// check command 0x01
		{1, 1, 8, 
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA}},
		{20, 37, 8, 
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84}},

					// check command 0x03
		{2, 1, 8, 
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}},
		{257, 20, 8, 
			{0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39}},

					// check command 0x05
		{173, 1, 8,	
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}},
		{173, 1, 8, 
			{0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}},

					// check command 0x06
		{2, 1, 8, //
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}},
		{273, 1, 8, //
			{0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3}},

					// check command 0x0F
		{20, 1, 10,  
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x01, 0x01, 0x00, 0xAA, 0x58}},
		{20, 10, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, 

					// check command 0x10
		{2, 1, 11, 
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x0A, 0xEA, 0x46}},
		{2, 2, 13, 
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}},
			
				// check command 0x11
		{0, 0, 4,
			{0x03, 0x11, 0xC1, 0x4C}}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numb; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddressLan(data[i].buf[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();

		uint16_t adr = tProtocolModbus.getStartAddress();
		if (adr != data[i].adr) {
			uint8_t cnt = sprintf(msg, "1.1 Error in start address on step %d", i);
			cnt += sprintf(&msg[cnt], "\n adr = %d, need = %d", adr, data[i].adr);
			cnt += sprintf(&msg[cnt], "\n err = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetNumOfAddress() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t adr;		// ��������� �����
		uint16_t num;		// ���������� �������
		uint8_t  numb;		// ���-�� ���� � �������
		uint8_t  buf[40];	// �������
	};

	sData data[] = {
					// check command 0x01
		{0, 1, 8, 
			{0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA}},
		{19, 37, 8, 
			{0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84}},

					// check command 0x03
		{1, 1, 8, 
			{0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}},
		{256, 20, 8, 
			{0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39}},

					// check command 0x05
		{172, 1, 8,	
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}},
		{1280, 1, 8, 
			{0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}},

					// check command 0x06
		{1, 1, 8, //
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}},
		{272, 1, 8, //
			{0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3}},

					// check command 0x0F
		{19, 1, 10,  
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x01, 0x01, 0x00, 0xAA, 0x58}},
		{19, 10, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, 

					// check command 0x10
		{1, 1, 11, 
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x0A, 0xEA, 0x46}},
		{1, 2, 13, 
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}},
			
				// check command 0x11
		{0, 0, 4,
			{0x03, 0x11, 0xC1, 0x4C}}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numb; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddressLan(buf[0]);
		tProtocolModbus.checkReadPackage();

		uint16_t num = tProtocolModbus.getNumOfAddress();
		if (num != data[i].num) {
			uint8_t cnt = sprintf(msg, "1.1 Error in number of data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].num);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetCoil() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint8_t flag; // ��������� ��������� (0�00, 0�01 ��� 0�FF)
		uint16_t adr; // ����� ����� ��� ����������
		uint8_t num; // ���-�� ���� � �������
		uint8_t buf[12]; // �������
	};

	sData data[] = {
					// write single coil. CRC not important
		{0x00, 0x00AD, 8,
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}}, // clear flag      
		{0xFF, 0x00AC, 8,
			{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}}, // wrong address for clear flag
		{0x01, 0x00AD, 8,
			{0x13, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4F, 0x69}}, // set flag
		{0xFF, 0xAD00, 8,
			{0x13, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x0E, 0x99}}, // wrong address for set flag

					// write multiple coil. CRC and address are not important
		{0x01, 0x0014, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in low first byte
		{0x00, 0x0015, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // clear flag in first byte
		{0x01, 0x001B, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in high first byte
		{0x01, 0x001C, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // set flag in second byte
		{0x00, 0x001D, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // clear flag in second byte
		{0xFF, 0x0013, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // wrong: address is less by 1
		{0xFF, 0x001E, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}}, // wrong: address in more by 1
		{0xFF, 0x003E, 11,
			{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}} // wrong address

	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].num; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddressLan(buf[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();

		uint8_t flag = tProtocolModbus.getCoil(data[i].adr);
		if (flag != data[i].flag) {
			uint8_t cnt = sprintf(msg, "1.1 Error read value on step %d", i);
			cnt += sprintf(&msg[cnt], "\n flag = 0x%X, need = 0x%X", flag, data[i].flag);
			cnt += sprintf(&msg[cnt], "\n err = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

void testProtocolModbus::testGetRegister() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	struct sData {
		uint16_t data; // ��������� ��������� 
		uint16_t adr; // ����� ����� ��� ����������
		uint8_t num; // ���-�� ���� � �������
		uint8_t buf[14]; // �������
	};

	sData data[] = {
					// write single register
		{0x0003, 0x0002, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, //   
		{0x0127, 0x0052, 8,
			{0x11, 0x06, 0x00, 0x51, 0x01, 0x27, 0x9B, 0x01}}, // 
		{0xF00F, 0x1318, 8,
			{0x11, 0x06, 0x13, 0x17, 0xF0, 0x0F, 0x3B, 0xDE}}, // 
		{0xFFFF, 0xAC01, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, // 
		{0xFFFF, 0x0101, 8,
			{0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}}, // 

					// write multiple registers
		{0x000A, 0x0002, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // read first register 
		{0x0102, 0x0003, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // read second register
		{0x0102, 0x0019, 13,
			{0x11, 0x10, 0x00, 0x17, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0x47, 0xD6}}, // read second register
		{0xFFFF, 0x0005, 13,
			{0x11, 0x10, 0x00, 0x05, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // wrong: address is less by 1
		{0xFFFF, 0xF003, 13,
			{0x11, 0x10, 0xF0, 0x00, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}}, // wrong: address is more by 1
		{0xFFFF, 0x0102, 13,
			{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}} // wrong: address 
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].num; j++) {
			tProtocolModbus.push(data[i].buf[j]);
		}
		tProtocolModbus.setAddressLan(buf[0]);
		tProtocolModbus.checkReadPackage();

		uint16_t val = tProtocolModbus.getRegister(data[i].adr);
		if (val != data[i].data) {
			uint8_t cnt = sprintf(msg, "1.1 Error read value on step %d", i);
			cnt += sprintf(&msg[cnt], "\n data = 0x%X, need = 0x%X", val, data[i].data);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}

/** ������������ ������� ������������ ����������.
 * 
 */
//void testProtocolModbus::testSetException() {
//	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
//	
//	struct sData{
//		uint8_t bufStart[15]; // �������� �������
//		uint8_t bufStop[10];	// ��������� ������� � �����������
//		uint8_t numTrBytes; // ���-�� ���� � �������������� �������
//		TProtocolModbus::EXCEPTION ex; // ����������� ����������
//	};
//	
//	sData data[] = {
//		{{0x12, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8E, 0xA1},
//		{0x12, 0x81, 0x01, 0x70, 0x55}, 
//		5, TProtocolModbus::EXCEPTION_01H_ILLEGAL_FUNCTION},
//		{{0x08, 0x03, 0x00, 0x6B, 0x00, 0x03, 0x8E, 0x74},
//		{0x08, 0x83, 0x02, 0x10, 0xF3}, 
//		5, TProtocolModbus::EXCEPTION_02H_ILLEGAL_DATA_ADR},
//		{{0x76, 0x06, 0x00, 0x01, 0x00, 0x03, 0x92, 0x8C},
//		{0x76, 0x86, 0x03, 0xB2, 0x7B},
//		5, TProtocolModbus::EXCEPTION_03H_ILLEGAL_DATA_VAL},
//		{{0xAA, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x55, 0xC0},
//		{0xAA, 0x85, 0x04, 0x32, 0xB3}, 
//		5, TProtocolModbus::EXCEPTION_04H_DEVICE_FAILURE},
//		{{0x01, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0x72, 0xCB},
//		{0x01, 0x8F, 0x10, 0x45, 0xFC},
//		5, TProtocolModbus::EXCEPTION_10H_TEMP_INAC_PARAM},
//		{{0x02, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0x9D, 0x74},
//		{0x02, 0x90, 0x11, 0x7C, 0x0C},
//		5, TProtocolModbus::EXCEPTION_11H_UNCHANG_PARAM}
//	};
//
//	for (uint16_t i = 0; i < (sizeof(data)/sizeof(data[0])); i++) {
//		for (uint8_t j = 0; j < sizeof(data[0].bufStart); j++) {
//			buf[j] = data[i].bufStart[j]; 
//		}
//		
//		tProtocolModbus.setException(data[i].ex);
//		
//		uint8_t numbytes = tProtocolModbus.getNumOfBytes();
//		if (numbytes != data[i].numTrBytes) {
//			uint8_t cnt = sprintf(msg, "1.1 Wrong number of byte for transferring on step %d", i);
//			cnt += sprintf(&msg[cnt], "\n Num of bytes = %d, need = %d", numbytes,  data[i].numTrBytes);
//			CPPUNIT_ASSERT_MESSAGE(msg, false);
//		}
//		
//		for(uint8_t j = 0; j < data[i].numTrBytes; j++) {
//			if (buf[j] != data[i].bufStop[j]) {
//				uint8_t cnt = sprintf(msg, "1.2 Wrong %d byte in package on step %d", j, i);
//				cnt += sprintf(&msg[cnt], "\n Byte[%d] = 0x%X, need 0x%X", j, buf[j], data[i].bufStop[j]);
//				CPPUNIT_ASSERT_MESSAGE(msg, false);
//			}
//		}
//	}
//	
//	// 
//}

void testProtocolModbus::testPush() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));

	tProtocolModbus.setState(TProtocolModbus::STATE_READ);

	// 1. �������� ������ ����������� ���������� ���-�� ���� ������
	// ��� �������� ������������ tick 
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

	// 2. �������� ������ ������� ��� ����� 1.5 ���������.
	struct sData {
		TProtocolModbus::STATE stateStart;	// ��������� ��������� � ������
		TProtocolModbus::STATE stateStop;	// ��������� ��������� � �����
		uint8_t numOfPush;	// ���-�� �������� ������
		uint8_t stepToReset;	// ���, �� ������� ����������� ����� ������
		uint8_t numOfByte;	// ���-�� �������� ���� �� ���������
	}; 
	
	sData data[] = {
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 20, 21, 0},
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 20, 5, 0},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 20, 20, 20},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_ERROR, 20, 18, 18},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_ERROR, 20, 2, 2},
		{TProtocolModbus::STATE_READ_ERROR, TProtocolModbus::STATE_READ_ERROR, 20, 18, 0},
		{TProtocolModbus::STATE_READ_OK, TProtocolModbus::STATE_READ_OK, 20, 18, 0},
		{TProtocolModbus::STATE_WRITE_WAIT, TProtocolModbus::STATE_WRITE_WAIT, 20, 18, 0},
		{TProtocolModbus::STATE_WRITE, TProtocolModbus::STATE_WRITE, 20, 21, 0}
	};
	
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		uint16_t step = tProtocolModbus.setTick(57600, 100);
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);	// ��� ������ ��������
		tProtocolModbus.setState(data[i].stateStart);	// ������ ��� ����� ���������
		
		uint8_t num = 0;
		for (uint8_t j = 0; j < data[i].numOfPush; j++) {
			// ������������ ����� ������
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


void testProtocolModbus::testTrResponse()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	// ���������� �� ��������� ��������� ������, ������� ���� ��������
	
	
	
	struct sData {
		TProtocolModbus::STATE stateStart;	// ��������� ���������
		TProtocolModbus::STATE stateStop;	// ��������� ���������
		uint8_t numBytes;	// ���-�� ���� ������ �� ��������
	};
	
	sData data[] = {
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 0},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 0},
		{TProtocolModbus::STATE_READ_ERROR, TProtocolModbus::STATE_READ_ERROR, 0},
		{TProtocolModbus::STATE_READ_OK, TProtocolModbus::STATE_WRITE, 8}, // ������� �����
		{TProtocolModbus::STATE_WRITE_WAIT, TProtocolModbus::STATE_WRITE_WAIT, 0},
		{TProtocolModbus::STATE_WRITE_READY, TProtocolModbus::STATE_WRITE, 6},	// ����� ����� ��� CRC
		{TProtocolModbus::STATE_WRITE, TProtocolModbus::STATE_WRITE, 0}
	};
	
	for (uint8_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		uint8_t pckg[] = {0x12, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8E, 0xA1};
		for (uint8_t j = 0; j < sizeof (pckg); j++) {
			tProtocolModbus.push(pckg[j]);
		}
		tProtocolModbus.setAddressLan(pckg[0]);
//		tProtocolModbus.setException(TProtocolModbus::EXCEPTION_01H_ILLEGAL_FUNCTION);

		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();
		tProtocolModbus.prepareResponse();
		tProtocolModbus.setState(data[i].stateStart);
		uint8_t num = tProtocolModbus.trResponse();

		TProtocolModbus::STATE state = tProtocolModbus.getState();
		if (state != data[i].stateStop) {
			uint8_t cnt = sprintf(msg, "1.1 Wrong state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", state, data[i].stateStop);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}

		if (num != data[i].numBytes) {
			uint8_t cnt = sprintf(msg, "1.2 Invalid number of bytes to transfer on step %d", i);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].numBytes);
			cnt += sprintf(&msg[cnt], "\n err = %d", err);
			cnt += sprintf(&msg[cnt], "\n");
			for (uint8_t j = 0; j < num; j++) {
				cnt += sprintf(&msg[cnt], "%02X ", buf[j]);
			}
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
		
		
	}
	
}

/** ������������ ������� ���������� / �������� �������� ���������:
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

	// 1. �������� ������ ���� �������� ����� �������������
	if (tProtocolModbus.isEnable()) {
		sprintf(msg, "1.1. State is not OFF after initialization.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_OFF)) {
		sprintf(msg, "1.2. State is not OFF after initialization.");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 2. �������� ������ ������� ���������
	tProtocolModbus.setEnable();

	if (!tProtocolModbus.isEnable()) {
		sprintf(msg, "2.1. State is not ON after setEnable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_READ)) {
		sprintf(msg, "2.1. State is not READ after setEnable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 3. �������� ���������� ���������
	tProtocolModbus.setDisable();

	if (tProtocolModbus.isEnable()) {
		sprintf(msg, "3.1. State is not OFF after setDisable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	if (!tProtocolModbus.checkState(TProtocolModbus::STATE_OFF)) {
		sprintf(msg, "3.1. State is not OFF after setDisable().");
		CPPUNIT_ASSERT_MESSAGE(msg, false);
	}

	// 4. ����������� �������� ������ ������� ��������� � �������� ��������� 
	// STATE_ERROR �� �����������
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

	// 5. �������� �� ������������� ���������� ��������� STATE_ERROR � ����
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

void testProtocolModbus::testSetTick() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		uint16_t baudrate; // �������� ������ ����� 
		uint16_t period; // ������ ������ ������� tick()
		uint16_t step; // ���������� ��� 
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

/**	�������� ������� �������� ������� ���������� � ������� ������ ����������
 *	����� �� ����.
 * 
 *	����������� �������� ������� �� ������� push(), � ������ ��� �����������
 *	����������� ������� �������� �������, ���-�� ���� � ������ ������ ���� �� 
 *	����� ������������� ��������.
 * 
 */
void testProtocolModbus::testTick() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		TProtocolModbus::STATE startState;	// ��������� ��������� ���������
		TProtocolModbus::STATE stopState;	// �������� ��������� ���������
		uint8_t	 numBytes;	// ���-�� ���� ������ � ������
		uint16_t baudrate;	// �������� ������ �����
		uint8_t  period; // ������ ������ ������� tick()
		uint16_t numTicks; // ���-�� �����
		
	};

	sData data[] = {
		{TProtocolModbus::STATE_OFF, TProtocolModbus::STATE_OFF, 8,19200, 50, 50},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 3, 57600, 50, 35},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 4, 57600, 50, 35},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 5, 57600, 50, 35},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 6, 19200, 50, 41},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ, 8, 19200, 50, 40},
		{TProtocolModbus::STATE_READ, TProtocolModbus::STATE_READ_OK, 8, 19200, 50, 41},
		{TProtocolModbus::STATE_READ_ERROR, TProtocolModbus::STATE_READ, 8, 19200, 50, 41},
		{TProtocolModbus::STATE_READ_OK, TProtocolModbus::STATE_READ_OK, 8, 19200, 50, 50},
		{TProtocolModbus::STATE_WRITE_WAIT, TProtocolModbus::STATE_WRITE_WAIT, 8, 19200, 50, 50},
		{TProtocolModbus::STATE_WRITE, TProtocolModbus::STATE_WRITE, 8, 19200, 50, 50}
	};
	
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = tProtocolModbus.setTick(data[i].baudrate, data[i].period);
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);	// ��� ������ ��������
		tProtocolModbus.setState(data[i].startState);	// ������ ��� ����� ���������
		
		// ������� � ����� �������� ���-�� ���� ������
		for(uint8_t j = 0; j < data[i].numBytes; j++) {
			tProtocolModbus.push(0x00);
		}
		
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

void testProtocolModbus::testPrepareResponse() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		uint8_t numin;		// ���-�� ���� � �������
		uint8_t bufin[20];	// �������
		uint8_t numout;		// ���-�� ���� � �������������� �������
		uint8_t	bufout[50];	// ��������� ������
		TProtocolModbus::STATE state;
	};

	sData data[] = {
					// check command 0x01
		{8,	{0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA},
		 4, {0x01, 0x01, 0x01, 0x00}, TProtocolModbus::STATE_READ},
		{8,	{0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84},
		 8, {0x11, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}, TProtocolModbus::STATE_READ},

					// check command 0x03
		{8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA},
		 5, {0x01, 0x03, 0x02, 0x00, 0x00}, TProtocolModbus::STATE_READ},
		{8,	{0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39},
		 43,{0x01, 0x03, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		 TProtocolModbus::STATE_READ},

					// check command 0x05
		{8,	{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99},
		 8,	{0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}, TProtocolModbus::STATE_WRITE_READY},
		{8, {0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D},
		 8, {0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}, TProtocolModbus::STATE_WRITE_READY},	

					// check command 0x06
		{8, {0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B},
		 8, {0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}, TProtocolModbus::STATE_WRITE_READY},
		{8, {0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3},
		 8, {0x11, 0x06, 0x01, 0x10, 0xFF, 0xFF, 0x8A, 0xD3}, TProtocolModbus::STATE_WRITE_READY},

					// check command 0x0F
		{10,{0x11, 0x0F, 0x00, 0x13, 0x00, 0x01, 0x01, 0x00, 0xAA, 0x58},
		 6, {0x11, 0x0F, 0x00, 0x13, 0x00, 0x01}, TProtocolModbus::STATE_READ},
		{11,{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B},
		 6, {0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A}, TProtocolModbus::STATE_READ},

					// check command 0x10
		{11,{0x11, 0x10, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x0A, 0xEA, 0x46},
		 6, {0x11, 0x10, 0x00, 0x01, 0x00, 0x01}, TProtocolModbus::STATE_READ},
		{13,{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0},
		 6, {0x11, 0x10, 0x00, 0x01, 0x00, 0x02}, TProtocolModbus::STATE_READ},
			
				// check command 0x11
		{4, {0x03, 0x11, 0xC1, 0x4C},
		 2, {0x03, 0x11}, TProtocolModbus::STATE_READ}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		
		// ���������� ��������� ������
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numin; j++) {
			tProtocolModbus.push(data[i].bufin[j]);
		}
		
		// �������� ���������� �������� �������
		tProtocolModbus.setAddressLan(buf[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();
		if (TProtocolModbus::CHECK_ERR_NO != err){
			uint8_t cnt = sprintf(msg, "1.0 Error input data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n check_error = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}
		uint8_t num = tProtocolModbus.prepareResponse();
		
		// �������� ������ �������������� ��� ��������
		for(uint8_t j = 0; j < data[i].numout; j++) {
			if (buf[j] != data[i].bufout[j]) {
				uint8_t cnt = sprintf(msg, "1.1 Error in out data on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[%d] = 0x%X, need = 0x%X", j, buf[j], data[i].bufout[j]);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
		
		// �������� ��������� ����� ���������� ������
		TProtocolModbus::STATE state = tProtocolModbus.getState();
		if (state != data[i].state) {
			uint8_t cnt = sprintf(msg, "1.2 Error state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", state, data[i].state);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
		
		// �������� ���-�� ���� �������������� ��� ��������
		if (num != data[i].numout) {
			uint8_t cnt = sprintf(msg, "1.3 Error number of bytes on step %d", i);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].numout);
			CPPUNIT_ASSERT_MESSAGE(msg, false);
		}
	}
}


void testProtocolModbus::testSendRegister() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		bool state;			// ���� ���������� �������� (false - ������)
		uint16_t adr;		// ����� ��������
		uint16_t val;		// �������� ��������	
		uint8_t numin;		// ���-�� ���� � ������ �� ������
		uint8_t	bufin[50];	// ����� �� ������
		uint8_t numout;		// ���-�� ���� � ������ ����� ������
		uint8_t bufout[50];	// ����� ����� ������
		
	};

	sData data[] = {	{true, 0x0002, 0x1113, 
						8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA},
						5, {0x01, 0x03, 0x02, 0x11, 0x13}},
						{true, 0x0003, 0x1113, 
						8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x03, 0x54, 0x0B},
						9, {0x01, 0x03, 0x06, 0x00, 0x00, 0x11, 0x13, 0x00, 0x00}},
						{true, 0x0004, 0x1113, 
						8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x03, 0x54, 0x0B},
						9, {0x01, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x11, 0x13}},
						{false, 0x0003, 0x0013, 
						8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA},
						5, {0x01, 0x03, 0x02, 0x00, 0x00}}					
	};
		 
	for(uint8_t i = 0; i < (sizeof(data)/sizeof(data[0])); i++){
		// ���������� ��������� ������
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numin; j++) {
			tProtocolModbus.push(data[i].bufin[j]);
		}
		// �������� ���������� �������� �������
		tProtocolModbus.setAddressLan(data[i].bufin[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();
		if (TProtocolModbus::CHECK_ERR_NO != err){
			uint8_t cnt = sprintf(msg, "1.0 Error input data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n check_error = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}
		tProtocolModbus.prepareResponse();
		
		bool state = tProtocolModbus.sendRegister(data[i].adr, data[i].val);
		
		// �������� ����� ���������� �������
		if (state != data[i].state) {
			uint8_t cnt = sprintf(msg, "1.1 Error state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", state, data[i].state);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}
		
		// �������� �������������� ��� �������� ������
		for (uint8_t j = 0; j < data[i].numout; j++) {
			if (buf[j] != data[i].bufout[j]) {
				uint8_t cnt = sprintf(msg, "1.2 Error bufout on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[%d] = 0x%X, need = 0x%X", j, buf[j], data[i].bufout[j]);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
	} 
}

void testProtocolModbus::testSendCoil() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		bool state;			// ���� ���������� �������� (false - ������)
		uint16_t adr;		// ����� ��������
		bool val;			// �������� ��������	
		uint8_t numin;		// ���-�� ���� � ������ �� ������
		uint8_t	bufin[50];	// ����� �� ������
		uint8_t numout;		// ���-�� ���� � ������ ����� ������
		uint8_t bufout[50];	// ����� ����� ������
		
	};

	sData data[] = {	{true, 0x1301, true, 
						8, {0x01, 0x01, 0x13, 0x00, 0x00, 0x01, 0xF9, 0x4E},
						4, {0x01, 0x01, 0x01, 0x01}},
						{true, 0x0014, true, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x01, 0x00, 0x00}},
						{true, 0x0016, true, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x04, 0x00, 0x00}},
						{true, 0x0023, true, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x80, 0x00}},
						{true, 0x0026, true, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x04}},
						{false, 0x0027, false, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},
						{false, 0x0013, false, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},
						{false, 0x0003, 0x0013, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},					
	};
		 
	for(uint8_t i = 0; i < (sizeof(data)/sizeof(data[0])); i++){
		// ���������� ��������� ������
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numin; j++) {
			tProtocolModbus.push(data[i].bufin[j]);
		}
		// �������� ���������� �������� �������
		tProtocolModbus.setAddressLan(buf[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();
		if (TProtocolModbus::CHECK_ERR_NO != err){
			uint8_t cnt = sprintf(msg, "1.0 Error input data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n check_error = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}
		tProtocolModbus.prepareResponse();
		
		bool state = tProtocolModbus.sendCoil(data[i].adr, data[i].val);
		
		// �������� ����� ���������� �������
		if (state != data[i].state) {
			uint8_t cnt = sprintf(msg, "1.1 Error state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", state, data[i].state);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}

		// �������� ��� ������ ������� ������ ���� ��������
		for (uint8_t j = 0; j < data[i].numout; j++) {
			if (buf[j] != data[i].bufout[j]) {
				uint8_t cnt = sprintf(msg, "1.1 Error bufout on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[%d] = 0x%X, need = 0x%X", j, buf[j], data[i].bufout[j]);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
	} 
}

void testProtocolModbus::testSendCoils() {
	TProtocolModbus tProtocolModbus(buf, sizeof (buf));
	
	struct sData {
		bool state;			// ���� ���������� �������� (false - ������)
		uint16_t adr;		// ����� ��������
		uint8_t val;		// �������� ��������	
		uint8_t numin;		// ���-�� ���� � ������ �� ������
		uint8_t	bufin[50];	// ����� �� ������
		uint8_t numout;		// ���-�� ���� � ������ ����� ������
		uint8_t bufout[50];	// ����� ����� ������
		
	};

	sData data[] = {	{true, 0x1301, 0x01, 
						8, {0x01, 0x01, 0x13, 0x00, 0x00, 0x01, 0xF9, 0x4E},
						4, {0x01, 0x01, 0x01, 0x01}},
						{true, 0x0013, 0x02, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x01, 0x00, 0x00}},
						{true, 0x0014, 0x12, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x12, 0x00, 0x00}},
						{true, 0x0016, 0xFF, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0xFC, 0x03, 0x00}},
						{true, 0x0023, 0x19, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x80, 0x04}},
						{true, 0x0026, 0xFF, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x04}},
						{false, 0x0027, 0xFF, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},		
						{false, 0x0003, 0x0013, 
						8, {0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02},
						6, {0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},					
	};
		 
	for(uint8_t i = 0; i < (sizeof(data)/sizeof(data[0])); i++){
		// ���������� ��������� ������
		tProtocolModbus.setState(TProtocolModbus::STATE_READ);
		for (uint8_t j = 0; j < data[i].numin; j++) {
			tProtocolModbus.push(data[i].bufin[j]);
		}
		// �������� ���������� �������� �������
		tProtocolModbus.setAddressLan(buf[0]);
		TProtocolModbus::CHECK_ERR err = tProtocolModbus.checkReadPackage();
		if (TProtocolModbus::CHECK_ERR_NO != err){
			uint8_t cnt = sprintf(msg, "1.0 Error input data on step %d", i);
			cnt += sprintf(&msg[cnt], "\n check_error = %d", err);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}
		tProtocolModbus.prepareResponse();
		
		bool state = tProtocolModbus.sendCoils(data[i].adr, data[i].val);
		
		// �������� ����� ���������� �������
		if (state != data[i].state) {
			uint8_t cnt = sprintf(msg, "1.1 Error state on step %d", i);
			cnt += sprintf(&msg[cnt], "\n state = %d, need = %d", state, data[i].state);
			CPPUNIT_ASSERT_MESSAGE(msg, false);	
		}

		// �������� ��� ������ ������� ������ ���� ��������
		for (uint8_t j = 0; j < data[i].numout; j++) {
			if (buf[j] != data[i].bufout[j]) {
				uint8_t cnt = sprintf(msg, "1.1 Error bufout on step %d", i);
				cnt += sprintf(&msg[cnt], "\n buf[%d] = 0x%X, need = 0x%X", j, buf[j], data[i].bufout[j]);
				CPPUNIT_ASSERT_MESSAGE(msg, false);
			}
		}
	} 
}