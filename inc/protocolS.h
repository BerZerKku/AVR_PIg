/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <stdint.h>
#include "glbDefine.h"
#include "debug.h"

enum ePRTS_ACTION
{
	PRTS_READ_COM,
	PRTS_WRITE_COM
};

enum ePRTS_STATUS
{
	PRTS_STATUS_NO = 0,		// состояние по-умолочанию (в том числе ошибочное)
	PRTS_STATUS_READ,		// считана команда (но еще не проверена КС)
	PRTS_STATUS_WRITE		// в буфере есть команда, надо передать
};

class clProtocolS
{

public:
	clProtocolS	(uint8_t *buf, uint8_t size, stGBparam *sParam);

//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// указатель на буфер
	uint8_t * const buf;

	/// Проверка флага наличия принятой посылки
	bool checkReadData();

	/// Отправка сообщения, возвращает длинну передаваемой посылки
	uint8_t trCom();

	/// Запуск работы данного протокола
	void setEnable() { enable = true; stat = PRTS_STATUS_NO; }

	/// Остановка работы данного протокола
	void setDisable() { enable = false; }

	/// Проверка текущего состояния протокола
	bool isEnable() const { return enable; }

	/// Возращает размер буфера данных
	uint8_t getBufSize() const { return size; }

	/// Текущая команда в буфере
	uint8_t getCurrentCom() const { return buf[2]; }

	/// Текущее кол-во байт в посылке
	uint8_t getCurrentLen() const { return maxLen; }

	/// Текущее кол-во принятых байт данных (по протоколу)
	uint8_t getCurrentCnt() const { return cnt; }

	/// Текущий статус работы протокола
	ePRTS_STATUS getCurrentStatus() const { return this->stat; }

	/// Копирование посылки из другого буфера
	bool copyCommandFrom(uint8_t * const bufSource);

	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	virtual bool getData() { stat = PRTS_STATUS_NO; return false; }

	///
//	bool sendData(uint8_t com, uint8_t *data);


	/// Проверка принятого байта на соответствие протоколу
	/// возвращает false в случае ошибки
	/// !!! Помещается в прерывание по приему
	bool checkByte(uint8_t byte)
	{
		uint8_t cnt = this->cnt;

		buf[cnt] = byte;

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
			{
				stat = PRTS_STATUS_READ;
			}
			break;
		}
		this->cnt = cnt;

		return cnt;
	}

protected:
	// Текущее состояние протокола. true - запущен
	bool enable;

	// структура параметров
	stGBparam *sParam;

	// текущий статус работы протокола
	ePRTS_STATUS stat;

	// Счетчик принятого байта по протоколу
	uint8_t cnt;

	// Заявленное кол-во байт данных в посылке
	uint8_t maxLen;

	// размер буфера
	const uint8_t size;

	// Подготовка к отправке команды (сама команда, кол-во данных и данные
	// уже должны лежать в буфере)
	uint8_t addCom();
	// Подготовка к отправке команды без данных (заполнение буфера)
//	uint8_t addCom	(uint8_t com);
	// Подготовка к отправке команды с 1 байтом данных (заполнение буфера)
//	uint8_t addCom	(uint8_t com, uint8_t byte);
	// Подготовка к отправке команды с данными (заполнение буфера)
//	uint8_t addCom	(uint8_t com, uint8_t size, uint8_t buf[]);

	// Проверка принятой контрольной суммы
	bool checkCRC();

	// вычисление контрольной суммы содержимого буфера
	uint8_t getCRC();
};


#endif /* PROTOCOLS_H_ */
