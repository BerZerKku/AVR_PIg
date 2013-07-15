/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
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


/// Определение размера массива в байтах
#define SIZEOF_MAS(a) (sizeof(a) / sizeof(a[0]))

/// Период обновления экрана * 100 мс
#define LCD_REFRESH_PERIOD 2

/// Размер буфера для общения с БСП
#define BUFF_SIZE_BSP 64

/// Размер буфера для общения с ПК
#define BUFF_SIZE_PC 64

/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 5


// Обработка принятых сообщений по последовательным портам
static bool uartRead();

// Запуск передачи сообщений по последовательным портам
static bool uartWrite();

/// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;


/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// Класс последовательного порта работающего с ПК
clUart 		uartPC	(UART_UART1, uBufUartPc, BUFF_SIZE_PC);
/// Класы последовательного порта работающего с БСП
clUart 		uartBSP	(UART_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// Класс стандартного протокола работающего с ПК
clProtocolS	protPCs	(uBufUartPc, BUFF_SIZE_PC);
/// Класс стандартного протокола работающего с БСП
clProtocolS protBSPs(uBufUartBsp, BUFF_SIZE_BSP);
/// Класс меню
//clMenu		menu	(&protPCs);
clMenu menu;

/**	Работа с принятыми данными по UART
 * 	@param Нет
 * 	@return False - в случае 5-и неполученных сообщений от БСП подряд
 */
static bool uartRead()
{
	bool stat = true;
	// код запрашиваемой с ПК команды
	static uint8_t lastPcCom = 0;
	// кол-во неполученных сообщений с БСП
	static uint8_t cntLostCom = 0;

	// Проверка наличия сообщения с БСП и ее обработка
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		// сброс счетчика принятых байт данных по UART
		uartBSP.clrCnt();

		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protBSPs.checkReadData())
		{
#ifdef DEBUG
			sDebug.byte1++;
#endif

			// обработка принятого сообщения
			protBSPs.getData();

			// проверка соответствия команды запрошенной с ПК и команды
			// полученной от БСП и если совпадают пересылка сообщения на ПК
			if (lastPcCom == protBSPs.getCurrentCom())
			{
				protPCs.copyCommandFrom(protBSPs.buf);
			}
		}

		// после принятия и обработки сообщения сбросим код предыдущей
		// запрашиваемой команды
		lastPcCom = 0;
		// сброс счетчика потерянных сообщений с БСП
		cntLostCom = 0;
	}
	else
	{
		// в случае превышения порога потерянных сообщений при обмене с БСП
		// флаг состояния сбрасывается в False
		if (cntLostCom < MAX_LOST_COM_FROM_BSP)
			cntLostCom++;
		else
			stat = false;
	}

	// проверка наличия команды с ПК и ее обработка
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
	{
		// сброс счетчика принятых байт данных по UART
		uartPC.clrCnt();


		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protPCs.checkReadData())
		{
#ifdef DEBUG
			sDebug.byte2++;
#endif

			// обработка принятого сообщения
			protPCs.getData();

			// сохранение запрашиваемой ПК команды
			lastPcCom = protPCs.getCurrentCom();

			// пересылка сообщения в БСП
			protBSPs.copyCommandFrom(protPCs.buf);
		}
	}

	return stat;
}

/**	Передача данных по UART
 * 	@param Нет
 * 	@return True - всегда
 */
static bool uartWrite()
{
	// проверка необходимости передачи команды на ПК и ее отправка
	if (protPCs.getCurrentStatus() == PRTS_STATUS_WRITE)
	{
		uartPC.trData(protPCs.trCom());
	}

	// проверим нет ли необходимости передачи команды с ПК
	// если нет, то возьмем команду с МЕНЮ
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

			// обарботка принятых сообщений с БСП/ПК
			// передача в МЕНЮ текущего сосотояния связи с БСП
			menu.setConnectionBsp(uartRead());

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

			// отправка сообщений в БСП/ПК
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

	// обработчик клавиатуры вызываем раз в 10мс
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

