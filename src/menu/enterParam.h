/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef ENTERPARAM_H_
#define ENTERPARAM_H_

#include "glbDefine.h"
#include "paramGlb.h"

// Настройки необходимые для ввода нового значения параметра.
class TEnterParam { 
    // Структура параметров для предыдущего ввода.
    struct last_t {
        eGB_PARAM param;
        eGB_COM com;
        int16_t val;
        uint16_t dopValue;
    };

public:
	/**	Конструктор.
	 */
    TEnterParam();

	/**	Проверка текущего статуса работы с параметром.
	 *
	 * 	@param Нет
	 * 	@return True - если идет ввод нового значения, иначе False.
	 */
	bool isEnable();

	/** Старт ввода нового значения параметра.
	 * 	 Если ввод из списка, автоматически дискретность и делитель равны 1.
	 * 	Для пароля автоматически выставляются диапазон ввода и начальное значение.
	 *
	 * 	@param s Режим работы функции ввода параметров.
	 */
	void setEnable(eMENU_ENTER_PARAM s = MENU_ENTER_PARAM_INT);

	/**	Окончание ввода нового значения параметра.
	 *
	 * 	@param Нет
	 * 	@return Нет
	 */
	void setDisable();

    /** Устанавливает диапазон значений.
     *
     *  Дополнительно вычисляется максимальное количество символов.
     *
     *  @param[in] min Минимум.
     *  @param[in] max Максимум.
     */
    void setValueRange(int16_t min, int16_t max);

    /** Возвращает минимальное значение параметра.
     *
     *  @return Минимальное значение.
     */
    int16_t getValueMin() const;

    /** Возвращает максимальное значение параметра.
     *
     *  @return Максимальное значение.
     */
    int16_t getValueMax() const;

    /** Устанавливает значение параметра.
     *
     *  Диапазон значений должен быть уже установлен!
     *
     *  @param[in] val Значение параметра.
     */
    void setValue(int16_t val);

    /** Возвращает текущее значение параметра.
     *
     *  @return Значение параметра.
     */
	int16_t getValue() const {
		return val_;
	}

    /** Возвращает текущее значение пароля.
     *
     *  @return Пароль.
     */
    uint8_t *getValuePwd();

    /// возвращает введеное значение с учетом дискретности и делителя
    int16_t getValueEnter() const;

	/** Увеличение текущего значения.
	 *
	 * 	@param velocity Скорость изменения значения (для ввода целых значений
	 * 	и коррекции напряжения).
	 * 	@argval 0 Увеличение на шаг заданный дискретностью.
	 * 	@argval 1 Увеличение на шаг в 10 раз больше заданной дискретности.
	 * 	@argval 2 Увеличение на шаг в 50 раз больше заданной дискретности.
	 */
	uint16_t incValue(uint8_t velocity = 0);

	/** Уменьшение текущего значения.
	 *
	 * 	@param velocity Скорость изменения значения (для ввода целых значений
	 * 	и коррекции напряжения).
	 * 	@argval 0 Увеличение на шаг заданный дискретностью.
	 * 	@argval 1 Увеличение на шаг в 10 раз больше заданной дискретности.
	 * 	@argval 2 Увеличение на шаг в 50 раз больше заданной дискретности.
	 */
	uint16_t decValue(uint8_t velocity = 0);

    /// Переход на разряд меньше.
    void decDigit();

    /** Возвращает номер выбранного разряда.
     *
     *  @return Номер разряда, начиная с 1.
     */
    uint8_t getDigit() const;

    /// Переход на разряд больше.
    void incDigit();

    /** Возвращает максимальное количество разрядов в числе.
     *
     *  @param[in] s Режим работы функции ввода параметров
     *  @return Максимальное количество разрядов в числе.
     */
    uint8_t getMaxDigitNumber(eMENU_ENTER_PARAM s) const;

    /** Возвращает минимальный значащий разряд.
     *
     *  @param[in] s Режим работы функции ввода параметров
     *  @return Минимальный значащий разряд.
     */
    uint8_t getMinDigitNumber(eMENU_ENTER_PARAM s) const;

	// установка номера параметра
	void setParam(eGB_PARAM param) {param_ = param; }

	// возвращает номер параметра
	eGB_PARAM getParam() const {return param_; }

	// установка дополнительного байта
	void setDopValue(uint16_t byte) { dopValue_ = byte; }

	// возвращает дополнительный байт
	uint16_t getDopValue() const { return dopValue_; }

	// установка дискретности
	void setDisc(uint16_t disc) { disc_ = disc; }

	// возвращает дискретность
	uint16_t getDisc() const { return disc_; }

	// установка делителя
	void setFract(uint8_t fract) { fract_ = fract; }

	// возвращает делитель
	uint8_t getFract() const { return fract_; }

	// возвращает текущий статус
	eMENU_ENTER_PARAM getStatus() const { return status_; }

	// установка флага окончания ввода параметра
	void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY) {
		status_ = status;

        if (param_ != GB_PARAM_IS_PWD) {
            saveSettings();
        }
    }

	// указатель на первый элемент списка
	PGM_P list;

	// указатель на массив значений
	uint8_t *listValue;

	// команда на передачу
	eGB_COM com;

    // Значения предыдущего ввода.
    last_t last;

private:
	// текущее значение
	int16_t val_;
    // значение пароля
    uint8_t pwd_[PWD_LEN+1];

	// максимальное значение
	int16_t max_;

	// минимальное значение
	int16_t min_;

	// байт с дополнительой информацией
	uint16_t dopValue_;

	// дискретность
	uint16_t disc_;

    // текущий выбранный разряд, начиная с 1
    uint8_t digit_;
    // максимальный разряд для изменения
    uint8_t digitMax_;
    // минимальный разряд для изменения
    uint8_t digitMin_;

	// делитель
	uint8_t fract_;

	// текущий статус устройства
	eMENU_ENTER_PARAM status_;

	// текущий параметр
	eGB_PARAM param_;

    /// Сбрасывает значение пароля в "ноль".
    void clearPwd();

    /// Сохраняет настройки ввода.
    void saveSettings();

    /// Очищает настройки ввода.
    void clearSetting();
};


#endif /* ENTERPARAM_H_ */
