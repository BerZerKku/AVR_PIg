/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

/// ����� �� ����������������� ������� (* ����� ����� ���)
#define TIME_TO_REINIT_LCD 50

enum eMNU_LVL
{
	MNU_LVL_START = 0,
	MNU_LVL_PARAM,
	MNU_LVL_FIRST
};

/// ��������� ���������� ���
struct stMNUparam
{
	// ����/�����
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

	// ���������� ���������
	int8_t voltDef;
	int8_t voltCF;
	int8_t voltNoise;
	uint8_t voltOutInt;
	uint8_t voltOutFract;
	uint16_t curOut;
	uint16_t resistOut;
};

void vMNUmain	(void);



#endif /* MENU_H_ */
