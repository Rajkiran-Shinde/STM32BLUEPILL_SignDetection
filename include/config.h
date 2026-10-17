#pragma once
#include <Arduino.h>

// I2C bus: STM32F103 I2C1 remapped/default pins used by this project
constexpr uint8_t I2C_SCL_PIN = PB6;
constexpr uint8_t I2C_SDA_PIN = PB7;
constexpr uint8_t ADS1115_ADDRESS = 0x48;
constexpr uint8_t OLED_ADDRESS = 0x3C;

// USART2: MCU TX -> DFPlayer RX; DFPlayer TX -> MCU RX
constexpr uint8_t DFPLAYER_RX_PIN = PA3; // MCU receives here
constexpr uint8_t DFPLAYER_TX_PIN = PA2; // MCU transmits here
constexpr uint32_t DFPLAYER_BAUD = 9600;

// External indicators and buttons
constexpr uint8_t STATUS_LED_PIN = PB12;
constexpr uint8_t BUZZER_PIN = PB13;
constexpr uint8_t MODE_BUTTON_PIN = PA0;
constexpr uint8_t START_STOP_BUTTON_PIN = PA1;

// Display: common 0.91-inch OLED; confirm actual module resolution
constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 32;

// Original ESP32 code's starting thresholds, in volts below baseline
constexpr float FLEX_TRIGGER_DROPS[4] = {0.040f, 0.050f, 0.045f, 0.035f};

constexpr uint16_t CALIBRATION_SAMPLES = 20;
constexpr uint16_t SENSOR_AVERAGE_SAMPLES = 10;
constexpr uint16_t BUTTON_DEBOUNCE_MS = 200;
constexpr uint16_t GESTURE_CONFIRM_DELAY_MS = 800;
constexpr uint32_t COMMAND_DISPLAY_MS = 5000;
constexpr uint32_t STOPPING_DISPLAY_MS = 4000;
constexpr uint32_t EMERGENCY_TIMEOUT_MS = 4000;
constexpr uint16_t EMERGENCY_BLINK_MS = 100;
