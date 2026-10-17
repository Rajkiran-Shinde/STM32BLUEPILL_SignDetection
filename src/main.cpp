
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "config.h"

Adafruit_ADS1115 ads;

const char* fingerNames[4] = {
    "Index",
    "Middle",
    "Ring",
    "Little"
};

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Initialize I2C on the project's configured pins.
    Wire.setSDA(I2C_SDA_PIN);
    Wire.setSCL(I2C_SCL_PIN);
    Wire.begin();

    Serial.println("\nFlex Sensor Test");
    Serial.println("----------------");

    if (!ads.begin(ADS1115_ADDRESS)) {
        Serial.println("ERROR: ADS1115 not detected!");
        while (true) {
            delay(1000);
        }
    }

    // GAIN_ONE: +/- 4.096 V ADC range.
    // Analog inputs must still remain within the supply rails.
    ads.setGain(GAIN_ONE);

    Serial.println("ADS1115 connected successfully.");
    Serial.println();
}

void loop() {
    for (uint8_t i = 0; i < 4; i++) {
        int16_t raw = ads.readADC_SingleEnded(i);

        // At GAIN_ONE, one ADC count = 0.125 mV.
        float voltage = raw * 0.000125f;

        Serial.print(fingerNames[i]);
        Serial.print(" (A");
        Serial.print(i);
        Serial.print("): raw = ");
        Serial.print(raw);
        Serial.print(", voltage = ");
        Serial.print(voltage, 3);
        Serial.println(" V");
    }

    Serial.println("----------------");
    delay(500);
}
