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

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100

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
#define EEPROM_START_ADDRESS 0x10

/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// Класс меню
clMenu menu;
/// Класс последовательного порта работающего с ПК
TUart uartPC(UART_UART1, uBufUartPc, BUFF_SIZE_PC);
/// Класc последовательного порта работающего с БСП
TUart uartBSP(UART_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// Класс стандартного протокола работающего с ПК
clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// Класс стандартного протокола работающего с БСП
clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);

static bool uartRead();
static bool uartWrite();
static void setInterface(eGB_INTERFACE val);

/**	Работа с принятыми данными по UART
 * 	@param Нет
 * 	@return False - в случае 5-и неполученных сообщений от БСП подряд
 */
static bool uartRead() {
	bool stat = true;
	// код запрашиваемой с ПК команды
	static uint8_t lastPcCom = 0;
	// кол-во неполученных сообщений с БСП
	static uint8_t cntLostCom = 0;

	// перед приемом проверим статус на залипание
	protBSPs.checkStat();
	// Проверка наличия сообщения с БСП и ее обработка
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protBSPs.checkReadData()) {
			// обработка принятого сообщения
			protBSPs.getData();

			// проверка соответствия команды запрошенной с ПК и команды
			// полученной от БСП и если совпадают пересылка сообщения на ПК
			if (lastPcCom == protBSPs.getCurrentCom()) {
				protPCs.copyCommandFrom(protBSPs.buf);
			}
		}

		// после принятия и обработки сообщения сбросим код предыдущей
		// запрашиваемой команды
		lastPcCom = 0;
		// сброс счетчика потерянных сообщений с БСП
		cntLostCom = 0;
	} else {
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
	if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protPCs.checkReadData()) {
			// обработка принятого сообщения
			// если сообщение небыло обработано, перешлем его в БСП
			// (т.е. если это не запрос/изменение пароля)
			if (!protPCs.getData()) {
				// сохранение запрашиваемой ПК команды
				lastPcCom = protPCs.getCurrentCom();

				// пересылка сообщения в БСП
				protBSPs.copyCommandFrom(protPCs.buf);
			}
		}
	}

	return stat;
}

/**	Передача данных по UART.
 * 	@retval True - всегда.
 */
static bool uartWrite() {
	// Перед передачей проверим статус протокола на залипание.
	protPCs.checkStat();
	// проверка необходимости передачи команды на ПК и ее отправка
	ePRTS_STATUS stat = protPCs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC) {
		// пересылка ответа БСП
		uartPC.trData(protPCs.trCom());
	} else if (stat == PRTS_STATUS_WRITE) {
		// отправка ответа ПИ
		uartPC.trData(protPCs.trCom());
	}

	// Перед передачей проверим статус протокола на залипание.
	protBSPs.checkStat();
	// проверим нет ли необходимости передачи команды с ПК
	// если нет, то возьмем команду с МЕНЮ
	stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC) {
		// пересылка запроса ПК
		uartBSP.trData(protBSPs.trCom());
	} else if (stat == PRTS_STATUS_NO) {
		// отправка запроса БСП
		eGB_COM com = menu.getTxCommand();
		// если есть команда, отправляем ее в БСП
		if (com != GB_COM_NO) {
			uint8_t num = protBSPs.sendData(com);
			uartBSP.trData(num);
		}
	}

	return true;
}

/**	Установка интерфейса связи с АВАНТом.
 * 	@param val Текущий интерфейс
 * 	@return Нет
 */
static void setInterface(eGB_INTERFACE val) {
	if (val == GB_INTERFACE_USB) {
		PORTD &= ~(1 << PD4);
	} else if (val == GB_INTERFACE_RS485) {
		PORTD |= (1 << PD4);
	}
}

/** Проверка необходимости переинициализации порта.
 * 	Сравниваются текущие настройки, с необходимыми. И если они не совпадают
 * 	происходит сохранение новых значений.
 * 	@param *current Текущие настройки порта.
 * 	@param *newparam Необходимые настройки порта.
 * 	@retval True, если настройки отличаются.
 * 	@retval False, если настройки совпадают.
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

/**	main.c
 * 	@param Нет
 * 	@return Нет
 */
int __attribute__ ((OS_main))
main(void) {
	// параметры хранимые в ЕЕПРОМ
	sEeprom eeprom;

	// счетчик для обновления ЖКИ
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// установка настроек пользователя, считанных из ЕЕПРОМ
	// проводится до включения прерываний, чтобы ничего не мешало
	eeprom_read_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS, sizeof(eeprom));
	EEAR = 0;	// сброс адреса ЕЕПРОМ в 0, для защиты данных
	menu.sParam.password.init(eeprom.password);
	menu.sParam.Uart.Interface.set(eeprom.interface);
	menu.sParam.Uart.Protocol.set(eeprom.protocol);
	menu.sParam.Uart.BaudRate.set(eeprom.baudRate);
	menu.sParam.Uart.DataBits.set(eeprom.dataBits);
	menu.sParam.Uart.Parity.set(eeprom.parity);
	menu.sParam.Uart.StopBits.set(eeprom.stopBits);
	// выбор интерфейса связи
	setInterface(menu.sParam.Uart.Interface.get());

	// запуск последовательного порта для связи с БСП
	// все настройки фиксированы
	uartBSP.open(UART_BAUD_RATE_4800, UART_DATA_BITS_8, UART_PARITY_NONE,
			UART_STOP_BITS_TWO);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// запуск последовательного порта для связи с ПК/Локальной сети.
	TUartData *uart = &menu.sParam.Uart;
	uartPC.open(uart->BaudRate.get(), uart->DataBits.get(), uart->Parity.get(),
			uart->StopBits.get());
	protPCs.setEnable(PRTS_STATUS_READ);

	sei();

	// настройка ЖКИ
	vLCDinit();
	vLCDclear();

	// зададим тип аппарата
	// menu.setTypeDevice(AVANT_NO);

	while (1) {
		if (b100ms) {
			b100ms = false;

			cnt_wdt++;
			// обработка принятых сообщений с БСП/ПК
			// передача в МЕНЮ текущего сосотояния связи с БСП
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// обновление экрана
			// где 100 - время рабочего цикла
			if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// отправка сообщений в БСП/ПК
			uartWrite();

			cnt_wdt++;
			// задачи выполняемые раз в 1с
			if (++cnt_1s >= 10) {
				cnt_1s = 0;

				//uint16_t password = menu.sParam.password.get();
				//eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);

				// сравнение занчений записанных в ЕЕПРОМ с
				// текущими настройками порта
				// если они отличаются - порт переинициализируется
				// , а новые настройки подготавливаются к записи в ЕЕПРОМ
				if (isUartPcReinit(&eeprom, &menu.sParam.Uart)) {
					uartPC.close();
					TUartData *uart = &menu.sParam.Uart;

					// если идет связь с ПК, то настройки фиксированные
					// если идет связь по Лок.сети, то настройки пользователя
					switch (uart->Interface.get()) {
					case GB_INTERFACE_USB:
						uartPC.open(UART_BAUD_RATE_19200, UART_DATA_BITS_8,
								UART_PARITY_NONE, UART_STOP_BITS_TWO);
						protPCs.setEnable(PRTS_STATUS_READ);
						break;
					case GB_INTERFACE_RS485:
						uartPC.open(uart->BaudRate.get(), uart->DataBits.get(),
								uart->Parity.get(), uart->StopBits.get());
						protPCs.setEnable(PRTS_STATUS_READ);
						break;
					case GB_INTERFACE_MAX:
						// ошибочное значение
						break;
					}

					// установка интерфейса связи с АВАНТом
					setInterface(menu.sParam.Uart.Interface.get());
				}

				// считывание текущего пароля в буфер ЕЕПРОМ
				eeprom.password = menu.sParam.password.get();

				// проверка Протокола связи с записанным в еепром значением
				if (eeprom.protocol != menu.sParam.Uart.Protocol.get()) {
					eeprom.protocol = menu.sParam.Uart.Protocol.get();
				}

				// обновление настроек пользователя в ЕЕПРОМ
				eeprom_update_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS,
						sizeof(eeprom));
				EEAR = 0;	// сброс адреса ЕЕПРОМ в 0, для защиты данных
			}

			// сброс wdt, если все шаги цикла были пройдены
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;
		}
	}
}

///	Прерывание по совпадению Таймер0. Срабатывает раз в 50 мкс.
ISR(TIMER0_COMP_vect) {
	// Обработчик ЖКИ
	vLCDmain();
	// подсветка ЖКИ
	vLCDled();
}

///	Прерывание по совпадению А Таймер1. Срабатывает раз в 10 мс.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// обработчик клавиатуры вызываем раз в 10мс
	vKEYmain();

	// установка флага раз в 100мс
	if (cnt > 0)
		cnt--;
	else {
		cnt = 10 - 1;
		b100ms = true;
	}
}

///Прерывание по опустошению передающего буфера UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// Прерывание по окончанию передачи данных UART1
ISR(USART1_TX_vect) {
	uartPC.isrTX();
	protPCs.setCurrentStatus(PRTS_STATUS_READ);
}

/// Прерывание по получению данных UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	if ((state & ((1 << FE) | (1 << DOR) | (1 << UPE))) == 0) {
		// в случае ошибки сброс счетчика принятых данных
		// и текущего статуса работы протокола
		uartPC.clrCnt();
		protPCs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		// обработчик протокола "Стандартный"
		if (protPCs.isEnable()) {
			if (protPCs.getCurrentStatus() == PRTS_STATUS_READ) {
				protPCs.checkByte(byte);
			}
		}
	}
}

///	Прерывание по опустошению передающего буфера UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// Прерывание по окончанию передачи данных UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_READ);
}

/// Прерывание по получению данных UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	// В случае обнаружения ошибки
	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// в случае ошибки сброс счетчика принятых данных
		// и текущего статуса работы протокола
		uartBSP.clrCnt();
		protBSPs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		// обработчик протокола "Стандартный"
		if (protBSPs.isEnable()) {
			if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ)
				protBSPs.checkByte(byte);
		}
	}
}
