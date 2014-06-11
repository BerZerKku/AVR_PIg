/**
 * uart.h
 *
 *  Created on: 22.05.2012
 *      Author: Shcheblykin
 *
 *     Работа с UART.
 *     Настройка портов ввода вывода производится заранее.
 */

#ifndef UART_H_
#define UART_H_

#include "glbDefine.h"
#include "paramUart.h"

/// порт
enum eUART_PORT {
	UART_UART0, 				//
	UART_UART1					//
};

///	Последовательный порт.
class TUart {
public:

	/** Конструктор.
	 * 	Порты ввода-вывода должны быть настроены заранее.
	 * 	@param port Имя порта.
	 * 	@param *buf Указатель на буфер данных.
	 * 	@param size Размер буфера.
	 */
	TUart(eUART_PORT port, uint8_t * const buf, uint8_t size);

	/**	Открывает порт.
	 * 	@param baud	Скорость работы порта.
	 * 	@param databits	Количество битов данных.
	 * 	@param parity Протокол контроля четности.
	 * 	@param stopbits Количество стоп битов.
	 * 	@return True - в случае успешного открытия
	 */
	bool open(eUART_BAUD_RATE baud, eUART_DATA_BITS databits,
			eUART_PARITY parity, eUART_STOP_BITS stopbits);

	/** Закрывает порт.
	 */
	void close();

	/*	Возвращает текущее состояние порта.
	 * 	@retval True порт открыт
	 * 	@retval False порт закрыт.
	 */
	bool isOpen() const;

	// функции передачи данных
	void trByte(uint8_t byte);
	uint8_t trData(uint8_t size);
	uint8_t trData(uint8_t size, const uint8_t *buf);
	uint8_t trBuf();

	/**	Считывание кол-ва байт данных в буфере.
	 */
	uint8_t getCnt() const {
		return cnt_;
	}

	/**	Сброс счетчика байт данных в буфере.
	 */
	void clrCnt() {
		cnt_ = 0;
	}

	/**	Обработчик прерывания опустошения буфера передачика.
	 * 	Должен распологаться в прерывании!!!
	 */
	void isrUDR() {
		if (cnt_ < numTrByte_) {
			*udr_ = buf_[cnt_++];
		} else {
			*ucsrb_ &= ~(1 << UDRIE1);
		}
	}

	/**	Обработчик прерывания окончания передачи.
	 * 	Запрещается прерывание на передачу, разрешается прерывание на прием.
	 * 	Должен распологаться в прерывании!!!
	 */
	void isrTX() {
		cnt_ = 0;
		numTrByte_ = 0;
		*ucsrb_ |= (1 << RXCIE);
		*ucsrb_ &= ~(1 << TXCIE);

	}

	/**	Обработчик прерывания по прихожду байта в буфер приемника.
	 * 	Должен распологаться в прерывании!!!
	 * 	@param byte Принятый байт данных
	 */
	void isrRX(uint8_t byte) {
		if (cnt_ < size_) {
			buf_[cnt_++] = byte;
		}
	}

private:
	// кол-во принятых/полученных данных
	uint8_t cnt_;
	// указатель на буфер
	uint8_t * const buf_;
	// размер буфера
	const uint8_t size_;
	// кол-во данных на передачу
	uint8_t numTrByte_;

	// регистр данных
	volatile uint8_t *udr_;

	// регистры настройки
	volatile uint8_t *ubbrh_;
	volatile uint8_t *ubbrl_;
	volatile uint8_t *ucsrc_;
	volatile uint8_t *ucsrb_;
	volatile uint8_t *ucsra_;
};

#endif /* UART_H_ */
