#include "avr.h"
#include "base.hpp"

#include "protocols/standart/protocolBspS.h"
#include "protocols/standart/protocolPcS.h"
#include "protocols/modbus/protocolPcM.h"
#include "protocols/iec101/protocolPcI.h"

#include "drivers/ks0108.h"

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100



/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 10

/// адрес датчика температуры
#define TEMP_IC_ADR 0x48

/// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

/// Последняя команда полученная с ПК
static uint8_t lastPcCom = 0;
/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// Класс меню
clMenu menu;
/// Класс стандартного протокола работающего с ПК
static clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// Класс работы протокола MODBUS
static TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс протокола МЭК 101 работающего с ПК
static TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс стандартного протокола работающего с БСП
static clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);

static void setInterface(TUartData *uart);
static bool isUartPcReinit(TUartData *newparam);
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

void bspRead() {
    // кол-во неполученных сообщений с БСП
    static uint8_t cntLostCom = 0;

    // перед приемом проверим статус на залипание
    protBSPs.checkStat();
    // Проверка наличия сообщения с БСП и ее обработка
    if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
        // проверка контрольной суммы полученного сообщения и
        // обработка данных если она соответствует полученной
        if (protBSPs.checkReadData()) {
            // обработка принятого сообщения
            protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

            // проверка соответствия команды запрошенной с ПК и команды
            // полученной от БСП и если совпадают пересылка сообщения на ПК
            // для команды GB_COM_GET_VERS происходит добавление версии БСП-ПИ
            if (lastPcCom == protBSPs.getCurrentCom()) {
                if (protPCs.isEnable()) {
                    if (protPCs.copyCommandFrom(protBSPs.buf))
                        protPCs.modifyVersionCom();
                }
            }
        }

        // после принятия и обработки сообщения сбросим код предыдущей
        // запрашиваемой команды
        lastPcCom = 0;
        // сброс счетчика потерянных сообщений с БСП
        cntLostCom = 0;
    } else {
        // в случае превышения порога потерянных сообщений при обмене с БСП
        // флаг состояния сбрасывается в False
        if (cntLostCom < MAX_LOST_COM_FROM_BSP) {
            cntLostCom++;
        }
    }

    menu.setConnectionBsp(cntLostCom < MAX_LOST_COM_FROM_BSP);
}

void pcRead() {
    if (protPCs.isEnable()) {
        // перед приемом проверим статус на залипание
        protPCs.checkStat();
        // проверка наличия команды с ПК и ее обработка
        if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
            // проверка контрольной суммы полученного сообщения и
            // обработка данных если она соответствует полученной
            if (protPCs.checkReadData()) {
                // обработка принятого сообщения
                // если сообщение небыло обработано, перешлем его в БСП
                // (т.е. если это не запрос/изменение пароля)
                if (!protPCs.getData()) {
                    // сохранение запрашиваемой ПК команды
                    lastPcCom = protPCs.getCurrentCom();

                    // пересылка сообщения в БСП
                    if (protBSPs.getCurrentStatus() == PRTS_STATUS_NO) {
                        protBSPs.copyCommandFrom(protPCs.buf);
                        protPCs.setCurrentStatus(PRTS_STATUS_WAIT_ANSWER);
                    }
                }
            }
        }
    } else if (protPCm.isEnable()) {
        if (protPCm.isReadData()) {
            protPCm.readData();
        }
    } else if (protPCi.isEnable()) {
        if (protPCi.isReadData()) {
            protPCi.readData();
        }
    }
}

uint8_t pcWrite() {
    uint8_t num = 0;
    if (protPCs.isEnable()) {
        // Перед передачей проверим статус протокола на залипание.
        protPCs.checkStat();
        // проверка необходимости передачи команды на ПК и ее отправка
        ePRTS_STATUS stat = protPCs.getCurrentStatus();
        if (stat == PRTS_STATUS_WRITE_PC) {
            // пересылка ответа БСП
            num = protPCs.trCom();
        } else if (stat == PRTS_STATUS_WRITE) {
            // отправка ответа ПИ
            num = protPCs.trCom();
        }
    } else if (protPCm.isEnable()) {
        num = protPCm.sendData();
    } else if (protPCi.isEnable()) {
        num =protPCi.sendData();
    }

    return num;
}

uint8_t bspWrite() {
    uint8_t num = 0;

    // Перед передачей проверим статус протокола на залипание.
    protBSPs.checkStat();
    // проверим нет ли необходимости передачи команды с ПК
    // если нет, то возьмем команду с МЕНЮ
    ePRTS_STATUS stat = protBSPs.getCurrentStatus();
    if (stat == PRTS_STATUS_WRITE_PC) {
        // пересылка запроса ПК
        num = protBSPs.trCom();
    } else if (stat == PRTS_STATUS_NO) {
        // отправка запроса БСП
        eGB_COM com = menu.getTxCommand();
        // если есть команда, отправляем ее в БСП
        if (com != GB_COM_NO) {
            num = protBSPs.sendData(com);
        }
    }

    return num;
}

void setInterface(TUartData *uart) {
    TInterface::INTERFACE val = uart->Interface.get();

    switch (val) {
        case TInterface::USB:
            setProtocol(TProtocol::STANDART, 19200);
            break;
        case TInterface::RS485:
            setProtocol(uart->Protocol.get(), uart->BaudRate.getValue());
            break;
        case TInterface::MAX:		// заглушка
            break;
    }
}

bool isUartPcReinit(TUartData *newparam) {

	return false;
}


void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud) {
    protPCs.setDisable();
    protPCi.setDisable();
    protPCm.setDisable();

    switch(protocol) {
        case TProtocol::STANDART:
            protPCs.setEnable(PRTS_STATUS_NO);
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::MODBUS:
            protPCm.setTick(baud, 50);
            protPCm.setEnable();
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::IEC_101:
            protPCi.setTick(baud, 50);
            protPCi.setEnable();
            menu.sParam.jrnScada.setReadyToRead();
            break;
        case TProtocol::MAX:		// заглушка
            break;
    }
}

void mainInit() {
    protBSPs.setEnable(PRTS_STATUS_NO);
    setInterface(&menu.sParam.Uart);
    vLCDinit();
    vLCDclear();
}

void mainCycle(void) {
    static uint8_t cnt_lcd = 0;
    static uint8_t cnt_1s = 0;

    if (++cnt_1s >= 10) {
        cnt_1s = 0;


        // TODO Добавить смену интерфейса и протокола на железе
        if (isUartPcReinit(&menu.sParam.Uart)) {
            setInterface(&menu.sParam.Uart);
        }

        // TODO Перенести установку сетевого адреса в другое место
        uint8_t nadr = menu.sParam.glb.getNetAddress();
        if (protPCm.getAddressLan() != nadr) {
            protPCm.setAddressLan(nadr);
        } else if (protPCi.getAddressLan() != nadr) {
            protPCi.setAddressLan(nadr);
        }
    }

    if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
        cnt_lcd = 0;
        menu.proc();
    }
}

/// Прерывание по окончанию передачи данных UART1
void pcTxEnd() {
    if (protPCs.isEnable()) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else if (protPCm.isEnable()) {
        protPCm.setReadState();
    } else if (protPCi.isEnable()) {
        protPCi.setReadState();
    }
}


void pcPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        if (protPCs.isEnable()) {
            // протокол "Стандартный"
            protPCs.checkByte(byte);
        } else if (protPCm.isEnable()) {
            // протокол MODBUS
            protPCm.push(byte);
        } else if (protPCi.isEnable()) {
            protPCi.push(byte);
        }
    }
}

void bspTxEnd() {
    protBSPs.setCurrentStatus(PRTS_STATUS_NO);
}

void bspPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protBSPs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        protBSPs.checkByte(byte);
    }
}

void timer50us() {
	if (protPCm.isEnable()) {
		protPCm.tick();
	} else if (protPCi.isEnable()) {
		protPCi.tick();
	}
}


