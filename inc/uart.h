/** @brief Последовательный порт.
 *
 *  Класс работы с последовательным портом.
 */

#ifndef UART_H_
#define UART_H_

#include "paramUart.h"

class TUart {
public:
	/// Имя последовательного порта
	enum ePORT {
		PORT_UART0, 				///< Последовательный порт UART0
		PORT_UART1					///< Последовательный порт UART1
	};

	/** Конструктор
	 *
	 * 	Порты ввода-вывода должны быть настроены заранее.
	 *
	 * 	@param port Имя порта
	 * 	@see ePORT
	 * 	@param *buf Указатель на буфер данных
	 * 	@param size Размер буфера
	 */
	TUart(TUart::ePORT port, uint8_t * const buf, uint8_t size);

	/**	Открывает порт.
	 *
	 *	В случае ошибочных данных настроек порта, будут применены параметры
	 *	по умолчанию:
	 *	- UART_BAUD_RATE_19200
	 *	- UART_DATA_BITS_8
	 *	- UART_PARITY_NONE
	 *	- UART_STOP_BITS_TWO
	 *
	 * 	@param baud	Скорость работы порта.
	 * 	@param databits	Количество битов данных.
	 * 	@param parity Протокол контроля четности.
	 * 	@param stopbits Количество стоп битов.
	 * 	@return True - в случае успешного открытия
	 */
	bool open(eUART_BAUD_RATE baud, eUART_DATA_BITS databits,
			eUART_PARITY parity, eUART_STOP_BITS stopbits);

	/** Закрывает порт.
	 *
	 * 	Запрещается работа приемника и передатчика UART, а таке же сбрасываются
	 * 	все прерывания.
	 */
	void close();

	/**	Передача заданного кол-ва данных, заранее помещенных в буфер.
	 *
	 * 	@param size Кол-во байт данных для передачи.
	 * 	@return Кол-во отправляемых байт данных
	 */
	void trData(uint8_t size);

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
	 *
	 *	При передаче последнего байта из буфера, запрещается прерывание
	 *	по опустошению буфера.
	 *
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
	 *
	 * 	Запрещается прерывание на передачу, разрешается прерывание на прием.
	 *
	 * 	Должен распологаться в прерывании!!!
	 */
	void isrTX() {
		cnt_ = 0;
		numTrByte_ = 0;
		*ucsrb_ |= (1 << RXCIE);
		*ucsrb_ &= ~(1 << TXCIE);
	}

	/**	Обработчик прерывания по прихожду байта в буфер приемника.
	 *
	 * 	Должен распологаться в прерывании!!!
	 *
	 * 	@param byte Принятый байт данных
	 */
	void isrRX(uint8_t byte) {
		if (cnt_ < size_) {
			buf_[cnt_++] = byte;
		}
	}

private:
	/// кол-во принятых/полученных данных
	uint8_t cnt_;
	/// указатель на буфер
	uint8_t * const buf_;
	/// размер буфера
	const uint8_t size_;
	/// кол-во данных на передачу
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
