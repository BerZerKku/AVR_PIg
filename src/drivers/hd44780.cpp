#include <avr/io.h>
#include <util/delay.h>

#include "debug.h"
#include "hd44780.h"

// таблица преобразования русских символов в коды ЖКИ
static const uint8_t decode[] = {
		//A	  Б    В	Г    Д    Е    Ж    З    И    й    К    Л    М    Н    О    П
		0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
		//Р   С    Т    У    Ф    Х    Ц    Ч    Ш    Щ    Ъ    Ы    Ь    Э    Ю    Я
		0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
		//а   б    в    г    д    е    ж    з    и    й    к    л    м    н    о    п
		0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
		//р   с    т    у    ф    х    ц    ч    ш    щ    ъ    ы    ь    э    ю    я
		0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};

/// флаг необходимости обновить данные на ЖКИ
volatile uint8_t bRefresh = 0;

/// буфер данных для вывода на ЖКИ
char *buf;

void sendData(uint8_t val);	//	Отправка байта данных в ЖКИ.
void sendCom(uint8_t val);	//	Отправка команды в ЖКИ.

// Инициализация дисплея.
void vLCDinit(void) {
	static volatile  uint8_t poweron = 0;

	// при включении идет последовательность команд, которые при повторной
	// инициализации уже не нужны.
	if (poweron == 0) {
		_delay_ms(150);
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
};

// Очистка содержимого буфера
//void vLCDclear(void) {
//	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
//		buf[i] = ' ';
//	}
//}

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
				com = 0xD4;
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
			buf[i] = decode[buf[i] - 'А'];
		} else if (buf[i] == 0x00) {
			buf[i] = ' ';
		} else if (buf[i] == 0xB0) {
			buf[i] = 0xEF;	// символ градуса '°'
		}
	}
}

/**	Отправка байта данных в ЖКИ.
 *
 * 	@param val Код символа в таблице знаков ЖКИ.
 */
void sendData(uint8_t val) {
	PORT_RS	|=  (1 << PIN_RS);
	PORT_ES	|=  (1 << PIN_ES);
	PORT_DB	 = val;
	PORT_ES	&= ~(1 << PIN_ES);
}

/**	Отправка команды в ЖКИ.
 *
 * 	@param val Команда.
 *
 */
void sendCom(uint8_t val) {
	PORT_RS	&= ~(1 << PIN_RS);
	PORT_ES	|=  (1 << PIN_ES);
	PORT_DB	 = val;
	PORT_ES	&= ~(1 << PIN_ES);
}
