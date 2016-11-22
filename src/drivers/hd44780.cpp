#include <avr/io.h>
#include <avr/pgmspace.h>
//#include <util/delay.h>


#include "debug.h"
#include "hd44780.h"

/// размер массива
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

// структура команд инициализации ЖКИ
typedef struct {
	uint8_t com;	// команда для передачи в ЖКИ
	uint16_t time;	// время ожидания после передачи команды в ЖКИ
} SComInit;

// таблица преобразования русских символов в коды ЖКИ
static const uint8_t decode[] PROGMEM = {
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
volatile bool bRefresh = false;
/// флаг необходимости переинициализировать дисплей
volatile bool bReInit = false;


/// буфер данных для вывода на ЖКИ
char *buf;

void sendData(uint8_t val);	//	Отправка байта данных в ЖКИ.
void sendCom(uint8_t val);	//	Отправка команды в ЖКИ.

// Инициализация дисплея.
void vLCDinit(void) {
	bReInit = true;
};

// Очистка содержимого буфера
//void vLCDclear(void) {
//	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
//		buf[i] = ' ';
//	}
//}

// Основная функция обновления содержимого ЖКИ.
void vLCDmain(void) {
	// массив команд инициализации ЖКИ
	static const SComInit comInit[] PROGMEM = {
			// переинициализация
			{0x38,  50},	// 8-бит, 2 линии и ждем после 5мс
			{0x38,	10},	// 8-бит, 2 линии и ждем после 1мс
			{0x38, 	2},		// 8-бит, 2 линии и ждем после 0.2мс
			{0x14, 	2},		// сдвиг вправо DDRAM и ждем после 0.2мс
			{0x06, 	2},		// инкремент счетчика DDRAM и ждем после 0.2мс
			{0x02, 	40},	// адрес DDRAM и свдиг в 0 и ждем после 4мс
			{0x0C, 	2}		// вкл. дисплея и откл. курсора и ждем после 0.2мс
	};

	// флаг процесса инициализации
	static bool init = true;
	// текущее положение в буфере символов или инициализации
	// при старте должен быть 0, чтобы инициализация пошла с начала
	static uint8_t cnt = 0;
	// команда на передачу, должна быть 0
	static uint8_t com = 0;
	// время ожидания до передачи команды (40 мс) после включения
	static uint16_t time = 400;

	// формирование паузы после переданной команды
	if ((time > 0) && (--time > 0)) {
		return;
	}

	// проверка необходимости переинициализации
	if (bReInit) {
		init = true;
		cnt = 0;
		bReInit = false;
	}

	// инициализация ЖКИ
	if (init) {
		if (cnt < SIZE_OF(comInit)) {
			com = pgm_read_byte(&comInit[cnt].com);
			time = pgm_read_byte(&comInit[cnt].time);
			cnt++;
		} else {
			init = false;
			cnt = 0;
		}
	}


	if (com != 0) {
		// передача команды
		sendCom(com);
		com = 0;
	} else if (bRefresh) {
		// обновление содержимого ЖКИ

		// отправка байта данных
		sendData(buf[cnt]);

		// завершение вывода данных в ЖКИ
		if (++cnt >= SIZE_BUF_STRING) {
			cnt = 0;
			bRefresh = false;
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

// Начало обновления содержимого ЖКИ.
void vLCDrefresh(void) {
	bRefresh = true;
}

// Преобразование массива символов, в данные для вывода на ЖКИ.
void vLCDputchar(char *b) {
	buf = b;
	for(uint8_t i = 0; i < SIZE_BUF_STRING; i++) {
		if ((buf[i] >= 'А') && (buf[i] <= 'я')) {
			buf[i] = pgm_read_byte(&decode[buf[i] - 'А']);
		} else if (buf[i] == 0x00) {
			buf[i] = ' ';
		}
		// символ '°' сделан в ЖКИ довольно коряво, поэтому не используем
//		else if (buf[i] == 0xB0) {
//			buf[i] = 0xEF;	// символ градуса '°'
//		}
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
