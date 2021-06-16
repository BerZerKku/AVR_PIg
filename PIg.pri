#PIG_PATH = $$system(pwd)/PIg
PIG_PATH = $$system(pwd)

INCLUDEPATH += \
    $$system(pwd) \
    $${PIG_PATH}

HEADERS += \
    $${PIG_PATH}/src/avr.h \
    $${PIG_PATH}/src/flash.h \
    $${PIG_PATH}/src/flashParams.h \
    $${PIG_PATH}/src/glbDefine.h \
    $${PIG_PATH}/src/menu/dateTime.h \
    $${PIG_PATH}/src/menu/password.h \
    $${PIG_PATH}/src/menu/test.h \
    $${PIG_PATH}/src/menu/txCom.h \
    $${PIG_PATH}/src/protocols/iec101/CIec101.h \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.h

SOURCES += \
    $${PIG_PATH}/src/menu/test.cpp \
    $${PIG_PATH}/src/menu/dateTime.cpp \
    $${PIG_PATH}/src/menu/txCom.cpp \    
    $${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp
