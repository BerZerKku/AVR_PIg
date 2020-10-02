/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "glbDefine.h"

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
	}

	// очистка буфера
	void clear();

    /** Записывает команду в буфер 1.
     *
     * 	@param[in] com Код команды.
     * 	@param[in] last Флаг замены последней команды в буфере.
     *  @return true в случае успешной записи, иначе false.
	 */
    bool addCom1(eGB_COM com, bool last=false);

	/** Последовательная выдача имеющихся в буфере 1 команд (по кругу).
     *
     *  Команды выдаются по кругу. На вызове следующем после передачи
     *  "последней" команды в буфере, будет возвращена команда из начала
     *  буфера.
     *
	 * 	@return Код текущей команды.
	 */
    eGB_COM getCom1();

    /** Возвращает последнюю команду в буфере команд 1.
     *
     *  @return Последняя команда.
     *  @retval GB_COM_NO В случае если буфер пуст.
     */
    eGB_COM lastCom1() const;

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

    /** Проверяет наличие каоманды в буфере быстрых команд.
     *
     *  @param[in] com Команда
     *  @return true если есть команда в буфере, иначе false.
     */
    bool containsFastCom(eGB_COM com) const;

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

    /** Очищает содержимое в буфере быстрой команды для указанной позиции.
     *
     *  @param[i] pos Позиция [0..SIZE_OF(comFast_)).
     */
    void clearFastCom(uint8_t pos);

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

private:
    // количество команд в буфере срочных команд
    uint8_t cntComFast;
    buf_t comFast_[MAX_NUM_FAST_COM];

	// первый буфер команд
	eGB_COM com1_[MAX_NUM_COM_BUF1];
	// кол-во команд в первом буфере
	uint8_t numCom1_;
	// номер текущей команды в первом буфере
	uint8_t cnt1_;

	// второй буфер команд
	eGB_COM com2_[MAX_NUM_COM_BUF2];
	// кол-во команд во втором буфере
	uint8_t numCom2_;
	// номер текущей команды во втором буфере
	uint8_t cnt2_;
};

#endif /* TX_COM_HPP_ */
