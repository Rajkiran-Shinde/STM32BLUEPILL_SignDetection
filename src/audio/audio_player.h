#pragma once
#include <Arduino.h>
#include "app_types.h"

bool audioPlayerInit();
bool audioPlayCommand(GestureCommand command);
void audioSetVolume(uint8_t volume);
void audioStop();
