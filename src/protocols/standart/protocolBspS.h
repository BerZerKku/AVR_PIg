/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS {
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

    void hdlrComGetComPrdKeep();
    void hdlrComGetDeviceNum();
    void hdlrComGetFault();
    void hdlrComGetMeas();
    void hdlrComGetNetAdr();
    void hdlrComSetNetAdr();
    void hdlrComGetSost();
    void hdlrComGetTime();
    void hdlrComGetVers();


    /** Устанавливает дату и время для записи журнала.
     *
     *  !!! Если год находится в начале (записи в оптике, безопасность):
     *  - нет дня недели;
     *  - миллисекунды идут старшим байтом вперед.
     *
     *
     *  @param[in] pos Начальная позиция в буфере.
     *  @param[in] yearfirst true если год идет первым, иначе false.
     *  @return Позиция в буфере после установки.
     */
    uint8_t setJrnDateTime(uint8_t pos, bool yearfirst);

    /** Устанавливает запись журнала.
     *
     *  @param[in] device Устройство для которого получена запись.
     *  @param[in] pc Флаг запроса с ПК.
     */
    void setJrnEntry(eGB_DEVICE device, bool pc);

    /// Устанавливает запись для журнала защиты.
    void setJrnEntryDef();
    /// Устанавливает запись для журнала событий.
    void setJrnEntryGlb();
    /// Устанавливает запись для журнала передатчика.
    void setJrnEntryPrd();
    /// Устанавливает запись для журнала приемника.
    void setJrnEntryPrm();
    /// Устанавливает запись для журнала безопасности.
    void setJrnEntrySec();

    /** Устанавлиает запись журнала для оптического варианта.
     *
     *  @param[in] device Устройство для которого получена запись.
     *  @param[in] pc Флаг запроса с ПК.
     */
    void setJrnEntryOpto(eGB_DEVICE device , bool pc);

    /** Устанавливает размер журнала.
     *
     *  @param[in] device Устройство для которого получен размрер журнала.
     */
    void setJrnSize(eGB_DEVICE device);

    /** Считывание параметров для команд GB_COM_GET_NET_ADR и SET.
     *
     *  @param pos Номер параметра.
     *  @param buf Начальный адрес буфера.
     *  @param len Количество оставшихся байт в буфере.
     *  @return Количество считанных байт данных.
     *  @retval len если в буфере не достаточно байт для параметра.
     */
    uint8_t getComNetAdr(posComNetAdr_t pos, const uint8_t *buf, uint8_t len);

//	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);
};


#endif /* PROTOCOLBSPS_H_ */
