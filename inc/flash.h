/*
 * flash.h
 *
 *  Created on: 30.04.2012
 *      Author: ������
 */

//#ifndef FLASH_H_
//#define FLASH_H_

#include <avr/pgmspace.h>

const char fcUout[] PROGMEM = "U=%u.%u�";
const char fcIout[] PROGMEM	= "I=%u��";
const char fcRout[] PROGMEM = "R=%u��";
const char fcUz[] 	PROGMEM	= "U�=%.2d��";
const char fcUcf[] 	PROGMEM	= "U�=%.2d��";
const char fcUn[] 	PROGMEM	= "U�=%.2d��";
const char fcDate[] PROGMEM = "%02X.%02X.%02X";
const char fcTime[] PROGMEM = "%02X:%02X:%02X";


//#endif /* FLASH_H_ */
