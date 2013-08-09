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

// кол-во циклов, после которого залипшее текущее состояние будет сброшено
// в состояние по-умолчанию
#define MAX_CYCLE_TO_REST_SOST 10

enum ePRTS_ACTION
{
	PRTS_READ_COM,
	PRTS_WRITE_COM
};

enum ePRTS_STATUS
{
	PRTS_STATUS_NO = 0,		// состояние по-умолочанию (в том числе ошибочное)
	PRTS_STATUS_READ,		// идет считывание сообщения
	PRTS_STATUS_READ_OK,	// соообщение считано полностью, но КС не проверена
	PRTS_STATUS_WRITE_PC,	// надо передать сообщене на(с) ПК
	PRTS_STATUS_WRITE		// передается сообщение
};

// номера байт данных в протоколе
enum ePRTS_DATA_BYTE_NAME
{
	B1 = 4,
	B2,
	B3,
	B4,
	B5,
	B6,
	B7,
	B8,
	B9,
	B10,
	B11,
	B12,
	B13,
	B14,
	B15,
	B16
};

class clProtocolS
{

public:
	clProtocolS	(uint8_t *buf, uint8_t size, stGBparam *sParam);

//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// буфер принятых/передаваемых данных
	uint8_t * const buf;

	/// Проверка флага наличия принятой посылки
	bool checkReadData();

	/// Отправка сообщения, возвращает длинну передаваемой посылки
	uint8_t trCom();

	/// Запуск работы данного протокола
	void setEnable(ePRTS_STATUS stat) { enable_ = true; stat_= statDef_= stat; }

	/// Остановка работы данного протокола
	void setDisable() { enable_ = false; }

	/// Проверка текущего состояния протокола
	bool isEnable() const { return enable_; }

	/// Возращает размер буфера данных
	uint8_t getBufSize() const { return size_; }

	/// Текущая команда в буфере
	uint8_t getCurrentCom() const { return buf[2]; }

	/// Текущее кол-во байт в посылке
	uint8_t getCurrentLen() const { return maxLen_; }

	/// Текущее кол-во принятых байт данных (по протоколу)
	uint8_t getCurrentCnt() const { return cnt_; }

	/// Текущий статус работы протокола
	ePRTS_STATUS getCurrentStatus() const { return stat_; }

	/// Смена текущего статуса работы протокола
	void setCurrentStatus(ePRTS_STATUS stat) { stat_ = stat; }

	/// Копирование посылки из другого буфера
	bool copyCommandFrom(uint8_t * const bufSource);

	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	virtual bool getData() { stat_ = statDef_; return false; }

	/// Проверка текущего состояния
	void checkStat();

	/// Проверка принятого байта на соответствие протоколу
	/// возвращает false в случае ошибки
	/// !!! Помещается в прерывание по приему
	bool checkByte(uint8_t byte)
	{
		uint8_t cnt = this->cnt_;

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
			if (byte < (size_ - 5))
			{
				cnt++;
				maxLen_ = byte + 5;
			}
			else
				cnt = 0;
			break;
		default:
			cnt++;
			if (cnt >= maxLen_)
			{
				stat_ = PRTS_STATUS_READ_OK;
			}
			break;
		}
		this->cnt_ = cnt;

		return cnt;
	}

protected:
	// Текущее состояние протокола. true - запущен
	bool enable_;

	// структура параметров
	stGBparam *sParam_;

	// текущий статус работы протокола
	ePRTS_STATUS stat_;

	// Счетчик принятого байта по протоколу
	uint8_t cnt_;

	// Заявленное кол-во байт данных в посылке
	uint8_t maxLen_;

	// размер буфера
	const uint8_t size_;

	// состояние работы протокола по-умолчанию
	ePRTS_STATUS statDef_;

	// Подготовка к отправке команды (сама команда, кол-во данных и данные
	// уже должны лежать в буфере)
	uint8_t addCom();
	// Подготовка к отправке команды без данных (заполнение буфера)
	uint8_t addCom	(uint8_t com);
	// Подготовка к отправке команды с 1 байтом данных (заполнение буфера)
	uint8_t addCom	(uint8_t com, uint8_t byte);
	// Подготовка к отправке команды с данными (заполнение буфера)
//	uint8_t addCom	(uint8_t com, uint8_t size, uint8_t buf[]);

	// Проверка принятой контрольной суммы
	bool checkCRC();

	// вычисление контрольной суммы содержимого буфера
	uint8_t getCRC();
};


#endif /* PROTOCOLS_H_ */
