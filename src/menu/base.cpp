#include "avr.h"
#include "base.hpp"

#include "protocols/standart/protocolBspS.h"
#include "protocols/standart/protocolPcS.h"
#include "protocols/modbus/protocolPcM.h"
#include "protocols/iec101/protocolPcI.h"

#include "drivers/ks0108.h"

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100



/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 10

/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

/// ��������� ������� ���������� � ��
static uint8_t lastPcCom = 0;
/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����
clMenu menu;
/// ����� ������������ ��������� ����������� � ��
static clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// ����� ������ ��������� MODBUS
static TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ��������� ��� 101 ����������� � ��
static TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ������������ ��������� ����������� � ���
static clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);

static void setInterface(TUartData *uart);
static bool isUartPcReinit(TUartData *newparam);
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

void bspRead() {
    // ���-�� ������������ ��������� � ���
    static uint8_t cntLostCom = 0;

    // ����� ������� �������� ������ �� ���������
    protBSPs.checkStat();
    // �������� ������� ��������� � ��� � �� ���������
    if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
        // �������� ����������� ����� ����������� ��������� �
        // ��������� ������ ���� ��� ������������� ����������
        if (protBSPs.checkReadData()) {
            // ��������� ��������� ���������
            protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

            // �������� ������������ ������� ����������� � �� � �������
            // ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
            // ��� ������� GB_COM_GET_VERS ���������� ���������� ������ ���-��
            if (lastPcCom == protBSPs.getCurrentCom()) {
                if (protPCs.isEnable()) {
                    if (protPCs.copyCommandFrom(protBSPs.buf))
                        protPCs.modifyVersionCom();
                }
            }
        }

        // ����� �������� � ��������� ��������� ������� ��� ����������
        // ������������� �������
        lastPcCom = 0;
        // ����� �������� ���������� ��������� � ���
        cntLostCom = 0;
    } else {
        // � ������ ���������� ������ ���������� ��������� ��� ������ � ���
        // ���� ��������� ������������ � False
        if (cntLostCom < MAX_LOST_COM_FROM_BSP) {
            cntLostCom++;
        }
    }

    menu.setConnectionBsp(cntLostCom < MAX_LOST_COM_FROM_BSP);
}

void pcRead() {
    if (protPCs.isEnable()) {
        // ����� ������� �������� ������ �� ���������
        protPCs.checkStat();
        // �������� ������� ������� � �� � �� ���������
        if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
            // �������� ����������� ����� ����������� ��������� �
            // ��������� ������ ���� ��� ������������� ����������
            if (protPCs.checkReadData()) {
                // ��������� ��������� ���������
                // ���� ��������� ������ ����������, �������� ��� � ���
                // (�.�. ���� ��� �� ������/��������� ������)
                if (!protPCs.getData()) {
                    // ���������� ������������� �� �������
                    lastPcCom = protPCs.getCurrentCom();

                    // ��������� ��������� � ���
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
        // ����� ��������� �������� ������ ��������� �� ���������.
        protPCs.checkStat();
        // �������� ������������� �������� ������� �� �� � �� ��������
        ePRTS_STATUS stat = protPCs.getCurrentStatus();
        if (stat == PRTS_STATUS_WRITE_PC) {
            // ��������� ������ ���
            num = protPCs.trCom();
        } else if (stat == PRTS_STATUS_WRITE) {
            // �������� ������ ��
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

    // ����� ��������� �������� ������ ��������� �� ���������.
    protBSPs.checkStat();
    // �������� ��� �� ������������� �������� ������� � ��
    // ���� ���, �� ������� ������� � ����
    ePRTS_STATUS stat = protBSPs.getCurrentStatus();
    if (stat == PRTS_STATUS_WRITE_PC) {
        // ��������� ������� ��
        num = protBSPs.trCom();
    } else if (stat == PRTS_STATUS_NO) {
        // �������� ������� ���
        eGB_COM com = menu.getTxCommand();
        // ���� ���� �������, ���������� �� � ���
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
        case TInterface::MAX:		// ��������
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
        case TProtocol::MAX:		// ��������
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


        // TODO �������� ����� ���������� � ��������� �� ������
        if (isUartPcReinit(&menu.sParam.Uart)) {
            setInterface(&menu.sParam.Uart);
        }

        // TODO ��������� ��������� �������� ������ � ������ �����
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

/// ���������� �� ��������� �������� ������ UART1
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
            // �������� "�����������"
            protPCs.checkByte(byte);
        } else if (protPCm.isEnable()) {
            // �������� MODBUS
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


