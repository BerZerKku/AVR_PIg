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

/** Тестирование установки/чтения адреса устройства.
 * 
 */
void testProtocolModbus::testAddress()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    // 1. Проверка адреса по-умолчанию.
    if (tProtocolModbus.getAddress() != tProtocolModbus.ADDRESS_ERR) {
        sprintf(msg, "1.1 Address by default is not ADDRESS_ERR.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    // 2. Проверка установки/считывания адреса.
    for (uint16_t i = 0; i <= 255; i++) {

        bool t = tProtocolModbus.setAddress(i);

        // проверка возвращаемого значения функцией serAddress())
        if (((i < tProtocolModbus.ADDRESS_MIN) || (i > tProtocolModbus.ADDRESS_MAX)) == t) {
            sprintf(msg, "2.1 Function setAddress() returned error state on step %d", i);
            CPPUNIT_ASSERT_MESSAGE(msg, false);
        }

        // проверка установленного значения
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

/** Тестирование функции проверки принятого пакета.
 * 
 */
void testProtocolModbus::testCheckReadPackage()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));
    
    
    buf[0] = 0x01;
    buf[1] = tProtocolModbus.COM_01H_READ_COIL;
    buf[2] = 0x00; buf[3] = 0x00;   // adr hi/low
    buf[4] = 0x00; buf[5] = 0x01;   // num hi/low
    buf[6] = 0xFD; buf[7] = 0xCA;   // crc hi/low
    
    sprintf(msg, "0x%X", tProtocolModbus.getCRC());
    CPPUNIT_ASSERT_MESSAGE(msg, false);
    
    
    // 1. Проверка правильного определения адреса устройства
    // Вызванная ошибка может быть из-за проблем в других проверках
    // и для выявления проблемы их можно просто заккоментировать в исходнике

    if (tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "1.1 A positive result in default address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    tProtocolModbus.setAddress(buf[0] + 2);
    if (tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "1.2 A positive result in incorrect address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    tProtocolModbus.setAddress(buf[0]);
    if (!tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "1.3 A negative result in correct address.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    // 2. Проверка контрольной суммы.
    // Вызванная ошибка может быить из-за проблем в других проверках
    // и для выявления проблемы их можно просто заккоментировать в исходнике
    
    buf[0] = 0x01;
    tProtocolModbus.setAddress(buf[0]);
    buf[1] = TProtocolModbus::COM_01H_READ_COIL;
    buf[2] = 0x00; buf[3] = 0x00;   // adr hi/low
    buf[4] = 0x00; buf[5] = 0x01;   // num hi/low
    buf[6] = 0xFD; buf[7] = 0xCA;   // crc hi/low
    if (!tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "2.1 A negative check crc for correct data.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    buf[0] = 0x01;
    tProtocolModbus.setAddress(buf[0]);
    buf[1] = TProtocolModbus::COM_06H_WRITE_SINGLE_REGISTER;
    buf[2] = 0x80; buf[3] = 0x12;   // adr hi/low
    buf[4] = 0x00; buf[5] = 0x13;   // num hi/low
    buf[6] = 0x41; buf[7] = 0xC2;   // crc hi/low
    if (!tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "2.2 A negative check crc for correct data.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    buf[2] += 1;
    if (tProtocolModbus.checkReadPackage()) {
        sprintf(msg, "2.3 A positive check crc for incorrect data.");
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
    
    // 3. Проверка правильного определения команды.
    // Вызванная ошибка может быить из-за проблем в других проверках
    // и для выявления проблемы их можно просто заккоментировать в исходнике

    buf[0] = 0x13;
    tProtocolModbus.setAddress(buf[0]);

    for (uint16_t i = 0; i <= 255; i++) {
        buf[1] = i;

        bool iscom = false;
        TProtocolModbus::COM com = static_cast<TProtocolModbus::COM> (i);
        switch(com) {
            case TProtocolModbus::COM_01H_READ_COIL:
                iscom = true;
                break;
            case TProtocolModbus::COM_03H_READ_HOLDING_REGISTER:
                iscom = true;
                break;
            case TProtocolModbus::COM_05H_WRITE_SINGLE_COIL:
                iscom = true;
                break;
            case TProtocolModbus::COM_06H_WRITE_SINGLE_REGISTER:
                iscom = true;
                break;
            case TProtocolModbus::COM_0EH_READ_DEVICE_INFORMATION:
                iscom = true;
                break;
            case TProtocolModbus::COM_0FH_WRITE_MULTIPLIE_COILS:
                iscom = true;
                break;
            case TProtocolModbus::COM_10H_WRITE_MULITPLIE_REGISTERS:
                iscom = true;
                break;
            case TProtocolModbus::COM_2BH_READ_DEVICE_INDENTIFICATION:
                iscom = true;
                break;
        }

        if (iscom != tProtocolModbus.checkReadPackage()) {
            sprintf(msg, "3.1 A positive result for an unsupported command or vice versa on step %d", i);
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

void testProtocolModbus::testGetMaxNumRegisters()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    uint16_t result = tProtocolModbus.getMaxNumRegisters();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void testProtocolModbus::testGetMinNumRegisters()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    uint16_t result = tProtocolModbus.getMinNumRegisters();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void testProtocolModbus::testPush()
{
    TProtocolModbus tProtocolModbus(buf, sizeof (buf));

    tProtocolModbus.setState(TProtocolModbus::STATE_READ);

    uint8_t byte;
    tProtocolModbus.push(byte);
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
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
void testProtocolModbus::testStates()
{
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
