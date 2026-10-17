#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"
#include "config.h"

namespace {
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
bool ready = false;
}

bool displayInit() {
    ready = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
    if (ready) {
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Sign-to-Speech");
        display.println("Initializing...");
        display.display();
    }
    return ready;
}

void displayMessage(const char* line1, const char* line2) {
    if (!ready) return;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(line1);
    display.println(line2);
    display.display();
}
