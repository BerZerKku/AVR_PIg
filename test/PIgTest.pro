include(../PIg.pri)

QT += testlib         #Include QtTest to use SignalSpy, QTest::mouseClick, etc

TARGET = Tests
TEMPLATE = app

CONFIG += c++14
CONFIG += testcase
CONFIG -= debug_and_release
CONFIG += cmdline

DEFINES += \
    NDEBUG \
    MY_TESTS

INCLUDEPATH += \
    googletest \
    $${PIG_PATH}/src/ \
    $${PIG_PATH}/src/menu \
    $${PIG_PATH}/src/protocols/iec101 \
    $${PIG_PATH}/src/protocols/modbus

HEADERS += \

SOURCES += \
    googletest/gtest-all.cc \
    main.cpp \
    test_CIec101.cpp \
    test_protocolModbus.cpp \
    test_txCom.cpp
