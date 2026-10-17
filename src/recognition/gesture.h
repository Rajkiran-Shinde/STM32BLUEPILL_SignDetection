#pragma once
#include "app_types.h"

void gestureInit();
GestureCommand gestureClassify(const float values[4], const float baseline[4]);
