/*
 * protocolS.cpp
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#include <stdint.h>
#include "protocolS.h"
#include "debug.h"

clProtocolS::clProtocolS(uint8_t *buf, uint8_t size, stMNUparam *param) :
							buf(buf), size(size), param(param)
{
	enable = false;
	rdy = false;
	cnt = 0;
	maxLen = 0;
}

/**	Проверка принятой контрольной суммы
 * 	@param Нет
 * 	@return true - если верная контрольная сумма
 *
 */
bool
clProtocolS::checkCRC()
{
	uint8_t crc = 0;

	for(uint8_t i = 2; i < (maxLen - 1); i++)
		crc += buf[i];

	if (crc == buf[maxLen - 1])
		return true;

	return false;
}

/**	Заполнение буфера для отправки данных из массива
 * 	@param *buf Указатель на начало данных
 * 	@param size Кол-во байт данных
 * 	@return Кол-во отправляемых байт данных
 */
uint8_t
clProtocolS::trData(uint8_t com, uint8_t size, const uint8_t *buf)
{
	uint8_t cnt = 0;

	if (size <= (this->size - 4))
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = size;

		// Скопируем данные в буфер
		for(uint8_t i = 0; i < size; i++, cnt++)
			this->buf[cnt] = buf[i];

		setCRC();
	}

	// +1 - учет контрольной суммы
	return (cnt + 1);
}

/**	Заполнение буфера для отправки одного байта данных
 * 	@param *buf Указатель на начало данных
 * 	@param size Кол-во байт данных
 * 	@return Кол-во отправляемых байт данных
 */
uint8_t
clProtocolS::trByte(uint8_t com, uint8_t byte)
{
	uint8_t cnt = 0;

	if (size >= 5)
	{
		this->buf[cnt++] = 0x55;
		this->buf[cnt++] = 0xAA;
		this->buf[cnt++] = com;
		this->buf[cnt++] = 01;
		this->buf[cnt++] = byte;

		setCRC();
	}

	// +1 - учет контрольной суммы
	return (cnt + 1);
}

/** Обработка принятой посылки, извлекаем данные
 * 	@param Нет
 * 	@return false - если такой команды нет
 */
bool
clProtocolS::getData()
{
	bool tmp = false;
	switch(buf[2])
	{
		case 0x30:
			sDebug.byte1= 0x30;
			tmp = true;
			break;
		case 0x31:
			sDebug.byte1= 0x31;
			tmp = true;
			break;
		case 0x32:
			param->day = buf[4];
			param->month = buf[5];
			param->year = buf[6];
			param->hour = buf[7];
			param->minute = buf[8];
			param->second = buf[9];
			tmp = true;
			break;
	}

	return tmp;
}

/**	Вычислене контрольной суммы содержимого буфера
 * 	и добавление ее в конец посылки.
 * 	@param Нет
 * 	@return false - в случае нехватки места в буфере для КС
 *
 */
bool
clProtocolS::setCRC()
{
	uint8_t crc = 0;
	uint8_t len = buf[3] + 5;
	uint8_t i = 2;

	if (len > size)
		return false;

	for(; i < (len - 1); i++)
		crc += buf[i];
	buf[i] = crc;

	return true;
}
