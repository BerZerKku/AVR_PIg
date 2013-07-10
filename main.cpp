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


/// определение размера массива в байтах
#define SIZEOF_MAS(a) (sizeof(a) / sizeof(a[0]))

/// период обновления экрана * 100 мс
#define LCD_REFRESH_PERIOD 2


static bool uartRead();
static bool uartWrite();

/// флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

/// Буфер для UART
uint8_t uBufUartPc[32];
uint8_t uBufUartBsp[32];

clUart 		uartPC	(UART_UART1, uBufUartPc, SIZEOF_MAS(uBufUartPc));
clUart 		uartBSP	(UART_UART0, uBufUartBsp, SIZEOF_MAS(uBufUartBsp));
clProtocolS	protPCs	(uBufUartPc, SIZEOF_MAS(uBufUartPc));
clProtocolS protBSPs(uBufUartBsp, SIZEOF_MAS(uBufUartBsp));
clMenu		menu	(&protPCs);


/**	Работа с принятыми данными с UART
 * 	@param Нет
 * 	@return True - всегда
 */
static bool uartRead()
{
	// код запрашиваемой с ПК команды
	static uint8_t lastPcCom = 0;

	// Проверка наличия команды с БСП и ее обработка
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

	// Проверка наличия команды с ПК и ее обработка
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

/**	Передача данных по UART
 * 	@param Нет
 * 	@return True - всегда
 */
static bool uartWrite()
{
	// Проверка необходимости передачи команды на ПК и ее отправка
	if (protPCs.getCurrentStatus() == PRTS_STATUS_WRITE)
	{
		uartPC.trData(protPCs.trCom());

#ifdef DEBUG
//		sDebug.byte3++;
#endif
	}

	// Проверка необходимости передачи команды на ПК и ее отправка
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
 * 	@param Нет
 * 	@return Нет
 */
int __attribute__ ((OS_main))
main (void)
{
	// счетчик для обновления ЖКИ
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

	// зададим тип аппарата
	menu.setTypeDevice(AVANT_K400_OPTIC);

	while(1)
	{
		if (b100ms)
		{
			// задачи выполняемые раз в 100мс

			uartRead();

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

			uartWrite();

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
		if (protPCs.getCurrentStatus() != PRTS_STATUS_READ)
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
		if (protBSPs.getCurrentStatus() != PRTS_STATUS_READ)
		{
			if (!protBSPs.checkByte(tmp))
				uartBSP.clrCnt();
		}
	}
}

