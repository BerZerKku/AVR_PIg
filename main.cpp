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

/// ������ ���������� ������ * 100 ��
#define LCD_REFRESH_PERIOD 2

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
#define EEPROM_PASSWORD 0x10

/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����
clMenu menu;
/// ����� ����������������� ����� ����������� � ��
clUart 		uartPC	(UART_UART1, uBufUartPc, BUFF_SIZE_PC);
/// ����c ����������������� ����� ����������� � ���
clUart 		uartBSP	(UART_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// ����� ������������ ��������� ����������� � ��
clProtocolPcS	protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// ����� ������������ ��������� ����������� � ���
clProtocolBspS	protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);


/**	������ � ��������� ������� �� UART
 * 	@param ���
 * 	@return False - � ������ 5-� ������������ ��������� �� ��� ������
 */
static bool uartRead()
{
	bool stat = true;
	// ��� ������������� � �� �������
	static uint8_t lastPcCom = 0;
	// ���-�� ������������ ��������� � ���
	static uint8_t cntLostCom = 0;

	// ����� ������� �������� ������ �� ���������
	protBSPs.checkStat();
	// �������� ������� ��������� � ��� � �� ���������
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK)
	{
		// �������� ����������� ����� ����������� ��������� �
		// ��������� ������ ���� ��� ������������� ����������
		if (protBSPs.checkReadData())
		{
			// ��������� ��������� ���������
			protBSPs.getData();

			// �������� ������������ ������� ����������� � �� � �������
			// ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
			if (lastPcCom == protBSPs.getCurrentCom())
			{
				protPCs.copyCommandFrom(protBSPs.buf);
			}
		}

		// ����� �������� � ��������� ��������� ������� ��� ����������
		// ������������� �������
		lastPcCom = 0;
		// ����� �������� ���������� ��������� � ���
		cntLostCom = 0;
	}
	else
	{
		// � ������ ���������� ������ ���������� ��������� ��� ������ � ���
		// ���� ��������� ������������ � False
		if (cntLostCom < MAX_LOST_COM_FROM_BSP)
			cntLostCom++;
		else
			stat = false;
	}

	// ����� ������� �������� ������ �� ���������
	protPCs.checkStat();
	// �������� ������� ������� � �� � �� ���������
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK)
	{
		// �������� ����������� ����� ����������� ��������� �
		// ��������� ������ ���� ��� ������������� ����������
		if (protPCs.checkReadData())
		{
			// ��������� ��������� ���������
			// ���� ��������� ������ ����������, �������� ��� � ���
			// (�.�. ���� ��� �� ������/��������� ������)
			if (!protPCs.getData())
			{
				// ���������� ������������� �� �������
				lastPcCom = protPCs.getCurrentCom();

				// ��������� ��������� � ���
				protBSPs.copyCommandFrom(protPCs.buf);
			}
		}
	}

	return stat;
}

/**	�������� ������ �� UART
 * 	@param ���
 * 	@return True - ������
 */
static bool uartWrite()
{
	// ����� ��������� �������� ������ ��������� �� ���������.
	protPCs.checkStat();
	// �������� ������������� �������� ������� �� �� � �� ��������
	ePRTS_STATUS stat = protPCs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC)
	{
		// ��������� ������ ���
		uartPC.trData(protPCs.trCom());
	}
	else if (stat == PRTS_STATUS_WRITE)
	{
		// �������� ������ ��
		uartPC.trData(protPCs.trCom());
	}

	// ����� ��������� �������� ������ ��������� �� ���������.
	protBSPs.checkStat();
	// �������� ��� �� ������������� �������� ������� � ��
	// ���� ���, �� ������� ������� � ����
	stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC)
	{
		// ��������� ������� ��
		uartBSP.trData(protBSPs.trCom());
	}
	else if (stat == PRTS_STATUS_NO)
	{
		// �������� ������� ���
		eGB_COM com = menu.getTxCommand();
		uint8_t num = protBSPs.sendData(com);
		uartBSP.trData(num);
	}


	return true;
}

/**	main.c
 * 	@param ���
 * 	@return ���
 */
int __attribute__ ((OS_main))
main (void)
{
	// ������� ��� ���������� ���
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// ��������� ������ �� ���������
	// ���������� �� ��������� ����������, ����� ������ �� ������
	menu.sParam.password.init(eeprom_read_word((uint16_t*) EEPROM_PASSWORD));

	sei();

	vLCDinit();
	vLCDclear();

	uartPC.init(19200);
	protPCs.setEnable(PRTS_STATUS_READ);

	uartBSP.init(4800);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// ������� ��� ��������
	// menu.setTypeDevice(AVANT_NO);



	while(1)
	{
		if (b100ms)
		{
			b100ms = false;

			cnt_wdt++;
			// ��������� �������� ��������� � ���/��
			// �������� � ���� �������� ���������� ����� � ���
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// ������ ����������� ��� � 1�
			if (++cnt_1s >= 10)
			{
				cnt_1s = 0;

				uint16_t password = menu.sParam.password.get();
				eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);
			}

			cnt_wdt++;
			// ���������� ������
			// ����������� � �������� LCD_REFRESH_PERIOD * 100��
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// �������� ��������� � ���/��
			uartWrite();

			// ����� wdt, ���� ��� ���� ����� ���� ��������
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;
		}
	}
}

/**	���������� �� ���������� ������0
 * 	����������� ��� � 100 ���
 * 	@param ���
 * 	@return ���
 */
ISR(TIMER0_COMP_vect)
{
	// ���������� ���
	vLCDmain();
}

/**	���������� �� ���������� � ������1
 * 	����������� ��� � 10 ��
 * 	@param ���
 * 	@return ���
 */
ISR(TIMER1_COMPA_vect)
{
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	vKEYmain();

	// ��������� ����� ��� � 100��
	if (cnt >  0)
		cnt--;
	else
	{
		cnt = 10 - 1;
		b100ms = true;
	}
}

/**	���������� �� ����������� ����������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_UDRE_vect)
{
	uartPC.isrUDR();
}

/** ���������� �� ��������� �������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_TX_vect)
{
	uartPC.isrTX();
	protPCs.setCurrentStatus(PRTS_STATUS_READ);
}

/** ���������� �� ��������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_RX_vect)
{
	uint8_t tmp = UDR1;

	// ���������� ��������� "�����������"
	if (protPCs.isEnable())
	{
		if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
		{
			protPCs.checkByte(tmp);
		}
	}
}

/**	���������� �� ����������� ����������� ������ UART0
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_UDRE_vect)
{
	uartBSP.isrUDR();
}

/** ���������� �� ��������� �������� ������ UART0
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_TX_vect)
{
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_READ);
}

/** ���������� �� ��������� ������ UART0
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_RX_vect)
{
	uint8_t tmp = UDR0;

	// ���������� ��������� "�����������"
	if (protBSPs.isEnable())
	{
		if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
			protBSPs.checkByte(tmp);
	}
}
