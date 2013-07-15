/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "debug.h"
#include "setup.h"
#include "ks0108.h"
#include "menu.h"
#include "keyboard.h"
#include "uart.h"
#include "protocolS.h"


/// ����������� ������� ������� � ������
#define SIZEOF_MAS(a) (sizeof(a) / sizeof(a[0]))

/// ������ ���������� ������ * 100 ��
#define LCD_REFRESH_PERIOD 2

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 64

/// ������ ������ ��� ������� � ��
#define BUFF_SIZE_PC 64

/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 5


// ��������� �������� ��������� �� ���������������� ������
static bool uartRead();

// ������ �������� ��������� �� ���������������� ������
static bool uartWrite();

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;


/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����������������� ����� ����������� � ��
clUart 		uartPC	(UART_UART1, uBufUartPc, BUFF_SIZE_PC);
/// ����� ����������������� ����� ����������� � ���
clUart 		uartBSP	(UART_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// ����� ������������ ��������� ����������� � ��
clProtocolS	protPCs	(uBufUartPc, BUFF_SIZE_PC);
/// ����� ������������ ��������� ����������� � ���
clProtocolS protBSPs(uBufUartBsp, BUFF_SIZE_BSP);
/// ����� ����
//clMenu		menu	(&protPCs);
clMenu menu;

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

	// �������� ������� ��������� � ��� � �� ���������
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		// ����� �������� �������� ���� ������ �� UART
		uartBSP.clrCnt();

		// �������� ����������� ����� ����������� ��������� �
		// ��������� ������ ���� ��� ������������� ����������
		if (protBSPs.checkReadData())
		{
#ifdef DEBUG
			sDebug.byte1++;
#endif

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

	// �������� ������� ������� � �� � �� ���������
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		// ����� �������� �������� ���� ������ �� UART
		uartPC.clrCnt();


		// �������� ����������� ����� ����������� ��������� �
		// ��������� ������ ���� ��� ������������� ����������
		if (protPCs.checkReadData())
		{
#ifdef DEBUG
			sDebug.byte2++;
#endif

			// ��������� ��������� ���������
			protPCs.getData();

			// ���������� ������������� �� �������
			lastPcCom = protPCs.getCurrentCom();

			// ��������� ��������� � ���
			protBSPs.copyCommandFrom(protPCs.buf);
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
	// �������� ������������� �������� ������� �� �� � �� ��������
	if (protPCs.getCurrentStatus() == PRTS_STATUS_WRITE)
	{
		uartPC.trData(protPCs.trCom());
	}

	// �������� ��� �� ������������� �������� ������� � ��
	// ���� ���, �� ������� ������� � ����
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_WRITE)
	{
		uartBSP.trData(protBSPs.trCom());
	}
	else
	{
//		uartBSP.trData(protBSPs.trCom());
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
	uint_fast8_t cnt_lcd = 0;
	uint_fast8_t cnt_1s = 0;

	vSETUP();
	sei();

	vLCDinit();
	vLCDclear();

	uartPC.init(19200);
	protPCs.setEnable();

	uartBSP.init(4800);
	protBSPs.setEnable();

	// ������� ��� ��������
	menu.setTypeDevice(AVANT_K400_OPTIC);

	while(1)
	{
		if (b100ms)
		{
			// ������ ����������� ��� � 100��

			// ��������� �������� ��������� � ���/��
			// �������� � ���� �������� ���������� ����� � ���
			menu.setConnectionBsp(uartRead());

			// ������ ����������� ��� � 1�
			if (++cnt_1s >= 10)
			{
				cnt_1s = 0;
			}

			// ���������� ������
			// ����������� � �������� LCD_REFRESH_PERIOD * 100��
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				cnt_lcd = 0;

				menu.main();
			}

			// �������� ��������� � ���/��
			uartWrite();

			b100ms = false;
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
}

/** ���������� �� ��������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART1_RX_vect)
{
	uint8_t tmp = UDR1;

	uartPC.isrRX(tmp);

	// ���������� ��������� "�����������"
	if (protPCs.isEnable())
	{
		if (protPCs.getCurrentStatus() != PRTS_STATUS_READ)
		{
			if (!protPCs.checkByte(tmp))
				uartPC.clrCnt();
		}
	}
}

/**	���������� �� ����������� ����������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_UDRE_vect)
{
	uartBSP.isrUDR();
}

/** ���������� �� ��������� �������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_TX_vect)
{
	uartBSP.isrTX();
}

/** ���������� �� ��������� ������ UART1
 * 	@param ���
 * 	@return ���
 */
ISR(USART0_RX_vect)
{
	uint8_t tmp = UDR0;

	uartBSP.isrRX(tmp);

	// ���������� ��������� "�����������"
	if (protBSPs.isEnable())
	{
		if (protBSPs.getCurrentStatus() != PRTS_STATUS_READ)
		{
			if (!protBSPs.checkByte(tmp))
				uartBSP.clrCnt();
		}
	}
}

