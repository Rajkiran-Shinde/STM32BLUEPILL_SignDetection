#pragma once
#include <Arduino.h>

enum class AppState : uint8_t {
    IDLE,
    CALIBRATING,
    ACTIVE,
    COMMAND_DISPLAY,
    STOPPING,
    ERROR
};

enum class GestureCommand : int8_t {
    NONE = -1,
    COFFEE = 0,
    WATER = 1,
    FOOD = 2,
    WASHROOM = 3,
    EMERGENCY = 4
};

enum class OperatingMode : uint8_t {
    SPEECH,
    SILENT
};
