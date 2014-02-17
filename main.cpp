/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
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

/// Период обновления экрана * 100 мс
#define LCD_REFRESH_PERIOD 2

/// Размер буфера для общения с БСП
#define BUFF_SIZE_BSP 128

/// Размер буфера для общения с ПК
#define BUFF_SIZE_PC 128

/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 10

// Обработка принятых сообщений по последовательным портам
static bool uartRead();

// Запуск передачи сообщений по последовательным портам
static bool uartWrite();

/// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

/// пароль пользователя
#define EEPROM_PASSWORD 0x10

/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// Класс меню
clMenu menu;
/// Класс последовательного порта работающего с ПК
clUart 		uartPC	(UART_UART1, uBufUartPc, BUFF_SIZE_PC);
/// Класc последовательного порта работающего с БСП
clUart 		uartBSP	(UART_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// Класс стандартного протокола работающего с ПК
clProtocolPcS	protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// Класс стандартного протокола работающего с БСП
clProtocolBspS	protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);


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

	// перед приемом проверим статус на залипание
	protBSPs.checkStat();
	// Проверка наличия сообщения с БСП и ее обработка
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK)
	{
		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protBSPs.checkReadData())
		{
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

	// перед приемом проверим статус на залипание
	protPCs.checkStat();
	// проверка наличия команды с ПК и ее обработка
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK)
	{
		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protPCs.checkReadData())
		{
			// обработка принятого сообщения
			// если сообщение небыло обработано, перешлем его в БСП
			// (т.е. если это не запрос/изменение пароля)
			if (!protPCs.getData())
			{
				// сохранение запрашиваемой ПК команды
				lastPcCom = protPCs.getCurrentCom();

				// пересылка сообщения в БСП
				protBSPs.copyCommandFrom(protPCs.buf);
			}
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
	// Перед передачей проверим статус протокола на залипание.
	protPCs.checkStat();
	// проверка необходимости передачи команды на ПК и ее отправка
	ePRTS_STATUS stat = protPCs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC)
	{
		// пересылка ответа БСП
		uartPC.trData(protPCs.trCom());
	}
	else if (stat == PRTS_STATUS_WRITE)
	{
		// отправка ответа ПИ
		uartPC.trData(protPCs.trCom());
	}

	// Перед передачей проверим статус протокола на залипание.
	protBSPs.checkStat();
	// проверим нет ли необходимости передачи команды с ПК
	// если нет, то возьмем команду с МЕНЮ
	stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC)
	{
		// пересылка запроса ПК
		uartBSP.trData(protBSPs.trCom());
	}
	else if (stat == PRTS_STATUS_NO)
	{
		// отправка запроса БСП
		eGB_COM com = menu.getTxCommand();
		uint8_t num = protBSPs.sendData(com);
		uartBSP.trData(num);
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
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// установка пароля по умолчанию
	// проводится до включения прерываний, чтобы ничего не мешало
	menu.sParam.password.init(eeprom_read_word((uint16_t*) EEPROM_PASSWORD));

	sei();

	vLCDinit();
	vLCDclear();

	uartPC.init(19200);
	protPCs.setEnable(PRTS_STATUS_READ);

	uartBSP.init(4800);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// зададим тип аппарата
	// menu.setTypeDevice(AVANT_NO);



	while(1)
	{
		if (b100ms)
		{
			b100ms = false;

			cnt_wdt++;
			// обработка принятых сообщений с БСП/ПК
			// передача в МЕНЮ текущего сосотояния связи с БСП
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// задачи выполняемые раз в 1с
			if (++cnt_1s >= 10)
			{
				cnt_1s = 0;

				uint16_t password = menu.sParam.password.get();
				eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);
			}

			cnt_wdt++;
			// обновление экрана
			// выполняется с периодом LCD_REFRESH_PERIOD * 100мс
			if (++cnt_lcd >= LCD_REFRESH_PERIOD)
			{
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// отправка сообщений в БСП/ПК
			uartWrite();

			// сброс wdt, если все шаги цикла были пройдены
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;
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
	protPCs.setCurrentStatus(PRTS_STATUS_READ);
}

/** Прерывание по получению данных UART1
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART1_RX_vect)
{
	uint8_t tmp = UDR1;

	// обработчик протокола "Стандартный"
	if (protPCs.isEnable())
	{
		if (protPCs.getCurrentStatus() == PRTS_STATUS_READ)
		{
			protPCs.checkByte(tmp);
		}
	}
}

/**	Прерывание по опустошению передающего буфера UART0
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_UDRE_vect)
{
	uartBSP.isrUDR();
}

/** Прерывание по окончанию передачи данных UART0
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_TX_vect)
{
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_READ);
}

/** Прерывание по получению данных UART0
 * 	@param Нет
 * 	@return Нет
 */
ISR(USART0_RX_vect)
{
	uint8_t tmp = UDR0;

	// обработчик протокола "Стандартный"
	if (protBSPs.isEnable())
	{
		if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
			protBSPs.checkByte(tmp);
	}
}
