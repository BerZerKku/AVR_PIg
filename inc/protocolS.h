/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

class clProtocolS
{
public:
	clProtocolS(uint8_t *buf, uint8_t size);

	bool checkCRC	();
	uint8_t trData	(uint8_t com, uint8_t size, const uint8_t * buf);
	uint8_t trByte	(uint8_t com, uint8_t byte);
	bool getData	();

	uint8_t tmp;

	// Проверка принятого байта на соответствие протоколу
	// возвращает false в случае ошибки
	// !!! Помещается в прерывание по приему
	bool checkByte(uint8_t byte)
	{
		switch(cnt)
		{
			case 0:
				if (byte == 0x55)
					cnt++;
				break;
			case 1:
				cnt = (byte == 0xAA) ? 2 : 0;
				break;
			case 2:
				cnt++;
				break;
			case 3:
				if (byte < (size - 5))
				{
					cnt++;
					maxLen = byte + 5;
				}
				else
					cnt = 0;
				break;
			default:
				cnt++;
				if (cnt >= maxLen)
					rdy = true;
				break;
		}

		return cnt;
	}

	/// Проверка флага принятой посылки
	bool isRdy() const { return rdy; };

	/// Сброс флага принятой посылки
	void clrRdy() { rdy = false; cnt = 0;}

	/// Запуск работы данного протокола
	void setEnable() { enable = true; clrRdy(); }

	/// Остановка работы данного протокола
	void setDisable() { enable = false; }

	/// Проверка текущего состояния протокола
	bool isEnable() const { return enable; }

private:
	// Счетчик принятого байта по протоколу
	uint8_t cnt;
	// Заявленное кол-во байт данных в посылке
	uint8_t maxLen;
	// указатель на буфер
	uint8_t * const buf;
	// размер буфера
	const uint8_t size;
	// флаг проверки на соответствие протоколу
	bool rdy;
	// Текущее состояние протокола. true - запущен
	bool enable;

	// добавление к посылке контрольной суммы
	bool setCRC();
};


#endif /* PROTOCOLS_H_ */
