/*
 * File:   testProtocolModbus.h
 * Author: Shcheblykin
 *
 * Created on Aug 22, 2014, 9:56:28 AM
 */

#ifndef TESTPROTOCOLMODBUS_H
#define	TESTPROTOCOLMODBUS_H 

#include <cppunit/extensions/HelperMacros.h>
#include "inc/protocolModbus.h"

class testProtocolModbus : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(testProtocolModbus);
	
	CPPUNIT_TEST(testConstant);
//	CPPUNIT_TEST(testGetMaxNumRegisters);
//	CPPUNIT_TEST(testGetMaxNumCoils);
//	CPPUINT_TEST(testGetAddressError);
//	CPPUNIT_TEST(testGetAddressMin);
//	CPPUNIT_TEST(testGetAddressMax);
	CPPUNIT_TEST(testStates);
//	CPPUNIT_TEST(testCheckState);
//	CPPUNIT_TEST(testGetState);
//	CPPUNIT_TEST(testIsEnable);
//	CPPUNIT_TEST(testIsReadData);
//	CPPUNIT_TEST(testSetDisable);
//	CPPUNIT_TEST(testSetEnable);
//	CPPUNIT_TEST(testSetState);
	CPPUNIT_TEST(testAddress);
	CPPUNIT_TEST(testGetStartAddress);
	CPPUNIT_TEST(testGetNumOfAddress);
	CPPUNIT_TEST(testSetTick);
	CPPUNIT_TEST(testTick);
	CPPUNIT_TEST(testPush);
	CPPUNIT_TEST(testGetCoil);
	CPPUNIT_TEST(testGetRegister);
	CPPUNIT_TEST(testSetException);
	CPPUNIT_TEST(testCheckReadPackage);
	CPPUNIT_TEST(testTrCom);
	CPPUNIT_TEST(testWriteRegister);
   

	CPPUNIT_TEST_SUITE_END();

public:
	testProtocolModbus();
	virtual ~testProtocolModbus();
	void setUp();
	void tearDown();

private:
	
	// проверка констант
	void testConstant();
//	void testGetMaxNumRegisters();
//	void testGetMaxNumCoils();
//	void testGetAddressError();
//	void testGetAddressMin();
//	void testGetAddressMax();
    // проверка нескольких методов статуса работы
	void testStates();
//	void testCheckState();
//	void testGetState();
//	void testIsEnable();
//	void testIsReadData();
//	void testSetDisable();
//	void testSetEnable();
//	void testSetState();
    // проверка установки/чтения текущего адреса
	void testAddress();
//	void testSetAddress();
//	void testGetAddress();
	// проверка правильности определения адреса и кол-ва адресов 
	void testGetStartAddress();
	void testGetNumOfAddress();
	//
	void testSetTick();
	void testTick(); 
	void testPush();
	void testGetCoil();
	void testGetRegister();
	void testSetException();
	void testCheckReadPackage();
	void testTrCom();
	//
	void testWriteRegister();
    
    // буфер данных Modbus
    uint8_t buf[128];
    
    // буфер сообщения об ошибке
    char msg[256]; 
};

#endif	/* TESTPROTOCOLMODBUS_H */

