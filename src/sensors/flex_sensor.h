#pragma once
#include <Arduino.h>

bool flexSensorInit();
bool flexSensorReadVolts(float values[4]);
bool flexSensorCalibrate(float baseline[4]);
