
#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <Arduino.h>
#include "app_types.h"

// Initialize the DFPlayer Mini
bool audioPlayerInit();

// Play the audio associated with a recognized gesture
bool audioPlayCommand(GestureCommand command);

// Set volume from 0 to 30
void audioSetVolume(uint8_t volume);

// Stop current playback
void audioStop();

#endif // AUDIO_PLAYER_H
