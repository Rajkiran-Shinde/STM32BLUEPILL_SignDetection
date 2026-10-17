#include <Arduino.h>
#include "gesture.h"
#include "config.h"

void gestureInit() {
    // Reserved for recognition state and release-to-rearm logic.
}

GestureCommand gestureClassify(const float values[4], const float baseline[4]) {
    bool bent[4] = {false, false, false, false};
    uint8_t bentCount = 0;

    for (uint8_t i = 0; i < 4; ++i) {
        bent[i] = values[i] < (baseline[i] - FLEX_TRIGGER_DROPS[i]);
        if (bent[i]) ++bentCount;
    }

    if (bentCount >= 3) return GestureCommand::EMERGENCY;
    if (bent[0]) return GestureCommand::COFFEE;
    if (bent[1]) return GestureCommand::WATER;
    if (bent[2]) return GestureCommand::FOOD;
    if (bent[3]) return GestureCommand::WASHROOM;
    return GestureCommand::NONE;
}
