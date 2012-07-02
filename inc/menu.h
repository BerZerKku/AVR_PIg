/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"

/// время до переинициализации дисплея (* время цикла ЖКИ)
#define TIME_TO_REINIT_LCD 50

/// Структура параметров БСП
struct stMNUparam
{
	// дата/время
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

	// измеряемые параметры
	int8_t voltDef;
	int8_t voltCF;
	int8_t voltNoise;
	uint8_t voltOutInt;
	uint8_t voltOutFract;
	uint16_t curOut;
	uint16_t resistOut;
};

class clMenu
{
public:
	clMenu(stMNUparam *param);
	void main	();

	// возвращает кол-во линий отведенных для параметров
	uint8_t getLineParam() { return lineParam; }

private:
	// код кнопки
	eKEY key;
	// кол-во отображаемых параметров
	uint8_t lineParam;
	// создание уровня меню
	bool lvlCreate;

	stMNUparam * const sParam;

	void clearTextBuf();

	void lvlStart	();
	void lvlFirst	();
	void lvlParam	();

	void (clMenu:: *lvlMenu) ();


};

#endif /* MENU_H_ */
