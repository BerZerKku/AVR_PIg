#include "param.h"

/** ���������� ��������� �� ��������� ���������.
 *
 * @param[in] params ������ ����������.
 * @param[in] pn ��������.
 * @return ��������� �� ��������� ���������.
 */
Param const * getPtrParam(eGB_PARAM pn) {
    return (Param const*) (pgm_read_ptr(&fParams[pn]));
}

/** ���������� ���������� �������� �������� ���������.
 *
 *  ������� �������� ����� �������� �� ������ ����������.
 *
 *  @param[in] pn ��������.
 * 	@return ���������� �������� �������� ���������.
 */
int16_t getAbsMax(eGB_PARAM pn) {
    return static_cast<int16_t> (pgm_read_word(&getPtrParam(pn)->max));
}

/** ���������� ���������� �������� ���������� ���������� ����������.
 *
 *  ������� ���������� ����� �������� �� ������ ����������.
 *
 *  @param[in] pn ��������.
 * 	@return ���������� �������� ���������� ���������� ����������.
 */
uint8_t getAbsMaxNumOfSameParams(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->num);
}

/** ���������� ���������� � ������ ��� ��������� ���������.
 *
 *  @param[in] pn ��������.
 *  @return ���������� � ������ ��� ��������� ���������.
 */
Param::CHANGE_REG getChangeReg(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->changeReg);
    return static_cast<Param::CHANGE_REG> (value);
}

/**	���������� ������� ������������ ��������� ��� ��������.
 *
 *  @param[in] pn ��������.
 * 	@return ������� ������������ ���������.
 */
eGB_COM getCom(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->com);
    return static_cast<eGB_COM> (value);
}

/**	���������� ������������ ������������ ��� ��������� ���������.
 *
 *  @param[in] pn ��������.
 *	@return ������� ��� ��������� ���������.
 */
user_t getChangeUser(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->changeUser);
    return static_cast<user_t> (value) ;
}

/** ���������� ����������� ��������� ��� ���������.
 *
 *  @param[in] pn ��������.
 *  @return ����������� ��������� ��� ���������.
 */
Param::DEPEND_MAX getDependMax(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dependMax);
    return static_cast<Param::DEPEND_MAX> (value);
}

/**	���������� ����������� ���������� ��� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ����������� ���������� ��� ���������.
 */
Param::DEPEND_SAME getDependSame(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dependSame);
    return static_cast<Param::DEPEND_SAME> (value);
}

/**	���������� ����������� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ����������� ���������.
 */
Param::DIMENSION getDim(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dim);
    return static_cast<Param::DIMENSION> (value);
}

/** ���������� ������������ ��� �������� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ������������ ��� �������� ���������.
 */
uint8_t getDisc(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->disc);
}

/** ���������� ��������� �������� ��� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ��������� ��� �������� ���������.
 */
uint8_t getFract(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->fract);
}

/** ���������� ��������� �� ������ ������ �������� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ��������� �� ������ ������ ��������.
 */
PGM_P getListOfValues(eGB_PARAM pn) {
    return static_cast<PGM_P> (pgm_read_ptr(&getPtrParam(pn)->listValues));
}

/** ���������� ����������� �������� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ����������� �������� ���������.
 */
int16_t getMin(eGB_PARAM pn) {
    return static_cast<int16_t> (pgm_read_word(&getPtrParam(pn)->min));
}

/** ���������� ��������� �� ������ � ��������� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ��������� �� ������ � ��������� ���������.
 */
PGM_P getNameOfParam(eGB_PARAM pn) {
    return (PGM_P) &getPtrParam(pn)->name;
}

/** ���������� ��� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ��� ���������.
 */
Param::PARAM_TYPE getParamType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->param);
    return static_cast<Param::PARAM_TYPE> (value);
}

/**	���������� ��� ��������� �������� ��� ���������.
 *
 *  @param[in] pn ��������.
 * 	@return ��� ��������� ��������.
 */
Param::RANGE_TYPE getRangeType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->range);
    return static_cast<Param::RANGE_TYPE> (value);
}

/**	���������� �������� ����� ���. ���������� ��� ���������� ������ ��������.
 *
 *  @param[in] pn ��������.
 * 	@return �������� ����� ���. ����������.
 */
uint8_t getSendDop(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->sendDop);
}

/**	���������� ��� ��������� ��� ���������� ������ ��������.
 *
 *  @param[in] pn ��������.
 * 	@return ��� ��������� ��� ����������.
 */
eGB_SEND_TYPE getSendType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->send);
    return static_cast<eGB_SEND_TYPE> (value);
}
