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

	uint8_t cnt_uart = 0;

	uint8_t time[] = {0x55, 0xAA, 0x32, 0x00, 0x32};
	uint8_t clear[] = {0x55, 0xAA, 0xAA, 0x00, 0xAA};
	uint8_t pusk[] = {0x55, 0xAA, 0x51, 0x00, 0x51};
	uint8_t reset[] = {0x55, 0xAA, 0x72, 0x01, 0x01, 0x74};
	uint8_t regime[] = {0x55, 0xAA, 0x30, 0x00, 0x30};
	uint8_t avar[] = {0x55, 0xAA, 0x31, 0x00, 0x31};

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
			uint8_t com = menu.txCommand();
			if (com != 0)
			{
				eMNU_TYPE_DEVICE type = menu.getTypeDevice();

				// ����� ���������
				if (com == 1)
				{
					if ( (type == AVANT_K400_OPTIC) ||
						 (type == AVANT_R400) ||
						 (type == AVANT_RZSK) )
					{
						uartBSP.trData(sizeof(clear), clear);
					}
				}
				else if (com == 2)
				{
					if ( (type == AVANT_K400_OPTIC) ||
						 (type == AVANT_K400) ||
						 (type == AVANT_RZSK) )
					{
						uartBSP.trData(sizeof(pusk), pusk);
					}
				}
				else if (com == 3)
				{
					uartBSP.trData(sizeof(reset), reset);
				}
			}
			else
			{
				cnt_uart = (cnt_uart < 2) ? cnt_uart + 1 : 0;
				if (cnt_uart == 0)
					uartBSP.trData(sizeof(time), time);
				else if (cnt_uart == 1)
					uartBSP.trData(sizeof(regime), regime);
				else if (cnt_uart == 2)
					uartBSP.trData(sizeof(avar), avar);
			}

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

