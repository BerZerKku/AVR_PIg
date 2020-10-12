#ifndef TISEVENT_H
#define TISEVENT_H

#include "glbDefine.h"

class TIsEvent {
public:
    // события для журнала ИБ
    enum IS_EVENT_BIT {
        PWD_CHANGE = 0, // пароль изменен
        PWD_IN_ERROR,   // пароль введен неверно
        PWD_BLOCK,      // блокировка ввода пароля
        USR_IN,         // вход пользователя
        USR_OUT,        // выход пользователя
        USR_OUT_AUTO,   // выход пользователя автоматический
        //
        IS_EVENT_BIT_MAX
    };

    /// Конструктор.
    TIsEvent();

    /** Установка флага события.
     *
     *  Флаг добавляется к текущему значению событий.
     *
     * @param event Событие.
     * @return Флаги событий.
     */
    uint8_t set(IS_EVENT_BIT event) {
        if (event < IS_EVENT_BIT_MAX) {
            event_ |= (1 << event);
        }
        return event_;
    }

    /** Возвращает текущее состояние флагов событий.
     *
     *  После считывания флаги очищаются.
     *
     *  @return Флаги событий.
     */
    uint8_t get() {
        uint8_t tevent = event_;
        event_ = 0;
        return tevent;
    }

private:
    uint8_t event_;
};

#endif // TISEVENT_H
