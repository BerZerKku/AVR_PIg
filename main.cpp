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

/// ������ ���������� ������ * 100 ��
#define LCD_REFRESH_PERIOD 2

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

/// ����� ��� UART
uint8_t uBufUart0[32];
uint8_t uBufUart1[32];

/// ��������� ���
stMNUparam sParam;


clMenu		menu	(&sParam);
clUart 		uartPC	(UART1, uBufUart0, sizeof(uBufUart0) / sizeof(uBufUart0[0]));
clUart 		uartBSP	(UART0, uBufUart1, sizeof(uBufUart1) / sizeof(uBufUart1[0]));
clProtocolS	protPCs	(uBufUart0, sizeof(uBufUart1) / sizeof(uBufUart1[0]), &sParam);
clProtocolS protBSPs(uBufUart1, sizeof(uBufUart1) / sizeof(uBufUart1[0]), &sParam);

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

	uint8_t time[] = {0x55, 0xAA, 0x32, 0x00, 0x32};

	vSETUP();
	sei();

	vLCDinit();
	vLCDclear();

	uartPC.init(19200);
	protPCs.setEnable();

	uartBSP.init(4800);
	protBSPs.setEnable();

	while(1)
	{
		if (b100ms)
		{
			// ������ ����������� ��� � 100��

			if (protBSPs.isRdy())
			{
				if (protBSPs.checkCRC())
				{
					protBSPs.getData();
				}
				protBSPs.clrRdy();
			}

			// �������� ������� �� ������������ ���������
			if (protPCs.isRdy())
			{
				if (protPCs.checkCRC())
				{
					uartPC.trData(protPCs.trCom(0x22, 'A'));
				}

				protPCs.clrRdy();
			}

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

			// ���������� ������ � ���
			uartBSP.trData(sizeof(time), time);

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
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	if (cnt >  0)
		cnt--;
	else
	{
		cnt = 100 - 1;
		vKEYmain();
	}

	// ���������� ���
	vLCDmain();
}

/**	���������� �� ���������� � ������1
 * 	����������� ��� � 100 ��
 * 	@param ���
 * 	@return ���
 */
ISR(TIMER1_COMPA_vect)
{
	b100ms = true;
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
		if (!protPCs.isRdy())
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
		if (!protBSPs.isRdy())
		{
			if (!protBSPs.checkByte(tmp))
				uartBSP.clrCnt();
		}
	}
}
