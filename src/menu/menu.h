/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "keyboard.h"
#include "glbDefine.h"
#include "paramBsp.h"
#include "LocalParams.h"
#include "menuPunkt.h"
#include "enterParam.h"

/// время до первой инициализации дисплея, мс
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)

/// время до переинициализации дисплея, мс
#define TIME_TO_REINIT_LCD (10000UL / MENU_TIME_CYLCE)

/// максимальное кол-во отображаемых на экране параметров
#define MAX_NUM_MEAS_PARAM 6

/// максимальное кол-во отображаемых параметров в меню "Измерение"
#define MAX_NUM_MEAS_PARAM_LVL 10

/// время вывода измеряемого параметра на экран, мс
#define TIME_MEAS_PARAM (3000UL / MENU_TIME_CYLCE)

/// время вывода текста на экран, мс (например авария/код аварии)
#define TIME_TEXT (1000UL / MENU_TIME_CYLCE)

/// время до выхода из Тест 1 / Тест 2 и т.д. если режим != данному тесту, мс
#define TIME_TEST_EXIT (1000UL / MENU_TIME_CYLCE)

/// период проверки необходимости сохранения параметров защиты, мс
#define TIME_SECURITY (1000UL / MENU_TIME_CYLCE)

/// Измеряемые параметры
enum eMENU_MEAS_PARAM {
    MENU_MEAS_PARAM_NO,         ///< нет измеряемого параметра
    MENU_MEAS_PARAM_DATE,       ///< дата
    MENU_MEAS_PARAM_TIME,       ///< время
    MENU_MEAS_PARAM_UZ,         ///< запас по защите
    MENU_MEAS_PARAM_UZ1,        ///< запас по защите 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UZ2,        ///< запас по защите 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UC,         ///< запас по КЧ
    MENU_MEAS_PARAM_UC1,        ///< запас по КЧ 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UC2,        ///< запас по КЧ 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UN,         ///< уровень шумов
    MENU_MEAS_PARAM_UN1,        ///< уровень шумов 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UN2,        ///< уровень шумов 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UOUT,       ///< напряжени выхода
    MENU_MEAS_PARAM_IOUT,       ///< ток выхода
    MENU_MEAS_PARAM_ROUT,       ///< сопротивление линии
    MENU_MEAS_PARAM_SD,         ///< просечки в сигнале
    MENU_MEAS_PARAM_D,          ///< запас по тест.команде (двухчаст) или отношение сигнал/помеха (одночаст)
    MENU_MEAS_PARAM_TEMPERATURE,///< температура на плате БСП-ПИ
    MENU_MEAS_PARAM_DF          ///< отклонение частоты КС на ПРМ (К400)
};

// класс меню
class clMenu {
    /// Сообщение.
    enum msg_t {
        MSG_NO = 0,             ///< Сообщений нет.
        MSG_WRONG_REGIME,       ///< Параметр изменяется в режиме "Выведен".
        MSG_WRONG_USER_EDIT,    ///< Недостаточно прав для изменения.
        MSG_WRONG_USER_READ,    ///< Недостаточно прав для просмотра.
        MSG_WRONG_PWD,          ///< Введен не верный пароль.
        MSG_WRONG_DEVICE,       ///< Тип аппарата не определен.
        MSG_BLOCK_USER,         ///< Выбор роли пользователя заблокирован.
        MSG_DISABLE,            ///< Нужно изменить режим на "Выведен".
        MSG_INIT,               ///< Инициализация.
        MSG_RESET_PWD,          ///< Сброс паролей до заводских настроек.
        //
        MSG_MAX
    };

public:

    /** Конструктор.
     *  Устнанавливается по умолчанию неопределеныый тип аппарата.
     *  Заполняются необходимые для работы массивы.
     *  Производится переход на стартовый уровень меню.
     */
    clMenu();

    /** Работа с меню.
     */
    void proc();

    /** Установка типа аппарата и настройка меню с его учетом.
     *  По умолчанию будет сделан выбор исходя из меющейся информации в
     *  настройках отдельных устройств.
     *  @param device Тип устройства
     *  @return False в случае ошибки, иначе True
     */
    bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

    /** Возвращает имеющуюся команду на исполнение.
     *
     *  Сначала проверяется срочная команда, если ее нет идет перебор текущих.
     *
     *  Каждый цикл состоящий из 8 команд опрашиваются:
     *  - текущее состояние;
     *  - время;
     *  - команда из буфера 2
     *      - первым всегда идет текущий параметр, если есть
     *      - затем команды необходимые для настройки меню (совместимость и т.д.)
     *  - команда из буфера 1
     *      команды опроса остальных параметров в данном уровне меню
     *  - если команд нет, то счетчик цикла сбрасывается.
     *
     *  @param Нет
     *  @return Команда
     */
    eGB_COM getTxCommand();

    /** Возвращает текущее состояние связи с БСП.
     *
     *  @return true если есть связь с БСП, иначе false.
     */
    bool isConnectionBsp() const {
        return connectionBsp_;
    }

    /** Установливает флаг наличия связи с БСП.
     *
     *  @param[in] connection Флаг наличия связи.
     */
    void setConnectionBsp(bool connection) {
        connectionBsp_ = connection;
    }

    /** Возвращает текущее состояние связи с ПК.
     *
     *  @return true если есть связь с ПК, иначе false.
     */
    bool isConnectionPc() const {
        return connectionPc_;
    }

    /** Установливает флага наличия связи с ПК.
     *
     *  @param[in] connection Флаг наличия связи.
     */
    void setConnectionPc(bool connection) {
        connectionPc_ = connection;
    }

    /** Параметры
     */
    stGBparam sParam;

private:
        // код кнопки
    eKEY key_;

    // флаг мигания надписей
    bool blink_;

    // флаг смены измеряемых параметров
    bool blinkMeasParam_;

    // флаг текущего сосотояния связи с БСП, true - есть
    bool connectionBsp_;
    // флаг текущего состояния связи с ПК, true - есть
    bool connectionPc_;

    // true - необходимо вывести на экран курсор
    bool cursorEnable_;

    // текущее положение курсора (номер строки)
    uint8_t cursorLine_;

    // кол-во отображаемых параметров
    uint8_t lineParam_;

    // текущий подпункт (например номер текущей команды параметра, начиная с 1)
    uint8_t curCom_;

    // если true - необходимо создать уровень меню
    bool lvlCreate_;

    // время вывода доп.сообщения на экран (например сообщения об ошибке)
    msg_t msg_;
    uint8_t delay_;

    // измеряемые параметры
    eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM * 2];

    // измеряемые параметры для уровня меню "Измерения"
    eMENU_MEAS_PARAM measParamLvl[MAX_NUM_MEAS_PARAM_LVL];

    // текущие пункты меню
    TMenuPunkt Punkts_;

    // параметры для ввода новых значений
    TEnterParam EnterParam;

    //  настройки для соответствующих аппаратов
    bool setDeviceK400();
    bool setDeviceRZSK();
    bool setDeviceR400M();
    bool setDeviceOPTO();

    // очистка текстового буфера
    void clearTextBuf();

    // очистка строки
    void clearLine(uint8_t line);

    /// Сброс сообщений.
    void clrMessages();

    /** Добавление сообщения.
     *
     *  @param[in] msg Сообщение.
     */
    void setMessage(msg_t msg);

    /** Выводит сообщения на экран
     *
     *  @return True если сообщение выведено на экран.
     */
    bool printMessage();

    /** Проверяет наличие сообщений.
     *
     *  @return true если есть сообщение.
     */
    bool isMessage() const;

        // вывод на экран измеряемого параметра
    void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

    // вывод на экран текущего состояния устройств
    void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

    // вывод в пунтке меню "Режим" текущего режима устройств
    void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

    // Уровни меню
    void lvlError();
    void lvlStart();
    void lvlFirst();
    void lvlInfo();
    void lvlJournal();
    void lvlJournalEvent();
    void lvlJournalDef();
    void lvlJournalPrm();
    void lvlJournalPrd();
    void lvlJournalSecurity();
    void lvlControl();
    void lvlSetup();
    void lvlRegime();
    void lvlSetupParam();
    void lvlSetupParamDef();
    void lvlSetupParamPrm();
    void lvlSetupParamPrd();
    void lvlSetupParamGlb();
    void lvlSetupParamRing();
    void lvlSetupDT();
    void lvlMeasure();
    void lvlTest();
    void lvlTest1();
    void lvlTest2();
    void lvlSetupInterface();
    void lvlUser();

    /** Ввод параметра.
     *
     *  @return True - по окончанию
     */
    eMENU_ENTER_PARAM inputValue();

    // перемещение курсора вверх
    void cursorLineUp() {
        cursorLine_=(cursorLine_>1)? cursorLine_-1 : Punkts_.getMaxNumPunkts();
    }

    // пермещение курсора вниз
    void cursorLineDown() {
        cursorLine_=(cursorLine_<Punkts_.getMaxNumPunkts())? cursorLine_+1 : 1;
    }

    // вывод на экран текущих пунктов меню и курсора
    void printPunkts();

    /// Вывод на экран текущего параметра.
    void printParam();

    /// Вывод на экран названия параметра.
    void printName();

    /// Вывод на экран текущего номера и их колчиество для однотипных пар-ов.
    void printSameNumber(uint8_t pos);

    /// Вывод на экран диапазона значений параметра.
    void printRange(uint8_t pos);

    /** Выводит курсор при вводе нового значения.
     *
     *  @param[in] start Начальная позиция вывода значения.
     *  @param[in] stop Конечная позиция вывода значения.
     */
    void printCursor(uint8_t start, uint8_t stop);

    /** Вывод на экран текущего значения параметра.
     *
     *  Если значение параметра записано с ошибкой, будет поочердено выводится
     *  предупреждающая надпись и текущее значение параметра.
     *
     *  Для строковых параметров учитывается минимальное значение. Т.е. из
     *  текущего значения параметра вычитается минимальное значение, а затем
     *  осуществляется выбор из массива строк значений параметра.
     */
    void printValue(uint8_t pos);

    /** Выводит на экран время и дату для записи журнала.
     *
     *  @param[in] pos Начальная позиция на экране.
     *  @return Количество используемых символов.
     */
    uint8_t printDateAdnTimeInJournal(uint8_t pos);

    /** Проверяет режима работы на возможность изменения параметра.
     *
     *  @return True если можно менять.
     */
    bool checkChangeReg() const;

    /** Проверяет введенный пароль.
     *
     *  @param[in] user Роль пользователя для которого вводился пароль.
     *  @param[in] pwd Пароль для проверки.
     *  @return true если верный пароль, иначе false.
     */
    bool checkPwdInput(user_t user, const uint8_t *pwd);

    /** Проверка доступа на уровень меню для указанного пользователя
     *
     *  Уровни:
     *  - lvlJournalSecurity, нужен администратор
     *  - lvlTest1, нужны инженер или администратор;
     *  - lvlTest2, нужны инженер или администратор
     *
     *  В случае если доступ не пройден, будет установлен код сообщения.
     *
     *  @param[in] lvl Уровень меню.
     *  @return
     */
    bool checkUserReq(void (clMenu::*lvl)());

    /** Проверяет необходимость ввода пароля после изменения параметра.
     *
     *  Ввести пароль необходимо:
     *  - при смене роли, кроме выбора Оператора.
     *
     *  @param[in] param Параметр
     *  @param[in] value Знаение параметра.
     *  @return Необходимый пользователь для изменения.
     */
    user_t checkPwdReq(eGB_PARAM param, int16_t value) const;

    /** Настройка параметров для ввода значения с клавиатуры.
     *
     *  Для параметров типа \a Param::PARAM_NO функция ввода значения не
     *  вызывается.
     *
     *  Для параметров типа \a Param::PARAM_LIST вызывается функция выбора
     *  значения из списка.
     *
     *  Для параметров типа \a Param::PARAM_INT вызывается функция ввода
     *  целого числа.
     *
     *  Для параметров типа \a Param::PARAM_U_COR вызывается функция ввода
     *  коррекции напряжения, а также минимальное значение подменяется на 0.
     *
     *  Для параметров типа \a Param::PARAM_I_COR вызывается функция ввода целого
     *  числа, при этом минимальное значение подменяется на 0.
     *
     *  Для параметров типа \a Param::PARAM_BITES вызывается функция выбора
     *  значения из списка.
     */
    void enterParameter();

    /// Формирование списка локальных параметров для Защиты.
    void crtListOfParamsDef();

    /// Формирование списка локальных параметров для Приемника.
    void crtListOfParamsPrm();

    /// Формирование списка локальных параметров для Передатчика.
    void crtListOfParamsPrd();

    /// Формирование списка локальных параметров для Общие.
    void crtListOfParamsGlb();

    // текущая функция ввода
    eMENU_ENTER_PARAM (clMenu::*enterFunc)();

    // текущий уровень меню
    void (clMenu::*lvlMenu)();

    /// Сохраняет параметр.
    void saveParam();

    /** Сохраняет параметр в БСП.
     *
     *  Формируется команда для передачи в БСП.
     */
    void saveParamToBsp();

    /// Сохранят параметр в оперативной памяти.
    void saveParamToRam();

    /// Работа блока информационной безопасности.
    void security();

    /** Работа в меню настройки параметров.
     *
     *  Если текущее количество параметров равно 0, то будет осуществлен возврат
     *  к предыдущему уровню.
     */
    void setupParam();    

    /** Проверка необходимости в подсветке.
     *
     *  Необходимость включения подсветки проводится для каждого имеющегося
     *  устройства.
     *
     *  ОБЩ:
     *  - режим не "Введен";
     *  - имеется неисправность или предупреждение.
     *  ЗАЩ:
     *  - состояние не "Контроль";
     *  - имеется неисправность или предупреждение.
     *  ПРМ:
     *  - состояние не "ПРМ КЧ";
     *  - имеется неисправность или предупреждение.
     *  ПРД:
     *  - состояние не "ПРД КЧ";
     *  - имеется неисправность или предупреждение.
     *
     *  @retval True Включить подсветку.
     *  @retval False Необходимости в подсветке нет.
     */
    bool checkLedOn();
};

#endif /* MENU_H_ */
