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
    static const uint8_t kAlarmResetMode = 0;     /// Режим сброса сигнализации "0 - автомат"
    static const uint8_t kVpSac1         = 1;     /// Вывод ПРМ (SAC1) "1 - Введен"
    static const uint8_t kVpSac2         = 0;     /// Управление (SAC2) "0 - местное"
    static const uint8_t kVpSam          = 0x00;  /// Направление передачи SA.m "0x00 - все Введено"

public:
    clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);


    bool getData();


    void modifyComForPc();

private:
    bool hdlrComGetPassword(eGB_COM com);
    bool hdlrComSetPassword(eGB_COM com);

    bool hdlrComGetPrmDisable(eGB_COM com);
    bool hdlrComSetPrmDisable(eGB_COM com);

    bool hdlrComSetComPrdKeep(eGB_COM com);

    bool hdlrComSetNetAdr(eGB_COM com);

    void modifyComGetComPrdKeep();
    void modifyComGetNetAdr();
    void modifyComGetVersion();
};


#endif /* PROTOCOLPC_H_ */
