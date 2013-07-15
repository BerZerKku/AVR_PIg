/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCS_H_
#define PROTOCOLPCS_H_

#include "protocolS.h"

class clProtocolPcS : public clProtocolS
{
public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	bool getData();
};


#endif /* PROTOCOLPC_H_ */
