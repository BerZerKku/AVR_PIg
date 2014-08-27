/*
 * File:   testProtocolModbus.cpp
 * Author: Shcheblykin
 *
 * Created on Aug 22, 2014, 9:56:28 AM
 */

#include "testProtocolModbus.h"


CPPUNIT_TEST_SUITE_REGISTRATION(testProtocolModbus);

testProtocolModbus::testProtocolModbus()
{
}

testProtocolModbus::~testProtocolModbus()
{
}

void testProtocolModbus::setUp()
{
}

void testProtocolModbus::tearDown()
{
}

/** ������������ ���������/������ ������ ����������.
 * 
 */
void testProtocolModbus::testAddress()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    // 1. �������� ������ ��-���������.
    if (tProtocolModbus.getAddress() != tProtocolModbus.ADDRESS_ERR) {
        sprintf(msg, "1.1 Address by default is not ADDRESS_ERR.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    // 2. �������� ���������/���������� ������.
    for (uint16_t i = 0; i <= 255; i++) {

        bool t = tProtocolModbus.setAddress(i);

        // �������� ������������� �������� �������� serAddress())
        if (((i < tProtocolModbus.ADDRESS_MIN) || (i > tProtocolModbus.ADDRESS_MAX)) == t) {
            sprintf(msg, "2.1 Function setAddress() returned error state on step %d", i);
            CPPUNIT_ASSERT_MESSAGE(msg, false);
        }

        // �������� �������������� ��������
        if (i < tProtocolModbus.ADDRESS_MIN) {
            if (tProtocolModbus.getAddress() != tProtocolModbus.ADDRESS_ERR) {
                sprintf(msg, "2.2 Set Address less ADDRESS_MIN get not ADDRESS_ERR on step %d", i);
                CPPUNIT_ASSERT_MESSAGE(msg, false);
            }
        } else if (i > tProtocolModbus.ADDRESS_MAX) {
            if (tProtocolModbus.getAddress() != tProtocolModbus.ADDRESS_MAX) {
                sprintf(msg, "2.3 Set Address greater ADDRESS_MAX get not ADDRESS_MAX on step %d", i);
                CPPUNIT_ASSERT_MESSAGE(msg, false);
            }
        } else if (i != tProtocolModbus.getAddress()) {
            sprintf(msg, "2.4 Set Address not equivalent get Address on step %d", i);
            CPPUNIT_ASSERT_MESSAGE(msg, false);
        }
    }
}

/** ������������ ������� �������� ��������� ������.
 * 
 */
void testProtocolModbus::testCheckReadPackage()
{
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
    for(uint8_t i = 0; i < sizeof(buf1); i++) {
        tProtocolModbus.push(buf1[i]);
    }
    
    // 1. �������� ����������� ����������� ������ ����������
    
    if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
        sprintf(msg, "1.1 A positive result in default address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    tProtocolModbus.setState(TProtocolModbus::STATE_READ);
    for(uint8_t i = 0; i < sizeof(buf1); i++) {
        tProtocolModbus.push(buf1[i]);
    }
    
    tProtocolModbus.setAddress(buf[0] + 2);
    if (tProtocolModbus.checkReadPackage() != TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
        sprintf(msg, "1.2 A positive result in incorrect address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    tProtocolModbus.setState(TProtocolModbus::STATE_READ);
    for(uint8_t i = 0; i < sizeof(buf1); i++) {
        tProtocolModbus.push(buf1[i]);
    }
    
    tProtocolModbus.setAddress(buf[0]);
    if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_ADR_DEVICE) {
        printf(msg, "1.3 A negative result in correct address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    // 2. �������� ����������� �����.
    tProtocolModbus.setState(TProtocolModbus::STATE_READ);
    for(uint8_t i = 0; i < sizeof(buf1); i++) {
        tProtocolModbus.push(buf1[i]);
    }
    tProtocolModbus.setAddress(buf[0]);
    
    if (tProtocolModbus.checkReadPackage() == TProtocolModbus::CHECK_ERR_CRC) {
        uint8_t cnt = sprintf(msg, "2.1 A negative check crc for correct data.");
//        cnt += sprintf(&msg[cnt], "\n calccrc = 0x%X", tProtocolModbus.calcCRC(sizeof(buf[1]) - 2));
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    tProtocolModbus.setState(TProtocolModbus::STATE_READ);
    for(uint8_t i = 0; i < sizeof(buf2); i++) {
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
    
    // 3. �������� ����������� ����������� �������, � ����� ������������ ������.
    // first byte - number of bytes in package
    struct sData {
        TProtocolModbus::CHECK_ERR err; // ������ 
        uint8_t num;                    // ���-�� ���� � �������
        uint8_t buf[14];                // �������
        
    };
    
    sData data[] = {
        
        // check command 0x01
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA}},               // ���-�� ������� �������, 1   
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84}},               // ���-�� ������� - � ���������
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x11, 0x01, 0x00, 0x13, 0x01, 0x00, 0xCE, 0xCF}},               // ���-�� ������� ��������, 256
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0A}},    // ���-�� ������� 0
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0xFC, 0x5A}},    // ���-�� ������� 257
    
        // check command 0x03
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}},               // ���-�� ������� �������, 1
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x01, 0x03, 0x01, 0x00, 0x00, 0x14, 0x44, 0x39}},               // ���-�� ������� - � ���������
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x01, 0x03, 0x03, 0x30, 0x00, 0x20, 0x44, 0x59}},               // ���-�� ������� ��������, 32
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x01, 0x03, 0x00, 0x01, 0x00, 0x00, 0x14, 0x0A}},    // ���-�� ������� 0
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x01, 0x03, 0x00, 0x02, 0x00, 0x21, 0x24, 0x12}},    // ���-�� ������� 33
                
        // check command 0x05
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x13, 0x05, 0x00, 0xAC, 0x00, 0x00, 0x0E, 0x99}},               // �������� 0x0000 = 0
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x27, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4B, 0x1D}},               // �������� 0xFF00 = 1
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x13, 0x05, 0x00, 0xAC, 0x00, 0x01, 0xCF, 0x59}},   
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x13, 0x05, 0x00, 0xAC, 0x00, 0xFF, 0x0E, 0x99}}, 
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x13, 0x05, 0x00, 0xAC, 0x01, 0x00, 0x0E, 0x99}},
        {TProtocolModbus::CHECK_ERR_FUNCTION_DATA, 8, {0x13, 0x05, 0x00, 0xAC, 0xFF, 0xFF, 0x0E, 0x99}}, 
         
        
        {TProtocolModbus::CHECK_ERR_NO, 8, {0x11, 0x06, 0x00, 0x01, 0x00, 0x03, 0x9A, 0x9B}},
        {TProtocolModbus::CHECK_ERR_NO, 11,{0x11, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0xBF, 0x0B}},
        {TProtocolModbus::CHECK_ERR_NO, 13,{0x11, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0xC6, 0xF0}},
        {TProtocolModbus::CHECK_ERR_NO, 4, {0x03, 0x11, 0xC1, 0x4C}},
        
        // check incorrect function
        {TProtocolModbus::CHECK_ERR_FUNCTION, 4, {0x11, 0x04, 0x0C, 0x23}},                                        
        {TProtocolModbus::CHECK_ERR_FUNCTION, 8, {0x03, 0x07, 0x00, 0x01, 0x00, 0x01, 0x25, 0xE8}}
    };


    for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
        tProtocolModbus.setState(TProtocolModbus::STATE_READ);
        for (uint8_t j = 0; j <= data[i].num; j++) {
            tProtocolModbus.push(data[i].buf[j]);
        }
        tProtocolModbus.setAddress(buf[0]);
  
        TProtocolModbus::CHECK_ERR check = tProtocolModbus.checkReadPackage();
        if (check != data[i].err) {
            uint8_t cnt = sprintf(msg, "3.1 Error check function or function data on step %d", i);
            cnt += sprintf(&msg[cnt], "\n com = 0x%X", data[i].buf[1]);
            cnt += sprintf(&msg[cnt], "\n check = %d, need = %d", check, data[i].err);
            cnt += sprintf(&msg[cnt], "\n startadr = 0x%X", tProtocolModbus.getStartAddress());
            cnt += sprintf(&msg[cnt], "\n numofadr = 0x%X", tProtocolModbus.getNumOfAddress());
            uint16_t crc = tProtocolModbus.calcCRC(data[i].num - 2);
            cnt += sprintf(&msg[cnt], "\n calccrc = 0x%X", crc);
            CPPUNIT_ASSERT_MESSAGE(msg, false);
        }
    }
}

void testProtocolModbus::testReadData()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    uint16_t num;
    uint16_t result = tProtocolModbus.readData(num);
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void testProtocolModbus::testWriteData()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    uint16_t num;
    uint16_t val;
    bool result = tProtocolModbus.writeData(num, val);
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void testProtocolModbus::testPush()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    tProtocolModbus.setState(TProtocolModbus::STATE_READ);

    // 1. �������� ������ ����������� ���������� ���-�� ���� ������
    // ��� �������� ������������ tick 
    for(uint16_t i = 1; i <= 2*sizeof(buf); i++) {
        uint16_t t = tProtocolModbus.push(i);
        if (i <= sizeof (buf)){
            if (t != i) {
                sprintf(msg, "1.1 Error correct writing to the buffer on step %d", i);
                CPPUNIT_ASSERT_MESSAGE(msg, false);
            }
        } else {
            if (t != sizeof (buf)) {
                sprintf(msg, "1.1 Error incorrect writing to the buffer on step %d", i);
                CPPUNIT_ASSERT_MESSAGE(msg, false);
            }
        }
    }
    
    // 2. �������� ������ ������� ��� ����� 1.5 ���������.
    // TODO �������� ������ ������� ��� ����� 1.5 ���������.
    
}

void testProtocolModbus::testTick()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    tProtocolModbus.tick();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void testProtocolModbus::testTrCom()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    uint8_t result = tProtocolModbus.trCom();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
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
void testProtocolModbus::testStates()
{
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

void testProtocolModbus::testGetStartAddress()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));
    
    struct sData {
        uint16_t adr;
        uint8_t buf[9]; 
    };
    
    sData data[] = {
        {0,     {0x01, 0x01, 0x00, 0x00, 0x00, 0x01}},   
        {19,    {0x11, 0x01, 0x00, 0x13, 0x00, 0x25}},
        {0,     {0x01, 0x03, 0x00, 0x00, 0x00, 0x07}},
        {1027,  {0x11, 0x03, 0x04, 0x03, 0x02, 0x05}}
    };
    
    for(uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
        tProtocolModbus.setState(TProtocolModbus::STATE_READ);
        for (uint8_t j = 0; j <= sizeof(data[i].buf); j++) {
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

void testProtocolModbus::testGetNumOfAddress()
{
   TProtocolModbus tProtocolModbus(buf, sizeof (buf));
    
    struct sData {
        uint16_t num;
        uint8_t buf[9]; 
    };
    
    sData data[] = {
        {1,     {0x01, 0x01, 0x00, 0x00, 0x00, 0x01}},   
        {37,    {0x11, 0x01, 0x00, 0x13, 0x00, 0x25}},
        {7,     {0x01, 0x03, 0x00, 0x00, 0x00, 0x07}},
        {1535,  {0x11, 0x03, 0x04, 0x00, 0x05, 0xFF}}
    };
    
    for(uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
        tProtocolModbus.setState(TProtocolModbus::STATE_READ);
        for (uint8_t j = 0; j <= sizeof(data[i].buf); j++) {
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