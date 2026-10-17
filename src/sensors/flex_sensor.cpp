#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "flex_sensor.h"
#include "config.h"

namespace {
Adafruit_ADS1115 ads;
bool ready = false;
}

bool flexSensorInit() {
    Wire.begin();
    ready = ads.begin(ADS1115_ADDRESS);
    if (ready) {
        ads.setGain(GAIN_ONE);
    }
    return ready;
}

bool flexSensorReadVolts(float values[4]) {
    if (!ready) return false;

    // At GAIN_ONE, the ADS1115 LSB is 125 microvolts.
    for (uint8_t i = 0; i < 4; ++i) {
        int16_t raw = ads.readADC_SingleEnded(i);
        values[i] = static_cast<float>(raw) * 0.000125f;
    }
    return true;
}

bool flexSensorCalibrate(float baseline[4]) {
    if (!ready) return false;

    float sums[4] = {0, 0, 0, 0};
    float sample[4];

    for (uint16_t n = 0; n < CALIBRATION_SAMPLES; ++n) {
        if (!flexSensorReadVolts(sample)) return false;
        for (uint8_t i = 0; i < 4; ++i) sums[i] += sample[i];
        delay(10);
    }

    for (uint8_t i = 0; i < 4; ++i) {
        baseline[i] = sums[i] / CALIBRATION_SAMPLES;
    }
    return true;
}
