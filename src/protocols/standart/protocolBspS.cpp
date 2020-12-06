/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolBspS.h"
#include "glbDefine.h"
#include "paramBsp.h"

clProtocolBspS::clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
                                                                                clProtocolS(buf, size, sParam) {
}

/**	Обработка принятого сообщения.
 *
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getData(bool pc) {
    uint8_t mask = 0;
    eGB_COM com = (eGB_COM) buf[2];

    // сообщение обработано, выставим флаг на чтение
    setCurrentStatus(PRTS_STATUS_NO);

    //    QString pkg;
    //    for(uint8_t i = 0; i < buf[NUM] + 5; i++) {
    //        pkg += QString("%1 ").arg(buf[i], 2, 16, QLatin1Char('0'));
    //    }
    //    if ((buf[COM] & 0x70) == 0x70) {
    //        qDebug() << "rx pkg: " << pkg;
    //    }

    mask = com & GB_COM_MASK_GROUP;
    // ответ на команду изменения параметра/режима не требуется
    if (mask == GB_COM_MASK_GROUP_WRITE_PARAM) {
        if (com == GB_COM_SET_TIME) {
            if (buf[NUM] >= 8) {
                sParam_->DateTimeReq.setYear(buf[B1]);
                sParam_->DateTimeReq.setMonth(buf[B2]);
                sParam_->DateTimeReq.setDay(buf[B3]);
                sParam_->DateTimeReq.setHour(buf[B4]);
                sParam_->DateTimeReq.setMinute(buf[B5]);
                sParam_->DateTimeReq.setSecond(buf[B6]);
                sParam_->DateTimeReq.setMsSecond(*((uint16_t *) &buf[B7]));
                sParam_->DateTimeReq.setTimeBsp_ = true;
            }
        } else if (com == GB_COM_SET_NET_ADR) {
            hdlrComSetNetAdr();
        }
    } else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME) {
        //
    } else {
        mask = com & GB_COM_MASK_DEVICE;

        if (mask == GB_COM_MASK_DEVICE_DEF)
            getDefCommand(com, pc);				// команды защиты
        else if (mask == GB_COM_MASK_DEVICE_PRM)
            getPrmCommand(com, pc);				// команды приемника
        else if (mask == GB_COM_MASK_DEVICE_PRD)
            getPrdCommand(com, pc);				// команды передатчика
        else
            getGlbCommand(com, pc);				// команды общие

        LocalParams *lp = &sParam_->local;
        if (com == getCom(lp->getParam())) {
            // по умолчанию загружается значение первого байта,
            // на отличные от этого параметры далее ведется проверка
            int16_t val = -1000;
            switch(lp->getParam()) {
                case GB_PARAM_IN_DEC:
                    val = buf[B2 + lp->getNumOfCurrSameParam() - 1];
                    break;
                case GB_PARAM_FREQ:
                    val = TO_INT16(buf[B1], buf[B2]);
                    break;
                case GB_PARAM_COR_U:
                    val = ((int8_t) buf[B1])*10;
                    val += ((int8_t) buf[B2])/10;
                    break;
                case GB_PARAM_COR_I:
                    val = TO_INT16(buf[B3], buf[B4]);
                    break;
                case GB_PARAM_IS_PWD_ADMIN: {
                    lp->setValuePwd(sParam_->security.pwd.getPwd(USER_admin));
                } break;
                case GB_PARAM_IS_PWD_ENGINEER: {
                    lp->setValuePwd(sParam_->security.pwd.getPwd(USER_engineer));
                } break;
                default:
                    uint8_t pos = B1;
                    // смещение в зависимости от номера однотипного параметра
                    if (getParamType(lp->getParam()) == Param::PARAM_BITES) {
                        pos += (lp->getNumOfCurrSameParam() - 1) / 8;
                    } else {
                        pos += lp->getNumOfCurrSameParam() - 1;
                    }

                    if (getSendDop(lp->getParam()) != 0) {
                        pos += getSendDop(lp->getParam()) - 1;
                    }

                    // приведение к знаковому типу, в случае если возможно
                    // отрицательное значение параметра
                    val = (getMin(lp->getParam()) < 0) ?
                                                       (int8_t) buf[pos] : buf[pos];
                    break;
            }

            // Для битовых переменных передается указатель на начало данных,
            // а для остальных текущее значение.
            lp->setValue(val);
        }
    }

    return true;
}

/**	Формирование посылки и отправка заданной команды.
 *
 * 	По-умолчанию в сообщении опроса параметра посылается только код команды.
 * 	По-умолчанию в сообщениии изменения параметра посылается 1 байт данных.
 *
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых байт
 */
uint8_t clProtocolBspS::sendData(eGB_COM com) {
    uint8_t num = 0;
    uint8_t mask = 0;

    // TODO Добавить првоерку подтверждения передачи команды! И повтор в противном случае.

    mask = com & GB_COM_MASK_GROUP;

    if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM) ||
        (mask == GB_COM_MASK_GROUP_WRITE_REGIME)) {

        Q_ASSERT(com == sParam_->txComBuf.getFastCom());

        eGB_SEND_TYPE sendType = sParam_->txComBuf.getFastComType();

        if ((sendType > GB_SEND_NO) && (sendType < GB_SEND_MAX)) {
            switch(sendType) {
                case GB_SEND_INT8: {
                    uint8_t byte0 = sParam_->txComBuf.getUInt8(0);
                    num = addCom(com, byte0);
                } break;
                case GB_SEND_BITES_DOP:
                case GB_SEND_INT8_DOP: {
                    uint8_t byte0 = sParam_->txComBuf.getUInt8(0);
                    uint8_t dop = sParam_->txComBuf.getFastComDopByte();
                    num = addCom(com, byte0, dop);
                } break;
                case GB_SEND_DOP_INT8:	// DOWN
                case GB_SEND_DOP_BITES: {
                    uint8_t byte0 = sParam_->txComBuf.getUInt8(0);
                    uint8_t dop = sParam_->txComBuf.getFastComDopByte();
                    num = addCom(com, dop, byte0);
                } break;
                case GB_SEND_INT16_BE: {
                    uint8_t byte0 = sParam_->txComBuf.getUInt8(0);
                    uint8_t byte1 = sParam_->txComBuf.getUInt8(1);
                    num = addCom(com, byte0, byte1);
                } break;
                case GB_SEND_COR_U: // DOWN
                case GB_SEND_COR_I: {
                    num = 3;
                    uint8_t *array = sParam_->txComBuf.getBuferAddress();
                    num = addCom(com, num, array);
                } break;
                case GB_SEND_DOP_PWD: {
                    num = PWD_LEN + 1;
                    uint8_t *array = sParam_->txComBuf.getBuferAddress();
                    num = addCom(com, num, array);
                } break;
                case GB_SEND_TIME: {
                    num = 9;
                    uint8_t *array = sParam_->txComBuf.getBuferAddress();
                    num = addCom(com, num, array);
                } break;
                case GB_SEND_NO_DATA: {
                    num = addCom(com);
                } break;

                case GB_SEND_IS_ENTRY:
                case GB_SEND_NO:
                case GB_SEND_MAX: {
                } break;
            }
        }
    } else if (mask == GB_COM_MASK_GROUP_READ_PARAM) {
        // команды опроса параметров
        // по умолчанию отправляется только код команды
        if (com == GB_COM_GET_MEAS) {
            num = addCom(com, 0);
        } else if (com == GB_COM_GET_SOST) {
            num = addCom(com, sParam_->measParam.getTemperature());
        } else if (com == GB_COM_GET_TIME) {
            num = addCom(com, sParam_->jrnScada.getState());
        } else {
            num = addCom(com);
        }
    } else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL) {
        if (sParam_->txComBuf.getFastCom() == com) {
            Q_ASSERT(com == GB_COM_JRN_IS_SET_ENTRY);
            if (sParam_->txComBuf.getFastComType() == GB_SEND_IS_ENTRY) {
                // FIXME Избаиться от числа 3, сделать define или enum.
                num = 3;
                uint8_t *array = sParam_->txComBuf.getBuferAddress();
                num = addCom(com, num, array);
            }
        } else {
            num = sendReadJrnCommand(com);
        }
    }

    // установка статуса, в зависимости от необходимости передачи сообщения
    if (num != 0) {
        setCurrentStatus(PRTS_STATUS_WRITE);
    } else {
        setCurrentStatus(PRTS_STATUS_NO);
    }

    if (sParam_->txComBuf.getFastCom() == com) {
        sParam_->txComBuf.removeFastCom();
    }

    return num;
}

/**	Обработка принятой команды Защиты.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
void clProtocolBspS::getDefCommand(eGB_COM com, bool pc) {
    if (com == GB_COM_DEF_GET_LINE_TYPE) {
        sParam_->def.setNumDevices((eGB_NUM_DEVICES) (buf[B1]));
        uint8_t act = sParam_->glb.setNumDevices((eGB_NUM_DEVICES) (buf[B1]));
        sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
        if (act & GB_ACT_NEW)
            sParam_->device = false;
    } else  if (com == GB_COM_DEF_GET_TYPE_AC) {
        // 1 байт - тип АК
        // 2-5 - время до АК
        sParam_->def.setTypeAC((eGB_TYPE_AC) buf[B1]);
        //				uint32_t t = buf[B2];
        //				t <<= 8;
        //				t += buf[B3];
        //				t <<= 8;
        //				t += buf[B4];
        //				t <<= 8;
        //				t += buf[B5];
        sParam_->def.setTimeToAC(*((uint32_t *) &buf[B2]));
    } else if (com == GB_COM_DEF_GET_JRN_CNT) {
        setJrnSize(GB_DEVICE_DEF);
    } else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
        setJrnEntry(GB_DEVICE_DEF, pc);
    }
}

/**	Обработка принятой команды Приемника.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@retval True - в случае успешной обработки
 * 	@retval False - в случае ошибки.
 */
void clProtocolBspS::getPrmCommand(eGB_COM com, bool pc) {
    switch(com) {
        case GB_COM_PRM_GET_COM: {
            uint8_t act = GB_ACT_NO;
            if (sParam_->typeDevice == AVANT_K400) {
                act = sParam_->prm.setNumCom(buf[B1] * 4);
                sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
            }
            // в случае записи нового значения, сбросим флаг конфигурации
            if (act & GB_ACT_NEW)
                sParam_->device = false;
        } break;

        case GB_COM_PRM_GET_JRN_CNT: {
            setJrnSize(GB_DEVICE_PRM);
        } break;

        case GB_COM_PRM_GET_JRN_ENTRY: {
            setJrnEntry(GB_DEVICE_PRM, pc);
        } break;

        default:
            break;
    }
}

/**	Обработка принятой команды Передатчика.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
void clProtocolBspS::getPrdCommand(eGB_COM com, bool pc) {
    switch (com) {
        case GB_COM_PRD_GET_COM: {
            uint8_t act = GB_ACT_NO;
            if (sParam_->typeDevice == AVANT_K400) {
                act = sParam_->prd.setNumCom(buf[B1] * 4);
                sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
            }
            // в случае записи нового значения, сбросим флаг конфигурации
            if (act & GB_ACT_NEW)
                sParam_->device = false;
        } break;

        case GB_COM_PRD_GET_JRN_CNT: {
            setJrnSize(GB_DEVICE_PRD);
        } break;

        case GB_COM_PRD_GET_JRN_ENTRY: {
            setJrnEntry(GB_DEVICE_PRD, pc);
        }
        break;

        default:
            break;
    }
}

/**	Обработка принятой Общей команды.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
void clProtocolBspS::getGlbCommand(eGB_COM com, bool pc) {
    switch(com) {
        case GB_COM_GET_SOST: {
            hdlrComGetSost();
        } break;

        case GB_COM_GET_FAULT: {
            hdlrComGetFault();
        } break;

        case GB_COM_GET_TIME: {
            hdlrComGetTime();
        } break;      

        case GB_COM_GET_MEAS: {
            hdlrComGetMeas();
        } break;

        case GB_COM_GET_COM_PRD_KEEP: {
            hdlrComGetComPrdKeep();
        }  break;

        case GB_COM_GET_DEVICE_NUM: {
            hdlrComGetDeviceNum();
        } break;

        case GB_COM_GET_VERS: {
            hdlrComGetVers();
        } break;

        case GB_COM_GET_NET_ADR: {
            hdlrComGetNetAdr();
        } break;

        case GB_COM_GET_TEST: {
            eGB_TYPE_DEVICE type = sParam_->typeDevice;
            eGB_TYPE_OPTO opto = sParam_->glb.getTypeOpto();
            // определеим макимальное кол-во
            sParam_->test.setCurrentSignal(&buf[B1], type, opto);
        } break;

        case GB_COM_GET_JRN_CNT: {
            setJrnSize(GB_DEVICE_GLB);
        } break;

        case GB_COM_GET_JRN_ENTRY: {
            setJrnEntry(GB_DEVICE_GLB, pc);
        } break;

        case GB_COM_GET_JRN_IS_CNT: {
            setJrnSize(GB_DEVICE_SEC);
        } break;

        case GB_COM_GET_JRN_IS_ENTRY: {
            setJrnEntry(GB_DEVICE_SEC, pc);
        } break;

        default:
            break;

    }
}

void clProtocolBspS::hdlrComGetComPrdKeep() {
    uint8_t act = GB_ACT_NO;

    if (sParam_->typeDevice == AVANT_R400M) {
        // в Р400м/Р400 это Своместимость (тип удаленного аппарата)
        act = sParam_->glb.setCompatibility((eGB_COMPATIBILITY)buf[B1]);
    } else if (sParam_->typeDevice == AVANT_K400) {
        // совместимость К400
        act = sParam_->glb.setCompK400((eGB_COMP_K400) buf[B2]);
    }

    // в случае записи нового значения, сбросим флаг конфигурации
    if (act & GB_ACT_NEW) {
        sParam_->device = false;
    }
}

void clProtocolBspS::hdlrComGetDeviceNum() {
    sParam_->glb.setDeviceNum(buf[B1]);
}

void clProtocolBspS::hdlrComGetFault() {
    eGB_TYPE_DEVICE device = sParam_->typeDevice;

    sParam_->def.status.setFault(TO_INT16(buf[B1], buf[B2]));
    sParam_->def.status.setWarning(TO_INT16(buf[B3], buf[B4]));

    // установка номера(ов) уд.аппаратов ддля Р400(М) в режиме ПВЗУ-Е
    uint8_t n = 0;
    if ((device == AVANT_R400) || (device == AVANT_R400M)) {
        if (sParam_->glb.getCompatibility() == GB_COMPATIBILITY_PVZUE) {
            n = buf[B5];
        }
    }
    sParam_->def.status.setRemoteNumber(n);

    sParam_->prm.status.setFault(TO_INT16(buf[B5], buf[B6]));
    sParam_->prm.status.setWarning(TO_INT16(buf[B7], buf[B8]));

    sParam_->prd.status.setFault(TO_INT16(buf[B9], buf[B10]));
    sParam_->prd.status.setWarning(TO_INT16(buf[B11],buf[B12]));

    sParam_->glb.status.setFault(TO_INT16(buf[B13], buf[B14]));
    sParam_->glb.status.setWarning(TO_INT16(buf[B15],buf[B16]));
}

//
void clProtocolBspS::hdlrComGetMeas() {
    // обработаем посылку, если стоит флаг опроса всех параметров
    if (buf[B1] == 0) {
        sParam_->measParam.setResistOut(TO_INT16(buf[B2], buf[B3]));
        sParam_->measParam.setCurrentOut(TO_INT16(buf[B4],buf[B5]));
        // в buf[B7] передатся дробная часть напряжения * 100
        // т.е. если там 90, то это 0.9В.
        sParam_->measParam.setVoltageOut(buf[B6], (buf[B7] / 10));
        sParam_->measParam.setVoltageDef(buf[B8]);
        sParam_->measParam.setD(buf[B8]);	// для К400
        sParam_->measParam.setVoltageDef2(buf[B9]);
        sParam_->measParam.setVoltageCf(buf[B10]);
        sParam_->measParam.setVoltageCf2(buf[B11]);
        sParam_->measParam.setVoltageNoise(buf[B12]);
        sParam_->measParam.setVoltageNoise2(buf[B13]);
        sParam_->measParam.setPulseWidth(TO_INT16(buf[B14], buf[B15]));
        sParam_->measParam.setFreqDev(buf[B17]);
    }
}

void clProtocolBspS::hdlrComGetNetAdr() {
    uint8_t index = 1;
    uint8_t nbytes = 0;

    while(nbytes < buf[NUM]) {
        nbytes += getComNetAdr(static_cast<posComNetAdr_t> (index),
                               &buf[B1 + nbytes], buf[NUM] - nbytes);
        index++;
    }
}

void clProtocolBspS::hdlrComSetNetAdr() {    
    getComNetAdr(static_cast<posComNetAdr_t> (buf[B1]), &buf[B2], buf[NUM]);
}



void clProtocolBspS::hdlrComGetSost() {
    sParam_->def.status.setRegime((eGB_REGIME) buf[B1]);
    sParam_->def.status.setState(buf[B2]);
    sParam_->def.status.setDopByte(buf[B3]);

    sParam_->prm.status.setRegime((eGB_REGIME) buf[B4]);
    sParam_->prm.status.setState(buf[B5]);
    sParam_->prm.status.setDopByte(buf[B6]);

    sParam_->prd.status.setRegime((eGB_REGIME) buf[B7]);
    sParam_->prd.status.setState(buf[B8]);
    sParam_->prd.status.setDopByte(buf[B9]);

    // далее проверяется кол-во принятых байт
    uint8_t num = buf[NUM];

    if (sParam_->prm.status.isEnable()) {
        if (num >= 13) 	sParam_->prm.setIndCom8(0, buf[B13]);
        if (num >= 14)  sParam_->prm.setIndCom8(1, buf[B14]);
        if (num >= 15)  sParam_->prm.setIndCom8(2, buf[B15]);
        if (num >= 16)  sParam_->prm.setIndCom8(3, buf[B16]);
    }

    if (sParam_->prd.status.isEnable()) {
        if (num >= 17) 	sParam_->prd.setIndCom8(0, buf[B17]);
        if (num >= 18)  sParam_->prd.setIndCom8(1, buf[B18]);
        if (num >= 19)  sParam_->prd.setIndCom8(2, buf[B19]);
        if (num >= 20)  sParam_->prd.setIndCom8(3, buf[B20]);
    }

    // текущее состояние дискретных входов (Пуск ПРМ, Сброс индикации и т.д.)
    // проверка необходимости включить подсветку
    if (num >= 21) {
        sParam_->glb.setLedOn((buf[B21] > 0));
        sParam_->glb.setDInputState(buf[B21]);
    }

    eGB_REGIME reg = GB_REGIME_MAX;
    eGB_REGIME regTmp = GB_REGIME_MAX;
    // определение общего режима аппарата
    // по умолчанию оно будет GB_REGIME_ENABLED
    // другое состояние возможно, если все устройства аппарата
    // имеют один и тот же режим
    if (sParam_->prd.status.isEnable()) {
        regTmp = sParam_->prd.status.getRegime();
        if (reg == GB_REGIME_MAX)
            reg = regTmp;
        else if (reg != regTmp)
            reg = GB_REGIME_ENABLED;
    }

    if (sParam_->prm.status.isEnable()) {
        regTmp = sParam_->prm.status.getRegime();
        if (reg == GB_REGIME_MAX)
            reg = regTmp;
        else if (reg != regTmp)
            reg = GB_REGIME_ENABLED;
    }

    if (sParam_->def.status.isEnable()) {
        regTmp = sParam_->def.status.getRegime();
        if (reg == GB_REGIME_MAX)
            reg = regTmp;
        else if (reg != regTmp)
            reg = GB_REGIME_ENABLED;
    }
    sParam_->glb.status.setRegime(reg);
}

void clProtocolBspS::hdlrComGetTime() {
    // FIXME Разобраться что делает переменная. Задержка в работе журнала?!
    static uint8_t cntTimeFrame = 0;

    sParam_->DateTime.setYear(buf[B1]);
    sParam_->DateTime.setMonth(buf[B2]);
    sParam_->DateTime.setDay(buf[B3]);
    sParam_->DateTime.setHour(buf[B4]);
    sParam_->DateTime.setMinute(buf[B5]);
    sParam_->DateTime.setSecond(buf[B6]);
    // миллисекунд может не быть в посылке
    uint16_t ms = (buf[NUM] >= 8) ? *((uint16_t *) &buf[B7]) : 0;
    sParam_->DateTime.setMsSecond(ms);

    // новая запись журнала, для передачи в АСУ ТП
    if (buf[NUM] >= 21) {
        TJrnSCADA *jrn = &sParam_->jrnScada;
        if (jrn->isReadyToWrite()) {
            jrn->setJrn(buf[B9]);
            jrn->setEvent(buf[B10]);
            jrn->setCom(buf[B11]);
            jrn->setComSource(buf[B12]);
            // B13
            jrn->dtime.setYear(buf[B14]);
            jrn->dtime.setMonth(buf[B15]);
            jrn->dtime.setDay(buf[B16]);
            jrn->dtime.setHour(buf[B17]);
            jrn->dtime.setMinute(buf[B18]);
            jrn->dtime.setSecond(buf[B19]);
            jrn->dtime.setMsSecond(*((uint16_t *) &buf[B20]));

            jrn->setReadyToSend();
            cntTimeFrame = 0;
        }
    } else {
        if (sParam_->jrnScada.isReadyToWrite()) {
            if (cntTimeFrame > 5) {
                sParam_->jrnScada.setReadyToEvent();
            } else {
                cntTimeFrame++;
            }
        }
    }
}

void clProtocolBspS::hdlrComGetVers() {
    uint8_t act = GB_ACT_NO;
    // данные о типе аппарата
    act |= sParam_->def.status.setEnable(buf[B1] == 1);
    act |= sParam_->prm.setNumCom(buf[B2] * 4);
    sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
    // buf[B3] - прм2
    act |= sParam_->prd.setNumCom(buf[B4] * 4);
    sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
    // кол-во аппаратов в линии
    // в def хранится значение параметра
    // все действия с меню производятся относительно значения в glb.
    // !!! и значение == кол-ву аппаратов, а раньше было на 1 меньше
    act |= sParam_->glb.setNumDevices((eGB_NUM_DEVICES) (buf[B5]-1));
    sParam_->def.setNumDevices((eGB_NUM_DEVICES) (buf[B5]-1));
    sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
    // тип линии (вч, оптика, ...)
    act |= sParam_->glb.setTypeLine((eGB_TYPE_LINE) buf[B6]);
    // версия прошивки АТмега БСП
    TDeviceGlb *glb = &sParam_->glb;
    glb->setVersProgIC16(TO_INT16(buf[B7], buf[B8]) , GB_IC_BSP_MCU);
    glb->setVersProgIC16(TO_INT16(buf[B9], buf[B10]), GB_IC_BSP_DSP);

    // совместимость, только в Р400м
    act |= sParam_->glb.setCompatibility((eGB_COMPATIBILITY) buf[B11]);

    glb->setVersProgIC16(VERS, GB_IC_PI_MCU);
    glb->setVersProgIC8(buf[B12], GB_IC_BSK_PLIS_PRD1);
    glb->setVersProgIC8(buf[B13], GB_IC_BSK_PLIS_PRD2);
    glb->setVersProgIC8(buf[B14], GB_IC_BSK_PLIS_PRM1);
    glb->setVersProgIC8(buf[B15], GB_IC_BSK_PLIS_PRM2);
    glb->setVersProgIC8(buf[B16], GB_IC_BSZ_PLIS);
    glb->setVersProgIC16(TO_INT16(buf[B21], buf[B22]), GB_IC_BSP_DSP_PLIS);

    // совместимость, для Р400/Р400м и К400 отличаются
    if (sParam_->def.status.isEnable()) {
        eGB_COMPATIBILITY comp;
        comp = static_cast<eGB_COMPATIBILITY>(buf[B11]);
        act |= sParam_->glb.setCompatibility(comp);
    } else {
        eGB_COMP_K400 comp;
        comp = static_cast<eGB_COMP_K400> (buf[B11]);
        act |= sParam_->glb.setCompK400(comp);
    }

    if (buf[3] >= 17) {
        // Тип аппарата, в сентябре 2014 появился у РЗСК и Р400
        // в ноябре 2014 появился  у К400
        act |= glb->setTypeDevice((eGB_TYPE_DEVICE) buf[B17]);
    } else {
        glb->setTypeDevice(AVANT_NO);
    }

    // B18 - старший байт прошивки БСП-ПИ
    // B19 - младший байт прошивки БСП-ПИ

    // в апреле 2017  появилось однонаправленное кольцо
    // FIXME А какое именно значение должно передаваться для стандартной оптики и двунапр. кольца?!
    if (buf[3] >= 20) {
        if (buf[B20] == 0xAB) {
            glb->setTypeOpto(TYPE_OPTO_RING_UNI);
        } else {
            glb->setTypeOpto(TYPE_OPTO_STANDART);
        }
    } else {
        glb->setTypeOpto(TYPE_OPTO_STANDART);
    }

    // B21, B22 - версия прошивки DSP, смотри выше.

    // проверим необходимость обновления типа аппарата
    if (act & GB_ACT_NEW) {
        sParam_->device = false;
    }
}

//
uint8_t
clProtocolBspS::setJrnDateTime(uint8_t pos, bool yearfirst) {
    TJournalEntry *entry = &sParam_->jrnEntry;

    if (yearfirst) {
        entry->setYear(buf[pos++]);
        entry->setMonth(buf[pos++]);
        entry->setDay(buf[pos++]);
        entry->setHour(buf[pos++]);
        entry->setMinute(buf[pos++]);
        entry->setSecond(buf[pos++]);
        entry->setMSecond(TO_INT16(buf[pos], buf[pos+1]));
        pos += 2;
    } else {
        entry->setMSecond(TO_INT16(buf[pos+1], buf[pos]));
        pos += 2;
        entry->setSecond(buf[pos++]);
        entry->setMinute(buf[pos++]);
        entry->setHour(buf[pos++]);
        pos++; // день недели
        entry->setDay(buf[pos++]);
        entry->setMonth(buf[pos++]);
        entry->setYear(buf[pos++]);
    }

    return pos;
}

//
void clProtocolBspS::setJrnEntry(eGB_DEVICE device, bool pc) {
    if (!pc && (sParam_->jrnEntry.getCurrentDevice() == device)) {
        switch(device) {
            case GB_DEVICE_DEF: setJrnEntryDef(); break;
            case GB_DEVICE_GLB: setJrnEntryGlb(); break;
            case GB_DEVICE_PRD: setJrnEntryPrd(); break;
            case GB_DEVICE_PRM: setJrnEntryPrm(); break;
            case GB_DEVICE_SEC: setJrnEntrySec(); break;
            case GB_DEVICE_MAX: break;
        }
    }
}

//
void
clProtocolBspS::setJrnEntryDef() {
    TJournalEntry *entry = &sParam_->jrnEntry;

    if (sParam_->typeDevice == AVANT_OPTO) {
        entry->setSignalDef((buf[B1] << 4) + (buf[B2] & 0x0F));
        setJrnDateTime(B3, true);
        entry->setReady();
    } else {
        entry->setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
        entry->setSignalDef((buf[B2] << 4) + (buf[B4] & 0x0F));
        entry->setEventType(buf[B3]);
        setJrnDateTime(B8, false);
        entry->setReady();
    }
}

//
void
clProtocolBspS::setJrnEntryGlb() {
    TJournalEntry *entry = &sParam_->jrnEntry;

    entry->val = true;
    if (sParam_->typeDevice == AVANT_OPTO) {
        entry->setRegime((eGB_REGIME) buf[B1]);
        entry->setOpticEntry((uint8_t *) &buf[B2]);
        setJrnDateTime(B6, true);
        entry->setReady();
    } else {
        entry->setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
        entry->setEventType(buf[B2]);
        entry->setRegime((eGB_REGIME) buf[B3]);
        setJrnDateTime(B8, false);
        entry->setReady();
    }
}

//
void
clProtocolBspS::setJrnEntryPrd() {
    TJournalEntry *entry = &sParam_->jrnEntry;

    if (sParam_->typeDevice == AVANT_OPTO) {
        entry->setOpticEntry((uint8_t *) &buf[B1]);
        setJrnDateTime(B5, true);

        if (buf[NUM] >= 16) {
            // В ОПТИКЕ передаются флаги ЦПП
            entry->setOpticEntryDR((uint8_t *) &buf[B13]);
        }
        entry->setReady();
    } else {
        entry->setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
        entry->setNumCom(buf[B2]);
        entry->setEventType(buf[B3]);
        entry->setSourceCom((eGB_SOURCE_COM) buf[B4]);
        setJrnDateTime(B8, false);
        entry->setReady();
    }
}

//
void
clProtocolBspS::setJrnEntryPrm() {
    TJournalEntry *entry = &sParam_->jrnEntry;

    if (sParam_->typeDevice == AVANT_OPTO) {
        entry->setOpticEntry((uint8_t *) &buf[B1]);
        setJrnDateTime(B5, true);
        entry->setReady();
    } else {
        entry->setDeviceJrn((eGB_DEVICE_K400) buf[B1]);
        entry->setNumCom(buf[B2]);
        entry->setEventType(buf[B3]);
        entry->setSrcCom(buf[B4]);
        setJrnDateTime(B8, false);
        entry->setReady();
    }
}

//
void
clProtocolBspS::setJrnEntrySec() {
    TJournalEntry *entry = &sParam_->jrnEntry;

    entry->setUser(buf[B1]);
    entry->setUserSrc(buf[B2]);
    entry->setEvent(buf[B3]);
    setJrnDateTime(B8, false);
    entry->setReady();
}

//
void clProtocolBspS::setJrnSize(eGB_DEVICE device) {
    if (sParam_->jrnEntry.getCurrentDevice() == device) {
        uint16_t size = *((uint16_t *) &buf[B1]);
        sParam_->jrnEntry.setNumJrnEntry(size);
    }
}

//
uint8_t clProtocolBspS::getComNetAdr(posComNetAdr_t pos, const uint8_t *buf,
                                     uint8_t len) {
    uint8_t numbytes = len;

    switch(pos) {
        case POS_COM_NET_ADR_netAdr: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.NetAddress.set(*buf);
            }
        } break;
        case POS_COM_NET_ADR_interface: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.Interface.set((TInterface::INTERFACE) *buf);
            }
        } break;
        case POS_COM_NET_ADR_protocol: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.Protocol.set((TProtocol::PROTOCOL) *buf);
            }
        } break;
        case POS_COM_NET_ADR_baudrate: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.BaudRate.set((TBaudRate::BAUD_RATE) *buf);
            }
        } break;
        case POS_COM_NET_ADR_dataBits: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.DataBits.set((TDataBits::DATA_BITS) *buf);
            }
        } break;
        case POS_COM_NET_ADR_parity: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.Parity.set((TParity::PARITY) *buf);
            }
        } break;
        case POS_COM_NET_ADR_stopBits: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->Uart.StopBits.set((TStopBits::STOP_BITS) *buf);
            }
        } break;
        case POS_COM_NET_ADR_pwdEngineer: {
            if (len >= 8) {
                numbytes = 8;
                sParam_->security.pwd.setPwd(USER_engineer, buf);
            }
        } break;
        case POS_COM_NET_ADR_pwdAdmin: {
            if (len >= 8) {
                numbytes = 8;
                sParam_->security.pwd.setPwd(USER_admin, buf);
            }
        } break;
        case POS_COM_NET_ADR_cntEngineer: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->security.pwd.setCounter(USER_engineer, *buf);
            }
        } break;
        case POS_COM_NET_ADR_cntAdmin: {
            if (len >= 1) {
                numbytes = 1;
                sParam_->security.pwd.setCounter(USER_admin, *buf);
            }
        } break;
        case POS_COM_NET_ADR_resetPwd: {
            // номер для сброса до заводских настроек, данных нет
            numbytes = 0;
        } break;
    }

    return numbytes;
}

/**	Формирование сообщения команды считавания кол-ва и самих записей журнала.
 *
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendReadJrnCommand(eGB_COM com) {
    uint8_t num = 0;
    uint8_t mask = 0;
    uint16_t t = sParam_->jrnEntry.getEntryAdress();

    uint8_t byte1 = (sParam_->typeDevice == AVANT_R400M) ? t : t >> 8;
    uint8_t byte2 = (sParam_->typeDevice == AVANT_R400M) ? t >> 8 : t;

    // команды работы с журналом
    mask = com & GB_COM_MASK_DEVICE;
    if (mask == GB_COM_MASK_DEVICE_DEF) {
        if (com == GB_COM_DEF_GET_JRN_CNT) {
            num = addCom(com);
        } else if (com == GB_COM_DEF_GET_JRN_ENTRY) {
            num = addCom(com, byte1, byte2);
        }
    } else if (mask == GB_COM_MASK_DEVICE_PRM) {
        if (com == GB_COM_PRM_GET_JRN_CNT) {
            num = addCom(com);
        } else if (com == GB_COM_PRM_GET_JRN_ENTRY) {
            num = addCom(com, byte1, byte2);
        }
    } else if (mask == GB_COM_MASK_DEVICE_PRD) {
        if (com == GB_COM_PRD_GET_JRN_CNT) {
            num = addCom(com);
        } else if (com == GB_COM_PRD_GET_JRN_ENTRY) {
            num = addCom(com, byte1, byte2);
        }
    } else {
        if (com == GB_COM_GET_JRN_CNT) {
            num = addCom(com);
        } else if (com == GB_COM_GET_JRN_ENTRY) {
            num = addCom(com, byte1, byte2);
        } else if (com == GB_COM_GET_JRN_IS_CNT) {
            num = addCom(com);
        } else if (com == GB_COM_GET_JRN_IS_ENTRY) {
            // В журнале безопасности всегда старшим вперед
            num = addCom(com, t >> 8, t);
        } else if (com == GB_COM_JRN_IS_CLR) {
            num = addCom(com);
        }
    }

    return num;
}
