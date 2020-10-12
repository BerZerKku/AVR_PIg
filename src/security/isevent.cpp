#include "isevent.h"

//
TIsEvent::TIsEvent() {
    COMPILE_TIME_ASSERT((sizeof(event_) * CHAR_BIT) >= IS_EVENT_BIT_MAX);
    event_ = 0;
}
