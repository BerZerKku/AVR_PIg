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
    
    
    CPPUNIT_TEST(testStates);
//    CPPUNIT_TEST(testCheckState);
//    CPPUNIT_TEST(testGetState);
//    CPPUNIT_TEST(testIsEnable);
//    CPPUNIT_TEST(testIsReadData);
//    CPPUNIT_TEST(testSetDisable);
//    CPPUNIT_TEST(testSetEnable);
//    CPPUNIT_TEST(testSetState);
    CPPUNIT_TEST(testAddress); 
    CPPUNIT_TEST(testCheckReadPackage);
    CPPUNIT_TEST(testPush);
    
    CPPUNIT_TEST(testReadData);
    CPPUNIT_TEST(testWriteData);
    CPPUNIT_TEST(testGetMaxNumRegisters);
    CPPUNIT_TEST(testGetMinNumRegisters);
    CPPUNIT_TEST(testTick);
    CPPUNIT_TEST(testTrCom);
   

    CPPUNIT_TEST_SUITE_END();

public:
    testProtocolModbus();
    virtual ~testProtocolModbus();
    void setUp();
    void tearDown();

private:
 
    
    void testReadData();
    void testWriteData();
    void testGetMaxNumRegisters();
    void testGetMinNumRegisters();
    void testTick();
    void testTrCom();
    
    // тест сразу нескольких методов статуса работы
    void testStates();
//    void testCheckState();
//    void testGetState();
//    void testIsEnable();
//    void testIsReadData();
//    void testSetDisable();
//    void testSetEnable();
//    void testSetState();
    
    // проверка установки/чтения текущего адреса
    void testAddress();
//    void testSetAddress();
//    void testGetAddress();
    
    void testCheckReadPackage();
    void testPush();
    
    
    // буфер данных Modbus
    uint8_t buf[128];
    
    // буфер сообщения об ошибке
    char msg[256];
    
    
};

#endif	/* TESTPROTOCOLMODBUS_H */

