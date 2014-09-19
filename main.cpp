/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "inc/debug.h"
#include "inc/glbDefine.h"
#include "inc/ks0108.h"
#include "inc/menu.h"
#include "inc/keyboard.h"
#include "inc/uart.h"
#include "inc/protocolPcS.h"
#include "inc/protocolBspS.h"
#include "inc/protocolPcM.h"

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 128

/// ������ ������ ��� ������� � ��
#define BUFF_SIZE_PC 128

/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 10

// ��������� �������� ��������� �� ���������������� ������
static bool uartRead();

// ������ �������� ��������� �� ���������������� ������
static bool uartWrite();

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

/// ������ ������������
#define EEPROM_START_ADDRESS 0x10

/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����
clMenu menu;
/// ����� ����������������� ����� ����������� � ��
TUart uartPC(TUart::PORT_UART1, uBufUartPc, BUFF_SIZE_PC);
/// ����c ����������������� ����� ����������� � ���
TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// ����� ������������ ��������� ����������� � ��
clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// ����� ������ ��������� MODBUS
TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ������������ ��������� ����������� � ���
clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);


static bool uartRead();
static bool uartWrite();
static void setInterface(eGB_INTERFACE val);


/**	������ � ��������� ������� �� UART
 *
 * 	@param ���
 * 	@return False - � ������ 5-� ������������ ��������� �� ��� ������
 */
static bool uartRead() {
	bool stat = true;
	// ��� ������������� � �� �������
	static uint8_t lastPcCom = 0;
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
			protBSPs.getData();

			// �������� ������������ ������� ����������� � �� � �������
			// ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
			if (lastPcCom == protBSPs.getCurrentCom()) {
				if (protPCs.isEnable()) {
					protPCs.copyCommandFrom(protBSPs.buf);
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
		if (cntLostCom < MAX_LOST_COM_FROM_BSP)
			cntLostCom++;
		else
			stat = false;
	}

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
					protBSPs.copyCommandFrom(protPCs.buf);
				}
			}
		}
	} else if (protPCm.isEnable()) {
		if (protPCm.isReadData()) {
			protPCm.readData();
		}
	}

	return stat;
}

/**	�������� ������ �� UART.
 *
 * 	@retval True - ������.
 */
static bool uartWrite() {

	if (protPCs.isEnable()) {
		// ����� ��������� �������� ������ ��������� �� ���������.
		protPCs.checkStat();
		// �������� ������������� �������� ������� �� �� � �� ��������
		ePRTS_STATUS stat = protPCs.getCurrentStatus();
		if (stat == PRTS_STATUS_WRITE_PC) {
			// ��������� ������ ���
			uartPC.trData(protPCs.trCom());
		} else if (stat == PRTS_STATUS_WRITE) {
			// �������� ������ ��
			uartPC.trData(protPCs.trCom());
		}
	} else if (protPCm.isEnable()) {
		uartPC.trData((protPCm.trResponse()));
	}

	// ����� ��������� �������� ������ ��������� �� ���������.
	protBSPs.checkStat();
	// �������� ��� �� ������������� �������� ������� � ��
	// ���� ���, �� ������� ������� � ����
	ePRTS_STATUS stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC) {
		// ��������� ������� ��
		uartBSP.trData(protBSPs.trCom());
	} else if (stat == PRTS_STATUS_NO) {
		// �������� ������� ���
		eGB_COM com = menu.getTxCommand();
		// ���� ���� �������, ���������� �� � ���
		if (com != GB_COM_NO) {
			uint8_t num = protBSPs.sendData(com);
			uartBSP.trData(num);
		}
	}

	return true;
}

/**	��������� ���������� ����� � �������.
 *
 * 	@param val ������� ���������
 * 	@return ���
 */
static void setInterface(eGB_INTERFACE val) {
	if (val == GB_INTERFACE_USB) {
		PORTD &= ~(1 << PD4);
	} else if (val == GB_INTERFACE_RS485) {
		PORTD |= (1 << PD4);
	}
}

/** �������� ������������� ����������������� �����.
 *
 * 	������������ ������� ���������, � ������������. � ���� ��� �� ���������
 * 	���������� ���������� ����� ��������.
 *
 * 	@param *current ������� ��������� �����.
 * 	@param *newparam ����������� ��������� �����.
 * 	@retval True, ���� ��������� ����������.
 * 	@retval False, ���� ��������� ���������.
 */
bool isUartPcReinit(sEeprom *current, TUartData *newparam) {
	bool stat = false;

	if (current->interface != newparam->Interface.get()) {
		current->interface = newparam->Interface.get();
		stat = true;
	}

	if (current->baudRate != newparam->BaudRate.get()) {
		current->baudRate = newparam->BaudRate.get();
		stat = true;
	}

	if (current->dataBits != newparam->DataBits.get()) {
		current->dataBits = newparam->DataBits.get();
		stat = true;
	}

	if (current->parity != newparam->Parity.get()) {
		current->parity = newparam->Parity.get();
		stat = true;
	}

	if (current->stopBits != newparam->StopBits.get()) {
		current->stopBits = newparam->StopBits.get();
		stat = true;
	}

	return stat;
}

void setProtocol(eGB_PROTOCOL protocol) {

	switch(protocol) {
		case GB_PROTOCOL_STANDART:
			protPCs.setEnable(PRTS_STATUS_READ);
			protPCm.setDisable();
			break;
		case GB_PROTOCOL_MODBUS:
			protPCm.setTick(19200, 50);
			protPCm.setAddressLan(1);
			protPCm.setEnable();
			protPCs.setDisable();
			break;
		case GB_PROTOCOL_IEC_101:
			// TODO ��� 101
			break;
		case GB_PROTOCOL_MAX:		// ��������
			break;
	}
}

/**	main.c
 *
 * 	@param ���
 * 	@return ���
 */
int __attribute__ ((OS_main))
main(void) {
	// ��������� �������� � ������
	sEeprom eeprom;

	// ������� ��� ���������� ���
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// ��������� �������� ������������, ��������� �� ������
	// ���������� �� ��������� ����������, ����� ������ �� ������
	eeprom_read_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS, sizeof(eeprom));
	EEAR = 0;	// ����� ������ ������ � 0, ��� ������ ������
	menu.sParam.password.init(eeprom.password);
	menu.sParam.Uart.Interface.set(eeprom.interface);
	menu.sParam.Uart.Protocol.set(eeprom.protocol);
	menu.sParam.Uart.BaudRate.set(eeprom.baudRate);
	menu.sParam.Uart.DataBits.set(eeprom.dataBits);
	menu.sParam.Uart.Parity.set(eeprom.parity);
	menu.sParam.Uart.StopBits.set(eeprom.stopBits);

	// ����� ���������� �����
	setInterface(menu.sParam.Uart.Interface.get());


	// ������ ����������������� ����� ��� ����� � ���
	// ��� ��������� �����������
	uartBSP.open(UART_BAUD_RATE_4800, UART_DATA_BITS_8, UART_PARITY_NONE,
			UART_STOP_BITS_TWO);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// ������ ����������������� ����� ��� ����� � ��/��������� ����.
	TUartData *uart = &menu.sParam.Uart;
	uartPC.open(uart->BaudRate.get(), uart->DataBits.get(), uart->Parity.get(),
			uart->StopBits.get());
	// ����� ������������ ��������� ������ � ������� �����
	setProtocol(uart->Protocol.get());

	sei();

	// ��������� ���
	vLCDinit();
	vLCDclear();

	// ������� ��� ��������
	// menu.setTypeDevice(AVANT_NO);

	while (1) {
		if (b100ms) {
			b100ms = false;

			cnt_wdt++;
			// ��������� �������� ��������� � ���/��
			// �������� � ���� �������� ���������� ����� � ���
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// ���������� ������
			// ��� 100 - ����� �������� �����
			if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// �������� ��������� � ���/��
			uartWrite();

			cnt_wdt++;
			// ������ ����������� ��� � 1�
			if (++cnt_1s >= 10) {
				cnt_1s = 0;

				//uint16_t password = menu.sParam.password.get();
				//eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);

				// ��������� �������� ���������� � ������ �
				// �������� ����������� �����
				// ���� ��� ���������� - ���� ��������������������
				// , � ����� ��������� ���������������� � ������ � ������
				if (isUartPcReinit(&eeprom, &menu.sParam.Uart)) {
					uartPC.close();
					TUartData *uart = &menu.sParam.Uart;

					// ���� ���� ����� � ��, �� ��������� �������������
					// ���� ���� ����� �� ���.����, �� ��������� ������������
					switch (uart->Interface.get()) {
					case GB_INTERFACE_USB:
						// �� ����� ������� ��������� USB ����� �����������
#ifndef DEBUG
						uartPC.open(UART_BAUD_RATE_19200, UART_DATA_BITS_8,
								UART_PARITY_NONE, UART_STOP_BITS_TWO);
#else
						uartPC.open(uart->BaudRate.get(), uart->DataBits.get(),
								uart->Parity.get(), uart->StopBits.get());
#endif
						protPCs.setEnable(PRTS_STATUS_READ);
						break;
					case GB_INTERFACE_RS485:
						uartPC.open(uart->BaudRate.get(), uart->DataBits.get(),
								uart->Parity.get(), uart->StopBits.get());
						protPCs.setEnable(PRTS_STATUS_READ);
						break;
					case GB_INTERFACE_MAX:		// ��������
						break;
					}

					// ��������� ���������� ����� � �������
					setInterface(menu.sParam.Uart.Interface.get());
				}

				// ���������� �������� ������ � ����� ������
				eeprom.password = menu.sParam.password.get();

				// �������� ��������� �����
				if (eeprom.protocol != menu.sParam.Uart.Protocol.get()) {
					eeprom.protocol = menu.sParam.Uart.Protocol.get();
					setProtocol(menu.sParam.Uart.Protocol.get());
				}

				// ���������� �������� ������������ � ������
				eeprom_update_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS,
						sizeof(eeprom));
				EEAR = 0;	// ����� ������ ������ � 0, ��� ������ ������
			}

			// ����� wdt, ���� ��� ���� ����� ���� ��������
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;
		}
	}
}

///	���������� �� ���������� ������0. ����������� ��� � 50 ���.
ISR(TIMER0_COMP_vect) {
	// ���������� ���
	vLCDmain();
	// ��������� ���
	vLCDled();

	// ��� ������ ��������� MODBUS
	if (protPCm.isEnable()) {
		protPCm.tick();
	}
}

///	���������� �� ���������� � ������1. ����������� ��� � 10 ��.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	vKEYmain();

	// ��������� ����� ��� � 100��
	cnt = (cnt > 0) ? cnt - 1 : (b100ms = true, 10 - 1);
}

///���������� �� ����������� ����������� ������ UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART1
ISR(USART1_TX_vect) {
	uartPC.isrTX();
	if (protPCs.isEnable()) {
		protPCs.setCurrentStatus(PRTS_STATUS_READ);
	} else if (protPCm.isEnable()) {
		protPCm.setState(TProtocolModbus::STATE_READ);
	}
}

/// ���������� �� ��������� ������ UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// � ������ ������ ����� �������� �������� ������
		// � �������� ������� ������ ���������
		protPCs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		if (protPCs.isEnable()) {
			// �������� "�����������"
			protPCs.checkByte(byte);
		} else if (protPCm.isEnable()) {
			// �������� MODBUS
			protPCm.push(byte);
		}
	}
}

///	���������� �� ����������� ����������� ������ UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_READ);
}

/// ���������� �� ��������� ������ UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	// � ������ ����������� ������
	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// � ������ ������ ����� �������� �������� ������
		// � �������� ������� ������ ���������
		protBSPs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		// ���������� ��������� "�����������"
		protBSPs.checkByte(byte);
	}
}
