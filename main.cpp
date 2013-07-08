/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
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

/// период обновления экрана * 100 мс
#define LCD_REFRESH_PERIOD 2

/// флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

/// Буфер для UART
uint8_t uBufUart0[32];
uint8_t uBufUart1[32];

clUart 		uartPC	(UART1, uBufUart0, sizeof(uBufUart0) / sizeof(uBufUart0[0]));
clUart 		uartBSP	(UART0, uBufUart1, sizeof(uBufUart1) / sizeof(uBufUart1[0]));
clProtocolS	protPCs	(uBufUart0, sizeof(uBufUart1) / sizeof(uBufUart1[0]));
clProtocolS protBSPs(uBufUart1, sizeof(uBufUart1) / sizeof(uBufUart1[0]));
clMenu		menu	(&protPCs);

/**	main.c
 * 	@param Нет
 * 	@return Нет
 */
int __attribute__ ((OS_main))
main (void)
{
	// счетчик для обновления ЖКИ
	uint_fast8_t cnt_lcd = 0;
	uint_fast8_t cnt_1s = 0;

	// код запрашиваемой с ПК команды
	uint8_t lastPcCom = 0;

	vSETUP();
	sei();

	vLCDinit();
	vLCDclear();

	uartPC.init(19200);
	protPCs.setEnable();

	uartBSP.init(4800);
	protBSPs.setEnable();

	// зададим тип аппарата
	menu.setTypeDevice(AVANT_K400_OPTIC);

	while(1)
	{
		if (b100ms)
		{
			// задачи выполняемые раз в 100мс

//			if (protBSPs.isRdy())
//			{
				// при необходимости пересылаем посылку в БСП
//				if ((lastPcCom != 0) && (lastPcCom == protBSPs.getCurrentCom()))
////				{
//					protPCs.copyCommandFrom(protBSPs.buf);
////				}
//				protPCs.getData();
//
//				lastPcCom = 0;
//				protBSPs.clrRdy();
//			}

			// Проверка наличия команды с БСП и ее обработка
			// Проверка наличия команды с ПК и ее обработка
			if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
			{
				protBSPs.checkReadData();

				if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK)
				{
					protBSPs.getData();
					protPCs.copyCommandFrom(protBSPs.buf);
				}
			}

			// Проверка наличия команды с ПК и ее обработка
			if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
			{
				protPCs.checkReadData();

				if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK)
				{
					protPCs.getData();
					protBSPs.copyCommandFrom(protPCs.buf);
				}
			}

			// Проверка необходимости передачи команды на ПК и ее отправка
			if (protPCs.getCurrentStatus() == PRTS_STATUS_WRITE)
			{
				uartPC.trData(protPCs.trCom());
			}

			// Проверка необходимости передачи команды на ПК и ее отправка
			if (protBSPs.getCurrentStatus() == PRTS_STATUS_WRITE)
			{
				uartBSP.trData(protBSPs.trCom());
			}


			// задачи выполняемые раз в 1с
			if (++cnt_1s >= 10)
			{
				cnt_1s = 0;
			}

			// обновление экрана
			// выполняется с периодом LCD_REFRESH_PERIOD * 100мс
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				cnt_lcd = 0;

				menu.main();
			}

			// Отправляем запрос в БСП
			uint8_t com = menu.txCommand();
			if (com != 0)
			{
				eMNU_TYPE_DEVICE type = menu.getTypeDevice();

//				// сброс индикации
//				if (com == 1)
//				{
//					if ( (type == AVANT_K400_OPTIC) ||
//						 (type == AVANT_R400) ||
//						 (type == AVANT_RZSK) )
//					{
//						uartBSP.trData(sizeof(clear), clear);
//					}
//				}
//				else if (com == 2)
//				{
//					if ( (type == AVANT_K400_OPTIC) ||
//						 (type == AVANT_K400) ||
//						 (type == AVANT_RZSK) )
//					{
//						uartBSP.trData(sizeof(pusk), pusk);
//					}
//				}
//				else if (com == 3)
//				{
//					uartBSP.trData(sizeof(reset), reset);
//				}
//			}
//			else
//			{
//				cnt_uart = (cnt_uart < 2) ? cnt_uart + 1 : 0;
//				if (cnt_uart == 0)
//					uartBSP.trData(sizeof(time), time);
//				else if (cnt_uart == 1)
//					uartBSP.trData(sizeof(regime), regime);
//				else if (cnt_uart == 2)
//					uartBSP.trData(sizeof(avar), avar);
			}

			b100ms = false;
		}
	}
}

/**	Прерывание по совпадению Таймер0
 * 	Срабатывает раз в 100 мкс
 * 	@param Нет
 * 	@return Нет
 */
ISR(TIMER0_COMP_vect)
{
	// Обработчик ЖКИ
	vLCDmain();
}

/**	Прерывание по совпадению А Таймер1
 * 	Срабатывает раз в 10 мс
 * 	@param Нет
 * 	@return Нет
 */
ISR(TIMER1_COMPA_vect)
{
	static uint_fast8_t cnt = 0;

	// Обработчик клавиатуры вызываем раз в 10мс
	vKEYmain();

	// установка флага раз в 100мс
	if (cnt >  0)
		cnt--;
	else
	{
		cnt = 10 - 1;
		b100ms = true;
	}
}

/**	Прерывание по опустошению передающего буфера UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_UDRE_vect)
{
	uartPC.isrUDR();
}

/** Прерывание по окончанию передачи данных UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_TX_vect)
{
	uartPC.isrTX();
}

/** Прерывание по получению данных UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_RX_vect)
{
	uint8_t tmp = UDR1;

	uartPC.isrRX(tmp);

	// обработчик протокола "Стандартный"
	if (protPCs.isEnable())
	{
		if ( (protPCs.getCurrentStatus() == PRTS_STATUS_NO) ||
			 (protPCs.getCurrentStatus() == PRTS_STATUS_READ) )
		{
			if (!protPCs.checkByte(tmp))
				uartPC.clrCnt();
		}
	}
}

/**	Прерывание по опустошению передающего буфера UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_UDRE_vect)
{
	uartBSP.isrUDR();
}

/** Прерывание по окончанию передачи данных UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_TX_vect)
{
	uartBSP.isrTX();
}

/** Прерывание по получению данных UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_RX_vect)
{
	uint8_t tmp = UDR0;

	uartBSP.isrRX(tmp);

	// обработчик протокола "Стандартный"
	if (protBSPs.isEnable())
	{
		if ( (protBSPs.getCurrentStatus() == PRTS_STATUS_NO) ||
			 (protBSPs.getCurrentStatus() == PRTS_STATUS_READ) )
		{
			if (!protBSPs.checkByte(tmp))
				uartBSP.clrCnt();
		}
	}
}

