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
	CPPUNIT_TEST(testAddressLan);
	CPPUNIT_TEST(testSetTick);
	CPPUNIT_TEST(testTick);
	CPPUNIT_TEST(testPush);
//	CPPUNIT_TEST(testSetException);
	CPPUNIT_TEST(testCheckReadPackage);
	CPPUNIT_TEST(testGetStartAddress);
	CPPUNIT_TEST(testGetNumOfAddress);
	CPPUNIT_TEST(testGetCoil);
	CPPUNIT_TEST(testGetRegister);
	CPPUNIT_TEST(testPrepareResponse);
	CPPUNIT_TEST(testTrResponse);
	CPPUNIT_TEST(testSendRegister);
	CPPUNIT_TEST(testSendCoil);
	CPPUNIT_TEST(testSendCoils);
   
	CPPUNIT_TEST_SUITE_END();

public:
	testProtocolModbus();
	virtual ~testProtocolModbus();
	void setUp();
	void tearDown();

private:
	
	// �������� ��������
	void testConstant();
//	void testGetMaxNumRegisters();
//	void testGetMaxNumCoils();
//	void testGetAddressError();
//	void testGetAddressMin();
//	void testGetAddressMax();
    // �������� ���������� ������� ������� ������
	void testStates();
//	void testCheckState();
//	void testGetState();
//	void testIsEnable();
//	void testIsReadData();
//	void testSetDisable();
//	void testSetEnable();
//	void testSetState();
    // �������� ���������/������ �������� ������
	void testAddressLan();
//	void testSetAddress();
//	void testGetAddress();
	//
	void testSetTick();
	void testTick(); 
	void testPush();
//	void testSetException();
	void testCheckReadPackage();
	void testTrResponse();
	//
	void testGetStartAddress();
	void testGetNumOfAddress();
	void testGetCoil();
	void testGetRegister();
	void testPrepareResponse();
	void testSendRegister();
	void testSendCoil();
	void testSendCoils();
    void testTrRespose();
	
    // ����� ������ Modbus
    uint8_t buf[128];
    
    // ����� ��������� �� ������
    char msg[256]; 
};

#endif	/* TESTPROTOCOLMODBUS_H */

