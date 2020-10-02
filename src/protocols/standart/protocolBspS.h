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
    bool getData(bool pc);

	/// Отправка команды
	uint8_t sendData(eGB_COM com);

private:
	void getDefCommand(eGB_COM com, bool pc);
	void getPrmCommand(eGB_COM com, bool pc);
	void getPrdCommand(eGB_COM com, bool pc);
	void getGlbCommand(eGB_COM com, bool pc);

    void hdlrComGetComPrdKeep(bool pc);
    void hdlrComGetDeviceNum(bool pc);
    void hdlrComGetFault(bool pc);
    void hdlrComGetJrnEntry(bool pc);
    void hdlrComGetMeas(bool pc);
    void hdlrComGetNetAdr(bool pc);
    void hdlrComGetSost(bool pc);
    void hdlrComGetTime(bool pc);
    void hdlrComGetVers(bool pc);

//	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);
};


#endif /* PROTOCOLBSPS_H_ */
