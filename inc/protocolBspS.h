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
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	bool getData();
};


#endif /* PROTOCOLBSPS_H_ */
