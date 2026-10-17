#include <Arduino.h>
#include "indicators.h"
#include "config.h"

void indicatorsInit() {
    pinMode(STATUS_LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    statusLedOff();
    buzzerOff();
}

void statusLedOn() {
    digitalWrite(STATUS_LED_PIN, HIGH);
}

void statusLedOff() {
    digitalWrite(STATUS_LED_PIN, LOW);
}

void buzzerOn() {
    // Only use this direct GPIO output if the buzzer module/driver is compatible.
    digitalWrite(BUZZER_PIN, HIGH);
}

void buzzerOff() {
    digitalWrite(BUZZER_PIN, LOW);
}
