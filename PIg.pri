#PIG_PATH = $$system(pwd)/PIg
PIG_PATH = $$system(pwd)

INCLUDEPATH += \
    $$system(pwd) \
    $${PIG_PATH}

HEADERS += \
    $${PIG_PATH}/src/avr.h \
    $${PIG_PATH}/src/glbDefine.h \
    $${PIG_PATH}/src/menu/txCom.h \
    $${PIG_PATH}/src/protocols/iec101/CIec101.h \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.h

SOURCES += \
    $${PIG_PATH}/src/menu/txCom.cpp \
    $${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp
