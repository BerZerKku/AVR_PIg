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

void vMNUmain	(void);


#endif /* MENU_H_ */
