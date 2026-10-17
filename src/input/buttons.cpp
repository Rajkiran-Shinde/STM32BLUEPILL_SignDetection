#include <Arduino.h>
#include "buttons.h"
#include "config.h"

namespace {
uint8_t lastMode = HIGH;
uint8_t lastStartStop = HIGH;
uint32_t lastModeChange = 0;
uint32_t lastStartStopChange = 0;
}

void buttonsInit() {
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
}

bool modeButtonPressed() {
    uint8_t current = digitalRead(MODE_BUTTON_PIN);
    uint32_t now = millis();
    bool pressed = (lastMode == HIGH && current == LOW &&
                    now - lastModeChange >= BUTTON_DEBOUNCE_MS);
    if (current != lastMode) lastModeChange = now;
    lastMode = current;
    return pressed;
}

bool startStopButtonPressed() {
    uint8_t current = digitalRead(START_STOP_BUTTON_PIN);
    uint32_t now = millis();
    bool pressed = (lastStartStop == HIGH && current == LOW &&
                    now - lastStartStopChange >= BUTTON_DEBOUNCE_MS);
    if (current != lastStartStop) lastStartStopChange = now;
    lastStartStop = current;
    return pressed;
}
