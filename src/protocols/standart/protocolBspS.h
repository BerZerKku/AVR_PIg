/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS
{
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam); // конструктор

	bool 	getData();							///< Обработка принятого сообщения
	uint8_t sendData(eGB_COM com);				///< Отправка команды

private:
	bool getRpsCommand(eGB_COM com);			///< Обработка команд Терминала
	bool getGlbCommand(eGB_COM com);			///< Обработка команд Общих

	uint8_t sendModifRpsCommand(eGB_COM com);	///< Передача команд Терминала
	uint8_t sendModifGlbCommand(eGB_COM com);	///< Передача команд Общих
	uint8_t sendReadJrnCommand(eGB_COM com);	///< Передача команд Журналов
};


#endif /* PROTOCOLBSPS_H_ */
