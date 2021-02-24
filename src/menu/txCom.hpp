/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "src/glbDefine.h"

/// максимальное кол-во команд в первом буфере
#define MAX_NUM_COM_BUF1 10

///	максимальное кол-во быстрых команд
#define MAX_NUM_FAST_COM 8

/// максимальное кол-во команд во втором буфере
#define MAX_NUM_COM_BUF2 4

/// минимальное кол-во команд передаваемых за один цикл
// 3 = (команда состояния + команда времени + команда из буфера1 + локальная команда)
#define MAX_NUM_COM_SEND_IN_CYLCE (MAX_NUM_COM_BUF2 + 4)

//
template<typename T, uint8_t size, T defvalue, bool isend>
class buf {
    uint8_t cnt;
    uint8_t len;
    T buffer[size];

  public:
    buf ();
    void clear();
    bool set(T value);
    T get();
};

// класс для передачи команд
class TTxCom {
	static const uint8_t BUFFER_SIZE = 16;

    typedef struct {
        eGB_SEND_TYPE type;
        eGB_COM com;
        uint8_t dopByte;
        uint8_t buf[BUFFER_SIZE];
    } buf_t;

public:
	TTxCom() {
		clear();
        clearCom1();
        clearFastCom();
	}

	// очистка буфера
	void clear();

    /** Записывает команду в буфер 1.
     *
     *  Команды используются для постоянного опроса, независимо от текущего
     *  уровня меню.
     *  Устанавливаются при формировании меню для устройства.
     *
     * 	@param[in] com Код команды.
     * 	@param[in] last Флаг замены последней команды в буфере.
     *  @return true в случае успешной записи, иначе false.
	 */
    bool addCom1(eGB_COM com);

	/** Последовательная выдача имеющихся в буфере 1 команд (по кругу).
     *
     *  Команды выдаются по кругу. На вызове следующем после передачи
     *  "последней" команды в буфере, будет возвращена команда из начала
     *  буфера.
     *
	 * 	@return Код текущей команды.
	 */
    eGB_COM getCom1();

    /// Очистка буфера команд 1.
    void clearCom1();

	/** Запись команды в буфер 2.
	 * 	@param com Код команды.
	 * 	@retval True - в случае успешной записи.
	 * 	@retval False - если не удалось поместить команду в буфер.
	 */
    bool addCom2(eGB_COM com);

	/** Последовательная выдача имеющихся в буфере 2 команд (по кругу).
     *
     *  После передачи "последней" команды в буфере, будет возвращена
     *  команда GB_COM_NO. При последующем вызове будет передана команда
     *  из начала буфера.
     *
	 * 	@return Код текущей команды.
	 */
    eGB_COM getCom2();

    /** Добавляет команду в группу срочных команд.
	 *
     *	После добавления команды нужно записать его содержимое!
	 *
     * 	@param[in] com Команда.
     *  @param[in] type Тип команды.
     *  @return true если команда добавлена, иначе false.
	 */
    bool addFastCom(eGB_COM com, eGB_SEND_TYPE type);

    /** Устанавливает дополнительный байт данных для последней быстрой команды.
     *
     *  @param[in] dopbyte Дополнительный байт.
     *  @return true если байт добавлен, иначе false.
     */
    bool setFastComDopByte(uint8_t dopbyte);

    /**	Возвращает код срочной команды.
     *
	 * 	@return Код срочной команды.
	 */
    eGB_COM getFastCom() const;

    /** Возвращает тип срочной команды.
     *
     *  @return Тип срочной команды.
     */
    eGB_SEND_TYPE getFastComType() const;

    /// Убирает последнюю быструю команду из очереди.
    void removeFastCom();

    /// Очищает очередь быстрых команд.
    void clearFastCom();

    /** Возвращает дополнительный байт для срочной команды.
    *
    *   @return Дополнительный байт.
    */
    uint8_t getFastComDopByte() const;

    /**	Записывает байт данных в буфер.
     *
     * 	@param[in] byte	Байт данных.
     * 	@param[in] pos Индекс элемента буфера данных.
	 */
    void setInt8(uint8_t byte, uint8_t pos = 0);

    /** Возвращает значение заданного элемента из буфера срочной команды.
     *
     * 	@param[in] pos Индекс элемента буфера данных.
	 * 	@return Байт данных.
	 */
    uint8_t getUInt8(uint8_t pos) const;

	/**	Запись 2-х байтного числа (uint16_t) в буфер.
	 * 	В буфере данные записываются в 1 и 2 элемент массива uint8_t.
	 * 	@param val Данные.
	 */
	void setInt16(uint16_t val);

    void setInt16BE(int16_t val);

    /** Считывание 2-х абйтного числа (int16_t) из буфера.
	 * 	Данные хранятся в 1 и 2 элементах массива uint8_t.
	 * 	@return Данные.
	 */
    int16_t getInt16() const;

    /** Записывает массив данных в буфер.
     *
     *  @param[in] array Данные.
     *  @param[in] size Количество данных.
     *  @param[in] pos Начальная позиция в буфере записи.
     */
    void setArray(const uint8_t *array, uint8_t size, uint8_t pos=0);

    /**	Возвращает указатель на буфер данных.
     *
     * 	@return Указатель на буфер данных.
     */
    uint8_t* getBuferAddress();

    /** Устанавливает локальную команду.
     *
     *  Если команда GB_COM_NO, то смены локальной команды не произойдет.
     *
     *  @param[in] com Команда.
     */
    void setLocalCom(eGB_COM com) {
        if (com != GB_COM_NO) {
            localCom = com;
        }
    }

    /** Возвращает локальную команду.
     *
     *  @return Команда.
     */
    eGB_COM getLocalCom() const {
        return localCom;
    }
private:
    // количество команд в буфере срочных команд
    uint8_t cntComFast;
    buf_t comFast_[MAX_NUM_FAST_COM];

    eGB_COM localCom;
    buf<eGB_COM, MAX_NUM_COM_BUF1, GB_COM_NO, false> bufCom1;
    buf<eGB_COM, MAX_NUM_COM_BUF2, GB_COM_NO, true> bufCom2;

    /** Очищает содержимое в буфере быстрой команды для указанной позиции.
     *
     *  @param[i] pos Позиция [0..SIZE_OF(comFast_)).
     */
    void clearFastCom(uint8_t pos);
};

template<typename T, uint8_t size, T defvalue, bool isend>
buf<T, size, defvalue, isend>::buf() {
}

template<typename T, uint8_t size, T defvalue, bool isend>
void  buf<T, size, defvalue, isend>::clear() {
    len = 0;
    cnt = 0;
    for(uint8_t i = 0; i < size; i++) {
        buffer[i] = defvalue;
    }
}

template<typename T, uint8_t size, T defvalue, bool isend>
bool  buf<T, size, defvalue, isend>::set(T value) {
    if (len < size) {
        buffer[len++] = value;
        return true;
    } else {
        QDEBUG("Command adding error. Buffer is full!");
    }
    return false;
}

template<typename T, uint8_t size, T defvalue, bool isend>
T  buf<T, size, defvalue, isend>::get() {
    T value = defvalue;
    if (cnt < len) {
        value = buffer[cnt++];
    } else {
        cnt = 0;
        if (!isend) {
            value = buffer[cnt++];
        }
    }
    return value;
}

#endif /* TX_COM_HPP_ */
