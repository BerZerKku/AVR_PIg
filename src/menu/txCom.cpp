/*
 * txCom.cpp
 *
 *  Created on: 2 окт. 2020 г.
 *      Author: Shcheblykin
 */

#include "txCom.hpp"

//
void TTxCom::clear() {
		cntComFast = 0;
        for(uint_fast8_t i = 0; i < SIZE_OF(comFast_); i++) {
            clearFastCom(i);
		}

        cnt1_ = 0;
        numCom1_ = 0;
        for(uint_fast8_t i = 0; i < SIZE_OF(com1_); i++) {
            com1_[i] = GB_COM_NO;
        }

        cnt2_ = 0;
        numCom2_ = 0;
        for(uint_fast8_t i = 0; i < SIZE_OF(com2_); i++) {
            com2_[i] = GB_COM_NO;
        }
}

//
bool TTxCom::addCom1(eGB_COM com, bool last) {
	bool stat = false;

	if (com != GB_COM_NO) {
		if (last) {
			if (numCom1_ == 0) {
				numCom1_ = 1;
			}
			com1_[numCom1_ - 1] = com;
			stat = true;
		} else {
			if (numCom1_ < MAX_NUM_COM_BUF1) {
				com1_[numCom1_++] = com;
				stat = true;
			} else {
//				qDebug() << hex << "Wrong add command " << com <<
//						", buffer size = " << numCom1_;
			}
		}
	}

	return stat;
}

//
eGB_COM TTxCom::getCom1() {
	eGB_COM com= GB_COM_NO;

	if (cnt1_ >= numCom1_) {
		cnt1_ = 0;
	}

	com = com1_[cnt1_++];

	return com;
}

//
eGB_COM TTxCom::lastCom1() const {
	return (numCom1_ > 0) ? com1_[numCom1_ - 1] : GB_COM_NO;
}

//
bool TTxCom::addCom2(eGB_COM com) {
	bool stat = false;

	if ((numCom2_ < MAX_NUM_COM_BUF2) && (com != GB_COM_NO)) {
		com2_[numCom2_++] = com;
		stat = true;
	} else {
//		qDebug() << hex << "Wrong add command " << com <<
//				", buffer size = " << numCom2_;
	}

	return stat;
}

//
eGB_COM TTxCom::getCom2() {
	eGB_COM com = GB_COM_NO;

	if (cnt2_ >= numCom2_) {
		cnt2_ = 0;
	} else {
		com = com2_[cnt2_++];
	}

	return com;
}

//
bool TTxCom::addFastCom(eGB_COM com, eGB_SEND_TYPE type) {
	bool isadd = false;

	if ((cntComFast < MAX_NUM_FAST_COM) && (com != GB_COM_NO)) {
		comFast_[cntComFast].com = com;
		comFast_[cntComFast].type = type;
		cntComFast++;
		isadd = true;
	}

	return isadd;
}

//
bool TTxCom::containsFastCom(eGB_COM com) const {
	for(uint_fast8_t i = 0 ; i < cntComFast; i++) {
		if (comFast_[i].com == com) {
			return true;
		}
	}
	return false;
}

//
bool TTxCom::setFastComDopByte(uint8_t dopbyte) {
	bool isadd = false;

	if (cntComFast > 0) {
		comFast_[cntComFast-1].dopByte = dopbyte;
		isadd = true;
	}

	return isadd;
}

//
eGB_COM TTxCom::getFastCom() const {
	eGB_COM com = GB_COM_NO;

	if (cntComFast > 0) {
		com = comFast_[cntComFast-1].com;
	}

	return com;
}

//
eGB_SEND_TYPE TTxCom::getFastComType() const {
	eGB_SEND_TYPE type = GB_SEND_NO;

	if (cntComFast > 0) {
		type = comFast_[cntComFast-1].type;
	}

	return type;
}

//
void TTxCom::removeFastCom() {
	if (cntComFast > 0) {
		cntComFast--;
		clearFastCom(cntComFast);
	}
}

//
void TTxCom::clearFastCom(uint8_t pos) {
	if (pos < SIZE_OF(comFast_)) {
		comFast_[pos].com = GB_COM_NO;
		comFast_[pos].type = GB_SEND_NO;
		comFast_[pos].dopByte = 0;
		for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
			comFast_[pos].buf[i] = 0;
		}
	}
}

//
uint8_t TTxCom::getFastComDopByte() const {
	uint8_t dopbyte = 0;

	if (cntComFast > 0) {
		dopbyte = comFast_[cntComFast-1].dopByte;
	}

	return dopbyte;
}

//
void TTxCom::setInt8(uint8_t byte, uint8_t pos) {
	if ((cntComFast > 0) && (pos < BUFFER_SIZE)) {
		comFast_[cntComFast-1].buf[pos] = byte;
	}
}

//
uint8_t TTxCom::getUInt8(uint8_t pos) const {
	uint8_t byte = 0;

	if ((cntComFast > 0) && (pos < BUFFER_SIZE)){
		byte = comFast_[cntComFast-1].buf[pos];
	}

	return byte;
}

//
void TTxCom::setInt16(uint16_t val) {
	if (cntComFast > 0) {
		*((uint16_t *) comFast_[cntComFast-1].buf) = val;
	}
}

//
void TTxCom::setInt16BE(int16_t val) {
	if (cntComFast > 0) {
		comFast_[cntComFast-1].buf[0] = static_cast<uint8_t> (val >> 8);
		comFast_[cntComFast-1].buf[1] = static_cast<uint8_t> (val);
	}
}

//
int16_t TTxCom::getInt16() const {
	uint16_t value = 0;

	if (cntComFast > 0) {
		value = *((int16_t *) comFast_[cntComFast].buf);
	}

	return value;
}

//
void TTxCom::setArray(const uint8_t* array, uint8_t size, uint8_t pos) {
	if ((cntComFast > 0) && ((pos + size) <= BUFFER_SIZE)) {
		for(uint8_t i = 0; i < size; i++) {
			comFast_[cntComFast-1].buf[i + pos] = array[i];
		}
	}
}

//
uint8_t* TTxCom::getBuferAddress() {
	uint8_t pos = (cntComFast > 0) ? cntComFast - 1 : 0;

	return comFast_[pos].buf;
}
