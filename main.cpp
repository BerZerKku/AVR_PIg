/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <math.h>
//#include <util/delay.h>

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


static bool uartRead();
static bool uartWrite();

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

/// ����� ��� UART
uint8_t uBufUartPc[32];
uint8_t uBufUartBsp[32];

clUart 		uartPC	(UART_UART1, uBufUartPc, SIZEOF_MAS(uBufUartPc));
clUart 		uartBSP	(UART_UART0, uBufUartBsp, SIZEOF_MAS(uBufUartBsp));
clProtocolS	protPCs	(uBufUartPc, SIZEOF_MAS(uBufUartPc));
clProtocolS protBSPs(uBufUartBsp, SIZEOF_MAS(uBufUartBsp));
clMenu		menu	(&protPCs);


/**	������ � ��������� ������� � UART
 * 	@param ���
 * 	@return True - ������
 */
static bool uartRead()
{
	// ��� ������������� � �� �������
	static uint8_t lastPcCom = 0;

	// �������� ������� ������� � ��� � �� ���������
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		uartBSP.clrCnt();

		if (protBSPs.checkReadData())
		{
			protBSPs.getData();

			if (lastPcCom == protBSPs.getCurrentCom())
				protPCs.copyCommandFrom(protBSPs.buf);

#ifdef DEBUG
//			sDebug.byte5++;
#endif
		}

		lastPcCom = 0;

#ifdef DEBUG
//		sDebug.byte1++;
#endif
	}

	// �������� ������� ������� � �� � �� ���������
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		uartPC.clrCnt();

		if (protPCs.checkReadData())
		{
			protPCs.getData();

			lastPcCom = protPCs.getCurrentCom();
			protBSPs.copyCommandFrom(protPCs.buf);

#ifdef DEBUG
//			sDebug.byte6++;
#endif
		}

#ifdef DEBUG
		sDebug.byte1 = uBufUartPc[0];
		sDebug.byte2 = uBufUartPc[1];
		sDebug.byte3 = uBufUartPc[2];
		sDebug.byte4 = uBufUartPc[3];
		sDebug.byte5 = uBufUartPc[4];
		sDebug.byte6 = uBufUartPc[4];
		sDebug.byte7 = protPCs.getCurrentLen();
		sDebug.byte8 = protPCs.getCurrentCnt();
#endif
	}

	return true;
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

#ifdef DEBUG
//		sDebug.byte3++;
#endif
	}

	// �������� ������������� �������� ������� �� �� � �� ��������
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_WRITE)
	{
		uartBSP.trData(protBSPs.trCom());

#ifdef DEBUG
//		sDebug.byte4++;
#endif
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

			uartRead();

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

