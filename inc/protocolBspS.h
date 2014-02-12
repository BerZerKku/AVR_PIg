/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"
#include "glbDefine.h"

class clProtocolBspS : public clProtocolS
{
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam);
	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	bool getData();

	/// Отправка команды
	uint8_t sendData(eGB_COM com);
private:
	bool getDefCommand(eGB_COM com);
	bool getPrmCommand(eGB_COM com);
	bool getPrdCommand(eGB_COM com);
	bool getGlbCommand(eGB_COM com);

	uint8_t sendModifDefCommand(eGB_COM com);
	uint8_t sendModifPrmCommand(eGB_COM com);
	uint8_t sendModifPrdCommand(eGB_COM com);
	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);
};


#endif /* PROTOCOLBSPS_H_ */
