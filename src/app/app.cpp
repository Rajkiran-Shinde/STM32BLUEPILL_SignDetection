
#include <Arduino.h>

#include "app.h"
#include "indicators/indicators.h"
#include "audio/audio_player.h"

void appInit() {
    Serial.begin(115200);

    indicatorsInit();

    if (audioPlayerInit()) {
        Serial.println("Audio system ready");
    } else {
        Serial.println("Audio system unavailable");
    }
}

void appUpdate() {
    // Your sensor, gesture, button and display integration
    // will be added here.
}
