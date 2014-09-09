#include <util/delay.h>

#include "../inc/debug.h"
#include "../inc/hd44780.h"

// таблица преобразования русских символов в коды ЖКИ
static const uint8_t decode[] = {
		//A		Б	В	Г   Д     Е   Ж     З   И     й   К     Л   М     Н   О     П
		0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
		//Р     С   Т     У    Ф    Х   Ц     Ч   Ш     Щ   Ъ     Ы    Ь    Э   Ю     Я
		0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
		//а     б    в    г   д     е   ж     з    и    й   к     л   м     н   о     п
		0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
		//р    с   т     у   ф     х   ц     ч   ш     щ   ъ     ы    ь    э    ю    я
		0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};

/// флаг необходимости обновить данные на ЖКИ
volatile uint8_t bRefresh = 0;

/// буфер данных для вывода на ЖКИ
char *buf;

void sendData(uint8_t val);	//	Отправка байта данных в ЖКИ.
void sendCom(uint8_t val);	//	Отправка команды в ЖКИ.

//static __flash strLCDinit stInit[] = {	0x30, 63,	//
//										0x30, 2,	//
//										0x30, 3,	// первоначальная инициализация
//										0x38, 1,	// 8-бит, 4 строки, 5х8 шрифт
//										0x0C, 1,	// вкл.дисплея, курсор отключен
//										0x01, 32,	// очистка дисплея, DDRAM в 0
//										0x02, 1,	// сброс DDRAM и смещения в 0
//										0x00, 0		};

// Инициализация дисплея.
void vLCDinit(void) {
	static volatile  uint8_t poweron = 0;

	SET_TP;
	// при включении идет последовательность команд, которые при повторной
	// инициализации уже не нужны.
	if (poweron == 0) {
		_delay_ms(200);
		sendCom(0x30);
		_delay_ms(20);
		sendCom(0x30);
		_delay_us(200);
		sendCom(0x30);
		poweron = 1;
	}

	_delay_ms(2);
	sendCom(0x38);
	_delay_us(100);
	sendCom(0x0C);
	_delay_us(100);
	sendCom(0x01);
	_delay_ms(2);
	sendCom(0x02);
	_delay_us(100);
	CLR_TP;
};

// Очистка содержимого буфера
void vLCDclear(void) {
	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
		buf[i] = ' ';
	}
}

// Основная функция обновления содержимого ЖКИ.
void vLCDmain(void) {
	static uint8_t cnt = 0;
	static uint8_t com = 0;

	if (bRefresh) {
		if (com != 0) {
			// отправка команды перехода на другую строку
			sendCom(com);
			com = 0;
		} else {
			// отправка байта данных
			sendData(buf[cnt++]);

			// завершение вывода данных в ЖКИ
			if (cnt >= SIZE_BUF_STRING) {
				cnt = 0;
				bRefresh = 0;
			}

			// переход на другую строку
			if (cnt == 0) {
				com = 0x80;
			} else if (cnt == 20) {
				com = 0xC0;
			} else if (cnt == 40) {
				com = 0x94;
			} else if (cnt == 60) {
				cnt = 0xD4;
			}
		}
	}
}

// Начало обновления содержимого ЖКИ.
void vLCDrefresh(void) {
	bRefresh = 1;
}

// Преобразование массива символов, в данные для вывода на ЖКИ.
void vLCDputchar(char *b) {
	buf = b;
	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
		if ((buf[i] >= 'А') && (buf[i] <= 'я')) {
			buf[i] = decode[buf[i] - 'A'];
		}
	}
}

/**	Отправка байта данных в ЖКИ.
 *
 * 	@param val Код символа в таблице знаков ЖКИ.
 */
void sendData(uint8_t val) {
	PORT_RS	|= (1 << PIN_RS);
	PORT_E	|= (1 << PIN_E);
	PORT_D	 = val;
	PORTE	&= ~(1 << PIN_E);
}

/**	Отправка команды в ЖКИ.
 *
 * 	@param val Команда.
 *
 */
void sendCom(uint8_t val) {
	PORT_RS	&= ~(1 << PIN_RS);
	PORT_E	|= (1 << PIN_E);
	PORT_D	 = val;
	PORTE	&= ~(1 << PIN_E);
}

//#pragma vector=TIMER2_OVF_vect
//__interrupt void Timer2_ovf(void){
//  switch(LCDstat){
//    case 2:{
//      TCNT2=243;
//      WriteLCDdata(LCDbuf[i1]);
//      if (i1<79) i1++; else i1=0;
//      switch(i1){
//        case 0: {CommandLCD=0x80; LCDstat=1;} break;
//        case 20: {CommandLCD=0xC0; LCDstat=1;} break;
//        case 40: {CommandLCD=0x94; LCDstat=1;} break;
//        case 60: {CommandLCD=0xD4; LCDstat=1;} break;
//      }
//    }break;
//    case 1:{
//      if (CommandLCD==0x01) TCNT2=48;
//      else TCNT2=243;
//      WriteLCDcommand(CommandLCD);
//      LCDstat=2;
//    }break;
//    case 3:{ //инициализация ЖК
//      if (InitLCD[i3]==0x01) TCNT2=48;
//      else TCNT2=243;
//      if (InitLCD[i3]!=0x00)  {WriteLCDcommand(InitLCD[i3]); i3++;}
//      else{LCDstat=2;i3=0;i1=0;}
//    }break;
//  }
//}

//void LCDprint(unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned char* bufer, unsigned char convers){
//  unsigned char StartChar, i2=0;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  while(bufer[i2]!=0x00){
//    if (convers==1){
//      if ((bufer[i2]>='А')&&(bufer[i2]<='я')) LCDbuf[StartChar+i2]=CodeLetterLCD[bufer[i2]-192];
//      else LCDbuf[StartChar+i2]=bufer[i2];
//    }else LCDbuf[StartChar+i2]=bufer[i2];
//    i2++;
//  }
//}

//void LCDbufClear(void){
//  for (unsigned char i4=0;i4<80;i4++) LCDbuf[i4]=0x20;  //записывем пробел
//  i1=0;
//}

//void LCDbufClMenu(void){  //очистка 3 нижних строк
//  for (unsigned char i4=20;i4<80;i4++) LCDbuf[i4]=0x20;
//}

//void FuncInitLCD(void){
//  i3=0;
//  LCDstat=3;
//}

//void FuncCommandLCD(unsigned char com){
//  CommandLCD=com;
//  LCDstat=1;
//}

//void FuncClearCharLCD(unsigned char Line, unsigned char AddressInLine, unsigned char NumberChar){
//  unsigned char StartChar, i2;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  for (i2=0; i2<NumberChar;i2++) LCDbuf[StartChar++]=0x20;
//}
//
//void LCDprintHEX(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  LCDbuf[StartChar]=CodePrint/16;
//  LCDbuf[StartChar+1]=CodePrint-LCDbuf[StartChar]*16;
//  if (LCDbuf[StartChar]>9) LCDbuf[StartChar]+=0x37;
//  else  LCDbuf[StartChar]+=0x30;
//  if (LCDbuf[StartChar+1]>9) LCDbuf[StartChar+1]+=0x37;
//  else  LCDbuf[StartChar+1]+=0x30;
//}
////вывод на экран числа в десятичном виде, со сдвигом влево
//void LCDprintDEC(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  if (CodePrint>9) LCDbuf[StartChar++]=(CodePrint/10)+0x30;
//  LCDbuf[StartChar]=(CodePrint%10)+0x30;
//}
//
////вывод на экран числа в десятичном виде, со сдвигом вправо
//void LCDprintDEC1(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  if(CodePrint>9)
//    LCDbuf[StartChar]=CodePrint/10+0x30;
//  else
//    LCDbuf[StartChar]=' ';
//  StartChar++;
//  LCDbuf[StartChar]=(CodePrint%10)+0x30;
//}
//
////вывод на экран числа в десятичном виде 4 знаков, со сдвигом вправо
//void LCDprintDEC2(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  LCDbuf[StartChar]=CodePrint/100+0x30;
//  CodePrint=CodePrint-(LCDbuf[StartChar]-0x30)*100;
//  StartChar++;
//  LCDbuf[StartChar]=CodePrint/10+0x30;
//  StartChar++;
//  LCDbuf[StartChar]=CodePrint%10+0x30;
//  if (LCDbuf[StartChar-2]==0x30) LCDbuf[StartChar-2]=' ';
//  if (LCDbuf[StartChar-1]==0x30) LCDbuf[StartChar-1]=' ';
//  StartChar++;
//  LCDbuf[StartChar]='0';
//}
//
//void LCDprintBits(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  unsigned char temp=0x80;
//
//  StartChar=(Line-1)*20+AddressInLine-1;
//  do{
//    LCDbuf[StartChar]=(CodePrint&temp)+0x30;
//    if (LCDbuf[StartChar]!=0x30) LCDbuf[StartChar]=0x31;
//    StartChar++;
//    temp=temp>>1;
//  }while(temp!=0);
//}
//
//void LCDprintTetr(unsigned char Line, unsigned char AddressInLine, unsigned char CodePrint){
//  unsigned char StartChar;
//  unsigned char temp=0x08;
//
//  StartChar=(Line-1)*20+AddressInLine-1;
//  do{
//    LCDbuf[StartChar]=(CodePrint&temp)+0x30;
//    if (LCDbuf[StartChar]!=0x30) LCDbuf[StartChar]=0x31;
//    StartChar++;
//    temp=temp>>1;
//  }while(temp!=0);
//}
//
//void LCDprintf(unsigned char Line, unsigned char AddressInLine, unsigned char LCDstatus, unsigned __flash char* buferf, unsigned char convers)
//{
//  unsigned char StartChar, i2=0;
//  StartChar=(Line-1)*20+AddressInLine-1;
//  while(buferf[i2]!=0x00)
//    {
//      if (convers==1)
//        {
//          if ((buferf[i2]>='А')&&(buferf[i2]<='я')) LCDbuf[StartChar+i2]=CodeLetterLCD[buferf[i2]-192];
//          else LCDbuf[StartChar+i2]=buferf[i2];
//        }
//      else
//        if (convers==2)
//          {
//            LCDbuf[StartChar+i2]=0x20;
//          }
//        else
//          {
//            LCDbuf[StartChar+i2]=buferf[i2];
//          }
//      i2++;
//    }
// // LCDstat=LCDstatus;
//}
//
//void LCDprintInsertion(unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//
//  LCDbuf[StartChar]=CodePrint/1000+0x30;
//  CodePrint=CodePrint-(LCDbuf[StartChar]-0x30)*1000;
//  StartChar++;
//  LCDbuf[StartChar]=CodePrint/100+0x30;
//  CodePrint=CodePrint-(LCDbuf[StartChar]-0x30)*100;
//  StartChar++;
//  LCDbuf[StartChar]=CodePrint/10+0x30;
//  StartChar++;
//  LCDbuf[StartChar]=CodePrint%10+0x30;
//}
//
//void LCDprintChar (unsigned char Line, unsigned char AddressInLine, unsigned int CodePrint){
//  unsigned char StartChar;
//  StartChar=(Line-1)*20+AddressInLine-1;
//
//  LCDbuf[StartChar] = CodePrint;
//}
//
//void LCDptinrArchCount(unsigned char AddressInLine, unsigned int Count, unsigned char Shift){
//  unsigned char tt, cc, ii;
//  AddressInLine+=59;  //четвертая строка
//  cc=AddressInLine;
//  LCDbuf[AddressInLine++]='(';
//  if ((Count==0)||(Count>256)){
//    LCDbuf[AddressInLine++]=0x30;
//  }else{
//    tt=0;
//    if (Shift==255){
//      LCDbuf[AddressInLine++]='2';
//      LCDbuf[AddressInLine++]='5';
//      LCDbuf[AddressInLine++]='6';
//    }else{
//      Shift++;  //т.к. сдвиг у нас считался с 0, увеличим значение на 1
//      if (Shift>99){
//          tt=Shift/100;
//          LCDbuf[AddressInLine++]=tt+0x30;
//      }
//      if (Shift>9){
//        tt=Shift/10 - tt*10 ;
//        LCDbuf[AddressInLine++]=tt+0x30;
//      }
//      LCDbuf[AddressInLine++]=Shift%10+0x30;
//    }
//  }
//
//  LCDbuf[AddressInLine++]='/';
//  if (Count==256){
//    LCDbuf[AddressInLine++]='2';
//    LCDbuf[AddressInLine++]='5';
//    LCDbuf[AddressInLine++]='6';
//  }else
//    if (Count<256){
//      tt=0;
//      ii=(unsigned char) Count;
//      if (ii>99){
//        tt=ii/100;
//        LCDbuf[AddressInLine++]=tt+0x30;
//      }
//      if (ii>9){
//        tt=(ii/10) - (tt*10);
//        LCDbuf[AddressInLine++]=tt+0x30;
//      }
//      LCDbuf[AddressInLine++]=ii%10+0x30;
//    }else LCDbuf[AddressInLine++]='?';  //ошибочное значение
//
//
//  LCDbuf[AddressInLine]=')';
//
//  for(cc=(AddressInLine-cc); cc<8; cc++) LCDbuf[++AddressInLine]=' '; //сотрем незадействованные ячейки
//}
//
//void LCDprintData(unsigned char Adr, unsigned char* Mass){
//  char i, CodePrint;
//
//  for(i=0; i<3; i++){
//    if (i==0) CodePrint=Mass[13];
//    else
//      if (i==1) CodePrint=Mass[14];
//      else CodePrint=Mass[15];
//
//    LCDbuf[Adr]=CodePrint>>4;
//    if (LCDbuf[Adr]>9) LCDbuf[Adr]+=0x37;
//    else  LCDbuf[Adr]+=0x30;
//    LCDbuf[++Adr]=CodePrint&0x0F;
//    if (LCDbuf[Adr]>9) LCDbuf[Adr]+=0x37;
//    else  LCDbuf[Adr]+=0x30;
//
//    if (i<2) LCDbuf[++Adr]='.';
//    Adr++;
//  }
//}
//
//void LCDprintTime(unsigned char Adr, unsigned char* Mass){
//  char i, CodePrint;
//  unsigned int ms;
//  for(i=0; i<3; i++){
//    if (i==0) CodePrint=Mass[11];
//    else
//      if (i==1) CodePrint=Mass[10];
//      else CodePrint=Mass[9];
//
//    LCDbuf[Adr]=CodePrint>>4;
//    if (LCDbuf[Adr]>9) LCDbuf[Adr]+=0x37;
//    else  LCDbuf[Adr]+=0x30;
//    LCDbuf[++Adr]=CodePrint&0x0F;
//    if (LCDbuf[Adr]>9) LCDbuf[Adr]+=0x37;
//    else  LCDbuf[Adr]+=0x30;
//
//    if (i<2) LCDbuf[++Adr]=':';
//    else LCDbuf[++Adr]='.';
//    Adr++;
//  }
//
//  ms=Mass[7]+ (Mass[8]<<8);
//  if (ms>999){  //ошибочное значение.
//    for(i=0; i<3; i++) LCDbuf[Adr++]='?';
//  }else{
//    LCDbuf[Adr+2]=ms%10 +0x30;
//    ms=ms/10;
//    LCDbuf[Adr+1]=ms%10 + 0x30;
//    LCDbuf[Adr]=ms/10 + 0x30;
//  }
//}
//
//void LCDprintBitMask(unsigned char Adr, unsigned char Val, unsigned char Mask){
//  unsigned char temp=0x80;
//  do{
//    if (Mask&temp){ //если надо вывести этот бит
//      if (Val&temp) LCDbuf[Adr]='1';
//      else LCDbuf[Adr]='0';
//      Adr++;
//    }
//    temp>>=1;
//  }while(temp>0);
//}
//
//extern uchar cNumLine;
//extern unsigned int iTimeToAKnow, iTimeToAK;
//void LCDprintTimeAK(unsigned char AK, uchar dev, unsigned char Num, unsigned char* Time)
//{
//  	//			АВАНТ			ПВЗ-90 / АВЗК		ПВЗЛ
//	//AK = 1 - авто ускор 		нормальный			нормальный
//  	//АК = 2 - авто норм
//  	//АК = 3 - ускор			ускор.
//  	//АК = 4 - АК выключен		выкл.				выкл.
//	//AK = 5 - 					проверка			односторон.
//	//AK = 6 - 					испыт.
//
//	// dev = 0 - АВАНТ
//	// dev = 1 - ПВЗ-90
//	// dev = 2 - АВЗК
//	// dev = 3 - ПВЗУ-Е
//	// dev = 4 - ПВЗЛ
//
//    uchar 	time1 = 59,	// минуты
//	time2 = 0, 	// секунды
//	time3 = 0;	// часы
//    uchar 	start1 = 48;
//
//
//	if  ( (AK > 6) || (AK == 4) || (AK == 0) || (dev > 4) || (iTimeToAKnow > iTimeToAK) )
//	{
//		// если ошибочный тип АК или тип удаленного устройства
//		// либо текущий счетчик времени больше времени до АК
//		// очистим строку и выйдем из функции
//		for( ; start1 < 60; start1++)
//			LCDbuf[start1]=' ';
//		return;
//	}
//
//	//если предыдущий символ не пробел, пропустим 1 знак
//	if (LCDbuf[start1 - 1] != ' ')
//		LCDbuf[start1++] = ' ';
//
//	// в АВАНТе время высчитывается
//	// при совместимости выводится полученное значение
//	switch(dev)
//	{
//		case 0:		// АВАНТ
//		{
//			time2 = 60 - (Time[6]-0x30)*10 - (Time[7]-0x30);
//
//			if (cNumLine == 2)
//			{
//				if (Num == '2') time2 += 30; //если 2 аппарат, добавим 30 секунд
//			}
//			else
//				if (cNumLine == 3)
//				{
//					if (Num == '2') time2+=20;
//					else
//						if (Num == '3') time2+=40;
//				}
//
//			if (time2 > 59) {time2 -= 60; time1++;} //если 60 сек, то добавим 1 мин и обнулим секунды
//
//			if ( (AK == 1)||(AK == 3) )
//				time1 = 0; //если АК ускоренныЙ, то нужны только секунды
//			else
//			{
//			  	time1 -= (Time[3] - 0x30) * 10 + (Time[4] - 0x30);
//			  	if (time1 == 60)
//					time1 = 0;
//			}
//		}
//		break;
//
//		case 1:
//		case 2:
//		case 3:
//		case 4:
//		{
//			unsigned int a;
//
////			a = iTimeToAK - iTimeToAKnow;
//			a = iTimeToAK;
//			time2 = a % 60;
//			a /= 60;
//			time1 = a % 60;
//			a /= 60;
//			time3 = a % 60;
//		}
//		break;
//	}
//
//
//
//	// выведем время до АК
//	// если есть часы, не выводим "через"
//
//	if (time3 == 0)
//	{
//		LCDbuf[start1++] = 0xC0; // 'ч';
//		LCDbuf[start1++] = 0x65; // 'е';
//		LCDbuf[start1++] = 0x70; // 'р';
//		LCDbuf[start1++] = 0x65; // 'е';
//		LCDbuf[start1++] = 0xB7; // 'з';
//		LCDbuf[start1++] = ' ';
//	}
//	else
//	{
//
//		LCDbuf[start1++] = (time3 / 10) + '0';
//		LCDbuf[start1++] = (time3 % 10) + '0';
//		LCDbuf[start1++] = ':';
//	}
//	LCDbuf[start1++] = (time1 / 10) + '0';
//	LCDbuf[start1++] = (time1 % 10) + '0';
//
//	LCDbuf[start1++] = ':';
//	LCDbuf[start1++] = (time2 / 10) + '0';
//	LCDbuf[start1++] = (time2 % 10) + '0';
//
//  	for(; start1 < 60; start1++)
//		LCDbuf[start1] = ' ';  //очистим строку до конца
//}
//
//extern unsigned char *Measuring[];
//extern __flash unsigned __flash char* fAKdecr[];
////функция вывода Uk в главном меню, в чистой Защите
//void LCDprintUkDef(unsigned char AKdecrease)
//{
//  unsigned char i=0, j = 60, t;
//
//  if (AKdecrease>2) AKdecrease=2;
//
//  while(fAKdecr[AKdecrease] [i] != 0x00){
//    t=fAKdecr[AKdecrease] [i++];
//    if ((t>='А')&&(t<='я')) t =  CodeLetterLCD[t - 192];
//    LCDbuf[j++] = t;
//  }
//
//  i=3;
//
//  while(Measuring[5] [i] != 'д'){
//    LCDbuf[j++] = Measuring[5] [i++] ;
//  }
//
//  LCDbuf[j++]=CodeLetterLCD['д'-192];; //'д'
//  LCDbuf[j++]=CodeLetterLCD['Б'-192];;  //'Б'
//
//  for(; j<80; j++) LCDbuf[j]=' ';
//}
